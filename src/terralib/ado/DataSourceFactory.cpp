/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/ado/DataSourceFactory.cpp

  \brief This is the concrete factory for the TerraLib ADO Data Access driver.
*/

// TerraLib
#include "DataSource.h"
#include "DataSourceFactory.h"
#include "Globals.h"

te::ado::DataSourceFactory* te::ado::DataSourceFactory::sm_factory(0);

const std::string& te::ado::DataSourceFactory::getType() const
{
  return Globals::sm_driverIdentifier;
}

void te::ado::DataSourceFactory::getConnectionParameters(std::vector<std::string>& params) const
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
  params.push_back("provider");
}

te::ado::DataSourceFactory::~DataSourceFactory()
{
}


te::da::DataSource* te::ado::DataSourceFactory::build()
{
  return new DataSource;
}

te::ado::DataSourceFactory::DataSourceFactory()
  : te::da::DataSourceFactory(ADO_DRIVER_IDENTIFIER)
{
}

void te::ado::DataSourceFactory::initialize()
{
  finalize();
  sm_factory = new DataSourceFactory;
}

void te::ado::DataSourceFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

