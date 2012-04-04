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

#include "RenameProperty.h"

te::qt::widgets::RenameProperty::RenameProperty(te::da::DataSource* ds, QWidget* parent)
  : m_ds(ds), m_transactor(0), m_catalogLoader(0), m_property(0), m_propertyParent(0), QDialog(parent)
{
  if (m_ds == 0)
    QMessageBox::critical(this, tr("Missing a Valid Data Source"), tr("Provide a valid data source!"));

  setupUi(this);

  // Get a data source catalog loader to access the datasource catalog
  m_transactor = m_ds->getTransactor();
  m_catalogLoader = m_transactor->getCatalogLoader();
    
  // Load the catalog to find out the information in the data source (only the schemas)
  m_catalogLoader->loadCatalog();

  // Get the dataset names of the data source
  std::vector<std::string*> datasets;
  m_catalogLoader->getDataSets(datasets);

  // Fill alphabetically the dataSetCombobox with the dataset names of the data source
  QStringList dataSetList;

  size_t numDataSets = datasets.size();
  for (size_t i = 0; i < numDataSets; ++i)
    dataSetList << (*datasets[i]).c_str();

  dataSetList.sort();
  dataSetComboBox->addItems(dataSetList);

  // Connect the signals/slots
  connect(dataSetComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(dataSetComboBoxChanged(const QString&)));
  connect(okPushButton, SIGNAL(clicked()), this, SLOT(okPushButtonClicked()));
  connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(cancelPushButtonClicked()));
  connect(helpPushButton, SIGNAL(clicked()), this, SLOT(helpPushButtonClicked()));

  dataSetComboBoxChanged(dataSetComboBox->currentText());
}

te::qt::widgets::RenameProperty::~RenameProperty()
{
  // Release the transactor
  if (m_transactor)
   delete m_transactor;
}

std::string te::qt::widgets::RenameProperty::getOldPropertyName()
{
  return m_property->getName();
}

std::string te::qt::widgets::RenameProperty::getNewPropertyName()
{
  return newPropertyNameLineEdit->text().toStdString();
}

void te::qt::widgets::RenameProperty::dataSetComboBoxChanged(const QString& dataSet)
{
  propertiesComboBox->clear();

  // Get the DataSetType associated to the dataset selected; 
  // it will be used as the parent of the property to be added.
  if(m_propertyParent)
    delete m_propertyParent;

  if(dataSetComboBox->currentText().isEmpty() == false)
    m_propertyParent = m_catalogLoader->getDataSetType(dataSet.toStdString(), false);

  // Fill the propertiesComboBox with the names of the properties of the dataset selected
  size_t numProperties = m_propertyParent->size();

  for (size_t i = 0; i < numProperties; ++i)
    propertiesComboBox->addItem(m_propertyParent->getProperty(i)->getName().c_str());
}

void te::qt::widgets::RenameProperty::okPushButtonClicked()
{
  // Get the property selected
  int propertyPos = propertiesComboBox->currentIndex();

  m_property = m_propertyParent->getProperty(propertyPos);

  // Release the transactor
  if (m_transactor)
  {
    delete m_transactor;
    m_transactor = 0;
  }

  accept();
}

void te::qt::widgets::RenameProperty::cancelPushButtonClicked()
{
  // Release the transactor
  if (m_transactor)
  {
    delete m_transactor;
    m_transactor = 0;
  }

  reject();
}

void te::qt::widgets::RenameProperty::helpPushButtonClicked()
{
}

void te::qt::widgets::RenameProperty::closeEvent(QCloseEvent* /*e*/)
{
  // Release the transactor
  if (m_transactor)
  {
    delete m_transactor;
    m_transactor = 0;
  }
}
