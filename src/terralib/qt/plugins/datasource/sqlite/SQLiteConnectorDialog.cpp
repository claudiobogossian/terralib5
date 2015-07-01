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
  \file terralib/qt/widgets/connector/sqlite/SQLiteConnectorDialog.cpp

  \brief ....
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../widgets/Exception.h"
#include "Ui_SQLiteConnectorDialogForm.h"
#include "SQLiteConnectorDialog.h"

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

// Qt
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

te::qt::plugins::sqlite::SQLiteConnectorDialog::SQLiteConnectorDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::SQLiteConnectorDialogForm)
{
// add controls
  m_ui->setupUi(this);

// connect signal and slots
  connect(m_ui->m_openPushButton, SIGNAL(pressed()), this, SLOT(openPushButtonPressed()));
  connect(m_ui->m_testPushButton, SIGNAL(pressed()), this, SLOT(testPushButtonPressed()));
  connect(m_ui->m_helpPushButton, SIGNAL(pressed()), this, SLOT(helpPushButtonPressed()));
  connect(m_ui->m_searchDatabaseToolButton, SIGNAL(pressed()), this, SLOT(searchDatabaseToolButtonPressed()));
}

te::qt::plugins::sqlite::SQLiteConnectorDialog::~SQLiteConnectorDialog()
{
}

const te::da::DataSourceInfoPtr& te::qt::plugins::sqlite::SQLiteConnectorDialog::getDataSource() const
{
  return m_datasource;
}

const te::da::DataSourcePtr& te::qt::plugins::sqlite::SQLiteConnectorDialog::getDriver() const
{
  return m_driver;
}

void te::qt::plugins::sqlite::SQLiteConnectorDialog::set(const te::da::DataSourceInfoPtr& ds)
{
  m_datasource = ds;

  if(m_datasource.get() != 0)
  {
    setConnectionInfo(m_datasource->getConnInfo());

    m_ui->m_datasourceTitleLineEdit->setText(QString::fromStdString(m_datasource->getTitle()));

    m_ui->m_datasourceDescriptionTextEdit->setText(QString::fromStdString(m_datasource->getDescription()));
  }
}

void te::qt::plugins::sqlite::SQLiteConnectorDialog::openPushButtonPressed()
{
  try
  {
    if(te::da::DataSourceFactory::find("SQLITE") == 0)
      throw te::qt::widgets::Exception(TR_QT_WIDGETS("Sorry! No data access driver loaded for SQLite data sources!"));

    std::map<std::string, std::string> dsInfo;

    getConnectionInfo(dsInfo);

    m_driver.reset(te::da::DataSourceFactory::make("SQLITE").release());

    m_driver->setConnectionInfo(dsInfo);
    m_driver->open();

    if(m_driver.get() == 0)
      throw te::qt::widgets::Exception(TR_QT_WIDGETS("Could not open SQLite data source due to an unknown error!"));

    QString title = m_ui->m_datasourceTitleLineEdit->text().trimmed();

    if(title.isEmpty())
      title = m_ui->m_fileLineEdit->text().trimmed();

    if(m_datasource.get() == 0)
    {
      m_datasource.reset(new te::da::DataSourceInfo);

      m_datasource->setConnInfo(dsInfo);

      boost::uuids::basic_random_generator<boost::mt19937> gen;
      boost::uuids::uuid u = gen();
      std::string dsId = boost::uuids::to_string(u);

      m_datasource->setId(dsId);
      m_driver->setId(dsId);
      m_datasource->setTitle(title.toUtf8().data());
      m_datasource->setDescription(m_ui->m_datasourceDescriptionTextEdit->toPlainText().trimmed().toUtf8().data());
      m_datasource->setAccessDriver("SQLITE");
      m_datasource->setType("SQLITE");
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
                         tr("Unknown error while opening SQLite database!"));
    return;
  }

  accept();
}

void te::qt::plugins::sqlite::SQLiteConnectorDialog::testPushButtonPressed()
{
  try
  {
    if(te::da::DataSourceFactory::find("SQLITE") == 0)
      throw te::qt::widgets::Exception(TR_QT_WIDGETS("Sorry! No data access driver loaded for SQLite data sources!"));

    std::map<std::string, std::string> dsInfo;

    getConnectionInfo(dsInfo);

    std::auto_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::make("SQLITE"));

    ds->setConnectionInfo(dsInfo);
    ds->open();

    if(ds.get() == 0)
      throw te::qt::widgets::Exception(TR_QT_WIDGETS("Could not open SQLite database!"));

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
                         tr("Unknown error while testing SQLite data source!"));
  }
}

void te::qt::plugins::sqlite::SQLiteConnectorDialog::helpPushButtonPressed()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

void te::qt::plugins::sqlite::SQLiteConnectorDialog::searchDatabaseToolButtonPressed()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open SQLite Database"), QString(""), tr("Database files (*.sqlite *.db);; All Files (*.*)"), 0, QFileDialog::ReadOnly);

  if(fileName.isEmpty())
    return;

  m_ui->m_fileLineEdit->setText(fileName);
}

void te::qt::plugins::sqlite::SQLiteConnectorDialog::getConnectionInfo(std::map<std::string, std::string>& connInfo) const
{
// clear input
  connInfo.clear();

  QString qstr = m_ui->m_fileLineEdit->text().trimmed();

  if(qstr.isEmpty())
    connInfo["SQLITE_FILE"] = ":memory:";
  else
    connInfo["SQLITE_FILE"] = qstr.toUtf8().data();
  
  connInfo["SQLITE_OPEN_READWRITE"] = m_ui->m_rwAccessCheckBox->isChecked() ? "FALSE" : "TRUE";

  connInfo["SQLITE_OPEN_CREATE"] = m_ui->m_createdbCheckBox->isChecked() ? "TRUE" : "FALSE";

  connInfo["SQLITE_CREATE_INTERMEDIATE_DIR"] = m_ui->m_createdbCheckBox->isChecked() ? "TRUE" : "FALSE";

  connInfo["SQLITE_CREATE_SPATIALITE_METADATA_TABLES"] = "TRUE";

  connInfo["SQLITE_HIDE_SPATIAL_METADATA_TABLES"] = "TRUE";
}

void te::qt::plugins::sqlite::SQLiteConnectorDialog::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  std::map<std::string, std::string>::const_iterator it = connInfo.find("SQLITE_FILE");
  std::map<std::string, std::string>::const_iterator itend = connInfo.end();

  if(it != itend)
    m_ui->m_fileLineEdit->setText(QString::fromUtf8(it->second.c_str()));

  it = connInfo.find("SQLITE_OPEN_READWRITE");

  m_ui->m_rwAccessCheckBox->setChecked((it != itend) && (boost::to_upper_copy(it->second) == "TRUE"));

  it = connInfo.find("SQLITE_OPEN_CREATE");

  m_ui->m_createdbCheckBox->setChecked((it != itend) && (boost::to_upper_copy(it->second) == "TRUE"));

  it = connInfo.find("SQLITE_CREATE_INTERMEDIATE_DIR");

  m_ui->m_allowIntermediateDirCheckBox->setChecked((it != itend) && (boost::to_upper_copy(it->second) == "TRUE"));

  it = connInfo.find("SQLITE_CREATE_OGR_METADATA_TABLES");
}
