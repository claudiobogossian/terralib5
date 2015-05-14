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
  \file terralib/se/SymbolizerColorFinder.cpp
  
  \brief A Symbology Enconding visitor that finds a color given a symbolizer.
*/

// TerraLib

#include "../color/RGBAColor.h"
#include "Fill.h"
#include "Graphic.h"
#include "LineSymbolizer.h"
#include "Mark.h"
#include "PointSymbolizer.h"
#include "PolygonSymbolizer.h"
#include "Rule.h"
#include "Stroke.h"
#include "Symbolizer.h"
#include "SymbolizerColorFinder.h"
#include "Style.h"
#include "Utils.h"

te::se::SymbolizerColorFinder::SymbolizerColorFinder()
{
}

te::se::SymbolizerColorFinder::~SymbolizerColorFinder()
{
}

void te::se::SymbolizerColorFinder::find(const te::se::Symbolizer* symbolizer)
{
  symbolizer->accept(*this);
}

void te::se::SymbolizerColorFinder::visit(const te::se::Style& visited)
{
}

void te::se::SymbolizerColorFinder::visit(const te::se::FeatureTypeStyle& visited)
{
}

void te::se::SymbolizerColorFinder::visit(const te::se::CoverageStyle& visited)
{
}

void te::se::SymbolizerColorFinder::visit(const te::se::Symbolizer& visited)
{
}

void te::se::SymbolizerColorFinder::visit(const te::se::PolygonSymbolizer& visited)
{
  const te::se::Fill* fill = visited.getFill();

  if(fill)
    find(fill);
}

void te::se::SymbolizerColorFinder::visit(const te::se::LineSymbolizer& visited)
{
  const te::se::Stroke* stroke = visited.getStroke();

  if(stroke)
    find(stroke);
}

void te::se::SymbolizerColorFinder::visit(const te::se::PointSymbolizer& visited)
{
  const te::se::Graphic* graphic = visited.getGraphic();

  if(graphic && !graphic->getMarks().empty())
    find(graphic->getMarks()[0]);
}

void te::se::SymbolizerColorFinder::visit(const te::se::TextSymbolizer& visited)
{
}

void te::se::SymbolizerColorFinder::visit(const te::se::RasterSymbolizer& visited)
{
}

void te::se::SymbolizerColorFinder::find(const te::se::Stroke* stroke)
{
  te::se::GetColor(stroke, m_color);
}

void te::se::SymbolizerColorFinder::find(const te::se::Fill* fill)
{
  te::se::GetColor(fill, m_color);
}

void te::se::SymbolizerColorFinder::find(const te::se::Mark* mark)
{
  if(mark)
    find(mark->getFill());
}
