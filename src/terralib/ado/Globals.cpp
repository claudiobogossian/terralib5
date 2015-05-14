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
  \file terralib/ado/Globals.cpp
   
  \brief An static class with global definitions.
*/

// TerraLib
#include "Globals.h"

const std::string te::ado::Globals::sm_driverIdentifier(ADO_DRIVER_IDENTIFIER);

te::da::SQLDialect* te::ado::Globals::sm_queryDialect(0);
te::da::DataSourceCapabilities* te::ado::Globals::sm_capabilities(0);

const std::string te::ado::Globals::sm_geometryTypeName("GEOMETRY");
const std::string te::ado::Globals::sm_pointTypeName("POINT");
const std::string te::ado::Globals::sm_lineStringTypeName("LINESTRING");
const std::string te::ado::Globals::sm_polygonTypeName("POLYGON");
const std::string te::ado::Globals::sm_multiPointTypeName("MULTIPOINT");
const std::string te::ado::Globals::sm_multiLineStringTypeName("MULTILINESTRING");
const std::string te::ado::Globals::sm_multiPolygonTypeName("MULTIPOLYGON");
const std::string te::ado::Globals::sm_curvePolygonTypeName("CURVE");
const std::string te::ado::Globals::sm_polyhedralSurfaceTypeName("POLYHEDRALSURFACE");
const std::string te::ado::Globals::sm_geometryZTypeName("GEOMETRYZ");
const std::string te::ado::Globals::sm_pointZTypeName("POINTZ");
const std::string te::ado::Globals::sm_lineStringZTypeName("LINESTRINGZ");
const std::string te::ado::Globals::sm_polygonZTypeName("POLYGONZ");
const std::string te::ado::Globals::sm_multiPointZTypeName("MULTIPOINTZ");
const std::string te::ado::Globals::sm_multiLineStringZTypeName("MULTILINESTRINGZ");
const std::string te::ado::Globals::sm_multiPolygonZTypeName("MULTIPOLYGONZ");
const std::string te::ado::Globals::sm_curvePolygonZTypeName("CURVEZ");
const std::string te::ado::Globals::sm_polyhedralSurfaceZTypeName("POLYHEDRALSURFACEZ");
const std::string te::ado::Globals::sm_geometryMTypeName("GEOMETRYM");
const std::string te::ado::Globals::sm_pointMTypeName("POINTM");
const std::string te::ado::Globals::sm_lineStringMTypeName("LINESTRINGM");
const std::string te::ado::Globals::sm_polygonMTypeName("POLYGONM");
const std::string te::ado::Globals::sm_multiPointMTypeName("MULTIPOINTM");
const std::string te::ado::Globals::sm_multiLineStringMTypeName("MULTILINESTRINGM");
const std::string te::ado::Globals::sm_multiPolygonMTypeName("MULTIPOLYGONM");
const std::string te::ado::Globals::sm_curvePolygonMTypeName("CURVEM");
const std::string te::ado::Globals::sm_polyhedralSurfaceMTypeName("POLYHEDRALSURFACEM");
const std::string te::ado::Globals::sm_geometryZMTypeName("GEOMETRYZM");
const std::string te::ado::Globals::sm_pointZMTypeName("POINTZM");
const std::string te::ado::Globals::sm_lineStringZMTypeName("LINESTRINGZM");
const std::string te::ado::Globals::sm_polygonZMTypeName("POLYGONZM");
const std::string te::ado::Globals::sm_multiPointZMTypeName("MULTIPOINTZM");
const std::string te::ado::Globals::sm_multiLineStringZMTypeName("MULTILINESTRINGZM");
const std::string te::ado::Globals::sm_multiPolygonZMTypeName("MULTIPOLYGONZM");
const std::string te::ado::Globals::sm_curvePolygonZMTypeName("CURVEZM");
const std::string te::ado::Globals::sm_polyhedralSurfaceZMTypeName("POLYHEDRALSURFACEZM");

