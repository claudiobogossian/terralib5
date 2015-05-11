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
  \file terralib/se/GraphicStroke.cpp
  
  \brief A GraphicStroke defines a repeated-linear graphic pattern to be used for stroking a line.
*/

// TerraLib
#include "Graphic.h"
#include "GraphicStroke.h"
#include "ParameterValue.h"

te::se::GraphicStroke::GraphicStroke()
  : m_graphic(0),
    m_initialGap(0),
    m_gap(0)
{
}

te::se::GraphicStroke::~GraphicStroke()
{
  delete m_graphic;
  delete m_initialGap;
  delete m_gap;
}

void te::se::GraphicStroke::setGraphic(Graphic* g)
{
  delete m_graphic;
  m_graphic = g;
}

const te::se::Graphic* te::se::GraphicStroke::getGraphic() const
{
  return m_graphic;
}

void te::se::GraphicStroke::setInitialGap(ParameterValue* initialGap)
{
  delete m_initialGap;
  m_initialGap = initialGap;
}

const te::se::ParameterValue* te::se::GraphicStroke::getInitialGap() const
{
  return m_initialGap;
}

void te::se::GraphicStroke::setGap(ParameterValue* gap)
{
  delete m_gap;
  m_gap = gap;
}

const te::se::ParameterValue* te::se::GraphicStroke::getGap() const
{
  return m_gap;
}

te::se::GraphicStroke* te::se::GraphicStroke::clone() const
{
  GraphicStroke* graphicStroke = new GraphicStroke;
  
  if(m_graphic)
    graphicStroke->setGraphic(m_graphic->clone());

  if(m_initialGap)
    graphicStroke->setInitialGap(new ParameterValue(*m_initialGap));

  if(m_gap)
    graphicStroke->setGap(new ParameterValue(*m_gap));

  return graphicStroke;
}
