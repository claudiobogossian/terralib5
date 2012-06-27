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
  \file terralib/json/cajun/GeomReader.cpp

  \brief A class that converts a GeoJSON geometry to a TerraLib geometry.
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
#include "GeomReader.h"

// STL
#include <cassert>

// Boost
#include <boost/algorithm/string/case_conv.hpp>

#ifdef TE_USE_LIB_CAJUN

te::gm::Geometry* te::json::cajun::GeomReader::read(const ::json::Object& jgeom)
{
  const ::json::String& geomtype = jgeom["type"];

  assert(geomtype.Value().empty() == false);

  const std::string gtype = boost::to_upper_copy(geomtype.Value());

  if(gtype == "GEOMETRYCOLLECTION")
  {
    const ::json::Array& geometries = jgeom["geometries"];

    assert(geometries.Empty() == false);

    return readGeomCollection(geometries);
  }

  const ::json::Array& coordinates = jgeom["coordinates"];

  assert(coordinates.Empty() == false);

  if(gtype == "POINT")
    return readPoint(coordinates);
  else if(gtype == "LINESTRING")
    return readLineString(coordinates);
  else if(gtype == "POLYGON")
    return readPolygon(coordinates);
  else if(gtype == "MULTIPOINT")
    return readMPoint(coordinates);
  else if(gtype == "MULTILINESTRING")
    return readMLineString(coordinates);
  else if(gtype == "MULTIPOLYGON")
    return readMPolygon(coordinates);
  else
    throw te::json::Exception(TR_JSON("Unknown GeoJSON geometry!"));
}

te::gm::Point* te::json::cajun::GeomReader::readPoint(const ::json::Array& jpt)
{
  assert(jpt.Empty() == false);

  std::size_t ncoords = jpt.Size();

  assert(ncoords >= 2 && ncoords <= 4);

  const ::json::Number& jx = jpt[0u];
  const ::json::Number& jy = jpt[1u];

  if(ncoords == 2)
  {
    return new te::gm::Point(jx.Value(), jy.Value());
  }
  
  if(ncoords == 3)
  {
    const ::json::Number& jz = jpt[2u];

    return new te::gm::PointZ(jx.Value(), jy.Value(), jz.Value());
  }
  
  if(ncoords == 4)  
  {
    const ::json::Number& jz = jpt[2u];
    const ::json::Number& jm = jpt[3u];

    return new te::gm::PointZM(jx.Value(), jy.Value(), jz.Value(), jm.Value());
  }
  
  throw te::json::Exception(TR_JSON("Unknown dimension for GeoJSON point geometry!"));
}

te::gm::LineString* te::json::cajun::GeomReader::readLineString(const ::json::Array& jline)
{
  assert(jline.Empty() == false);

  std::size_t npts = jline.Size();

  assert(npts >= 2);

// check the coord dimension of the first point
  const ::json::Array& jpt = jline[0u];

  std::size_t ncoords = jpt.Size();

  assert(ncoords >= 2 && ncoords <= 4);

  std::auto_ptr<te::gm::LineString> lstring;

  if(ncoords == 2)
    lstring.reset(new te::gm::LineString(npts, te::gm::LineStringType));
  else if(ncoords == 3)
    lstring.reset(new te::gm::LineString(npts, te::gm::LineStringZType));
  else if(ncoords == 4)
    lstring.reset(new te::gm::LineString(npts, te::gm::LineStringZMType));
  else
    throw te::json::Exception(TR_JSON("Unknown dimension for GeoJSON LineString geometry!"));

  readLineString(lstring.get(), jline);

  return lstring.release();
}

void te::json::cajun::GeomReader::readLineString(te::gm::LineString* line, const ::json::Array& jline)
{
  assert(jline.Empty() == false);

  std::size_t npts = jline.Size();

  assert(npts >= 2);

// check the coord dimension of the first point
  const ::json::Array& jpt = jline[0u];

  std::size_t ncoords = jpt.Size();

  assert(ncoords >= 2 && ncoords <= 4);

  if(ncoords == 2)
  {
    for(std::size_t i = 0; i != npts; ++i)
    {
      const ::json::Array& njpt = jline[i];

      assert(njpt.Size() == 2);

      const ::json::Number& jx = njpt[0u];
      const ::json::Number& jy = njpt[1u];

      line->setX(i, jx.Value());
      line->setY(i, jy.Value());
    }
  }
  else if(ncoords == 3)
  {
    for(std::size_t i = 0; i != npts; ++i)
    {
      const ::json::Array& njpt = jline[i];

      assert(njpt.Size() == 3);

      const ::json::Number& jx = njpt[0u];
      const ::json::Number& jy = njpt[1u];
      const ::json::Number& jz = njpt[2u];

      line->setX(i, jx.Value());
      line->setY(i, jy.Value());
      line->setZ(i, jz.Value());
    }
  }
  else if(ncoords == 4)
  {
    for(std::size_t i = 0; i != npts; ++i)
    {
      const ::json::Array& njpt = jline[i];

      assert(njpt.Size() == 4);

      const ::json::Number& jx = njpt[0u];
      const ::json::Number& jy = njpt[1u];
      const ::json::Number& jz = njpt[2u];
      const ::json::Number& jm = njpt[3u];

      line->setX(i, jx.Value());
      line->setY(i, jy.Value());
      line->setZ(i, jz.Value());
      line->setM(i, jm.Value());
    }
  }
  else
    throw te::json::Exception(TR_JSON("Unknown dimension for GeoJSON LineString geometry!"));
}

te::gm::Polygon* te::json::cajun::GeomReader::readPolygon(const ::json::Array& jpoly)
{
  assert(jpoly.Empty() == false);

  std::size_t nrings = jpoly.Size();

  assert(nrings >= 1);

// check the coord dimension of the first point
  const ::json::Array& jpt = jpoly[0u][0u];

  std::size_t ncoords = jpt.Size();

  assert(ncoords >= 2 && ncoords <= 4);

  std::auto_ptr<te::gm::Polygon> poly;

  if(ncoords == 2)
  {
    poly.reset(new te::gm::Polygon(nrings, te::gm::PolygonType));

    for(std::size_t i = 0; i != nrings; ++i)
    {
      const ::json::Array& ring = jpoly[i];
      std::size_t npts = ring.Size();
      std::auto_ptr<te::gm::LinearRing> r(new te::gm::LinearRing(npts, te::gm::LineStringType));
      readLineString(r.get(), ring);
      poly->setRingN(i, r.release());
    }
  }
  else if(ncoords == 3)
  {
    poly.reset(new te::gm::Polygon(nrings, te::gm::PolygonZType));

    for(std::size_t i = 0; i != nrings; ++i)
    {
      const ::json::Array& ring = jpoly[i];
      std::size_t npts = ring.Size();
      std::auto_ptr<te::gm::LinearRing> r(new te::gm::LinearRing(npts, te::gm::LineStringZType));
      readLineString(r.get(), ring);
      poly->setRingN(i, r.release());
    }
  }
  else if(ncoords == 4)
  {
    poly.reset(new te::gm::Polygon(nrings, te::gm::PolygonZMType));

    for(std::size_t i = 0; i != nrings; ++i)
    {
      const ::json::Array& ring = jpoly[i];
      std::size_t npts = ring.Size();
      std::auto_ptr<te::gm::LinearRing> r(new te::gm::LinearRing(npts, te::gm::LineStringZMType));
      readLineString(r.get(), ring);
      poly->setRingN(i, r.release());
    }
  }
  else
  {
    throw te::json::Exception(TR_JSON("Unknown dimension for GeoJSON polygon geometry!"));
  }

  return poly.release();
}

te::gm::MultiPolygon* te::json::cajun::GeomReader::readMPolygon(const ::json::Array& jmpoly)
{
  assert(jmpoly.Empty() == false);

  std::size_t npols = jmpoly.Size();

  assert(npols >= 1);

  const ::json::Array& jpoly = jmpoly[0u];

  std::auto_ptr<te::gm::Polygon> pt(readPolygon(jpoly));

  assert(pt.get() != 0);

  std::auto_ptr<te::gm::MultiPolygon> mpoly;

  te::gm::GeomType pttype = pt->getGeomTypeId();

  if(pttype == te::gm::PolygonType)
    mpoly.reset(new te::gm::MultiPolygon(npols, te::gm::MultiPolygonType));
  else if(pttype == te::gm::PolygonZType)
    mpoly.reset(new te::gm::MultiPolygon(npols, te::gm::MultiPolygonZType));
  else if(pttype == te::gm::PolygonZMType)
    mpoly.reset(new te::gm::MultiPolygon(npols, te::gm::MultiPolygonZMType));
  else
    throw te::json::Exception(TR_JSON("Unknown dimension for GeoJSON multipolygon geometry!"));

  mpoly->setGeometryN(0, pt.release());

  for(std::size_t i = 1; i != npols; ++i)
  {
    const ::json::Array& njpoly = jmpoly[i];
    te::gm::Polygon* npt = readPolygon(njpoly);
    assert(npt);
    mpoly->setGeometryN(i, npt);
  }

  return mpoly.release();
}

te::gm::MultiLineString* te::json::cajun::GeomReader::readMLineString(const ::json::Array& jmline)
{
  assert(jmline.Empty() == false);

  std::size_t nlns = jmline.Size();

  assert(nlns >= 1);

  const ::json::Array& jln = jmline[0u];

  std::auto_ptr<te::gm::LineString> ln(readLineString(jln));

  assert(ln.get() != 0);

  std::auto_ptr<te::gm::MultiLineString> mln;

  te::gm::GeomType lntype = ln->getGeomTypeId();

  if(lntype == te::gm::LineStringType)
    mln.reset(new te::gm::MultiLineString(nlns, te::gm::MultiLineStringType));
  else if(lntype == te::gm::LineStringZType)
    mln.reset(new te::gm::MultiLineString(nlns, te::gm::MultiLineStringZType));
  else if(lntype == te::gm::LineStringZMType)
    mln.reset(new te::gm::MultiLineString(nlns, te::gm::MultiLineStringZMType));
  else
    throw te::json::Exception(TR_JSON("Unknown dimension for GeoJSON multilinestring geometry!"));

  mln->setGeometryN(0, ln.release());

  for(std::size_t i = 1; i != nlns; ++i)
  {
    const ::json::Array& njln = jmline[i];
    te::gm::LineString* nln = readLineString(njln);
    assert(nln);
    mln->setGeometryN(i, nln);
  }

  return mln.release();
}

te::gm::MultiPoint* te::json::cajun::GeomReader::readMPoint(const ::json::Array& jmpt)
{
  assert(jmpt.Empty() == false);

  std::size_t npts = jmpt.Size();

  assert(npts >= 1);

  const ::json::Array& jpt = jmpt[0u];

  std::auto_ptr<te::gm::Point> pt(readPoint(jpt));

  assert(pt.get() != 0);

  std::auto_ptr<te::gm::MultiPoint> mpt;

  te::gm::GeomType pttype = pt->getGeomTypeId();

  if(pttype == te::gm::PointType)
    mpt.reset(new te::gm::MultiPoint(npts, te::gm::MultiPointType));
  else if(pttype == te::gm::PointZType)
    mpt.reset(new te::gm::MultiPoint(npts, te::gm::MultiPointZType));
  else if(pttype == te::gm::PointZMType)
    mpt.reset(new te::gm::MultiPoint(npts, te::gm::MultiPointZMType));
  else
    throw te::json::Exception(TR_JSON("Unknown dimension for GeoJSON multipoint geometry!"));

  mpt->setGeometryN(0, pt.release());

  for(std::size_t i = 1; i != npts; ++i)
  {
    const ::json::Array& njpt = jmpt[i];
    te::gm::Point* npt = readPoint(njpt);
    assert(npt);
    mpt->setGeometryN(i, npt);
  }

  return mpt.release();
}

te::gm::GeometryCollection* te::json::cajun::GeomReader::readGeomCollection(const ::json::Array& jgeomcoll)
{
  assert(jgeomcoll.Empty() == false);

  std::size_t ngeoms = jgeomcoll.Size();

  assert(ngeoms >= 1);

  const ::json::Object& jgeom = jgeomcoll[0u];

  std::auto_ptr<te::gm::Geometry> g(read(jgeom));

  assert(g.get() != 0);

  std::auto_ptr<te::gm::GeometryCollection> gcoll;

  bool is3D = g->is3D();
  bool isMeasured = g->isMeasured();

  if(!is3D)
    gcoll.reset(new te::gm::GeometryCollection(ngeoms, te::gm::GeometryCollectionType));
  else if(!isMeasured)  // is3D
    gcoll.reset(new te::gm::GeometryCollection(ngeoms, te::gm::GeometryCollectionZType));
  else // is3D and isMeasured
    gcoll.reset(new te::gm::GeometryCollection(ngeoms, te::gm::GeometryCollectionZMType));

  gcoll->setGeometryN(0, g.release());

  for(std::size_t i = 1; i != ngeoms; ++i)
  {
    const ::json::Object& njgeom = jgeomcoll[i];
    te::gm::Geometry* ng = read(njgeom);
    assert(ng);
    gcoll->setGeometryN(i, ng);
  }

  return gcoll.release();
}

#endif  // TE_USE_LIB_CAJUN

