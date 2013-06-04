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
  \file terralib/dataaccess/core/dataset/UniqueKey.cpp

  \brief It describes a unique key (uk) constraint.
*/

// TerraLib
#include "../../../datatype/Property.h"
#include "DataSetType.h"
#include "UniqueKey.h"

te::da::core::UniqueKey::UniqueKey(DataSetType* dt, unsigned int id)
  : AbstractConstraint(id),
    m_index(0)
{
  if(dt)
    dt->add(this);
}

te::da::core::UniqueKey::UniqueKey(const std::string& name, DataSetType* dt, unsigned int id)
  : AbstractConstraint(name, id),
    m_index(0)
{ 
  if(dt)
    dt->add(this);
}

te::da::core::UniqueKey::UniqueKey(const UniqueKey& rhs)
  : AbstractConstraint(rhs.getName(), 0),
    m_index(rhs.m_index),
    m_properties(rhs.m_properties)
{
}

te::da::core::UniqueKey& te::da::core::UniqueKey::operator=(const UniqueKey& rhs)
{
  if(this != &rhs)
  {
    AbstractConstraint::operator=(rhs);

    m_index = rhs.m_index;

    m_properties = rhs.m_properties;
  }

  return *this;
}

bool te::da::core::UniqueKey::has(const te::dt::Property* p) const
{
  std::size_t size = m_properties.size();

  for(std::size_t i = 0; i < size; ++i)
    if(m_properties[i] == p)
      return true;

  return false;
}

void te::da::core::UniqueKey::replace(te::dt::Property* p, te::dt::Property* pp)
{
  std::size_t size = m_properties.size();

  for(std::size_t i = 0; i < size; ++i)
    if(m_properties[i] == p)
    {
      m_properties[i] = pp;
      break;
    }
}

te::da::core::AbstractConstraint* te::da::core::UniqueKey::clone()
{
  return new UniqueKey(*this);
}
