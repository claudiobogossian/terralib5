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
  \file terralib/dataaccess/core/dataset/AbstractConstraint.cpp

  \brief A base class for constraints.
*/

// TerraLib
#include "../../../datatype/Property.h"
#include "AbstractConstraint.h"
#include "DataSetType.h"

te::da::core::AbstractConstraint::AbstractConstraint(unsigned int id)
  : m_id(id),
    m_dt(0),
    m_name("")
{  
}

te::da::core::AbstractConstraint::AbstractConstraint(const std::string& name, unsigned int id)
  : m_id(id),
    m_dt(0),
    m_name(name)
{
}

te::da::core::AbstractConstraint::AbstractConstraint(const AbstractConstraint& rhs)
  : m_id(rhs.m_id),
    m_dt(0),
    m_name(rhs.m_name)
{
}

te::da::core::AbstractConstraint& te::da::core::AbstractConstraint::operator=(const AbstractConstraint& rhs)
{
  if(this != &rhs)
  {
    m_id = rhs.m_id;

    m_dt = 0;

    m_name = rhs.m_name;
  }

  return *this;
}
