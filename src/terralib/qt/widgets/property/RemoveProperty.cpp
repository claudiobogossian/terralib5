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

#include "RemoveProperty.h"

te::qt::widgets::RemoveProperty::RemoveProperty(te::da::DataSource* ds, QWidget* parent)
  : QDialog(parent),
    m_ds(ds),
    m_property(0)
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
  connect(okPushButton, SIGNAL(clicked()), this, SLOT(okPushButtonClicked()));
  connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(cancelPushButtonClicked()));
  connect(helpPushButton, SIGNAL(clicked()), this, SLOT(helpPushButtonClicked()));

  dataSetComboBoxChanged(dataSetComboBox->currentText());
}

te::qt::widgets::RemoveProperty::~RemoveProperty()
{
}

void te::qt::widgets::RemoveProperty::dataSetComboBoxChanged(const QString& datasetName)
{
  propertiesComboBox->clear();

  std::vector<std::string> pNames = m_ds->getPropertyNames(datasetName.toStdString());

  for (size_t i = 0; i < pNames.size(); ++i)
    propertiesComboBox->addItem(pNames[i].c_str());
}

void te::qt::widgets::RemoveProperty::okPushButtonClicked()
{
  // Get the property selected
  int propertyPos = propertiesComboBox->currentIndex();
  std::string ds_name = (dataSetComboBox->currentText()).toStdString();
 
  te::dt::Property* p = (m_ds->getProperty(ds_name, propertyPos)).release();
   
  m_property = (m_ds->getProperty(dataSetComboBox->currentText().toStdString(), propertyPos)).release();

  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, tr("Remove Property"),
              tr("Do you really want to remove the property \"%1\"?").arg(p->getName().c_str()),
              QMessageBox::Yes | QMessageBox::Cancel);

  if (reply == QMessageBox::Yes)
    accept();
}

void te::qt::widgets::RemoveProperty::cancelPushButtonClicked()
{
  reject();
}

void te::qt::widgets::RemoveProperty::helpPushButtonClicked()
{
}

void te::qt::widgets::RemoveProperty::closeEvent(QCloseEvent* /*e*/)
{
}
