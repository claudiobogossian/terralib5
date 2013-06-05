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
  \file terralib/dataaccess/core/dataset/CheckConstraint.cpp

  \brief A class that describes a check constraint.
*/

// TerraLib
#include "CheckConstraint.h"

te::da::core::CheckConstraint::CheckConstraint(const std::string& name,
                                               const std::string& expression)
  : AbstractConstraint(name),
    m_expression(expression)
{
}

te::da::core::CheckConstraint::CheckConstraint(const CheckConstraint& rhs)
  : AbstractConstraint(rhs),
    m_expression(rhs.m_expression)
{
}

te::da::core::CheckConstraint& te::da::core::CheckConstraint::operator=(const CheckConstraint& rhs)
{
  if(this != &rhs)
  {
    AbstractConstraint::operator=(rhs);

    m_expression = rhs.m_expression;
  }

  return *this;
}

te::da::core::AbstractConstraint* te::da::core::CheckConstraint::clone()
{
  return new CheckConstraint(*this);
}
