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
  \file STDataLoaderImplFactory.cpp

  \brief This is the abstract factory for STDataLoaderImpl.
*/

// TerraLib
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../dataaccess/query/QueryCapabilities.h"

// ST
#include "../Exception.h"
#include "../Globals.h"
#include "STDataLoaderImplFactory.h"

// STL
#include <memory>

te::st::STDataLoaderImpl* te::st::STDataLoaderImplFactory::make(const std::string& dsType)
{
  //use the factory only to create a data source
  std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make(dsType);
  if(ds.get()==0)
    throw Exception(TE_TR("ST Loader: Could not find a data source!"));
  
  std::string loaderType = te::st::Globals::sm_loaderFromMemDSIdentifier; 

  //TO DO: olhar o capabilities do Query - spatial and temporal.
  const te::da::QueryCapabilities& qcpb = ds->getCapabilities().getQueryCapabilities(); 
  if(qcpb.supportsSpatialSQLDialect())
    loaderType = te::st::Globals::sm_loaderFromDSIdentifier;
      
  return te::common::AbstractFactory<STDataLoaderImpl, std::string>::make(loaderType);
}

te::st::STDataLoaderImplFactory::STDataLoaderImplFactory(const std::string& factoryKey)
  : te::common::AbstractFactory<STDataLoaderImpl, std::string>(factoryKey)
{
}

te::st::STDataLoaderImplFactory::~STDataLoaderImplFactory()
{
}

