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

  // Connect signal/slots
  connect(typeComboBox, SIGNAL(activated(int)), this, SLOT(typeComboBoxActivated(int)));
  connect(okPushButton, SIGNAL(clicked()), this, SLOT(okPushButtonClicked()));
  connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(cancelPushButtonClicked()));
  connect(helpPushButton, SIGNAL(clicked()), this, SLOT(helpPushButtonClicked()));

  typeComboBoxActivated(typeComboBox->currentIndex());
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

  if(index == 0 || index == 1)
    slicesComboBox->setEnabled(true);        // Equal Steps or Quantil
  else if(index == 2)
    stdDeviationComboBox->setEnabled(true);  // Standard Deviation
}

void te::qt::widgets::Legend::okPushButtonClicked()
{
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

    std::string rmean;
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
