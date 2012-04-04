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
  \file PointPlacement.cpp
  
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

