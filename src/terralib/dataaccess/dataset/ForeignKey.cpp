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
  \file terralib/dataaccess/dataset/ForeignKey.cpp

  \brief It models a foreign key constraint for a DataSetType.
*/

// TerraLib
#include "../../datatype/Property.h"
#include "DataSetType.h"
#include "ForeignKey.h"

te::da::ForeignKey::ForeignKey(unsigned int id)
  : Constraint(id),
    m_onDelete(NO_ACTION),
    m_onUpdate(NO_ACTION),
    m_refDt(0)    
{
}

te::da::ForeignKey::ForeignKey(const std::string& name,
                               unsigned int id)
  : Constraint(name, id),
    m_onDelete(NO_ACTION),
    m_onUpdate(NO_ACTION),
    m_refDt(0)
{
}

te::da::ForeignKey::ForeignKey(const ForeignKey& rhs)
  : Constraint(rhs),
    m_onDelete(rhs.m_onDelete),
    m_onUpdate(rhs.m_onUpdate),
    m_refDt(rhs.m_refDt),
    m_properties(rhs.m_properties),
    m_refProperties(rhs.m_refProperties)
{
}

te::da::ForeignKey& te::da::ForeignKey::operator=(const ForeignKey& rhs)
{
  if(this != &rhs)
  {
    Constraint::operator=(rhs);

    m_onDelete = rhs.m_onDelete;

    m_onUpdate = rhs.m_onUpdate;

    m_refDt = rhs.m_refDt;

    m_properties = rhs.m_properties;
    
    m_refProperties = rhs.m_refProperties;
  }

  return *this;
}

bool te::da::ForeignKey::has(te::dt::Property* p)
{
  size_t size = m_properties.size();

  for(size_t i = 0; i < size; ++i)
    if(m_properties[i] == p)
      return true;

  return false;
}

bool te::da::ForeignKey::isReferenced(te::dt::Property* p)
{
  size_t size = m_refProperties.size();

  for(size_t i = 0; i < size; ++i)
    if(m_refProperties[i] == p)
      return true;

  return false;
}

void te::da::ForeignKey::replace(te::dt::Property* p, te::dt::Property* pp)
{
  {
    std::size_t size = m_properties.size();

    for(std::size_t i = 0; i < size; ++i)
      if(m_properties[i] == p)
      {
        m_properties[i] = pp;
        break;
      }
  }

  {
    std::size_t size = m_refProperties.size();

    for(std::size_t i = 0; i < size; ++i)
      if(m_refProperties[i] == p)
      {
        m_properties[i] = pp;
        break;
      }
  }
}

te::da::Constraint* te::da::ForeignKey::clone()
{
  return new ForeignKey(*this);
}

