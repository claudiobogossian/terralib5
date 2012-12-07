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
  \file terralib/qt/widgets/connector/gdal/GDALConnectorDialog.cpp

  \brief ....
*/

// TerraLib
#include <terralib/common/Translator.h>
#include <terralib/dataaccess/datasource/DataSource.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/dataaccess/datasource/DataSourceManager.h>
#include <terralib/qt/widgets/Exception.h>
#include <terralib/qt/widgets/datasource/core/DataSource.h>
#include "ui_GDALConnectorDialogForm.h"
#include "GDALConnectorDialog.h"

// Boost
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

// Qt
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

namespace te_qt = te::qt::widgets;
namespace plg_gdal = qt_af::plugin::gdal;

plg_gdal::GDALConnectorDialog::GDALConnectorDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::GDALConnectorDialogForm)
{
// add controls
  m_ui->setupUi(this);

// connect signal and slots
  connect(m_ui->m_openPushButton, SIGNAL(pressed()), this, SLOT(openPushButtonPressed()));
  connect(m_ui->m_testPushButton, SIGNAL(pressed()), this, SLOT(testPushButtonPressed()));
  connect(m_ui->m_helpPushButton, SIGNAL(pressed()), this, SLOT(helpPushButtonPressed()));
  connect(m_ui->m_searchDatasetToolButton, SIGNAL(pressed()), this, SLOT(searchDatasetToolButtonPressed()));
}

plg_gdal::GDALConnectorDialog::~GDALConnectorDialog()
{
}

const te_qt::DataSourcePtr& plg_gdal::GDALConnectorDialog::getDataSource() const
{
  return m_datasource;
}

const te::da::DataSourcePtr& plg_gdal::GDALConnectorDialog::getDriver() const
{
  return m_driver;
}

void plg_gdal::GDALConnectorDialog::set(const te_qt::DataSourcePtr& ds)
{
  m_datasource = ds;

  if(m_datasource.get() != 0)
  {
    setConnectionInfo(m_datasource->getConnInfo());

    m_ui->m_datasourceTitleLineEdit->setText(QString::fromStdString(m_datasource->getTitle()));

    m_ui->m_datasourceDescriptionTextEdit->setText(QString::fromStdString(m_datasource->getDescription()));
  }
}

void plg_gdal::GDALConnectorDialog::openPushButtonPressed()
{
  try
  {
// check if driver is loaded
    if(te::da::DataSourceFactory::find("GDAL") == 0)
      throw te_qt::Exception(TR_QT_WIDGETS("Sorry! No data access driver loaded for GDAL data sources!"));

// get data source connection info based on form data
    std::map<std::string, std::string> dsInfo;

    getConnectionInfo(dsInfo);

// perform connection
    m_driver.reset(te::da::DataSourceFactory::open("GDAL", dsInfo));

    if(m_driver.get() == 0)
      throw te_qt::Exception(TR_QT_WIDGETS("Could not open dataset via GDAL due to an unknown error!"));

    QString title = m_ui->m_datasourceTitleLineEdit->text().trimmed();

    if(title.isEmpty())
      title = m_ui->m_datasetLineEdit->text();

    if(m_datasource.get() == 0)
    {
// create a new data source based on form data
      m_datasource.reset(new te_qt::DataSource);

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

void plg_gdal::GDALConnectorDialog::testPushButtonPressed()
{
  try
  {
// check if driver is loaded
    if(te::da::DataSourceFactory::find("GDAL") == 0)
      throw te_qt::Exception(TR_QT_WIDGETS("Sorry! No data access driver loaded for GDAL data sources!"));

// get data source connection info based on form data
    std::map<std::string, std::string> dsInfo;

    getConnectionInfo(dsInfo);

// perform connection
    std::auto_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::open("GDAL", dsInfo));

    if(ds.get() == 0)
      throw te_qt::Exception(TR_QT_WIDGETS("Could not open dataset via GDAL!"));

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

void plg_gdal::GDALConnectorDialog::helpPushButtonPressed()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

void plg_gdal::GDALConnectorDialog::searchDatasetToolButtonPressed()
{
  if(m_ui->m_fileRadioButton->isChecked())
  {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Geo Spatial File"), QString(""), tr("Image File (*.png, *.jpg, *.jpeg, *.tif, *.tiff, *.geotif, *.geotiff);; Web Map Service - WMS (*.xml, *.wms);; Web Coverage Service - WCS (*.xml, *.wcs);; All Files (*.*)"), 0, QFileDialog::ReadOnly);

    if(fileName.isEmpty())
      return;

    m_ui->m_datasetLineEdit->setText(fileName);
  }
  else if(m_ui->m_dirRadioButton->isChecked())
  {
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Select a directory with image files"), QString(""), QFileDialog::ShowDirsOnly);

    if(dirName.isEmpty())
      return;

    m_ui->m_datasetLineEdit->setText(dirName);
  }
  else
  {
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr("Sorry, network files are not implemented yet!\nWe will provide it soon!"));
  }
}

void plg_gdal::GDALConnectorDialog::getConnectionInfo(std::map<std::string, std::string>& connInfo) const
{
  connInfo.clear();

  QString qstr = m_ui->m_datasetLineEdit->text().trimmed();
  
  if(qstr.isEmpty())
    throw te_qt::Exception(TR_QT_WIDGETS("Please select a dataset first!"));

  if(boost::filesystem::is_directory(qstr.toUtf8().data()))
    connInfo["URI"] = qstr.toUtf8().data();
  else
    connInfo["SOURCE"] = qstr.toUtf8().data();
}

void plg_gdal::GDALConnectorDialog::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
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

