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
  \file terralib/dataaccess/dataset/PrimaryKey.cpp

  \brief A PrimaryKey describes a primary key (pk) constraint.
*/

// TerraLib
#include "../../datatype/Property.h"
#include "DataSetType.h"
#include "PrimaryKey.h"

te::da::PrimaryKey::PrimaryKey(DataSetType* dt, unsigned int id)
  : Constraint(id),
    m_index(0)
{
  if(dt)
    dt->add(this);
}

te::da::PrimaryKey::PrimaryKey(const std::string& name, DataSetType* dt, unsigned int id)
  : Constraint(name, id),
    m_index(0)    
{ 
  if(dt)
    dt->add(this);
}

te::da::PrimaryKey::PrimaryKey(const PrimaryKey& rhs)
  : Constraint(rhs.getName(), 0),
    m_index(rhs.m_index),
    m_properties(rhs.m_properties)
{
}

te::da::PrimaryKey& te::da::PrimaryKey::operator=(const PrimaryKey& rhs)
{
  if(this != &rhs)
  {
    Constraint::operator=(rhs);
    m_index = rhs.m_index;
    m_properties = rhs.m_properties;
  }

  return *this;
}

bool te::da::PrimaryKey::has(const te::dt::Property* p) const
{
  std::size_t size = m_properties.size();

  for(std::size_t i = 0; i < size; ++i)
    if(m_properties[i] == p)
      return true;

  return false;
}

void te::da::PrimaryKey::replace(te::dt::Property* p, te::dt::Property* pp)
{
  std::size_t size = m_properties.size();

  for(std::size_t i = 0; i < size; ++i)
    if(m_properties[i] == p)
    {
      m_properties[i] = pp;
      break;
    }
}

te::da::Constraint* te::da::PrimaryKey::clone()
{
  return new PrimaryKey(*this);
}

