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
  \file terralib/datatype/Array.cpp

  \brief Class for dealing with arrays of abstract data.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "Array.h"

// STL
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

te::dt::Array::Array(std::size_t d, int t)
  : m_dimension(d),
    m_type(t)
{
  for (std::size_t i = 0; i < m_dimension; i++)
    m_dimensionSizes.push_back(0);
}

te::dt::Array::Array(const Array& rhs)
  : m_dimension(rhs.m_dimension),
    m_dimensionSizes(rhs.m_dimensionSizes),
    m_type(rhs.m_type)
{
  std::map<std::vector<std::size_t>, te::dt::AbstractData*>::const_iterator it = rhs.m_data.begin();

  while (it != rhs.m_data.end())
  {
    m_data[it->first] = it->second->clone();

    ++it;
  }
}

te::dt::Array& te::dt::Array::operator=(const Array& rhs)
{
  if(this != &rhs)
  {
    te::common::FreeContents(m_data);

    m_data.clear();

    m_dimension = rhs.m_dimension;

    m_dimensionSizes = rhs.m_dimensionSizes;

    std::map<std::vector<std::size_t>, te::dt::AbstractData*>::const_iterator it = rhs.m_data.begin();

    while (it != rhs.m_data.end())
    {
      m_data[it->first] = it->second->clone();

      ++it;
    }

    m_type = rhs.m_type;
  }

  return *this;
}

te::dt::Array::~Array()
{
  te::common::FreeContents(m_data);
}
    
std::size_t te::dt::Array::getDimension() const
{
  return m_dimension;
}

int te::dt::Array::getElementsTypeCode()
{
  return m_type;
}

std::size_t te::dt::Array::getDimensionSize(std::size_t i) const
{
  assert(i < getDimension());
  
  return m_dimensionSizes[i];
}

void te::dt::Array::insert(te::dt::AbstractData* data, const std::vector<std::size_t>& pos)
{
  assert(pos.size() == getDimension());

// check if pos is out of current bounds, and expand the array dimension info if needed
  for (std::size_t i = 0; i < getDimension(); i++)
    m_dimensionSizes[i] = std::max(m_dimensionSizes[i], pos[i] + 1);

// remove previously data on the position
  delete m_data[pos];

  m_data[pos] = data;
}

te::dt::AbstractData* te::dt::Array::getData(const std::vector<std::size_t>& i)
{
  assert(i.size() == getDimension());

  return te::common::GetPValue(m_data, i);
}

te::dt::AbstractData& te::dt::Array::operator[](const std::vector<std::size_t>& i)
{
  assert(i.size() == getDimension());

  return *te::common::GetPValue(m_data, i);
}

te::dt::AbstractData* te::dt::Array::clone() const
{
  return new Array(*this);
}

std::string te::dt::Array::toString() const
{
  std::ostringstream output;
  
  std::size_t total_size = 1;

  std::vector<std::size_t> products_back (getDimension(), 1);

  std::vector<std::size_t> products_front (getDimension(), 1);
  
  for (std::size_t i = 0; i < getDimension(); i++)
  {
    total_size *= getDimensionSize(i);
    
    for (std::size_t f = i + 1; f < getDimension(); f++)
      products_front[i] *= getDimensionSize(f);
      
    for (std::size_t b = 0; b < i; b++)
      products_back[i] *= getDimensionSize(b);
  }
  
  std::vector<std::vector<std::size_t> > poses(total_size, std::vector<std::size_t>(getDimension(), 0));
 
  for (std::size_t ii = 0; ii < getDimension(); ii++)
  {
    std::size_t i = getDimension() - ii - 1;

    std::size_t pi = 0;
    for (std::size_t j = 0; j < products_back[i]; j++)
      for (std::size_t k = 0; k < getDimensionSize(i); k++)
        for (std::size_t l = 0; l < products_front[i]; l++)
        {
          poses[pi++][i] = k;
        }
  }

  for (std::size_t i = 0; i < poses.size(); i++)
  {
    std::map<std::vector<std::size_t>, te::dt::AbstractData*>::const_iterator it = m_data.find(poses[i]);
    
    if(it != m_data.end())
      output << it->second->toString() << " ";
  }
  output << std::endl;

  return output.str();
}

