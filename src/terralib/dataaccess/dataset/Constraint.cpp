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
  \file terralib/dataaccess/dataset/Constraint.cpp

  \brief A class that describes a constraint.
*/

// TerraLib
#include "../../datatype/Property.h"
#include "Constraint.h"
#include "DataSetType.h"

te::da::Constraint::Constraint(unsigned int id)
  : m_id(id),
    m_dt(0),
    m_name("")
{  
}

te::da::Constraint::Constraint(const std::string& name, unsigned int id)
  : m_id(id),
    m_dt(0),
    m_name(name)
{
}

te::da::Constraint::Constraint(const Constraint& rhs)
  : m_id(rhs.m_id),
    m_dt(0),
    m_name(rhs.m_name)
{
}

te::da::Constraint& te::da::Constraint::operator=(const Constraint& rhs)
{
  if(this != &rhs)
  {
    m_id = rhs.m_id;

    m_dt = 0;

    m_name = rhs.m_name;
  }

  return *this;
}


