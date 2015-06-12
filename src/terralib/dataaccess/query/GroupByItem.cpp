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
  \file terralib/dataaccess/query/GroupByItem.cpp

  \brief A class that can be used in an ORDER BY clause to sort the items of a resulting query.
*/

// TerraLib
#include "Expression.h"
#include "GroupByItem.h"
#include "PropertyName.h"

te::da::GroupByItem::GroupByItem(const Expression& e)
  : m_field(e.clone())
{
}

te::da::GroupByItem::GroupByItem(Expression* e)
  : m_field(e)
{
}

te::da::GroupByItem::GroupByItem(const std::string& propertyName)
  : m_field(new PropertyName(propertyName))
{
}

te::da::GroupByItem::GroupByItem(const GroupByItem& rhs)
  : m_field(0)
{
  m_field.reset(rhs.m_field.get() ? rhs.m_field->clone() : 0);
}

te::da::GroupByItem::~GroupByItem()
{
}

te::da::GroupByItem& te::da::GroupByItem::operator=(const GroupByItem& rhs)
{
  if(this != &rhs)
  {
    m_field.reset(rhs.m_field.get() ? m_field->clone() : 0);
  }

  return *this;
}

void te::da::GroupByItem::setExpression(Expression* e)
{
  m_field.reset(e);
}

te::da::Expression* te::da::GroupByItem::getExpression() const
{
  return m_field.get();
}

