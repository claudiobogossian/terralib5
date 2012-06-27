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
  \file terralib/json/cpp/GeomReader.cpp

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

#ifdef TE_USE_LIB_JSONCPP

// JSONCpp
#include <json/json.h>

te::gm::Geometry* te::json::cpp::GeomReader::read(const Json::Value& jgeom)
{
  assert(jgeom.isObject());

  const Json::Value& geomtype = jgeom["type"];
  assert(geomtype.isNull() == false);

  const std::string gtype = boost::to_upper_copy(geomtype.asString());

  if(gtype == "GEOMETRYCOLLECTION")
  {
    const Json::Value& geometries = jgeom["geometries"];
    assert(geometries.isNull() == false);
    return readGeomCollection(geometries);
  }

  const Json::Value& coordinates = jgeom["coordinates"];
  assert(coordinates.isNull() == false);

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

te::gm::Point* te::json::cpp::GeomReader::readPoint(const Json::Value& jpt)
{
  assert(jpt.isArray());

  Json::Value::UInt ncoords = jpt.size();

  assert(ncoords >= 2 && ncoords <= 4);

  double x = jpt[0u].asDouble();
  double y = jpt[1u].asDouble();

  if(ncoords == 2)
  {
    return new te::gm::Point(x, y);
  }
  
  if(ncoords == 3)
  {
    double z = jpt[2u].asDouble();
    return new te::gm::PointZ(x, y, z);
  }
  
  if(ncoords == 4)  
  {
    double z = jpt[2u].asDouble();
    double m = jpt[3u].asDouble();
    return new te::gm::PointZM(x, y, z, m);
  }
  
  throw te::json::Exception(TR_JSON("Unknown dimension for GeoJSON point geometry!"));
}

te::gm::LineString* te::json::cpp::GeomReader::readLineString(const Json::Value& jline)
{
  assert(jline.isArray());

  Json::Value::UInt npts = jline.size();

  assert(npts >= 2);

// check the coord dimension of the first point
  const Json::Value& jpt = jline[0u];

  Json::Value::UInt ncoords = jpt.size();

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

void te::json::cpp::GeomReader::readLineString(te::gm::LineString* line, const Json::Value& jline)
{
  assert(jline.isArray());

  Json::Value::UInt npts = jline.size();

  assert(npts >= 2);

// check the coord dimension of the first point
  const Json::Value& jpt = jline[0u];

  Json::Value::UInt ncoords = jpt.size();

  assert(ncoords >= 2 && ncoords <= 4);

  if(ncoords == 2)
  {
    for(Json::Value::UInt i = 0; i != npts; ++i)
    {
      const Json::Value& jjpt = jline[i];

      assert((jjpt.isNull() == false) && (jjpt.size() == 2));

      double x = jjpt[0u].asDouble();
      double y = jjpt[1u].asDouble();

      line->setX(i, x);
      line->setY(i, y);
    }
  }
  else if(ncoords == 3)
  {
    for(Json::Value::UInt i = 0; i != npts; ++i)
    {
      const Json::Value& jjpt = jline[i];

      assert((jjpt.isNull() == false) && (jjpt.size() == 3));

      double x = jjpt[0u].asDouble();
      double y = jjpt[1u].asDouble();
      double z = jjpt[2u].asDouble();

      line->setX(i, x);
      line->setY(i, y);
      line->setZ(i, z);
    }
  }
  else if(ncoords == 4)
  {
    for(Json::Value::UInt i = 0; i != npts; ++i)
    {
      const Json::Value& jjpt = jline[i];

      assert((jjpt.isNull() == false) && (jjpt.size() == 4));

      double x = jjpt[0u].asDouble();
      double y = jjpt[1u].asDouble();
      double z = jjpt[2u].asDouble();
      double m = jjpt[3u].asDouble();

      line->setX(i, x);
      line->setY(i, y);
      line->setZ(i, z);
      line->setM(i, m);
    }
  }
  else
    throw te::json::Exception(TR_JSON("Unknown dimension for GeoJSON linestring geometry!"));
}

te::gm::Polygon* te::json::cpp::GeomReader::readPolygon(const Json::Value& jpoly)
{
  assert(jpoly.isArray());

  Json::Value::UInt nrings = jpoly.size();

  assert(nrings >= 1);

// check the coord dimension of the first point
  const Json::Value& jpt = jpoly[0u][0u];

  Json::Value::UInt ncoords = jpt.size();

  assert(ncoords >= 2 && ncoords <= 4);

  std::auto_ptr<te::gm::Polygon> poly;

  if(ncoords == 2)
  {
    poly.reset(new te::gm::Polygon(nrings, te::gm::PolygonType));

    for(Json::Value::UInt i = 0; i != nrings; ++i)
    {
      const Json::Value& ring = jpoly[i];
      Json::Value::UInt npts = ring.size();
      std::auto_ptr<te::gm::LinearRing> r(new te::gm::LinearRing(npts, te::gm::LineStringType));
      readLineString(r.get(), jpoly[i]);
      poly->setRingN(i, r.release());
    }
  }
  else if(ncoords == 3)
  {
    poly.reset(new te::gm::Polygon(nrings, te::gm::PolygonZType));

    for(Json::Value::UInt i = 0; i != nrings; ++i)
    {
      const Json::Value& ring = jpoly[i];
      Json::Value::UInt npts = ring.size();
      std::auto_ptr<te::gm::LinearRing> r(new te::gm::LinearRing(npts, te::gm::LineStringZType));
      readLineString(r.get(), jpoly[i]);
      poly->setRingN(i, r.release());
    }
  }
  else if(ncoords == 4)
  {
    poly.reset(new te::gm::Polygon(nrings, te::gm::PolygonZMType));

    for(Json::Value::UInt i = 0; i != nrings; ++i)
    {
      const Json::Value& ring = jpoly[i];
      Json::Value::UInt npts = ring.size();
      std::auto_ptr<te::gm::LinearRing> r(new te::gm::LinearRing(npts, te::gm::LineStringZMType));
      readLineString(r.get(), jpoly[i]);
      poly->setRingN(i, r.release());
    }
  }
  else
  {
    throw te::json::Exception(TR_JSON("Unknown dimension for GeoJSON polygon geometry!"));
  }

  return poly.release();
}

te::gm::MultiPolygon* te::json::cpp::GeomReader::readMPolygon(const Json::Value& jmpoly)
{
  assert(jmpoly.isArray());

  Json::Value::UInt npols = jmpoly.size();

  assert(npols >= 1);

  const Json::Value& jpoly = jmpoly[0u];

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

  for(Json::Value::UInt i = 1; i != npols; ++i)
  {
    const Json::Value& njpoly = jmpoly[i];
    te::gm::Polygon* npt = readPolygon(njpoly);
    assert(npt);
    mpoly->setGeometryN(i, npt);
  }

  return mpoly.release();
}

te::gm::MultiLineString* te::json::cpp::GeomReader::readMLineString(const Json::Value& jmline)
{
  assert(jmline.isArray());

  Json::Value::UInt nlns = jmline.size();

  assert(nlns >= 1);

  const Json::Value& jln = jmline[0u];

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

  for(Json::Value::UInt i = 1; i != nlns; ++i)
  {
    const Json::Value& njln = jmline[i];
    te::gm::LineString* nln = readLineString(njln);
    assert(nln);
    mln->setGeometryN(i, nln);
  }

  return mln.release();
}

te::gm::MultiPoint* te::json::cpp::GeomReader::readMPoint(const Json::Value& jmpt)
{
  assert(jmpt.isArray());

  Json::Value::UInt npts = jmpt.size();

  assert(npts >= 1);

  const Json::Value& jpt = jmpt[0u];

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

  for(Json::Value::UInt i = 1; i != npts; ++i)
  {
    const Json::Value& njpt = jmpt[i];
    te::gm::Point* npt = readPoint(njpt);
    assert(npt);
    mpt->setGeometryN(i, npt);
  }

  return mpt.release();
}

te::gm::GeometryCollection* te::json::cpp::GeomReader::readGeomCollection(const Json::Value& jgeomcoll)
{
  assert(jgeomcoll.isArray());

  Json::Value::UInt ngeoms = jgeomcoll.size();

  assert(ngeoms >= 1);

  const Json::Value& jgeom = jgeomcoll[0u];

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

  for(Json::Value::UInt i = 1; i != ngeoms; ++i)
  {
    const Json::Value& njgeom = jgeomcoll[i];
    te::gm::Geometry* ng = read(njgeom);
    assert(ng);
    gcoll->setGeometryN(i, ng);
  }

  return gcoll.release();
}

#endif  // TE_USE_LIB_JSONCPP

