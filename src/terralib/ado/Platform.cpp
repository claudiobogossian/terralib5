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
   
  \brief A utility class to initialize and terminate TerraLib ADO driver support.  
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

bool te::ado::Platform::sm_adoInitialized = false;
const std::string te::ado::Platform::sm_unknownTypeName("UNKNOWN");
const std::string te::ado::Platform::sm_int2TypeName("INT2");
const std::string te::ado::Platform::sm_intTypeName("INT");
const std::string te::ado::Platform::sm_int8TypeName("INT8");
const std::string te::ado::Platform::sm_numericTypeName("NUMERIC");
const std::string te::ado::Platform::sm_dateTypeName("DATE");
const std::string te::ado::Platform::sm_timeTypeName("TIME");
const std::string te::ado::Platform::sm_timeTZTypeName("TIMETZ");
const std::string te::ado::Platform::sm_timeStampTypeName("TIMESTAMP");
const std::string te::ado::Platform::sm_timeStampTZTypeName("TIMESTAMPTZ");
const std::string te::ado::Platform::sm_floatTypeName("FLOAT4");
const std::string te::ado::Platform::sm_doubleTypeName("FLOAT8");
const std::string te::ado::Platform::sm_booleanTypeName("BOOL");
const std::string te::ado::Platform::sm_charTypeName("CHAR");
const std::string te::ado::Platform::sm_varcharTypeName("VARCHAR");
const std::string te::ado::Platform::sm_stringTypeName("TEXT");
const std::string te::ado::Platform::sm_byteArrayTypeName("BYTEA");
const std::string te::ado::Platform::sm_pointTypeName("POINT");
const std::string te::ado::Platform::sm_pointMTypeName("POINTM");
const std::string te::ado::Platform::sm_lineStringTypeName("LINESTRING");
const std::string te::ado::Platform::sm_lineStringMTypeName("LINESTRINGM");
const std::string te::ado::Platform::sm_polygonTypeName("POLYGON");
const std::string te::ado::Platform::sm_polygonMTypeName("POLYGONM");
const std::string te::ado::Platform::sm_geometryCollectionTypeName("GEOMETRYCOLLECTION");
const std::string te::ado::Platform::sm_geometryCollectionMTypeName("GEOMETRYCOLLECTIONM");
const std::string te::ado::Platform::sm_multiPointTypeName("MULTIPOINT");
const std::string te::ado::Platform::sm_multiPointMTypeName("MULTIPOINTM");
const std::string te::ado::Platform::sm_multiLineStringTypeName("MULTILINESTRING");
const std::string te::ado::Platform::sm_multiLineStringMTypeName("MULTILINESTRINGM");
const std::string te::ado::Platform::sm_multiPolygonTypeName("MULTIPOLYGON");
const std::string te::ado::Platform::sm_multiPolygonMTypeName("MULTIPOLYGONM");
const std::string te::ado::Platform::sm_geometryTypeName("GEOMETRY");

te::ado::Platform::Platform(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::CppPlugin(pluginInfo)
{
}

te::ado::Platform::~Platform()
{
}

void te::ado::Platform::startup()
{
  te::ado::Platform::initialize();
}

void te::ado::Platform::shutdown()
{
  te::ado::Platform::finalize();
}

void te::ado::Platform::initialize()
{
  if(sm_adoInitialized)
    return;

// it initializes the Translator support for the TerraLib ADO driver support
  TE_ADD_TEXT_DOMAIN(TE_ADO_TEXT_DOMAIN, TE_ADO_TEXT_DOMAIN_DIR, "UTF-8");

  DataSourceFactory::initialize();
  
// if we are here... the common is initialized!
  sm_adoInitialized = true;

  TE_LOG_TRACE(TR_ADO("TerraLib ADO driver support initialized!"));
}

void te::ado::Platform::finalize()
{
  if(!sm_adoInitialized)
    return;

// Free ADO resources
  te::da::DataSourceManager::getInstance().detachAll(getDriverID());
  //DataSource::setDialect(0);
  //DataSource::setCapabilities(0);

  DataSourceFactory::finalize();

  TE_LOG_TRACE(TR_ADO("TerraLib ADO driver support finalized!"));
  
  sm_adoInitialized = false;
}

const std::string& te::ado::Platform::getDriverID()
{
  static const std::string driverIdentifier(ADO_DRIVER_IDENTIFIER);

  return driverIdentifier;
}

PLUGIN_CALL_BACK_IMPL(te::ado::Platform)

