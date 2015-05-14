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
  \file terralib/xsd/Schema.cpp

  \brief A class that models an XML Schema (XSD).
*/

// TerraLib
#include "Schema.h"

// STL
#include <cassert>

te::xsd::Schema::Schema(std::string* id)
  : Identifiable(id),
    m_attributeFormDefault(Unqualified),
    m_elementFormDefault(Unqualified),
    m_blockDefault(UNKNOWN),
    m_finalDefault(UNKNOWN)
{
}

te::xsd::Schema::~Schema()
{
}

const boost::ptr_vector<te::xsd::Include>& te::xsd::Schema::getIncludes() const
{
  return m_includeVec;
}

const boost::ptr_vector<te::xsd::Import>& te::xsd::Schema::getImports() const
{
  return m_importVec;
}

const boost::ptr_vector<te::xsd::Redefine>& te::xsd::Schema::getRedefines() const
{
  return m_redefineVec;
}

const boost::ptr_vector<te::xsd::Annotation>& te::xsd::Schema::getAnnotations() const
{
  return m_annotationVec;
}

const boost::ptr_vector<te::xsd::SimpleType>& te::xsd::Schema::getSimpleTypes() const
{
  return m_simpleTypeVec;
}

const boost::ptr_vector<te::xsd::ComplexType>& te::xsd::Schema::getComplexTypes() const
{
  return m_complexTypeVec;
}

const boost::ptr_vector<te::xsd::Group>& te::xsd::Schema::getGroups() const
{
  return m_groupVec;
}

const boost::ptr_vector<te::xsd::AttributeGroup>& te::xsd::Schema::getAttributeGroups() const
{
  return m_attributeGroupVec;
}

const boost::ptr_vector<te::xsd::AbstractAttribute>& te::xsd::Schema::getAttributes() const
{
  return m_attributeVec;
}

const boost::ptr_vector<te::xsd::Element>& te::xsd::Schema::getElements() const
{
  return m_elementVec;
}

const boost::ptr_vector<te::xsd::Notation>& te::xsd::Schema::getNotations() const
{
  return m_notationVec;
}

void te::xsd::Schema::addNamespace(const std::string& ns, const std::string& nsprefix)
{
  m_xmlns.insert(boost::bimap<std::string, std::string>::value_type(nsprefix, ns));
}

void te::xsd::Schema::addInclude(Include* i)
{
  m_includeVec.push_back(i);
}

void te::xsd::Schema::addImport(te::xsd::Import* i)
{
  m_importVec.push_back(i);
}

void te::xsd::Schema::addRedefine(te::xsd::Redefine* r)
{
  m_redefineVec.push_back(r);
}

void te::xsd::Schema::addAnnotation(te::xsd::Annotation* a)
{
  m_annotationVec.push_back(a);
}

void te::xsd::Schema::addSimpleType(te::xsd::SimpleType* s)
{
  m_simpleTypeVec.push_back(s);
}

void te::xsd::Schema::addComplexType(te::xsd::ComplexType* c)
{
  m_complexTypeVec.push_back(c);
}

void te::xsd::Schema::addGroup(Group* g)
{
  m_groupVec.push_back(g);
}

void te::xsd::Schema::addAttributeGroup(AttributeGroup* ag)
{
  m_attributeGroupVec.push_back(ag);
}

void te::xsd::Schema::addAttribute(AbstractAttribute* a)
{
  m_attributeVec.push_back(a);
}

void te::xsd::Schema::addElement(Element* e)
{
  m_elementVec.push_back(e);
}

void te::xsd::Schema::addNotation(Notation* n)
{
  m_notationVec.push_back(n);
}
