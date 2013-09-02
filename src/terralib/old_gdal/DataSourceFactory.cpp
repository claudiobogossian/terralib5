/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/gdal/DataSourceFactory.cpp

  \brief This is the concrete factory for a GDAL managed DataSource.  
*/

// TerraLib
#include "DataSource.h"
#include "DataSourceFactory.h"
#include "Globals.h"

// STL
#include <cassert>

te::gdal::DataSourceFactory* te::gdal::DataSourceFactory::sm_factory(0);

te::gdal::DataSourceFactory::~DataSourceFactory()
{
}

void te::gdal::DataSourceFactory::initialize()
{
  finalize();
  sm_factory = new DataSourceFactory;
}

void te::gdal::DataSourceFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

const std::string& te::gdal::DataSourceFactory::getType() const
{
  return Globals::getInstance().getDriverID();
}

void te::gdal::DataSourceFactory::getConnectionParameters(std::vector<std::string>& params) const
{
  params.push_back("SOURCE");
  params.push_back("URI");
}

te::da::DataSource* te::gdal::DataSourceFactory::build()
{
  return new te::gdal::DataSource();
}

te::gdal::DataSourceFactory::DataSourceFactory()
  : te::da::DataSourceFactory(Globals::getInstance().getDriverID())
{
}

