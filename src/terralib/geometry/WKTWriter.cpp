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
  \file terralib/geometry/WKTWriter.cpp

  \brief A class that serializes a geometry to the WKT format.
*/

// TerraLib
#include "../common/Translator.h"
#include "Exception.h"
#include "GeometryCollection.h"
#include "LinearRing.h"
#include "MultiLineString.h"
#include "MultiPoint.h"
#include "MultiPolygon.h"
#include "PointM.h"
#include "PointZ.h"
#include "PointZM.h"
#include "Polygon.h"
#include "PolyhedralSurface.h"
#include "WKTWriter.h"

// STL
#include <cassert>
#include <cstring>
#include <sstream>

te::gm::WKTWriter::WKTWriter(std::ostream& o)
  : m_ostream(o),
    m_tagged(true)
{
}

te::gm::WKTWriter::~WKTWriter()
{
}

void te::gm::WKTWriter::write(const Geometry* geom)
{
  geom->accept(*this);
}

void te::gm::WKTWriter::write(const Geometry& geom)
{
  geom.accept(*this);
}

void te::gm::WKTWriter::write(const Geometry* geom, std::ostream& o)
{
  WKTWriter w(o);
  w.write(geom);
}

void te::gm::WKTWriter::write(const Geometry& geom, std::ostream& o)
{
  WKTWriter w(o);
  w.write(geom);
}

void te::gm::WKTWriter::visit(const GeometryCollection& visited)
{
  if(m_tagged)
  {
    switch(visited.getGeomTypeId())
    {
      case GeometryCollectionType:
        m_ostream << "geometrycollection";
      break;

      case GeometryCollectionZType:
        m_ostream << "geometrycollection z";
      break;

      case GeometryCollectionMType:
        m_ostream << "geometrycollection m";
      break;

      case GeometryCollectionZMType:
        m_ostream << "geometrycollection zm";
      break;

      default:
      break;
    }
  }

  m_ostream << "(";

  std::size_t size = visited.getNumGeometries();

  for(std::size_t i = 0; i < size; ++i)
  {
    if(i != 0)
      m_ostream << ",";

    write(visited.getGeometryN(i));
  }

  m_ostream << ")";
}

void te::gm::WKTWriter::visit(const LinearRing& /*visited*/)
{
}

void te::gm::WKTWriter::visit(const LineString& visited)
{
  if(m_tagged)
  {
    switch(visited.getGeomTypeId())
    {
      case LineStringType:
        m_ostream << "linestring";
      break;

      case LineStringZType:
        m_ostream << "linestring z";
      break;

      case LineStringMType:
        m_ostream << "linestring m";
      break;

      case LineStringZMType:
        m_ostream << "linestring zm";
      break;

      default:
      break;
    }
  }

  m_ostream << "(";

  const std::size_t nPts = visited.size();

  if(visited.getZ() == 0 && visited.getM() == 0)
  {
    for(std::size_t i = 0; i < nPts; ++i)
    {
      if(i != 0)
        m_ostream << ",";

      m_ostream << visited.getCoordinates()[i].x << " " << visited.getCoordinates()[i].y;
    }
  }
  else if(visited.getM() == 0)
  {
    for(std::size_t i = 0; i < nPts; ++i)
    {
      if(i != 0)
        m_ostream << ",";

      m_ostream << visited.getCoordinates()[i].x << " " << visited.getCoordinates()[i].y << " "  << visited.getZ()[i];
    }
  }
  else if(visited.getZ() == 0)
  {
    for(std::size_t i = 0; i < nPts; ++i)
    {
      if(i != 0)
        m_ostream << ",";

      m_ostream << visited.getCoordinates()[i].x << " " << visited.getCoordinates()[i].y << " "  << visited.getM()[i];
    }
  }
  else
  {
    for(std::size_t i = 0; i < nPts; ++i)
    {
      if(i != 0)
        m_ostream << ",";

      m_ostream << visited.getCoordinates()[i].x << " " << visited.getCoordinates()[i].y << " "  << visited.getZ()[i] << " "  << visited.getM()[i];
    }
  }

  m_ostream << ")";
}

void te::gm::WKTWriter::visit(const MultiLineString& visited)
{
  if(m_tagged)
  {
    switch(visited.getGeomTypeId())
    {
      case MultiLineStringType:
        m_ostream << "multilinestring";
      break;

      case MultiLineStringZType:
        m_ostream << "multilinestring z";
      break;

      case MultiLineStringMType:
        m_ostream << "multilinestring m";
      break;

      case MultiLineStringZMType:
        m_ostream << "multilinestring zm";
      break;

      default:
      break;
    }
  }

  m_tagged = false;

  visit(static_cast<const GeometryCollection&>(visited));
}

void te::gm::WKTWriter::visit(const MultiPoint& visited)
{
  if(m_tagged)
  {
    switch(visited.getGeomTypeId())
    {
      case MultiPointType:
        m_ostream << "multipoint";
      break;

      case MultiPointZType:
        m_ostream << "multipoint z";
      break;

      case MultiPointMType:
        m_ostream << "multipoint m";
      break;

      case MultiPointZMType:
        m_ostream << "multipoint zm";
      break;

      default:
      break;
    }
  }

  m_tagged = false;

  visit(static_cast<const GeometryCollection&>(visited));
}

void te::gm::WKTWriter::visit(const MultiPolygon& visited)
{
  if(m_tagged)
  {
    switch(visited.getGeomTypeId())
    {
      case MultiPolygonType:
        m_ostream << "multipolygon";
        break;

      case MultiPolygonZType:
        m_ostream << "multipolygon z";
        break;

      case MultiPolygonMType:
        m_ostream << "multipolygon m";
        break;

      case MultiPolygonZMType:
        m_ostream << "multipolygon zm";
        break;

      default:
        break;
    }
  }

  m_tagged = false;

  visit(static_cast<const GeometryCollection&>(visited));
}

void te::gm::WKTWriter::visit(const Point& visited)
{
  if(m_tagged)
    m_ostream << "point";
  
  m_ostream << "(" << visited.getX() << " " << visited.getY() << ")";
}

void te::gm::WKTWriter::visit(const PointM& visited)
{
  if(m_tagged)
    m_ostream << "point m";
  
  m_ostream << "(" << visited.getX() << " " << visited.getY() << " " << visited.getM() << ")";
}

void te::gm::WKTWriter::visit(const PointZ& visited)
{
  if(m_tagged)
    m_ostream << "point z";
  
  m_ostream << "(" << visited.getX() << " " << visited.getY() << " " << visited.getZ() << ")";
}

void te::gm::WKTWriter::visit(const PointZM& visited)
{
  if(m_tagged)
    m_ostream << "point zm";

  m_ostream << "(" << visited.getX() << " " << visited.getY() << " " << visited.getZ() << " " << visited.getM() << ")";
}

void te::gm::WKTWriter::visit(const Polygon& visited)
{
  if(m_tagged)
  {
    switch(visited.getGeomTypeId())
    {
      case PolygonType:
        m_ostream << "polygon";
      break;

      case PolygonZType:
        m_ostream << "polygon z";
      break;

      case PolygonMType:
        m_ostream << "polygon m";
      break;

      case PolygonZMType:
        m_ostream << "polygon zm";
      break;

      default:
      break;
    }
  }

  m_tagged = false;

  m_ostream << "(";

  std::size_t size = visited.getNumRings();

  for(std::size_t i = 0; i < size; ++i)
  {
    if(i != 0)
      m_ostream << ",";

    visit((const LineString&)(*visited[i]));
  }

  m_ostream << ")";
}

void te::gm::WKTWriter::visit(const PolyhedralSurface& /*visited*/)
{
  /*
  if(tagged)
  {
    switch(m_gType)
    {
      case PolyhedralSurfaceType:
        o << "polyhedralsurface";
        break;

      case PolyhedralSurfaceZType:
        o << "polyhedralsurface z";
        break;

      case PolyhedralSurfaceMType:
        o << "polyhedralsurface m";
        break;

      case PolyhedralSurfaceZMType:
        o << "polyhedralsurface zm";
        break;

      default:
        break;
    }
  }

  o << "(";

  std::size_t size = m_polygons.size();

  for(std::size_t i = 0; i < size; ++i)
  {
    if(i != 0)
      o << ",";

    m_polygons[i]->asText(o, false);
  }

  o << ")";*/
}

void te::gm::WKTWriter::visit(const TIN& /*visited*/)
{
  /*
  if(tagged)
  {
    switch(m_gType)
    {
      case TINType:
        o << "tin";
        break;

      case TINZType:
        o << "tin z";
        break;

      case TINMType:
        o << "tin m";
        break;

      case TINZMType:
        o << "tin zm";
        break;

      default:
        break;
    }
  }

  PolyhedralSurface::asText(o, false);*/
}

void te::gm::WKTWriter::visit(const Triangle& /*visited*/)
{
  /*if(tagged)
  {
    switch(m_gType)
    {
      case TriangleType:
        o << "triangle";
        break;

      case TriangleZType:
        o << "triangle z";
        break;

      case TriangleMType:
        o << "triangle m";
        break;

      case TriangleZMType:
        o << "triangle zm";
        break;

      default:
        break;
    }
  }

  Polygon::asText(o, false);*/
}

