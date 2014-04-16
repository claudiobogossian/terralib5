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
  \file terralib/qt/plugins/datasource/pgis/PostGISConnectorDialog.cpp

  \brief A dialog window for showing the PostGIS connector widget.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../widgets/Exception.h"
#include "PostGISConnectorDialog.h"
#include "ui_PostGISConnectorDialogForm.h"

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

// Qt
#include <QtGui/QMessageBox>

te::qt::plugins::pgis::PostGISConnectorDialog::PostGISConnectorDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::PostGISConnectorDialogForm)
{
// add controls
  m_ui->setupUi(this);

// init controls
  m_ui->m_advancedConnectionOptionsGroupBox->hide();
  
// connect signal and slots
  connect(m_ui->m_openPushButton, SIGNAL(pressed()), this, SLOT(openPushButtonPressed()));
  connect(m_ui->m_testPushButton, SIGNAL(pressed()), this, SLOT(testPushButtonPressed()));
  connect(m_ui->m_userNameLineEdit, SIGNAL(editingFinished()), this, SLOT(passwordLineEditEditingFinished()));
  connect(m_ui->m_passwordLineEdit, SIGNAL(editingFinished()), this, SLOT(passwordLineEditEditingFinished()));
  connect(m_ui->m_advancedConnectionOptionsCheckBox, SIGNAL(toggled(bool)), this, SLOT(advancedConnectionOptionsCheckBoxToggled(bool)));

  m_ui->m_portLineEdit->setValidator(new QIntValidator(0, 9999, this));

  m_ui->m_helpPushButton->setNameSpace("dpi.inpe.br.plugins");
  m_ui->m_helpPushButton->setPageReference("plugins/pgis/pgis.html");
}

te::qt::plugins::pgis::PostGISConnectorDialog::~PostGISConnectorDialog()
{
}

const te::da::DataSourceInfoPtr& te::qt::plugins::pgis::PostGISConnectorDialog::getDataSource() const
{
  return m_datasource;
}

const te::da::DataSourcePtr& te::qt::plugins::pgis::PostGISConnectorDialog::getDriver() const
{
  return m_driver;
}

void te::qt::plugins::pgis::PostGISConnectorDialog::set(const te::da::DataSourceInfoPtr& ds)
{
  m_datasource = ds;

  if(m_datasource.get() != 0)
  {
    setConnectionInfo(m_datasource->getConnInfo());

    m_ui->m_datasourceTitleLineEdit->setText(QString::fromStdString(m_datasource->getTitle()));

    m_ui->m_datasourceDescriptionTextEdit->setText(QString::fromStdString(m_datasource->getDescription()));
  }
}

void te::qt::plugins::pgis::PostGISConnectorDialog::openPushButtonPressed()
{
  try
  {
    // Check if driver is loaded
    if(te::da::DataSourceFactory::find("POSTGIS") == 0)
      throw te::qt::widgets::Exception(TR_QT_WIDGETS("Sorry! No data access driver loaded for PostgreSQL + PostGIS data sources!"));

    // Get the data source connection info based on form data
    std::map<std::string, std::string> dsInfo;

    getConnectionInfo(dsInfo);

    // Perform connection
    std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("POSTGIS");
    ds->setConnectionInfo(dsInfo);
    ds->open();
    m_driver.reset(ds.release());

    if(m_driver.get() == 0)
      throw te::qt::widgets::Exception(TR_QT_WIDGETS("Could not open PostgreSQL + PostGIS data source due to an unknown error!"));

    QString title = m_ui->m_datasourceTitleLineEdit->text().trimmed();

    if(title.isEmpty())
      title = m_ui->m_hostNameLineEdit->text().trimmed() + QString::fromStdString("@") + m_ui->m_databaseComboBox->currentText().trimmed() + QString::fromStdString("@") + m_ui->m_userNameLineEdit->text().trimmed();

    if(m_datasource.get() == 0)
    {
      // Create a new data source based on the form data
      m_datasource.reset(new te::da::DataSourceInfo);

      m_datasource->setConnInfo(dsInfo);

      boost::uuids::basic_random_generator<boost::mt19937> gen;
      boost::uuids::uuid u = gen();
      std::string dsId = boost::uuids::to_string(u);

      m_datasource->setId(dsId);
      m_driver->setId(dsId);
      m_datasource->setTitle(title.toStdString());
      m_datasource->setDescription(m_ui->m_datasourceDescriptionTextEdit->toPlainText().trimmed().toStdString());
      m_datasource->setAccessDriver("POSTGIS");
      m_datasource->setType("POSTGIS");
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
                         tr("Unknown error while opening PostgreSQL database!"));
    return;
  }

  accept();
}

void te::qt::plugins::pgis::PostGISConnectorDialog::testPushButtonPressed()
{
  try
  {
    // Check if driver is loaded
    if(te::da::DataSourceFactory::find("POSTGIS") == 0)
      throw te::qt::widgets::Exception(TR_QT_WIDGETS("Sorry! No data access driver loaded for PostgreSQL + PostGIS data sources!"));

    // Get the data source connection info based on form data
    std::map<std::string, std::string> dsInfo;

    getConnectionInfo(dsInfo);

    // Perform connection
    std::auto_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::make("POSTGIS"));

    if(ds.get() == 0)
      throw te::qt::widgets::Exception(TR_QT_WIDGETS("Could not open PostgreSQL + PostGIS database!"));

    ds->setConnectionInfo(dsInfo);
    ds->open();

    QMessageBox::information(this,
                             tr("TerraLib Qt Components"),
                             tr("Data source is ok!"));

    ds->close();
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
                         tr("Unknown error while testing PostgreSQL + PostGIS data source!"));
  }
}

void te::qt::plugins::pgis::PostGISConnectorDialog::getConnectionInfo(std::map<std::string, std::string>& connInfo) const
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

// get dbname
  qstr = m_ui->m_databaseComboBox->currentText().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["PG_DB_NAME"] = qstr.toStdString();

// get client encoding
  qstr = m_ui->m_clientEncodingComboBox->currentText().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["PG_CLIENT_ENCODING"] = qstr.toStdString();
  
// get user
  qstr = m_ui->m_userNameLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["PG_USER"] = qstr.toStdString();

// get password
  qstr = m_ui->m_passwordLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["PG_PASSWORD"] = qstr.toStdString();

// get table info
  qstr = m_ui->m_tablesToHideLineEdit->text().trimmed();

  if(!qstr.isEmpty())
    connInfo["PG_HIDE_TABLES"] = qstr.toStdString();

  connInfo["PG_HIDE_SPATIAL_METADATA_TABLES"] = m_ui->m_hideMetadataTablesCheckBox->isChecked() ? "TRUE" : "FALSE";

  connInfo["PG_HIDE_RASTER_TABLES"] = m_ui->m_hideRasterTablesCheckBox->isChecked() ? "TRUE" : "FALSE";

// get connect_timeout
  qstr = m_ui->m_connectTimeoutSpinBox->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["PG_CONNECT_TIMEOUT"] = qstr.toStdString();

// get MinPoolSize
  qstr = m_ui->m_minPoolSizeSpinBox->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["PG_MIN_POOL_SIZE"] = qstr.toStdString();

// get MaxPoolSize
  qstr = m_ui->m_maxPoolSizeSpinBox->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["PG_MAX_POOL_SIZE"] = qstr.toStdString();

// get options
  qstr = m_ui->m_optionsLineEdit->text().trimmed();
  
  if(!qstr.isEmpty())
    connInfo["PG_OPTIONS"] = qstr.toStdString();
}

void te::qt::plugins::pgis::PostGISConnectorDialog::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  std::map<std::string, std::string>::const_iterator it = connInfo.find("PG_HOST");
  std::map<std::string, std::string>::const_iterator itend = connInfo.end();

  if(it != itend)
    m_ui->m_hostNameLineEdit->setText(QString::fromStdString(it->second));

  it = connInfo.find("PG_HOST_ADDR");

  if(it != itend)
    m_ui->m_hostNameLineEdit->setText(QString::fromStdString(it->second));

  it = connInfo.find("PG_PORT");

  if(it != itend)
    m_ui->m_portLineEdit->setText(QString::fromStdString(it->second));

  it = connInfo.find("PG_DB_NAME");

  if(it != itend)
  {
    int pos = m_ui->m_databaseComboBox->findText(QString::fromStdString(it->second));

    if(pos != -1)
      m_ui->m_databaseComboBox->setCurrentIndex(pos);
    else
    {
      m_ui->m_databaseComboBox->addItem(QString::fromStdString(it->second));
      m_ui->m_databaseComboBox->setCurrentIndex(0);
    }
  }

  it = connInfo.find("PG_CLIENT_ENCODING");

  if(it != itend)
  {
    int pos = m_ui->m_clientEncodingComboBox->findText(QString::fromStdString(it->second));

    if(pos != -1)
      m_ui->m_clientEncodingComboBox->setCurrentIndex(pos);
    else
    {
      m_ui->m_clientEncodingComboBox->addItem(QString::fromStdString(it->second));
      m_ui->m_clientEncodingComboBox->setCurrentIndex(0);
    }
  }

  it = connInfo.find("PG_USER");

  if(it != itend)
    m_ui->m_userNameLineEdit->setText(QString::fromStdString(it->second));

  it = connInfo.find("PG_PASSWORD");

  if(it != itend)
    m_ui->m_passwordLineEdit->setText(QString::fromStdString(it->second));

  it = connInfo.find("PG_CONNECT_TIMEOUT");

  if(it != itend)
    m_ui->m_connectTimeoutSpinBox->setValue(boost::lexical_cast<int>(it->second.c_str()));

  it = connInfo.find("PG_MIN_POOL_SIZE");

  if(it != itend)
    m_ui->m_minPoolSizeSpinBox->setValue(boost::lexical_cast<int>(it->second.c_str()));

  it = connInfo.find("PG_MAX_POOL_SIZE");

  if(it != itend)
    m_ui->m_maxPoolSizeSpinBox->setValue(boost::lexical_cast<int>(it->second.c_str()));

  it = connInfo.find("PG_OPTIONS");

  if(it != itend)
    m_ui->m_optionsLineEdit->setText(QString::fromStdString(it->second));

  it = connInfo.find("PG_HIDE_SPATIAL_METADATA_TABLES");

  m_ui->m_hideMetadataTablesCheckBox->setChecked((it != itend) && (boost::to_upper_copy(it->second) == "TRUE"));

  it = connInfo.find("PG_HIDE_RASTER_TABLES");

  m_ui->m_hideRasterTablesCheckBox->setChecked((it != itend) && (boost::to_upper_copy(it->second) == "TRUE"));

  it = connInfo.find("PG_HIDE_TABLES");

  if(it != itend)
    m_ui->m_tablesToHideLineEdit->setText(QString::fromStdString(it->second));
}

void te::qt::plugins::pgis::PostGISConnectorDialog::advancedConnectionOptionsCheckBoxToggled(bool t)
{
  m_ui->m_advancedConnectionOptionsGroupBox->setVisible(t);
}

void te::qt::plugins::pgis::PostGISConnectorDialog::passwordLineEditEditingFinished()
{
  if(m_ui->m_userNameLineEdit->text() != "" || m_ui->m_passwordLineEdit->text() != "")
  {
    try{
      std::map<std::string, std::string> dsInfo;
      getConnectionInfo(dsInfo);

      // Get DataSources
      std::vector<std::string> dbNames = te::da::DataSource::getDataSourceNames("POSTGIS", dsInfo);
      if(!dbNames.empty())
      {
        m_ui->m_databaseComboBox->clear();
        for(std::size_t i = 0; i < dbNames.size(); i++)
          m_ui->m_databaseComboBox->addItem(dbNames[i].c_str());
      }

      // Get Encodings
      m_ui->m_clientEncodingComboBox->clear();
      m_ui->m_clientEncodingComboBox->addItem("");
      std::vector<te::common::CharEncoding> encodings = te::da::DataSource::getEncodings("POSTGIS", dsInfo);
      if(!encodings.empty())
      {
        for(std::size_t i = 0; i < encodings.size(); i++)
          m_ui->m_clientEncodingComboBox->addItem(te::common::CharEncodingConv::getCharEncodingName(encodings[i]).c_str());

        int idx;
#ifdef WIN32
        idx = m_ui->m_clientEncodingComboBox->findText("CP1252");
#else
        idx = m_ui->m_clientEncodingComboBox->findText("UTF-8");
#endif
        m_ui->m_clientEncodingComboBox->setCurrentIndex(idx);
      }
    }
    catch(...)
    {}
  }
}