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
  \file terralib/qt/plugins/datasource/geofile/GeoFileConnector.cpp

  \brief GeoFile connector implementation for the Qt data source widget.
*/

// TerraLib
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "GeoFileConnector.h"
#include "GeoFileConnectorDialog.h"

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>

// Qt
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

te::qt::plugins::geofile::GeoFileConnector::GeoFileConnector(QWidget* parent, Qt::WindowFlags f)
  : te::qt::widgets::AbstractDataSourceConnector(parent, f)
{
}

te::qt::plugins::geofile::GeoFileConnector::~GeoFileConnector()
{
}

void te::qt::plugins::geofile::GeoFileConnector::connect(std::list<te::da::DataSourceInfoPtr>& datasources)
{
  std::auto_ptr<GeoFileConnectorDialog> cdialog(new GeoFileConnectorDialog(static_cast<QWidget*>(parent())));

  int retval = cdialog->exec();

  if(retval == QDialog::Rejected)
    return;

  datasources = cdialog->getDataSources();

  for(std::list<te::da::DataSourceInfoPtr>::iterator it = datasources.begin(); it != datasources.end(); ++it)
    te::da::DataSourceInfoManager::getInstance().add(*it);
}

void te::qt::plugins::geofile::GeoFileConnector::create(std::list<te::da::DataSourceInfoPtr>& datasources)
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

void te::qt::plugins::geofile::GeoFileConnector::update(std::list<te::da::DataSourceInfoPtr>& /*datasources*/)
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

void te::qt::plugins::geofile::GeoFileConnector::remove(std::list<te::da::DataSourceInfoPtr>& datasources)
{
  for(std::list<te::da::DataSourceInfoPtr>::iterator it = datasources.begin(); it != datasources.end(); ++it)
  {
    if(it->get() == 0)
      continue;
// first remove driver
    te::da::DataSourcePtr rds = te::da::DataSourceManager::getInstance().find((*it)->getId());

    if(rds.get())
    {
      te::da::DataSourceManager::getInstance().detach(rds);
      rds.reset();
    }

// then remove data source
    te::da::DataSourceInfoManager::getInstance().remove((*it)->getId());
  }
}

