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
  \file terralib/qt/plugins/datasource/gdal/GDALConnectorDialog.cpp

  \brief A dialog window for showing the GDAL connector widget.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../af/Utils.h"
#include "../../../widgets/Exception.h"
#include "../../../widgets/Utils.h"
#include "GDALConnectorDialog.h"
#include "ui_GDALConnectorDialogForm.h"

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

// Qt
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>

te::qt::plugins::gdal::GDALConnectorDialog::GDALConnectorDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::GDALConnectorDialogForm)
{
// add controls
  m_ui->setupUi(this);

// connect signal and slots
  connect(m_ui->m_openPushButton, SIGNAL(pressed()), this, SLOT(openPushButtonPressed()));
  connect(m_ui->m_testPushButton, SIGNAL(pressed()), this, SLOT(testPushButtonPressed()));
  connect(m_ui->m_searchDatasetToolButton, SIGNAL(pressed()), this, SLOT(searchDatasetToolButtonPressed()));

  m_ui->m_helpPushButton->setNameSpace("dpi.inpe.br.plugins");
  m_ui->m_helpPushButton->setPageReference("plugins/gdal/gdal.html");
}

te::qt::plugins::gdal::GDALConnectorDialog::~GDALConnectorDialog()
{
}

const te::da::DataSourceInfoPtr& te::qt::plugins::gdal::GDALConnectorDialog::getDataSource() const
{
  return m_datasource;
}

const te::da::DataSourcePtr& te::qt::plugins::gdal::GDALConnectorDialog::getDriver() const
{
  return m_driver;
}

void te::qt::plugins::gdal::GDALConnectorDialog::set(const te::da::DataSourceInfoPtr& ds)
{
  m_datasource = ds;

  if(m_datasource.get() != 0)
  {
    setConnectionInfo(m_datasource->getConnInfo());

    m_ui->m_datasourceTitleLineEdit->setText(QString::fromStdString(m_datasource->getTitle()));

    m_ui->m_datasourceDescriptionTextEdit->setText(QString::fromStdString(m_datasource->getDescription()));
  }
}

void te::qt::plugins::gdal::GDALConnectorDialog::openPushButtonPressed()
{
  try
  {
// check if driver is loaded
    if(te::da::DataSourceFactory::find("GDAL") == 0)
      throw te::qt::widgets::Exception(TE_TR("Sorry! No data access driver loaded for GDAL data sources!"));

// get data source connection info based on form data
    std::map<std::string, std::string> dsInfo;

    getConnectionInfo(dsInfo);

// perform connection
    //m_driver.reset(te::da::DataSourceFactory::open("GDAL", dsInfo));
    std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("GDAL");
    ds->setConnectionInfo(dsInfo);
    ds->open();
    m_driver.reset(ds.release());

    if(m_driver.get() == 0)
      throw te::qt::widgets::Exception(TE_TR("Could not open dataset via GDAL due to an unknown error!"));

    QString title = m_ui->m_datasourceTitleLineEdit->text().trimmed();

    if(title.isEmpty())
      title = m_ui->m_datasetLineEdit->text();

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
      m_datasource->setAccessDriver("GDAL");
      m_datasource->setType("GDAL");
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
                         tr("Unknown error while opening dataset via GDAL!"));
    return;
  }

  accept();
}

void te::qt::plugins::gdal::GDALConnectorDialog::testPushButtonPressed()
{
  try
  {
// check if driver is loaded
    if(te::da::DataSourceFactory::find("GDAL") == 0)
      throw te::qt::widgets::Exception(TE_TR("Sorry! No data access driver loaded for GDAL data sources!"));

// get data source connection info based on form data
    std::map<std::string, std::string> dsInfo;

    getConnectionInfo(dsInfo);

// perform connection
    //std::auto_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::open("GDAL", dsInfo));
    std::auto_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::make("GDAL"));
    ds->setConnectionInfo(dsInfo);
    ds->open();

    if(ds.get() == 0)
      throw te::qt::widgets::Exception(TE_TR("Could not open dataset via GDAL!"));

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
                         tr("Unknown error while testing GDAL data source!"));
  }
}

void te::qt::plugins::gdal::GDALConnectorDialog::searchDatasetToolButtonPressed()
{
  if(m_ui->m_fileRadioButton->isChecked())
  {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Geo Spatial File"), te::qt::widgets::GetFilePathFromSettings("raster"), 
      tr("Image File (*.png *.jpg *.jpeg *.tif *.tiff *.geotif *.geotiff);; Web Map Service - WMS (*.xml *.wms);; Web Coverage Service - WCS (*.xml *.wcs);; All Files (*.*)"), 0, QFileDialog::ReadOnly);

    if(fileName.isEmpty())
      return;

    QFileInfo info(fileName);

    te::qt::widgets::AddFilePathToSettings(info.absolutePath(), "raster");

    m_ui->m_datasetLineEdit->setText(fileName);
  }
  else if(m_ui->m_dirRadioButton->isChecked())
  {
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Select a directory with image files"), te::qt::widgets::GetFilePathFromSettings("raster"), QFileDialog::ShowDirsOnly);

    if(dirName.isEmpty())
      return;

    te::qt::widgets::AddFilePathToSettings(dirName, "raster");

    m_ui->m_datasetLineEdit->setText(dirName);
  }
  else
  {
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr("Sorry, network files are not implemented yet!\nWe will provide it soon!"));
  }
}

void te::qt::plugins::gdal::GDALConnectorDialog::getConnectionInfo(std::map<std::string, std::string>& connInfo) const
{
  connInfo.clear();

  QString qstr = m_ui->m_datasetLineEdit->text().trimmed();
  
  if(qstr.isEmpty())
    throw te::qt::widgets::Exception(TE_TR("Please select a dataset first!"));

  if(boost::filesystem::is_directory(qstr.toUtf8().data()))
    connInfo["URI"] = qstr.toUtf8().data();
  else
    connInfo["SOURCE"] = qstr.toUtf8().data();
}

void te::qt::plugins::gdal::GDALConnectorDialog::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  std::map<std::string, std::string>::const_iterator it = connInfo.find("URI");
  std::map<std::string, std::string>::const_iterator itend = connInfo.end();

  if(it != itend)
  {
    m_ui->m_datasetLineEdit->setText(QString::fromUtf8(it->second.c_str()));
    m_ui->m_dirRadioButton->setChecked(true);
    return;
  }

  it = connInfo.find("SOURCE");

  if(it != itend)
  {
    m_ui->m_datasetLineEdit->setText(QString::fromUtf8(it->second.c_str()));
    m_ui->m_fileRadioButton->setChecked(true);
  }
}

