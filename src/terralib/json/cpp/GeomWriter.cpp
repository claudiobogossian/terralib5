/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/json/GeomWriter.cpp

  \brief A class that converts a TerraLib Geometry to GeoJSON.
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../geometry/GeometryCollection.h"
#include "../../geometry/LinearRing.h"
#include "../../geometry/LineString.h"
#include "../../geometry/MultiLineString.h"
#include "../../geometry/MultiPoint.h"
#include "../../geometry/MultiPolygon.h"
#include "../../geometry/Point.h"
#include "../../geometry/PointZ.h"
#include "../../geometry/PointZM.h"
#include "../../geometry/Polygon.h"
#include "../Exception.h"
#include "GeomWriter.h"

// STL
#include <cassert>

// jsoncpp
#include <json/json.h>

void te::json::cpp::GeomWriter::write(const te::gm::Geometry* g, Json::Value& jgeom)
{
  assert(g);

  switch(g->getGeomTypeId())
  {
    case te::gm::PointType:
      write(static_cast<const te::gm::Point*>(g), jgeom);
    break;

    case te::gm::LineStringType:
      write(static_cast<const te::gm::LineString*>(g), jgeom);
    break;

    case te::gm::PolygonType:
      write(static_cast<const te::gm::Polygon*>(g), jgeom);
    break;

    case te::gm::MultiPointType:
      write(static_cast<const te::gm::MultiPoint*>(g), jgeom);
    break;

    case te::gm::MultiLineStringType:
      write(static_cast<const te::gm::MultiLineString*>(g), jgeom);
    break;

    case te::gm::MultiPolygonType:
      write(static_cast<const te::gm::MultiPolygon*>(g), jgeom);
    break;

    case te::gm::GeometryCollectionType:
      write(static_cast<const te::gm::GeometryCollection*>(g), jgeom);
    break;

    default :

      throw te::json::Exception(TR_JSON("This TerraLib geometry can not be converted to a GeoJSON geometry!"));
  }
}

void te::json::cpp::GeomWriter::write(const te::gm::Point* pt, Json::Value& jpt, bool onlyCoords)
{
  assert(pt);

  Json::Value coords(Json::arrayValue);
  coords.append(Json::Value(pt->getX()));
  coords.append(Json::Value(pt->getY()));

  if(onlyCoords)
  {
    jpt.append(coords);
    return;
  }

  jpt["type"] = Json::Value("Point");
  jpt["coordinates"] = coords;
}

void te::json::cpp::GeomWriter::write(const te::gm::LineString* line, Json::Value& jline, bool onlyCoords)
{
  assert(line);

  Json::Value coords(Json::arrayValue);

  write(line->getCoordinates(), line->getNPoints(), coords);

  if(onlyCoords)
  {
    jline.append(coords);
    return;
  }

  jline["type"] = Json::Value("LineString");
  jline["coordinates"] = coords;
}

void te::json::cpp::GeomWriter::write(const te::gm::Polygon* poly, Json::Value& jpoly, bool onlyCoords)
{
  assert(poly);

  Json::Value rings(Json::arrayValue);

  const std::size_t nrings = poly->getNumRings();

  for(std::size_t i = 0; i != nrings; ++i)
  {
    te::gm::LinearRing* ring = static_cast<te::gm::LinearRing*>(poly->getRingN(i));

    Json::Value line(Json::arrayValue);

    write(ring->getCoordinates(), ring->getNPoints(), line);

    rings.append(line);
  }

  if(onlyCoords)
  {
    jpoly.append(rings);
    return;
  }

  jpoly["type"] = Json::Value("Polygon");

  jpoly["coordinates"] = rings;
}

void te::json::cpp::GeomWriter::write(const te::gm::MultiPolygon* mpoly, Json::Value& jmpoly)
{
  assert(mpoly);

  jmpoly["type"] = Json::Value("MultiPolygon");

  Json::Value pols(Json::arrayValue);

  const std::size_t npols = mpoly->getNumGeometries();

  for(std::size_t i = 0; i != npols; ++i)
  {
    te::gm::Polygon* pol = static_cast<te::gm::Polygon*>(mpoly->getGeometryN(i));

    write(pol, pols, true);
  }

  jmpoly["coordinates"] = pols;
}

void te::json::cpp::GeomWriter::write(const te::gm::MultiLineString* mline, Json::Value& jmline)
{
  assert(mline);

  jmline["type"] = Json::Value("MultiLineString");

  Json::Value lines(Json::arrayValue);

  const std::size_t nlns = mline->getNumGeometries();

  for(std::size_t i = 0; i != nlns; ++i)
  {
    te::gm::LineString* l = static_cast<te::gm::LineString*>(mline->getGeometryN(i));

    write(l, lines, true);
  }

  jmline["coordinates"] = lines;
}

void te::json::cpp::GeomWriter::write(const te::gm::MultiPoint* mpt, Json::Value& jmpt)
{
  assert(mpt);

  jmpt["type"] = Json::Value("MultiPoint");

  Json::Value pts(Json::arrayValue);

  const std::size_t npts = mpt->getNumGeometries();

  for(std::size_t i = 0; i != npts; ++i)
  {
    te::gm::Point* pt = static_cast<te::gm::Point*>(mpt->getGeometryN(i));

    write(pt, pts, true);
  }

  jmpt["coordinates"] = pts;
}

void te::json::cpp::GeomWriter::write(const te::gm::GeometryCollection* geomcoll, Json::Value& jgeomcoll)
{
  assert(geomcoll);

  jgeomcoll["type"] = Json::Value("GeometryCollection");

  Json::Value geometries(Json::arrayValue);

  const std::size_t ngs = geomcoll->getNumGeometries();

  for(std::size_t i = 0; i != ngs; ++i)
  {
    Json::Value jgeom(Json::objectValue);

    te::gm::Geometry* g = geomcoll->getGeometryN(i);

    write(g, jgeom);

    geometries.append(jgeom);
  }

  jgeomcoll["geometries"] = geometries;
}

void te::json::cpp::GeomWriter::write(const te::gm::Coord2D* coords, std::size_t npts, Json::Value& jcoords)
{
  for(std::size_t i = 0; i != npts; ++i)
  {
    Json::Value coord(Json::arrayValue);

    coord.append(Json::Value(coords[i].x));
    coord.append(Json::Value(coords[i].y));

    jcoords.append(coord);
  }
}

