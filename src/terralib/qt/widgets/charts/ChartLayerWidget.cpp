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
  \file terralib/qt/widgets/charts/ChartLayerWidget.cpp

  \brief A widget used to build a grouping.
*/

// TerraLib
#include "../../../color/RGBAColor.h"
#include "../../../common/STLUtils.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../dataaccess/query/OrderByItem.h"
#include "../../../maptools/Chart.h"
#include "../../../maptools/Enums.h"
#include "../../../maptools/QueryLayer.h"
#include "../utils/ColorPickerToolButton.h"
#include "../Utils.h"
#include "ChartLayerWidget.h"
#include "ui_ChartLayerWidgetForm.h"

// Qt
#include <QString>
#include <QColor>
#include <QMessageBox>
#include <QPainterPath>

// STL
#include <memory>

//Boost
#include <boost/lexical_cast.hpp>


te::qt::widgets::ChartLayerWidget::ChartLayerWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::ChartLayerWidgetForm)
{
  m_ui->setupUi(this);

// Adjusting the color picker
  QGridLayout* layout = new QGridLayout(m_ui->m_contourColorWidget);
  layout->setContentsMargins(0, 0, 0, 0);

//Color Picker
  m_colorPicker = new te::qt::widgets::ColorPickerToolButton(m_ui->m_contourColorWidget);
  m_colorPicker->setFixedSize(107, 24);
  layout->addWidget(m_colorPicker);

  //connects
  connect(m_ui->m_addToolButton, SIGNAL(clicked()), this, SLOT(onAddToolButtonClicked()));
  connect(m_ui->m_removeToolButton, SIGNAL(clicked()), this, SLOT(onRemoveToolButtonClicked()));
  connect(m_ui->m_contourWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateUi()));
  connect(m_ui->m_tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(onItemClicked(int, int)));
  connect(m_colorPicker, SIGNAL(colorChanged(const QColor&)), this, SLOT(updateUi()));

  initialize();
}

te::qt::widgets::ChartLayerWidget::~ChartLayerWidget()
{
  m_chartVec.clear();
}

void te::qt::widgets::ChartLayerWidget::setLayer(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  listAttributes();

  m_ui->m_summaryComboBox->clear();
  if(te::da::HasLinkedTable(layer->getSchema().get()))
  {

    m_ui->m_summaryComboBox->addItem("MIN");
    m_ui->m_summaryComboBox->addItem("MAX");
    m_ui->m_summaryComboBox->addItem("SUM");
    m_ui->m_summaryComboBox->addItem("AVERAGE");
    m_ui->m_summaryComboBox->addItem("MEDIAN");
    m_ui->m_summaryComboBox->addItem("STDDEV");
    m_ui->m_summaryComboBox->addItem("VARIANCE");

    if(m_layer->getChart())
    {
      int index = m_ui->m_summaryComboBox->findText(QString::fromStdString(m_layer->getChart()->getSummary()));
      m_ui->m_summaryComboBox->setCurrentIndex(index);
    }

    m_ui->m_summaryComboBox->setEnabled(true);
    m_ui->m_summaryComboBox->show();
    m_ui->m_summaryLabel->show();
  }
  else
  {
    m_ui->m_summaryComboBox->addItem("NONE");
    m_ui->m_summaryComboBox->setEnabled(false);
    m_ui->m_summaryComboBox->hide();
    m_ui->m_summaryLabel->hide();
  }
}

bool te::qt::widgets::ChartLayerWidget::buildChart()
{
  if(m_chartVec.empty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("No attribute selected."));
    return false;
  }

  //chart type
  int index = m_ui->m_typeComboBox->currentIndex();
  int chartType = m_ui->m_typeComboBox->itemData(index).toInt();

  //get attributes
  std::vector<std::string> attrs;
  std::vector<te::color::RGBAColor> colors;

  for(std::size_t t = 0; t < m_chartVec.size(); ++t)
  {
    attrs.push_back(m_chartVec[t].first);
    te::color::RGBAColor c = te::qt::widgets::Convert2TerraLib(m_chartVec[t].second);
    colors.push_back(c);
  }

  te::map::Chart* chart = new te::map::Chart((te::map::ChartType)chartType, attrs, colors);
  chart->setHeight((std::size_t)m_ui->m_sizeSpinBox->value());
  chart->setContourWidth((std::size_t)m_ui->m_contourWidthSpinBox->value());
  chart->setContourColor(te::qt::widgets::Convert2TerraLib(m_colorPicker->getColor()));
  chart->setAvoidConflicts(m_ui->m_avoidConflictsCheckBox->isChecked());
  chart->setSummary(m_ui->m_summaryComboBox->currentText().toStdString());

   // Is necessary compute the max value?
  if(chart->getType() == te::map::Bar)
  {
    double maxValue = getMaxValue(chart);
    chart->setMaxValue(maxValue);
  }

  m_layer->setChart(chart);

  return true;
}

void te::qt::widgets::ChartLayerWidget::setChart(te::map::Chart* chart)
{
  m_chartVec.clear();

  int type = (int)chart->getType();

  for(int i = 0; i < m_ui->m_typeComboBox->count(); ++i)
  {
    int t = m_ui->m_typeComboBox->itemData(i).toInt();

    if(t == type)
    {
      m_ui->m_typeComboBox->setCurrentIndex(i);
      break;
    }
  }

  m_ui->m_sizeSpinBox->setValue((int)chart->getHeight());
  m_ui->m_contourWidthSpinBox->setValue((int)chart->getContourWidth());
  m_colorPicker->setColor(te::qt::widgets::Convert2Qt(chart->getContourColor()));
  m_ui->m_avoidConflictsCheckBox->setChecked(chart->getAvoidConflicts());

  for(std::size_t t = 0; t < chart->getProperties().size(); ++t)
  {
    std::string value = chart->getProperties()[t];
    QColor c = te::qt::widgets::Convert2Qt(chart->getColor(t));
    std::pair<std::string, QColor> pair(value, c);
    m_chartVec.push_back(pair);
  }

  updateUi();
}

void te::qt::widgets::ChartLayerWidget::initialize()
{
  //add chart types
  m_ui->m_typeComboBox->clear();
  m_ui->m_typeComboBox->addItem(tr("Pie Chart"), QVariant(te::map::Pie));
  m_ui->m_typeComboBox->addItem(tr("Bar Chart"), QVariant(te::map::Bar));


  //set icons
  m_ui->m_addToolButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_removeToolButton->setIcon(QIcon::fromTheme("list-remove"));
}

void te::qt::widgets::ChartLayerWidget::updateUi()
{
  m_ui->m_tableWidget->setRowCount(0);

  for(std::size_t t = 0; t < m_chartVec.size(); ++t)
  {
    int newrow = m_ui->m_tableWidget->rowCount();
    m_ui->m_tableWidget->insertRow(newrow);

    QIcon icon(te::qt::widgets::CreatePixmapIcon(28, m_colorPicker->getColor(), m_chartVec[t].second, m_ui->m_contourWidthSpinBox->value()));

    QTableWidgetItem* item = new QTableWidgetItem(icon, "");
    item->setFlags(Qt::ItemIsEnabled);
    m_ui->m_tableWidget->setItem(newrow, 0, item);

    //attr name
    QTableWidgetItem* itemAttr = new QTableWidgetItem(m_chartVec[t].first.c_str());
    itemAttr->setFlags(Qt::ItemIsEnabled);
    m_ui->m_tableWidget->setItem(newrow, 1, itemAttr);
  }
}

void te::qt::widgets::ChartLayerWidget::onAddToolButtonClicked()
{
  std::string value = m_ui->m_attrComboBox->currentText().toStdString();

  bool found = false;

  for(std::size_t t = 0; t < m_chartVec.size(); ++t)
  {
    if(m_chartVec[t].first == value)
    {
      found = true;
      break;
    }
  }

  if(found)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Attribute already selected."));
    return;
  }

  QColor c(rand() % 255,rand() % 255,rand() % 255);

  std::pair<std::string, QColor> pair(value, c);

  m_chartVec.push_back(pair);

  updateUi();
}

void te::qt::widgets::ChartLayerWidget::onRemoveToolButtonClicked()
{
  if(m_ui->m_tableWidget->currentRow() == -1)
    return;

  m_chartVec.erase(m_chartVec.begin() + m_ui->m_tableWidget->currentRow());

  updateUi();
}

void te::qt::widgets::ChartLayerWidget::onItemClicked(int row, int column)
{
  if(column != 0)
    return;

  QColor curColor = m_chartVec[row].second;

  QColor c = QColorDialog::getColor(curColor, this);

  if(c.isValid())
  {
    curColor = c;
  }

  m_chartVec[row].second = curColor;

  updateUi();
}

void te::qt::widgets::ChartLayerWidget::listAttributes()
{
  m_ui->m_attrComboBox->clear();

  std::auto_ptr<te::map::LayerSchema> dsType(m_layer->getSchema());

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

double te::qt::widgets::ChartLayerWidget::getMaxValue(te::map::Chart* chart)
{
  double maxValue = std::numeric_limits<double>::min();

  // Gets the chart properties
  const std::vector<std::string>& properties = chart->getProperties();
  bool hasChartNullValue = false;

  std::auto_ptr<te::da::DataSetType> schema = m_layer->getSchema();
  if(te::da::HasLinkedTable(schema.get()) == false)
  {
    // Gets the dataset
    std::auto_ptr<te::da::DataSet> dataset(m_layer->getData());
    std::vector<size_t> chartPropPos;
    size_t csize = properties.size();
    for(std::size_t i = 0; i < csize; ++i)
      chartPropPos.push_back(te::da::GetPropertyPos(dataset.get(), properties[i]));
    chart->setPropertiesPos(chartPropPos);

    while(dataset->moveNext())
    {
      for(std::size_t i = 0; i < csize; ++i)
      {
        if(dataset->isNull(chart->getPropertiesPos()[i]))
        {
          hasChartNullValue = true;
          break;
        }
      }

      if(hasChartNullValue == false)
      {
        for(std::size_t i = 0; i < csize; ++i)
        {
          double value = te::da::GetValueAsDouble(dataset.get(), chart->getPropertiesPos()[i]);
          maxValue = std::max(maxValue, value); 
        }
      }
    }
    return maxValue;
  }

  te::da::PrimaryKey* pk = schema->getPrimaryKey();
  std::vector<te::dt::Property*> props = pk->getProperties();
  std::vector<std::string> boid;
  size_t pksize = 0;
  while(++pksize < props.size())
  {
    boid.push_back(props[pksize-1]->getName());
    if(props[pksize-1]->getDatasetName() != props[pksize]->getDatasetName())
      break;
  }

  // make sorting by object id
  te::map::QueryLayer* qlayer = dynamic_cast<te::map::QueryLayer*>(m_layer.get());
  te::da::Select* select = dynamic_cast<te::da::Select*>(qlayer->getQuery()->clone());
  te::da::Select* selectaux = dynamic_cast<te::da::Select*>(select->clone());
  te::da::OrderBy* orderBy = new te::da::OrderBy;

  for(size_t i = 0; i < pksize; ++i)
    orderBy->push_back(new te::da::OrderByItem(boid[i]));

  selectaux->setOrderBy(orderBy);
  qlayer->setQuery(selectaux);
  std::auto_ptr<te::da::DataSet> dataset(qlayer->getData());
  qlayer->setQuery(select);

  std::vector<std::string> pkdata(pksize), pkdataaux(pksize);
  std::string cfunction = chart->getSummary();
  std::map<std::string, std::vector<double> > chartValues;
  std::map<std::string, double> chartValue;
  std::vector<size_t> chartPropPos;
  bool hasChartNullValueAux = false;
  size_t csize = properties.size();
  for(std::size_t i = 0; i < csize; ++i)
  {
    std::vector<double> v;
    chartValues[properties[i]] = v;
    chartPropPos.push_back(te::da::GetPropertyPos(dataset.get(), properties[i]));
  }
  chart->setPropertiesPos(chartPropPos);

  dataset->moveFirst();
  do
  {
    // calculate summarized value
    size_t i;
    for(i = 0; i < pksize; ++i)
      pkdata[i] = dataset->getAsString(boid[i]);
      
    for(i = 0; i < pksize; ++i)
    {
      if(dataset->isAtBegin())
        pkdataaux[i] = dataset->getAsString(boid[i]);
      else
      {
        if(pkdata[i] != pkdataaux[i])
        {
          pkdataaux = pkdata;
          break;
        }
      }      
    }
    if(i == pksize) // it is the same object
    {
      if(hasChartNullValue == false)
      {
        // read value chart value
        for(std::size_t i = 0; i < csize; ++i)
        {
          if(dataset->isNull(chart->getProperties()[i]) == false)
            chartValues[properties[i]].push_back(te::da::GetValueAsDouble(dataset.get(), chart->getPropertiesPos()[i]));
            //chartValues[properties[i]].push_back(boost::lexical_cast<double>(dataset->getAsString(properties[i])));
          else
          {
            hasChartNullValue = true;
            break;
          }
        }
      }
      // read other values
      continue;
    }
    else // it is other object
    {
      // sumarize chart value according to the required summarization 
      if(hasChartNullValue == false)
      {
        for(std::size_t i = 0; i < csize; ++i)
          chartValue[properties[i]] = te::da::GetSummarizedValue(chartValues[properties[i]], cfunction);
      }

      // prepare the next loop
      for(std::size_t i = 0; i < csize; ++i)
        chartValues[properties[i]].clear();

      hasChartNullValueAux = false;
      for(std::size_t i = 0; i < csize; ++i)
      {
        if(dataset->isNull(properties[i]))
        {
          hasChartNullValueAux = true;
          break;
        }
      }
      if(hasChartNullValueAux == false)
      {
        for(std::size_t i = 0; i < csize; ++i)
          chartValues[properties[i]].push_back(boost::lexical_cast<double>(dataset->getAsString(properties[i])));
      }
    }

    if(hasChartNullValue == false)
    {
      // get max value
      std::map<std::string, double>::iterator it = chartValue.begin();
      while(it != chartValue.end())
      {
        maxValue = std::max(maxValue, it->second); 
        ++it;
      }
    }

    hasChartNullValue = hasChartNullValueAux;
    hasChartNullValueAux = false;
  } while(dataset->moveNext());

  if(hasChartNullValue == false)
  {
    for(std::size_t i = 0; i < csize; ++i)
      chartValue[properties[i]] = te::da::GetSummarizedValue(chartValues[properties[i]], cfunction);

    // get max value
    std::map<std::string, double>::iterator it = chartValue.begin();
    while(it != chartValue.end())
    {
      maxValue = std::max(maxValue, it->second); 
      ++it;
    }
  }
  return maxValue;
}
