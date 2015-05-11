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
  \file SpatialOperators.cpp
  
  \brief Spatial operators.
 */

// TerraLib
#include "../common/STLUtils.h"
#include "SpatialOperator.h"
#include "SpatialOperators.h"

// STL
#include <cassert>

te::fe::SpatialOperators::SpatialOperators()
{
}

te::fe::SpatialOperators::~SpatialOperators()
{
  te::common::FreeContents(m_spatialOperators);
}

size_t te::fe::SpatialOperators::size() const
{
  return  m_spatialOperators.size();
}

void te::fe::SpatialOperators::push_back(SpatialOperator* sOp)
{
  assert(sOp);
  m_spatialOperators.push_back(sOp);
}

const te::fe::SpatialOperator* te::fe::SpatialOperators::operator[](size_t i) const
{
  assert(i < m_spatialOperators.size());
  return m_spatialOperators[i];
}