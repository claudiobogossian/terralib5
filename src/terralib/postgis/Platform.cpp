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
  \file terralib/postgis/Platform.cpp
   
  \brief A utility class to initialize and terminate TerraLib PostGIS driver support.  
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/Translator.h"
#include "../dataaccess/datasource/DataSourceManager.h"
#include "../dataaccess/query/BinaryOpEncoder.h"
#include "../dataaccess/query/FunctionEncoder.h"
#include "../dataaccess/query/SQLDialect.h"
#include "../dataaccess/query/SQLFunctionEncoder.h"
#include "../dataaccess/query/TemplateEncoder.h"
#include "../dataaccess/query/UnaryOpEncoder.h"
#include "DataSource.h"
#include "DataSourceFactory.h"
#include "Platform.h"

bool te::pgis::Platform::sm_pgisInitialized = false;
const std::string te::pgis::Platform::sm_unknownTypeName("UNKNOWN");
const std::string te::pgis::Platform::sm_int2TypeName("INT2");
const std::string te::pgis::Platform::sm_intTypeName("INT");
const std::string te::pgis::Platform::sm_int8TypeName("INT8");
const std::string te::pgis::Platform::sm_numericTypeName("NUMERIC");
const std::string te::pgis::Platform::sm_dateTypeName("DATE");
const std::string te::pgis::Platform::sm_timeTypeName("TIME");
const std::string te::pgis::Platform::sm_timeTZTypeName("TIMETZ");
const std::string te::pgis::Platform::sm_timeStampTypeName("TIMESTAMP");
const std::string te::pgis::Platform::sm_timeStampTZTypeName("TIMESTAMPTZ");
const std::string te::pgis::Platform::sm_floatTypeName("FLOAT4");
const std::string te::pgis::Platform::sm_doubleTypeName("FLOAT8");
const std::string te::pgis::Platform::sm_booleanTypeName("BOOL");
const std::string te::pgis::Platform::sm_charTypeName("CHAR");
const std::string te::pgis::Platform::sm_varcharTypeName("VARCHAR");
const std::string te::pgis::Platform::sm_stringTypeName("TEXT");
const std::string te::pgis::Platform::sm_byteArrayTypeName("BYTEA");
const std::string te::pgis::Platform::sm_pointTypeName("POINT");
const std::string te::pgis::Platform::sm_pointMTypeName("POINTM");
const std::string te::pgis::Platform::sm_lineStringTypeName("LINESTRING");
const std::string te::pgis::Platform::sm_lineStringMTypeName("LINESTRINGM");
const std::string te::pgis::Platform::sm_polygonTypeName("POLYGON");
const std::string te::pgis::Platform::sm_polygonMTypeName("POLYGONM");
const std::string te::pgis::Platform::sm_geometryCollectionTypeName("GEOMETRYCOLLECTION");
const std::string te::pgis::Platform::sm_geometryCollectionMTypeName("GEOMETRYCOLLECTIONM");
const std::string te::pgis::Platform::sm_multiPointTypeName("MULTIPOINT");
const std::string te::pgis::Platform::sm_multiPointMTypeName("MULTIPOINTM");
const std::string te::pgis::Platform::sm_multiLineStringTypeName("MULTILINESTRING");
const std::string te::pgis::Platform::sm_multiLineStringMTypeName("MULTILINESTRINGM");
const std::string te::pgis::Platform::sm_multiPolygonTypeName("MULTIPOLYGON");
const std::string te::pgis::Platform::sm_multiPolygonMTypeName("MULTIPOLYGONM");
const std::string te::pgis::Platform::sm_geometryTypeName("GEOMETRY");

te::pgis::Platform::Platform(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::CppPlugin(pluginInfo)
{
}

te::pgis::Platform::~Platform()
{
}

void te::pgis::Platform::startup()
{
  te::pgis::Platform::initialize();
}

void te::pgis::Platform::shutdown()
{
  te::pgis::Platform::finalize();
}

void te::pgis::Platform::initialize()
{
  if(sm_pgisInitialized)
    return;

// it initializes the Translator support for the TerraLib PostGIS driver support
  TE_ADD_TEXT_DOMAIN(TE_PGIS_TEXT_DOMAIN, TE_PGIS_TEXT_DOMAIN_DIR, "UTF-8");

  DataSourceFactory::initialize();

  #include "PostGISCapabilities.h"
  #include "PostGISDialect.h"

// if we are here... the common is initialized!
  sm_pgisInitialized = true;

  TE_LOG_TRACE(TR_PGIS("TerraLib PostGIS driver support initialized!"));
}

void te::pgis::Platform::finalize()
{
  if(!sm_pgisInitialized)
    return;

// Free PostGIS resources
  te::da::DataSourceManager::getInstance().removeAll(getDriverID());
  DataSource::setDialect(0);
  //DataSource::setCapabilities(0);

  DataSourceFactory::finalize();

  TE_LOG_TRACE(TR_PGIS("TerraLib PostGIS driver support finalized!"));
  
  sm_pgisInitialized = false;
}

const std::string& te::pgis::Platform::getDriverID()
{
  static const std::string driverIdentifier(PGIS_DRIVER_IDENTIFIER);

  return driverIdentifier;
}

PLUGIN_CALL_BACK_IMPL(te::pgis::Platform)

