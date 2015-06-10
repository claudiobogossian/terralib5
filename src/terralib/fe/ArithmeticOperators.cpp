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
  \file ArithmeticOperators.cpp
  
  \brief It is used to indicate what arithmetic operators the a service can support.
 */

// TerraLib
#include "ArithmeticOperators.h"
#include "FunctionName.h"

// STL
#include <cassert>

te::fe::ArithmeticOperators::ArithmeticOperators()
  : m_simpleArithmetic(false)
{
}

te::fe::ArithmeticOperators::~ArithmeticOperators()
{
  te::common::FreeContents(m_arithmeticOperators);
}

void te::fe::ArithmeticOperators::enableSimpleArithmetic()
{
  m_simpleArithmetic = true;
}

void te::fe::ArithmeticOperators::disableSimpleArithmetic()
{
  m_simpleArithmetic = false;
}

void te::fe::ArithmeticOperators::push_back(FunctionName* f)
{
  assert(f);
  m_arithmeticOperators.push_back(f);
}

const te::fe::FunctionName* te::fe::ArithmeticOperators::operator[](size_t i) const
{
  assert(i < m_arithmeticOperators.size());
  return m_arithmeticOperators[i];
}

