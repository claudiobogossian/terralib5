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
  \file terralib/common/Distance.cpp

  \brief A given distance has a measurement and a unit-of-measure.
*/

// TerraLib
#include "Distance.h"

te::common::Distance::Distance(const double& value,  size_t uomIdx)
  : m_value(value),
    m_uom(uomIdx)
{
}

te::common::Distance::~Distance()
{
}

void te::common::Distance::setDistance(const double& value)
{
  m_value = value;
}

const double& te::common::Distance::getDistance() const
{
  return m_value;
}

void te::common::Distance::setUom(size_t uom)
{
  m_uom = uom;
}

size_t te::common::Distance::getUom() const
{
  return m_uom;
}

