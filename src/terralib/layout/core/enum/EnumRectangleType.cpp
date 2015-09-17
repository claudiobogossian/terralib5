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
  \file EnumRectangleType.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnumRectangleType.h"

te::layout::EnumRectangleType::EnumRectangleType() :
  m_noneType(0),
  m_simpleRect(0),
  m_roundedRect(0),
  m_singleCornerTrimmedRect(0)
{
  init();
}

te::layout::EnumRectangleType::~EnumRectangleType()
{
  
}

void te::layout::EnumRectangleType::init()
{
  m_noneType = createEnum("NoneScale", this, "None Scale");

  m_simpleRect = createEnum("Simple Rectangle", this);

  m_roundedRect = createEnum("Rounded Rectangle", this);

  m_singleCornerTrimmedRect = createEnum("Single Corner Trimmed Rectangle", this);
}

te::layout::EnumType* te::layout::EnumRectangleType::getNoneType() const
{
  return m_noneType;
}

te::layout::EnumType* te::layout::EnumRectangleType::getSimpleRectangleType() const
{
  return m_simpleRect;
}

te::layout::EnumType* te::layout::EnumRectangleType::getRoundedRetangleType() const
{
  return m_roundedRect;
}

te::layout::EnumType* te::layout::EnumRectangleType::getSingleCornerTrimmedRectangleType() const
{
  return m_singleCornerTrimmedRect;
}