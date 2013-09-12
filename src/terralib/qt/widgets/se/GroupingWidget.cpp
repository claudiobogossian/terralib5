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
  \file terralib/qt/widgets/se/GroupingWidget.cpp

  \brief A widget used to build a grouping.
*/

// TerraLib
#include "../../../color/ColorBar.h"
#include "../../../common/STLUtils.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../maptools/Enums.h"
#include "../../../maptools/Grouping.h"
#include "../../../maptools/GroupingAlgorithms.h"
#include "../../../maptools/GroupingItem.h"
#include "../../../se/Utils.h"
#include "../colorbar/ColorBar.h"
#include "../se/SymbologyPreview.h"
#include "GroupingWidget.h"
#include "ui_GroupingWidgetForm.h"

// STL
#include <cassert>

#define MAX_SLICES 200
#define PRECISION 15

te::qt::widgets::GroupingWidget::GroupingWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::GroupingWidgetForm)
{
  m_ui->setupUi(this);

  QGridLayout* l = new QGridLayout(m_ui->m_colorBarWidget);
  l->setContentsMargins(0,0,0,0);
  m_colorBar = new  te::qt::widgets::colorbar::ColorBar(m_ui->m_colorBarWidget);
  l->addWidget(m_colorBar);

//connects
  connect(m_colorBar, SIGNAL(colorBarChanged()), this, SLOT(onColorBarChanged()));
  connect(m_ui->m_typeComboBox, SIGNAL(activated(int)), this, SLOT(onTypeComboBoxActivated(int)));
  connect(m_ui->m_attrComboBox, SIGNAL(activated(int)), this, SLOT(onAttrComboBoxActivated(int)));

  initialize();
}

te::qt::widgets::GroupingWidget::~GroupingWidget()
{
  te::common::FreeContents(m_legend);
  m_legend.clear();
}

void te::qt::widgets::GroupingWidget::setLayer(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  //set data type
  setDataSetType();

  //set grouping
  setGrouping();
}

std::auto_ptr<te::map::Grouping> te::qt::widgets::GroupingWidget::getGrouping()
{
  std::string attr = m_ui->m_attrComboBox->currentText().toStdString();
  int attrIdx =  m_ui->m_attrComboBox->currentIndex();
  int attrType = m_ui->m_attrComboBox->itemData(attrIdx).toInt();

  int index = m_ui->m_typeComboBox->currentIndex();
  int type = m_ui->m_typeComboBox->itemData(index).toInt();

  std::auto_ptr<te::map::Grouping> group(new te::map::Grouping(attr, (te::map::GroupingType)type));

  group->setPropertyType(attrType);

  group->setNumSlices(m_ui->m_slicesSpinBox->value());

  group->setPrecision(m_ui->m_precSpinBox->value());

  group->setStdDeviation(m_ui->m_stdDevDoubleSpinBox->value());

  std::vector<te::map::GroupingItem*> groupingItems;
  for(std::size_t t = 0; t < m_legend.size(); ++t)
  {
    te::map::GroupingItem* gi = new te::map::GroupingItem(*m_legend[t]);

    groupingItems.push_back(gi);
  }
  group->setGroupingItems(groupingItems);

  return group;
}

void te::qt::widgets::GroupingWidget::initialize()
{
  // create color bar
  m_cb = new te::color::ColorBar(te::color::RGBAColor(255, 0, 0, TE_OPAQUE), te::color::RGBAColor(0, 0, 0, TE_OPAQUE), 256);

  m_colorBar->setHeight(20);
  m_colorBar->setColorBar(m_cb);
  m_colorBar->setScaleVisible(false);

  // fill grouping type combo box
  m_ui->m_typeComboBox->addItem(tr("Equal Steps"), te::map::EQUAL_STEPS);
  m_ui->m_typeComboBox->addItem(tr("Quantil"), te::map::QUANTIL);
  m_ui->m_typeComboBox->addItem(tr("Standard Deviation"), te::map::STD_DEVIATION);
  m_ui->m_typeComboBox->addItem(tr("Unique Value"), te::map::UNIQUE_VALUE);

  onTypeComboBoxActivated(0);

  //set number of slices
  m_ui->m_slicesSpinBox->setMinimum(1);
  m_ui->m_slicesSpinBox->setMaximum(MAX_SLICES);
  m_ui->m_slicesSpinBox->setValue(5);
  m_ui->m_slicesSpinBox->setSingleStep(1);

  //set standard deviation values
  m_ui->m_stdDevDoubleSpinBox->setMinimum(0.25);
  m_ui->m_stdDevDoubleSpinBox->setMaximum(1.0);
  m_ui->m_stdDevDoubleSpinBox->setValue(0.5);
  m_ui->m_stdDevDoubleSpinBox->setSingleStep(0.25);

  //set number of precision
  m_ui->m_precSpinBox->setMinimum(1);
  m_ui->m_precSpinBox->setMaximum(PRECISION);
  m_ui->m_precSpinBox->setValue(6);
  m_ui->m_precSpinBox->setSingleStep(1);

  //adjust table
  m_ui->m_tableWidget->resizeColumnsToContents();
  m_ui->m_tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}

void te::qt::widgets::GroupingWidget::updateUi()
{
  m_ui->m_tableWidget->setRowCount(0);

  for(std::size_t t = 0; t < m_legend.size(); ++t)
  {
    te::map::GroupingItem* gi = m_legend[t];

    int newrow = m_ui->m_tableWidget->rowCount();
    m_ui->m_tableWidget->insertRow(newrow);

    //symbol
    {
      const std::vector<te::se::Symbolizer*>& ss = gi->getSymbolizers();
      QPixmap pix = te::qt::widgets::SymbologyPreview::build(ss, QSize(24, 24));
      QIcon icon(pix);
      QTableWidgetItem* item = new QTableWidgetItem(icon, "");
      item->setFlags(Qt::ItemIsEnabled);
      m_ui->m_tableWidget->setItem(newrow, 0, item);
    }

    //title
    {
      QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(gi->getTitle()));
      item->setFlags(Qt::ItemIsEnabled);
      m_ui->m_tableWidget->setItem(newrow, 1, item);
    }

    //Min
    {
      QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(gi->getLowerLimit()));
      item->setFlags(Qt::ItemIsEnabled);
      m_ui->m_tableWidget->setItem(newrow, 2, item);
    }

    //Max
    {
      QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(gi->getUpperLimit()));
      item->setFlags(Qt::ItemIsEnabled);
      m_ui->m_tableWidget->setItem(newrow, 3, item);
    }

    //Count
    {
      QTableWidgetItem* item = new QTableWidgetItem(QString::number(gi->getCount()));
      item->setFlags(Qt::ItemIsEnabled);
      m_ui->m_tableWidget->setItem(newrow, 4, item);
    }
  }

  m_ui->m_tableWidget->resizeColumnsToContents();
  m_ui->m_tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}

void te::qt::widgets::GroupingWidget::setDataSetType()
{
  listAttributes();
}

void te::qt::widgets::GroupingWidget::setGrouping()
{
  te::map::Grouping* grouping = m_layer->getGrouping();

  if(!grouping)
    return;

  //type
  te::map::GroupingType type = grouping->getType();
  
  for(int i = 0; i < m_ui->m_typeComboBox->count(); ++i)
  {
    if(type == m_ui->m_typeComboBox->itemData(i).toInt())
    {
      m_ui->m_typeComboBox->setCurrentIndex(i);
      break;
    }
  }

  //attr name
  std::string attrName = grouping->getPropertyName();

  for(int i = 0; i < m_ui->m_attrComboBox->count(); ++i)
  {
    if(attrName == m_ui->m_attrComboBox->itemText(i).toStdString())
    {
      m_ui->m_attrComboBox->setCurrentIndex(i);
      break;
    }
  }

  //precision
  size_t prec = grouping->getPrecision();

  m_ui->m_precSpinBox->setValue((int)prec);

  //slices
  size_t slices = grouping->getNumSlices();

  m_ui->m_slicesSpinBox->setValue((int)slices);

  //std dev
  float stdDev = grouping->getStdDeviation();

  m_ui->m_stdDevDoubleSpinBox->setValue((double)stdDev);

  //grouping items
  te::common::FreeContents(m_legend);
  m_legend.clear();

  for(size_t t = 0; t < grouping->getGroupingItems().size(); ++t)
  {
    te::map::GroupingItem* gi = new te::map::GroupingItem(*grouping->getGroupingItems()[t]);

    m_legend.push_back(gi);
  }

  updateUi();
}

void te::qt::widgets::GroupingWidget::onApplyPushButtonClicked()
{
  int index = m_ui->m_typeComboBox->currentIndex();

  int type = m_ui->m_typeComboBox->itemData(index).toInt();
  int slices = m_ui->m_slicesSpinBox->value();
  int prec = m_ui->m_precSpinBox->value();
  double stdDev = m_ui->m_stdDevDoubleSpinBox->value();

  std::string attr = m_ui->m_attrComboBox->currentText().toStdString();
  int attrIdx =  m_ui->m_attrComboBox->currentIndex();
  int attrType = m_ui->m_attrComboBox->itemData(attrIdx).toInt();

  te::common::FreeContents(m_legend);
  m_legend.clear();

  if(type == te::map::EQUAL_STEPS)
  {
    std::vector<double> vec;

    getDataAsDouble(vec, attr, attrType);

    te::map::GroupingByEqualSteps(vec.begin(), vec.end(), slices, m_legend, prec);
  }
  else if(type == te::map::QUANTIL) 
  {
    std::vector<double> vec;

    getDataAsDouble(vec, attr, attrType);

    te::map::GroupingByQuantil(vec.begin(), vec.end(), slices, m_legend, prec);
  }
  else if(type == te::map::STD_DEVIATION) 
  {
    std::vector<double> vec;
    std::string mean = "0";

    getDataAsDouble(vec, attr, attrType);

    te::map::GroupingByStdDeviation(vec.begin(), vec.end(), stdDev, m_legend, mean, prec);
  }
  else if(type == te::map::UNIQUE_VALUE) 
  {
    std::vector<std::string> vec;

    getDataAsString(vec, attr);

    te::map::GroupingByUniqueValues(vec, attrType, m_legend, prec);
  }

  buildSymbolizer();

  updateUi();
}

void te::qt::widgets::GroupingWidget::onTypeComboBoxActivated(int idx)
{
  int type = m_ui->m_typeComboBox->itemData(idx).toInt();

  if(type == te::map::EQUAL_STEPS)
  {
    m_ui->m_slicesSpinBox->setEnabled(true);
    m_ui->m_precSpinBox->setEnabled(true);
    m_ui->m_stdDevDoubleSpinBox->setEnabled(false);
  }
  else if(type == te::map::QUANTIL) 
  {
    m_ui->m_slicesSpinBox->setEnabled(true);
    m_ui->m_precSpinBox->setEnabled(true);
    m_ui->m_stdDevDoubleSpinBox->setEnabled(false);
  }
  else if(type == te::map::STD_DEVIATION) 
  {
    m_ui->m_slicesSpinBox->setEnabled(false);
    m_ui->m_precSpinBox->setEnabled(true);
    m_ui->m_stdDevDoubleSpinBox->setEnabled(true);
  }
  else if(type == te::map::UNIQUE_VALUE) 
  {
    m_ui->m_slicesSpinBox->setEnabled(false);
    m_ui->m_precSpinBox->setEnabled(true);
    m_ui->m_stdDevDoubleSpinBox->setEnabled(false);
  }

  if(m_layer.get())
    listAttributes();
}

void te::qt::widgets::GroupingWidget::onAttrComboBoxActivated(int idx)
{
  int attrType = m_ui->m_attrComboBox->itemData(idx).toInt();
}

void te::qt::widgets::GroupingWidget::onColorBarChanged()
{
  if(m_layer.get())
  {
    buildSymbolizer();

    updateUi();
  }
}

void te::qt::widgets::GroupingWidget::getDataAsDouble(std::vector<double>& vec, const std::string& attrName, const int& dataType)
{
  assert(m_layer.get());

  std::auto_ptr<te::map::LayerSchema> dsType(m_layer->getSchema());

  std::size_t idx;

  for(std::size_t t = 0; t < dsType->getProperties().size(); ++t)
  {
    if(dsType->getProperty(t)->getName() == attrName)
    {
      idx = t;
      break;
    }
  }

  std::auto_ptr<te::da::DataSet> ds(m_layer->getData());

  ds->moveBeforeFirst();

  while(ds->moveNext())
  {
    if(ds->isNull(idx))
      continue;

    if(dataType == te::dt::INT16_TYPE)
      vec.push_back((double)ds->getInt16(idx));
    else if(dataType == te::dt::INT32_TYPE)
      vec.push_back((double)ds->getInt32(idx));
    else if(dataType == te::dt::INT64_TYPE)
      vec.push_back((double)ds->getInt64(idx));
    else if(dataType == te::dt::FLOAT_TYPE)
      vec.push_back((double)ds->getFloat(idx));
    else if(dataType == te::dt::DOUBLE_TYPE)
      vec.push_back(ds->getDouble(idx));
    else if(dataType == te::dt::NUMERIC_TYPE)
    {
      QString strNum = ds->getNumeric(idx).c_str();

      bool ok = false;

      double value = strNum.toDouble(&ok);

      if(ok)
        vec.push_back(value);
    }
  }
}

void te::qt::widgets::GroupingWidget::getDataAsString(std::vector<std::string>& vec, const std::string& attrName)
{
  assert(m_layer.get());

  std::auto_ptr<te::map::LayerSchema> dsType(m_layer->getSchema());

  std::size_t idx;

  for(std::size_t t = 0; t < dsType->getProperties().size(); ++t)
  {
    if(dsType->getProperty(t)->getName() == attrName)
    {
      idx = t;
      break;
    }
  }

  std::auto_ptr<te::da::DataSet> ds(m_layer->getData());

  ds->moveBeforeFirst();

  while(ds->moveNext())
  {
    if(!ds->isNull(idx))
      vec.push_back(ds->getAsString(idx));
  }
}

int te::qt::widgets::GroupingWidget::getGeometryType()
{
  assert(m_layer.get());

  std::auto_ptr<te::map::LayerSchema> dsType(m_layer->getSchema());

  te::gm::GeometryProperty* geometryProperty = te::da::GetFirstGeomProperty(dsType.get());

  return geometryProperty->getGeometryType();
}

void te::qt::widgets::GroupingWidget::buildSymbolizer()
{
  int legendSize = m_legend.size();

  std::vector<te::color::RGBAColor> colorVec = m_cb->getSlices(legendSize);

  int geomType = getGeometryType();

  for(size_t t = 0; t < colorVec.size(); ++t)
  {
    std::vector<te::se::Symbolizer*> symbVec;

    te::se::Symbolizer* s = te::se::CreateSymbolizer((te::gm::GeomType)geomType, colorVec[t].getColor());

    symbVec.push_back(s);

    m_legend[t]->setSymbolizers(symbVec);
  }
}

void te::qt::widgets::GroupingWidget::listAttributes()
{
  m_ui->m_attrComboBox->clear();

  std::auto_ptr<te::map::LayerSchema> dsType(m_layer->getSchema());

  //grouping type
  int index = m_ui->m_typeComboBox->currentIndex();
  int type = m_ui->m_typeComboBox->itemData(index).toInt();

  if(type == te::map::EQUAL_STEPS ||type == te::map::QUANTIL || type == te::map::STD_DEVIATION)
  {
    for(size_t t = 0; t < dsType->getProperties().size(); ++t)
    {
      te::dt::Property* p = dsType->getProperty(t);

      int dataType = p->getType();

      switch(dataType)
      {
        case te::dt::INT16_TYPE:
        case te::dt::INT32_TYPE:
        case te::dt::INT64_TYPE:
        case te::dt::FLOAT_TYPE:
        case te::dt::DOUBLE_TYPE:
        case te::dt::NUMERIC_TYPE:
          m_ui->m_attrComboBox->addItem(p->getName().c_str(), p->getType());

        default:
          continue;
      }
    }
  }
  else if(type == te::map::UNIQUE_VALUE) 
  {
    for(size_t t = 0; t < dsType->getProperties().size(); ++t)
    {
      te::dt::Property* p = dsType->getProperty(t);

      int dataType = p->getType();

      switch(dataType)
      {
        case te::dt::INT16_TYPE:
        case te::dt::INT32_TYPE:
        case te::dt::INT64_TYPE:
        case te::dt::FLOAT_TYPE:
        case te::dt::DOUBLE_TYPE:
        case te::dt::NUMERIC_TYPE:
        case te::dt::STRING_TYPE:
          m_ui->m_attrComboBox->addItem(p->getName().c_str(), p->getType());

        default:
          continue;
      }
    }
  }
}
