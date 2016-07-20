/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file Key.cpp

  \brief This class models a key element from an XML Schema.
*/

// TerraLib
#include "Key.h"

te::xsd::Key::Key(std::string* name, Annotation* ann, std::string* id)
  : IdentityConstraint(name, ann, id)
{
}

te::xsd::Key::Key(const Key& rhs)
  : IdentityConstraint(rhs)
{
}

te::xsd::Key::~Key()
{
  delete m_name;
}

te::xsd::Key& te::xsd::Key::operator=(const Key& rhs)
{
  return *this;
}

te::xsd::IdentityConstraint* te::xsd::Key::clone() const
{
  return new Key(*this);
}

std::string* te::xsd::Key::getName() const
{
  return m_name;
}

void te::xsd::Key::setName(std::string* name)
{
  delete m_name;
  m_name = name;
}
