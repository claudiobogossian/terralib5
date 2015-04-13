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
  \file terralib/qt/widgets/connector/ado/ADOConnectorDialog.cpp

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
#include "ui_ADOConnectorDialogForm.h"
#include "ADOConnectorDialog.h"

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

// Qt
#include <QFileDialog>
#include <QMessageBox>

te::qt::plugins::ado::ADOConnectorDialog::ADOConnectorDialog(QWidget* parent, Qt::WindowFlags f, Operation op)
  : QDialog(parent, f),
    m_ui(new Ui::ADOConnectorDialogForm)
{
// add controls
  m_ui->setupUi(this);

// popule providers
#ifdef _M_IX86
  m_ui->m_providerComboBox->addItem("Microsoft.Jet.OLEDB.4.0");
#else
  m_ui->m_providerComboBox->addItem("Microsoft.ACE.OLEDB.12.0");
#endif

// connect signal and slots
  connect(m_ui->m_openPushButton, SIGNAL(pressed()), this, SLOT(openPushButtonPressed()));
  connect(m_ui->m_testPushButton, SIGNAL(pressed()), this, SLOT(testPushButtonPressed()));
  //connect(m_ui->m_helpPushButton, SIGNAL(pressed()), this, SLOT(helpPushButtonPressed()));
  connect(m_ui->m_searchDatabaseToolButton, SIGNAL(pressed()), this, SLOT(searchDatabaseToolButtonPressed()));

  m_ui->m_helpPushButton->setNameSpace("dpi.inpe.br.plugins");
  m_ui->m_helpPushButton->setPageReference("plugins/ado/ado.html");

  if(op == UPDATE)
    m_ui->m_openPushButton->setText(TE_TR("Update"));
}

te::qt::plugins::ado::ADOConnectorDialog::~ADOConnectorDialog()
{
}

const te::da::DataSourceInfoPtr& te::qt::plugins::ado::ADOConnectorDialog::getDataSource() const
{
  return m_datasource;
}

const te::da::DataSourcePtr& te::qt::plugins::ado::ADOConnectorDialog::getDriver() const
{
  return m_driver;
}

void te::qt::plugins::ado::ADOConnectorDialog::set(const te::da::DataSourceInfoPtr& ds)
{
  m_datasource = ds;

  if(m_datasource.get() != 0)
  {
    setConnectionInfo(m_datasource->getConnInfo());

    m_ui->m_datasourceTitleLineEdit->setText(QString::fromStdString(m_datasource->getTitle()));

    m_ui->m_datasourceDescriptionTextEdit->setText(QString::fromStdString(m_datasource->getDescription()));
  }
}

void te::qt::plugins::ado::ADOConnectorDialog::openPushButtonPressed()
{
  try
  {
// check if driver is loaded
    if(te::da::DataSourceFactory::find("ADO") == 0)
      throw te::qt::widgets::Exception(TE_TR("Sorry! No data access driver loaded for ADO data sources!"));

// get data source connection info based on form data
    std::map<std::string, std::string> dsInfo;

    getConnectionInfo(dsInfo);

// perform connection
    //m_driver.reset(te::da::DataSourceFactory::open("ADO", dsInfo));
    std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("ADO");
    ds->setConnectionInfo(dsInfo);
    ds->open();
    m_driver.reset(ds.release());

    if(m_driver.get() == 0)
      throw te::qt::widgets::Exception(TE_TR("Could not open ADO data source due to an unknown error!"));

    QString title = m_ui->m_datasourceTitleLineEdit->text().trimmed();

    if(title.isEmpty())
      title = m_ui->m_fileLineEdit->text().trimmed();

    if(m_datasource.get() == 0)
    {
// create a new data source based on form data
      m_datasource.reset(new te::da::DataSourceInfo);

      m_datasource->setConnInfo(dsInfo);

      boost::uuids::basic_random_generator<boost::mt19937> gen;
      boost::uuids::uuid u = gen();
      std::string dsId = boost::uuids::to_string(u);

      m_datasource->setId(dsId);
      m_driver->setId(dsId);
      m_datasource->setTitle(title.toUtf8().data());
      m_datasource->setDescription(m_ui->m_datasourceDescriptionTextEdit->toPlainText().trimmed().toUtf8().data());
      m_datasource->setAccessDriver("ADO");
      m_datasource->setType("ADO");
    }
    else
    {
      m_driver->setId(m_datasource->getId());
      m_datasource->setConnInfo(dsInfo);
      m_datasource->setTitle(title.toUtf8().data());
      m_datasource->setDescription(m_ui->m_datasourceDescriptionTextEdit->toPlainText().trimmed().toUtf8().data());
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

void te::qt::plugins::ado::ADOConnectorDialog::testPushButtonPressed()
{
  try
  {
// check if driver is loaded
    if(te::da::DataSourceFactory::find("ADO") == 0)
      throw te::qt::widgets::Exception(TE_TR("Sorry! No data access driver loaded for ADO data sources!"));

// get data source connection info based on form data
    std::map<std::string, std::string> dsInfo;

    getConnectionInfo(dsInfo);

// perform connection
    //std::auto_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::open("ADO", dsInfo));
    std::auto_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::make("ADO"));
    ds->setConnectionInfo(dsInfo);
    ds->open();

    if(ds.get() == 0)
      throw te::qt::widgets::Exception(TE_TR("Could not open ADO database!"));

    QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Data source is ok!"));
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr(e.what()));
  }
  catch(...)
  {
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr("Unknown error while testing ADO data source!"));
  }
}

void te::qt::plugins::ado::ADOConnectorDialog::helpPushButtonPressed()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

void te::qt::plugins::ado::ADOConnectorDialog::searchDatabaseToolButtonPressed()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open ADO Database"), te::qt::widgets::GetFilePathFromSettings("vector"), tr("Database files (*.accdb *.mdb);; All Files (*.*)"), 0, QFileDialog::ReadOnly);

  if(fileName.isEmpty())
    return;

  m_ui->m_fileLineEdit->setText(fileName);

  QFileInfo info(fileName);

  te::qt::widgets::AddFilePathToSettings(info.absolutePath(), "vector");
}

void te::qt::plugins::ado::ADOConnectorDialog::getConnectionInfo(std::map<std::string, std::string>& connInfo) const
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
}

void te::qt::plugins::ado::ADOConnectorDialog::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  std::map<std::string, std::string>::const_iterator it = connInfo.find("DB_NAME");
  std::map<std::string, std::string>::const_iterator itend = connInfo.end();

  if(it != itend)
    m_ui->m_fileLineEdit->setText(it->second.c_str());

  it = connInfo.find("PASSWORD");

  if(it != itend)
    m_ui->m_passwordLineEdit->setText(it->second.c_str());
}
