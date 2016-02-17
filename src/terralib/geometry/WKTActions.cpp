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
   \file terralib/geometry/WKTActions.cpp

  \brief A class that implements the semanthic actions to grammar rules for well known text (Wkt) format for Geometry.
         Basically, the class is responsible to generate a geometry element.
*/

// TerraLib
#include "Geometry.h"
#include "GeometryCollection.h"
#include "LineString.h"
#include "LinearRing.h"
#include "Point.h"
#include "PointM.h"
#include "PointZ.h"
#include "PointZM.h"
#include "Polygon.h"
#include "PolyhedralSurface.h"
#include "MultiLineString.h"
#include "MultiPoint.h"
#include "MultiPolygon.h"
#include "TIN.h"
#include "Triangle.h"
#include "WKTActions.h"
#include "../common/STLUtils.h"

te::gm::WKTActions::WKTActions() :
m_g(0)
{}

te::gm::WKTActions::~WKTActions()
{}

void te::gm::WKTActions::reset()
{
  m_g = 0;

  te::common::FreeContents(m_points);
  m_points.clear();
  
  te::common::FreeContents(m_lines);
  m_lines.clear();
  
  te::common::FreeContents(m_polygons);
  m_polygons.clear();

  te::common::FreeContents(m_geometries);
  m_geometries.clear();
}

void te::gm::WKTActions::createPoint(const boost::fusion::vector2<double, double>& c)
{
  te::gm::Point* p = new te::gm::Point(boost::fusion::at_c<0>(c), boost::fusion::at_c<1>(c));
  m_points.push_back(p);
  m_g = p;
}

void te::gm::WKTActions::createPointZ(const boost::fusion::vector3<double, double, double>& c)
{
  te::gm::PointZ* p = new te::gm::PointZ(boost::fusion::at_c<0>(c), boost::fusion::at_c<1>(c), boost::fusion::at_c<2>(c));
  m_points.push_back(p);
  m_g = p;
}

void te::gm::WKTActions::createPointM(const boost::fusion::vector3<double, double, double>& c)
{
  te::gm::PointM* p = new te::gm::PointM(boost::fusion::at_c<0>(c), boost::fusion::at_c<1>(c), boost::fusion::at_c<2>(c));
  m_points.push_back(p);
  m_g = p;
}

void te::gm::WKTActions::createPointZM(const boost::fusion::vector4<double, double, double, double>& c)
{
  te::gm::PointZM* p = new te::gm::PointZM(boost::fusion::at_c<0>(c), boost::fusion::at_c<1>(c), boost::fusion::at_c<2>(c), boost::fusion::at_c<3>(c));
  m_points.push_back(p);
  m_g = p;
}

void te::gm::WKTActions::createPoint()
{
  te::gm::Point* p = new te::gm::Point;
  m_points.push_back(p);
  m_g = p;
}

void te::gm::WKTActions::createPointZ()
{
  te::gm::PointZ* p = new te::gm::PointZ;
  m_points.push_back(p);
  m_g = p;
}

void te::gm::WKTActions::createPointM()
{
  te::gm::PointM* p = new te::gm::PointM;
  m_points.push_back(p);
  m_g = p;
}

void te::gm::WKTActions::createPointZM()
{
  te::gm::PointZM* p = new te::gm::PointZM;
  m_points.push_back(p);
  m_g = p;
}

void te::gm::WKTActions::createLine()
{
  te::gm::LineString* l = new te::gm::LineString(m_points.size(), te::gm::LineStringType);
  buildLine(l);
}

void te::gm::WKTActions::createLineZ()
{
  te::gm::LineString* l = new te::gm::LineString(m_points.size(), te::gm::LineStringZType);
  buildLine(l);
}

void te::gm::WKTActions::createLineM()
{
  te::gm::LineString* l = new te::gm::LineString(m_points.size(), te::gm::LineStringMType);
  buildLine(l);
}

void te::gm::WKTActions::createLineZM()
{
  te::gm::LineString* l = new te::gm::LineString(m_points.size(), te::gm::LineStringZMType);
  buildLine(l);
}

void te::gm::WKTActions::createLinearRing()
{
  te::gm::LineString* l = new te::gm::LinearRing(m_points.size(), te::gm::LineStringType);
  buildLine(l);
}

void te::gm::WKTActions::createLinearRingZ()
{
  te::gm::LineString* l = new te::gm::LinearRing(m_points.size(), te::gm::LineStringZType);
  buildLine(l);
}

void te::gm::WKTActions::createLinearRingM()
{
  te::gm::LineString* l = new te::gm::LinearRing(m_points.size(), te::gm::LineStringMType);
  buildLine(l);
}

void te::gm::WKTActions::createLinearRingZM()
{
  te::gm::LineString* l = new te::gm::LinearRing(m_points.size(), te::gm::LineStringZMType);
  buildLine(l);
}

void te::gm::WKTActions::createPolygon()
{
  te::gm::Polygon* p = new te::gm::Polygon(m_lines.size(), te::gm::PolygonType);
  buildPolygon(p);
}

void te::gm::WKTActions::createPolygonZ()
{
  te::gm::Polygon* p = new te::gm::Polygon(m_lines.size(), te::gm::PolygonZType);
  buildPolygon(p);
}

void te::gm::WKTActions::createPolygonM()
{
  te::gm::Polygon* p = new te::gm::Polygon(m_lines.size(), te::gm::PolygonMType);
  buildPolygon(p);
}

void te::gm::WKTActions::createPolygonZM()
{
  te::gm::Polygon* p = new te::gm::Polygon(m_lines.size(), te::gm::PolygonZMType);
  buildPolygon(p);
}

void te::gm::WKTActions::createMultiPoint()
{
  buildMultiPoint(te::gm::MultiPointType);
}

void te::gm::WKTActions::createMultiPointZ()
{
  buildMultiPoint(te::gm::MultiPointZType);
}

void te::gm::WKTActions::createMultiPointM()
{
  buildMultiPoint(te::gm::MultiPointMType);
}

void te::gm::WKTActions::createMultiPointZM()
{
  buildMultiPoint(te::gm::MultiPointZMType);
}

void te::gm::WKTActions::createMultiLineString()
{
  buildMultiLineString(te::gm::MultiLineStringType);
}

void te::gm::WKTActions::createMultiLineStringZ()
{
  buildMultiLineString(te::gm::MultiLineStringZType);
}

void te::gm::WKTActions::createMultiLineStringM()
{
  buildMultiLineString(te::gm::MultiLineStringMType);
}

void te::gm::WKTActions::createMultiLineStringZM()
{
  buildMultiLineString(te::gm::MultiLineStringZMType);
}

void te::gm::WKTActions::createMultiPolygon()
{
  buildMultiPolygon(te::gm::MultiPolygonType);
}

void te::gm::WKTActions::createMultiPolygonZ()
{
  buildMultiPolygon(te::gm::MultiPolygonZType);
}

void te::gm::WKTActions::createMultiPolygonM()
{
  buildMultiPolygon(te::gm::MultiPolygonMType);
}

void te::gm::WKTActions::createMultiPolygonZM()
{
  buildMultiPolygon(te::gm::MultiPolygonZMType);
}

void te::gm::WKTActions::createMultiSurface()
{
  buildMultiSurface(te::gm::MultiSurfaceType);
}

void te::gm::WKTActions::createMultiSurfaceZ()
{
  buildMultiSurface(te::gm::MultiSurfaceZType);
}

void te::gm::WKTActions::createMultiSurfaceM()
{
  buildMultiSurface(te::gm::MultiSurfaceMType);
}

void te::gm::WKTActions::createMultiSurfaceZM()
{
  buildMultiSurface(te::gm::MultiSurfaceZMType);
}

void te::gm::WKTActions::createPolyhedralSurface()
{
  te::gm::PolyhedralSurface* ps = new te::gm::PolyhedralSurface(m_polygons.size(), te::gm::PolyhedralSurfaceType);
  buildPolyhedralSurface(ps);
}

void te::gm::WKTActions::createPolyhedralSurfaceZ()
{
  te::gm::PolyhedralSurface* ps = new te::gm::PolyhedralSurface(m_polygons.size(), te::gm::PolyhedralSurfaceZType);
  buildPolyhedralSurface(ps);
}

void te::gm::WKTActions::createPolyhedralSurfaceM()
{
  te::gm::PolyhedralSurface* ps = new te::gm::PolyhedralSurface(m_polygons.size(), te::gm::PolyhedralSurfaceMType);
  buildPolyhedralSurface(ps);
}

void te::gm::WKTActions::createPolyhedralSurfaceZM()
{
  te::gm::PolyhedralSurface* ps = new te::gm::PolyhedralSurface(m_polygons.size(), te::gm::PolyhedralSurfaceZMType);
  buildPolyhedralSurface(ps);
}

void te::gm::WKTActions::createTIN()
{
  te::gm::PolyhedralSurface* tin = new te::gm::TIN(m_polygons.size(), te::gm::TINType);
  buildPolyhedralSurface(tin);
}

void te::gm::WKTActions::createTINZ()
{
  te::gm::PolyhedralSurface* tin = new te::gm::TIN(m_polygons.size(), te::gm::TINZType);
  buildPolyhedralSurface(tin);
}

void te::gm::WKTActions::createTINM()
{
  te::gm::PolyhedralSurface* tin = new te::gm::TIN(m_polygons.size(), te::gm::TINMType);
  buildPolyhedralSurface(tin);
}

void te::gm::WKTActions::createTINZM()
{
  te::gm::PolyhedralSurface* tin = new te::gm::TIN(m_polygons.size(), te::gm::TINZMType);
  buildPolyhedralSurface(tin);
}

void te::gm::WKTActions::createTriangle()
{
  te::gm::Polygon* t = new te::gm::Triangle(m_lines.size(), te::gm::TriangleType);
  buildPolygon(t);
}

void te::gm::WKTActions::createTriangleZ()
{
  te::gm::Polygon* t = new te::gm::Triangle(m_lines.size(), te::gm::TriangleZType);
  buildPolygon(t);
}

void te::gm::WKTActions::createTriangleM()
{
  te::gm::Polygon* t = new te::gm::Triangle(m_lines.size(), te::gm::TriangleMType);
  buildPolygon(t);
}

void te::gm::WKTActions::createTriangleZM()
{
  te::gm::Polygon* t = new te::gm::Triangle(m_lines.size(), te::gm::TriangleZMType);
  buildPolygon(t);
}

void te::gm::WKTActions::createGeometryCollection()
{
  buildGeometryCollection(te::gm::GeometryCollectionType);
}

void te::gm::WKTActions::createGeometryCollectionZ()
{
  buildGeometryCollection(te::gm::GeometryCollectionZType);
}

void te::gm::WKTActions::createGeometryCollectionM()
{
  buildGeometryCollection(te::gm::GeometryCollectionMType);
}

void te::gm::WKTActions::createGeometryCollectionZM()
{
  buildGeometryCollection(te::gm::GeometryCollectionZMType);
}

void te::gm::WKTActions::endGeometry()
{
  m_points.clear();
  m_lines.clear();
  m_polygons.clear();
  m_geometries.push_back(m_g);
}

void te::gm::WKTActions::end()
{
  m_geometries.clear();
}
  
void te::gm::WKTActions::buildLine(te::gm::LineString* l)
{
  for(std::size_t i = 0; i < l->getNPoints(); ++i)
    l->setPointN(i, *m_points[i]);
    
  m_lines.push_back(l);
    
  te::common::FreeContents(m_points);
  m_points.clear();

  m_g = l;
}

void te::gm::WKTActions::buildPolygon(te::gm::Polygon* p)
{
  std::size_t size = m_lines.size();
  for(std::size_t i = 0; i < size; ++i)
    p->setRingN(i, dynamic_cast<te::gm::LinearRing*>(m_lines[i]));

  m_lines.clear();

  m_polygons.push_back(p);

  m_g = p;
}

void te::gm::WKTActions::buildPolygon(const te::gm::GeomType& t)
{
  std::size_t size = m_lines.size();
  te::gm::Polygon* p = new te::gm::Polygon(size, t);

  for(std::size_t i = 0; i < size; ++i)
    p->setRingN(i, dynamic_cast<te::gm::LinearRing*>(m_lines[i]));

  m_lines.clear();

  m_polygons.push_back(p);

  m_g = p;
}

void te::gm::WKTActions::buildMultiPoint(const te::gm::GeomType& t)
{
  std::size_t size = m_points.size();
  te::gm::MultiPoint* mp = new te::gm::MultiPoint(size, t);

  for(std::size_t i = 0; i < size; ++i)
    mp->setGeometryN(i, m_points[i]);

  m_points.clear();

  m_g = mp;
}

void te::gm::WKTActions::buildMultiLineString(const te::gm::GeomType& t)
{
  std::size_t size = m_lines.size();
  te::gm::MultiLineString* ml = new te::gm::MultiLineString(size, t);

  for(std::size_t i = 0; i < size; ++i)
    ml->setGeometryN(i, m_lines[i]);

  m_lines.clear();

  m_g = ml;
}

void te::gm::WKTActions::buildMultiPolygon(const te::gm::GeomType& t)
{
  std::size_t size = m_polygons.size();
  te::gm::MultiPolygon* mp = new te::gm::MultiPolygon(size, t);

  for(std::size_t i = 0; i < size; ++i)
    mp->setGeometryN(i, m_polygons[i]);

  m_polygons.clear();

  m_g = mp;
}

void te::gm::WKTActions::buildMultiSurface(const te::gm::GeomType& t)
{
  std::size_t size = m_geometries.size();
  te::gm::MultiSurface* ms = new te::gm::MultiSurface(size, t);
  for (std::size_t i = 0; i < size; ++i)
    ms->setGeometryN(i, m_geometries[i]);

  m_geometries.clear();

  m_g = ms;
}

void te::gm::WKTActions::buildPolyhedralSurface(te::gm::PolyhedralSurface* ps)
{
  std::size_t size = m_polygons.size();
  for(std::size_t i = 0; i < size; ++i)
    ps->setPatchN(i, m_polygons[i]);

  m_polygons.clear();

  m_g = ps;
}

void te::gm::WKTActions::buildGeometryCollection(const te::gm::GeomType& t)
{
  std::size_t size = m_geometries.size();
  te::gm::GeometryCollection* gc = new te::gm::GeometryCollection(size, t);
  for(std::size_t i = 0; i < size; ++i)
    gc->setGeometryN(i, m_geometries[i]);

  m_geometries.clear();
  
  m_g = gc;
}

te::gm::Geometry* te::gm::WKTActions::getGeometry()
{
  return m_g;
}


