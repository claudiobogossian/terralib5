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
  \file terralib/se/PointSymbolizer.cpp
  
  \brief A PointSymbolizer specifies the rendering of a graphic Symbolizer at a point.
*/

// TerraLib
#include "../fe/PropertyName.h"
#include "Graphic.h"
#include "PointSymbolizer.h"

const std::string te::se::PointSymbolizer::sm_type("PointSymbolizer");

te::se::PointSymbolizer::PointSymbolizer()
  : te::se::Symbolizer(),
    m_geometry(0),
    m_graphic(0)
{
}

te::se::PointSymbolizer::PointSymbolizer(const te::se::PointSymbolizer& rhs)
  : te::se::Symbolizer(rhs),
    m_geometry(0),
    m_graphic(0)
{
  if(rhs.m_geometry)
    m_geometry = new te::fe::PropertyName(rhs.m_geometry->getName());

  if(rhs.m_graphic)
    m_graphic = rhs.m_graphic->clone();
}

te::se::PointSymbolizer::~PointSymbolizer()
{
  delete m_geometry;
  delete m_graphic;
}

void te::se::PointSymbolizer::setGeometry(te::fe::PropertyName* geometry)
{
  delete m_geometry;
  m_geometry = geometry;
}

const te::fe::PropertyName* te::se::PointSymbolizer::getGeometry() const
{
  return m_geometry;
}

void te::se::PointSymbolizer::setGraphic(Graphic* graphic)
{
  delete m_graphic;
  m_graphic = graphic;
}

const te::se::Graphic* te::se::PointSymbolizer::getGraphic() const
{
  return m_graphic;
}

const std::string& te::se::PointSymbolizer::getType() const
{
  return sm_type;

}
te::se::Symbolizer* te::se::PointSymbolizer::clone() const
{
  return new PointSymbolizer(*this);
}
