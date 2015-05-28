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
  \file EnumLineStyleType.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnumLineStyleType.h"

te::layout::EnumLineStyleType::EnumLineStyleType() :
  m_styleNone(0),
  m_styleSolid(0),
  m_styleDash(0),
  m_styleDot(0),
  m_styleDashDot(0),
  m_styleDashDotDot(0),
  m_styleCustomDash(0)
{
  init();
}

te::layout::EnumLineStyleType::~EnumLineStyleType()
{
  if(m_styleNone)
  {
    delete m_styleNone;
    m_styleNone = 0;
  }
  if(m_styleSolid)
  {
    delete m_styleSolid;
    m_styleSolid = 0;
  }
  if(m_styleDash)
  {
    delete m_styleDash;
    m_styleDash = 0;
  }
  if(m_styleDot)
  {
    delete m_styleDot;
    m_styleDot = 0;
  }
  if(m_styleDashDot)
  {
    delete m_styleDashDot;
    m_styleDashDot = 0;
  }
  if(m_styleDashDotDot)
  {
    delete m_styleDashDotDot;
    m_styleDashDotDot = 0;
  }
  if(m_styleCustomDash)
  {
    delete m_styleCustomDash;
    m_styleCustomDash = 0;
  }
}

void te::layout::EnumLineStyleType::init()
{
  m_styleNone = createEnum("No Line", this);

  m_styleSolid = createEnum("Solid", this, "Continuous");

  m_styleDash = createEnum("Dash", this, "Dashed");

  m_styleDot = createEnum("Dot", this, "Dotted");

  m_styleDashDot = createEnum("Dash Dot", this, "Dash Dot");

  m_styleDashDotDot = createEnum("Dash Dot Dot", this, "Dash Dot Dot");

  m_styleCustomDash = createEnum("Custom Dash", this);
}

te::layout::EnumType* te::layout::EnumLineStyleType::getStyleNone() const
{
  return m_styleNone;
}

te::layout::EnumType* te::layout::EnumLineStyleType::getStyleSolid() const
{
  return m_styleSolid;
}

te::layout::EnumType* te::layout::EnumLineStyleType::getStyleDash() const
{
  return m_styleDash;
}

te::layout::EnumType* te::layout::EnumLineStyleType::getStyleDot() const
{
  return m_styleDot;
}

te::layout::EnumType* te::layout::EnumLineStyleType::getStyleDashDot() const
{
  return m_styleDashDot;
}

te::layout::EnumType* te::layout::EnumLineStyleType::getStyleDashDotDot() const
{
  return m_styleDashDotDot;
}

te::layout::EnumType* te::layout::EnumLineStyleType::getStyleCustomDash() const
{
  return m_styleCustomDash;
}
