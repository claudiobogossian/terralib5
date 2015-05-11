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
  \file PropertyIsBetween.cpp
  
  \brief The PropertyIsBetween element is defined as a compact way of encoding a range check.  
 */

// TerraLib
#include "Expression.h"
#include "Globals.h"
#include "PropertyIsBetween.h"

te::fe::PropertyIsBetween::PropertyIsBetween()
  : ComparisonOp(Globals::sm_propertyIsBetween),
    m_expression(0),
    m_lowerBoundary(0),
    m_upperBoundary(0)
{
}

te::fe::PropertyIsBetween::~PropertyIsBetween()
{
  delete m_expression;
  delete m_lowerBoundary;
  delete m_upperBoundary;
}

void te::fe::PropertyIsBetween::setExpression(Expression* e)
{
  delete m_expression;
  m_expression = e;
}

te::fe::Expression* te::fe::PropertyIsBetween::getExpression() const
{
  return m_expression;
}

void te::fe::PropertyIsBetween::setLowerBoundary(Expression* e)
{
  delete m_lowerBoundary;
  m_lowerBoundary = e;
}

te::fe::Expression* te::fe::PropertyIsBetween::getLowerBoundary() const
{
  return m_lowerBoundary;
}

void te::fe::PropertyIsBetween::setUpperBoundary(Expression* e)
{
  delete m_upperBoundary;
  m_upperBoundary = e;
}

te::fe::Expression* te::fe::PropertyIsBetween::getUpperBoundary() const
{
  return m_upperBoundary;
}

