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
  \file terralib/geometry/GEOSWriter.cpp

  \brief A class that converts a TerraLib geometry to a GEOS geometry.
*/

// TerraLib
#include "GEOSWriter.h"

#ifdef TERRALIB_GEOS_ENABLED
// TerraLib
#include "../common/Translator.h"
#include "Exception.h"
#include "GeometryCollection.h"
#include "GEOSGeometryFactory.h"
#include "LineString.h"
#include "LinearRing.h"
#include "MultiLineString.h"
#include "MultiPoint.h"
#include "MultiPolygon.h"
#include "Point.h"
#include "PointM.h"
#include "PointZ.h"
#include "PointZM.h"
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

geos::geom::Geometry* te::gm::GEOSWriter::write(const Geometry* geom)
{
  assert(geom);

  switch(geom->getGeomTypeId())
  {
    case PointType:
      return write(static_cast<const Point*>(geom));

    case PointZType:
      return write(static_cast<const PointZ*>(geom));

    case PointMType:
      return write(static_cast<const PointM*>(geom));

    case PointZMType:
      return write(static_cast<const PointZM*>(geom));
      

    case LineStringType:
    case LineStringZType:
    case LineStringMType:
    case LineStringZMType:
      return write(static_cast<const LineString*>(geom));

    case PolygonType:
    case PolygonZType:
    case PolygonMType:
    case PolygonZMType:
      return write(static_cast<const Polygon*>(geom));

    case MultiPolygonType:
    case MultiPolygonZType:
    case MultiPolygonMType:
    case MultiPolygonZMType:
      return write(static_cast<const   MultiPolygon*>(geom));

    case MultiPointType:
    case MultiPointZType:
    case MultiPointMType:
    case MultiPointZMType:
      return write(static_cast<const  MultiPoint*>(geom));

    case MultiLineStringType:
    case MultiLineStringZType:
    case MultiLineStringMType:
    case MultiLineStringZMType:
      return write(static_cast<const  MultiLineString*>(geom));

    case GeometryCollectionType:
    case GeometryCollectionZType:
    case GeometryCollectionMType:
    case GeometryCollectionZMType:
      return write(static_cast<const  GeometryCollection*>(geom));

    /*case TriangleType:
    case TriangleZType:
    case TriangleMType:
    case TriangleZMType:
      return write(static_cast<const Triangle*>(geom));*/

    /*case TINType:
    case TINZType:
    case TINMType:
    case TINZMType:
      return write(static_cast<const TIN*>(geom));*/

    /*case PolyhedralSurfaceType:
    case PolyhedralSurfaceZType:
    case PolyhedralSurfaceMType:
    case PolyhedralSurfaceZMType:
      return write(static_cast<const PolyhedralSurface*>(geom));*/

    default:
      throw Exception(TE_TR("The type of informed geometry can not be converted from TerraLib to GEOS!"));
  }
}

geos::geom::Point* te::gm::GEOSWriter::write(const Point* tePt)
{
  assert((tePt != 0) && (tePt->getGeomTypeId() == PointType));
  
  geos::geom::Coordinate c(tePt->getX(), tePt->getY());

  geos::geom::Point* p = GEOSGeometryFactory::getGeomFactory()->createPoint(c);

  assert(p);

  p->setSRID(tePt->getSRID());

  return p;
}

geos::geom::Point* te::gm::GEOSWriter::write(const PointM* tePt)
{
  assert((tePt != 0) && (tePt->getGeomTypeId() == PointMType));
  
  geos::geom::Coordinate c(tePt->getX(), tePt->getY(), tePt->getM());

  geos::geom::Point* p = GEOSGeometryFactory::getGeomFactory()->createPoint(c);

  assert(p);

  p->setSRID(tePt->getSRID());

  return p;
}

geos::geom::Point* te::gm::GEOSWriter::write(const PointZ* tePt)
{
  assert((tePt != 0) && (tePt->getGeomTypeId() == PointZType));
  
  geos::geom::Coordinate c(tePt->getX(), tePt->getY(), tePt->getZ());

  geos::geom::Point* p = GEOSGeometryFactory::getGeomFactory()->createPoint(c);

  assert(p);

  p->setSRID(tePt->getSRID());

  return p;
}

geos::geom::Point* te::gm::GEOSWriter::write(const PointZM* tePt)
{
  assert((tePt != 0) && (tePt->getGeomTypeId() == PointZMType));
  
  geos::geom::Coordinate c(tePt->getX(), tePt->getY(), tePt->getZ());

  geos::geom::Point* p = GEOSGeometryFactory::getGeomFactory()->createPoint(c);

  assert(p);

  p->setSRID(tePt->getSRID());

  return p;
}

geos::geom::LineString* te::gm::GEOSWriter::write(const LineString* teLine)
{
  geos::geom::CoordinateSequence* cl = getCoordinateSequence(teLine);

  geos::geom::LineString* ls = GEOSGeometryFactory::getGeomFactory()->createLineString(cl);

  ls->setSRID(teLine->getSRID());

  return ls;
}

geos::geom::LinearRing* te::gm::GEOSWriter::write(const LinearRing* teRing)
{
  geos::geom::CoordinateSequence* cl = getCoordinateSequence(teRing);

  geos::geom::LinearRing* r = GEOSGeometryFactory::getGeomFactory()->createLinearRing(cl);

  r->setSRID(teRing->getSRID());

  return r;
}

geos::geom::Polygon* te::gm::GEOSWriter::write(const Polygon* tePoly)
{
  assert((tePoly != 0) && (tePoly->getGeomTypeId() == PolygonType ||
                           tePoly->getGeomTypeId() == PolygonMType ||
                           tePoly->getGeomTypeId() == PolygonZType ||
                           tePoly->getGeomTypeId() == PolygonZMType));

  std::size_t n = tePoly->getNumRings();

  if(n == 0)
  {
    geos::geom::Polygon* poly = GEOSGeometryFactory::getGeomFactory()->createPolygon();
    
    poly->setSRID(tePoly->getSRID());
    
    return poly;
  }

  std::vector<geos::geom::Geometry*>* holes = new std::vector<geos::geom::Geometry*>(n - 1);

  for(std::size_t i = 1; i < n; ++i)
    (*holes)[i - 1] = write(static_cast<const LinearRing*>(tePoly->getRingN(i)));

  geos::geom::LinearRing* outer = write(static_cast<const LinearRing*>(tePoly->getRingN(0)));

  geos::geom::Polygon* poly = GEOSGeometryFactory::getGeomFactory()->createPolygon(outer, holes);

  poly->setSRID(tePoly->getSRID());

  return poly;
}

geos::geom::MultiPolygon* te::gm::GEOSWriter::write(const MultiPolygon* teMPoly)
{
  assert((teMPoly != 0) && (teMPoly->getGeomTypeId() == MultiPolygonType ||
                            teMPoly->getGeomTypeId() == MultiPolygonMType ||
                            teMPoly->getGeomTypeId() == MultiPolygonZType ||
                            teMPoly->getGeomTypeId() == MultiPolygonZMType));

  std::vector<geos::geom::Geometry*>* geoms = getGeometries(teMPoly);

  geos::geom::MultiPolygon* mpoly = GEOSGeometryFactory::getGeomFactory()->createMultiPolygon(geoms);
  
  assert(mpoly);

  mpoly->setSRID(teMPoly->getSRID());

  return mpoly;
}

geos::geom::MultiLineString* te::gm::GEOSWriter::write(const MultiLineString* teMLine)
{
  assert((teMLine != 0) && (teMLine->getGeomTypeId() == MultiLineStringType ||
                            teMLine->getGeomTypeId() == MultiLineStringMType ||
                            teMLine->getGeomTypeId() == MultiLineStringZType ||
                            teMLine->getGeomTypeId() == MultiLineStringZMType));

  std::vector<geos::geom::Geometry*>* geoms = getGeometries(teMLine);

  geos::geom::MultiLineString* mline = GEOSGeometryFactory::getGeomFactory()->createMultiLineString(geoms);
  
  assert(mline);

  mline->setSRID(teMLine->getSRID());

  return mline;
}

geos::geom::MultiPoint* te::gm::GEOSWriter::write(const MultiPoint* teMPt)
{
  assert((teMPt != 0) && (teMPt->getGeomTypeId() == MultiPointType ||
                          teMPt->getGeomTypeId() == MultiPointMType ||
                          teMPt->getGeomTypeId() == MultiPointZType ||
                          teMPt->getGeomTypeId() == MultiPointZMType));

  std::vector<geos::geom::Geometry*>* geoms = getGeometries(teMPt);

  geos::geom::MultiPoint* mpt = GEOSGeometryFactory::getGeomFactory()->createMultiPoint(geoms);
  
  assert(mpt);

  mpt->setSRID(teMPt->getSRID());

  return mpt;
}

geos::geom::GeometryCollection* te::gm::GEOSWriter::write(const GeometryCollection* teGeomColl)
{
  assert((teGeomColl != 0) && (teGeomColl->getGeomTypeId() == GeometryCollectionType ||
                          teGeomColl->getGeomTypeId() == GeometryCollectionMType ||
                          teGeomColl->getGeomTypeId() == GeometryCollectionZType ||
                          teGeomColl->getGeomTypeId() == GeometryCollectionZMType));

  std::vector<geos::geom::Geometry*>* geoms = getGeometries(teGeomColl);

  geos::geom::GeometryCollection* geomColl= GEOSGeometryFactory::getGeomFactory()->createGeometryCollection(geoms);
  
  assert(geomColl);

  geomColl->setSRID(teGeomColl->getSRID());

  return geomColl;
}

geos::geom::CoordinateSequence* te::gm::GEOSWriter::getCoordinateSequence(const LineString* teLine)
{
  assert((teLine != 0) && (teLine->getGeomTypeId() == LineStringType ||
                           teLine->getGeomTypeId() == LineStringMType ||
                           teLine->getGeomTypeId() == LineStringZType ||
                           teLine->getGeomTypeId() == LineStringZMType));

  const std::size_t nPts = teLine->size();

  std::vector<geos::geom::Coordinate>* geosCoords = new std::vector<geos::geom::Coordinate>(nPts);

  Coord2D* teCoords = teLine->getCoordinates();

  for(std::size_t i = 0; i < nPts; ++i)
  {
    (*geosCoords)[i].x = teCoords[i].x;
    (*geosCoords)[i].y = teCoords[i].y;
  }

  geos::geom::CoordinateSequence* cl = new geos::geom::CoordinateArraySequence(geosCoords);

  return cl;
}

std::vector<geos::geom::Geometry*>* te::gm::GEOSWriter::getGeometries(const GeometryCollection* teGeomColl)
{
  assert(teGeomColl);

  std::size_t size = teGeomColl->getNumGeometries();

  std::vector<geos::geom::Geometry*>* geoms = new std::vector<geos::geom::Geometry*>(size);

  for(std::size_t i = 0; i < size; ++i)
  {
    geos::geom::Geometry* g = write(teGeomColl->getGeometryN(i));

    assert(g);

    (*geoms)[i] = g;
  }

  return geoms;
}

#endif  // TERRALIB_GEOS_ENABLED

