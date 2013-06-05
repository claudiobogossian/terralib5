/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/core/dataset/UniqueKey.cpp

  \brief It describes an unique key (uk) constraint.
*/

// TerraLib
#include "UniqueKey.h"

te::da::core::UniqueKey::UniqueKey(const std::string& name)
  : Constraint(name)
{ 
}

te::da::core::UniqueKey::UniqueKey(const UniqueKey& rhs)
  : Constraint(rhs.getName()),
    m_properties(rhs.m_properties)
{
}

te::da::core::UniqueKey& te::da::core::UniqueKey::operator=(const UniqueKey& rhs)
{
  if(this != &rhs)
  {
    Constraint::operator=(rhs);
    m_properties = rhs.m_properties;
  }

  return *this;
}

te::da::core::Constraint* te::da::core::UniqueKey::clone()
{
  return new UniqueKey(*this);
}
