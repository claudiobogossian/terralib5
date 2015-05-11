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
  \file DistanceBuffer.cpp
  
  \brief Distance buffer operator.  
 */

// TerraLib
#include "../common/Distance.h"
#include "../geometry/Geometry.h"
#include "DistanceBuffer.h"
#include "PropertyName.h"

te::fe::DistanceBuffer::DistanceBuffer(const char* opName)
  : SpatialOp(opName),
    m_property(0),
    m_geom(0),
    m_d(0)
{
}

te::fe::DistanceBuffer::DistanceBuffer(const char* opName,
                                       PropertyName* p,
                                       te::gm::Geometry* g,
                                       te::common::Distance* d)
  : SpatialOp(opName),
    m_property(p),
    m_geom(g),
    m_d(d)
{
}

te::fe::DistanceBuffer::~DistanceBuffer()
{
  delete m_property;
  delete m_geom;
  delete m_d;
}

void te::fe::DistanceBuffer::setProperty(PropertyName* p)
{
  delete m_property;
  m_property = p;
}

te::fe::PropertyName* te::fe::DistanceBuffer::getProperty() const
{
  return m_property;
}

void te::fe::DistanceBuffer::setGeometry(te::gm::Geometry* g)
{
  delete m_geom;
  m_geom = g;
}

te::gm::Geometry* te::fe::DistanceBuffer::getGeometry() const
{
  return m_geom;
}

void te::fe::DistanceBuffer::setDistance(te::common::Distance* d)
{
  delete m_d;
  m_d = d;
}

te::common::Distance* te::fe::DistanceBuffer::getDistance() const
{
  return m_d;
}

