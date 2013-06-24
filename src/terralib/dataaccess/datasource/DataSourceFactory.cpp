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
  \file terralib/dataaccess/datasource/DataSourceFactory.cpp

  \brief This is the abstract factory for DataSource.
*/

// TerraLib
#include "../../common/StringUtils.h"
#include "DataSourceFactory.h"

// STL
#include <memory>

te::da::DataSource* te::da::DataSourceFactory::make(const std::string& dsType)
{
  std::string ucase = te::common::Convert2UCase(dsType);
  return te::common::AbstractFactory<te::da::DataSource, std::string>::make(ucase);
}

te::da::DataSource* te::da::DataSourceFactory::open(const std::string& dsType, const std::map<std::string, std::string>& dsInfo)
{
  std::auto_ptr<DataSource> ds(make(dsType));

  ds->open(dsInfo);

  return ds.release();
}

te::da::DataSource* te::da::DataSourceFactory::open(const std::string& dsType, const std::string& dsInfo)
{
  std::auto_ptr<DataSource> ds(make(dsType));

  ds->open(dsInfo);

  return ds.release();
}

te::da::DataSourceFactory::DataSourceFactory(const std::string& factoryKey)
  : te::common::AbstractFactory<te::da::DataSource, std::string>(factoryKey)
{
}

