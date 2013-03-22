/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
#include "../../common/STLUtils.h"
#include "../../datatype/AbstractData.h"
#include "ObjectId.h"

// STL
#include <cassert>

te::da::ObjectId::ObjectId()
{
}

te::da::ObjectId::~ObjectId()
{
  te::common::FreeContents(m_data);
}

const std::map<std::size_t, te::dt::AbstractData*>& te::da::ObjectId::getValue() const
{
  return m_data;
}

std::string te::da::ObjectId::getValueAsString() const
{
  std::string value;

  for(std::map<std::size_t, te::dt::AbstractData*>::const_iterator it = m_data.begin(); it != m_data.end(); ++it)
    value = it->second->toString();
  
  return value;
}

void te::da::ObjectId::addValue(const std::size_t& index, te::dt::AbstractData* data)
{
  assert(data);

  std::map<std::size_t, te::dt::AbstractData*>::iterator it = m_data.find(index);
  if(it != m_data.end())
    delete it->second;

  m_data[index] = data;
}
