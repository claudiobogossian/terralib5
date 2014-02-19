/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
#include "../../../datatype.h"
#include "../../../maptools/GroupingAlgorithms.h"
#include "../../../maptools/Utils.h"
#include "../../../raster.h"
#include "../../../raster/RasterSummary.h"
#include "../../../raster/RasterSummaryManager.h"
#include "../../../se/ColorMap.h"
#include "../../../se/Categorize.h"
#include "../../../se/Enums.h"
#include "../../../se/Interpolate.h"
#include "../../../se/InterpolationPoint.h"
#include "../../../se/ParameterValue.h"
#include "../../widgets/colorbar/ColorBar.h"
#include "../../widgets/colorbar/ColorCatalogWidget.h"
#include "ColorMapWidget.h"
#include "ui_ColorMapWidgetForm.h"

// Qt
#include <QtGui/QColorDialog>
#include <QtGui/QPainter>
#include <QtGui/QValidator>

// STL
#include <cassert>


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

  updateUi();
}

te::se::ColorMap* te::qt::widgets::ColorMapWidget::getColorMap()
{
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

void te::qt::widgets::ColorMapWidget::updateUi()
{
  m_ui->m_tableWidget->setRowCount(0);

  if(!m_cm)
  {
    return;
  }

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

    for(size_t i = 1; i < tV.size() - 1; ++i)
    {
      QColor color;
      std::string lowerLimit = "";
      std::string upperLimit = "";

      if(i == 0)
      {
        lowerLimit = "...";
        upperLimit = te::map::GetString(t[i]);
        color.setNamedColor(te::map::GetString(tV[i]).c_str());
      }
      else if(i == tV.size() - 1)
      {
        lowerLimit = te::map::GetString(t[i - 1]);
        upperLimit = "...";
        color.setNamedColor(te::map::GetString(tV[i]).c_str());
      }
      else
      {
        lowerLimit = te::map::GetString(t[i - 1]);
        upperLimit = te::map::GetString(t[i]);
        color.setNamedColor(te::map::GetString(tV[i]).c_str());
      }

      //color
      QTableWidgetItem* item = new QTableWidgetItem();
      item->setBackgroundColor(color);
      item->setFlags(Qt::ItemIsEnabled);
      item->setData(Qt::UserRole, QVariant((int)i));
      m_ui->m_tableWidget->setItem(i - 1, 1, item);

      //value
      std::string range = lowerLimit + " - " + upperLimit;
      QTableWidgetItem* itemRange = new QTableWidgetItem();
      itemRange->setText(range.c_str());
      itemRange->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
      m_ui->m_tableWidget->setItem(i - 1, 0, itemRange);

      //label
      std::string rangeStr = lowerLimit + " to " + upperLimit;
      QTableWidgetItem* itemRangeStr = new QTableWidgetItem();
      itemRangeStr->setText(rangeStr.c_str());
      itemRangeStr->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
      m_ui->m_tableWidget->setItem(i - 1, 2, itemRangeStr);
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

    for(size_t i = 0; i < ip.size() - 1; ++i)
    {
      QColor color;
      QString valStrBegin;
      QString valStrEnd;

      te::se::InterpolationPoint* ipItem = ip[i];
            
      color.setNamedColor(te::map::GetString(ipItem->getValue()).c_str());

      valStrBegin.setNum(ipItem->getData());
      valStrEnd.setNum(ip[i+1]->getData());

      QString valStr;
      valStr.append(valStrBegin);
      valStr.append(" - ");
      valStr.append(valStrEnd);

    //color
      QTableWidgetItem* item = new QTableWidgetItem();
      item->setBackgroundColor(color);
      item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
      item->setData(Qt::UserRole, QVariant((int)i));
      m_ui->m_tableWidget->setItem(i, 1, item);

      //value
      QTableWidgetItem* itemRange = new QTableWidgetItem();
      itemRange->setText(valStr);
      itemRange->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
      m_ui->m_tableWidget->setItem(i, 0, itemRange);

      //label
      QTableWidgetItem* itemRangeStr = new QTableWidgetItem();
      itemRangeStr->setText(valStr);
      itemRangeStr->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
      m_ui->m_tableWidget->setItem(i, 2, itemRangeStr);
    }
  }

  m_ui->m_tableWidget->resizeColumnToContents(0);
  m_ui->m_tableWidget->resizeColumnToContents(1);
}

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

  te::map::GroupingByEqualSteps(vec.begin(), vec.end(), sliceValue, legVec, 1);

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

  te::map::GroupingByEqualSteps(vec.begin(), vec.end(), sliceValue, legVec, 1);


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

void te::qt::widgets::ColorMapWidget::buildRecodingMap()
{

}


void te::qt::widgets::ColorMapWidget::onApplyPushButtonClicked()
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

  emit applyPushButtonClicked();

  updateUi();
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
}

void te::qt::widgets::ColorMapWidget::onTableWidgetItemDoubleClicked(QTableWidgetItem* item)
{
  int curCol = m_ui->m_tableWidget->currentColumn();
  int curRow = m_ui->m_tableWidget->currentRow();

  if(curCol == 1)
  {
    int index = item->data(Qt::UserRole).toInt();

    QColor bgColor = item->backgroundColor();

    QColor newBgColor = QColorDialog::getColor(bgColor, m_ui->m_tableWidget);

    if(newBgColor.isValid())
      bgColor = newBgColor;

    te::se::ParameterValue* value = new te::se::ParameterValue(bgColor.name().toLatin1().data());

    int type = m_ui->m_transformComboBox->itemData(m_ui->m_transformComboBox->currentIndex()).toInt();

    if(type == te::se::CATEGORIZE_TRANSFORMATION)
    {
      // TODO
    }
    else if(type == te::se::INTERPOLATE_TRANSFORMATION)
    {
      std::vector<te::se::InterpolationPoint*> ip = m_cm->getInterpolate()->getInterpolationPoints();

      te::se::InterpolationPoint* ipItem = ip[index];

      ipItem->setValue(value);
    }

    item->setBackgroundColor(bgColor);

    emit applyPushButtonClicked();
  }
}