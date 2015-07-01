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
  \file terralib/datatype/CompositeData.cpp

  \brief A base class for composite data values.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "CompositeData.h"

// STL
#include <cassert>
#include <sstream>

te::dt::CompositeData::CompositeData(std::size_t nComponents)
  : m_values(nComponents),
    m_names(nComponents)
{
}

te::dt::CompositeData::CompositeData(const std::string& name)
  : m_name(name)
{
}

te::dt::CompositeData::CompositeData(const CompositeData& rhs)
  : m_names(rhs.m_names)
{
  te::common::Clone(rhs.m_values, m_values);
}

te::dt::CompositeData::~CompositeData()
{
  te::common::FreeContents(m_values);
}

te::dt::CompositeData& te::dt::CompositeData::operator=(const CompositeData& rhs)
{
  if(this != &rhs)
  {
    te::common::FreeContents(m_values);

    m_values.clear();

    te::common::Clone(rhs.m_values, m_values);

    m_names = rhs.m_names;
  }

  return *this;
}

void te::dt::CompositeData::setName(const std::string& name)
{
  m_name = name;
}

void te::dt::CompositeData::add(const std::string& name, AbstractData* value)
{
  m_names.push_back(name);
  m_values.push_back(value);
}

te::dt::AbstractData* te::dt::CompositeData::getValue(std::size_t i) const
{
  assert(i < m_values.size());
  return m_values[i];
}

void te::dt::CompositeData::setValue(std::size_t i, AbstractData* value)
{
  delete m_values[i];
  m_values[i] = value;
}

void te::dt::CompositeData::add(AbstractData* value)
{
  m_values.push_back(value);
}

te::dt::AbstractData* te::dt::CompositeData::clone() const
{
  return new CompositeData(*this);
}

std::string te::dt::CompositeData::toString() const
{
  std::stringstream ss(std::stringstream::in | std::stringstream::out);

  ss << "{\n";
  
  const std::size_t nComponents = m_values.size();

  for(std::size_t i = 0; i < nComponents; ++i)
  {
    if(i != 0)
      ss << ",\n";

    ss << m_values[i]->toString();
  }

  ss << "\n}";

  return ss.str();
}

