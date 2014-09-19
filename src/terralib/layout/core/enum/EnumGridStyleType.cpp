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
  \file EnumGridStyleType.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnumGridStyleType.h"

te::layout::EnumGridStyleType::EnumGridStyleType() :
  m_styleNone(0),
  m_styleContinuous(0),
  m_styleCross(0)
{
  init();
}

te::layout::EnumGridStyleType::~EnumGridStyleType()
{
  if(m_styleNone)
  {
    delete m_styleNone;
    m_styleNone = 0;
  }
  if(m_styleContinuous)
  {
    delete m_styleContinuous;
    m_styleContinuous = 0;
  }
  if(m_styleCross)
  {
    delete m_styleCross;
    m_styleCross = 0;
  }
}

void te::layout::EnumGridStyleType::init()
{
  m_styleNone = new EnumType(0, "None");
  m_enums.push_back(m_styleNone);

  m_styleContinuous = new EnumType(1, "Continuous");
  m_enums.push_back(m_styleContinuous);

  m_styleCross = new EnumType(2, "Cross");
  m_enums.push_back(m_styleCross);
}

te::layout::EnumType* te::layout::EnumGridStyleType::getStyleNone() const
{
  return m_styleNone;
}

te::layout::EnumType* te::layout::EnumGridStyleType::getStyleContinuous() const
{
  return m_styleContinuous;
}

te::layout::EnumType* te::layout::EnumGridStyleType::getStyleCross() const
{
  return m_styleCross;
}
