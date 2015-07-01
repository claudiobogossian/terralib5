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
  \file Filter.cpp
  
  \brief A Filter is any valid predicate expression.
 */

// TerraLib
#include "../common/STLUtils.h"
#include "Filter.h"

// STL
#include <cassert>

te::fe::Filter::Filter()
  : m_op(0),
    m_ids(0)
{
}

te::fe::Filter::~Filter()
{
  delete m_op;

  if(m_ids)
  {
    te::common::FreeContents(*m_ids);
    delete m_ids;
  }
}

void te::fe::Filter::setOp(AbstractOp* o)
{
  delete m_op;
  m_op = o;  
}

te::fe::AbstractOp* te::fe::Filter::getOp() const
{
  return m_op;
}

void te::fe::Filter::add(ObjectId* id)
{
  if(m_ids == 0)
    m_ids = new std::vector<ObjectId*>;

  m_ids->push_back(id);
}

te::fe::ObjectId* te::fe::Filter::operator[](size_t i) const
{
  assert(m_ids && (i < m_ids->size()));
  return (*m_ids)[i];
}

te::fe::ObjectId* te::fe::Filter::getOid(size_t i) const
{
  assert(m_ids && (i < m_ids->size()));
  return (*m_ids)[i];
}

size_t te::fe::Filter::getNumberOfOids() const
{
  return (m_ids ? m_ids->size() : 0);
}

void te::fe::Filter::clear()
{
  delete m_op;
  m_op = 0;

  if(m_ids)
  {
    te::common::FreeContents(*m_ids);
    delete m_ids;
    m_ids = 0;
  }
}

