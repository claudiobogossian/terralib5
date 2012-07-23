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
  \file terralib/json/rapid/GeomWriter.cpp

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

#ifdef TE_USE_LIB_RAPIDJSON

void te::json::rapid::GeomWriter::write(const te::gm::Geometry* g, ::rapidjson::Value& jgeom, ::rapidjson::Document::AllocatorType& allocator)
{
  assert(g);

  switch(g->getGeomTypeId())
  {
    case te::gm::PointType:
      write(static_cast<const te::gm::Point*>(g), jgeom, allocator);
    break;

    case te::gm::LineStringType:
      write(static_cast<const te::gm::LineString*>(g), jgeom, allocator);
    break;

    case te::gm::PolygonType:
      write(static_cast<const te::gm::Polygon*>(g), jgeom, allocator);
    break;

    case te::gm::MultiPointType:
      write(static_cast<const te::gm::MultiPoint*>(g), jgeom, allocator);
    break;

    case te::gm::MultiLineStringType:
      write(static_cast<const te::gm::MultiLineString*>(g), jgeom, allocator);
    break;

    case te::gm::MultiPolygonType:
      write(static_cast<const te::gm::MultiPolygon*>(g), jgeom, allocator);
    break;

    case te::gm::GeometryCollectionType:
      write(static_cast<const te::gm::GeometryCollection*>(g), jgeom, allocator);
    break;

    default :

      throw te::json::Exception(TR_JSON("This TerraLib geometry can not be converted to a GeoJSON geometry!"));
  }
}

void te::json::rapid::GeomWriter::write(const te::gm::Point* pt, ::rapidjson::Value& jpt, ::rapidjson::Document::AllocatorType& allocator)
{
  assert(pt);

  ::rapidjson::Value t("Point", 5, allocator);

  jpt.AddMember("type", t, allocator);

  ::rapidjson::Value coords;

  coords.SetArray();

  coords.PushBack(pt->getX(), allocator);
  coords.PushBack(pt->getY(), allocator);

  jpt.AddMember("coordinates", coords, allocator);
}

void te::json::rapid::GeomWriter::write(const te::gm::LineString* line, ::rapidjson::Value& jline, ::rapidjson::Document::AllocatorType& allocator)
{
  assert(line);

  ::rapidjson::Value t("LineString", 10, allocator);

  jline.AddMember("type", t, allocator);

  ::rapidjson::Value coords;

  coords.SetArray();

  copy(line->getCoordinates(), line->getNPoints(), coords, allocator);

  jline.AddMember("coordinates", coords, allocator);
}

void te::json::rapid::GeomWriter::write(const te::gm::Polygon* poly, ::rapidjson::Value& jpoly, ::rapidjson::Document::AllocatorType& allocator)
{
  assert(poly);

  ::rapidjson::Value t("Polygon", 7, allocator);

  jpoly.AddMember("type", t, allocator);

  ::rapidjson::Value rings;

  rings.SetArray();

  copy(poly, rings, allocator);

  jpoly.AddMember("coordinates", rings, allocator);
}

void te::json::rapid::GeomWriter::write(const te::gm::MultiPolygon* mpoly, ::rapidjson::Value& jmpoly, ::rapidjson::Document::AllocatorType& allocator)
{
  assert(mpoly);

  ::rapidjson::Value t("MultiPolygon", 12, allocator);

  jmpoly.AddMember("type", t, allocator);

  ::rapidjson::Value pols;

  pols.SetArray();

  const std::size_t npols = mpoly->getNumGeometries();

  for(std::size_t i = 0; i != npols; ++i)
  {
    te::gm::Polygon* pol = static_cast<te::gm::Polygon*>(mpoly->getGeometryN(i));

    ::rapidjson::Value jpol;

    jpol.SetArray();

    copy(pol, jpol, allocator);

    pols.PushBack(jpol, allocator);
  }

  jmpoly.AddMember("coordinates", pols, allocator);
}

void te::json::rapid::GeomWriter::write(const te::gm::MultiLineString* mline, ::rapidjson::Value& jmline, ::rapidjson::Document::AllocatorType& allocator)
{
  assert(mline);

  ::rapidjson::Value t("MultiLineString", 15, allocator);

  jmline.AddMember("type", t, allocator);

  ::rapidjson::Value lines;

  lines.SetArray();

  const std::size_t nlns = mline->getNumGeometries();

  for(std::size_t i = 0; i != nlns; ++i)
  {
    te::gm::LineString* l = static_cast<te::gm::LineString*>(mline->getGeometryN(i));

    ::rapidjson::Value jline;

    jline.SetArray();

    copy(l->getCoordinates(), l->getNPoints(), jline, allocator);

    lines.PushBack(jline, allocator);
  }

  jmline.AddMember("coordinates", lines, allocator);
}

void te::json::rapid::GeomWriter::write(const te::gm::MultiPoint* mpt, ::rapidjson::Value& jmpt, ::rapidjson::Document::AllocatorType& allocator)
{
  assert(mpt);

  ::rapidjson::Value t("MultiPoint", 10, allocator);

  jmpt.AddMember("type", t, allocator);

  ::rapidjson::Value pts;

  pts.SetArray();

  const std::size_t npts = mpt->getNumGeometries();

  for(std::size_t i = 0; i != npts; ++i)
  {
    te::gm::Point* pt = static_cast<te::gm::Point*>(mpt->getGeometryN(i));

    ::rapidjson::Value jpt;

    jpt.SetArray();

    copy(pt, jpt, allocator);

    pts.PushBack(jpt, allocator);
  }

  jmpt.AddMember("coordinates", pts, allocator);
}

void te::json::rapid::GeomWriter::write(const te::gm::GeometryCollection* geomcoll, ::rapidjson::Value& jgeomcoll, ::rapidjson::Document::AllocatorType& allocator)
{
  assert(geomcoll);

  ::rapidjson::Value t("GeometryCollection", 18, allocator);

  jgeomcoll.AddMember("type", t, allocator);

  ::rapidjson::Value geometries;

  geometries.SetArray();

  const std::size_t ngs = geomcoll->getNumGeometries();

  for(std::size_t i = 0; i != ngs; ++i)
  {
    ::rapidjson::Value jgeom;

    jgeom.SetObject();

    te::gm::Geometry* g = geomcoll->getGeometryN(i);

    write(g, jgeom, allocator);

    geometries.PushBack(jgeom, allocator);
  }

  jgeomcoll.AddMember("geometries", geometries, allocator);
}

void te::json::rapid::GeomWriter::copy(const te::gm::Point* pt, ::rapidjson::Value& jpt, ::rapidjson::Document::AllocatorType& allocator)
{
  assert(pt);

  jpt.PushBack(pt->getX(), allocator);
  jpt.PushBack(pt->getY(), allocator);
}

void te::json::rapid::GeomWriter::copy(const te::gm::Polygon* poly, ::rapidjson::Value& rings, ::rapidjson::Document::AllocatorType& allocator)
{
  assert(poly);

  const std::size_t nrings = poly->getNumRings();

  for(std::size_t i = 0; i != nrings; ++i)
  {
    te::gm::LinearRing* ring = static_cast<te::gm::LinearRing*>(poly->getRingN(i));

    ::rapidjson::Value line;

    line.SetArray();

    copy(ring->getCoordinates(), ring->getNPoints(), line, allocator);

    rings.PushBack(line, allocator);
  }
}

void te::json::rapid::GeomWriter::copy(const te::gm::Coord2D* coords, std::size_t npts, ::rapidjson::Value& jcoords, ::rapidjson::Document::AllocatorType& allocator)
{
  for(std::size_t i = 0; i != npts; ++i)
  {
    ::rapidjson::Value coord;

    coord.SetArray();

    coord.PushBack(coords[i].x, allocator);
    coord.PushBack(coords[i].y, allocator);

    jcoords.PushBack(coord, allocator);
  }
}

#endif // TE_USE_LIB_RAPIDJSON

