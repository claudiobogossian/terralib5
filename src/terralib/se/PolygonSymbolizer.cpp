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
  \file terralib/se/PolygonSymbolizer.cpp
  
  \brief A PolygonSymbolizer is used to draw a polygon (or other area-type geometries), including filling its interior and stroking its border (outline).
*/

// TerraLib
#include "../fe/PropertyName.h"
#include "Displacement.h"
#include "Fill.h"
#include "ParameterValue.h"
#include "PolygonSymbolizer.h"
#include "Stroke.h"

const std::string te::se::PolygonSymbolizer::sm_type("PolygonSymbolizer");

te::se::PolygonSymbolizer::PolygonSymbolizer()
  : te::se::Symbolizer(),
    m_geometry(0),
    m_fill(0),
    m_stroke(0),
    m_displacement(0),
    m_perpendicularOffset(0)
{
}

te::se::PolygonSymbolizer::PolygonSymbolizer(const te::se::PolygonSymbolizer& rhs)
  : te::se::Symbolizer(rhs),
    m_geometry(0),
    m_fill(0),
    m_stroke(0),
    m_displacement(0),
    m_perpendicularOffset(0)
{
  if(rhs.m_geometry)
    m_geometry = new te::fe::PropertyName(rhs.m_geometry->getName());

  if(rhs.m_fill)
    m_fill = rhs.m_fill->clone();

  if(rhs.m_stroke)
    m_stroke = rhs.m_stroke->clone();

  if(rhs.m_displacement)
    m_displacement = rhs.m_displacement->clone();

  if(rhs.m_perpendicularOffset)
    m_perpendicularOffset = rhs.m_perpendicularOffset->clone();
}

te::se::PolygonSymbolizer::~PolygonSymbolizer()
{
  delete m_geometry;
  delete m_fill;
  delete m_stroke;
  delete m_displacement;
  delete m_perpendicularOffset;
}

void te::se::PolygonSymbolizer::setGeometry(te::fe::PropertyName* geometry)
{
  delete m_geometry;
  m_geometry = geometry;
}

const te::fe::PropertyName* te::se::PolygonSymbolizer::getGeometry() const
{
  return m_geometry;
}

void te::se::PolygonSymbolizer::setFill(Fill* f)
{
  delete m_fill;
  m_fill = f;
}

const te::se::Fill* te::se::PolygonSymbolizer::getFill() const
{
  return m_fill;
}

void te::se::PolygonSymbolizer::setStroke(Stroke* stroke)
{
  delete m_stroke;
  m_stroke = stroke;
}

const te::se::Stroke* te::se::PolygonSymbolizer::getStroke() const
{
  return m_stroke;
}

void te::se::PolygonSymbolizer::setDisplacement(Displacement* d)
{
  delete m_displacement;
  m_displacement = d;
}

const te::se::Displacement* te::se::PolygonSymbolizer::getDisplacement() const
{
  return m_displacement;
}

void te::se::PolygonSymbolizer::setPerpendicularOffset(ParameterValue* perpendicularOffset)
{
  delete m_perpendicularOffset;
  m_perpendicularOffset = perpendicularOffset;
}

const te::se::ParameterValue* te::se::PolygonSymbolizer::getPerpendicularOffset() const
{
  return m_perpendicularOffset;
}

const std::string& te::se::PolygonSymbolizer::getType() const
{
  return sm_type;
}

te::se::Symbolizer* te::se::PolygonSymbolizer::clone() const
{
  return new PolygonSymbolizer(*this);
}
