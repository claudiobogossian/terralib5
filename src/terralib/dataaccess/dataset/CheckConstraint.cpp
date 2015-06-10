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
  \file terralib/dataaccess/dataset/CheckConstraint.cpp

  \brief A class that describes a check constraint.
*/

// TerraLib
#include "../../datatype/Property.h"
#include "CheckConstraint.h"
#include "DataSetType.h"

te::da::CheckConstraint::CheckConstraint(DataSetType* dt, unsigned int id)
  : Constraint(id)
{  
  if(dt)
    dt->add(this);
}

te::da::CheckConstraint::CheckConstraint(const std::string& name,
                                         DataSetType* dt,
                                         unsigned int id)
  : Constraint(name, id)
{  
  if(dt)
    dt->add(this);
}

te::da::CheckConstraint::CheckConstraint(const CheckConstraint& rhs)
  : Constraint(rhs),
    m_expression(rhs.m_expression)
{
}

te::da::CheckConstraint& te::da::CheckConstraint::operator=(const CheckConstraint& rhs)
{
  if(this != &rhs)
  {
    Constraint::operator=(rhs);

    m_expression = rhs.m_expression;
  }

  return *this;
}

te::da::Constraint* te::da::CheckConstraint::clone()
{
  return new CheckConstraint(*this);
}
