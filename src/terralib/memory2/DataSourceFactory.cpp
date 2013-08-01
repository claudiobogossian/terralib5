/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/memory/DataSourceFactory.cpp

  \brief This is the data source concrete factory for the In-Memory Data Access driver.
*/

// TerraLib
#include "DataSource.h"
#include "DataSourceFactory.h"
#include "Globals.h"

te::mem::DataSourceFactory te::mem::DataSourceFactory::sm_factory;

const std::string& te::mem::DataSourceFactory::getType() const
{
  return Globals::sm_driverIdentifier;
}

void te::mem::DataSourceFactory::getConnectionParameters(std::vector<std::string>& params) const
{
  params.push_back("MAX_DATASETS");
  params.push_back("OPERATION_MODE");
}

te::mem::DataSourceFactory::~DataSourceFactory()
{
}

te::da::DataSource* te::mem::DataSourceFactory::build()
{
  return new DataSource;
}

te::mem::DataSourceFactory::DataSourceFactory()
  : te::da::DataSourceFactory(TE_MEMORY_DRIVER_IDENTIFIER)
{
}


