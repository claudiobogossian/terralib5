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
  \file SortBy.cpp
  
  \brief It represents a sort by expression.  
 */

// TerraLib
#include "../common/STLUtils.h"
#include "SortBy.h"
#include "SortProperty.h"

// STL
#include <cassert>

te::fe::SortBy::SortBy()
{
}

te::fe::SortBy::~SortBy()
{
  te::common::FreeContents(m_sortPropertyVector);
}

std::size_t te::fe::SortBy:: size() const
{
  return m_sortPropertyVector.size();
}

void te::fe::SortBy::push_back(SortProperty* p)
{
  m_sortPropertyVector.push_back(p);
}

const te::fe::SortProperty* te::fe::SortBy::operator[](std::size_t i) const
{
  assert(i < m_sortPropertyVector.size());
  return m_sortPropertyVector[i];
}


