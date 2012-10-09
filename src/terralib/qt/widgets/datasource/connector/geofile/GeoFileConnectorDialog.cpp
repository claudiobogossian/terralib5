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
  \file terralib/qt/widgets/connector/geofile/GeoFileConnectorDialog.cpp

  \brief ....
*/

// TerraLib
#include "../../../../../common/Translator.h"
#include "../../../../../dataaccess/datasource/DataSource.h"
#include "../../../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../Exception.h"
#include "../../core/DataSource.h"
#include "ui/GeoFileConnectorDialogForm.h"
#include "GeoFileConnectorDialog.h"

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/filesystem.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

// Qt
#include <QtGui/QFileDialog>
#include <QtGui/QIcon>
#include <QtGui/QMessageBox>


te::qt::widgets::GeoFileConnectorDialog::GeoFileConnectorDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::GeoFileConnectorDialogForm)
{
// add controls
  m_ui->setupUi(this);

// add button icons
  m_ui->m_addDatasourceToolButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_removeDatasourceToolButton->setIcon(QIcon::fromTheme("list-remove"));
  m_ui->m_updateDatasourceToolButton->setIcon(QIcon::fromTheme("view-refresh"));

// connect signal and slots
  connect(m_ui->m_openPushButton, SIGNAL(pressed()), this, SLOT(openPushButtonPressed()));
  connect(m_ui->m_testPushButton, SIGNAL(pressed()), this, SLOT(testPushButtonPressed()));
  connect(m_ui->m_helpPushButton, SIGNAL(pressed()), this, SLOT(helpPushButtonPressed()));
  connect(m_ui->m_fileSearchToolButton, SIGNAL(pressed()), this, SLOT(fileSearchToolButtonPressed()));
  connect(m_ui->m_addDatasourceToolButton, SIGNAL(pressed()), this, SLOT(addDatasourceToolButtonPressed()));
  connect(m_ui->m_removeDatasourceToolButton, SIGNAL(pressed()), this, SLOT(removeDatasourceToolButtonPressed()));
  connect(m_ui->m_updateDatasourceToolButton, SIGNAL(pressed()), this, SLOT(updateDatasourceToolButtonPressed()));

  m_ui->m_openPushButton->setEnabled(false);
}

te::qt::widgets::GeoFileConnectorDialog::~GeoFileConnectorDialog()
{
}

const std::list<te::qt::widgets::DataSourcePtr>& te::qt::widgets::GeoFileConnectorDialog::getDataSources() const
{
  return m_datasources;
}

void te::qt::widgets::GeoFileConnectorDialog::set(const std::list<DataSourcePtr>& datasources)
{
  m_datasources = datasources;

  m_ui->m_datasourceListWidget->clear();

  for(std::list<DataSourcePtr>::iterator it = m_datasources.begin(); it != m_datasources.end(); ++it)
  {
    QString id = QString::fromStdString((*it)->getId());
    QString title = QString::fromStdString((*it)->getTitle());

    QListWidgetItem* item = new QListWidgetItem(title);
    item->setData(Qt::UserRole, QVariant(id));

    m_ui->m_datasourceListWidget->addItem(item);
  }

  if(m_ui->m_datasourceListWidget->count() != 0)
  {
    QListWidgetItem* item = m_ui->m_datasourceListWidget->item(0);
    m_ui->m_datasourceListWidget->setCurrentItem(item);
    dataSourcePressed(item);

    m_ui->m_openPushButton->setEnabled(true);
  }
}

void te::qt::widgets::GeoFileConnectorDialog::openPushButtonPressed()
{
  accept();
}

void te::qt::widgets::GeoFileConnectorDialog::testPushButtonPressed()
{
  try
  {
    test();

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
                         tr("Unknown error when probing data source!"));
  }
}

void te::qt::widgets::GeoFileConnectorDialog::helpPushButtonPressed()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

void te::qt::widgets::GeoFileConnectorDialog::fileSearchToolButtonPressed()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Geo Spatial File"), QString(""), tr("Image File (*.png, *.jpg, *.jpeg, *.tif, *.tiff, *.geotif, *.geotiff);; Vector File (*.shp, *.geojson, *.kml);; All Files (*.*)"), 0, QFileDialog::ReadOnly);

  if(fileName.isEmpty())
    return;

  m_ui->m_datasourceLineEdit->setText(fileName);
}

void te::qt::widgets::GeoFileConnectorDialog::addDatasourceToolButtonPressed()
{
  try
  {
    te::da::DataSourcePtr driver = test();

    assert(driver.get());

    if(driver.get() != 0)
    {
      DataSourcePtr nds(new DataSource);

      nds->setConnInfo(driver->getConnectionInfo());

      boost::uuids::basic_random_generator<boost::mt19937> gen;
      boost::uuids::uuid u = gen();
      std::string dsId = boost::uuids::to_string(u);

      QString title = m_ui->m_datasourceTitleLineEdit->text().trimmed();

      if(title.isEmpty())
        title = m_ui->m_datasourceLineEdit->text().trimmed();

      nds->setId(dsId);
      nds->setTitle(title.toUtf8().data());
      nds->setDescription(m_ui->m_datasourceDescriptionTextEdit->toPlainText().trimmed().toUtf8().data());
      nds->setAccessDriver(driver->getType());
      nds->setType("GEOFILE");

      m_datasources.push_back(nds);

      QString id = QString::fromStdString(nds->getId());

      QListWidgetItem* item = new QListWidgetItem(title);
      item->setData(Qt::UserRole, QVariant(id));

      m_ui->m_datasourceListWidget->addItem(item);

      m_ui->m_openPushButton->setEnabled(true);
    }
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
                         tr("Unknown error when probing data source!"));
  }
}

void te::qt::widgets::GeoFileConnectorDialog::removeDatasourceToolButtonPressed()
{
  QListWidgetItem* item = m_ui->m_datasourceListWidget->currentItem();

  if(item == 0)
    return;

  QVariant udata = item->data(Qt::UserRole);

  QString id = udata.toString();

  if(id.isEmpty())
    return;

  std::string dsId = id.toStdString();

  std::list<DataSourcePtr>::iterator it = std::find_if(m_datasources.begin(), m_datasources.end(), FindById(dsId));

  if(it == m_datasources.end())
    return;

  m_datasources.erase(it);

  int itemRow = m_ui->m_datasourceListWidget->row(item);

  item = m_ui->m_datasourceListWidget->takeItem(itemRow);

  delete item;

  if(m_datasources.empty())
    m_ui->m_openPushButton->setEnabled(false);
}

void te::qt::widgets::GeoFileConnectorDialog::updateDatasourceToolButtonPressed()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

void te::qt::widgets::GeoFileConnectorDialog::dataSourcePressed(QListWidgetItem* item)
{
  m_ui->m_datasourceLineEdit->setText("");
  m_ui->m_datasourceTitleLineEdit->setText("");
  m_ui->m_datasourceDescriptionTextEdit->setText("");

  if(item == 0)
    return;

  QVariant udata = item->data(Qt::UserRole);

  QString id = udata.toString();

  if(id.isEmpty())
    return;

  std::string dsId = id.toStdString();

  std::list<DataSourcePtr>::iterator it = std::find_if(m_datasources.begin(), m_datasources.end(), FindById(dsId));

  if(it == m_datasources.end())
    return;

  std::map<std::string, std::string>::const_iterator itdsinfo = (*it)->getConnInfo().find("SOURCE");  // this can change!

  if(itdsinfo == (*it)->getConnInfo().end())
    return;

  // in the future we must check the underlying driver!
  //if((*it)->getAccessDriver() == "OGR")
  //{
  //}
  //else if((*it)->getAccessDriver() == "GDAL")
  //{
  //}

  m_ui->m_datasourceLineEdit->setText(QString::fromStdString(itdsinfo->second));
  m_ui->m_datasourceTitleLineEdit->setText(QString::fromStdString((*it)->getTitle()));
  m_ui->m_datasourceDescriptionTextEdit->setText(QString::fromStdString((*it)->getDescription()));
}

te::da::DataSourcePtr te::qt::widgets::GeoFileConnectorDialog::test()
{
  std::map<std::string, std::string> dsInfo;

// check data acccess driver to be used
  std::string driverType;

  boost::filesystem::path fp = m_ui->m_datasourceLineEdit->text().trimmed().toUtf8().data();

  std::string fileExtension = boost::to_upper_copy(fp.extension().string());

  if((fileExtension == ".TIF") || (fileExtension == ".TIFF") || (fileExtension == ".GEOTIF") || (fileExtension == ".GEOTIFF") ||
     (fileExtension == ".PNG") || (fileExtension == ".JPG") || (fileExtension == ".JPEG"))
  {
    if(te::da::DataSourceFactory::find("GDAL") == 0)
      throw Exception(TR_QT_WIDGETS("Sorry! No data access driver loaded for GDAL data sources!"));

    dsInfo["SOURCE"] = m_ui->m_datasourceLineEdit->text().trimmed().toStdString();

    driverType = "GDAL";
  }
  else if((fileExtension == ".SHP") || (fileExtension == ".GEOJSON") || (fileExtension == ".KML"))
  {
    if(te::da::DataSourceFactory::find("OGR") == 0)
      throw Exception(TR_QT_WIDGETS("Sorry! No data access driver loaded for OGR data sources!"));

    dsInfo["SOURCE"] = m_ui->m_datasourceLineEdit->text().trimmed().toStdString();

    driverType = "OGR";
  }
  else
  {
    throw Exception(TR_QT_WIDGETS("Sorry! Unknown data access driver for the selected file!"));
  }

  te::da::DataSourcePtr ds(te::da::DataSourceFactory::open(driverType, dsInfo));

  if(ds.get() == 0)
    throw Exception(TR_QT_WIDGETS("Could not open data source due to an unexpected error!"));

  return ds;
}

bool te::qt::widgets::GeoFileConnectorDialog::FindById::operator()(const DataSourcePtr& ds) const
{
  return (ds->getId() == m_id);
}

