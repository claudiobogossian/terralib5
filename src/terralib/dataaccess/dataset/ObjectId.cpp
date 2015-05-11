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
  \file terralib/dataaccess/dataset/ObjectId.cpp

  \brief This class represents an unique id for a data set element.
*/

// TerraLib
#include "ObjectId.h"

// STL
#include <cassert>

te::da::ObjectId::ObjectId()
{
}

te::da::ObjectId::ObjectId(const ObjectId& rhs)
{
  for(std::size_t i = 0; i < rhs.m_data.size(); ++i)
    m_data.push_back(rhs.m_data[i].clone());
}

te::da::ObjectId::~ObjectId()
{
}

const boost::ptr_vector<te::dt::AbstractData>& te::da::ObjectId::getValue() const
{
  return m_data;
}

std::string te::da::ObjectId::getValueAsString() const
{
  std::string value;

  for(std::size_t i = 0; i < m_data.size(); ++i)
    value += m_data[i].toString();
  
  return value;
}

void te::da::ObjectId::addValue(te::dt::AbstractData* data)
{
  assert(data);

  m_data.push_back(data);
}

te::da::ObjectId* te::da::ObjectId::clone() const
{
  return new ObjectId(*this);
}

bool te::da::ObjectId::operator<(const ObjectId& rhs) const
{
  return getValueAsString() < rhs.getValueAsString();
}
