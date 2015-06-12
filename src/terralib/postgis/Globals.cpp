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
  \file terralib/postgis/Globals.cpp
   
  \brief An static class with global definitions.
*/

// TerraLib
#include "Globals.h"

te::da::SQLDialect* te::pgis::Globals::sm_queryDialect(0);
te::da::DataSourceCapabilities* te::pgis::Globals::sm_capabilities(0);

const std::string te::pgis::Globals::sm_unknownTypeName("UNKNOWN");
const std::string te::pgis::Globals::sm_charTypeName("\"char\"");
const std::string te::pgis::Globals::sm_int2TypeName("INT2");
const std::string te::pgis::Globals::sm_intTypeName("INT");
const std::string te::pgis::Globals::sm_int8TypeName("INT8");
const std::string te::pgis::Globals::sm_numericTypeName("NUMERIC");
const std::string te::pgis::Globals::sm_dateTypeName("DATE");
const std::string te::pgis::Globals::sm_timeTypeName("TIME");
const std::string te::pgis::Globals::sm_timeTZTypeName("TIMETZ");
const std::string te::pgis::Globals::sm_timeStampTypeName("TIMESTAMP");
const std::string te::pgis::Globals::sm_timeStampTZTypeName("TIMESTAMPTZ");
const std::string te::pgis::Globals::sm_intervalTypeName("INTERVAL");
const std::string te::pgis::Globals::sm_floatTypeName("FLOAT4");
const std::string te::pgis::Globals::sm_doubleTypeName("FLOAT8");
const std::string te::pgis::Globals::sm_booleanTypeName("BOOL");
const std::string te::pgis::Globals::sm_fixedcharTypeName("CHAR");
const std::string te::pgis::Globals::sm_varcharTypeName("VARCHAR");
const std::string te::pgis::Globals::sm_stringTypeName("TEXT");
const std::string te::pgis::Globals::sm_byteArrayTypeName("BYTEA");
const std::string te::pgis::Globals::sm_pointTypeName("POINT");
const std::string te::pgis::Globals::sm_pointMTypeName("POINTM");
const std::string te::pgis::Globals::sm_lineStringTypeName("LINESTRING");
const std::string te::pgis::Globals::sm_lineStringMTypeName("LINESTRINGM");
const std::string te::pgis::Globals::sm_polygonTypeName("POLYGON");
const std::string te::pgis::Globals::sm_polygonMTypeName("POLYGONM");
const std::string te::pgis::Globals::sm_geometryCollectionTypeName("GEOMETRYCOLLECTION");
const std::string te::pgis::Globals::sm_geometryCollectionMTypeName("GEOMETRYCOLLECTIONM");
const std::string te::pgis::Globals::sm_multiPointTypeName("MULTIPOINT");
const std::string te::pgis::Globals::sm_multiPointMTypeName("MULTIPOINTM");
const std::string te::pgis::Globals::sm_multiLineStringTypeName("MULTILINESTRING");
const std::string te::pgis::Globals::sm_multiLineStringMTypeName("MULTILINESTRINGM");
const std::string te::pgis::Globals::sm_multiPolygonTypeName("MULTIPOLYGON");
const std::string te::pgis::Globals::sm_multiPolygonMTypeName("MULTIPOLYGONM");
const std::string te::pgis::Globals::sm_geometryTypeName("GEOMETRY");

const std::string te::pgis::Globals::sm_driverIdentifier(PGIS_DRIVER_IDENTIFIER);

