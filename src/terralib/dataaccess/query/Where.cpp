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
  \file terralib/dataaccess/query/Where.cpp

  \brief A class that can be used to model a filter expression that can be applied to a query.
*/

// TerraLib
#include "Expression.h"
#include "Where.h"

te::da::Where::Where(Expression* e)
  : m_exp(e)
{
}

te::da::Where::Where(const Expression& e)
  : m_exp(e.clone())
{
}

te::da::Where::Where(const Where& rhs)
  : m_exp(0)
{
  m_exp.reset(rhs.m_exp.get() ? rhs.m_exp->clone() : 0);
}

te::da::Where::~Where()
{
}
        
te::da::Where& te::da::Where::operator=(const Where& rhs)
{
  if(this != &rhs)
  {
    m_exp.reset(rhs.m_exp.get() ? rhs.m_exp->clone() : 0);
  }

  return *this;
}

te::da::Expression* te::da::Where::getExp() const
{
  return m_exp.get();
}

void te::da::Where::setExp(Expression* exp)
{
  m_exp.reset(exp);
}

