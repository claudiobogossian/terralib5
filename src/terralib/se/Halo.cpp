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
  \file terralib/se/Halo.cpp
  
  \brief A Halo is a type of Fill that is applied to the backgrounds of font glyphs.
*/

// TerraLib
#include "Fill.h"
#include "Halo.h"
#include "ParameterValue.h"

te::se::Halo::Halo()
  : m_radius(0),
    m_fill(0)
{
}

te::se::Halo::~Halo()
{
  delete m_radius;
  delete m_fill;
}

void te::se::Halo::setRadius(ParameterValue* radius)
{
  delete m_radius;
  m_radius = radius;
}

const te::se::ParameterValue* te::se::Halo::getRadius() const
{
  return m_radius;
}

void te::se::Halo::setFill(Fill* fill)
{
  delete m_fill;
  m_fill = fill;
}

const te::se::Fill* te::se::Halo::getFill() const
{
  return m_fill;
}

te::se::Halo* te::se::Halo::clone() const
{
  Halo* halo = new Halo;

  if(m_radius)
    halo->setRadius(m_radius->clone());

  if(m_fill)
    halo->setFill(m_fill->clone());

  return halo;
}
