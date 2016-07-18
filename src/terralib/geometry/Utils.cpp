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
#include "../core/translator/Translator.h"
#include "Envelope.h"
#include "Exception.h"
#include "Geometry.h"
#include "GeometryCollection.h"
#include "GEOSGeometryFactory.h"
#include "GEOSReader.h"
#include "GEOSWriter.h"
#include "LinearRing.h"
#include "LineString.h"
#include "MultiPolygon.h"
#include "Point.h"
#include "Polygon.h"
#include "Utils.h"

#ifdef TERRALIB_GEOS_ENABLED
// GEOS
#include <geos/geom/Coordinate.h>
#include <geos/geom/Geometry.h>
#include <geos/operation/polygonize/Polygonizer.h>
#include <geos/operation/union/UnaryUnionOp.h>
#include <geos/operation/valid/IsValidOp.h>
#include <geos/util/GEOSException.h>
#endif

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
    {
      te::gm::Geometry* currentGeom = gc->getGeometryN(i);
      currentGeom->setSRID(gc->getSRID());
      Multi2Single(currentGeom, geoms);
    }
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


te::gm::Geometry* te::gm::UnaryUnion(te::gm::Geometry* geom)
{
#ifdef TERRALIB_GEOS_ENABLED

  std::auto_ptr<geos::geom::Geometry> geomGeos(GEOSWriter::write(geom));

  std::auto_ptr<geos::geom::Geometry> unionGeom = geos::operation::geounion::UnaryUnionOp::Union(*(geomGeos.get()));

  unionGeom->setSRID(geom->getSRID());

  return GEOSReader::read(unionGeom.get());

#else
  throw te::common::Exception(TE_TR("Union routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

void te::gm::Polygonizer(te::gm::Geometry* g, std::vector<te::gm::Polygon*>& pols)
{
#ifdef TERRALIB_GEOS_ENABLED

  geos::operation::polygonize::Polygonizer polygonizer;

  std::auto_ptr<geos::geom::Geometry> geomGeos(GEOSWriter::write(g));
  polygonizer.add(geomGeos.get());

  std::vector <geos::geom::Polygon*>* vecpolGeos = polygonizer.getPolygons();

  for (std::size_t i = 0; i < vecpolGeos->size(); i++)
  { 
    vecpolGeos->at(i)->setSRID(g->getSRID());
    pols.push_back(GEOSReader::read(vecpolGeos->at(i)));
  }

#else
  throw te::common::Exception(TE_TR("Polygonizer routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

bool te::gm::CheckValidity(const te::gm::Geometry* geom, te::gm::TopologyValidationError& error)
{
#ifdef TERRALIB_GEOS_ENABLED

  std::unique_ptr<geos::geom::Geometry> g(GEOSWriter::write(geom));

  geos::operation::valid::IsValidOp vop(g.get());

  geos::operation::valid::TopologyValidationError* err = vop.getValidationError();

  te::gm::TopologyValidationError validationError;

  if (err)
  {
    error.m_coordinate = Coord2D(err->getCoordinate().x, err->getCoordinate().y);
    error.m_message = err->getMessage();

    return false;
  }

  return true;

#else
  throw Exception(TE_TR("isValid routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

te::gm::Geometry* te::gm::Validate(te::gm::Geometry* geom)
{
#ifdef TERRALIB_GEOS_ENABLED

  std::unique_ptr<geos::geom::Geometry> g(GEOSWriter::write(geom));

  std::vector<te::gm::Geometry*> pAdd;
  geos::operation::polygonize::Polygonizer polygonizer;

  switch (g->getGeometryTypeId())
  {
    case geos::geom::GEOS_POLYGON:
    {
      if (g->isValid())
      {
        g->normalize(); // validate does not pick up rings in the wrong order - this will fix that
        return GEOSReader::read(g.get()); // If the polygon is valid just return it
      }

      AddPolygon(dynamic_cast<te::gm::Polygon*>(GEOSReader::read(g.get())), pAdd);
    }
    break;

    case geos::geom::GEOS_MULTIPOLYGON:
    {
      if (g->isValid()){
        g->normalize(); // validate does not pick up rings in the wrong order - this will fix that
        return GEOSReader::read(g.get()); // If the multipolygon is valid just return it
      }

      for (std::size_t n = g->getNumGeometries(); n-- > 0;)
        AddPolygon(dynamic_cast<te::gm::Polygon*>(GEOSReader::read(g->getGeometryN(n))), pAdd);

    }
    break;

    default:
      return geom;
  }

  for (std::size_t i = 0; i < pAdd.size(); i++)
    polygonizer.add(GEOSWriter::write(pAdd[i]));

  std::vector <geos::geom::Polygon*>* vecpolGeos = polygonizer.getPolygons();

  switch (vecpolGeos->size())
  {
    case 1:
      vecpolGeos->at(0)->setSRID(geom->getSRID());
      return GEOSReader::read(vecpolGeos->at(0)); // single polygon - no need to wrap
    default:
    {
      //polygons may still overlap! Need to sym difference them
      vecpolGeos->at(0)->setSRID(geom->getSRID());
      te::gm::Geometry* ret = GEOSReader::read(vecpolGeos->at(0));

      for (std::size_t i = 1; i < vecpolGeos->size(); i++)
      {
        vecpolGeos->at(i)->setSRID(geom->getSRID());
        ret = ret->symDifference(GEOSReader::read(vecpolGeos->at(i)));
      }
      return ret;
    }
  }

#else
  throw Exception(TE_TR("validate routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

void te::gm::AddPolygon(te::gm::Polygon* polygon, std::vector<te::gm::Geometry*>& pAdd)
{
#ifdef TERRALIB_GEOS_ENABLED

  AddLineString(dynamic_cast<te::gm::LineString*>(polygon->getExteriorRing()), pAdd);

  for (std::size_t n = polygon->getNumInteriorRings(); n-- > 0;){
    AddLineString(dynamic_cast<te::gm::LineString*>(polygon->getInteriorRingN(n)), pAdd);
  }

#else
  throw Exception(TE_TR("addPolygon routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

void te::gm::AddLineString(te::gm::LineString* lineString, std::vector<te::gm::Geometry*>& pAdd)
{
#ifdef TERRALIB_GEOS_ENABLED

  std::unique_ptr<geos::geom::LineString> ls(GEOSWriter::write(lineString));

  // unioning the linestring with the point makes any self intersections explicit.
  geos::geom::Point* point = GEOSGeometryFactory::getGeomFactory()->createPoint(ls->getCoordinateN(0)); 

  geos::geom::Geometry* toAdd = ls->Union(point);

  pAdd.push_back(GEOSReader::read(toAdd));

#else
  throw Exception(TE_TR("addLineString routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}