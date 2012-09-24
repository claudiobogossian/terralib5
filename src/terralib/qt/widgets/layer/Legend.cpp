/*  Copyright (C) 2001-2010 National Institute For Space Research (INPE) - Brazil.

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

// Qt
#include <QtGui>
#include <QComboBox>
#include <QMessageBox>

// TerraLib
#include "terralib/dataaccess/dataset/DataSet.h"
#include "terralib/dataaccess/dataset/DataSetType.h"
#include "terralib/dataaccess/datasource/DataSource.h"
#include "terralib/dataaccess/datasource/DataSourceFactory.h"
#include "terralib/dataaccess/datasource/DataSourceCatalogLoader.h"
#include "terralib/dataaccess/datasource/DataSourceTransactor.h"
#include "terralib/datatype/Property.h"
#include "terralib/maptools/Grouping.h"
#include "terralib/maptools/GroupingAlgorithms.h"
#include "terralib/maptools/Layer.h"
#include "LayerItem.h"
#include "Legend.h"


te::qt::widgets::Legend::Legend(te::qt::widgets::LayerItem* layerItem, QWidget* parent)
  : QDialog(parent), m_layerItem(layerItem), m_t(0), m_dataSetType(0)
{
  setupUi(this);

  // Set the grouping types available
  typeComboBox->addItem(tr("Equal Steps"));
  typeComboBox->addItem(tr("Quantil"));
  typeComboBox->addItem(tr("Standard Deviation"));
  typeComboBox->addItem(tr("Unique Value"));

  // Set the number of slices available
  for(size_t i = 1; i < 200; ++i)
    slicesComboBox->addItem(QString("%1").arg(i));

  // Set the precision values
  for(size_t i = 0; i < 15; ++i)
    precisionComboBox->addItem(QString("%1").arg(i));

  // Set the standard deviation values
  stdDeviationComboBox->addItem("1");
  stdDeviationComboBox->addItem("0.5");
  stdDeviationComboBox->addItem("0.25");

  // Set the attribute values
  te::da::DataSourceCatalogLoader* catalogLoader;

  te::map::Layer* refLayer = static_cast<te::map::Layer*>(m_layerItem->getRefLayer());

  m_t = refLayer->getDataSource()->getTransactor();
  catalogLoader = m_t->getCatalogLoader();
  m_dataSetType = catalogLoader->getDataSetType(refLayer->getDataSetName());

  size_t numAttributes = m_dataSetType->size();

  for (size_t i = 0; i < numAttributes; ++i)
    attributeComboBox->addItem(m_dataSetType->getProperty(i)->getName().c_str());

  // If the layer has already a legend associated to it, set the legend parameters
  if(refLayer->hasLegend())
  {
    te::map::Grouping* grouping = refLayer->getGrouping();

    // Get the grouping type of the layer legend
    const te::map::GroupingType type = grouping->getType();
    typeComboBox->setCurrentIndex(type);

    // Get the number of slices of the layer legend
    if(type == te::map::EQUAL_STEPS || type == te::map::QUANTIL)
      slicesComboBox->setCurrentIndex(grouping->getNumSlices() - 1);

    // Get the precision of the layer legend
    precisionComboBox->setCurrentIndex(grouping->getPrecision());

    // Get the standard deviation
    if(type == te::map::STD_DEVIATION)
    {
      const float stdDev = grouping->getStdDeviation();
      if(stdDev == 1)
        stdDeviationComboBox->setCurrentIndex(0);
      else if(stdDev == 0.5)
        stdDeviationComboBox->setCurrentIndex(1);
      else if(stdDev == 0.25)
        stdDeviationComboBox->setCurrentIndex(2);
    }

    // Get the layer attribute which the legend is associated to
    std::string propertyName = grouping->getPropertyName();

    for(size_t i = 0; i < numAttributes; ++i)
    {
      if(propertyName == attributeComboBox->itemText(i).toStdString())
      {
        attributeComboBox->setCurrentIndex(i);
        break;
      }
    }

    // Make a copy of the legend contents of the reference layer
    std::vector<te::map::LegendItem*>& layerLegend = *(refLayer->getLegend());
    m_legend.resize(layerLegend.size());
    for(size_t i = 0; i < layerLegend.size(); ++i)
      m_legend[i] = new te::map::LegendItem(*layerLegend[i]);

    setTableContents();
  }
  else
  {
    slicesComboBox->setCurrentIndex(4);
    precisionComboBox->setCurrentIndex(6);
  }


  // Connect signal/slots
  connect(typeComboBox, SIGNAL(activated(int)), this, SLOT(typeComboBoxActivated(int)));
  connect(applyPushButton, SIGNAL(clicked()), this, SLOT(applyPushButtonClicked()));
  connect(legendColorsPushButton, SIGNAL(clicked()), this, SLOT(legendColorsPushButtonClicked()));
  connect(legendTableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(legendTableItemChanged(QTableWidgetItem*)));
  connect(okPushButton, SIGNAL(clicked()), this, SLOT(okPushButtonClicked()));
  connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(cancelPushButtonClicked()));
  connect(helpPushButton, SIGNAL(clicked()), this, SLOT(helpPushButtonClicked()));

  typeComboBoxActivated(typeComboBox->currentIndex());

  okPushButton->setEnabled(false);
}

const std::vector<te::map::LegendItem*>& te::qt::widgets::Legend::getLegend() const
{
  return m_legend;
}

void te::qt::widgets::Legend::closeEvent(QCloseEvent* /*e*/)
{
  if(m_dataSetType)
    delete m_dataSetType;

  if(m_t)
    delete m_t;
}

void te::qt::widgets::Legend::typeComboBoxActivated(int index)
{
  slicesComboBox->setEnabled(false);
  stdDeviationComboBox->setEnabled(false);

  // Enable/Disable the grouping parameters according to the type selected
  if(index == 0 || index == 1)
    slicesComboBox->setEnabled(true);        // Equal Steps or Quantil
  else if(index == 2)
    stdDeviationComboBox->setEnabled(true);  // Standard Deviation
}

void te::qt::widgets::Legend::applyPushButtonClicked()
{
  setCursor(Qt::WaitCursor);

  m_changedItemLabel.clear();
  legendTableWidget->clear();

  // Delete the contents of the local legend and clear it
  for(size_t i = 0; i < m_legend.size(); ++i)
    delete m_legend[i];

  m_legend.clear();

  // Get the precision
  int precision = precisionComboBox->currentText().toInt();

  // Get the values of the attribute selected
  std::vector<std::string> attrValues;

  te::map::Layer* refLayer = static_cast<te::map::Layer*>(m_layerItem->getRefLayer());

  std::string sql = "SELECT " + attributeComboBox->currentText().toStdString();
  sql += " FROM " + refLayer->getDataSetName();

  te::da::DataSet* dataset = m_t->query(sql);

  size_t i = 0;
  while(dataset->moveNext())
    attrValues.push_back(dataset->getAsString(0, precision));

  delete dataset;

  // Get the groupings according to the grouping type selected
  std::vector<double> numValues;

  if(typeComboBox->currentIndex() == 3)
  {
    // Unique Value
    int dataType = m_dataSetType->getProperty(attributeComboBox->currentIndex())->getType();
    GroupingByUniqueValues(attrValues, dataType, m_legend, precision);
  }
  else
  {
    std::vector<double> numValues(attrValues.size());

    for(size_t i = 0; i < attrValues.size(); ++i)
      numValues[i] = atof(attrValues[i].c_str());

    if(typeComboBox->currentIndex() == 0)
      GroupingByEqualSteps(numValues.begin(), numValues.end(), slicesComboBox->currentText().toInt(),
                           m_legend, precision, true);    // Equal Steps
    else if(typeComboBox->currentIndex() == 1)
      GroupingByQuantil(numValues.begin(), numValues.end(), slicesComboBox->currentText().toInt(),
                        m_legend, precision, true);    // Quantil
    else if(typeComboBox->currentIndex() == 2)
    {
      // Standard Deviation
      std::string rmean;
      GroupingByStdDeviation(numValues.begin(), numValues.end(), stdDeviationComboBox->currentText().toDouble(),
                             m_legend, rmean, precisionComboBox->currentText().toInt(), true);
    }
  }

  setTableContents();

  okPushButton->setEnabled(true);

  unsetCursor();
}

void te::qt::widgets::Legend::legendColorsPushButtonClicked()
{
}

void te::qt::widgets::Legend::legendTableItemChanged(QTableWidgetItem* item)
{
  if((typeComboBox->currentIndex() == te::map::UNIQUE_VALUE && item->column() == 2) ||
     (typeComboBox->currentIndex() != te::map::UNIQUE_VALUE && item->column() == 3))
  {
    te::map::Layer* refLayer = static_cast<te::map::Layer*>(m_layerItem->getRefLayer());
    m_changedItemLabel[item->row()] = item->text().toStdString();
  }
}

void te::qt::widgets::Legend::okPushButtonClicked()
{
  // Save the grouping parameters in the layer
  te::map::GroupingType groupingType = (te::map::GroupingType)typeComboBox->currentIndex();

  te::map::Grouping* grouping = new te::map::Grouping(attributeComboBox->currentText().toStdString(),
                                                      groupingType, precisionComboBox->currentIndex());

  grouping->setNumSlices(slicesComboBox->currentIndex() + 1);
  
  grouping->setStdDeviation(stdDeviationComboBox->currentText().toFloat());

  te::map::Layer* refLayer = static_cast<te::map::Layer*>(m_layerItem->getRefLayer());

  refLayer->setGrouping(grouping);

  // Update the labels of the legend items that were changed
  std::map<int, std::string>::iterator it;
  for(it = m_changedItemLabel.begin(); it != m_changedItemLabel.end(); ++it)
    m_legend[it->first]->setTitle(it->second);
   
  delete m_t;

  accept();
}

void te::qt::widgets::Legend::cancelPushButtonClicked()
{
  if(m_dataSetType)
    delete m_dataSetType;

  if(m_t)
    delete m_t;

  reject();
}

void te::qt::widgets::Legend::helpPushButtonClicked()
{

}

void te::qt::widgets::Legend::setTableContents()
{
  // Set the number of rows of the legend table
  legendTableWidget->setRowCount(m_legend.size());

  // Set the number of columns and set the cell contents of the legend table
  QStringList headerLabels;
  if(typeComboBox->currentIndex() == te::map::UNIQUE_VALUE)
  {
    // Unique Value
    legendTableWidget->setColumnCount(4);
    headerLabels << tr("Color") << tr("Values") << tr("Label") << tr("Quantity");

    for(size_t i = 0; i < m_legend.size(); ++i)
    {
      legendTableWidget->setItem(i, 1, new QTableWidgetItem(m_legend[i]->getLowerLimit().c_str()));
      legendTableWidget->setItem(i, 2, new QTableWidgetItem(m_legend[i]->getLowerLimit().c_str()));
      legendTableWidget->setItem(i, 3, new QTableWidgetItem(QString("%1").arg(m_legend[i]->getCount())));
    }
  }
  else
  {
    legendTableWidget->setColumnCount(5);
    headerLabels << tr("Color") << tr("From") << tr("To") << tr("Label") << tr("Quantity");

    for(size_t i = 0; i < m_legend.size(); ++i)
    {
      legendTableWidget->setItem(i, 1, new QTableWidgetItem(m_legend[i]->getLowerLimit().c_str()));
      legendTableWidget->setItem(i, 2, new QTableWidgetItem(m_legend[i]->getUpperLimit().c_str()));
      legendTableWidget->setItem(i, 3, new QTableWidgetItem(m_legend[i]->getTitle().c_str()));
      legendTableWidget->setItem(i, 4, new QTableWidgetItem(QString("%1").arg(m_legend[i]->getCount())));
    }
  }

  legendTableWidget->setHorizontalHeaderLabels(headerLabels);
}
