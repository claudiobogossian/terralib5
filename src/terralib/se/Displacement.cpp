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
  \file terralib/se/Displacement.cpp

  \brief A Displacement gives X and Y offset displacements to use for rendering a text label, graphic or other Symbolizer near a point.
*/

// TerraLib
#include "Displacement.h"
#include "ParameterValue.h"

te::se::Displacement::Displacement()
  : m_x(0),
    m_y(0)
{
}

te::se::Displacement::~Displacement()
{
  delete m_x;
  delete m_y;
}

void te::se::Displacement::setDisplacementX(ParameterValue* x)
{
  delete m_x;
  m_x = x;
}

const te::se::ParameterValue* te::se::Displacement::getDisplacementX() const
{
  return m_x;
}

void te::se::Displacement::setDisplacementY(ParameterValue* y)
{
  delete m_y;
  m_y = y;
}

const te::se::ParameterValue* te::se::Displacement::getDisplacementY() const
{
  return m_y;
}

te::se::Displacement* te::se::Displacement::clone() const
{
  Displacement* displacement = new Displacement;

  if(m_x)
    displacement->setDisplacementX(m_x->clone());

  if(m_y)
    displacement->setDisplacementY(m_y->clone());

  return displacement;
}
