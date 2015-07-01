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
  \file AnyAttribute.cpp

  \brief It models an XML anyAttribute element.
*/

// TerraLib
#include "AnyAttribute.h"

te::xsd::AnyAttribute::AnyAttribute(Annotation* ann, std::string* id)
  : Identifiable(id),
    Annotated(ann),
    m_namespace(0),
    m_pContents(STRICT)
{
}

te::xsd::AnyAttribute::AnyAttribute(const AnyAttribute& rhs)
  : Identifiable(rhs),
    Annotated(rhs),
    m_namespace(0),
    m_pContents(rhs.m_pContents)
{
  m_namespace = rhs.m_namespace ? new std::string(*rhs.m_namespace) : 0;
}

te::xsd::AnyAttribute::~AnyAttribute()
{
  delete m_namespace;
}

te::xsd::AnyAttribute& te::xsd::AnyAttribute::operator=(const AnyAttribute& rhs)
{
  if(this != &rhs)
  {
    Identifiable::operator=(rhs);
    Annotated::operator=(rhs);

    delete m_namespace;

    m_namespace = rhs.m_namespace ? new std::string(*rhs.m_namespace) : 0;

    m_pContents = rhs.m_pContents;
  }

  return *this;
}

std::string* te::xsd::AnyAttribute::getNamespace() const
{
  return m_namespace;
}

te::xsd::ProcessContents te::xsd::AnyAttribute::getProcessContent() const
{
  return m_pContents;
}

void te::xsd::AnyAttribute::setNamespace(std::string* ns)
{
  delete m_namespace;
  m_namespace = ns;
}

void te::xsd::AnyAttribute::setProcessContents(te::xsd::ProcessContents pc)
{
  m_pContents = pc;
}


