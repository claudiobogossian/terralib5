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
  \file ScalarCapabilities.cpp
  
  \brief Scalar capabilities include the ability to process logical expressions, comparisons and arithmetic operations including the ability to process a list of named functions.
 */

// TerraLib
#include "ArithmeticOperators.h"
#include "ComparisonOperators.h"
#include "ScalarCapabilities.h"

te::fe::ScalarCapabilities::ScalarCapabilities()
  : m_hasLogicalOperators(false),
    m_comparisonOperators(0),
    m_arithmeticOperators(0)
{
}

te::fe::ScalarCapabilities::~ScalarCapabilities()
{
  delete m_comparisonOperators;
  delete m_arithmeticOperators;
}

bool te::fe::ScalarCapabilities::hasLogicalOperators() const
{
  return m_hasLogicalOperators;
}

void te::fe::ScalarCapabilities::enableLogicalOperators()
{
  m_hasLogicalOperators = true;
}

void te::fe::ScalarCapabilities::disableLogicalOperators()
{
  m_hasLogicalOperators = false;
}

void te::fe::ScalarCapabilities::setComparisonOperators(ComparisonOperators* cOps)
{
  delete m_comparisonOperators;
  m_comparisonOperators = cOps;
}

const te::fe::ComparisonOperators* te::fe::ScalarCapabilities::getComparisonOperators() const
{
  return m_comparisonOperators;
}

void te::fe::ScalarCapabilities::setArithmeticOperators(ArithmeticOperators* aOps)
{
  delete m_arithmeticOperators;
  m_arithmeticOperators = aOps;
}

const te::fe::ArithmeticOperators* te::fe::ScalarCapabilities::getArithmeticOperators() const
{
  return m_arithmeticOperators;
}

