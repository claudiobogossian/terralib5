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
  \file terralib/geometry/Line.cpp

  \brief A Line is LineString with 2 points.
*/

// TerraLib
#include "Line.h"
#include "Point.h"


// STL
#include <cassert>

//GEOS
#include <geos/algorithm/LineIntersector.h>


te::gm::Line::Line(GeomType t, int srid, Envelope* mbr)
  : LineString(2, t, srid, mbr)
{
}

te::gm::Line::Line(const Point& first, const Point& second, GeomType t, int srid, Envelope* mbr)
  : LineString(2, t, srid, mbr)
{
  m_coords[0].x = first.getX();
  m_coords[0].y = first.getY();

  m_coords[1].x = second.getX();
  m_coords[1].y = second.getY();

  if((t & 0xF00) == 0x300)
  {
    m_zA[0] = first.getZ();
    m_zA[1] = second.getZ();
  }
  else if((t & 0xF00) == 0x700)
  {
    m_mA[0] = first.getM();
    m_mA[1] = second.getM();
  }
  else if((t & 0xF00) == 0xB00)
  {
    m_zA[0] = first.getZ();
    m_zA[1] = second.getZ();

    m_mA[0] = first.getM();
    m_mA[1] = second.getM();
  }
}

te::gm::Line::Line(const Line& rhs)
  : LineString(rhs)
{
}

te::gm::Line::~Line()
{
}

te::gm::Line& te::gm::Line::operator=(const Line& rhs)
{
  LineString::operator=(rhs);

  return *this;
}

te::dt::AbstractData* te::gm::Line::clone() const
{
  return new Line(*this);
}

bool te::gm::Line::intersection(const Line& line, Point& ret) const
{
  geos::algorithm::LineIntersector li;
  geos::geom::Coordinate p0(m_coords[0].x, m_coords[0].y);
  geos::geom::Coordinate p1(m_coords[1].x, m_coords[1].y);
  geos::geom::Coordinate lp0(line.m_coords[0].x, line.m_coords[0].y);
  geos::geom::Coordinate lp1(line.m_coords[1].x, line.m_coords[1].y);
  li.computeIntersection(p0, p1, lp0, lp1);
  if (li.hasIntersection())
  {
    ret.setX(li.getIntersection(0).x);
    ret.setY(li.getIntersection(0).y);
    return true;
  }
  return false;
}

double te::gm::Line::Angle()
{
  double dx = m_coords[0].x - m_coords[1].x;
  double dy = m_coords[0].y - m_coords[1].y;
  if (dx == 0.)
    return DBL_MAX;
  return(dy / dx);
}


void te::gm::Line::setCoord(int index, double x, double y, double z, double m)
{
  m_coords[index].x = x;
  m_coords[index].y = y;
  if ((m_gType & 0xF00) == 0x300)
  {
    m_zA[index] = z;
  }
  else if ((m_gType & 0xF00) == 0x700)
  {
    m_mA[index] = m;
  }
  else if ((m_gType & 0xF00) == 0xB00)
  {
    m_zA[index] = z;
    m_mA[index] = m;
  }
}

double te::gm::Line::distance(te::gm::Point p)
{
  double A, B, C, distance;

  // coefficients of line p1,p2
  A = m_coords[0].y - m_coords[1].y;
  B = m_coords[1].x - m_coords[0].x;
  C = (m_coords[0].x - m_coords[1].x)* m_coords[0].y + (m_coords[1].y - m_coords[0].y)*m_coords[0].x;
  distance = abs(A*(p.getX()) + B*(p.getY()) + C) / sqrt(A*A + B*B);

  return(distance);
}



