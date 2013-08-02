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
 \file terralib/gdal/DataSourceFactory.cpp
 
 \brief This is the data source concrete factory for the GDAL Data Access driver.
 */

// TerraLib
#include "DataSource.h"
#include "DataSourceFactory.h"

std::string te::gdal::DataSourceFactory::sm_factory_identifier = TE_GDAL_DRIVER_IDENTIFIER;

te::gdal::DataSourceFactory te::gdal::DataSourceFactory::sm_factory;

const std::string& te::gdal::DataSourceFactory::getType() const
{
  return sm_factory_identifier;
}

void te::gdal::DataSourceFactory::getConnectionParameters(std::vector<std::string>& params) const
{
  params.push_back("URI");
  params.push_back("SOURCE");
}

te::gdal::DataSourceFactory::~DataSourceFactory()
{
}

te::da::DataSource* te::gdal::DataSourceFactory::build()
{
  return new DataSource;
}

te::gdal::DataSourceFactory::DataSourceFactory()
: te::da::DataSourceFactory(TE_GDAL_DRIVER_IDENTIFIER)
{
}