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
  \file terralib/dataaccess/query/JoinConditionOn.cpp

  \brief JoinConditionOn is a boolean expression and it specifies which items in a join are considered to match.
*/

// TerraLib
#include "Expression.h"
#include "JoinConditionOn.h"

te::da::JoinConditionOn::JoinConditionOn(Expression* e)
  : m_condition(e)
{
}

te::da::JoinConditionOn::JoinConditionOn(const Expression& e)
  : m_condition(0)
{
  m_condition.reset(e.clone());
}

te::da::JoinConditionOn::JoinConditionOn(const JoinConditionOn& rhs)
  : m_condition(0)
{
  m_condition.reset(rhs.m_condition.get() ? rhs.m_condition->clone() : 0);
}

te::da::JoinConditionOn::~JoinConditionOn()
{
}


te::da::JoinConditionOn& te::da::JoinConditionOn::operator=(const JoinConditionOn& rhs)
{
  if(this != &rhs)
  {
    m_condition.reset(rhs.m_condition.get() ? rhs.m_condition->clone() : 0);
  }

  return *this;
}

te::da::JoinCondition* te::da::JoinConditionOn::clone() const
{
  return new JoinConditionOn(*this);
}

void te::da::JoinConditionOn::setCondition(Expression* c)
{
  m_condition.reset(c);
}

te::da::Expression* te::da::JoinConditionOn::getCondition() const
{
  return m_condition.get();
}

