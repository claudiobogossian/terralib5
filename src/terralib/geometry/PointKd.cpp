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
  \file terralib/geometry/PointKd.cpp

  \brief A k-dimensional point.
*/

// TerraLib
#include "PointKd.h"

te::gm::PointKd::PointKd(unsigned int k, int srid, Envelope* mbr)
  : AbstractPoint(PointKdType, srid, mbr),
    m_coords(k)
{
}

te::gm::PointKd::PointKd(const PointKd& rhs)
  : AbstractPoint(rhs),
    m_coords(rhs.m_coords)
{
}

te::gm::PointKd& te::gm::PointKd::operator=(const PointKd& rhs)
{
  if(this != &rhs)
  {
    AbstractPoint::operator=(rhs);

    m_coords = rhs.m_coords;
  }

  return *this;
}

te::dt::AbstractData* te::gm::PointKd::clone() const
{
  return new PointKd(*this);
}

const double& te::gm::PointKd::getX() const
{
  return m_coords[0];
}

void te::gm::PointKd::setX(const double& x)
{
  m_coords[0] = x;
}

const double& te::gm::PointKd::getY() const
{
  return m_coords[1];
}

void te::gm::PointKd::setY(const double& y)
{
  m_coords[1] = y;
}

const double& te::gm::PointKd::getZ() const
{
  return m_coords[2];
}

void te::gm::PointKd::setZ(const double& z)
{
  m_coords[2] = z;
}

const double& te::gm::PointKd::getM() const
{
  return sm_notNumber;
}

void te::gm::PointKd::setM(const double& /*m*/)
{
}

