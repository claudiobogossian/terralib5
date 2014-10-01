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

  \brief A wizard page that provides the capability of connecting to a TerraLib 4.x database.
*/

// TerraLib
#include "ui_TL4ConnectorWizardPageForm.h"
#include "TL4ConnectorWizardPage.h"

// Qt
#include <QFileDialog>

// Boost
#include <boost/filesystem.hpp>

te::qt::plugins::terralib4::TL4ConnectorWizardPage::TL4ConnectorWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::TL4ConnectorWizardPageForm)
{
// setup controls
  m_ui->setupUi(this);

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
  m_ui->m_databaseTypeComboBox->addItem("Access");

  m_ui->m_databaseTypeComboBox->addItem("SQL Server");

  //m_ui->m_databaseTypeComboBox->addItem("SQL Server Spatial");
#endif

  /*
  m_ui->m_databaseTypeComboBox->addItem("Oracle");
  m_ui->m_databaseTypeComboBox->addItem("Oracle OCI");
  m_ui->m_databaseTypeComboBox->addItem("Oracle Spatial");
  m_ui->m_databaseTypeComboBox->addItem("MySQL");
  m_ui->m_databaseTypeComboBox->addItem("PostgreSQL");
  m_ui->m_databaseTypeComboBox->addItem("PostGIS");
  m_ui->m_databaseTypeComboBox->addItem("Firebird");*/

  connect(m_ui->m_databaseTypeComboBox,
          SIGNAL(currentIndexChanged(const QString&)),
          SLOT(onDatabaseTypeComboBoxActivated(const QString&)));

  connect(m_ui->m_directoryPushButton,
          SIGNAL(clicked()),
          SLOT(onDirectoryPushButtonClicked()));

  if(m_ui->m_databaseTypeComboBox->count() > 0)
  {
    m_ui->m_databaseTypeComboBox->setCurrentIndex(0);
    onDatabaseTypeComboBoxActivated(m_ui->m_databaseTypeComboBox->currentText());
  }
}

te::qt::plugins::terralib4::TL4ConnectorWizardPage::~TL4ConnectorWizardPage()
{
}

void te::qt::plugins::terralib4::TL4ConnectorWizardPage::onDirectoryPushButtonClicked()
{
  m_ui->m_dbComboBox->clear();

  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Database"), QString(""), tr("Database files (*.accdb *.mdb);; All Files (*.*)"), 0, QFileDialog::ReadOnly);

  if(fileName.isEmpty())
    return;

  m_ui->m_dbComboBox->addItem(fileName);

  boost::filesystem::path p(fileName.toStdString());

  p = p.filename();

  m_ui->m_databaseNameLineEdit->setText(p.string().c_str());
}

void te::qt::plugins::terralib4::TL4ConnectorWizardPage::onDatabaseTypeComboBoxActivated(const QString& text)
{
  if(text == QString("Access"))
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

    return;
  }

  if(text == QString("SQL Server") ||
    text == QString("SQL Server Spatial"))
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

    return;
  }

  //  // Oracle, Oracle OCI, Oracle Spatial
  //  case 3:
  //  case 4:
  //  case 5:
  //  {
  //    m_ui->m_hostTextLabel->setText(tr("Service Name:"));
  //    m_ui->m_hostLineEdit->setEnabled(true);
  //    m_ui->m_portLineEdit->setEnabled(false);
  //    m_ui->m_userLineEdit->setEnabled(true);
  //    m_ui->m_passwordLineEdit->setEnabled(true);
  //    m_ui->m_directoryPushButton->setEnabled(false);
  //    m_ui->m_dbComboBox->setEnabled(false);
  //    m_ui->m_databaseNameLineEdit->setEnabled(false);
  //    m_ui->m_encodingComboBox->setEnabled(false);
  //    break;
  //  }

  //  // MySQL, PostgreSQL, PostGIS
  //  case 6:
  //  case 7:
  //  case 8:
  //  {
  //    m_ui->m_hostTextLabel->setText(tr("Host:"));
  //    m_ui->m_hostLineEdit->setEnabled(true);
  //    m_ui->m_portLineEdit->setEnabled(true);
  //    m_ui->m_userLineEdit->setEnabled(true);
  //    m_ui->m_passwordLineEdit->setEnabled(true);
  //    m_ui->m_directoryPushButton->setEnabled(true);
  //    m_ui->m_dbComboBox->setEnabled(true);
  //    m_ui->m_databaseNameLineEdit->setEnabled(false);
  //    m_ui->m_encodingComboBox->setEnabled(false);
  //    break;
  //  }

  //  // Firebird
  //  case 9:
  //  {
  //    m_ui->m_hostTextLabel->setText(tr("Host:"));
  //    m_ui->m_hostLineEdit->setEnabled(true);
  //    m_ui->m_portLineEdit->setEnabled(true);
  //    m_ui->m_userLineEdit->setEnabled(true);
  //    m_ui->m_passwordLineEdit->setEnabled(true);
  //    m_ui->m_directoryPushButton->setEnabled(true);
  //    m_ui->m_dbComboBox->setEnabled(false);
  //    m_ui->m_databaseNameLineEdit->setEnabled(false);
  //    m_ui->m_encodingComboBox->setEnabled(false);
  //    break;
  //  }

  //  default:
  //  {
  //    break;
  //  }
  //}
}

std::map<std::string, std::string> te::qt::plugins::terralib4::TL4ConnectorWizardPage::getConnInfo()
{
  std::map<std::string, std::string> connInfo;

  connInfo["T4_DRIVER"] = getDriverName(m_ui->m_databaseTypeComboBox->currentText().toStdString());
  connInfo["T4_DB_NAME"] = m_ui->m_dbComboBox->currentText().toStdString();
  connInfo["T4_HOST"] = m_ui->m_hostLineEdit->text().toStdString();
  connInfo["T4_PORT"] = m_ui->m_portLineEdit->text().toStdString();
  connInfo["T4_USER"] = m_ui->m_userLineEdit->text().toStdString();
  connInfo["T4_PASSWORD"] = m_ui->m_passwordLineEdit->text().toStdString();

  return connInfo;
}

std::string te::qt::plugins::terralib4::TL4ConnectorWizardPage::getDriverName(const std::string& displayName)
{
  if("Access")
    return "Ado";
  else if("SQL Server")
    return "";
  else if("SQL Server Spatial")
    return "";
  else if("Oracle")
    return "";
  else if("Oracle OCI")
    return "";
  else if("Oracle Spatial")
    return "";
  else if("MySQL")
    return "MySQL";
  else if("PostgreSQL")
    return "PostgreSQL";
  else if("PostGIS")
    return "PostGIS";
  else if("Firebird")
    return "Firebird";
  else
    return "";
}
