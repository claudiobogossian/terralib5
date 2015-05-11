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
  \file terralib/se/PointPlacement.cpp
  
  \brief A PointPlacement specifies how a text label should be rendered relative to a geometric point.
*/

// TerraLib
#include "AnchorPoint.h"
#include "Displacement.h"
#include "ParameterValue.h"
#include "PointPlacement.h"

te::se::PointPlacement::PointPlacement()
  : m_anchorPoint(0),
    m_displacement(0),
    m_rotation(0)
{
}

te::se::PointPlacement::~PointPlacement()
{
  delete m_anchorPoint;
  delete m_displacement;
  delete m_rotation;
}

void te::se::PointPlacement::setAnchorPoint(AnchorPoint* a)
{
  delete m_anchorPoint;
  m_anchorPoint = a;
}

void te::se::PointPlacement::setDisplacement(Displacement* d)
{
  delete m_displacement;
  m_displacement = d;
}

void te::se::PointPlacement::setRotation(ParameterValue* r)
{
  delete m_rotation;
  m_rotation = r;
}

te::se::PointPlacement* te::se::PointPlacement::clone() const
{
  PointPlacement* pp = new PointPlacement;

  if(m_anchorPoint)
    pp->setAnchorPoint(m_anchorPoint->clone());

  if(m_displacement)
    pp->setDisplacement(m_displacement->clone());

  if(m_rotation)
    pp->setRotation(m_rotation->clone());

  return pp;
}
