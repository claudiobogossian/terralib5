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
  \file BinarySpatialOp.cpp
  
  \brief A class for binary spatial operators.
 */

// TerraLib
#include "../geometry/Geometry.h"
#include "../gml/Envelope.h"
#include "BinarySpatialOp.h"
#include "Globals.h"
#include "PropertyName.h"

// STL
#include <cassert>

te::fe::BinarySpatialOp::BinarySpatialOp(const char* opName)
  : SpatialOp(opName),
    m_property(0),
    m_geom(0),
    m_box(0)
{
}

te::fe::BinarySpatialOp::BinarySpatialOp(const char* opName, PropertyName* p, te::gm::Geometry* g)
  : SpatialOp(opName),
    m_property(p),
    m_geom(g),
    m_box(0)
{
}

te::fe::BinarySpatialOp::BinarySpatialOp(const char* opName, PropertyName* p, te::gml::Envelope* e)
  : SpatialOp(opName),
    m_property(p),
    m_geom(0),
    m_box(e)
{
}

te::fe::BinarySpatialOp::~BinarySpatialOp()
{
  delete m_property;
  delete m_geom;
  delete m_box;
}

void te::fe::BinarySpatialOp::setProperty(PropertyName* p)
{
  delete m_property;
  m_property = p;
}

te::fe::PropertyName* te::fe::BinarySpatialOp::getProperty() const
{
  return m_property;
}

void te::fe::BinarySpatialOp::setGeometry(te::gm::Geometry* g)
{
  delete m_geom;
  m_geom = g;
}

te::gm::Geometry* te::fe::BinarySpatialOp::getGeometry() const
{
  return m_geom;
}

void te::fe::BinarySpatialOp::setEnvelope(te::gml::Envelope* e)
{
  delete m_box;
  m_box = e;
}

te::gml::Envelope* te::fe::BinarySpatialOp::getEnvelope() const
{
  return m_box;
}

