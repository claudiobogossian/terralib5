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
  \file terralib/dataaccess/query/OrderByItem.cpp

  \brief A class that can be used in an ORDER BY clause to sort the items of a resulting query.
*/

// TerraLib
#include "Expression.h"
#include "OrderByItem.h"
#include "PropertyName.h"

te::da::OrderByItem::OrderByItem(const Expression& e, SortOrder order)
  : m_field(e.clone()),
    m_order(order)
{
}

te::da::OrderByItem::OrderByItem(Expression* e, SortOrder order)
  : m_field(e),
    m_order(order)
{
}

te::da::OrderByItem::OrderByItem(const std::string& propertyName, SortOrder order)
  : m_field(new PropertyName(propertyName)),
    m_order(order)
{
}

te::da::OrderByItem::OrderByItem(const OrderByItem& rhs)
  : m_field(0),
    m_order(rhs.m_order)
{
  m_field.reset(rhs.m_field.get() ? rhs.m_field->clone() : 0);
}

te::da::OrderByItem::~OrderByItem()
{
}

te::da::OrderByItem& te::da::OrderByItem::operator=(const OrderByItem& rhs)
{
  if(this != &rhs)
  {
    m_field.reset(rhs.m_field.get() ? m_field->clone() : 0);
    m_order = rhs.m_order;
  }

  return *this;
}

void te::da::OrderByItem::setExpression(Expression* e)
{
  m_field.reset(e);
}

te::da::Expression* te::da::OrderByItem::getExpression() const
{
  return m_field.get();
}

void te::da::OrderByItem::setSortOrder(SortOrder o)
{
  m_order = o;
}

te::da::SortOrder te::da::OrderByItem::getSortOrder() const
{
  return m_order;
}

