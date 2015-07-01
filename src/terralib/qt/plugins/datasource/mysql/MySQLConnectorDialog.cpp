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
  \file terralib/qt/widgets/connector/mysql/MySQLConnectorDialog.cpp

  \brief ....
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../widgets/Exception.h"
#include "MySQLConnectorDialog.h"
#include "Ui_MySQLConnectorDialogForm.h"

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

// Qt
#include <QtGui/QMessageBox>

te::qt::plugins::mysql::MySQLConnectorDialog::MySQLConnectorDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::MySQLConnectorDialogForm)
{
// add controls
  m_ui->setupUi(this);
  
// init controls
  m_ui->m_advancedConnectionOptionsGroupBox->hide();

// connect signal and slots
  connect(m_ui->m_openPushButton, SIGNAL(pressed()), this, SLOT(openPushButtonPressed()));
  connect(m_ui->m_testPushButton, SIGNAL(pressed()), this, SLOT(testPushButtonPressed()));
  connect(m_ui->m_helpPushButton, SIGNAL(pressed()), this, SLOT(helpPushButtonPressed()));
  connect(m_ui->m_userNameLineEdit, SIGNAL(editingFinished()), this, SLOT(passwordLineEditEditingFinished()));
  connect(m_ui->m_passwordLineEdit, SIGNAL(editingFinished()), this, SLOT(passwordLineEditEditingFinished()));
  connect(m_ui->m_advancedConnectionOptionsCheckBox, SIGNAL(toggled(bool)), this, SLOT(advancedConnectionOptionsCheckBoxToggled(bool)));
}

te::qt::plugins::mysql::MySQLConnectorDialog::~MySQLConnectorDialog()
{
}

const te::da::DataSourceInfoPtr& te::qt::plugins::mysql::MySQLConnectorDialog::getDataSource() const
{
  return m_datasource;
}

const te::da::DataSourcePtr& te::qt::plugins::mysql::MySQLConnectorDialog::getDriver() const
{
  return m_driver;
}

void te::qt::plugins::mysql::MySQLConnectorDialog::set(const te::da::DataSourceInfoPtr& ds)
{
  m_datasource = ds;

  if(m_datasource.get() != 0)
  {
    setConnectionInfo(m_datasource->getConnInfo());

    m_ui->m_datasourceTitleLineEdit->setText(QString::fromStdString(m_datasource->getTitle()));

    m_ui->m_datasourceDescriptionTextEdit->setText(QString::fromStdString(m_datasource->getDescription()));
  }
}

void te::qt::plugins::mysql::MySQLConnectorDialog::openPushButtonPressed()
{
  try
  {
// check if driver is loaded
    if(te::da::DataSourceFactory::find("MYSQL") == 0)
      throw te::qt::widgets::Exception(TR_QT_WIDGETS("Sorry! No data access driver loaded for MySQL data sources!"));

// get data source connection info based on form data
    std::map<std::string, std::string> dsInfo;

    getConnectionInfo(dsInfo);

// perform connection
    m_driver.reset(te::da::DataSourceFactory::open("MYSQL", dsInfo));

    if(m_driver.get() == 0)
      throw te::qt::widgets::Exception(TR_QT_WIDGETS("Could not open MySQL data source due to an unknown error!"));

    QString title = m_ui->m_datasourceTitleLineEdit->text().trimmed();

    if(title.isEmpty())
      title = m_ui->m_hostNameLineEdit->text().trimmed() + QString::fromStdString("@") + m_ui->m_schemaNameComboBox->currentText().trimmed() + QString::fromStdString("@") + m_ui->m_userNameLineEdit->text().trimmed();

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
      m_datasource->setTitle(title.toStdString());
      m_datasource->setDescription(m_ui->m_datasourceDescriptionTextEdit->toPlainText().trimmed().toStdString());
      m_datasource->setAccessDriver("MYSQL");
      m_datasource->setType("MYSQL");
    }
    else
    {
      m_driver->setId(m_datasource->getId());
      m_datasource->setConnInfo(dsInfo);
      m_datasource->setTitle(title.toStdString());
      m_datasource->setDescription(m_ui->m_datasourceDescriptionTextEdit->toPlainText().trimmed().toStdString());
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
                         tr("Unknown error while opening MySQL database!"));
    return;
  }

  accept();
}

void te::qt::plugins::mysql::MySQLConnectorDialog::testPushButtonPressed()
{
  try
  {
// check if driver is loaded
    if(te::da::DataSourceFactory::find("MYSQL") == 0)
      throw te::qt::widgets::Exception(TR_QT_WIDGETS("Sorry! No data access driver loaded for MySQL data sources!"));

// get data source connection info based on form data
    std::map<std::string, std::string> dsInfo;

    getConnectionInfo(dsInfo);

// perform connection
    std::auto_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::open("MYSQL", dsInfo));

    if(ds.get() == 0)
      throw te::qt::widgets::Exception(TR_QT_WIDGETS("Could not open MySQL database!"));

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
                         tr("Unknown error while testing MySQL data source!"));
  }
}

void te::qt::plugins::mysql::MySQLConnectorDialog::helpPushButtonPressed()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

void te::qt::plugins::mysql::MySQLConnectorDialog::getConnectionInfo(std::map<std::string, std::string>& connInfo) const
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

// get the default table engine for spatial tables
  qstr = m_ui->m_spatialTableEngineComboBox->currentText().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_DEFAULT_ENGINE_FOR_SPATIAL_TABLES"] = qstr.toStdString();

// get the list of tables to be hidden
  qstr = m_ui->m_tablesToHideLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_HIDE_TABLES"] = qstr.toStdString();

// check raster metadata creation
  connInfo["MY_CREATE_TERRALIB_RASTER_METADATA_TABLES"] = m_ui->m_createRasterMetadataCheckBox->isChecked() ? "TRUE" : "FALSE";

// check metadata tables visibility
  connInfo["MY_HIDE_METADATA_TABLES"] = m_ui->m_hideMetadataTablesCheckBox->isChecked() ? "TRUE" : "FALSE";

// check raster table visibility
 connInfo["MY_HIDE_RASTER_TABLES"] = m_ui->m_hideRasterTablesCheckBox->isChecked() ? "TRUE" : "FALSE";

// get MaxPoolSize
  qstr = m_ui->m_maxPoolSizeSpinBox->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_MAX_POOL_SIZE"] = qstr.toStdString();

// get MinPoolSize
  qstr = m_ui->m_minPoolSizeSpinBox->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_MIN_POOL_SIZE"] = qstr.toStdString();

// get idle time
  qstr = m_ui->m_maxIdleTimeSpinBox->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_MAX_IDLE_TIME"] = qstr.toStdString();
  
// get connect_timeout
  qstr = m_ui->m_connectTimeoutSpinBox->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_OPT_CONNECT_TIMEOUT"] = qstr.toStdString();

// get connect_timeout
  qstr = m_ui->m_readTimeoutSpinBox->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_OPT_READ_TIMEOUT"] = qstr.toStdString();

// get connect_timeout
  qstr = m_ui->m_writeTimeoutSpinBox->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_OPT_WRITE_TIMEOUT"] = qstr.toStdString();

// get ssl info
  qstr = m_ui->m_sslKeyLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_SSL_KEY"] = qstr.toStdString();

  qstr = m_ui->m_sslCertLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_SSL_CERT"] = qstr.toStdString();

  qstr = m_ui->m_sslCALineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_SSL_KEY"] = qstr.toStdString();

  qstr = m_ui->m_sslCAPathLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_SSL_CA"] = qstr.toStdString();

  qstr = m_ui->m_sslChiperLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_SSL_CA_PATH"] = qstr.toStdString();

// get socket info
  qstr = m_ui->m_socketLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_SOCKET"] = qstr.toStdString();

// get pipe info
  qstr = m_ui->m_pipeLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["MY_PIPE"] = qstr.toStdString();

// more options
  connInfo["MY_CLIENT_COMPRESS"] = m_ui->m_clientCompressCheckBox->isChecked() ? "TRUE" : "FALSE";
  connInfo["MY_CLIENT_LOCAL_FILES"] = m_ui->m_clientLocalFilesCheckBox->isChecked() ? "TRUE" : "FALSE";
  connInfo["MY_CLIENT_MULTI_STATEMENTS"] = m_ui->m_clientMultiStatementsCheckBox->isChecked() ? "TRUE" : "FALSE";
  connInfo["MY_CLIENT_MULTI_RESULTS"] = m_ui->m_clientMultiResultsCheckBox->isChecked() ? "TRUE" : "FALSE";
  connInfo["MY_OPT_RECONNECT"] = m_ui->m_reconnectCheckBox->isChecked() ? "TRUE" : "FALSE";
  connInfo["MY_CLIENT_IGNORE_SPACE"] = m_ui->m_ignoreSpaceCheckBox->isChecked() ? "TRUE" : "FALSE";
  connInfo["MY_OPT_REPORT_DATA_TRUNCATION"] = m_ui->m_reportDataTruncationCheckBox->isChecked() ? "TRUE" : "FALSE";
  connInfo["MY_CLIENT_NO_SCHEMA"] = m_ui->m_noSchemaCheckBox->isChecked() ? "TRUE" : "FALSE";
  connInfo["MY_CLIENT_INTERACTIVE"] = m_ui->m_clientInterativeCheckBox->isChecked() ? "TRUE" : "FALSE";
}

void te::qt::plugins::mysql::MySQLConnectorDialog::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  std::map<std::string, std::string>::const_iterator it = connInfo.find("MY_HOST_NAME");
  std::map<std::string, std::string>::const_iterator itend = connInfo.end();

  if(it != itend)
    m_ui->m_hostNameLineEdit->setText(QString::fromUtf8(it->second.c_str()));

  it = connInfo.find("MY_PORT");

  if(it != itend)
    m_ui->m_portLineEdit->setText(QString::fromUtf8(it->second.c_str()));

  it = connInfo.find("MY_USER_NAME");

  if(it != itend)
    m_ui->m_userNameLineEdit->setText(QString::fromUtf8(it->second.c_str()));

  it = connInfo.find("MY_PASSWORD");

  if(it != itend)
    m_ui->m_passwordLineEdit->setText(QString::fromUtf8(it->second.c_str()));

  it = connInfo.find("MY_SCHEMA");

  if(it != itend)
  {
    int pos = m_ui->m_schemaNameComboBox->findText(QString::fromUtf8(it->second.c_str()));

    if(pos != -1)
      m_ui->m_schemaNameComboBox->setCurrentIndex(pos);
    else
    {
      m_ui->m_schemaNameComboBox->addItem(QString::fromUtf8(it->second.c_str()));
      m_ui->m_schemaNameComboBox->setCurrentIndex(0);
    }
  }

  it = connInfo.find("MY_OPT_CHARSET_NAME");

  if(it != itend)
  {
    int pos = m_ui->m_charsetComboBox->findText(QString::fromUtf8(it->second.c_str()));

    if(pos != -1)
      m_ui->m_charsetComboBox->setCurrentIndex(pos);
  }

  it = connInfo.find("MY_DEFAULT_ENGINE_FOR_SPATIAL_TABLES");

  if(it != itend)
  {
    int pos = m_ui->m_spatialTableEngineComboBox->findText(QString::fromUtf8(it->second.c_str()));

    if(pos != -1)
      m_ui->m_spatialTableEngineComboBox->setCurrentIndex(pos);
  }

  it = connInfo.find("MY_HIDE_TABLES");

  if(it != itend)
    m_ui->m_tablesToHideLineEdit->setText(QString::fromUtf8(it->second.c_str()));

  it = connInfo.find("MY_CREATE_TERRALIB_RASTER_METADATA_TABLES");

  m_ui->m_createRasterMetadataCheckBox->setChecked((it != itend) && (boost::to_upper_copy(it->second) == "TRUE"));

  it = connInfo.find("MY_HIDE_METADATA_TABLES");

  m_ui->m_hideMetadataTablesCheckBox->setChecked((it != itend) && (boost::to_upper_copy(it->second) == "TRUE"));

  it = connInfo.find("MY_HIDE_RASTER_TABLES");

  m_ui->m_hideRasterTablesCheckBox->setChecked((it != itend) && (boost::to_upper_copy(it->second) == "TRUE"));

  it = connInfo.find("MY_MAX_POOL_SIZE");

  if(it != itend)
    m_ui->m_minPoolSizeSpinBox->setValue(boost::lexical_cast<int>(it->second.c_str()));

  it = connInfo.find("MY_MIN_POOL_SIZE");

  if(it != itend)
    m_ui->m_minPoolSizeSpinBox->setValue(boost::lexical_cast<int>(it->second.c_str()));

  it = connInfo.find("MY_MAX_IDLE_TIME");

  if(it != itend)
    m_ui->m_maxIdleTimeSpinBox->setValue(boost::lexical_cast<int>(it->second.c_str()));

  it = connInfo.find("MY_OPT_CONNECT_TIMEOUT");

  if(it != itend)
    m_ui->m_connectTimeoutSpinBox->setValue(boost::lexical_cast<int>(it->second.c_str()));

  it = connInfo.find("MY_OPT_READ_TIMEOUT");

  if(it != itend)
    m_ui->m_readTimeoutSpinBox->setValue(boost::lexical_cast<int>(it->second.c_str()));

  it = connInfo.find("MY_OPT_WRITE_TIMEOUT");

  if(it != itend)
    m_ui->m_writeTimeoutSpinBox->setValue(boost::lexical_cast<int>(it->second.c_str()));

  it = connInfo.find("MY_SSL_KEY");

  if(it != itend)
    m_ui->m_sslKeyLineEdit->setText(QString::fromUtf8(it->second.c_str()));

  it = connInfo.find("MY_SSL_CERT");

  if(it != itend)
    m_ui->m_sslCertLineEdit->setText(QString::fromUtf8(it->second.c_str()));

  it = connInfo.find("MY_SSL_CA");

  if(it != itend)
    m_ui->m_sslCALineEdit->setText(QString::fromUtf8(it->second.c_str()));

  it = connInfo.find("MY_SSL_CA_PATH");

  if(it != itend)
    m_ui->m_sslCAPathLineEdit->setText(QString::fromUtf8(it->second.c_str()));

  it = connInfo.find("MY_SSL_CIPHER");

  if(it != itend)
    m_ui->m_sslChiperLineEdit->setText(QString::fromUtf8(it->second.c_str()));

  it = connInfo.find("MY_SOCKET");

  if(it != itend)
    m_ui->m_socketLineEdit->setText(QString::fromUtf8(it->second.c_str()));

  it = connInfo.find("MY_PIPE");

  if(it != itend)
    m_ui->m_pipeLineEdit->setText(QString::fromUtf8(it->second.c_str()));

  it = connInfo.find("MY_CLIENT_COMPRESS");

   m_ui->m_clientCompressCheckBox->setChecked((it != itend) && (boost::to_upper_copy(it->second) == "TRUE"));

  it = connInfo.find("MY_CLIENT_LOCAL_FILES");

  if(it != itend)
    m_ui->m_clientLocalFilesCheckBox->setChecked((it != itend) && (boost::to_upper_copy(it->second) == "TRUE"));

  it = connInfo.find("MY_CLIENT_MULTI_STATEMENTS");

  if(it != itend)
    m_ui->m_clientMultiStatementsCheckBox->setChecked((it != itend) && (boost::to_upper_copy(it->second) == "TRUE"));

  it = connInfo.find("MY_CLIENT_MULTI_RESULTS");

  m_ui->m_clientMultiResultsCheckBox->setChecked((it != itend) && (boost::to_upper_copy(it->second) == "TRUE"));

  it = connInfo.find("MY_OPT_RECONNECT");

  m_ui->m_reconnectCheckBox->setChecked((it != itend) && (boost::to_upper_copy(it->second) == "TRUE"));

  it = connInfo.find("MY_CLIENT_IGNORE_SPACE");

  m_ui->m_ignoreSpaceCheckBox->setChecked((it != itend) && (boost::to_upper_copy(it->second) == "TRUE"));

  it = connInfo.find("MY_OPT_REPORT_DATA_TRUNCATION");

  m_ui->m_reportDataTruncationCheckBox->setChecked((it != itend) && (boost::to_upper_copy(it->second) == "TRUE"));

  it = connInfo.find("MY_CLIENT_NO_SCHEMA");

  m_ui->m_noSchemaCheckBox->setChecked((it != itend) && (boost::to_upper_copy(it->second) == "TRUE"));

  it = connInfo.find("MY_CLIENT_INTERACTIVE");

  m_ui->m_clientInterativeCheckBox->setChecked((it != itend) && (boost::to_upper_copy(it->second) == "TRUE"));
}

void te::qt::plugins::mysql::MySQLConnectorDialog::advancedConnectionOptionsCheckBoxToggled(bool t)
{
  m_ui->m_advancedConnectionOptionsGroupBox->setVisible(t);
}

void te::qt::plugins::mysql::MySQLConnectorDialog::passwordLineEditEditingFinished()
{
  if(m_ui->m_userNameLineEdit->text() != "" && m_ui->m_passwordLineEdit->text() != "")
  {
    try
    {
      std::map<std::string, std::string> dsInfo;
      getConnectionInfo(dsInfo);

      // Get DataSources
      std::vector<std::string> dbNames = te::da::DataSource::getDataSources("MYSQL", dsInfo);
      if(!dbNames.empty())
        for(std::size_t i = 0; i < dbNames.size(); i++)
          m_ui->m_schemaNameComboBox->addItem(dbNames[i].c_str());

      // Get Encodings
      m_ui->m_charsetComboBox->addItem("");
      std::vector<std::string> encodings = te::da::DataSource::getEncodings("MYSQL", dsInfo);
      if(!encodings.empty())
      {
        for(std::size_t i = 0; i < encodings.size(); i++)
          m_ui->m_charsetComboBox->addItem(encodings[i].c_str());

        int idx;
#ifdef WIN32
        idx = m_ui->m_charsetComboBox->findText("latin1");
#else
        idx = m_ui->m_charsetComboBox->findText("utf8");
#endif
        m_ui->m_charsetComboBox->setCurrentIndex(idx);
      }
    }
    catch(...)
    {}
  }
}