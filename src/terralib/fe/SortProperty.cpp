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
  \file SortProperty.cpp
  
  \brief A sort property name and order type.
 */

// TerraLib
#include "PropertyName.h"
#include "SortProperty.h"

te::fe::SortProperty::SortProperty()
  : m_property(0),
    m_sortOrder(SortProperty::ASC)
{
}

te::fe::SortProperty::~SortProperty()
{
  delete m_property;
}

void te::fe::SortProperty::setPropertyName(PropertyName* p)
{
  delete m_property;
  m_property = p;
}

const te::fe::PropertyName* te::fe::SortProperty::getPropertyName() const
{
  return m_property;
}

void te::fe::SortProperty::setSortOrder(SortProperty::SortOrder direction)
{
  m_sortOrder = direction;
}

te::fe::SortProperty::SortOrder te::fe::SortProperty::getSortOrder() const
{
  return m_sortOrder;
}

