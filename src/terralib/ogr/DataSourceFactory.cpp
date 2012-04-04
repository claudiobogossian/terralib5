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
  \file DataSourceFactory.cpp

  \brief This is the concrete factory for a OGR DataSource.  
*/

// TerraLib
#include "DataSource.h"
#include "DataSourceFactory.h"
#include "Globals.h"

// STL
#include <cassert>

te::ogr::DataSourceFactory* te::ogr::DataSourceFactory::sm_factory(0);

te::ogr::DataSourceFactory::~DataSourceFactory()
{
}

void te::ogr::DataSourceFactory::initialize()
{
  finalize();
  sm_factory = new DataSourceFactory;
}

void te::ogr::DataSourceFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

const std::string& te::ogr::DataSourceFactory::getType() const
{
  return Globals::m_driverIdentifier;
}

void te::ogr::DataSourceFactory::getConnectionParameters(std::vector<std::string>& params) const
{
  params.push_back("connection_string");
}

te::da::DataSource* te::ogr::DataSourceFactory::build()
{
  return new DataSource;
}

te::ogr::DataSourceFactory::DataSourceFactory()
  : te::da::DataSourceFactory(OGR_DRIVER_IDENTIFIER)
{
}

