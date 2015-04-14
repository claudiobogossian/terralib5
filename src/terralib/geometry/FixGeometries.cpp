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
  \file terralib/geometry/FixGeometries.cpp

  \brief This class contains attributes and functions necessary to fix and report unconsistencies of geometries.
*/

// TerraLib
#include "../common/Translator.h"
#include "Coord2D.h"
#include "Envelope.h"
#include "Exception.h"
#include "Geometry.h"
#include "LinearRing.h"
#include "LineString.h"
#include "Point.h"
#include "Polygon.h"
#include "FixGeometries.h"

te::gm::Geometry* te::gm::FixGeometry(te::gm::Geometry* inputGeom)
{
  te::gm::Geometry* outGeom;

  te::gm::GeomType type = inputGeom->getGeomTypeId();

  switch (type)
  {
  case te::gm::PointType:
    break;
  case te::gm::LineStringType:
    break;
  case te::gm::PolygonType:
    break;
  case te::gm::GeometryCollectionType:
    break;
  case te::gm::MultiPointType:
    break;
  case te::gm::MultiLineStringType:
    break;
  case te::gm::MultiPolygonType:
    break;
  default:
    //UnknownGeometryType:
    break;
  }

  return outGeom;
}