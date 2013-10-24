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
  \file terralib/qt/plugins/terralib4/TL4ConnectorWizardPage.cpp

  \brief A Qt dialog for ....
*/

// TerraLib
#include "ui_TL4ConnectorWizardPageForm.h"
#include "TL4ConnectorWizardPage.h"

// Qt
#include <QtGui/QFileDialog>

// Boost
#include <boost/filesystem.hpp>

te::qt::plugins::terralib4::TL4ConnectorWizardPage::TL4ConnectorWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::TL4ConnectorWizardPageForm)
{
// setup controls
  m_ui->setupUi(this);

  m_ui->m_databaseTypeComboBox->addItem("Access");
  m_ui->m_databaseTypeComboBox->addItem("SQL Server");
  m_ui->m_databaseTypeComboBox->addItem("SQL Server Spatial");
  m_ui->m_databaseTypeComboBox->addItem("Oracle");
  m_ui->m_databaseTypeComboBox->addItem("Oracle OCI");
  m_ui->m_databaseTypeComboBox->addItem("Oracle Spatial");
  m_ui->m_databaseTypeComboBox->addItem("MySQL");
  m_ui->m_databaseTypeComboBox->addItem("PostgreSQL");
  m_ui->m_databaseTypeComboBox->addItem("PostGIS");
  m_ui->m_databaseTypeComboBox->addItem("Firebird");

  connect(m_ui->m_databaseTypeComboBox, SIGNAL(activated(int)), SLOT(onDatabaseTypeComboBoxActivated(int)));
  connect(m_ui->m_directoryPushButton, SIGNAL(clicked()), SLOT(onDirectoryPushButtonClicked()));
}

te::qt::plugins::terralib4::TL4ConnectorWizardPage::~TL4ConnectorWizardPage()
{
}

void te::qt::plugins::terralib4::TL4ConnectorWizardPage::onDirectoryPushButtonClicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Database"), QString(""), tr("Database files (*.accdb *.mdb);; All Files (*.*)"), 0, QFileDialog::ReadOnly);

  if(fileName.isEmpty())
    return;

  m_ui->m_dbComboBox->addItem(fileName);

  boost::filesystem::path p(fileName.toStdString());
  p = p.filename();

  m_ui->m_databaseNameLineEdit->setText(p.string().c_str());
}

void te::qt::plugins::terralib4::TL4ConnectorWizardPage::onDatabaseTypeComboBoxActivated(int i)
{
  switch(i)
  {
    // Access
    case 0:
    {
      m_ui->m_hostTextLabel->setText(tr("Host:"));
      m_ui->m_hostLineEdit->setEnabled(false);
      m_ui->m_portLineEdit->setEnabled(false);
      m_ui->m_userLineEdit->setEnabled(false);
      m_ui->m_passwordLineEdit->setEnabled(false);
      m_ui->m_directoryPushButton->setEnabled(true);
      m_ui->m_dbComboBox->setEnabled(false);
      m_ui->m_databaseNameLineEdit->setEnabled(false);
      m_ui->m_encodingComboBox->setEnabled(false);
      
      break;
    }

    // SQL Server, SQL Server Spatial
    case 1:
    case 2:
    {
      m_ui->m_hostTextLabel->setText(tr("Host:"));
      m_ui->m_hostLineEdit->setEnabled(true);
      m_ui->m_portLineEdit->setEnabled(true);
      m_ui->m_userLineEdit->setEnabled(true);
      m_ui->m_passwordLineEdit->setEnabled(true);
      m_ui->m_directoryPushButton->setEnabled(false);
      m_ui->m_dbComboBox->setEnabled(false);
      m_ui->m_databaseNameLineEdit->setEnabled(true);
      m_ui->m_encodingComboBox->setEnabled(true);
      break;
    }

    // Oracle, Oracle OCI, Oracle Spatial
    case 3:
    case 4:
    case 5:
    {
      m_ui->m_hostTextLabel->setText(tr("Service Name:"));
      m_ui->m_hostLineEdit->setEnabled(true);
      m_ui->m_portLineEdit->setEnabled(false);
      m_ui->m_userLineEdit->setEnabled(true);
      m_ui->m_passwordLineEdit->setEnabled(true);
      m_ui->m_directoryPushButton->setEnabled(false);
      m_ui->m_dbComboBox->setEnabled(false);
      m_ui->m_databaseNameLineEdit->setEnabled(false);
      m_ui->m_encodingComboBox->setEnabled(false);
      break;
    }

    // MySQL, PostgreSQL, PostGIS
    case 6:
    case 7:
    case 8:
    {
      m_ui->m_hostTextLabel->setText(tr("Host:"));
      m_ui->m_hostLineEdit->setEnabled(true);
      m_ui->m_portLineEdit->setEnabled(true);
      m_ui->m_userLineEdit->setEnabled(true);
      m_ui->m_passwordLineEdit->setEnabled(true);
      m_ui->m_directoryPushButton->setEnabled(true);
      m_ui->m_dbComboBox->setEnabled(true);
      m_ui->m_databaseNameLineEdit->setEnabled(false);
      m_ui->m_encodingComboBox->setEnabled(false);
      break;
    }

    // Firebird
    case 9:
    {
      m_ui->m_hostTextLabel->setText(tr("Host:"));
      m_ui->m_hostLineEdit->setEnabled(true);
      m_ui->m_portLineEdit->setEnabled(true);
      m_ui->m_userLineEdit->setEnabled(true);
      m_ui->m_passwordLineEdit->setEnabled(true);
      m_ui->m_directoryPushButton->setEnabled(true);
      m_ui->m_dbComboBox->setEnabled(false);
      m_ui->m_databaseNameLineEdit->setEnabled(false);
      m_ui->m_encodingComboBox->setEnabled(false);
      break;
    }

    default:
    {
      break;
    }
  }
}