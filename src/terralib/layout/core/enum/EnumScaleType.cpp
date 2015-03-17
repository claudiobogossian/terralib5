/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  m_noneType = new EnumType(0, "NoneScale", this);
  m_noneType->setLabel("None Scale");
  m_enums.push_back(m_noneType);

  m_alternatingScaleBarType = new EnumType(1, "AlternantingScaleBar", this);
  m_alternatingScaleBarType->setLabel("Alternanting Scale Bar");
  m_enums.push_back(m_alternatingScaleBarType);

  m_doubleAlternatingScaleBarType = new EnumType(2, "DoubleAlternantingScaleBar", this);
  m_doubleAlternatingScaleBarType->setLabel("Double Alternanting Scale Bar");
  m_enums.push_back(m_doubleAlternatingScaleBarType);

  m_hollowScaleBarType = new EnumType(3, "HollowScaleBar", this);
  m_hollowScaleBarType->setLabel("Hollow Scale Bar");
  m_enums.push_back(m_hollowScaleBarType);
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

