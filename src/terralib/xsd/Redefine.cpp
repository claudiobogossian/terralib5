/*  Copyright (C) 2010-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file Redefine.cpp

  \brief It models a XML Schema redefine.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "AttributeGroup.h"
#include "Group.h"
#include "Redefine.h"
#include "Type.h"

te::xsd::Redefine::Redefine(const std::string& schemaLocation, Annotation* ann, std::string* id)
  : Identifiable(id),
    Annotated(ann),
    m_schemaLocation(schemaLocation),
    m_types(0),
    m_groups(0),
    m_attributeGroups(0)
{
}

te::xsd::Redefine::Redefine(const Redefine& rhs)
  : Identifiable(rhs),
    Annotated(rhs),
    m_schemaLocation(""),
    m_types(0),
    m_groups(0),
    m_attributeGroups(0)
{
}

te::xsd::Redefine::~Redefine()
{
  te::common::Free(m_types);

  te::common::Free(m_groups);

  te::common::Free(m_attributeGroups);
}

te::xsd::Redefine& te::xsd::Redefine::operator=(const Redefine& rhs)
{
  return *this;
}

const std::string& te::xsd::Redefine::getSchemaLocation() const
{
  return m_schemaLocation;
}

std::vector<te::xsd::Type*>* te::xsd::Redefine::getTypes() const
{
  return m_types;
}

std::vector<te::xsd::Group*>* te::xsd::Redefine::getGroups() const
{
  return m_groups;
}
        
std::vector<te::xsd::AttributeGroup*>* te::xsd::Redefine::getAttributeGroups() const
{
  return m_attributeGroups;
}


