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
  \file terralib/geometry/PointZM.cpp

  \brief A point with a z-coordinate value and an associated measurement.
*/

// TerraLib
#include "Exception.h"
#include "PointZM.h"

// STL
#include <cassert>

te::gm::PointZM::PointZM(const double& x, const double& y, const double& z, const double& m, int srid, Envelope* mbr)
  : Point(PointZMType, srid, mbr, x, y),
    m_z(z),
    m_m(m)
{
}

te::gm::PointZM::PointZM(int srid, Envelope* mbr)
  : Point(PointZMType, srid, mbr),
    m_z(sm_notNumber),
    m_m(sm_notNumber)
{
}

te::gm::PointZM::PointZM(const PointZM& rhs)
  : Point(rhs),
    m_z(rhs.m_z),
    m_m(rhs.m_m)
{
}

te::gm::PointZM& te::gm::PointZM::operator=(const PointZM& rhs)
{
  if(this != &rhs)
  {
    Point::operator=(rhs);

    m_z = rhs.m_z;
    m_m = rhs.m_m;
  }

  return *this;
}

te::dt::AbstractData* te::gm::PointZM::clone() const
{
  return new PointZM(*this);
}

te::gm::Geometry* te::gm::PointZM::locateBetween(const double& /*mStart*/, const double& /*mEnd*/) const throw(Exception)
{
  return 0;
}



