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
  \file terralib/geometry/Point.cpp

  \brief A point with x and y coordinate values.
*/

// TerraLib
#include "../Defines.h"
#include "Point.h"

// STL
#include <cassert>

const std::string te::gm::Point::sm_typeName("Point");

const double te::gm::Point::sm_notNumber(TE_DOUBLE_NOT_A_NUMBER);

te::gm::Point::Point(const double& x, const double& y, int srid, Envelope* mbr)
  : AbstractPoint(PointType, srid, mbr),
    m_x(x),
    m_y(y)
{
}

te::gm::Point::Point(int srid, Envelope* mbr)
  : AbstractPoint(PointType, srid, mbr),
    m_x(sm_notNumber),
    m_y(sm_notNumber)
{
}

te::gm::Point::Point(const Point& rhs)
  : AbstractPoint(rhs),
    m_x(rhs.m_x),
    m_y(rhs.m_y)
{
}

te::gm::Point& te::gm::Point::operator=(const Point& rhs)
{
  if(this != &rhs)
  {
    AbstractPoint::operator=(rhs);

    m_x = rhs.m_x;
    m_y = rhs.m_y;
  }

  return *this;
}

te::dt::AbstractData* te::gm::Point::clone() const
{
  return new Point(*this);
}

void te::gm::Point::setZ(const double& /*z*/)
{
}

void te::gm::Point::setM(const double& /*m*/)
{
}

te::gm::Point::Point(GeomType t, int srid, Envelope* mbr)
  : AbstractPoint(t, srid, mbr),
    m_x(sm_notNumber),
    m_y(sm_notNumber)
{
}

te::gm::Point::Point(GeomType t, int srid, Envelope* mbr, const double& x, const double& y)
  : AbstractPoint(t, srid, mbr),
    m_x(x),
    m_y(y)
{
}


