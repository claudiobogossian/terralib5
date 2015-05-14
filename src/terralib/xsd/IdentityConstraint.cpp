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
  \file IdentityConstraint.cpp

  \brief This is the base class for XML Schema elements that are related to identity constraint.
*/

// TerraLib
#include "Field.h"
#include "IdentityConstraint.h"
#include "Selector.h"

// STL
#include <cassert>

te::xsd::IdentityConstraint::IdentityConstraint(std::string* name, Annotation* ann, std::string* id)
  : Identifiable(id),
    Annotated(ann),
    m_name(name)
{
  assert(m_name);
}

te::xsd::IdentityConstraint::IdentityConstraint(const IdentityConstraint& rhs)
{
}

te::xsd::IdentityConstraint::~IdentityConstraint()
{
  delete m_name;
  delete m_selector;
}

te::xsd::IdentityConstraint& te::xsd::IdentityConstraint::operator=(const IdentityConstraint& rhs)
{
  return *this;
}

std::string* te::xsd::IdentityConstraint::getName() const
{
  return m_name;
}

te::xsd::Selector* te::xsd::IdentityConstraint::getSelector() const
{
  return m_selector;
}

const boost::ptr_vector<te::xsd::Field>& te::xsd::IdentityConstraint::getFields() const
{
  return m_fieldVec;
}

void te::xsd::IdentityConstraint::setName(std::string* name)
{
  assert(name);
  delete m_name;
  m_name = name;
}

void te::xsd::IdentityConstraint::setSelector(te::xsd::Selector* s)
{
  delete m_selector;
  m_selector = s;
}

void te::xsd::IdentityConstraint::addField(te::xsd::Field* f)
{
  m_fieldVec.push_back(f);
}
