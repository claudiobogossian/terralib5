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
  \file terralib/mongodb/DataSourceFactory.cpp

  \brief This is the concrete factory for the TerraLib MongoDB Data Access driver.
*/

// TerraLib
#include "DataSource.h"
#include "DataSourceFactory.h"
#include "Globals.h"

te::mongodb::DataSourceFactory* te::mongodb::DataSourceFactory::sm_factory(0);

const std::string& te::mongodb::DataSourceFactory::getType() const
{
  return Globals::sm_driverIdentifier;
}

void te::mongodb::DataSourceFactory::getConnectionParameters(std::vector<std::string>& params) const
{
  params.push_back("MONGODB_DB_NAME");
  params.push_back("MONGODB_URI");
  params.push_back("MONGODB_CLIENT_ENCODING");
}

te::mongodb::DataSourceFactory::~DataSourceFactory()
{
}

te::da::DataSource* te::mongodb::DataSourceFactory::build()
{
  return new DataSource;
}

te::mongodb::DataSourceFactory::DataSourceFactory()
  : te::da::DataSourceFactory(MONGODB_DRIVER_IDENTIFIER)
{
}

void te::mongodb::DataSourceFactory::initialize()
{
  finalize();
  sm_factory = new DataSourceFactory;
}

void te::mongodb::DataSourceFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

