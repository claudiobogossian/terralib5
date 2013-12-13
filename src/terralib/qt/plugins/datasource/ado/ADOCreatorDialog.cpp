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
  \file terralib/qt/widgets/connector/ado/ADOCreatorDialog.cpp

  \brief ....
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../widgets/Exception.h"
#include "../../../widgets/Utils.h"
#include "ui_ADOCreatorDialogForm.h"
#include "ADOCreatorDialog.h"

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

// Qt
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

te::qt::plugins::ado::ADOCreatorDialog::ADOCreatorDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::ADOCreatorDialogForm)
{
// add controls
  m_ui->setupUi(this);

// popule providers
  m_ui->m_providerComboBox->addItem("Microsoft.Jet.OLEDB.4.0");
  
// connect signal and slots
  connect(m_ui->m_applyPushButton, SIGNAL(pressed()), this, SLOT(applyPushButtonPressed()));
  //connect(m_ui->m_helpPushButton, SIGNAL(pressed()), this, SLOT(helpPushButtonPressed()));
  connect(m_ui->m_searchDatabaseToolButton, SIGNAL(pressed()), this, SLOT(searchDatabaseToolButtonPressed()));

  m_ui->m_helpPushButton->setNameSpace("dpi.inpe.br.plugins");
  m_ui->m_helpPushButton->setPageReference("plugins/ado/ado_creator.html");
}

te::qt::plugins::ado::ADOCreatorDialog::~ADOCreatorDialog()
{
}

const te::da::DataSourceInfoPtr& te::qt::plugins::ado::ADOCreatorDialog::getDataSource() const
{
  return m_datasource;
}

const te::da::DataSourcePtr& te::qt::plugins::ado::ADOCreatorDialog::getDriver() const
{
  return m_driver;
}

void te::qt::plugins::ado::ADOCreatorDialog::applyPushButtonPressed()
{
  try
  {
// check if driver is loaded
    if(te::da::DataSourceFactory::find("ADO") == 0)
      throw te::qt::widgets::Exception(TR_QT_WIDGETS("Sorry! No data access driver loaded for ADO data sources!"));

// get data source connection info based on form data
    std::map<std::string, std::string> dsInfo;

    getConnectionInfo(dsInfo);

    // create database
    te::da::DataSource::create("ADO", dsInfo);

    // Connect
    std::map<std::string, std::string> connInfo;
    connInfo["DB_NAME"] = dsInfo["DB_NAME"];
    connInfo["PROVIDER"] = dsInfo["PROVIDER"];
    if(!dsInfo["PASSWORD"].empty())
      connInfo["PASSWORD"] = dsInfo["PASSWORD"];

    std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("ADO");
    ds->setConnectionInfo(connInfo);
    ds->open();

    m_driver.reset(ds.release());

    if(m_driver.get() == 0)
      throw te::qt::widgets::Exception(TR_QT_WIDGETS("Could not open ADO data source due to an unknown error!"));
    
    QString title = m_ui->m_fileLineEdit->text().trimmed();

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
      m_datasource->setAccessDriver("ADO");
      m_datasource->setType("ADO");
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
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr(e.what()));
    return;
  }
  catch(...)
  {
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr("Unknown error while opening ADO database!"));
    return;
  }

  accept();
}

void te::qt::plugins::ado::ADOCreatorDialog::helpPushButtonPressed()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

void te::qt::plugins::ado::ADOCreatorDialog::searchDatabaseToolButtonPressed()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADO Database"), te::qt::widgets::GetFilePathFromSettings("vector"), tr("Database files (*.accdb *.mdb);; All Files (*.*)"), 0, QFileDialog::ReadOnly);

  if(fileName.isEmpty())
    return;

  m_ui->m_fileLineEdit->setText(fileName);

  QFileInfo info(fileName);

  te::qt::widgets::AddFilePathToSettings(info.absolutePath(), "vector");
}

void te::qt::plugins::ado::ADOCreatorDialog::getConnectionInfo(std::map<std::string, std::string>& connInfo) const
{
// clear input
  connInfo.clear();

  QString qstr = m_ui->m_fileLineEdit->text().trimmed();

  if(!qstr.isEmpty())
    connInfo["DB_NAME"] = qstr.toStdString();

  qstr = m_ui->m_passwordLineEdit->text().trimmed();

  if(!qstr.isEmpty())
    connInfo["PASSWORD"] = qstr.toStdString();

  qstr = m_ui->m_providerComboBox->currentText().trimmed();

  if(!qstr.isEmpty())
    connInfo["PROVIDER"] = qstr.toStdString();

  if(m_ui->m_createOGCTablesCheckBox->isChecked())
    connInfo["CREATE_OGC_METADATA_TABLES"] = "TRUE";
}