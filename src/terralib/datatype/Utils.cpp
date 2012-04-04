/*  Copyright (C) 2010-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/datatype/Utils.cpp

  \brief Utilitary function for data type module.
*/

// TerraLib
#include "CompositeProperty.h"
#include "Property.h"
#include "Utils.h"

void te::dt::GetPropertiesPosition(const std::vector<Property*>& properties,
                                   const CompositeProperty* cp,
                                   std::vector<std::size_t>& poslist)
{
  const std::size_t size = properties.size();

  for(std::size_t i = 0; i < size; ++i)
    poslist.push_back(cp->getPropertyPosition(properties[i]));
}

