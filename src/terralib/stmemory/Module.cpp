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

  \brief This singleton defines the TerraLib ST module entry.
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/TerraLib.h"
#include "../common/Translator.h"
#include "../dataaccess/datasource/DataSourceFactory.h"

#include "Config.h"
#include "DataSourceFactory.h"
#include "DataSource.h"
#include "Module.h"

const te::stmem::Module& sm_module = te::stmem::Module::getInstance();

te::stmem::Module::Module()
{
  TerraLib::Module m = { TE_STMEMORY_MODULE_NAME,
                         te::stmem::Module::initialize,
                         te::stmem::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);
}

te::stmem::Module::~Module()
{
  TerraLib::getInstance().remove(TE_STMEMORY_MODULE_NAME);
}

void te::stmem::Module::initialize()
{
  // register data source factory
  te::da::DataSourceFactory::add(TE_STMEMORY_DRIVER_IDENTIFIER, te::stmem::Build);
  
  TE_LOG_TRACE(TE_TR("TerraLib ST memory driver initialized!"));
}

void te::stmem::Module::finalize()
{
  // Unregister the data source factory
  te::da::DataSourceFactory::remove(TE_STMEMORY_DRIVER_IDENTIFIER);

  TE_LOG_TRACE(TE_TR("TerraLib ST memory driver finalized!"));
}



