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
  \file terralib/se/AnchorPoint.cpp
  
  \brief An AnchorPoint identifies the location inside of a text label to use an 'anchor' for positioning it relative to a point geometry.
*/

// TerraLib
#include "AnchorPoint.h"
#include "ParameterValue.h"

te::se::AnchorPoint::AnchorPoint()
  : m_x(0),
    m_y(0)
{
}

te::se::AnchorPoint::~AnchorPoint()
{
  delete m_x;
  delete m_y;
}

void te::se::AnchorPoint::setAnchorPointX(ParameterValue* x)
{
  delete m_x;
  m_x = x;
}

const te::se::ParameterValue* te::se::AnchorPoint::getAnchorPointX() const
{
  return m_x;
}

void te::se::AnchorPoint::setAnchorPointY(ParameterValue* y)
{
  delete m_y;
  m_y = y;
}

const te::se::ParameterValue* te::se::AnchorPoint::getAnchorPointY() const
{
  return m_y;
}

te::se::AnchorPoint* te::se::AnchorPoint::clone() const
{
  AnchorPoint* anchorPoint = new AnchorPoint;

  if(m_x)
    anchorPoint->setAnchorPointX(m_x->clone());

  if(m_y)
    anchorPoint->setAnchorPointY(m_y->clone());

  return anchorPoint;
}
