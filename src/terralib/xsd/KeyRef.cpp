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
  \file KeyRef.cpp

  \brief This class models a keyref element from a XML Schema element.
*/

// TerraLib
#include "KeyRef.h"
#include "QName.h"

// STL
#include <cassert>

te::xsd::KeyRef::KeyRef(QName* refer, std::string* name, Annotation* ann, std::string* id)
  : IdentityConstraint(name, ann, id),
    m_ref(refer)
{
  assert(m_ref);
}

te::xsd::KeyRef::KeyRef(const KeyRef& rhs)
  : IdentityConstraint(rhs)
{
}

te::xsd::KeyRef::~KeyRef()
{
  delete m_ref;
}

te::xsd::KeyRef& te::xsd::KeyRef::operator=(const KeyRef& rhs)
{
  return *this;
}

te::xsd::QName* te::xsd::KeyRef::getRef() const
{
  return m_ref;
}

void te::xsd::KeyRef::setRef(QName* ref)
{
  assert(ref);
  delete m_ref;
  m_ref = ref;
}

te::xsd::IdentityConstraint* te::xsd::KeyRef::clone() const
{
  return new KeyRef(*this);
}
