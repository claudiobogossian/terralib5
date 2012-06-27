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
  \file terralib/yaml/GeomReader.cpp

  \brief A class that converts a YAML geometry to a TerraLib geometry.
*/

// TerraLib
#include "../common/Translator.h"
#include "../geometry/GeometryCollection.h"
#include "../geometry/LinearRing.h"
#include "../geometry/LineString.h"
#include "../geometry/MultiLineString.h"
#include "../geometry/MultiPoint.h"
#include "../geometry/MultiPolygon.h"
#include "../geometry/Point.h"
#include "../geometry/PointZ.h"
#include "../geometry/PointZM.h"
#include "../geometry/Polygon.h"
#include "Exception.h"
#include "GeomReader.h"

// STL
#include <cassert>

// Boost
#include <boost/algorithm/string/case_conv.hpp>

#ifdef TE_USE_LIB_YAMLCPP

te::gm::Geometry* te::yaml::GeomReader::read(const YAML::Node& ygeom)
{
  std::string geomtype;

  ygeom["type"] >> geomtype;

  boost::to_upper(geomtype);

  if(geomtype == "GEOMETRYCOLLECTION")
  {
    const YAML::Node& geometries = ygeom["geometries"];

    return readGeomCollection(geometries);
  }

  const YAML::Node& coordinates = ygeom["coordinates"];

  if(geomtype == "POINT")
    return readPoint(coordinates);
  else if(geomtype == "LINESTRING")
    return readLineString(coordinates);
  else if(geomtype == "POLYGON")
    return readPolygon(coordinates);
  else if(geomtype == "MULTIPOINT")
    return readMPoint(coordinates);
  else if(geomtype == "MULTILINESTRING")
    return readMLineString(coordinates);
  else if(geomtype == "MULTIPOLYGON")
    return readMPolygon(coordinates);
  else
    throw te::yaml::Exception(TR_YAML("Unknown YAML geometry!"));
}

te::gm::Point* te::yaml::GeomReader::readPoint(const YAML::Node& ypt)
{
  std::size_t ncoords = ypt.size();

  assert(ncoords >= 2 && ncoords <= 4);

  double x = 0.0;
  double y = 0.0;

  ypt[0u] >> x;
  ypt[1u] >> y;

  if(ncoords == 2)
    return new te::gm::Point(x, y);

  if(ncoords == 3)
  {
    double z = 0.0;
    
    ypt[2u] >> z;

    return new te::gm::PointZ(x, y, z);
  }

  if(ncoords == 4)
  {
    double z = 0.0;
    double m = 0.0;
    
    ypt[2u] >> z;
    ypt[3u] >> m;

    return new te::gm::PointZM(x, y, z, m);
  }

  throw te::yaml::Exception(TR_YAML("Unknown dimension for YAML point geometry!"));
}

te::gm::LineString* te::yaml::GeomReader::readLineString(const YAML::Node& yline)
{
  std::size_t npts = yline.size();

  assert(npts >= 2);

// check the coord dimension of the first point
  const YAML::Node& ypt = yline[0u];

  std::size_t ncoords = ypt.size();

  assert(ncoords >= 2 && ncoords <= 4);

  std::auto_ptr<te::gm::LineString> lstring;

  if(ncoords == 2)
    lstring.reset(new te::gm::LineString(npts, te::gm::LineStringType));
  else if(ncoords == 3)
    lstring.reset(new te::gm::LineString(npts, te::gm::LineStringZType));
  else if(ncoords == 4)
    lstring.reset(new te::gm::LineString(npts, te::gm::LineStringZMType));
  else
    throw te::yaml::Exception(TR_YAML("Unknown dimension for YAML LineString geometry!"));

  readLineString(lstring.get(), yline);

  return lstring.release();
}

void te::yaml::GeomReader::readLineString(te::gm::LineString* line, const YAML::Node& yline)
{
  std::size_t npts = yline.size();

  assert(npts >= 2);

// check the coord dimension of the first point
  const YAML::Node& ypt = yline[0u];

  std::size_t ncoords = ypt.size();

  assert(ncoords >= 2 && ncoords <= 4);

  if(ncoords == 2)
  {
    for(std::size_t i = 0; i != npts; ++i)
    {
      const YAML::Node& yypt = yline[i];

      assert(yypt.size() == 2);

      double x = 0.0;
      double y = 0.0;

      yypt[0u] >> x;
      yypt[1u] >> y;

      line->setX(i, x);
      line->setY(i, y);
    }
  }
  else if(ncoords == 3)
  {
    for(std::size_t i = 0; i != npts; ++i)
    {
      const YAML::Node& yypt = yline[i];

      assert(yypt.size() == 3);

      double x = 0.0;
      double y = 0.0;
      double z = 0.0;

      yypt[0u] >> x;
      yypt[1u] >> y;
      yypt[2u] >> z;

      line->setX(i, x);
      line->setY(i, y);
      line->setZ(i, z);
    }
  }
  else if(ncoords == 4)
  {
    for(std::size_t i = 0; i != npts; ++i)
    {
      const YAML::Node& yypt = yline[i];

      assert(yypt.size() == 4);

      double x = 0.0;
      double y = 0.0;
      double z = 0.0;
      double m = 0.0;

      yypt[0u] >> x;
      yypt[1u] >> y;
      yypt[2u] >> z;
      yypt[3u] >> m;

      line->setX(i, x);
      line->setY(i, y);
      line->setZ(i, z);
      line->setM(i, m);
    }
  }
  else
    throw te::yaml::Exception(TR_YAML("Unknown dimension for YAML linestring geometry!"));
}

te::gm::Polygon* te::yaml::GeomReader::readPolygon(const YAML::Node& ypoly)
{
  std::size_t nrings = ypoly.size();

  assert(nrings >= 1);

// check the coord dimension of the first point
  const YAML::Node& ypt = ypoly[0u][0u];

  std::size_t ncoords = ypt.size();

  assert(ncoords >= 2 && ncoords <= 4);

  std::auto_ptr<te::gm::Polygon> poly;

  if(ncoords == 2)
  {
    poly.reset(new te::gm::Polygon(nrings, te::gm::PolygonType));

    for(std::size_t i = 0; i != nrings; ++i)
    {
      const YAML::Node& ring = ypoly[i];
      std::size_t npts = ring.size();
      std::auto_ptr<te::gm::LinearRing> r(new te::gm::LinearRing(npts, te::gm::LineStringType));
      readLineString(r.get(), ypoly[i]);
      poly->setRingN(i, r.release());
    }
  }
  else if(ncoords == 3)
  {
    poly.reset(new te::gm::Polygon(nrings, te::gm::PolygonZType));

    for(std::size_t i = 0; i != nrings; ++i)
    {
      const YAML::Node& ring = ypoly[i];
      std::size_t npts = ring.size();
      std::auto_ptr<te::gm::LinearRing> r(new te::gm::LinearRing(npts, te::gm::LineStringZType));
      readLineString(r.get(), ypoly[i]);
      poly->setRingN(i, r.release());
    }
  }
  else if(ncoords == 4)
  {
    poly.reset(new te::gm::Polygon(nrings, te::gm::PolygonZMType));

    for(std::size_t i = 0; i != nrings; ++i)
    {
      const YAML::Node& ring = ypoly[i];
      std::size_t npts = ring.size();
      std::auto_ptr<te::gm::LinearRing> r(new te::gm::LinearRing(npts, te::gm::LineStringZMType));
      readLineString(r.get(), ypoly[i]);
      poly->setRingN(i, r.release());
    }
  }
  else
  {
    throw te::yaml::Exception(TR_YAML("Unknown dimension for YAML polygon geometry!"));
  }

  return poly.release();
}

te::gm::MultiPolygon* te::yaml::GeomReader::readMPolygon(const YAML::Node& ympoly)
{
  std::size_t npols = ympoly.size();

  assert(npols >= 1);

  const YAML::Node& ypoly = ympoly[0u];

  std::auto_ptr<te::gm::Polygon> pt(readPolygon(ypoly));

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
    throw te::yaml::Exception(TR_YAML("Unknown dimension for YAML multipolygon geometry!"));

  mpoly->setGeometryN(0, pt.release());

  for(std::size_t i = 1; i != npols; ++i)
  {
    const YAML::Node& nypoly = ympoly[i];
    te::gm::Polygon* npt = readPolygon(nypoly);
    assert(npt);
    mpoly->setGeometryN(i, npt);
  }

  return mpoly.release();
}

te::gm::MultiLineString* te::yaml::GeomReader::readMLineString(const YAML::Node& ymline)
{
  std::size_t nlns = ymline.size();

  assert(nlns >= 1);

  const YAML::Node& yln = ymline[0u];

  std::auto_ptr<te::gm::LineString> ln(readLineString(yln));

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
    throw te::yaml::Exception(TR_YAML("Unknown dimension for YAML multilinestring geometry!"));

  mln->setGeometryN(0, ln.release());

  for(std::size_t i = 1; i != nlns; ++i)
  {
    const YAML::Node& nyln = ymline[i];
    te::gm::LineString* nln = readLineString(nyln);
    assert(nln);
    mln->setGeometryN(i, nln);
  }

  return mln.release();
}

te::gm::MultiPoint* te::yaml::GeomReader::readMPoint(const YAML::Node& ympt)
{
  std::size_t npts = ympt.size();

  assert(npts >= 1);

  const YAML::Node& ypt = ympt[0u];

  std::auto_ptr<te::gm::Point> pt(readPoint(ypt));

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
    throw te::yaml::Exception(TR_YAML("Unknown dimension for YAML multipoint geometry!"));

  mpt->setGeometryN(0, pt.release());

  for(std::size_t i = 1; i != npts; ++i)
  {
    const YAML::Node& nypt = ympt[i];
    te::gm::Point* npt = readPoint(nypt);
    assert(npt);
    mpt->setGeometryN(i, npt);
  }

  return mpt.release();
}

te::gm::GeometryCollection* te::yaml::GeomReader::readGeomCollection(const YAML::Node& ygeomcoll)
{
  std::size_t ngeoms = ygeomcoll.size();

  assert(ngeoms >= 1);

  const YAML::Node& ygeom = ygeomcoll[0u];

  std::auto_ptr<te::gm::Geometry> g(read(ygeom));

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
    const YAML::Node& njgeom = ygeomcoll[i];
    te::gm::Geometry* ng = read(njgeom);
    assert(ng);
    gcoll->setGeometryN(i, ng);
  }

  return gcoll.release();
}

#endif  // TE_USE_LIB_YAMLCPP

