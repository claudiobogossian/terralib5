/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/se/LineSymbolizer.cpp
  
  \brief A LineSymbolizer is used to style a stroke along a linear geometry type, such as a string of line segments.
*/

// TerraLib
#include "../fe/PropertyName.h"
#include "LineSymbolizer.h"
#include "ParameterValue.h"
#include "Stroke.h"

te::se::LineSymbolizer::LineSymbolizer()
  : m_geometry(0),
    m_stroke(0),
    m_perpendicularOffset(0)
{
}

te::se::LineSymbolizer::~LineSymbolizer()
{
  delete m_geometry;
  delete m_stroke;
  delete m_perpendicularOffset;
}

void te::se::LineSymbolizer::setGeometry(te::fe::PropertyName* geometry)
{
  delete m_geometry;
  m_geometry = geometry;
}

const te::fe::PropertyName* te::se::LineSymbolizer::getGeometry() const
{
  return m_geometry;
}

void te::se::LineSymbolizer::setStroke(Stroke* stroke)
{
  delete m_stroke;
  m_stroke = stroke;
}

const te::se::Stroke* te::se::LineSymbolizer::getStroke() const
{
  return m_stroke;
}

void te::se::LineSymbolizer::setPerpendicularOffset(ParameterValue* perpendicularOffset)
{
  delete m_perpendicularOffset;
  m_perpendicularOffset = perpendicularOffset;
}

const te::se::ParameterValue* te::se::LineSymbolizer::getPerpendicularOffset() const
{
  return m_perpendicularOffset;
}

