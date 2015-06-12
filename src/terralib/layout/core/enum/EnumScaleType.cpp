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
  \file EnumScaleType.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnumScaleType.h"

te::layout::EnumScaleType::EnumScaleType() :
  m_noneType(0),
  m_alternatingScaleBarType(0),
  m_doubleAlternatingScaleBarType(0),
  m_hollowScaleBarType(0)
{
  init();
}

te::layout::EnumScaleType::~EnumScaleType()
{
  if(m_noneType)
  {
    delete m_noneType;
    m_noneType = 0;
  }
  if(m_alternatingScaleBarType)
  {
    delete m_alternatingScaleBarType;
    m_alternatingScaleBarType = 0;
  }
  if(m_doubleAlternatingScaleBarType)
  {
    delete m_doubleAlternatingScaleBarType;
    m_doubleAlternatingScaleBarType = 0;
  }
  if(m_hollowScaleBarType)
  {
    delete m_hollowScaleBarType;
    m_hollowScaleBarType = 0;
  }
}
void te::layout::EnumScaleType::init()
{
  m_noneType = createEnum("NoneScale", this, "None Scale");

  m_alternatingScaleBarType = createEnum("AlternantingScaleBar", this, "Alternanting Scale Bar");

  m_doubleAlternatingScaleBarType = createEnum("DoubleAlternantingScaleBar", this, "Double Alternanting Scale Bar");

  m_hollowScaleBarType = createEnum("HollowScaleBar", this, "Hollow Scale Bar");
}

te::layout::EnumType* te::layout::EnumScaleType::getNoneType() const
{
  return m_noneType;
}

te::layout::EnumType* te::layout::EnumScaleType::getAlternatingScaleBarType() const
{
  return m_alternatingScaleBarType;
}

te::layout::EnumType* te::layout::EnumScaleType::getDoubleAlternatingScaleBarType() const
{
  return m_doubleAlternatingScaleBarType;
}

te::layout::EnumType* te::layout::EnumScaleType::getHollowScaleBarType() const
{
  return m_hollowScaleBarType;
}

