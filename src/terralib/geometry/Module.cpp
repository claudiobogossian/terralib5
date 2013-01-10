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
  \file Module.cpp
   
  \brief This singleton defines the TerraLib Vector Geometry module entry.  
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/TerraLib.h"
#include "../common/Translator.h"
#include "../datatype/DataConverterManager.h"
#include "../datatype/Enums.h"
#include "Geometry.h"
#include "GeometryConverters.h"
#include "GEOSGeometryFactory.h"
#include "Module.h"

///*!
//  \brief This is a helper function that will be automatically called when the TerraLib Geometry module is loaded.
//
//  The initialization includes:
//  <ul>
//  <li>GEOS geometry factory</li>
//  <li>The geometry name list</li>
//  </ul>
//*/
//static void TeGeomInitialize()
//{
//// it loads the geometry type id map
//  te::gm::Geometry::loadGeomTypeId();
//
//#if TE_USE_GEOS
//// it initializes the GEOS Geometry Factory
//  te::gm::GEOSGeometryFactory::initialize();
//#endif
//
//  TE_LOG_TRACE(TR_GEOM("TerraLib Vector Geometry module Initialized!"));
//}

///*!
//  \brief This is a helper function that will be automatically called when the TerraLib Geometry module is unloaded.
//
//  The Finalization includes:
//  <ul>
//  <li>GEOS geometry factory</li>
//  </ul>
//*/
//static void TeGeomFinalize()
//{
//#if TE_USE_GEOS
//// it finalizes the GEOS Geometry Factory
//  te::gm::GEOSGeometryFactory::finalize();
//#endif
//
//  TE_LOG_TRACE(TR_GEOM("TerraLib Vector Geometry module Finalized!"));
//}

const te::gm::Module& sm_module = te::gm::Module::getInstance();

te::gm::Module::Module()
{
  TerraLib::Module m = { TE_GEOMETRY_MODULE_NAME,
                         te::gm::Module::initialize,
                         te::gm::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);

// it initializes the Translator support for the TerraLib Vector Geometry Model
  TE_ADD_TEXT_DOMAIN(TE_GEOM_TEXT_DOMAIN, TE_GEOM_TEXT_DOMAIN_DIR, "UTF-8");
}

te::gm::Module::~Module()
{
  TerraLib::getInstance().remove(TE_GEOMETRY_MODULE_NAME);
}

void te::gm::Module::initialize()
{
// it loads the geometry type id map
  te::gm::Geometry::loadGeomTypeId();

#if TE_USE_GEOS
// it initializes the GEOS Geometry Factory
  te::gm::GEOSGeometryFactory::initialize();
#endif

// registering data type converters
  te::dt::DataConverterManager::getInstance().add(te::dt::GEOMETRY_TYPE, te::dt::BYTE_ARRAY_TYPE, GeometryToByteArrayConverter);
  te::dt::DataConverterManager::getInstance().add(te::dt::GEOMETRY_TYPE, te::dt::STRING_TYPE, GeometryToStringConverter);
  te::dt::DataConverterManager::getInstance().add(te::dt::BYTE_ARRAY_TYPE, te::dt::GEOMETRY_TYPE, ByteArrayToGeometryConverter);
  te::dt::DataConverterManager::getInstance().add(te::dt::STRING_TYPE, te::dt::GEOMETRY_TYPE, StringToGeometryConverter);

  TE_LOG_TRACE(TR_GEOM("TerraLib Vector Geometry module Initialized!"));
}

void te::gm::Module::finalize()
{
#if TE_USE_GEOS
// it finalizes the GEOS Geometry Factory
  te::gm::GEOSGeometryFactory::finalize();
#endif

  TE_LOG_TRACE(TR_GEOM("TerraLib Vector Geometry module Finalized!"));
}

//// TerraLib
//#include "../common/ModuleUtils.h"
//
//TE_REGISTER_MODULE(TE_GEOMETRY_MODULE_NAME, TeGeomInitialize, TeGeomFinalize)
//
