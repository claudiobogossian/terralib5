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
  \file terralib/dataaccess/core/query/Literal.cpp

  \brief This class models a literal value.
*/

// TerraLib
#include "../../../datatype/AbstractData.h"
#include "Literal.h"

te::da::core::Literal::Literal(te::dt::AbstractData* value)
  : m_value(value)
{
}

te::da::core::Literal::Literal(const te::dt::AbstractData& value)
  : m_value(value.clone())
{
}

te::da::core::Literal::Literal(const Literal& rhs)
  : m_value(0)
{
  m_value.reset(rhs.m_value.get() ? rhs.m_value->clone() : 0);
}

te::da::core::Literal::~Literal()
{
}

te::da::core::Literal& te::da::core::Literal::operator=(const Literal& rhs)
{
  if(this != &rhs)
  {
    m_value.reset(rhs.m_value.get() ? rhs.m_value->clone() : 0);
  }

  return *this;
}

te::da::core::Expression* te::da::core::Literal::clone() const
{
  return new Literal(*this);
}

te::dt::AbstractData* te::da::core::Literal::getValue() const
{
  return m_value.get();
}

void te::da::core::Literal::setValue(te::dt::AbstractData* v)
{
  m_value.reset(v);
}