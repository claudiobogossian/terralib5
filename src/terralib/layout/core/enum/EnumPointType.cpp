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
  \file EnumPointType.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnumPointType.h"

te::layout::EnumPointType::EnumPointType() :
  m_noneType(0),
  m_star(0),
  m_circle(0),
  m_x(0),
  m_square(0),
  m_rhombus(0),
  m_cross(0)
{
  init();
}

te::layout::EnumPointType::~EnumPointType()
{
  
}

void te::layout::EnumPointType::init()
{
  m_noneType = new EnumType(0, "NoneScale", this);
  m_noneType->setLabel("None Scale");
  m_enums.push_back(m_noneType);

  m_star = new EnumType(1, "Star", this);
  m_star->setLabel("Star");
  m_enums.push_back(m_star);

  m_circle = new EnumType(2, "Circle", this);
  m_circle->setLabel("Circle");
  m_enums.push_back(m_circle);

  m_x = new EnumType(3, "X", this);
  m_x->setLabel("X");
  m_enums.push_back(m_x);

  m_square = new EnumType(4, "Square", this);
  m_square->setLabel("Square");
  m_enums.push_back(m_square);

  m_rhombus = new EnumType(5, "Rhombus", this);
  m_rhombus->setLabel("Rhombus");
  m_enums.push_back(m_rhombus);    

  m_cross = new EnumType(6, "Cross", this);
  m_cross->setLabel("Cross");
  m_enums.push_back(m_cross);  
}

te::layout::EnumType* te::layout::EnumPointType::getNoneType() const
{
  return m_noneType;
}

te::layout::EnumType* te::layout::EnumPointType::getStarType() const
{
  return m_star;
}

te::layout::EnumType* te::layout::EnumPointType::getCircleType() const
{
  return m_circle;
}

te::layout::EnumType* te::layout::EnumPointType::getXType() const
{
  return m_x;
}

te::layout::EnumType* te::layout::EnumPointType::getSquareType() const
{
  return m_square;
}

te::layout::EnumType* te::layout::EnumPointType::getRhombusType() const
{
  return m_rhombus;
}

te::layout::EnumType* te::layout::EnumPointType::getCrossType() const
{
  return m_cross;
}

