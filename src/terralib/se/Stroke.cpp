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
  \file Stroke.cpp
  
  \brief A Stroke specifies the appearance of a linear geometry.
 */

// TerraLib
#include "../common/STLUtils.h"
#include "Graphic.h"
#include "GraphicStroke.h"
#include "Stroke.h"
#include "SvgParameter.h"

// STL
#include <cassert>

te::se::Stroke::Stroke()
  : m_fill(0),
    m_stroke(0)
{
}

te::se::Stroke::~Stroke()
{
  delete m_fill;
  delete m_stroke;
  te::common::FreeContents(m_svgParams);
}

void te::se::Stroke::setGraphicFill(Graphic* fill)
{
  delete m_fill;
  m_fill = fill;
}

const te::se::Graphic* te::se::Stroke::getGraphicFill() const
{
  return m_fill;
}

void te::se::Stroke::setGraphicStroke(GraphicStroke* stroke)
{
  delete m_stroke;
  m_stroke = stroke;
}

const te::se::GraphicStroke* te::se::Stroke::getGraphicStroke() const
{
  return m_stroke;
}

void te::se::Stroke::add(SvgParameter* p)
{
  m_svgParams.push_back(p);
}

