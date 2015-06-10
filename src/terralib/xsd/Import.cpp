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
  \file Import.cpp

  \brief It models a XML Schema import.
 */

// TerraLib
#include "Import.h"

te::xsd::Import::Import(std::string* ns, std::string* schemaLocation, Annotation* ann, std::string* id)
  : Identifiable(id),
    Annotated(ann),
    m_namespace(ns),
    m_schemaLocation(schemaLocation)
{
}

te::xsd::Import::Import(const Import& rhs)
  : Identifiable(rhs),
    Annotated(rhs),
    m_namespace(0),
    m_schemaLocation(0)
{
}

te::xsd::Import::~Import()
{
  delete m_namespace;
  delete m_schemaLocation;
}

te::xsd::Import& te::xsd::Import::operator=(const Import& rhs)
{
  return *this;
}

std::string* te::xsd::Import::getNamespace() const
{
  return m_namespace;
}

std::string* te::xsd::Import::getSchemaLocation() const
{
  return m_schemaLocation;
}

void te::xsd::Import::setNamespace(std::string* ns)
{
  delete m_namespace;
  m_namespace = ns;
}

void te::xsd::Import::setSchemaLocation(std::string* schemaLocation)
{
  delete m_schemaLocation;
  m_schemaLocation = schemaLocation;
}
