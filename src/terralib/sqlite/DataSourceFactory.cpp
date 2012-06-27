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
  \file terralib/sqlite/DataSourceFactory.cpp

  \brief This is the concrete factory for the SQLite Data Access driver.
*/

// TerraLib
#include "DataSource.h"
#include "DataSourceFactory.h"
#include "Globals.h"

te::sqlite::DataSourceFactory* te::sqlite::DataSourceFactory::sm_factory(0);

const std::string& te::sqlite::DataSourceFactory::getType() const
{
  return Globals::sm_driverIdentifier;
}

void te::sqlite::DataSourceFactory::getConnectionParameters(std::vector<std::string>& params) const
{
  params.push_back("SQLITE_FILE");
  params.push_back("SQLITE_OPEN_NOMUTEX");
  params.push_back("SQLITE_OPEN_FULLMUTEX");
  params.push_back("SQLITE_OPEN_SHAREDCACHE");
  params.push_back("SQLITE_OPEN_PRIVATECACHE");
  params.push_back("SQLITE_OPEN_READONLY");
  params.push_back("SQLITE_OPEN_READWRITE");
  params.push_back("SQLITE_OPEN_CREATE");
  params.push_back("SQLITE_CREATE_INTERMEDIATE_DIR");
  params.push_back("SQLITE_OPEN_URI");
  params.push_back("SQLITE_VFS");
  params.push_back("SQLITE_HIDE_RASTER_TABLES");
  params.push_back("SQLITE_HIDE_SPATIAL_METADATA_TABLES");
  params.push_back("SQLITE_HIDE_TABLES");
  params.push_back("SQLITE_CREATE_OGR_METADATA_TABLES");
  params.push_back("SQLITE_CREATE_SPATIALITE_METADATA_TABLES");
  params.push_back("SQLITE_CREATE_TERRALIB_RASTER_METADATA_TABLES");
}

te::sqlite::DataSourceFactory::~DataSourceFactory()
{
}

te::da::DataSource* te::sqlite::DataSourceFactory::build()
{
  return new DataSource;
}

te::sqlite::DataSourceFactory::DataSourceFactory()
  : te::da::DataSourceFactory(Globals::sm_driverIdentifier)
{
}

void te::sqlite::DataSourceFactory::initialize()
{
  finalize();
  sm_factory = new DataSourceFactory;
}

void te::sqlite::DataSourceFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

