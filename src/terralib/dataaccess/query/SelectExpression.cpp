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
  \file terralib/dataaccess/query/SelectExpression.cpp

  \brief A Select can be used as a source of information in another query.
*/

// TerraLib
#include "Select.h"
#include "SelectExpression.h"

te::da::SelectExpression::SelectExpression(Select* s)
 : m_select(s)
{
}

te::da::SelectExpression::SelectExpression(const Select& s)
  : m_select(static_cast<Select*>(s.clone()))
{
}

te::da::SelectExpression::SelectExpression(const SelectExpression& rhs)
  : m_select(0)
{
  m_select.reset(rhs.m_select.get() ? static_cast<Select*>(rhs.m_select->clone()) : 0);
}

te::da::SelectExpression& te::da::SelectExpression::operator=(const SelectExpression& rhs)
{
  if(this != &rhs)
    m_select.reset(rhs.m_select.get() ? static_cast<Select*>(rhs.m_select->clone()) : 0);

  return *this;
}

te::da::Expression* te::da::SelectExpression::clone() const
{
  return new SelectExpression(*this);
}

te::da::Select* te::da::SelectExpression::getSelect() const
{
  return m_select.get();
}

void te::da::SelectExpression::setSelect(Select* s)
{
  m_select.reset(s);
}
