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
  \file terralib/qt/widgets/datasource/connector/shp/ShapeFileConnector.cpp

  \brief ....
*/

// TerraLib
#include <terralib/dataaccess/datasource/DataSource.h>
#include <terralib/dataaccess/datasource/DataSourceManager.h>
#include <terralib/qt/widgets/datasource/core/DataSourceManager.h>
#include "ShapeFileConnector.h"

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>

// Qt
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

namespace te_qt = te::qt::widgets;
namespace plg_shp = qt_af::plugin::shp;

plg_shp::ShapeFileConnector::ShapeFileConnector(QWidget* parent, Qt::WindowFlags f)
  : AbstractDataSourceConnector(parent, f)
{
}

plg_shp::ShapeFileConnector::~ShapeFileConnector()
{
}

void plg_shp::ShapeFileConnector::create(std::list<te_qt::DataSourcePtr>& datasources)
{
  QStringList fileNames = QFileDialog::getOpenFileNames((QWidget*)parent(), tr("Open Shapefile"), QString(""), tr("Esri Shapefile (*.shp);; All Files (*.*)"), 0, QFileDialog::ReadOnly);

  for(QStringList::iterator it = fileNames.begin(); it != fileNames.end(); ++it)
  {
    te_qt::DataSourcePtr ds(new te_qt::DataSource);

    ds->setAccessDriver("OGR");

    std::map<std::string, std::string> dsinfo;
    dsinfo["URI"] = it->toStdString();

    ds->setConnInfo(dsinfo);

    ds->setDescription("A single shapefile");

    boost::uuids::basic_random_generator<boost::mt19937> gen;
    boost::uuids::uuid u = gen();
    std::string id = boost::uuids::to_string(u);

    ds->setId(id);

    boost::filesystem::path mpath(it->toStdString());

    std::string fileBaseName = mpath.stem().string();

    ds->setTitle(fileBaseName);

    ds->setType("SHAPEFILE");

    te_qt::DataSourceManager::getInstance().add(ds);

    datasources.push_back(ds);
  }
}

void plg_shp::ShapeFileConnector::update(std::list<te_qt::DataSourcePtr>& /*datasources*/)
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("Not implemented yet!\nWe will provide it soon!"));
}

void plg_shp::ShapeFileConnector::remove(std::list<te_qt::DataSourcePtr>& datasources)
{
  for(std::list<te_qt::DataSourcePtr>::iterator it = datasources.begin(); it != datasources.end(); ++it)
  {
    if(it->get() == 0)
      continue;

// remove from qt widget manager
    te_qt::DataSourceManager::getInstance().remove((*it)->getId());

//remove from data access manager if one exists
    te::da::DataSourcePtr rds = te::da::DataSourceManager::getInstance().find((*it)->getId());

    if(rds.get())
    {
      te::da::DataSourceManager::getInstance().detach(rds);
      rds.reset();
    }
  }
}

