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
  \file terralib/dataaccess/Module.cpp
   
  \brief This singleton defines the TerraLib Data Access module entry.
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/TerraLib.h"
#include "../common/Translator.h"
#include "dataset/AttributeConverterManager.h"
#include "dataset/AttributeConverters.h"
#include "datasource/ConnectionPoolManager.h"
#include "Config.h"
#include "Module.h"

const te::da::Module& sm_module = te::da::Module::getInstance();

te::da::Module::Module()
{
  TerraLib::Module m = { TE_DATAACCESS_MODULE_NAME,
                         te::da::Module::initialize,
                         te::da::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);

// it initializes the Translator support for the TerraLib Data Access support
  TE_ADD_TEXT_DOMAIN(TE_DATAACCESS_TEXT_DOMAIN, TE_DATAACCESS_TEXT_DOMAIN_DIR, "UTF-8");

// initializes the ConnectionPoolManager singleton
  ConnectionPoolManager::getInstance();
}

te::da::Module::~Module()
{
  TerraLib::getInstance().remove(TE_DATAACCESS_MODULE_NAME);
}

void te::da::Module::initialize()
{
#if TE_AUTOMATICALLY_INITIALIZE_CONNECTION_POOL_MANAGER
  ConnectionPoolManager::getInstance().start();
#endif

  // Reg the Attribute Converters
  AttributeConverterManager::getInstance().addConverter("GenericAttributeConverter", te::da::GenericAttributeConverter);
  AttributeConverterManager::getInstance().addConverter("XYToPointConverter", te::da::XYToPointConverter);
  AttributeConverterManager::getInstance().addConverter("XYZToPointConverter", te::da::XYZToPointConverter);
  AttributeConverterManager::getInstance().addConverter("XYMToPointConverter", te::da::XYMToPointConverter);
  AttributeConverterManager::getInstance().addConverter("XYZMToPointConverter", te::da::XYZMToPointConverter);
  AttributeConverterManager::getInstance().addConverter("PointToXConverter", te::da::PointToXConverter);
  AttributeConverterManager::getInstance().addConverter("PointToYConverter", te::da::PointToYConverter);
  AttributeConverterManager::getInstance().addConverter("PointToZConverter", te::da::PointToZConverter);
  AttributeConverterManager::getInstance().addConverter("PointToMConverter", te::da::PointToMConverter);
  AttributeConverterManager::getInstance().addConverter("TupleToStringConverter", te::da::TupleToStringConverter);

  TE_LOG_TRACE(TR_DATAACCESS("TerraLib Data Access module initialized!"));
}

void te::da::Module::finalize()
{
#if TE_AUTOMATICALLY_INITIALIZE_CONNECTION_POOL_MANAGER
  ConnectionPoolManager::getInstance().stop();
#endif

  TE_LOG_TRACE(TR_DATAACCESS("TerraLib Data Access module finalized!"));
}

