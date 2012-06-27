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
  \file terralib/json/rapid/GeomReader.cpp

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

#ifdef TE_USE_LIB_RAPIDJSON

te::gm::Geometry* te::json::rapid::GeomReader::read(const ::rapidjson::Value& jgeom)
{
  const ::rapidjson::Value& geomtype = jgeom["type"];
  assert(geomtype.IsString());

  const char* fg = geomtype.GetString();

  const std::string gtype = boost::to_upper_copy(std::string(fg));

  if(gtype == "GEOMETRYCOLLECTION")
  {
    const ::rapidjson::Value& geometries = jgeom["geometries"];
    assert(geometries.IsArray());
    return readGeomCollection(geometries);
  }

  const ::rapidjson::Value& coordinates = jgeom["coordinates"];
  assert(coordinates.IsArray());

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

te::gm::Point* te::json::rapid::GeomReader::readPoint(const ::rapidjson::Value& jpt)
{
  assert(!jpt.Empty() && jpt.IsArray());

  std::size_t ncoords = jpt.Size();

  assert(ncoords >= 2 && ncoords <= 4);

  const ::rapidjson::Value& jx = jpt[0u];
  const ::rapidjson::Value& jy = jpt[1u];

  if(ncoords == 2)
  {
    return new te::gm::Point(jx.GetDouble(), jy.GetDouble());
  }

  if(ncoords == 3)
  {
    const ::rapidjson::Value& jz = jpt[2u];

    return new te::gm::PointZ(jx.GetDouble(), jy.GetDouble(), jz.GetDouble());
  }

  if(ncoords == 4)
  {
    const ::rapidjson::Value& jz = jpt[2u];
    const ::rapidjson::Value& jm = jpt[3u];

    return new te::gm::PointZM(jx.GetDouble(), jy.GetDouble(), jz.GetDouble(), jm.GetDouble());
  }

  throw te::json::Exception(TR_JSON("Unknown dimension for GeoJSON point geometry!"));
}

te::gm::LineString* te::json::rapid::GeomReader::readLineString(const ::rapidjson::Value& jline)
{
  assert(!jline.Empty() && jline.IsArray());

  std::size_t npts = jline.Size();

  assert(npts >= 2);

// check the coord dimension of the first point
  const ::rapidjson::Value& jpt = jline[0u];

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

void te::json::rapid::GeomReader::readLineString(te::gm::LineString* line, const ::rapidjson::Value& jline)
{
  assert(!jline.Empty() && jline.IsArray());

  std::size_t npts = jline.Size();

  assert(npts >= 2);

// check the coord dimension of the first point
  const ::rapidjson::Value& jpt = jline[0u];

  std::size_t ncoords = jpt.Size();

  assert(ncoords >= 2 && ncoords <= 4);

  if(ncoords == 2)
  {
    for(std::size_t i = 0; i != npts; ++i)
    {
      const ::rapidjson::Value& njpt = jline[i];

      assert(njpt.Size() == 2);

      const ::rapidjson::Value& jx = njpt[0u];
      const ::rapidjson::Value& jy = njpt[1u];

      line->setX(i, jx.GetDouble());
      line->setY(i, jy.GetDouble());
    }
  }
  else if(ncoords == 3)
  {
    for(std::size_t i = 0; i != npts; ++i)
    {
      const ::rapidjson::Value& njpt = jline[i];

      assert(njpt.Size() == 3);

      const ::rapidjson::Value& jx = njpt[0u];
      const ::rapidjson::Value& jy = njpt[1u];
      const ::rapidjson::Value& jz = njpt[2u];

      line->setX(i, jx.GetDouble());
      line->setY(i, jy.GetDouble());
      line->setZ(i, jz.GetDouble());
    }
  }
  else if(ncoords == 4)
  {
    for(std::size_t i = 0; i != npts; ++i)
    {
      const ::rapidjson::Value& njpt = jline[i];

      assert(njpt.Size() == 4);

      const ::rapidjson::Value& jx = njpt[0u];
      const ::rapidjson::Value& jy = njpt[1u];
      const ::rapidjson::Value& jz = njpt[2u];
      const ::rapidjson::Value& jm = njpt[3u];

      line->setX(i, jx.GetDouble());
      line->setY(i, jy.GetDouble());
      line->setZ(i, jz.GetDouble());
      line->setM(i, jm.GetDouble());
    }
  }
  else
    throw te::json::Exception(TR_JSON("Unknown dimension for GeoJSON LineString geometry!"));
}

te::gm::Polygon* te::json::rapid::GeomReader::readPolygon(const ::rapidjson::Value& jpoly)
{
  assert(!jpoly.Empty() && jpoly.IsArray());

  std::size_t nrings = jpoly.Size();

  assert(nrings >= 1);

// check the coord dimension of the first point
  const ::rapidjson::Value& jpt = jpoly[0u][0u];

  std::size_t ncoords = jpt.Size();

  assert(ncoords >= 2 && ncoords <= 4);

  std::auto_ptr<te::gm::Polygon> poly;

  if(ncoords == 2)
  {
    poly.reset(new te::gm::Polygon(nrings, te::gm::PolygonType));

    for(std::size_t i = 0; i != nrings; ++i)
    {
      const ::rapidjson::Value& ring = jpoly[i];
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
      const ::rapidjson::Value& ring = jpoly[i];
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
      const ::rapidjson::Value& ring = jpoly[i];
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

te::gm::MultiPolygon* te::json::rapid::GeomReader::readMPolygon(const ::rapidjson::Value& jmpoly)
{
  assert(!jmpoly.Empty() && jmpoly.IsArray());

  std::size_t npols = jmpoly.Size();

  assert(npols >= 1);

  const ::rapidjson::Value& jpoly = jmpoly[0u];

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
    const ::rapidjson::Value& njpoly = jmpoly[i];
    te::gm::Polygon* npt = readPolygon(njpoly);
    assert(npt);
    mpoly->setGeometryN(i, npt);
  }

  return mpoly.release();
}

te::gm::MultiLineString* te::json::rapid::GeomReader::readMLineString(const ::rapidjson::Value& jmline)
{
  assert(!jmline.Empty() && jmline.IsArray());

  std::size_t nlns = jmline.Size();

  assert(nlns >= 1);

  const ::rapidjson::Value& jln = jmline[0u];

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
    const ::rapidjson::Value& njln = jmline[i];
    te::gm::LineString* nln = readLineString(njln);
    assert(nln);
    mln->setGeometryN(i, nln);
  }

  return mln.release();
}

te::gm::MultiPoint* te::json::rapid::GeomReader::readMPoint(const ::rapidjson::Value& jmpt)
{
  assert(!jmpt.Empty() && jmpt.IsArray());

  std::size_t npts = jmpt.Size();

  assert(npts >= 1);

  const ::rapidjson::Value& jpt = jmpt[0u];

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
    const ::rapidjson::Value& njpt = jmpt[i];
    te::gm::Point* npt = readPoint(njpt);
    assert(npt);
    mpt->setGeometryN(i, npt);
  }

  return mpt.release();
}

te::gm::GeometryCollection* te::json::rapid::GeomReader::readGeomCollection(const ::rapidjson::Value& jgeomcoll)
{
  assert(jgeomcoll.Empty() == false);

  std::size_t ngeoms = jgeomcoll.Size();

  assert(ngeoms >= 1);

  const ::rapidjson::Value& jgeom = jgeomcoll[0u];

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
    const ::rapidjson::Value& njgeom = jgeomcoll[i];
    te::gm::Geometry* ng = read(njgeom);
    assert(ng);
    gcoll->setGeometryN(i, ng);
  }

  return gcoll.release();
}

#endif  // TE_USE_LIB_RAPIDJSON

