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
  \file Include.cpp

  \brief It models a XML Schema include.
 */

// TerraLib
#include "Include.h"

te::xsd::Include::Include(const std::string& schemaLocation, Annotation* ann, std::string* id)
  : Identifiable(id),
    Annotated(ann),
    m_schemaLocation(schemaLocation)
{
}

te::xsd::Include::Include(const Include& rhs)
  : Identifiable(rhs),
    Annotated(rhs),
    m_schemaLocation(rhs.m_schemaLocation)
{
}

te::xsd::Include::~Include()
{
}

te::xsd::Include& te::xsd::Include::operator=(const Include& rhs)
{
  return *this;
}

const std::string& te::xsd::Include::getSchemaLocation() const
{
  return m_schemaLocation;
}

void te::xsd::Include::setSchemaLocation(const std::string& schemaLocation)
{
  m_schemaLocation = schemaLocation;
}
