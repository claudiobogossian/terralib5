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
  \file PropertyIsNull.cpp
  
  \brief The PropertyIsNull class encodes an operator that checks to see if the value of its content is NULL.
 */

// TerraLib
#include "Globals.h"
#include "PropertyIsNull.h"
#include "PropertyName.h"

// STL
#include <cassert>

te::fe::PropertyIsNull::PropertyIsNull()
  : ComparisonOp(Globals::sm_propertyIsNull),
    m_property(0)
{
}

te::fe::PropertyIsNull::~PropertyIsNull()
{
  delete m_property;
}

void te::fe::PropertyIsNull::setPropertyName(PropertyName* p)
{
  delete m_property;
  m_property = p;
}

te::fe::PropertyName* te::fe::PropertyIsNull::getPropertyName() const
{
  return m_property;
}

