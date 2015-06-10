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
  \file terralib/geometry/PointM.cpp

  \brief A point with an associated measure.
*/

// TerraLib
#include "PointM.h"

// STL
#include <cassert>

te::gm::PointM::PointM(const double& x, const double& y, const double& m, int srid, Envelope* mbr)
  : Point(PointMType, srid, mbr, x, y),
    m_m(m)
{
}

te::gm::PointM::PointM(int srid, Envelope* mbr)
  : Point(PointMType, srid, mbr),
    m_m(sm_notNumber)
{
}

te::gm::PointM::PointM(const PointM& rhs)
  : Point(rhs),
    m_m(rhs.m_m)
{
}

te::gm::PointM& te::gm::PointM::operator=(const PointM& rhs)
{
  if(this != &rhs)
  {
    Point::operator=(rhs);
    m_m = rhs.m_m;
  }
  return *this;
}

te::dt::AbstractData* te::gm::PointM::clone() const
{
  return new PointM(*this);
}

te::gm::Geometry* te::gm::PointM::locateBetween(const double& /*mStart*/, const double& /*mEnd*/) const throw(Exception)
{
  return 0;
}



