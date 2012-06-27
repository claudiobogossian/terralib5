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
  \file terralib/yaml/GeomWriter.cpp

  \brief A class that converts a TerraLib Geometry to YAML.
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
#include "GeomWriter.h"

// STL
#include <cassert>

#ifdef TE_USE_LIB_YAMLCPP

// yaml-cpp
#include <yaml-cpp/yaml.h>

void te::yaml::GeomWriter::write(const te::gm::Geometry* g, YAML::Emitter& emitter)
{
  assert(g);

  switch(g->getGeomTypeId())
  {
    case te::gm::PointType:
      write(static_cast<const te::gm::Point*>(g), emitter);
    break;

    case te::gm::LineStringType:
      write(static_cast<const te::gm::LineString*>(g), emitter);
    break;

    case te::gm::PolygonType:
      write(static_cast<const te::gm::Polygon*>(g), emitter);
    break;

    case te::gm::MultiPointType:
      write(static_cast<const te::gm::MultiPoint*>(g), emitter);
    break;

    case te::gm::MultiLineStringType:
      write(static_cast<const te::gm::MultiLineString*>(g), emitter);
    break;

    case te::gm::MultiPolygonType:
      write(static_cast<const te::gm::MultiPolygon*>(g), emitter);
    break;

    case te::gm::GeometryCollectionType:
      write(static_cast<const te::gm::GeometryCollection*>(g), emitter);
    break;

    default :

      throw te::yaml::Exception(TR_YAML("This TerraLib geometry can not be converted to a YAML geometry!"));
  }
}

void te::yaml::GeomWriter::write(const te::gm::Point* pt, YAML::Emitter& emitter, bool onlyCoords)
{
  assert(pt);

  if(!onlyCoords)
  {
    //emitter << YAML::BeginMap;

    emitter << YAML::Key << "type";
    emitter << YAML::Value << "Point";

    emitter << YAML::Key << "coordinates";
    emitter << YAML::Value;
  }

  emitter << YAML::Flow << YAML::BeginSeq << pt->getX() << pt->getY() << YAML::EndSeq;

  //if(!onlyCoords)
    //emitter << YAML::EndMap;
}

void te::yaml::GeomWriter::write(const te::gm::LineString* line, YAML::Emitter& emitter, bool onlyCoords)
{
  assert(line);

  if(!onlyCoords)
  {
    //emitter << YAML::BeginMap;

    emitter << YAML::Key << "type";
    emitter << YAML::Value << "LineString";

    emitter << YAML::Key << "coordinates";
    emitter << YAML::Value;
  }

  write(line->getCoordinates(), line->getNPoints(), emitter);

  //if(!onlyCoords)
  //  emitter << YAML::EndMap;
}

void te::yaml::GeomWriter::write(const te::gm::Polygon* poly, YAML::Emitter& emitter, bool onlyCoords)
{
  assert(poly);

  if(!onlyCoords)
  {
    //emitter << YAML::BeginMap;

    emitter << YAML::Key << "type";
    emitter << YAML::Value << "Polygon";

    emitter << YAML::Key << "coordinates";
    emitter << YAML::Value;
  }

  const std::size_t nrings = poly->getNumRings();

  for(std::size_t i = 0; i != nrings; ++i)
  {
    emitter << YAML::Flow << YAML::BeginSeq;

    te::gm::LinearRing* ring = static_cast<te::gm::LinearRing*>(poly->getRingN(i));

    write(ring->getCoordinates(), ring->getNPoints(), emitter);

    emitter << YAML::EndSeq;
  }

  //if(!onlyCoords)
    //emitter << YAML::EndMap;
}

void te::yaml::GeomWriter::write(const te::gm::MultiPolygon* mpoly, YAML::Emitter& emitter)
{
  assert(mpoly);

  //emitter << YAML::BeginMap;

  emitter << YAML::Key << "type";
  emitter << YAML::Value << "MultiPolygon";

  emitter << YAML::Key << "coordinates";
  emitter << YAML::Value;

  const std::size_t npols = mpoly->getNumGeometries();

  for(std::size_t i = 0; i != npols; ++i)
  {
    te::gm::Polygon* pol = static_cast<te::gm::Polygon*>(mpoly->getGeometryN(i));

    write(pol, emitter, true);
  }

  //emitter << YAML::EndMap;
}

void te::yaml::GeomWriter::write(const te::gm::MultiLineString* mline, YAML::Emitter& emitter)
{
  assert(mline);

  //emitter << YAML::BeginMap;

  emitter << YAML::Key << "type";
  emitter << YAML::Value << "MultiLineString";

  emitter << YAML::Key << "coordinates";
  emitter << YAML::Value;

  const std::size_t nlns = mline->getNumGeometries();

  for(std::size_t i = 0; i != nlns; ++i)
  {
    te::gm::LineString* l = static_cast<te::gm::LineString*>(mline->getGeometryN(i));

    write(l, emitter, true);
  }

  //emitter << YAML::EndMap;
}

void te::yaml::GeomWriter::write(const te::gm::MultiPoint* mpt, YAML::Emitter& emitter)
{
  assert(mpt);

  //emitter << YAML::BeginMap;

  emitter << YAML::Key << "type";
  emitter << YAML::Value << "MultiLineString";

  emitter << YAML::Key << "coordinates";
  emitter << YAML::Value;

  const std::size_t npts = mpt->getNumGeometries();

  for(std::size_t i = 0; i != npts; ++i)
  {
    te::gm::Point* pt = static_cast<te::gm::Point*>(mpt->getGeometryN(i));

    write(pt, emitter, true);
  }

  //emitter << YAML::EndMap;
}

void te::yaml::GeomWriter::write(const te::gm::GeometryCollection* geomcoll, YAML::Emitter& emitter)
{
  assert(geomcoll);

  //emitter << YAML::BeginMap;

  emitter << YAML::Key << "type";
  emitter << YAML::Value << "GeometryCollection";

  emitter << YAML::Key << "geometries";

  emitter << YAML::Value << YAML::BeginSeq;

  const std::size_t ngs = geomcoll->getNumGeometries();

  for(std::size_t i = 0; i != ngs; ++i)
  {
    emitter << YAML::BeginMap;

    te::gm::Geometry* g = geomcoll->getGeometryN(i);

    write(g, emitter);

    emitter << YAML::EndMap;
  }

  emitter << YAML::EndSeq;
}

void te::yaml::GeomWriter::write(const te::gm::Coord2D* coords, std::size_t npts, YAML::Emitter& emitter)
{
  emitter << YAML::Flow << YAML::BeginSeq;

  for(std::size_t i = 0; i != npts; ++i)
    emitter << YAML::Flow << YAML::BeginSeq << coords[i].x << coords[i].y << YAML::EndSeq;

  emitter << YAML::EndSeq;
}

#endif  // TE_USE_LIB_YAMLCPP

