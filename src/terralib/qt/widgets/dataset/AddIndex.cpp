/*  Copyright (C) 2001-2011 National Institute For Space Research (INPE) - Brazil.

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
#include <QtGui/QtGui>
#include <QtGui/QComboBox>
#include <QtGui/QMessageBox>

// TerraLib
#include "../../../dataaccess.h"

#include "AddIndex.h"

te::qt::widgets::AddIndex::AddIndex(te::da::DataSource* ds, QWidget* parent)
  : QDialog(parent),
    m_ds(ds),
    m_index(0)
{
  if (m_ds == 0)
    QMessageBox::critical(this, tr("Missing a Valid Data Source"), tr("Provide a valid data source!"));

  setupUi(this);

  // Get the dataset names of the data source
  std::vector<std::string> datasetNames = m_ds->getDataSetNames();

  // Fill alphabetically the dataSetCombobox with the dataset names of the data source
  QStringList dataSetList;

  size_t numDataSets = datasetNames.size();
  for (size_t i = 0; i < numDataSets; ++i)
    dataSetList << (datasetNames[i]).c_str();

  dataSetList.sort();
  dataSetComboBox->addItems(dataSetList);

  // Connect the signals/slots
  connect(dataSetComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(dataSetComboBoxChanged(const QString&)));
  connect(addToIndexPropertiesPushButton, SIGNAL(clicked()), this, SLOT(addToIndexPropertiesPushButtonClicked()));
  connect(removeFromIndexPropertiesPushButton, SIGNAL(clicked()), this, SLOT(removefromIndexPropertiesPushButtonClicked()));
  connect(okPushButton, SIGNAL(clicked()), this, SLOT(okPushButtonClicked()));
  connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(cancelPushButtonClicked()));
  connect(helpPushButton, SIGNAL(clicked()), this, SLOT(helpPushButtonClicked()));
}

te::qt::widgets::AddIndex::~AddIndex()
{
}

void te::qt::widgets::AddIndex::dataSetComboBoxChanged(const QString& datasetNames)
{
  dataSetPropertiesListWidget->clear();
  indexPropertiesListWidget->clear();

  std::auto_ptr<te::da::DataSetType> dt;

  // Get the DataSetType associated to the dataset selected 
  if(dataSetComboBox->currentText().isEmpty() == false)
     dt = m_ds->getDataSetType(datasetNames.toStdString());

  // Fill the dataSetPropertiesListWidget with the names of the properties of the dataset selected
  size_t numProperties = dt->size();

  for (size_t i = 0; i < numProperties; ++i)
    dataSetPropertiesListWidget->addItem(dt->getProperty(i)->getName().c_str());
}

void te::qt::widgets::AddIndex::addToIndexPropertiesPushButtonClicked()
{
}


void te::qt::widgets::AddIndex::removeFromIndexPropertiesPushButtonClicked()
{
}
 

void te::qt::widgets::AddIndex::okPushButtonClicked()
{
  accept();
}

void te::qt::widgets::AddIndex::cancelPushButtonClicked()
{
  reject();
}

void te::qt::widgets::AddIndex::helpPushButtonClicked()
{
}

void te::qt::widgets::AddIndex::closeEvent(QCloseEvent* /*e*/)
{
}
