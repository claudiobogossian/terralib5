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
  \file terralib/qt/widgets/connector/ogr/OGRConnectorDialog.cpp

  \brief ....
*/

// TerraLib
#include <terralib/common/Translator.h>
#include <terralib/dataaccess/datasource/DataSource.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/dataaccess/datasource/DataSourceManager.h>
#include <terralib/qt/widgets/Exception.h>
#include <terralib/qt/widgets/datasource/core/DataSource.h>
#include "ui_OGRConnectorDialogForm.h"
#include "OGRConnectorDialog.h"

// Boost
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

// Qt
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

namespace te_qt = te::qt::widgets;
namespace plg_ogr = qt_af::plugin::ogr;

plg_ogr::OGRConnectorDialog::OGRConnectorDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::OGRConnectorDialogForm)
{
// add controls
  m_ui->setupUi(this);

// connect signal and slots
  connect(m_ui->m_openPushButton, SIGNAL(pressed()), this, SLOT(openPushButtonPressed()));
  connect(m_ui->m_testPushButton, SIGNAL(pressed()), this, SLOT(testPushButtonPressed()));
  connect(m_ui->m_helpPushButton, SIGNAL(pressed()), this, SLOT(helpPushButtonPressed()));
  connect(m_ui->m_searchFeatureToolButton, SIGNAL(pressed()), this, SLOT(searchFeatureToolButtonPressed()));
}

plg_ogr::OGRConnectorDialog::~OGRConnectorDialog()
{
}

const te::qt::widgets::DataSourcePtr& plg_ogr::OGRConnectorDialog::getDataSource() const
{
  return m_datasource;
}

const te::da::DataSourcePtr& plg_ogr::OGRConnectorDialog::getDriver() const
{
  return m_driver;
}

void plg_ogr::OGRConnectorDialog::set(const te_qt::DataSourcePtr& ds)
{
  m_datasource = ds;

  if(m_datasource.get() != 0)
  {
    setConnectionInfo(m_datasource->getConnInfo());

    m_ui->m_datasourceTitleLineEdit->setText(QString::fromStdString(m_datasource->getTitle()));

    m_ui->m_datasourceDescriptionTextEdit->setText(QString::fromStdString(m_datasource->getDescription()));
  }
}

void plg_ogr::OGRConnectorDialog::openPushButtonPressed()
{
  try
  {
// check if driver is loaded
    if(te::da::DataSourceFactory::find("OGR") == 0)
      throw te_qt::Exception(TR_QT_WIDGETS("Sorry! No data access driver loaded for OGR data sources!"));

// get data source connection info based on form data
    std::map<std::string, std::string> dsInfo;

    getConnectionInfo(dsInfo);

// perform connection
    m_driver.reset(te::da::DataSourceFactory::open("OGR", dsInfo));

    if(m_driver.get() == 0)
      throw te_qt::Exception(TR_QT_WIDGETS("Could not open dataset via OGR due to an unknown error!"));

    QString title = m_ui->m_datasourceTitleLineEdit->text().trimmed();

    if(title.isEmpty())
      title = m_ui->m_featureRepoLineEdit->text();

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
      m_datasource->setAccessDriver("OGR");
      m_datasource->setType("OGR");
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
                         tr("Unknown error while opening feature via OGR!"));
    return;
  }

  accept();
}

void plg_ogr::OGRConnectorDialog::testPushButtonPressed()
{
  try
  {
// check if driver is loaded
    if(te::da::DataSourceFactory::find("OGR") == 0)
      throw te_qt::Exception(TR_QT_WIDGETS("Sorry! No data access driver loaded for OGR data sources!"));

// get data source connection info based on form data
    std::map<std::string, std::string> dsInfo;

    getConnectionInfo(dsInfo);

// perform connection
    std::auto_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::open("OGR", dsInfo));

    if(ds.get() == 0)
      throw te_qt::Exception(TR_QT_WIDGETS("Could not open feature repository via OGR!"));

    QMessageBox::information(this,
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
                         tr("Unknown error while testing OGR data source!"));
  }
}

void plg_ogr::OGRConnectorDialog::helpPushButtonPressed()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

void plg_ogr::OGRConnectorDialog::searchFeatureToolButtonPressed()
{
  if(m_ui->m_fileRadioButton->isChecked())
  {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Feature File"), QString(""), tr("Common Formats (*.shp, *.kml, *.geojson, *.gml);; Shapefile (*.shp);; GML (*.gml);; Web Feature Service - WFS (*.xml, *.wfs);; All Files (*.*)"), 0, QFileDialog::ReadOnly);

    if(fileName.isEmpty())
      return;

    m_ui->m_featureRepoLineEdit->setText(fileName);
  }
  else if(m_ui->m_dirRadioButton->isChecked())
  {
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Select a directory with vector files"), QString(""), QFileDialog::ShowDirsOnly);

    if(dirName.isEmpty())
      return;

    m_ui->m_featureRepoLineEdit->setText(dirName);
  }
  else
  {
    QMessageBox::warning(this,
                         tr("TerraLib Qt Components"),
                         tr("Sorry, network files are not implemented yet!\nWe will provide it soon!"));
  }
}

void plg_ogr::OGRConnectorDialog::getConnectionInfo(std::map<std::string, std::string>& connInfo) const
{
  connInfo.clear();

  QString qstr = m_ui->m_featureRepoLineEdit->text().trimmed();
  
  if(qstr.isEmpty())
    throw te_qt::Exception(TR_QT_WIDGETS("Please select a feature file first!"));

  if(boost::filesystem::is_directory(qstr.toUtf8().data()))
    connInfo["URI"] = qstr.toUtf8().data();
  else
    connInfo["SOURCE"] = qstr.toUtf8().data();
}

void plg_ogr::OGRConnectorDialog::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  std::map<std::string, std::string>::const_iterator it = connInfo.find("URI");
  std::map<std::string, std::string>::const_iterator itend = connInfo.end();

  if(it != itend)
  {
    m_ui->m_featureRepoLineEdit->setText(QString::fromUtf8(it->second.c_str()));
    m_ui->m_dirRadioButton->setChecked(true);
    return;
  }

  it = connInfo.find("SOURCE");

  if(it != itend)
  {
    m_ui->m_featureRepoLineEdit->setText(QString::fromUtf8(it->second.c_str()));
    m_ui->m_fileRadioButton->setChecked(true);
  }
}

