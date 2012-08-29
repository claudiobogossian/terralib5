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
#include "../../qwt/ColorBar.h"
#include "../../../se/ColorMap.h"
#include "../../../se/Categorize.h"
#include "../../../se/Interpolate.h"
#include "../../../se/InterpolationPoint.h"
#include "../../../se/ParameterValue.h"
#include "ColorMapWidget.h"
#include "../../../datatype.h"
#include "../../../maptools/GroupingAlgorithms.h"
#include "../../../raster.h"
#include "../../../raster/RasterSummary.h"
#include "../../../raster/RasterSummaryManager.h"
#include "../../../common/STLUtils.h"
#include "../../../maptools/Utils.h"
#include "ui_ColorMapWidgetForm.h"

// Qt
#include <QtGui/QPainter>
#include <QtGui/QValidator>

// STL
#include <cassert>


te::qt::widgets::ColorMapWidget::ColorMapWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::ColorMapWidgetForm),
    m_cm(0),
    m_raster(0)
{
  m_ui->setupUi(this);

  QGridLayout* l = new QGridLayout(m_ui->m_colorBarWidget);
  l->setContentsMargins(0,0,0,0);
  m_cbWidget = new te::qt::qwt::ColorBar(m_ui->m_colorBarWidget);
  l->addWidget(m_cbWidget);

  m_ui->m_minValueLineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_maxValueLineEdit->setValidator(new QDoubleValidator(this));
  
  initialize();

  // Signals & slots
  connect(m_ui->m_applyPushButton, SIGNAL(clicked()), this, SLOT(onApplyPushButtonClicked()));
  connect(m_ui->m_bandComboBox, SIGNAL(activated(QString)), this, SLOT(onBandSelected(QString)));
}

te::qt::widgets::ColorMapWidget::~ColorMapWidget()
{
  delete m_cb;
}

void te::qt::widgets::ColorMapWidget::setRaster(te::rst::Raster* r)
{
  assert(r);

  m_raster = r;

  int nBands = m_raster->getNumberOfBands();

  m_ui->m_bandComboBox->clear();
  m_ui->m_bandComboBox->addItem("");

  for(int i = 0; i < nBands; ++i)
  {
    QString strBand;
    strBand.setNum(i);

    m_ui->m_bandComboBox->addItem(strBand);
  }
}

void te::qt::widgets::ColorMapWidget::setColorMap(te::se::ColorMap* cm) 
{
  assert(cm);

  m_cm = cm;

  updateUi();
}

te::se::ColorMap* te::qt::widgets::ColorMapWidget::getColorMap()
{
  return m_cm;
}

void te::qt::widgets::ColorMapWidget::initialize()
{
  m_cb = new te::color::ColorBar(te::color::RGBAColor(255, 0, 0, TE_OPAQUE), te::color::RGBAColor(255, 255, 255, TE_OPAQUE), 256);

  m_cbWidget->setHeight(20);
  m_cbWidget->setColorBar(m_cb);
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
    std::vector<te::se::ParameterValue*> t = m_cm->getCategorize()->getThresholds();
    std::vector<te::se::ParameterValue*> tV = m_cm->getCategorize()->getThresholdValues();

    m_ui->m_tableWidget->setRowCount(tV.size());

    for(size_t i = 0; i < tV.size(); ++i)
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
      item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
      m_ui->m_tableWidget->setItem(i, 1, item);

      //value
      std::string range = lowerLimit + " - " + upperLimit;
      QTableWidgetItem* itemRange = new QTableWidgetItem();
      itemRange->setText(range.c_str());
      itemRange->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
      m_ui->m_tableWidget->setItem(i, 0, itemRange);

      //label
      std::string rangeStr = lowerLimit + " to " + upperLimit;
      QTableWidgetItem* itemRangeStr = new QTableWidgetItem();
      itemRangeStr->setText(rangeStr.c_str());
      itemRangeStr->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
      m_ui->m_tableWidget->setItem(i, 2, itemRangeStr);
    }
  }
  else if(m_cm->getInterpolate())
  {
    std::vector<te::se::InterpolationPoint*> ip = m_cm->getInterpolate()->getInterpolationPoints();

    m_ui->m_tableWidget->setRowCount(ip.size());

    for(size_t i = 0; i < ip.size(); ++i)
    {
      QColor color;
      QString valStr;

      te::se::InterpolationPoint* ipItem = ip[i];
            
      color.setNamedColor(te::map::GetString(ipItem->getValue()).c_str());

      valStr.setNum(ipItem->getData());


    //color
      QTableWidgetItem* item = new QTableWidgetItem();
      item->setBackgroundColor(color);
      item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
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
  int sliceValue = m_ui->m_slicesSpinBox->value();

  std::vector<te::color::RGBAColor> colorVec = m_cb->getSlices(sliceValue);

  std::vector<te::map::LegendItem*> legVec;

  std::vector<double> vec;
  vec.push_back(m_ui->m_minValueLineEdit->text().toDouble());
  vec.push_back(m_ui->m_maxValueLineEdit->text().toDouble());

  te::map::GroupingByEqualSteps(vec.begin(), vec.end(), sliceValue, legVec, 6);


  te::se::Categorize* c = new te::se::Categorize();

  c->setLookupValue(new te::se::ParameterValue("Rasterdata"));

  QColor cWhite(Qt::white);
  std::string colorWhiteStr = cWhite.name().toLatin1().data();

  //added dummy color for values < than min values...
  c->addValue(new te::se::ParameterValue(colorWhiteStr));

  for(size_t i = 0; i < colorVec.size(); ++i)
  {
    QColor color(colorVec[i].getRed(), colorVec[i].getGreen(), colorVec[i].getBlue(), colorVec[i].getAlpha());

    std::string rangeStr = legVec[i]->getLowerLimit();
    std::string colorStr = color.name().toLatin1().data();

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

  if(m_cm)
  {
    m_cm->setCategorize(c);
    m_cm->setInterpolate(0);
  }
}

void te::qt::widgets::ColorMapWidget::buildInterpolationMap()
{
  int sliceValue = m_ui->m_slicesSpinBox->value();

  std::vector<te::color::RGBAColor> colorVec = m_cb->getSlices(sliceValue + 1);
  
  std::vector<te::map::LegendItem*> legVec;

  std::vector<double> vec;
  vec.push_back(m_ui->m_minValueLineEdit->text().toDouble());
  vec.push_back(m_ui->m_maxValueLineEdit->text().toDouble());

  te::map::GroupingByEqualSteps(vec.begin(), vec.end(), sliceValue, legVec, 6);


  te::se::Interpolate* interpolate = new te::se::Interpolate();

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
  
  //using index position from a combo box... must better this

  if(index == 0)  // Categorization
  {
    buildCategorizationMap();
  }
  else if(index == 1) // Interpolation
  {
    buildInterpolationMap();
  }
  else if(index == 2) // Recoding
  {
    buildRecodingMap();
  }

  updateUi();
}

void te::qt::widgets::ColorMapWidget::onBandSelected(QString value)
{
  if(value.isEmpty())
  {
    return;
  }

  int band = value.toInt();

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


/*
std::vector<te::color::RGBAColor> colorVec;

  std::vector<te::map::LegendItem*> legVec;

  std::vector<double> vec;
  vec.push_back(m_ui->m_minValueLineEdit->text().toDouble());
  vec.push_back(m_ui->m_maxValueLineEdit->text().toDouble());


  //extract color bar from color map and fill inteface
  int sliceValue = m_ui->m_slicesSpinBox->value();

  //using index position from a combo box... must better this
  int index = m_ui->m_transformComboBox->currentIndex();

  if(index == 0)  // Categorization
  {
    colorVec = m_cb->getSlices(sliceValue);

    te::map::GroupingByEqualSteps(vec.begin(), vec.end(), sliceValue, legVec, 6);
  }
  else if(index == 1) // Interpolation
  {
    colorVec = m_cb->getSlices(sliceValue + 1);

    te::map::GroupingByEqualSteps(vec.begin(), vec.end(), sliceValue, legVec, 6);
  }
  else if(index == 2) // Recoding
  {
     m_ui->m_tableWidget->setRowCount(0);
    return;
  }

  m_ui->m_tableWidget->setRowCount(colorVec.size());

  for(size_t i = 0; i < colorVec.size(); ++i)
  {
    //color
    QTableWidgetItem* item = new QTableWidgetItem();

    item->setBackgroundColor(QColor(colorVec[i].getRed(), colorVec[i].getGreen(), colorVec[i].getBlue(), colorVec[i].getAlpha()));
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    m_ui->m_tableWidget->setItem(i, 1, item);

    //value
    std::string range = "";

    if(index == 0)  // Categorization
    {
      range = legVec[i]->getLowerLimit() + " - " + legVec[i]->getUpperLimit();
    }
    else if(index == 1) // Interpolation
    {
      if(i == colorVec.size() - 1)
      {
        range = legVec[i - 1]->getUpperLimit();
      }
      else
      {
        range = legVec[i]->getLowerLimit();
      }
    }
    
    QTableWidgetItem* itemRange = new QTableWidgetItem();
    itemRange->setText(range.c_str());
    itemRange->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    m_ui->m_tableWidget->setItem(i, 0, itemRange);

    //label
    std::string rangeStr = "";

    if(index == 0)  // Categorization
    {
      rangeStr = legVec[i]->getLowerLimit() + " to " + legVec[i]->getUpperLimit();
    }
    else if(index == 1) // Interpolation
    {
      if(i == colorVec.size() - 1)
      {
        range = legVec[i - 1]->getUpperLimit();
      }
      else
      {
        rangeStr = legVec[i]->getLowerLimit();
      }
    }

    QTableWidgetItem* itemRangeStr = new QTableWidgetItem();
    itemRangeStr->setText(rangeStr.c_str());
    itemRangeStr->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    m_ui->m_tableWidget->setItem(i, 2, itemRangeStr);
  }

  m_ui->m_tableWidget->resizeColumnToContents(0);
  m_ui->m_tableWidget->resizeColumnToContents(1);
  */