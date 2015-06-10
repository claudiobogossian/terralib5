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
  \file BinaryComparisonOp.cpp
  
  \brief A class for binary comparison operators.  
 */

// TerraLib
#include "BinaryComparisonOp.h"
#include "Expression.h"
#include "Globals.h"

// STL
#include <cassert>

te::fe::BinaryComparisonOp::BinaryComparisonOp(const char* opName)
  : ComparisonOp(opName),
    m_first(0),
    m_second(0),
    m_matchCase(true)
{
}

te::fe::BinaryComparisonOp::BinaryComparisonOp(const char* opName, Expression* f, Expression* s)
  : ComparisonOp(opName),
    m_first(f),
    m_second(s),
    m_matchCase(true)
{
}

te::fe::BinaryComparisonOp::~BinaryComparisonOp()
{
  delete m_first;
  delete m_second;
}

void te::fe::BinaryComparisonOp::setFirst(Expression* first)
{
  delete m_first;
  m_first = first;
}

te::fe::Expression* te::fe::BinaryComparisonOp::getFirst() const
{
  return m_first;
}

void te::fe::BinaryComparisonOp::setSecond(Expression* second)
{
  delete m_second;
  m_second = second;
}

te::fe::Expression* te::fe::BinaryComparisonOp::getSecond() const
{
  return m_second;
}

void te::fe::BinaryComparisonOp::enableMatchCase()
{
  m_matchCase = true;
}

void te::fe::BinaryComparisonOp::disableMatchCase()
{
  m_matchCase = false;
}

