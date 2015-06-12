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
  \file terralib/dataaccess/query/Field.cpp

  \brief The Field class can be used to model an expression that takes part of the output items of a SELECT.
*/

// TerraLib
#include "Expression.h"
#include "Field.h"
#include "PropertyName.h"

te::da::Field::Field(const Expression& e, const std::string& alias)
  : m_field(e.clone()),
    m_alias(0)
{
  m_alias.reset(alias.empty() ? 0 : new std::string(alias));
}

te::da::Field::Field(Expression* e, std::string* alias)
  : m_field(e),
    m_alias(alias)
{
}

te::da::Field::Field(const std::string& propertyName, const std::string& alias)
  : m_field(0),
    m_alias(0)
{
  m_field.reset(new PropertyName(propertyName));
  m_alias.reset(alias.empty() ? 0 : new std::string(alias));
}

te::da::Field::Field(const Field& rhs)
  : m_field(0),
    m_alias(0)
{
  m_field.reset(rhs.m_field.get() ? rhs.m_field->clone() : 0);
  m_alias.reset(rhs.m_alias.get() ? new std::string(*rhs.m_alias) : 0);
}

te::da::Field::~Field()
{
}

te::da::Field& te::da::Field::operator=(const Field& rhs)
{
  if(this != &rhs)
  {
    m_field.reset(rhs.m_field.get() ? rhs.m_field->clone() : 0);
    m_alias.reset(rhs.m_alias.get() ? new std::string(*rhs.m_alias) : 0);
  }

  return *this;
}

void te::da::Field::setExpression(Expression* e)
{
  m_field.reset(e);
}

te::da::Expression* te::da::Field::getExpression() const
{
  return m_field.get();
}

void te::da::Field::setAlias(std::string* alias)
{
  m_alias.reset(alias);
}

std::string* te::da::Field::getAlias() const
{
  return m_alias.get();
}

