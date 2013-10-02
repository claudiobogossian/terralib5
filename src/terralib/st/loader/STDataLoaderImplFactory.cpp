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
#include "../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../dataaccess/query/QueryCapabilities.h"
#include "../../dataaccess/utils/Utils.h"

// ST
#include "STDataLoaderImplFactory.h"

// STL
#include <memory>

te::st::STDataLoaderImpl* te::st::STDataLoaderImplFactory::make(const std::string& dsType)
{
  //use the DataSourceManager to get the DataSource and get its capabilites
  te::da::DataSourcePtr ds = te::da::GetDataSource(dsType, false);
  
  std::string loaderType = "STDATALOADERFROMMEMDS";

  const te::da::DataSourceCapabilities& cpb = ds->getCapabilities();
  if(cpb.supportsSpatialOperators())
    loaderType = "STDATALOADERFROMDS";
    
  //TO DO:olhar o capabilities do Query - spatial and temporal.
  /*
  const te::da::QueryCapabilities& cpb = ds->getCapabilities().getQueryCapabilities(); 
  
  //Rever esse capabilites!!!!! Ter um flag pra indicar
  //se o data source aceita spatial SQL também
  if(cpb.supportsSQLDialect())
    loaderType = "DATASOURCELOADER";
  */

  return te::common::AbstractFactory<STDataLoaderImpl, std::string>::make(loaderType);
}

te::st::STDataLoaderImplFactory::STDataLoaderImplFactory(const std::string& factoryKey)
  : te::common::AbstractFactory<STDataLoaderImpl, std::string>(factoryKey)
{
}

te::st::STDataLoaderImplFactory::~STDataLoaderImplFactory()
{
}

