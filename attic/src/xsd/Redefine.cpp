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
  \file Redefine.cpp

  \brief It models a XML Schema redefine.
*/

// TerraLib
#include "Annotation.h"
#include "AttributeGroup.h"
#include "Group.h"
#include "Redefine.h"
#include "Type.h"

te::xsd::Redefine::Redefine(const std::string& schemaLocation, std::string* id)
  : Identifiable(id),
    m_schemaLocation(schemaLocation)
{
}

te::xsd::Redefine::Redefine(const Redefine& rhs)
  : Identifiable(rhs),
    m_schemaLocation("")
{
}

te::xsd::Redefine::~Redefine()
{
}

te::xsd::Redefine& te::xsd::Redefine::operator=(const Redefine& rhs)
{
  return *this;
}

const std::string& te::xsd::Redefine::getSchemaLocation() const
{
  return m_schemaLocation;
}

const boost::ptr_vector<te::xsd::Annotation>& te::xsd::Redefine::getAnnotations() const
{
  return m_annotationVec;
}

const boost::ptr_vector<te::xsd::Type>& te::xsd::Redefine::getTypes() const
{
  return m_typeVec;
}

const boost::ptr_vector<te::xsd::Group>& te::xsd::Redefine::getGroups() const
{
  return m_groupVec;
}

const boost::ptr_vector<te::xsd::AttributeGroup>& te::xsd::Redefine::getAttributeGroups() const
{
  return m_attributeGroupVec;
}

void te::xsd::Redefine::setSchemaLocation(const std::string& schemaLocation)
{
  m_schemaLocation = schemaLocation;
}

void te::xsd::Redefine::addAnnotation(Annotation* ann)
{
  m_annotationVec.push_back(ann);
}

void te::xsd::Redefine::addType(Type* t)
{
  m_typeVec.push_back(t);
}

void te::xsd::Redefine::addGroup(Group* g)
{
  m_groupVec.push_back(g);
}

void te::xsd::Redefine::addAttributeGroup(AttributeGroup* ag)
{
  m_attributeGroupVec.push_back(ag);
}
