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
  \file terralib/geometry/GEOSReader.cpp

  \brief A class that converts a GEOS geometry to a TerraLib geometry.
*/

// TerraLib
#include "GEOSReader.h"

#ifdef TERRALIB_GEOS_ENABLED
// TerraLib
#include "../common/Translator.h"
#include "Exception.h"
#include "GeometryCollection.h"
#include "LineString.h"
#include "LinearRing.h"
#include "MultiLineString.h"
#include "MultiPoint.h"
#include "MultiPolygon.h"
#include "Point.h"
#include "Polygon.h"

// GEOS
#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateArraySequence.h>
#include <geos/geom/GeometryCollection.h>
#include <geos/geom/LineString.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/MultiPoint.h>
#include <geos/geom/MultiPolygon.h>
#include <geos/geom/Point.h>
#include <geos/geom/Polygon.h>

// STL
#include <cassert>

te::gm::Geometry* te::gm::GEOSReader::read(const geos::geom::Geometry* geom)
{
  assert(geom);

  switch(geom->getGeometryTypeId())
  {
    case geos::geom::GEOS_POINT:
      return read(dynamic_cast<const geos::geom::Point*>(geom));

    case geos::geom::GEOS_LINESTRING:
      return read(dynamic_cast<const geos::geom::LineString*>(geom));

    case geos::geom::GEOS_LINEARRING:
      return read(dynamic_cast<const geos::geom::LinearRing*>(geom));

    case geos::geom::GEOS_POLYGON:
      return read(dynamic_cast<const geos::geom::Polygon*>(geom));

    case geos::geom::GEOS_MULTIPOLYGON:
      return read(dynamic_cast<const geos::geom::MultiPolygon*>(geom));

    case geos::geom::GEOS_MULTIPOINT:
      return read(dynamic_cast<const geos::geom::MultiPoint*>(geom));

    case geos::geom::GEOS_MULTILINESTRING:
      return read(dynamic_cast<const geos::geom::MultiLineString*>(geom));

    case geos::geom::GEOS_GEOMETRYCOLLECTION:
      return read(dynamic_cast<const geos::geom::GeometryCollection*>(geom));

    default:
      throw Exception(TE_TR("The type of informed geometry can not be converted from GEOS to TerraLib!"));
  }
}

te::gm::Point* te::gm::GEOSReader::read(const geos::geom::Point* geosPt)
{
  assert((geosPt != 0) && (geosPt->getGeometryTypeId() == geos::geom::GEOS_POINT));

  Point* pt = new Point(geosPt->getX(), geosPt->getY(), geosPt->getSRID(), 0);

  return pt;
}

te::gm::LineString* te::gm::GEOSReader::read(const geos::geom::LineString* geosLine)
{
  assert((geosLine != 0) && (geosLine->getGeometryTypeId() == geos::geom::GEOS_LINESTRING));

  const geos::geom::CoordinateSequence* cs = geosLine->getCoordinatesRO();

  std::size_t size = cs->getSize();

  LineString* l = new LineString(size, LineStringType, geosLine->getSRID(), 0);
  
  for(std::size_t i = 0; i < size; ++i)
  {
    l->setX(i, cs->getX(i));
    l->setY(i, cs->getY(i));
  }

  return l;
}

te::gm::LinearRing* te::gm::GEOSReader::read(const geos::geom::LinearRing* geosRing)
{
  assert((geosRing != 0) && (geosRing->getGeometryTypeId() == geos::geom::GEOS_LINEARRING));

  const geos::geom::CoordinateSequence* cs = geosRing->getCoordinatesRO();

  std::size_t size = cs->getSize();

  LinearRing* r = new LinearRing(size, LineStringType, geosRing->getSRID(), 0);

  for(std::size_t i = 0; i < size; ++i)
  {
    r->setX(i, cs->getX(i));
    r->setY(i, cs->getY(i));
  } 
  
  return r;

}

te::gm::Polygon* te::gm::GEOSReader::read(const geos::geom::Polygon* geosPoly)
{
  assert((geosPoly != 0) && (geosPoly->getGeometryTypeId() == geos::geom::GEOS_POLYGON));

  if(geosPoly->isEmpty())
    return new Polygon(0, PolygonType, geosPoly->getSRID());

  std::size_t holesSize = geosPoly->getNumInteriorRing();

  Polygon* poly = new Polygon(holesSize + 1, PolygonType, geosPoly->getSRID(), 0);

  LinearRing* r = read(static_cast<const geos::geom::LinearRing*>(geosPoly->getExteriorRing()));

  poly->setRingN(0, r);

  for(std::size_t i = 0; i < holesSize; ++i)
  {
    LinearRing* r = read(static_cast<const geos::geom::LinearRing*>(geosPoly->getInteriorRingN(i)));

    poly->setRingN(i + 1, r);
  }

  return poly;
}

te::gm::MultiPolygon* te::gm::GEOSReader::read(const geos::geom::MultiPolygon* geosMPoly)
{
  assert((geosMPoly != 0) && (geosMPoly->getGeometryTypeId() == geos::geom::GEOS_MULTIPOLYGON));

  const std::size_t size = geosMPoly->getNumGeometries();

  MultiPolygon* mpoly = new MultiPolygon(size, MultiPolygonType, geosMPoly->getSRID(), 0);

  for(std::size_t i = 0; i < size; ++i)
    mpoly->setGeometryN(i, read(geosMPoly->getGeometryN(i)));

  return mpoly;
}

te::gm::MultiLineString* te::gm::GEOSReader::read(const geos::geom::MultiLineString* geosMLine)
{
  assert((geosMLine != 0) && (geosMLine->getGeometryTypeId() == geos::geom::GEOS_MULTILINESTRING));

  const std::size_t size = geosMLine->getNumGeometries();

  MultiLineString* mline = new MultiLineString(size, MultiLineStringType, geosMLine->getSRID(), 0);

  for(std::size_t i = 0; i < size; ++i)
    mline->setGeometryN(i, read(geosMLine->getGeometryN(i)));

  return mline;
}

te::gm::MultiPoint* te::gm::GEOSReader::read(const geos::geom::MultiPoint* geosMPt)
{
  assert((geosMPt != 0) && (geosMPt->getGeometryTypeId() == geos::geom::GEOS_MULTIPOINT));

  const std::size_t size = geosMPt->getNumGeometries();

  MultiPoint* mpt = new MultiPoint(size,MultiPointType, geosMPt->getSRID(), 0);

  for(std::size_t i = 0; i < size; ++i)
    mpt->setGeometryN(i, read(geosMPt->getGeometryN(i)));

  return mpt;
}

te::gm::GeometryCollection* te::gm::GEOSReader::read(const geos::geom::GeometryCollection* geosGeomColl)
{
  assert((geosGeomColl != 0) && (geosGeomColl->getGeometryTypeId() == geos::geom::GEOS_GEOMETRYCOLLECTION));

  const std::size_t size = geosGeomColl->getNumGeometries();

  GeometryCollection* geomColl = new GeometryCollection(size, GeometryCollectionType, geosGeomColl->getSRID(), 0);

  for(std::size_t i = 0; i < size; ++i)
    geomColl->setGeometryN(i, read(geosGeomColl->getGeometryN(i)));

  return geomColl;
}

#endif  // TERRALIB_GEOS_ENABLED

