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
  \file terralib/qt/widgets/datasource/connector/geofile/GeoFileConnector.cpp

  \brief ....
*/

// TerraLib
#include <terralib/dataaccess/datasource/DataSource.h>
#include <terralib/dataaccess/datasource/DataSourceManager.h>
#include <terralib/qt/widgets/datasource/core/DataSourceManager.h>
#include "GeoFileConnector.h"
#include "GeoFileConnectorDialog.h"

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>

// Qt
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

namespace te_qt = te::qt::widgets;
namespace plg_geofile = qt_af::plugin::geofile;

plg_geofile::GeoFileConnector::GeoFileConnector(QWidget* parent, Qt::WindowFlags f)
  : te_qt::AbstractDataSourceConnector(parent, f)
{
}

plg_geofile::GeoFileConnector::~GeoFileConnector()
{
}

void plg_geofile::GeoFileConnector::create(std::list<te_qt::DataSourcePtr>& datasources)
{
  std::auto_ptr<GeoFileConnectorDialog> cdialog(new GeoFileConnectorDialog(static_cast<QWidget*>(parent())));

  int retval = cdialog->exec();

  if(retval == QDialog::Rejected)
    return;

  datasources = cdialog->getDataSources();

  for(std::list<te_qt::DataSourcePtr>::iterator it = datasources.begin(); it != datasources.end(); ++it)
    te_qt::DataSourceManager::getInstance().add(*it);
}

void plg_geofile::GeoFileConnector::update(std::list<te_qt::DataSourcePtr>& /*datasources*/)
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

void plg_geofile::GeoFileConnector::remove(std::list<te_qt::DataSourcePtr>& datasources)
{
  for(std::list<te_qt::DataSourcePtr>::iterator it = datasources.begin(); it != datasources.end(); ++it)
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
    te_qt::DataSourceManager::getInstance().remove((*it)->getId());
  }
}

