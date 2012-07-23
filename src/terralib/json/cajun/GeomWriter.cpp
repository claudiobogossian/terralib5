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
  \file terralib/json/cajun/GeomWriter.cpp

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

#ifdef TE_USE_LIB_CAJUN

void te::json::cajun::GeomWriter::write(const te::gm::Geometry* g, ::json::Object& jgeom)
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

void te::json::cajun::GeomWriter::write(const te::gm::Point* pt, ::json::Object& jpt)
{
  assert(pt);

  jpt["type"] = ::json::String("Point");

  ::json::Array coords;

  coords.Insert(::json::Number(pt->getX()));
  coords.Insert(::json::Number(pt->getY()));

  jpt["coordinates"] = coords;
}

void te::json::cajun::GeomWriter::write(const te::gm::LineString* line, ::json::Object& jline)
{
  assert(line);

  ::json::Array coords;

  write(line->getCoordinates(), line->getNPoints(), coords);

  jline["type"] = ::json::String("LineString");
  jline["coordinates"] = coords;
}

void te::json::cajun::GeomWriter::write(const te::gm::Polygon* poly, ::json::Object& jpoly)
{
  assert(poly);

  ::json::Array rings;

  write(poly, rings);

  jpoly["type"] = ::json::String("Polygon");

  jpoly["coordinates"] = rings;
}

void te::json::cajun::GeomWriter::write(const te::gm::MultiPolygon* mpoly, ::json::Object& jmpoly)
{
  assert(mpoly);

  jmpoly["type"] = ::json::String("MultiPolygon");

  ::json::Array pols;

  const std::size_t npols = mpoly->getNumGeometries();

  for(std::size_t i = 0; i != npols; ++i)
  {
    te::gm::Polygon* pol = static_cast<te::gm::Polygon*>(mpoly->getGeometryN(i));

    ::json::Array jpol;

    write(pol, jpol);

    pols.Insert(jpol);
  }

  jmpoly["coordinates"] = pols;
}

void te::json::cajun::GeomWriter::write(const te::gm::MultiLineString* mline, ::json::Object& jmline)
{
  assert(mline);

  jmline["type"] = ::json::String("MultiLineString");

  ::json::Array lines;

  const std::size_t nlns = mline->getNumGeometries();

  for(std::size_t i = 0; i != nlns; ++i)
  {
    te::gm::LineString* l = static_cast<te::gm::LineString*>(mline->getGeometryN(i));

    ::json::Array jline;

    write(l->getCoordinates(), l->getNPoints(), jline);

    lines.Insert(jline);
  }

  jmline["coordinates"] = lines;
}

void te::json::cajun::GeomWriter::write(const te::gm::MultiPoint* mpt, ::json::Object& jmpt)
{
  assert(mpt);

  jmpt["type"] = ::json::String("MultiPoint");

  ::json::Array pts;

  const std::size_t npts = mpt->getNumGeometries();

  for(std::size_t i = 0; i != npts; ++i)
  {
    te::gm::Point* pt = static_cast<te::gm::Point*>(mpt->getGeometryN(i));

    ::json::Array jpt;

    write(pt, jpt);

    pts.Insert(jpt);
  }

  jmpt["coordinates"] = pts;
}

void te::json::cajun::GeomWriter::write(const te::gm::GeometryCollection* geomcoll, ::json::Object& jgeomcoll)
{
  assert(geomcoll);

  jgeomcoll["type"] = ::json::String("GeometryCollection");

  ::json::Array geometries;

  const std::size_t ngs = geomcoll->getNumGeometries();

  for(std::size_t i = 0; i != ngs; ++i)
  {
    ::json::Object jgeom;

    te::gm::Geometry* g = geomcoll->getGeometryN(i);

    write(g, jgeom);

    geometries.Insert(jgeom);
  }

  jgeomcoll["geometries"] = geometries;
}

void te::json::cajun::GeomWriter::write(const te::gm::Point* pt, ::json::Array& jpt)
{
  assert(pt);

  jpt.Insert(::json::Number(pt->getX()));
  jpt.Insert(::json::Number(pt->getY()));
}

void te::json::cajun::GeomWriter::write(const te::gm::Polygon* poly, ::json::Array& rings)
{
  assert(poly);

  const std::size_t nrings = poly->getNumRings();

  for(std::size_t i = 0; i != nrings; ++i)
  {
    te::gm::LinearRing* ring = static_cast<te::gm::LinearRing*>(poly->getRingN(i));

    ::json::Array line;

    write(ring->getCoordinates(), ring->getNPoints(), line);

    rings.Insert(line);
  }
}

void te::json::cajun::GeomWriter::write(const te::gm::Coord2D* coords, std::size_t npts, ::json::Array& jcoords)
{
  for(std::size_t i = 0; i != npts; ++i)
  {
    ::json::Array coord;

    coord.Insert(::json::Number(coords[i].x));
    coord.Insert(::json::Number(coords[i].y));

    jcoords.Insert(coord);
  }
}

#endif // TE_USE_LIB_CAJUN

