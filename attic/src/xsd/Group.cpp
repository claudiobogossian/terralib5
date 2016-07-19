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
  \file Group.cpp

  \brief This class models a group element in a XML Schema.
*/

// TerraLib
#include "Content.h"
#include "Group.h"
#include "QName.h"

te::xsd::Group::Group(unsigned int minOccurs, unsigned int maxOccurs, Annotation* ann, std::string* id)
  : Occurs(minOccurs, maxOccurs),
    Identifiable(id),
    Annotated(ann),
    m_name(0),
    m_ref(0),
    m_content(0)
{
}

te::xsd::Group::Group(const Group& rhs)
  : Occurs(rhs),
    Identifiable(rhs),
    Annotated(rhs),
    m_name(0),
    m_ref(0),
    m_content(0)
{
}

te::xsd::Group::~Group()
{
  delete m_name;
  delete m_ref;
  delete m_content;
}

te::xsd::Group& te::xsd::Group::operator=(const Group& rhs)
{
  return *this;
}

std::string* te::xsd::Group::getName() const
{
  return m_name;
}

te::xsd::QName* te::xsd::Group::getRef() const
{
  return m_ref;
}

te::xsd::Content* te::xsd::Group::getContent() const
{
  return m_content;
}

void te::xsd::Group::setName(std::string* name)
{
  delete m_name;
  m_name = name;

  delete m_ref;
  m_ref = 0;
}

void te::xsd::Group::setRef(te::xsd::QName* ref)
{
  delete m_ref;
  m_ref = ref;

  delete m_name;
  m_name = 0;
}

void te::xsd::Group::setContent(te::xsd::Content* c)
{
  delete m_content;
  m_content = c;
}

te::xsd::Content* te::xsd::Group::clone() const
{
  return new Group(*this);
}
