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
  \file PropertyName.cpp
  
  \brief This class can be used to is used to encode the name of any property of an object.
 */

// TerraLib
#include "PropertyName.h"

// STL
#include <cassert>

te::fe::PropertyName::PropertyName(const std::string& name)
  : m_name(name)
{
}

te::fe::PropertyName::~PropertyName()
{
}

void te::fe::PropertyName::setName(const std::string& name)
{
  m_name = name;
}

const std::string& te::fe::PropertyName::getName() const
{
  return m_name;
}

te::fe::Expression* te::fe::PropertyName::clone() const
{
  return new PropertyName(m_name);
}
