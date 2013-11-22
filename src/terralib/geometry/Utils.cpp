/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/geometry/Utils.cpp

  \brief Utility functions for the Geometry Module.
*/

// TerraLib
#include "../common/Translator.h"
#include "Envelope.h"
#include "Exception.h"
#include "Geometry.h"
#include "LinearRing.h"
#include "Polygon.h"
#include "Utils.h"

te::gm::Geometry* te::gm::GetGeomFromEnvelope(const Envelope* const e, int srid)
{
// create an outer ring with the same envelope as our envelope
  LinearRing* r = new LinearRing(5, LineStringType, srid, new Envelope(*e));

  r->setPoint(0, e->m_llx, e->m_lly);
  r->setPoint(1, e->m_urx, e->m_lly);
  r->setPoint(2, e->m_urx, e->m_ury);
  r->setPoint(3, e->m_llx, e->m_ury);
  r->setPoint(4, e->m_llx, e->m_lly);

// create the polygon
  Polygon* p = new Polygon(1, PolygonType, srid, new Envelope(*e));
  p->setRingN(0, r);

  return p;
}

bool te::gm::SatisfySpatialRelation(const Geometry* g1,
                                    const Geometry* g2, 
                                    SpatialRelation relation)
{
  switch(relation)
  {
    case CONTAINS:
      return g1->contains(g2);

    case COVEREDBY:
      return g1->coveredBy(g2);

    case COVERS:
      return g1->covers(g2);

    case CROSSES:
      return g1->crosses(g2);

    case DISJOINT:
      return g1->disjoint(g2);

    case EQUALS:
      return g1->equals(g2);

    case INTERSECTS:
      return g1->intersects(g2);

    case OVERLAPS:
      return g1->overlaps(g2);

    case TOUCHES:
      return g1->touches(g2);

    case WITHIN:
      return g1->within(g2);

    default:
      throw Exception(TR_GEOM("Invalid spatial relation!"));
  }
}

te::gm::GeomType te::gm::GetGeomType(const std::size_t type)
{
  switch(type)
  {
    case 0:
      return te::gm::GeometryType;
    case 1000:
      return te::gm::GeometryZType;
    case 2000:
      return te::gm::GeometryMType;
    case 3000:
      return te::gm::GeometryZMType;

    case 1:
      return te::gm::PointType;
    case 1001:
      return te::gm::PointZType;
    case 2001:
      return te::gm::PointMType;
    case 3001:
      return te::gm::PointZMType;

    case 2:
      return te::gm::LineStringType;
    case 1002:
      return te::gm::LineStringZType;
    case 2002:
      return te::gm::LineStringMType;
    case 3002:
      return te::gm::LineStringZMType;

    case 8:
      return te::gm::CircularStringType;
    case 1008:
      return te::gm::CircularStringZType;
    case 2008:
      return te::gm::CircularStringMType;
    case 3008:
      return te::gm::CircularStringZMType;

    case 9:
      return te::gm::CompoundCurveType;
    case 1009:
      return te::gm::CompoundCurveZType;
    case 2009:
      return te::gm::CompoundCurveMType;
    case 3009:
      return te::gm::CompoundCurveZMType;

    case 3:
      return te::gm::PolygonType;
    case 1003:
      return te::gm::PolygonZType;
    case 2003:
      return te::gm::PolygonMType;
    case 3003:
      return te::gm::PolygonZMType;

    case 10:
      return te::gm::CurvePolygonType;
    case 1010:
      return te::gm::CurvePolygonZType;
    case 2010:
      return te::gm::CurvePolygonMType;
    case 3010:
      return te::gm::CurvePolygonZMType;

    case 7:
      return te::gm::GeometryCollectionType;
    case 1007:
      return te::gm::GeometryCollectionZType;
    case 2007:
      return te::gm::GeometryCollectionMType;
    case 3007:
      return te::gm::GeometryCollectionZMType;

    case 4:
      return te::gm::MultiPointType;
    case 1004:
      return te::gm::MultiPointZType;
    case 2004:
      return te::gm::MultiPointMType;
    case 3004:
      return te::gm::MultiPointZMType;

    case 5:
      return te::gm::MultiLineStringType;
    case 1005:
      return te::gm::MultiLineStringZType;
    case 2005:
      return te::gm::MultiLineStringMType;
    case 3005:
      return te::gm::MultiLineStringZMType;

    case 6:
      return te::gm::MultiPolygonType;
    case 1006:
      return te::gm::MultiPolygonZType;
    case 2006:
      return te::gm::MultiPolygonMType;
    case 3006:
      return te::gm::MultiPolygonZMType;

    case 15:
      return te::gm::PolyhedralSurfaceType;
    case 1015:
      return te::gm::PolyhedralSurfaceZType;
    case 2015:
      return te::gm::PolyhedralSurfaceMType;
    case 3015:
      return te::gm::PolyhedralSurfaceZMType;

    case 16:
      return te::gm::TINType;
    case 1016:
      return te::gm::TINZType;
    case 2016:
      return te::gm::TINMType;
    case 3016:
      return te::gm::TINZMType;

    case 17:
      return te::gm::TriangleType;
    case 1017:
      return te::gm::TriangleZType;
    case 2017:
      return te::gm::TriangleMType;
    case 3017:
      return te::gm::TriangleZMType;

    default:
      return te::gm::UnknownGeometryType;
  }
}

