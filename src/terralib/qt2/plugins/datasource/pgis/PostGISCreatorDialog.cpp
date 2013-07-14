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
  \file terralib/qt/plugins/datasource/pgis/PostGISCreatorDialog.cpp

  \brief A dialog window for showing the PostgreSQL creator widget.
*/

// TerraLib
#include "PostGISCreatorDialog.h"
#include "../../../../dataaccess2/datasource/DataSource.h"
//#include "../../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../../dataaccess2/datasource/DataSourceInfo.h"
#include "../../../../dataaccess2/datasource/DataSourceManager.h"
#include "../../../widgets/Exception.h"
#include "ui_PostGISCreatorDialogForm.h"

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

// Qt
#include <QtGui/QMessageBox>

te::qt::plugins::pgis::PostGISCreatorDialog::PostGISCreatorDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::PostGISCreatorDialogForm)
{
  // add controls
  m_ui->setupUi(this);

// init controls
  m_ui->m_advancedOptionsGroupBox->hide();

// connect signal and slots
  connect(m_ui->m_advancedOptionsCheckBox, SIGNAL(toggled(bool)), this, SLOT(advancedCreationOptionsCheckBoxToggled(bool)));
  connect(m_ui->m_applyPushButton, SIGNAL(pressed()), this, SLOT(applyPushButtonPressed()));
  connect(m_ui->m_closePushButton, SIGNAL(pressed()), this, SLOT(closePushButtonPressed()));
  connect(m_ui->m_userNameLineEdit, SIGNAL(editingFinished()), this, SLOT(passwordLineEditEditingFinished()));
  connect(m_ui->m_passwordLineEdit, SIGNAL(editingFinished()), this, SLOT(passwordLineEditEditingFinished()));

  m_ui->m_helpPushButton->setNameSpace("dpi.inpe.br.plugins");
  m_ui->m_helpPushButton->setPageReference("plugins/pgis/pgis_creator.html");
}

te::qt::plugins::pgis::PostGISCreatorDialog::~PostGISCreatorDialog()
{

}

void te::qt::plugins::pgis::PostGISCreatorDialog::applyPushButtonPressed()
{
  try
  {
// check if driver is loaded
    if(te::da::DataSourceFactory::find("POSTGIS") == 0)
      throw te::qt::widgets::Exception(TR_QT_WIDGETS("Sorry! No data access driver loaded for PostgreSQL data sources!"));

// get data source connection info based on form data
    std::map<std::string, std::string> dsInfo;

    getConnectionInfo(dsInfo);

// create database
    te::da::DataSource::create("POSTGIS", dsInfo);

  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr(e.what()));
    return;
  }
  catch(...)
  {
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr("Unknown error while opening PostgreSQL database!"));
    return;
  }

  accept();
}

void te::qt::plugins::pgis::PostGISCreatorDialog::closePushButtonPressed()
{
  reject();
}

void te::qt::plugins::pgis::PostGISCreatorDialog::advancedCreationOptionsCheckBoxToggled(bool t)
{
  m_ui->m_advancedOptionsGroupBox->setVisible(t);
}

void te::qt::plugins::pgis::PostGISCreatorDialog::getConnectionInfo(std::map<std::string, std::string>& connInfo) const
{
// clear input
  connInfo.clear();

// get host
  QString qstr = m_ui->m_hostNameLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["PG_HOST"] = qstr.toStdString();

// get port
  qstr = m_ui->m_portLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["PG_PORT"] = qstr.toStdString();

// get user
  qstr = m_ui->m_userNameLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["PG_USER"] = qstr.toStdString();

// get password
  qstr = m_ui->m_passwordLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["PG_PASSWORD"] = qstr.toStdString();

// get dbname
  qstr = m_ui->m_newDatabaseNameLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["PG_NEWDB_NAME"] = qstr.toStdString();

// get Template
  qstr = m_ui->m_templateComboBox->currentText().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["PG_NEWDB_TEMPLATE"] = qstr.toStdString();

// get Owner
  qstr = m_ui->m_ownerComboBox->currentText().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["PG_NEWDB_OWNER"] = qstr.toStdString();

// get Encoding
  qstr = m_ui->m_encodingComboBox->currentText().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["PG_NEWDB_ENCODING"] = qstr.toStdString();

// get Tablespace
  qstr = m_ui->m_tablespaceLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["PG_NEWDB_TABLESPACE"] = qstr.toStdString();

// get Connections limit
  if(!m_ui->m_noLimitConnectionsGroupBox->isChecked())
  {
    qstr = m_ui->m_connectionsLimitSpinBox->text().trimmed();

    if(!qstr.isEmpty())
    {
      if(boost::lexical_cast<int>(qstr.toStdString()) >= 1)
        connInfo["PG_NEWDB_CONN_LIMIT"] = qstr.toStdString();
      else
        connInfo["PG_NEWDB_CONN_LIMIT"] = "-1";
    }
  }
}

const te::da::DataSourceInfoPtr& te::qt::plugins::pgis::PostGISCreatorDialog::getDataSource() const
{
  return m_datasource;
}

const te::da::DataSourcePtr& te::qt::plugins::pgis::PostGISCreatorDialog::getDriver() const
{
  return m_driver;
}

void te::qt::plugins::pgis::PostGISCreatorDialog::passwordLineEditEditingFinished()
{
  if(m_ui->m_userNameLineEdit->text() != "" && m_ui->m_passwordLineEdit->text() != "")
  {
    std::map<std::string, std::string> dsInfo;
    getConnectionInfo(dsInfo);

    // Get Templates/Databases
    std::vector<std::string> templates = te::da::DataSource::getDataSources("POSTGIS", dsInfo);
    if(!templates.empty())
      for(std::size_t i = 0; i < templates.size(); i++)
        m_ui->m_templateComboBox->addItem(templates[i].c_str());

    m_ui->m_templateComboBox->setCurrentIndex(m_ui->m_templateComboBox->findText("postgis"));

    // Get Encodings
    m_ui->m_encodingComboBox->addItem("");
    std::vector<std::string> encodings = te::da::DataSource::getEncodings("POSTGIS", dsInfo);
    if(!encodings.empty())
      for(std::size_t i = 0; i < encodings.size(); i++)
        m_ui->m_encodingComboBox->addItem(encodings[i].c_str());
  }
}