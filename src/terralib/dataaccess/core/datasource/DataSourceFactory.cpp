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
  \file terralib/dataaccess/core/datasource/DataSourceFactory.cpp

  \brief A factory of data sources.
*/

// TerraLib
#include "../../../common/Translator.h"
#include "DataSource.h"
#include "DataSourceFactory.h"

// Boost
#include <boost/format.hpp>

std::map<std::string, te::da::core::DataSourceFactory::FactoryFnctType>
te::da::core::DataSourceFactory::sm_factories;

std::auto_ptr<te::da::core::DataSource>
te::da::core::DataSourceFactory::make(const std::string& dsType) throw(Exception)
{
  std::map<std::string, FactoryFnctType>::const_iterator it = sm_factories.find(dsType);

  if(it == sm_factories.end())
    throw Exception((boost::format(TR_DATAACCESS("Could not find a data source factory named: %1.")) % dsType).str());

  std::auto_ptr<DataSource> ds(sm_factories[dsType]());

  return ds;
}

void
te::da::core::DataSourceFactory::add(const std::string& dsType, 
                                     FactoryFnctType f) throw(Exception)
{
  std::map<std::string, FactoryFnctType>::const_iterator it = sm_factories.find(dsType);

  if(it != sm_factories.end())
    throw Exception((boost::format(TR_DATAACCESS("A data source factory with name '%1' is already registered.")) % dsType).str());

  sm_factories[dsType] = f;
}

void
te::da::core::DataSourceFactory::remove(const std::string& dsType) throw(Exception)
{
  std::map<std::string, FactoryFnctType>::iterator it = sm_factories.find(dsType);

  if(it == sm_factories.end())
    throw Exception((boost::format(TR_DATAACCESS("There is no registered data source factory named '%1'.")) % dsType).str());

  sm_factories.erase(it);
}