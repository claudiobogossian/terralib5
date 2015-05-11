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
  \file BinaryLogicOp.cpp
  
  \brief A logical operator can be used to combine two or more conditional expressions.
 */

// TerraLib
#include "../common/STLUtils.h"
#include "AbstractOp.h"
#include "BinaryLogicOp.h"
#include "Globals.h"

// STL
#include <cassert>
#include <cstring>

te::fe::BinaryLogicOp::BinaryLogicOp(const char* opName)
  : LogicOp(opName)
{
}

te::fe::BinaryLogicOp::BinaryLogicOp(const char* opName,
                                     AbstractOp* first,
                                     AbstractOp* second)
  : LogicOp(opName)
{
  m_ops.push_back(first);
  m_ops.push_back(second);
}

te::fe::BinaryLogicOp::~BinaryLogicOp()
{
  te::common::FreeContents(m_ops);
}

size_t te::fe::BinaryLogicOp::size() const
{
  return m_ops.size();
}

void te::fe::BinaryLogicOp::add(AbstractOp* o)
{
  assert(o);
  m_ops.push_back(o);
}

te::fe::AbstractOp* te::fe::BinaryLogicOp::operator[](size_t i) const
{
  assert(i < m_ops.size());
  return m_ops[i];
}

te::fe::AbstractOp* te::fe::BinaryLogicOp::getOp(size_t i) const
{
  assert(i < m_ops.size());
  return m_ops[i];
}

