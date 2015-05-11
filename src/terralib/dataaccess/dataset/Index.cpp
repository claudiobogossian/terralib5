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
  \file terralib/dataaccess/dataset/Index.cpp

  \brief It describes an index associated to a DataSetType.
*/

// TerraLib
#include "../../datatype/Property.h"
#include "DataSetType.h"
#include "Index.h"

te::da::Index::Index(DataSetType* dt, unsigned int id)
  : m_id(id),
    m_type(B_TREE_TYPE),
    m_dt(dt)
{
  if(m_dt)
    m_dt->add(this);
}

te::da::Index::Index(const std::string& name, IndexType t, DataSetType* dt, unsigned int id)
  : m_id(id),
    m_type(t),
    m_dt(dt),
    m_name(name)  
{
  if(m_dt)
    m_dt->add(this);
}

te::da::Index::Index(const std::string& name,
                     IndexType t,
                     const std::vector<te::dt::Property*>& properties,
                     DataSetType* dt,
                     unsigned int id)
  : m_id(id),
    m_type(t),
    m_dt(dt),
    m_name(name),    
    m_properties(properties)
{
  if(m_dt)
    m_dt->add(this);
}

te::da::Index::Index(const Index& rhs)
  : m_id(rhs.m_id),
    m_type(rhs.m_type),
    m_dt(0),
    m_name(rhs.m_name),
    m_properties(rhs.m_properties)
{
}

te::da::Index& te::da::Index::operator=(const Index& rhs)
{
  if(this != &rhs)
  {
    m_id = rhs.m_id;
    m_type = rhs.m_type;
    m_dt = 0;
    m_name = rhs.m_name;
    m_properties = rhs.m_properties;
  }

  return *this;
}

bool te::da::Index::has(te::dt::Property* p)
{
  size_t size = m_properties.size();

  for(size_t i = 0; i < size; ++i)
    if(m_properties[i] == p)
      return true;

  return false;
}

void te::da::Index::replace(te::dt::Property* p, te::dt::Property* pp)
{
  std::size_t size = m_properties.size();

  for(std::size_t i = 0; i < size; ++i)
    if(m_properties[i] == p)
    {
      m_properties[i] = pp;
      break;
    }
}

te::da::Index* te::da::Index::clone()
{
  return new Index(*this);
}
