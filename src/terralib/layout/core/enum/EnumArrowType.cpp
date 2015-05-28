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
  \file EnumArrowType.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnumArrowType.h"

te::layout::EnumArrowType::EnumArrowType() :
  m_noneType(0),
  m_rightArrow(0),
	m_doubleArrow(0)
{
  init();
}

te::layout::EnumArrowType::~EnumArrowType()
{
  
}

void te::layout::EnumArrowType::init()
{
  m_noneType = createEnum("NoneScale", this, "None Scale");

  m_rightArrow = createEnum("Right Arrow", this);

  m_doubleArrow = createEnum("Double Arrow", this);

}

te::layout::EnumType* te::layout::EnumArrowType::getNoneType() const
{
  return m_noneType;
}

te::layout::EnumType* te::layout::EnumArrowType::getRightArrowType() const
{
  return m_rightArrow;
}

te::layout::EnumType* te::layout::EnumArrowType::getDoubleArrowType() const
{
  return m_doubleArrow;
}