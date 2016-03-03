/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../widgets/Exception.h"
#include "../../../widgets/utils/ScopedCursor.h"
#include "ui_PostGISCreatorDialogForm.h"

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

// Qt
#include <QMessageBox>

te::qt::plugins::pgis::PostGISCreatorDialog::PostGISCreatorDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::PostGISCreatorDialogForm)
{
  // add controls
  m_ui->setupUi(this);

// init controls
  m_ui->m_advancedOptionsGroupBox->hide();

// connect signal and slots
  connect(m_ui->m_advancedOptionsCheckBox, SIGNAL(toggled(bool)), this, SLOT(onAdvancedCreationOptionsCheckBoxToggled(bool)));
  connect(m_ui->m_applyPushButton, SIGNAL(pressed()), this, SLOT(onApplyPushButtonPressed()));
  connect(m_ui->m_closePushButton, SIGNAL(pressed()), this, SLOT(onClosePushButtonPressed()));
  connect(m_ui->m_userNameLineEdit, SIGNAL(editingFinished()), this, SLOT(onLineEditEditingFinished()));
  connect(m_ui->m_passwordLineEdit, SIGNAL(editingFinished()), this, SLOT(onLineEditEditingFinished()));
  connect(m_ui->m_hostNameLineEdit, SIGNAL(editingFinished()), this, SLOT(onLineEditEditingFinished()));
  connect(m_ui->m_portLineEdit, SIGNAL(editingFinished()), this, SLOT(onLineEditEditingFinished()));

  m_ui->m_portLineEdit->setValidator(new QIntValidator(0, 99999, this));

  m_ui->m_helpPushButton->setNameSpace("dpi.inpe.br.plugins");
  m_ui->m_helpPushButton->setPageReference("plugins/pgis/pgis_creator.html");
}

te::qt::plugins::pgis::PostGISCreatorDialog::~PostGISCreatorDialog()
{

}

void te::qt::plugins::pgis::PostGISCreatorDialog::onApplyPushButtonPressed()
{
  QApplication::setOverrideCursor(Qt::WaitCursor);

  try
  {
// check if driver is loaded
    if(te::da::DataSourceFactory::find("POSTGIS") == 0)
    {
      QApplication::restoreOverrideCursor();
      throw te::qt::widgets::Exception(TE_TR("Sorry! No data access driver loaded for PostgreSQL data sources!"));
    }

// get data source connection info based on form data
    std::map<std::string, std::string> dsInfo = getConnectionInfo(true);

// create database
    te::da::DataSource::create("POSTGIS", dsInfo);

    {
      // Connect
      std::map<std::string, std::string> connInfo;
      if(!dsInfo["PG_HOST"].empty())
        connInfo["PG_HOST"] = dsInfo["PG_HOST"];

      if(!dsInfo["PG_PORT"].empty())
        connInfo["PG_PORT"] = dsInfo["PG_PORT"];

      connInfo["PG_USER"] = dsInfo["PG_USER"];
      connInfo["PG_PASSWORD"] = dsInfo["PG_PASSWORD"];
      connInfo["PG_DB_NAME"] = dsInfo["PG_NEWDB_NAME"];
      connInfo["PG_MIN_POOL_SIZE"] = "1";
      connInfo["PG_MAX_POOL_SIZE"] = "4";
      connInfo["PG_CONNECT_TIMEOUT"] = "5";
      if(!dsInfo["PG_NEWDB_ENCODING"].empty())
        connInfo["PG_CLIENT_ENCODING"] = dsInfo["PG_NEWDB_ENCODING"];

      std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("POSTGIS");
      ds->setConnectionInfo(connInfo);
      ds->open();

      m_driver.reset(ds.release());
    }

    if(m_driver.get() == 0)
    {
      QApplication::restoreOverrideCursor();
      throw te::qt::widgets::Exception(TE_TR("Could not open POSTGIS data source due to an unknown error!"));
    }
    
    QString title = m_ui->m_hostNameLineEdit->text().trimmed() + QString::fromStdString("@") + m_ui->m_newDatabaseNameLineEdit->text().trimmed() + QString::fromStdString("@") + m_ui->m_userNameLineEdit->text().trimmed();

    std::map<std::string, std::string> connInfo;
    if(!dsInfo["PG_HOST"].empty())
      connInfo["PG_HOST"] = dsInfo["PG_HOST"];

    if(!dsInfo["PG_PORT"].empty())
      connInfo["PG_PORT"] = dsInfo["PG_PORT"];

    connInfo["PG_USER"] = dsInfo["PG_USER"];

    if(m_ui->m_savePasswordCheckBox)
      connInfo["PG_PASSWORD"] = dsInfo["PG_PASSWORD"];

    connInfo["PG_DB_NAME"] = dsInfo["PG_NEWDB_NAME"];
    connInfo["PG_MIN_POOL_SIZE"] = "1";
    connInfo["PG_MAX_POOL_SIZE"] = "4";
    connInfo["PG_CONNECT_TIMEOUT"] = "5";
    if(!dsInfo["PG_NEWDB_ENCODING"].empty())
    connInfo["PG_CLIENT_ENCODING"] = dsInfo["PG_NEWDB_ENCODING"];

    if(m_datasource.get() == 0)
    {
// create a new data source based on form data
      m_datasource.reset(new te::da::DataSourceInfo);

      m_datasource->setConnInfo(connInfo);

      boost::uuids::basic_random_generator<boost::mt19937> gen;
      boost::uuids::uuid u = gen();
      std::string dsId = boost::uuids::to_string(u);

      m_datasource->setId(dsId);
      m_driver->setId(dsId);
      m_datasource->setTitle(title.toUtf8().data());
      m_datasource->setDescription("");
      m_datasource->setAccessDriver("POSTGIS");
      m_datasource->setType("POSTGIS");
    }
    else
    {
      m_driver->setId(m_datasource->getId());
      m_datasource->setConnInfo(connInfo);
      m_datasource->setTitle(title.toUtf8().data());
      m_datasource->setDescription("");
    }

  }
  catch(const std::exception& e)
  {
    QApplication::restoreOverrideCursor();
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr(e.what()));
    return;
  }
  catch(...)
  {
    QApplication::restoreOverrideCursor();
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr("Unknown error while opening PostgreSQL database!"));
    return;
  }

  QApplication::restoreOverrideCursor();
  accept();
}

void te::qt::plugins::pgis::PostGISCreatorDialog::onClosePushButtonPressed()
{
  reject();
}

void te::qt::plugins::pgis::PostGISCreatorDialog::onAdvancedCreationOptionsCheckBoxToggled(bool t)
{
  m_ui->m_advancedOptionsGroupBox->setVisible(t);
}

std::map<std::string, std::string> te::qt::plugins::pgis::PostGISCreatorDialog::getConnectionInfo(bool getPrivateKeys) const
{
  std::map<std::string, std::string> connInfo;

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
  if(getPrivateKeys)
  {
    qstr = m_ui->m_passwordLineEdit->text().trimmed();
  
    if(!qstr.isEmpty())
      connInfo["PG_PASSWORD"] = qstr.toStdString();
  }

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
  
  if (!qstr.isEmpty())
  {
    if (qstr.toStdString() == "UTF-8")
      connInfo["PG_NEWDB_ENCODING"] = "UTF8";
    else
      connInfo["PG_NEWDB_ENCODING"] = qstr.toStdString();
  }

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

  return connInfo;
}

const te::da::DataSourceInfoPtr& te::qt::plugins::pgis::PostGISCreatorDialog::getDataSource() const
{
  return m_datasource;
}

const te::da::DataSourcePtr& te::qt::plugins::pgis::PostGISCreatorDialog::getDriver() const
{
  return m_driver;
}

void te::qt::plugins::pgis::PostGISCreatorDialog::onLineEditEditingFinished()
{
  try
  {
    if(m_ui->m_userNameLineEdit->text() != "" && m_ui->m_passwordLineEdit->text() != "")
    {
      std::map<std::string, std::string> dsInfo = getConnectionInfo(true);

      // Get Templates/Databases
      std::vector<std::string> templates = te::da::DataSource::getDataSourceNames("POSTGIS", dsInfo);
      if (!templates.empty())
      {
        m_ui->m_templateComboBox->clear();

        for (std::size_t i = 0; i < templates.size(); i++)
          m_ui->m_templateComboBox->addItem(templates[i].c_str());
      }

      m_ui->m_templateComboBox->setCurrentIndex(m_ui->m_templateComboBox->findText("postgis"));

      // Get Encodings
      m_ui->m_encodingComboBox->clear();
      m_ui->m_encodingComboBox->addItem("");
      std::vector<te::common::CharEncoding> encodings = te::da::DataSource::getEncodings("POSTGIS", dsInfo);
      if(!encodings.empty())
        for(std::size_t i = 0; i < encodings.size(); i++)
          m_ui->m_encodingComboBox->addItem(te::common::CharEncodingConv::getCharEncodingName(encodings[i]).c_str());

      // Try to go the owners
      m_ui->m_ownerComboBox->clear();
      std::map<std::string, std::string> info;
      std::map<std::string, std::string> aux = getConnectionInfo(true);
      info["PG_HOST"] = aux["PG_HOST"].empty()?"localhost":aux["PG_HOST"];
      info["PG_PORT"] = aux["PG_PORT"].empty()?"5432":aux["PG_PORT"];
      info["PG_USER"] = aux["PG_USER"];
      info["PG_PASSWORD"] = aux["PG_PASSWORD"];

      std::auto_ptr<te::da::DataSource> auxDs = te::da::DataSourceFactory::make("POSTGIS");
      auxDs->setConnectionInfo(info);
      auxDs->open();

      std::auto_ptr<te::da::DataSet> dsRoles = auxDs->query("select * from pg_roles");

      while(dsRoles->moveNext())
        m_ui->m_ownerComboBox->addItem(dsRoles->getString("rolname").c_str());

      auxDs->close();
    }
  }
  catch(...)
  {
    return;
  }
}