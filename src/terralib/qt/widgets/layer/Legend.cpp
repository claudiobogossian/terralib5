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
#include "ScopedCursor.h"


te::qt::widgets::Legend::Legend(te::qt::widgets::LayerItem* layerItem, QWidget* parent)
  : QDialog(parent), m_layerItem(layerItem), m_t(0), m_dataSetType(0),
    m_colorBar(0), m_numValuesNotGrouped(0)
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

  // Set the opacity values
  for(size_t i = 0; i <= 100; ++i)
    opacityComboBox->addItem(QString("%1").arg(i));

  // Set the attribute values
  te::da::DataSourceCatalogLoader* catalogLoader;

  te::map::Layer* refLayer = static_cast<te::map::Layer*>(m_layerItem->getRefLayer());

  m_t = refLayer->getDataSource()->getTransactor();
  catalogLoader = m_t->getCatalogLoader();
  m_dataSetType = catalogLoader->getDataSetType(refLayer->getId());

  // If the layer has already a legend associated to it, set the legend parameters
  if(refLayer->hasLegend())
  {
    te::map::Grouping* grouping = refLayer->getGrouping();

    // Get the grouping type of the layer legend
    const te::map::GroupingType type = grouping->getType();
    typeComboBox->setCurrentIndex(type);

    // Set the possible layer attributes according to the grouping type and set the 
    // layer attribute which the legend is associated to as the current one.
    typeComboBoxActivated(typeComboBox->currentIndex());

    std::string propertyName = grouping->getPropertyName();

    size_t numAttributes = m_dataSetType->size();
    for(size_t i = 0; i < numAttributes; ++i)
    {
      if(propertyName == attributeComboBox->itemText(i).toStdString())
      {
        attributeComboBox->setCurrentIndex(i);
        break;
      }
    }

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

    // Make a copy of the legend contents of the reference layer
    std::vector<te::map::LegendItem*> layerLegend = refLayer->getLegend();
    size_t legendSize = layerLegend.size();

    m_legend.resize(legendSize);
    m_legendColors.resize(legendSize);
    for(size_t i = 0; i < legendSize; ++i)
    {
      m_legend[i] = new te::map::LegendItem(*layerLegend[i]);
      m_legendColors[i] = m_legend[i]->getColor();
    }

    m_colorBar = refLayer->getColorBar();

    // Set the opacity
    te::color::RGBAColor color = m_legend[0]->getColor();
    int opacity = (int)(color.getAlpha() * 100./255. + 0.5);
    opacityComboBox->setCurrentIndex(opacity);

    setTableContents();
  }
  else
  {
    slicesComboBox->setCurrentIndex(4);
    precisionComboBox->setCurrentIndex(6);

    m_colorBar = new te::color::ColorBar();

    m_colorBar->setBarSize(colorBar->width());
    m_colorBar->addColor(te::color::RGBAColor(255, 0, 0, TE_OPAQUE), 0.);      // red
    m_colorBar->addColor(te::color::RGBAColor(0, 255, 0, TE_OPAQUE), 0.25);    // green
    m_colorBar->addColor(te::color::RGBAColor(255, 255, 0, TE_OPAQUE), 0.5);   // yellow
    m_colorBar->addColor(te::color::RGBAColor(255, 0, 255, TE_OPAQUE), 0.75);  // magenta
    m_colorBar->addColor(te::color::RGBAColor(0, 0, 255, TE_OPAQUE), 1.0);     // blue

    opacityComboBox->setCurrentIndex(100);

    typeComboBoxActivated(typeComboBox->currentIndex());
  }

  colorBar->setHeight(25);

  colorBar->setColorBar(m_colorBar);

  // Connect signal/slots
  connect(typeComboBox, SIGNAL(activated(int)), this, SLOT(typeComboBoxActivated(int)));
  connect(applyPushButton, SIGNAL(clicked()), this, SLOT(applyPushButtonClicked()));
  connect(legendTableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(legendTableItemChanged(QTableWidgetItem*)));
  connect(okPushButton, SIGNAL(clicked()), this, SLOT(okPushButtonClicked()));
  connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(cancelPushButtonClicked()));
  connect(helpPushButton, SIGNAL(clicked()), this, SLOT(helpPushButtonClicked()));
}

const std::vector<te::map::LegendItem*>& te::qt::widgets::Legend::getLegend() const
{
  return m_legend;
}

size_t te::qt::widgets::Legend::getNumberOfValuesNotGrouped() const
{
  return m_numValuesNotGrouped;
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
  attributeComboBox->clear();
  legendTableWidget->clear();
  okPushButton->setEnabled(false);

  size_t numAttributes = m_dataSetType->size();
  for(size_t i = 0; i < numAttributes; ++i)
  {
    int type = m_dataSetType->getProperty(i)->getType();

    if(type == te::dt::GEOMETRY_TYPE || type == te::dt::DATETIME_TYPE ||
      (index != 3 && type == te::dt::STRING_TYPE))
      continue;

    attributeComboBox->addItem(m_dataSetType->getProperty(i)->getName().c_str());
  }

  slicesComboBox->setEnabled(false);
  stdDeviationComboBox->setEnabled(false);
  precisionComboBox->setEnabled(false);

  // Enable/Disable the grouping parameters according to the type selected
  if(index == 0 || index == 1)
  {
    // Equal Steps or Quantil
    slicesComboBox->setEnabled(true);
    precisionComboBox->setEnabled(true);
  }
  else if(index == 2)
  {
    // Standard Deviation
    stdDeviationComboBox->setEnabled(true);
    precisionComboBox->setEnabled(true);
  }
}

void te::qt::widgets::Legend::applyPushButtonClicked()
{
  ScopedCursor cursor(Qt::WaitCursor);

  m_changedItemLabel.clear();
  legendTableWidget->clear();

  // Delete the contents of the local legend and clear it
  for(size_t i = 0; i < m_legend.size(); ++i)
    delete m_legend[i];

  m_legend.clear();

  m_numValuesNotGrouped = 0;

  // Get the precision
  int precision = precisionComboBox->currentText().toInt();

  // Get the values of the attribute selected
  std::vector<std::string> attrValues;

  te::map::Layer* refLayer = static_cast<te::map::Layer*>(m_layerItem->getRefLayer());

  std::string sql = "SELECT " + attributeComboBox->currentText().toStdString();
  sql += " FROM " + refLayer->getId();

  te::da::DataSet* dataset = m_t->query(sql);

  size_t i = 0;
  while(dataset->moveNext())
  {
    if(dataset->isNull(i))
      ++m_numValuesNotGrouped;
    else
      attrValues.push_back(dataset->getAsString(0, precision));
  }

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

  // Clear the legend colors and get the new colors
  m_legendColors.clear();
  m_legendColors = m_colorBar->getSlices(m_legend.size());

  // Set the alpha component(opacity) of the generated colors
  for(size_t i = 0; i < m_legendColors.size(); ++i)
  {
    te::color::RGBAColor& c = m_legendColors[i];
    c.setColor(c.getRed(), c.getGreen(), c.getBlue(), opacityComboBox->currentText().toInt() * 255/100);
  }

  // If there are null values, generate a legend item at the end
  // of the legend and set its color as being totally white
  if(m_numValuesNotGrouped)
  {
    te::map::LegendItem* notGroupedItem = new te::map::LegendItem(QString("Null Values").toStdString(),
                                                                  QString("Null Values").toStdString());
    notGroupedItem->setTitle(QString("Null Values").toStdString());
    notGroupedItem->setCount(m_numValuesNotGrouped);

    te::color::RGBAColor color(255, 255, 255, 255);
    notGroupedItem->setColor(color);

    m_legend.push_back(notGroupedItem);
    m_legendColors.push_back(color);
  }

  // Set the new table contents
  setTableContents();
  okPushButton->setEnabled(true);
}

void te::qt::widgets::Legend::legendTableItemChanged(QTableWidgetItem* item)
{
  if((typeComboBox->currentIndex() == te::map::UNIQUE_VALUE && item->column() == 2) ||
     (typeComboBox->currentIndex() != te::map::UNIQUE_VALUE && item->column() == 3))
  {
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

  refLayer->setColorBar(m_colorBar);

  // Update the labels of the legend items that were changed
  std::map<int, std::string>::iterator it;
  for(it = m_changedItemLabel.begin(); it != m_changedItemLabel.end(); ++it)
    m_legend[it->first]->setTitle(it->second);

  // Set the colors of the legend generated
  for(size_t i = 0; i < m_legend.size(); ++i)
    m_legend[i]->setColor(m_legendColors[i]);
   
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
  QColor color;
  if(typeComboBox->currentIndex() == te::map::UNIQUE_VALUE)
  {
    // Unique Value
    legendTableWidget->setColumnCount(4);
    headerLabels << tr("Color") << tr("Values") << tr("Label") << tr("Quantity");

    for(size_t i = 0; i < m_legend.size(); ++i)
    {
      color.setRgba(m_legendColors[i].getRgba());
      QPixmap pixmap(16, 12);
      pixmap.fill(color);
      QIcon icon(pixmap);

      legendTableWidget->setItem(i, 0, new QTableWidgetItem(icon, ""));
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
      if((typeComboBox->currentIndex() == te::map::STD_DEVIATION &&
         m_legend[i]->getUpperLimit().empty()) == false)
      {
        color.setRgba(m_legendColors[i].getRgba());
        QPixmap pixmap(16, 12);
        pixmap.fill(color);
        QIcon icon(pixmap);
        legendTableWidget->setItem(i, 0, new QTableWidgetItem(icon, ""));
      }
      legendTableWidget->setItem(i, 1, new QTableWidgetItem(m_legend[i]->getLowerLimit().c_str()));
      legendTableWidget->setItem(i, 2, new QTableWidgetItem(m_legend[i]->getUpperLimit().c_str()));
      legendTableWidget->setItem(i, 3, new QTableWidgetItem(m_legend[i]->getTitle().c_str()));
      legendTableWidget->setItem(i, 4, new QTableWidgetItem(QString("%1").arg(m_legend[i]->getCount())));
    }
  }

  legendTableWidget->setHorizontalHeaderLabels(headerLabels);
}
