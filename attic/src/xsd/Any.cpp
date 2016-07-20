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
  \file Any.cpp

  \brief It models the XML Schema any element.
*/

// TerraLib
#include "Any.h"

te::xsd::Any::Any(Annotation* ann, std::string* id)
  : Identifiable(id),
    Annotated(ann),
    m_namespace(0),
    m_pContents(STRICT)
{
}

te::xsd::Any::Any(const Any& rhs)
  : Occurs(rhs),
    Identifiable(rhs),
    Annotated(rhs),
    m_namespace(0),
    m_pContents(rhs.m_pContents)
{
  m_namespace = rhs.m_pContents ? new std::string(*rhs.m_namespace) : 0;
}

te::xsd::Any::~Any()
{
  delete m_namespace;
}

te::xsd::Any& te::xsd::Any::operator=(const Any& rhs)
{
  if(this != &rhs)
  {
    Occurs::operator=(rhs);

    Identifiable::operator=(rhs);

    Annotated::operator=(rhs);

    delete m_namespace;

    m_namespace = rhs.m_pContents ? new std::string(*rhs.m_namespace) : 0;

    m_pContents = rhs.m_pContents;
  }

  return *this;
}

std::string* te::xsd::Any::getNamespace() const
{
  return m_namespace;
}

te::xsd::ProcessContents te::xsd::Any::getProcessContent() const
{
  return m_pContents;
}

void te::xsd::Any::setNamespace(std::string* ns)
{
  delete m_namespace;
  m_namespace = ns;
}

void te::xsd::Any::setProcessContents(te::xsd::ProcessContents pc)
{
  m_pContents = pc;
}




