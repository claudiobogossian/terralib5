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
  \file Notation.cpp

  \brief This class models a notation element from a XML Schema.
*/

// TerraLib
#include "Notation.h"

// STL
#include <cassert>

te::xsd::Notation::Notation(std::string* name, std::string* pub, Annotation* ann, std::string* id)
  : Identifiable(id),
    Annotated(ann),
    m_name(name),
    m_public(pub),
    m_system(0)
{
  assert(m_name);
  assert(m_public);
}

te::xsd::Notation::Notation(const Notation& rhs)
  : Identifiable(rhs),
    Annotated(rhs),
    m_name(0),
    m_public(0),
    m_system(0)
{
}

te::xsd::Notation::~Notation()
{
  delete m_name;
  delete m_public;
}

te::xsd::Notation& te::xsd::Notation::operator=(const Notation& rhs)
{
  return *this;
}

std::string* te::xsd::Notation::getName() const
{
  return m_name;
}

std::string* te::xsd::Notation::getPublic() const
{
  return m_public;
}

std::string* te::xsd::Notation::getSystem() const
{
  return m_system;
}

void te::xsd::Notation::setName(std::string* name)
{
  assert(name);
  delete m_name;
  m_name = name;
}

void te::xsd::Notation::setPublic(std::string* pub)
{
  assert(pub);
  delete m_public;
  m_public = pub;
}

void te::xsd::Notation::setSystem(std::string* sys)
{
  delete m_system;
  m_system = sys;
}
