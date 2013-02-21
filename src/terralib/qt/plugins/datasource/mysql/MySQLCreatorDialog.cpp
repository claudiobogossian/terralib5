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
  \file terralib/qt/plugins/datasource/mysql/MySQLCreatorDialog.cpp

  \brief A dialog window for showing the MySQL creator widget.
*/

// TerraLib
#include "MySQLCreatorDialog.h"
#include "ui_MySQLCreatorDialogForm.h"

// Qt
#include <QtGui/QMessageBox>

te::qt::plugins::mysql::MySQLCreatorDialog::MySQLCreatorDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::MySQLCreatorDialogForm)
{
  // add controls
  m_ui->setupUi(this);

// init controls
  m_ui->m_advancedOptionsGroupBox->hide();

// connect signal and slots
  connect(m_ui->m_advancedOptionsCheckBox, SIGNAL(toggled(bool)), this, SLOT(advancedCreationOptionsCheckBoxToggled(bool)));
  connect(m_ui->m_applyPushButton, SIGNAL(pressed()), this, SLOT(applyPushButtonPressed()));
  connect(m_ui->m_closePushButton, SIGNAL(pressed()), this, SLOT(closePushButtonPressed()));
  connect(m_ui->m_helpPushButton, SIGNAL(pressed()), this, SLOT(helpPushButtonPressed()));
}

te::qt::plugins::mysql::MySQLCreatorDialog::~MySQLCreatorDialog()
{

}

void te::qt::plugins::mysql::MySQLCreatorDialog::applyPushButtonPressed()
{

}

void te::qt::plugins::mysql::MySQLCreatorDialog::closePushButtonPressed()
{
  reject();
}

void te::qt::plugins::mysql::MySQLCreatorDialog::helpPushButtonPressed()
{

}

void te::qt::plugins::mysql::MySQLCreatorDialog::advancedCreationOptionsCheckBoxToggled(bool t)
{
  m_ui->m_advancedOptionsGroupBox->setVisible(t);
}

void te::qt::plugins::mysql::MySQLCreatorDialog::getConnectionInfo(std::map<std::string, std::string>& connInfo) const
{
// clear input
  connInfo.clear();

// get host
  QString qstr = m_ui->m_hostNameLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_HOST_NAME"] = qstr.toStdString();

// get port
  qstr = m_ui->m_portLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_PORT"] = qstr.toStdString();

// get user
  qstr = m_ui->m_userNameLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_USER_NAME"] = qstr.toStdString();

// get password
  qstr = m_ui->m_passwordLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_PASSWORD"] = qstr.toStdString();

// get dbname
  qstr = m_ui->m_schemaNameComboBox->currentText().trimmed();
  //qstr = m_ui->m_schemaNameLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_SCHEMA"] = qstr.toStdString();

// get charset
  qstr = m_ui->m_charsetComboBox->currentText().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_OPT_CHARSET_NAME"] = qstr.toStdString();
    
// get new host
  qstr = m_ui->m_newHostNameLineEdit->text().trimmed();

  if(!qstr.isEmpty())
    connInfo["MY_NEW_SCHEMA_HOST"] = qstr.toStdString();

// get new port
  qstr = m_ui->m_newPortLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_NEW_SCHEMA_PORT"] = qstr.toStdString();

// get new user
  qstr = m_ui->m_newUserNameLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_NEW_SCHEMA_USER_NAME"] = qstr.toStdString();

// get new password
  qstr = m_ui->m_newPasswordLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_NEW_SCHEMA_PASSWORD"] = qstr.toStdString();

// get new dbname
  qstr = m_ui->m_newSchemaNameLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_NEW_SCHEMA_NAME"] = qstr.toStdString();

// get new charset
  qstr = m_ui->m_newCharsetComboBox->currentText().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_NEW_SCHEMA_CHARSET_NAME"] = qstr.toStdString();

// get new collate
  qstr = m_ui->m_newCollateComboBox->currentText().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_NEW_SCHEMA_COLLATE_NAME"] = qstr.toStdString();

  if(m_ui->m_createRasterMetadataCheckBox->isChecked())
    connInfo["MY_NEW_SCHEMA_CREATE_TERRALIB_RASTER_METADATA_TABLES"] = qstr.toStdString();

  if(m_ui->m_createOGCMetadataCheckBox->isChecked())
    connInfo["MY_NEW_SCHEMA_CREATE_OGC_METADATA_TABLES"] = qstr.toStdString();
}

void te::qt::plugins::mysql::MySQLCreatorDialog::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{

}

const te::da::DataSourceInfoPtr& te::qt::plugins::mysql::MySQLCreatorDialog::getDataSource() const
{
  return m_datasource;
}

const te::da::DataSourcePtr& te::qt::plugins::mysql::MySQLCreatorDialog::getDriver() const
{
  return m_driver;
}

void te::qt::plugins::mysql::MySQLCreatorDialog::set(const te::da::DataSourceInfoPtr& ds)
{

}