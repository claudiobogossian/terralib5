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
  \file terralib/datatype/CompositeProperty.cpp

  \brief A base class for a compound property type (non-atomic properties).
*/

// TerraLib
#include "../common/STLUtils.h"
#include "CompositeProperty.h"
#include "Enums.h"

// STL
#include <cassert>

te::dt::CompositeProperty::CompositeProperty(const std::string& cname, const std::string& name, unsigned int id, Property* parent)
  : Property(name, COMPOSITE_TYPE, id, parent),
    m_cname(cname)
{
}

te::dt::CompositeProperty::CompositeProperty(const CompositeProperty& rhs)
  : Property(rhs),
    m_cname(rhs.m_cname)
{
  copy(rhs.m_properties);
}

te::dt::CompositeProperty::~CompositeProperty()
{
  te::common::FreeContents(m_properties);
}

te::dt::CompositeProperty& te::dt::CompositeProperty::operator=(const CompositeProperty& rhs)
{
  if(this != &rhs)
  {
    Property::operator=(rhs);

    m_cname = rhs.m_cname;

    te::common::FreeContents(m_properties);
    
    m_properties.clear();

    copy(rhs.m_properties);
  }

  return *this;
}

void te::dt::CompositeProperty::add(Property* p)
{
  assert(p && p->getParent() == 0);
  m_properties.push_back(p);
  p->setParent(this);
}

void te::dt::CompositeProperty::add(const std::vector<Property*>& ps)
{
  std::size_t size = ps.size();

  for(std::size_t i = 0; i < size; ++i)
  {    
    assert(ps[i]->getParent() == 0);

    m_properties.push_back(ps[i]);

    ps[i]->setParent(this);
  }
}

void te::dt::CompositeProperty::add(const boost::ptr_vector<te::dt::Property>& ps)
{
  std::size_t size = ps.size();

  for(std::size_t i = 0; i < size; ++i)
  {
    te::dt::Property* p = ps[i].clone();

    m_properties.push_back(p);

    p->setParent(this);
  }
}

void te::dt::CompositeProperty::remove(Property* p)
{
  std::size_t size = m_properties.size();

  for(std::size_t i = 0; i < size; ++i)
  {
    if(p == m_properties[i])
    {
      delete p;
      m_properties.erase(m_properties.begin() + i);
      break;
    }
  }
}

te::dt::Property* te::dt::CompositeProperty::getProperty(const std::string& name) const
{
  const std::size_t size = m_properties.size();

  for(std::size_t i = 0; i < size; ++i)
    if(m_properties[i]->getName() == name)
      return m_properties[i];

  return 0;
}

std::size_t te::dt::CompositeProperty::getPropertyPosition(const std::string& name) const
{
  const std::size_t size = m_properties.size();

  for(std::size_t i = 0; i < size; ++i)
    if(m_properties[i]->getName() == name)
      return i;

  return std::string::npos;
}

std::size_t te::dt::CompositeProperty::getPropertyPosition(const Property* p) const
{
  const std::size_t size = m_properties.size();

  for(std::size_t i = 0; i < size; ++i)
    if(m_properties[i] == p)
      return i;

  return std::string::npos;
}

te::dt::Property* te::dt::CompositeProperty::getPropertyById(unsigned int id) const
{
  const std::size_t size = m_properties.size();

  for(std::size_t i = 0; i < size; ++i)
    if(m_properties[i]->getId() == id)
      return m_properties[i];

  return 0;
}

void te::dt::CompositeProperty::copy(const std::vector<Property*>& ps)
{
  std::size_t size = ps.size();

  m_properties.resize(m_properties.size() + size);

  for(std::size_t i = 0; i < size; ++i)
  {    
    Property* p = ps[i]->clone();

    m_properties[i] = p;
    
    p->setParent(this);
  }
}

void te::dt::CompositeProperty::clear()
{
  te::common::FreeContents(m_properties);
  m_properties.clear();
}

bool te::dt::CompositeProperty::has(Property* p) const
{
  size_t size = m_properties.size();

  for(size_t i = 0; i < size; ++i)
  {
    if(m_properties[i] == p)
      return true;

    if(m_properties[i]->has(p))
      return true;
  }

  return false;
}

te::dt::Property* te::dt::CompositeProperty::clone() const
{
  return new CompositeProperty(*this);
}

te::dt::CompositeProperty::CompositeProperty(const std::string& cname, const std::string& name, int t, unsigned int id, Property* parent)
  : Property(name, t, id, parent),
    m_cname(cname)
{
}


