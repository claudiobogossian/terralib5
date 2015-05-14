/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib/qt/widgets/se/ColorMapWidget.cpp

  \brief A dialog used to build a Color Map element.
*/

// TerraLib

#include "../../../color/ColorBar.h"
#include "../../../common/STLUtils.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../datatype.h"
#include "../../../maptools/DataSetLayer.h"
#include "../../../maptools/GroupingAlgorithms.h"
#include "../../../maptools/RasterLayer.h"
#include "../../../fe/Literal.h"
#include "../../../raster.h"
#include "../../../raster/RasterSummary.h"
#include "../../../raster/RasterSummaryManager.h"
#include "../../../se/ColorMap.h"
#include "../../../se/Categorize.h"
#include "../../../se/Enums.h"
#include "../../../se/Interpolate.h"
#include "../../../se/InterpolationPoint.h"
#include "../../../se/ParameterValue.h"
#include "../../../se/RasterSymbolizer.h"
#include "../../../se/Utils.h"
#include "../../widgets/colorbar/ColorBar.h"
#include "../../widgets/colorbar/ColorCatalogWidget.h"
#include "ColorMapWidget.h"
#include "ui_ColorMapWidgetForm.h"

// Qt
#include <QColorDialog>
#include <QMessageBox>
#include <QPainter>
#include <QValidator>

// STL
#include <cassert>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::widgets::ColorMapWidget::ColorMapWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::ColorMapWidgetForm),
    m_cm(0),
    m_cb(0),
    m_raster(0)
{
  m_ui->setupUi(this);

  QGridLayout* l = new QGridLayout(m_ui->m_colorBarWidget);
  l->setContentsMargins(0,0,0,0);
  m_colorBar = new  te::qt::widgets::ColorCatalogWidget(m_ui->m_colorBarWidget);
  l->addWidget(m_colorBar);

  m_ui->m_minValueLineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_maxValueLineEdit->setValidator(new QDoubleValidator(this));
  
  initialize();

  // Signals & slots
  connect(m_colorBar, SIGNAL(colorBarChanged()), this, SLOT(onApplyPushButtonClicked()));
  connect(m_ui->m_bandComboBox, SIGNAL(activated(QString)), this, SLOT(onBandSelected(QString)));
  connect(m_ui->m_applyPushButton, SIGNAL(clicked()), this, SLOT(onApplyPushButtonClicked()));
  connect(m_ui->m_tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemDoubleClicked(QTableWidgetItem*)));
  connect(m_ui->m_importPushButton, SIGNAL(clicked()), this, SLOT(onImportPushButtonClicked()));
}

te::qt::widgets::ColorMapWidget::~ColorMapWidget()
{
  delete m_cb;

  delete m_cm;
}

void te::qt::widgets::ColorMapWidget::setRaster(te::rst::Raster* r)
{
  assert(r);

  m_raster = r;

  int nBands = m_raster->getNumberOfBands();

  m_ui->m_bandComboBox->clear();

  for(int i = 0; i < nBands; ++i)
  {
    QString strBand;
    strBand.setNum(i);

    m_ui->m_bandComboBox->addItem(strBand);
  }

  if(nBands > 0)
    onBandSelected(m_ui->m_bandComboBox->itemText(0));
}

void te::qt::widgets::ColorMapWidget::setColorMap(te::se::ColorMap* cm) 
{
  assert(cm);

  m_cm = cm->clone();

  updateUi(true);
}

te::se::ColorMap* te::qt::widgets::ColorMapWidget::getColorMap()
{
  int index = m_ui->m_transformComboBox->currentIndex();

  int type = m_ui->m_transformComboBox->itemData(index).toInt();
  
  if(type == te::se::CATEGORIZE_TRANSFORMATION)
  {
    buildCategorizationMap();
  }
  else if(type == te::se::INTERPOLATE_TRANSFORMATION)
  {
    buildInterpolationMap();
  }
  else if(type == te::se::RECODE_TRANSFORMATION)
  {
    buildRecodingMap();
  }

  return m_cm->clone();
}

std::string te::qt::widgets::ColorMapWidget::getCurrentBand()
{
  if(m_ui->m_bandComboBox->count() != 0)
  {
    return m_ui->m_bandComboBox->currentText().toStdString();
  }

  return "";
}

void te::qt::widgets::ColorMapWidget::initialize()
{
  m_colorBar->getColorBar()->setHeight(20);
  m_colorBar->getColorBar()->setScaleVisible(false);

  m_ui->m_transformComboBox->clear();

  m_ui->m_transformComboBox->addItem(tr("Categorize"), te::se::CATEGORIZE_TRANSFORMATION);
  m_ui->m_transformComboBox->addItem(tr("Interpolate"), te::se::INTERPOLATE_TRANSFORMATION);
  //m_ui->m_transformComboBox->addItem(tr("Recode"), te::se::RECODE_TRANSFORMATION);
}

void te::qt::widgets::ColorMapWidget::updateUi(bool loadColorBar)
{
  m_ui->m_tableWidget->setRowCount(0);

  if(!m_cm)
  {
    return;
  }

  te::color::ColorBar* cb = 0;

  if(m_cm->getCategorize())
  {
    for(int i = 0; i < m_ui->m_transformComboBox->count(); ++i)
    {
      if(m_ui->m_transformComboBox->itemData(i).toInt() == te::se::CATEGORIZE_TRANSFORMATION)
        m_ui->m_transformComboBox->setCurrentIndex(i);
    }

    std::vector<te::se::ParameterValue*> t = m_cm->getCategorize()->getThresholds();
    std::vector<te::se::ParameterValue*> tV = m_cm->getCategorize()->getThresholdValues();

    m_ui->m_slicesSpinBox->setValue(tV.size() - 2);

    m_ui->m_tableWidget->setRowCount(tV.size() - 2);

    te::color::RGBAColor initColor(te::se::GetString(tV[1]).c_str());
    te::color::RGBAColor endColor(te::se::GetString(tV[tV.size() - 2]).c_str());

    if(loadColorBar)
      cb = new te::color::ColorBar(initColor, endColor, 256);

    int count = 0;

    for(size_t i = 1; i < tV.size() - 1; ++i)
    {
      QColor color;
      std::string lowerLimit = "";
      std::string upperLimit = "";

      if(i == 0)
      {
        lowerLimit = "...";
        upperLimit = te::se::GetString(t[i]);
        color.setNamedColor(te::se::GetString(tV[i]).c_str());
      }
      else if(i == tV.size() - 1)
      {
        lowerLimit = te::se::GetString(t[i - 1]);
        upperLimit = "...";
        color.setNamedColor(te::se::GetString(tV[i]).c_str());
      }
      else
      {
        lowerLimit = te::se::GetString(t[i - 1]);
        upperLimit = te::se::GetString(t[i]);
        color.setNamedColor(te::se::GetString(tV[i]).c_str());
      }

      if(loadColorBar)
      {
        if(count != 0 && count != tV.size() - 2)
        {
          double pos = (1. / (tV.size() - 2)) * count;

          te::color::RGBAColor color(te::se::GetString(tV[i]).c_str());

          cb->addColor(color, pos);
        }
      }
              
      ++count;

      //color
      QPixmap pix(24, 24);
      QPainter p(&pix);
      
      p.fillRect(0,0,24, 24, color);
      p.setBrush(Qt::transparent);
      p.setPen(Qt::black);
      p.drawRect(0, 0, 23, 23);

      QIcon icon(pix);

      QTableWidgetItem* item = new QTableWidgetItem(pix, "");
      item->setFlags(Qt::ItemIsEnabled);
      m_ui->m_tableWidget->setItem(i - 1, 0, item);

      //value From
      std::string rangeLower = lowerLimit;
      QTableWidgetItem* itemRangeLower = new QTableWidgetItem();
      itemRangeLower->setText(rangeLower.c_str());
      itemRangeLower->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);
      m_ui->m_tableWidget->setItem(i - 1, 1, itemRangeLower);

      //value To
      std::string rangeUpper = upperLimit;
      QTableWidgetItem* itemRangeUpper = new QTableWidgetItem();
      itemRangeUpper->setText(rangeUpper.c_str());
      itemRangeUpper->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);
      m_ui->m_tableWidget->setItem(i - 1, 2, itemRangeUpper);

      ////label
      //std::string rangeStr = lowerLimit + " to " + upperLimit;
      //QTableWidgetItem* itemRangeStr = new QTableWidgetItem();
      //itemRangeStr->setText(rangeStr.c_str());
      //itemRangeStr->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
      //m_ui->m_tableWidget->setItem(i - 1, 3, itemRangeStr);
    }
  }
  else if(m_cm->getInterpolate())
  {
    for(int i = 0; i < m_ui->m_transformComboBox->count(); ++i)
    {
      if(m_ui->m_transformComboBox->itemData(i).toInt() == te::se::INTERPOLATE_TRANSFORMATION)
        m_ui->m_transformComboBox->setCurrentIndex(i);
    }

    std::vector<te::se::InterpolationPoint*> ip = m_cm->getInterpolate()->getInterpolationPoints();

    m_ui->m_slicesSpinBox->setValue(ip.size() - 1);

    m_ui->m_tableWidget->setRowCount(ip.size() - 1);

    te::color::RGBAColor initColor(te::se::GetString(ip[0]->getValue()).c_str());
    te::color::RGBAColor endColor(te::se::GetString(ip[ip.size() - 1]->getValue()).c_str());

    if(loadColorBar)
      cb = new te::color::ColorBar(initColor, endColor, 256);

    int count = 0;

    for(size_t i = 0; i < ip.size() - 1; ++i)
    {
      QColor color1;
      QColor color2;
      QString valStrBegin;
      QString valStrEnd;

      te::se::InterpolationPoint* ipItem = ip[i];
      te::se::InterpolationPoint* ipItem2 = ip[i + 1];
            
      color1.setNamedColor(te::se::GetString(ipItem->getValue()).c_str());
      color2.setNamedColor(te::se::GetString(ipItem2->getValue()).c_str());

      valStrBegin.setNum(ipItem->getData());
      valStrEnd.setNum(ipItem2->getData());

      QString valStr;
      valStr.append(valStrBegin);
      valStr.append(" - ");
      valStr.append(valStrEnd);

      if(loadColorBar)
      {
        if(count != 0 && count != ip.size() - 1)
        {
          double pos = (1. / (ip.size() - 1)) * count;

          te::color::RGBAColor color(te::se::GetString(ipItem->getValue()).c_str());

          cb->addColor(color, pos);
        }
      }

      ++count;

    //color
      QPixmap pix(24, 24);
      QPainter p(&pix);
      
      p.fillRect(0,0,12, 24, color1);
      p.fillRect(12,0,12, 24, color2);
      p.setBrush(Qt::transparent);
      p.setPen(Qt::black);
      p.drawRect(0, 0, 23, 23);

      QIcon icon(pix);

      QTableWidgetItem* item = new QTableWidgetItem(icon, "");
      //item->setBackgroundColor(color);
      item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
      m_ui->m_tableWidget->setItem(i, 0, item);

      //value lower
      QTableWidgetItem* itemRangeLower = new QTableWidgetItem();
      itemRangeLower->setText(valStrBegin);
      itemRangeLower->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);
      m_ui->m_tableWidget->setItem(i, 1, itemRangeLower);

      //value upper
      QTableWidgetItem* itemRangeUpper = new QTableWidgetItem();
      itemRangeUpper->setText(valStrEnd);
      itemRangeUpper->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);
      m_ui->m_tableWidget->setItem(i, 2, itemRangeUpper);

      ////label
      //QTableWidgetItem* itemRangeStr = new QTableWidgetItem();
      //itemRangeStr->setText(valStr);
      //itemRangeStr->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);
      //m_ui->m_tableWidget->setItem(i, 3, itemRangeStr);
    }
  }

  if(cb)
  {
    disconnect(m_colorBar, SIGNAL(colorBarChanged()), this, SLOT(onApplyPushButtonClicked()));

    te::qt::widgets::colorbar::ColorBar* cbW = m_colorBar->getColorBar();
    cbW->setColorBar(cb);

    connect(m_colorBar, SIGNAL(colorBarChanged()), this, SLOT(onApplyPushButtonClicked()));
  }

  m_ui->m_tableWidget->resizeColumnsToContents();
#if (QT_VERSION >= 0x050000)
  m_ui->m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
  m_ui->m_tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#endif
}

void te::qt::widgets::ColorMapWidget::buildCategorizationMap()
{
  te::se::Categorize* c = new te::se::Categorize();

  c->setFallbackValue("#000000");
  c->setLookupValue(new te::se::ParameterValue("Rasterdata"));

  QColor cWhite(Qt::white);
  std::string colorWhiteStr = cWhite.name().toLatin1().data();

  //added dummy color for values < than min values...
  c->addValue(new te::se::ParameterValue(colorWhiteStr));

  for(int i = 0; i < m_ui->m_tableWidget->rowCount(); ++i)
  {
    QColor color = QColor::fromRgb(m_ui->m_tableWidget->item(i, 0)->icon().pixmap(24, 24).toImage().pixel(0,0));

    std::string rangeStr = m_ui->m_tableWidget->item(i, 1)->text().toStdString();
    std::string colorStr = color.name().toStdString();

    c->addThreshold(new te::se::ParameterValue(rangeStr));
    c->addValue(new te::se::ParameterValue(colorStr));

    if(i == m_ui->m_tableWidget->rowCount() - 1)
    {
      rangeStr = m_ui->m_tableWidget->item(i, 2)->text().toStdString();
      c->addThreshold(new te::se::ParameterValue(rangeStr));
    }
  }

  //added dummy color for values > than max values...
  c->addValue(new te::se::ParameterValue(colorWhiteStr));

  c->setThresholdsBelongTo(te::se::Categorize::SUCCEEDING);

  if(m_cm)
  {
    m_cm->setCategorize(c);
    m_cm->setInterpolate(0);
  }
}
/*
void te::qt::widgets::ColorMapWidget::buildCategorizationMap()
{
  delete m_cb;

  m_cb = m_colorBar->getColorBar()->getColorBar();

  int sliceValue = m_ui->m_slicesSpinBox->value();

  std::vector<te::color::RGBAColor> colorVec = m_cb->getSlices(sliceValue);

  std::vector<te::map::GroupingItem*> legVec;

  std::vector<double> vec;
  vec.push_back(m_ui->m_minValueLineEdit->text().toDouble());
  vec.push_back(m_ui->m_maxValueLineEdit->text().toDouble());

  te::map::GroupingByEqualSteps(vec.begin(), vec.end(), sliceValue, legVec, m_ui->m_precisionSpinBox->value());

  te::se::Categorize* c = new te::se::Categorize();

  c->setFallbackValue("#000000");
  c->setLookupValue(new te::se::ParameterValue("Rasterdata"));

  QColor cWhite(Qt::white);
  std::string colorWhiteStr = cWhite.name().toLatin1().data();

  //added dummy color for values < than min values...
  c->addValue(new te::se::ParameterValue(colorWhiteStr));

  for(size_t i = 0; i < colorVec.size(); ++i)
  {
    QColor color(colorVec[i].getRed(), colorVec[i].getGreen(), colorVec[i].getBlue(), colorVec[i].getAlpha());

    std::string rangeStr = legVec[i]->getLowerLimit();
    std::string colorStr = color.name().toStdString();

    c->addThreshold(new te::se::ParameterValue(rangeStr));
    c->addValue(new te::se::ParameterValue(colorStr));

    if(i == colorVec.size() - 1)
    {
      rangeStr = legVec[i]->getUpperLimit();
      c->addThreshold(new te::se::ParameterValue(rangeStr));
    }
  }

  //added dummy color for values > than max values...
  c->addValue(new te::se::ParameterValue(colorWhiteStr));

  c->setThresholdsBelongTo(te::se::Categorize::SUCCEEDING);

  if(m_cm)
  {
    m_cm->setCategorize(c);
    m_cm->setInterpolate(0);
  }
}
*/

void te::qt::widgets::ColorMapWidget::buildInterpolationMap()
{
  te::se::Interpolate* interpolate = new te::se::Interpolate();

  interpolate->setFallbackValue("#000000");
  interpolate->setLookupValue(new te::se::ParameterValue("Rasterdata"));
  interpolate->setMethodType(te::se::Interpolate::COLOR);

  for(int i = 0; i < m_ui->m_tableWidget->rowCount(); ++i)
  {
    QColor color = QColor::fromRgb(m_ui->m_tableWidget->item(i, 0)->icon().pixmap(24, 24).toImage().pixel(0,0));

    if(i == m_ui->m_tableWidget->rowCount() - 1)
    {
      {
        QString rangeStr = m_ui->m_tableWidget->item(i, 1)->text();
        std::string colorStr = color.name().toLatin1().data();

        te::se::InterpolationPoint* ip = new te::se::InterpolationPoint();

        ip->setData(rangeStr.toDouble());
        ip->setValue(new te::se::ParameterValue(colorStr));

        interpolate->add(ip);
      }

      {
        color = QColor::fromRgb(m_ui->m_tableWidget->item(i, 0)->icon().pixmap(24, 24).toImage().pixel(23,0));

        QString rangeStr = m_ui->m_tableWidget->item(i, 2)->text();
        std::string colorStr = color.name().toLatin1().data();

        te::se::InterpolationPoint* ip = new te::se::InterpolationPoint();

        ip->setData(rangeStr.toDouble());
        ip->setValue(new te::se::ParameterValue(colorStr));

        interpolate->add(ip);
      }

    }
    else
    {
      QString rangeStr = m_ui->m_tableWidget->item(i, 1)->text();
      std::string colorStr = color.name().toLatin1().data();

      te::se::InterpolationPoint* ip = new te::se::InterpolationPoint();

      ip->setData(rangeStr.toDouble());
      ip->setValue(new te::se::ParameterValue(colorStr));

      interpolate->add(ip);
    }
  }

  if(m_cm)
  {
    m_cm->setInterpolate(interpolate);
    m_cm->setCategorize(0);
  }
}

/*
void te::qt::widgets::ColorMapWidget::buildInterpolationMap()
{
  delete m_cb;

  m_cb = m_colorBar->getColorBar()->getColorBar();

  int sliceValue = m_ui->m_slicesSpinBox->value();

  std::vector<te::color::RGBAColor> colorVec = m_cb->getSlices(sliceValue + 1);
  
  std::vector<te::map::GroupingItem*> legVec;

  std::vector<double> vec;
  vec.push_back(m_ui->m_minValueLineEdit->text().toDouble());
  vec.push_back(m_ui->m_maxValueLineEdit->text().toDouble());

  te::map::GroupingByEqualSteps(vec.begin(), vec.end(), sliceValue, legVec, m_ui->m_precisionSpinBox->value());


  te::se::Interpolate* interpolate = new te::se::Interpolate();

  interpolate->setFallbackValue("#000000");
  interpolate->setLookupValue(new te::se::ParameterValue("Rasterdata"));
  interpolate->setMethodType(te::se::Interpolate::COLOR);


  for(size_t i = 0; i < colorVec.size(); ++i)
  {
    QColor color(colorVec[i].getRed(), colorVec[i].getGreen(), colorVec[i].getBlue(), colorVec[i].getAlpha());

    if(i == colorVec.size() - 1)
    {
      QString rangeStr = legVec[i - 1]->getUpperLimit().c_str();
      std::string colorStr = color.name().toLatin1().data();

      te::se::InterpolationPoint* ip = new te::se::InterpolationPoint();

      ip->setData(rangeStr.toDouble());
      ip->setValue(new te::se::ParameterValue(colorStr));

      interpolate->add(ip);
    }
    else
    {
      QString rangeStr = legVec[i]->getLowerLimit().c_str();
      std::string colorStr = color.name().toLatin1().data();

      te::se::InterpolationPoint* ip = new te::se::InterpolationPoint();

      ip->setData(rangeStr.toDouble());
      ip->setValue(new te::se::ParameterValue(colorStr));

      interpolate->add(ip);
    }
  }

  if(m_cm)
  {
    m_cm->setInterpolate(interpolate);
    m_cm->setCategorize(0);
  }
}
*/

void te::qt::widgets::ColorMapWidget::buildRecodingMap()
{

}


void te::qt::widgets::ColorMapWidget::onApplyPushButtonClicked()
{
  delete m_cb;

  m_cb = m_colorBar->getColorBar()->getColorBar();

  int sliceValue = m_ui->m_slicesSpinBox->value();

  std::vector<te::color::RGBAColor> colorVec;

  int index = m_ui->m_transformComboBox->currentIndex();

  int type = m_ui->m_transformComboBox->itemData(index).toInt();
  
  if(type == te::se::CATEGORIZE_TRANSFORMATION)
  {
    colorVec = m_cb->getSlices(sliceValue);
  }
  else if(type == te::se::INTERPOLATE_TRANSFORMATION)
  {
    colorVec = m_cb->getSlices(sliceValue + 1);
  }

  std::vector<te::map::GroupingItem*> legVec;

  std::vector<double> vec;
  vec.push_back(m_ui->m_minValueLineEdit->text().toDouble());
  vec.push_back(m_ui->m_maxValueLineEdit->text().toDouble());

  te::map::GroupingByEqualSteps(vec.begin(), vec.end(), sliceValue, legVec, m_ui->m_precisionSpinBox->value());

  m_ui->m_tableWidget->setRowCount(legVec.size());

  for(std::size_t t = 0; t < legVec.size(); ++t)
  {
    if(type == te::se::CATEGORIZE_TRANSFORMATION)
    {
      //color
      QColor color(colorVec[t].getRed(), colorVec[t].getGreen(), colorVec[t].getBlue(), colorVec[t].getAlpha());

      QPixmap pix(24, 24);
      QPainter p(&pix);
      
      p.fillRect(0,0,24, 24, color);
      p.setBrush(Qt::transparent);
      p.setPen(Qt::black);
      p.drawRect(0, 0, 23, 23);

      QIcon icon(pix);

      QTableWidgetItem* item = new QTableWidgetItem(icon, "");
      item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
      m_ui->m_tableWidget->setItem(t, 0, item);
    }
    else if(type == te::se::INTERPOLATE_TRANSFORMATION)
    {
      QColor color1(colorVec[t].getRed(), colorVec[t].getGreen(), colorVec[t].getBlue(), colorVec[t].getAlpha());
      QColor color2(colorVec[t + 1].getRed(), colorVec[t + 1].getGreen(), colorVec[t + 1].getBlue(), colorVec[t + 1].getAlpha());

      QPixmap pix(24, 24);
      QPainter p(&pix);
      
      p.fillRect(0,0,12, 24, color1);
      p.fillRect(12,0,12, 24, color2);

      p.setBrush(Qt::transparent);
      p.setPen(Qt::black);
      p.drawRect(0, 0, 23, 23);

      QIcon icon(pix);

      QTableWidgetItem* item = new QTableWidgetItem(icon, "");
      item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
      m_ui->m_tableWidget->setItem(t, 0, item);
    }

    //value lower
    std::string rangeStrLower = legVec[t]->getLowerLimit();

    QTableWidgetItem* itemRangeLower = new QTableWidgetItem();
    itemRangeLower->setText(rangeStrLower.c_str());
    itemRangeLower->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);
    m_ui->m_tableWidget->setItem(t, 1, itemRangeLower);

    //value upper
    std::string rangeStrUpper = legVec[t]->getUpperLimit();

    QTableWidgetItem* itemRangeUpper = new QTableWidgetItem();
    itemRangeUpper->setText(rangeStrUpper.c_str());
    itemRangeUpper->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);
    m_ui->m_tableWidget->setItem(t, 2, itemRangeUpper);

    ////label
    //std::string valStr = rangeStrLower + " - " + rangeStrUpper;

    //QTableWidgetItem* itemRangeStr = new QTableWidgetItem();
    //itemRangeStr->setText(valStr.c_str());
    //itemRangeStr->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    //m_ui->m_tableWidget->setItem(t, 3, itemRangeStr);
  }

  m_ui->m_tableWidget->resizeColumnsToContents();
#if (QT_VERSION >= 0x050000)
  m_ui->m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
  m_ui->m_tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#endif

  emit applyPushButtonClicked();
}

void te::qt::widgets::ColorMapWidget::onBandSelected(QString value)
{
  if(value.isEmpty())
  {
    return;
  }

  const te::rst::RasterSummary* rsMin = te::rst::RasterSummaryManager::getInstance().get(m_raster, te::rst::SUMMARY_MIN);
  const te::rst::RasterSummary* rsMax = te::rst::RasterSummaryManager::getInstance().get(m_raster, te::rst::SUMMARY_MAX);
  const std::complex<double>* cmin = rsMin->at(0).m_minVal;
  const std::complex<double>* cmax = rsMax->at(0).m_maxVal;
  double min = cmin->real();
  double max = cmax->real();

  QString strMin;
  strMin.setNum(min);
  m_ui->m_minValueLineEdit->setText(strMin);

  QString strMax;
  strMax.setNum(max);
  m_ui->m_maxValueLineEdit->setText(strMax);

  emit applyPushButtonClicked();
}

void te::qt::widgets::ColorMapWidget::onTableWidgetItemDoubleClicked(QTableWidgetItem* item)
{
  int curCol = m_ui->m_tableWidget->currentColumn();

  int index = m_ui->m_transformComboBox->currentIndex();

  int type = m_ui->m_transformComboBox->itemData(index).toInt();

  if(curCol == 0)
  {
    if(type == te::se::CATEGORIZE_TRANSFORMATION)
    {
      QColor bgColor = QColor::fromRgb(item->icon().pixmap(24, 24).toImage().pixel(0,0));

      QColor newBgColor = QColorDialog::getColor(bgColor, m_ui->m_tableWidget);

      if(newBgColor.isValid())
        bgColor = newBgColor;
    
      QPixmap pix(24, 24);
      pix.fill(bgColor);
      QIcon icon(pix);

      item->setIcon(icon);
    }
    else if(type == te::se::INTERPOLATE_TRANSFORMATION)
    {
      QMessageBox::information(this, tr("Classification"), 
        tr("Set the colors for the min and max values of this range. Also necessary to change the colors equivalents at another level to maintain consistency."));

      QColor color1 = QColor::fromRgb(item->icon().pixmap(24, 24).toImage().pixel(0,0));

      QColor newBgColor = QColorDialog::getColor(color1, m_ui->m_tableWidget);

      if(newBgColor.isValid())
        color1 = newBgColor;

      QColor color2 = QColor::fromRgb(item->icon().pixmap(24, 24).toImage().pixel(23,0));

      newBgColor = QColorDialog::getColor(color2, m_ui->m_tableWidget);

      if(newBgColor.isValid())
        color2 = newBgColor;

      QPixmap pix(24, 24);
      QPainter p(&pix);
      
      p.fillRect(0,0,12, 24, color1);
      p.fillRect(12,0,12, 24, color2);

      QIcon icon(pix);

      item->setIcon(icon);
    }
  }

  emit applyPushButtonClicked();
}

te::se::ColorMap* getLayerColorMap(te::map::AbstractLayerPtr layer)
{
  te::se::RasterSymbolizer* symb = 0;

  if(layer->getType() == "DATASETLAYER")
  {
    te::map::DataSetLayer* l = dynamic_cast<te::map::DataSetLayer*>(layer.get());

    if(l)
    {
      symb = te::se::GetRasterSymbolizer(l->getStyle());
    }
  }
  else if(layer->getType() == "RASTERLAYER")
  {
    te::map::RasterLayer* l = dynamic_cast<te::map::RasterLayer*>(layer.get());

    if(l)
    {
      symb = te::se::GetRasterSymbolizer(l->getStyle());
    }
  }

  if(symb)
  {
    if(symb->getColorMap())
    {
      return symb->getColorMap();
    }
  }

  return 0;
}

te::rst::Raster* getLayerRaster(te::map::AbstractLayerPtr layer)
{
  if(layer->getType() == "DATASETLAYER")
  {
    te::map::DataSetLayer* l = dynamic_cast<te::map::DataSetLayer*>(layer.get());

    if(l)
    {
      std::auto_ptr<te::da::DataSet> ds = layer->getData();

      if(ds.get())
      {
        std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
        return ds->getRaster(rpos).release();
      }
    }
  }
  else if(layer->getType() == "RASTERLAYER")
  {
    te::map::RasterLayer* l = dynamic_cast<te::map::RasterLayer*>(layer.get());

    if(l)
    {
      return l->getRaster();
    }
  }

  return 0;
}

void te::qt::widgets::ColorMapWidget::setLayers(te::map::AbstractLayerPtr selectedLayer, std::vector<te::map::AbstractLayerPtr> allLayers)
{
  for(std::size_t i = 0; i < allLayers.size(); ++i)
  {
    std::auto_ptr<te::da::DataSetType> dt(allLayers[i]->getSchema());

    if(dt->hasRaster() && getLayerColorMap(allLayers[i]) && allLayers[i]->getId() != selectedLayer->getId())
    {
      m_ui->m_layersComboBox->addItem(allLayers[i]->getTitle().c_str(), QVariant::fromValue(allLayers[i]));
    }
  }
}

void te::qt::widgets::ColorMapWidget::onImportPushButtonClicked()
{
  if(m_ui->m_layersComboBox->currentText() == "")
  {
    QMessageBox::warning(this, tr("Grouping"), tr("There are no other layers with Grouping!"));
    return;
  }
  
  QVariant varLayer = m_ui->m_layersComboBox->itemData(m_ui->m_layersComboBox->currentIndex(), Qt::UserRole);
  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  te::se::ColorMap* cm = getLayerColorMap(layer);

  setRaster(getLayerRaster(layer));
  setColorMap(cm);

  updateUi(true);

  emit applyPushButtonClicked();
}