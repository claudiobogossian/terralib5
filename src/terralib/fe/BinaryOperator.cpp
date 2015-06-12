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
  \file BinaryOperator.cpp
  
  \brief This class can be used to represent binary operation expressions.
 */

// TerraLib
#include "BinaryOperator.h"
#include "Globals.h"

// STL
#include <cassert>

te::fe::BinaryOperator::BinaryOperator(const char* opName)
  : m_opName(opName),
    m_first(0),
    m_second(0)
{
}

te::fe::BinaryOperator::BinaryOperator(const char* opName, Expression* first, Expression* second)
  : m_opName(opName),
    m_first(first),
    m_second(second)
{
}

te::fe::BinaryOperator::~BinaryOperator()
{
  delete m_first;
  delete m_second;
}

void te::fe::BinaryOperator::setFirst(Expression* first)
{
  delete m_first;
  m_first = first;
}

te::fe::Expression* te::fe::BinaryOperator::getFirst() const
{
  return m_first;
}

void te::fe::BinaryOperator::setSecond(Expression* second)
{
  delete m_second;
  m_second = second;
}

te::fe::Expression* te::fe::BinaryOperator::getSecond() const
{
  return m_second;
}

te::fe::Expression* te::fe::BinaryOperator::clone() const
{
  te::fe::Expression* arg1;
  m_first ? arg1 = m_first->clone() : arg1 = 0;
  
  te::fe::Expression* arg2;
  m_second ? arg2 = m_second->clone() : arg2 = 0;

  return new BinaryOperator(m_opName, arg1, arg2);
}
