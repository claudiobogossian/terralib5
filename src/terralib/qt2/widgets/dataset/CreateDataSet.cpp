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
#include "../../../dataaccess2.h"

#include "CreateDataSet.h"

te::qt::widgets::CreateDataSet::CreateDataSet(te::da::DataSource* ds, QWidget* parent)
  : QDialog(parent), m_ds(ds), m_catalog(0), m_dataSetType(0)
{
  if (m_ds == 0)
    QMessageBox::critical(this, tr("Missing a Valid Data Source"), tr("Provide a valid data source!"));

  setupUi(this);

  // Get a data source catalog
  m_catalog = m_ds->getCatalog();
    
  // Connect the signals/slots
  connect(okPushButton, SIGNAL(clicked()), this, SLOT(okPushButtonClicked()));
  connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(cancelPushButtonClicked()));
  connect(helpPushButton, SIGNAL(clicked()), this, SLOT(helpPushButtonClicked()));
}

te::qt::widgets::CreateDataSet::~CreateDataSet()
{
  //// Release the transactor
  //if (m_transactor)
  // delete m_transactor;
}

void te::qt::widgets::CreateDataSet::okPushButtonClicked()
{
  // Get the schema name of the dataset to be created
  std::string schemaName = schemaNameLineEdit->text().toStdString();

  // Get the dataset name
  std::string dataSetName = dataSetNameLineEdit->text().toStdString();

  // Set the full dataset name
  std::string fullDataSetName = schemaName + "." + dataSetName;

  // Create the dataset type
  m_dataSetType = new te::da::DataSetType(fullDataSetName);
  //m_dataSetType->setCatalog(m_catalog);

  accept();
}

void te::qt::widgets::CreateDataSet::cancelPushButtonClicked()
{
  // Release the transactor
  //delete m_transactor;
  //m_transactor = 0;

  reject();
}

void te::qt::widgets::CreateDataSet::helpPushButtonClicked()
{
}

void te::qt::widgets::CreateDataSet::closeEvent(QCloseEvent* /*e*/)
{
  // Release the transactor
  //delete m_transactor;
}
