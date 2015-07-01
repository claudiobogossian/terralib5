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
  \file terralib/dataaccess/query/PropertyName.cpp

  \brief A class that models the name of any property of an object.
*/

// TerraLib
#include "PropertyName.h"

te::da::PropertyName::PropertyName(const PropertyName& rhs)
  : m_name(rhs.m_name)
{
}

te::da::PropertyName& te::da::PropertyName::operator=(const PropertyName& rhs)
{
  if(this != &rhs)
    m_name = rhs.m_name;

  return *this;
}

te::da::Expression* te::da::PropertyName::clone() const
{
  return new PropertyName(*this);
}

