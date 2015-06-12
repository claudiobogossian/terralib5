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
  \file EnumStarType.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnumStarType.h"

te::layout::EnumStarType::EnumStarType() :
  m_noneType(0),
  m_star5points(0),
	m_star6points(0),
	m_star7points(0),
	m_star8points(0)
{
  init();
}

te::layout::EnumStarType::~EnumStarType()
{
  
}

void te::layout::EnumStarType::init()
{
  m_noneType = createEnum("NoneScale", this, "None Scale");

  m_star5points = createEnum("5 points", this);

  m_star6points = createEnum("6 points", this);

	m_star7points = createEnum("7 points", this);

	m_star8points = createEnum("8 points", this);

}

te::layout::EnumType* te::layout::EnumStarType::getNoneType() const
{
  return m_noneType;
}

te::layout::EnumType* te::layout::EnumStarType::getStar1Type() const
{
  return m_star5points;
}

te::layout::EnumType* te::layout::EnumStarType::getStar2Type() const
{
  return m_star6points;
}

te::layout::EnumType* te::layout::EnumStarType::getStar3Type() const
{
  return m_star7points;
}

te::layout::EnumType* te::layout::EnumStarType::getStar4Type() const
{
  return m_star8points;
}
