/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/memory/Module.cpp

  \brief This singleton defines the TerraLib In-Memory module entry.
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/TerraLib.h"
#include "../common/Translator.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/datasource/DataSourceFactory.h"
#include "Config.h"
#include "DataSource.h"
#include "ExpansibleRasterFactory.h"
#include "DataSourceFactory.h"
#include "Module.h"

const te::mem::Module& sm_module = te::mem::Module::getInstance();

te::mem::Module::Module()
{
  TerraLib::Module m = { TE_MEMORY_MODULE_NAME,
                         te::mem::Module::initialize,
                         te::mem::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);
}

te::mem::Module::~Module()
{
  TerraLib::getInstance().remove(TE_MEMORY_MODULE_NAME);
}

void te::mem::Module::initialize()
{
  // Register the data source factory
  te::da::DataSourceFactory::add(TE_MEMORY_DRIVER_IDENTIFIER, te::mem::Build);

  te::da::DataSourceCapabilities capabilities;
  capabilities.setAccessPolicy(te::common::RWAccess);
  capabilities.setSupportDataSetPesistenceAPI(true);
  capabilities.setSupportDataSetTypePesistenceAPI(true);

  // DataType Capabilities
  te::da::DataTypeCapabilities dataTypeCapabilities;
  dataTypeCapabilities.setSupportAll();
  capabilities.setDataTypeCapabilities(dataTypeCapabilities);

  // DataSetType Capabilites
  te::da::DataSetTypeCapabilities dataSetTypeCapabilities;
  dataSetTypeCapabilities.setSupportAll();
  dataSetTypeCapabilities.setSupportForeingKey(false);
  dataSetTypeCapabilities.setSupportSequence(false);
  dataSetTypeCapabilities.setSupportCheckConstraints(false);
  dataSetTypeCapabilities.setSupportQuadTreeIndex(false);
  capabilities.setDataSetTypeCapabilities(dataSetTypeCapabilities);

  // DataSet Capabilities
  te::da::DataSetCapabilities dataSetCapabilities;
  dataSetCapabilities.setSupportAll();
  capabilities.setDataSetCapabilities(dataSetCapabilities);

  // Query Capabilities
  te::da::QueryCapabilities queryCapabilities;
  capabilities.setQueryCapabilities(queryCapabilities);

  DataSource::setCapabilities(capabilities);
  
  // Register the expansible raster factory
  
  te::mem::ExpansibleRasterFactory::initialize();

  TE_LOG_TRACE(TE_TR("TerraLib In-Memory driver initialized!"));
}

void te::mem::Module::finalize()
{
  // unregister the expansible raster factory
  
  te::mem::ExpansibleRasterFactory::finalize();  
  
  // Unregister the data source factory
  te::da::DataSourceFactory::remove(TE_MEMORY_DRIVER_IDENTIFIER);

  TE_LOG_TRACE(TE_TR("TerraLib In-Memory driver finalized!"));
}
