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
  \file EnumAlignmentType.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnumAlignmentType.h"

te::layout::EnumAlignmentType::EnumAlignmentType() :
  m_noneType(0),
  m_alignmentCenter(0),
	m_alignmentLeft(0),
	m_alignmentRight(0)
{
  init();
}

te::layout::EnumAlignmentType::~EnumAlignmentType()
{
  
}

void te::layout::EnumAlignmentType::init()
{
  m_noneType = createEnum("NoneScale", this, "None Scale");

  m_alignmentCenter = createEnum("Center", this);

  m_alignmentLeft = createEnum("Left", this);

	m_alignmentRight = createEnum("Right", this);
}

te::layout::EnumType* te::layout::EnumAlignmentType::getNoneType() const
{
  return m_noneType;
}

te::layout::EnumType* te::layout::EnumAlignmentType::getAlignmentCenterType() const
{
  return m_alignmentCenter;
}

te::layout::EnumType* te::layout::EnumAlignmentType::getAlignmentLeftType() const
{
  return m_alignmentLeft;
}

te::layout::EnumType* te::layout::EnumAlignmentType::getAlignmentRightType() const
{
	return m_alignmentRight;
}