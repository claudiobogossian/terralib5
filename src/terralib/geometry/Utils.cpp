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
  \file terralib/geometry/Utils.cpp

  \brief Utility functions for the Geometry Module.
*/

// TerraLib
#include "../common/Translator.h"
#include "Coord2D.h"
#include "Envelope.h"
#include "Exception.h"
#include "Geometry.h"
#include "GeometryCollection.h"
#include "LinearRing.h"
#include "LineString.h"
#include "MultiPolygon.h"
#include "Point.h"
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
      throw Exception(TE_TR("Invalid spatial relation!"));
  }
}

te::gm::Envelope te::gm::AdjustToCut(const Envelope & env, double bWidth, double bHeight)
{
  double auxD;
  int auxI;

  int magicX; 
  auxD = env.m_llx/bWidth;
  auxI = (int)(env.m_llx/bWidth);
  if (env.m_llx < 0 && (auxD - auxI) != 0)
    magicX = (int) (env.m_llx/bWidth - 1);
  else
    magicX = auxI;

  int magicY;
  auxD = env.m_lly/bHeight;
  auxI = (int)(env.m_lly/bHeight);
  if (env.m_lly < 0 && (auxD - auxI) != 0)
    magicY  = (int)(env.m_lly/bHeight - 1);
  else
    magicY  = auxI;

  double xi = magicX*bWidth;
  double yi = magicY*bHeight;

  int magicX2;
  auxD = env.m_urx/bWidth;
  auxI = (int)(env.m_urx/bWidth);
  if ((env.m_urx < 0) || (auxD - auxI) == 0)
    magicX2 = (int) (env.m_urx/bWidth);
  else
    magicX2 = (int) (env.m_urx/bWidth + 1);

  int magicY2;
  auxD = env.m_ury/bHeight;
  auxI = (int)(env.m_ury/bHeight);
  if ((env.m_ury < 0) || (auxD - auxI) == 0)
    magicY2 = (int) (env.m_ury/bHeight);
  else
    magicY2 = (int) (env.m_ury/bHeight + 1);

  double xf = (magicX2)*bWidth;
  double yf = (magicY2)*bHeight;

  return te::gm::Envelope(xi,yi,xf,yf);
}

template<class T1, class T2> bool te::gm::Intersects(const T1& o1, const T2& o2)
{
  return o1->intersects(o2);
}

template<> bool te::gm::Intersects(const te::gm::Point& point, const te::gm::Envelope& e)
{
  // point to the right of envelope
  if(point.getX() > e.m_urx)
    return false;

  // point to the left of envelope
  if(e.m_llx > point.getX())
    return false;

  // point is above envelope
  if(point.getY() > e.m_ury)
    return false;

  // point is below envelope
  if(e.m_lly > point.getY())
    return false;

  return true;
}

te::gm::Coord2D* te::gm::locateAlong(const LineString* line, double initial, double final, double target)
{
  double tTof; // Distance of target to fist point

  std::map<int, double> pointLenghtFromFirst;

  double fullLineLenght = 0;
  
  pointLenghtFromFirst[0] = 0;
  for(std::size_t i = 1; i < line->getNPoints(); i++)
  {
    fullLineLenght += line->getPointN(i)->distance(line->getPointN(i-1));
    pointLenghtFromFirst[i] = fullLineLenght;
  }

  double diference = final-initial;
  
  tTof = ((target-initial)*fullLineLenght)/diference;

  int onTheFly = -1;

  int pointBeforeTarget;
  for(std::size_t i = 0; i < pointLenghtFromFirst.size(); i++)
  {
    if(pointLenghtFromFirst[i] == tTof)
    {
      onTheFly = i;
      break;
    }
    if(tTof >= pointLenghtFromFirst[i] && tTof < pointLenghtFromFirst[i+1])
    {
      pointBeforeTarget = i;
      break;
    }
  }

  te::gm::Coord2D* targetCoord = new te::gm::Coord2D();

  if(onTheFly >= 0)
  {
    targetCoord->x = line->getPointN(onTheFly)->getX();
    targetCoord->y = line->getPointN(onTheFly)->getY();

    return targetCoord;
  }

  Point* p1 = line->getPointN(pointBeforeTarget);
  Point* p2 = line->getPointN(pointBeforeTarget+1);

  // Partial distance: target to p1
  double pd = tTof - pointLenghtFromFirst[pointBeforeTarget];
  
  // Total distance: p2 to p1
  double td = pointLenghtFromFirst[pointBeforeTarget+1] - pointLenghtFromFirst[pointBeforeTarget];
  
  targetCoord->x = ((pd*(p2->getX()-p1->getX()))/td)+p1->getX();  

  targetCoord->y = ((pd*(p2->getY()-p1->getY()))/td)+p1->getY();

  return targetCoord;
}

void te::gm::Multi2Single(te::gm::Geometry* g, std::vector<te::gm::Geometry*>& geoms)
{
  te::gm::GeometryCollection* gc = dynamic_cast<te::gm::GeometryCollection*>(g);
  if (gc)
  {
    for (std::size_t i = 0; i < gc->getNumGeometries(); ++i)
      Multi2Single(gc->getGeometryN(i), geoms);
  }
  else
    geoms.push_back(g);
}

std::vector<te::gm::Geometry*> te::gm::FixSelfIntersection(const te::gm::Geometry* g)
{
  std::vector<te::gm::Geometry*> result;

  std::auto_ptr<te::gm::Geometry> geomBuffer(g->buffer(0.0));

  std::vector<te::gm::Geometry*> vecSingleGeoms;
  te::gm::Multi2Single(geomBuffer.get(), vecSingleGeoms);

  if (g->getGeomTypeId() == te::gm::MultiPolygonType)
  {
    for (std::size_t j = 0; j < vecSingleGeoms.size(); ++j)
    {
      te::gm::MultiPolygon* pol = new te::gm::MultiPolygon(vecSingleGeoms.size(), te::gm::MultiPolygonType, g->getSRID());

      pol->add((te::gm::Geometry*)vecSingleGeoms[j]->clone());

      result.push_back(pol);
    }
  }
  else if (g->getGeomTypeId() == te::gm::PolygonType)
  {
    for (std::size_t j = 0; j < vecSingleGeoms.size(); ++j)
    {
      result.push_back((te::gm::Geometry*)vecSingleGeoms[j]->clone());
    }
  }
  else
  {
    throw te::common::Exception(TE_TR("Unexpected geometry type!"));
  }

  return result;
}