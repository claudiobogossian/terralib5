/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/postgis/DataSourceFactory.cpp

  \brief This is the concrete factory for PostGIS data sources.
*/

// TerraLib
#include "DataSource.h"
#include "DataSourceFactory.h"
#include "Globals.h"

te::pgis::DataSourceFactory* te::pgis::DataSourceFactory::sm_factory(0);

void te::pgis::DataSourceFactory::initialize()
{
  finalize();
  sm_factory = new DataSourceFactory;
}

void te::pgis::DataSourceFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

const std::string& te::pgis::DataSourceFactory::getType() const
{
  return Globals::sm_driverIdentifier;
}

void te::pgis::DataSourceFactory::getConnectionParameters(std::vector<std::string>& params) const
{
  params.push_back("host");
  params.push_back("hostaddr");
  params.push_back("port");
  params.push_back("dbname");
  params.push_back("user");
  params.push_back("password");
  params.push_back("connect_timeout");
  params.push_back("options");
  params.push_back("sslmode");
  params.push_back("krbsrvname");
  params.push_back("gsslib");
  params.push_back("InitialPoolSize");
  params.push_back("MinPoolSize");
  params.push_back("MaxPoolSize");
  params.push_back("MaxIdleTime");
  params.push_back("CLIENT_ENCODING");
}

te::pgis::DataSourceFactory::~DataSourceFactory()
{
}


te::da::DataSource* te::pgis::DataSourceFactory::build()
{
  return new DataSource;
}

te::pgis::DataSourceFactory::DataSourceFactory()
  : te::da::DataSourceFactory(Globals::sm_driverIdentifier)
{
}



