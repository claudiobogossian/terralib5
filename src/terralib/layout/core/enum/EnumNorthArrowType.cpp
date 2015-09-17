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
  \file EnumNorthArrowType.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnumNorthArrowType.h"

te::layout::EnumNorthArrowType::EnumNorthArrowType() :
  m_noneType(0),
  m_northArrow1(0),
  m_northArrow2(0),
  m_northArrow3(0)
{
  init();
}

te::layout::EnumNorthArrowType::~EnumNorthArrowType()
{
  
}

void te::layout::EnumNorthArrowType::init()
{
  m_noneType = createEnum("NoneScale", this, "None Scale");

  m_northArrow1 = createEnum("North Arrow 1", this);

  m_northArrow2 = createEnum("North Arrow 2", this);

  m_northArrow3 = createEnum("North Arrow 3", this);

}

te::layout::EnumType* te::layout::EnumNorthArrowType::getNoneType() const
{
  return m_noneType;
}

te::layout::EnumType* te::layout::EnumNorthArrowType::getNorthArrowType1() const
{
  return m_northArrow1;
}

te::layout::EnumType* te::layout::EnumNorthArrowType::getNorthArrowType2() const
{
  return m_northArrow2;
}

te::layout::EnumType* te::layout::EnumNorthArrowType::getNorthArrowType3() const
{
  return m_northArrow3;
}