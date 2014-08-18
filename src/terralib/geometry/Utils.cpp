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