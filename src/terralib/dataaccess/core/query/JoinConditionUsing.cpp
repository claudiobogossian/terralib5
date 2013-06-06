/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/core/query/JoinConditionUsing.cpp

  \brief JoinConditionUsing class can be used to model a USING clause in a Join.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "Expression.h"
#include "JoinConditionUsing.h"

// STL
#include <cassert>

te::da::core::JoinConditionUsing::JoinConditionUsing(const JoinConditionUsing& rhs)
  : m_fields(rhs.m_fields)
{
}

te::da::core::JoinConditionUsing::~JoinConditionUsing()
{
}

te::da::core::JoinConditionUsing& te::da::core::JoinConditionUsing::operator=(const JoinConditionUsing& rhs)
{
  if(this != &rhs)
  {
    m_fields = rhs.m_fields;
  }

  return *this;
}

te::da::core::JoinCondition* te::da::core::JoinConditionUsing::clone() const
{
  return new JoinConditionUsing(*this);
}

void te::da::core::JoinConditionUsing::push_back(Expression* e)
{
  m_fields.push_back(e);
}

const te::da::core::Expression* te::da::core::JoinConditionUsing::operator[](std::size_t i) const
{
  assert(i < m_fields.size());
  return &(m_fields[i]);
}

void te::da::core::JoinConditionUsing::erase(std::size_t i)
{
  assert(i < m_fields.size());
  m_fields.erase(m_fields.begin() + i);
}

void te::da::core::JoinConditionUsing::clear()
{
  m_fields.clear();
}
