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

//std::vector<te::xsd::Include*>* te::xsd::Schema::getIncludes() const
//{
//  return m_includes;
//}
//
//std::vector<te::xsd::Import*>* te::xsd::Schema::getImports() const
//{
//  return m_imports;
//}
//
//std::vector<te::xsd::Redefine*>* te::xsd::Schema::getRedefines() const
//{
//  return m_redefines;
//}
//
//std::vector<te::xsd::SimpleType*>* te::xsd::Schema::getSimpleTypes() const
//{
//  return m_simpleTypes;
//}
//
//std::vector<te::xsd::ComplexType*>* te::xsd::Schema::getComplexTypes() const
//{
//  return m_complexTypes;
//}
//
//std::vector<te::xsd::Group*>* te::xsd::Schema::getGroups() const
//{
//  return m_groups;
//}
//
//std::vector<te::xsd::AbstractAttribute*>* te::xsd::Schema::getAttributes() const
//{
//  return m_attributes;
//}
//
//std::vector<te::xsd::Element*>* te::xsd::Schema::getElements() const
//{
//  return m_elements;
//}
//
//std::vector<te::xsd::Notation*>* te::xsd::Schema::getNotations() const
//{
//  return m_notations;
//}
//
//void te::xsd::Schema::setAttributeFormDefault(Form f)
//{
//  m_attributeFormDefault = f;
//}
//
//void te::xsd::Schema::setElementFormDefault(Form f)
//{
//  m_elementFormDefault = f;
//}
//
//void te::xsd::Schema::setBlockDefault(int v)
//{
//  m_blockDefault = v;
//}
//
//void te::xsd::Schema::setFinalDefault(int v)
//{
//  m_finalDefault = v;
//}
//
//void te::xsd::Schema::setTargetNamespace(std::string* ns)
//{
//  delete m_targetNamespace;
//  m_targetNamespace = ns;
//}
//
//void te::xsd::Schema::setTargetNamespacePrefix(std::string* nsprefix)
//{
//  delete m_targetNamespacePrefix;
//  m_targetNamespacePrefix = nsprefix;
//}
//
//void te::xsd::Schema::setVersion(std::string* v)
//{
//  delete m_version;
//  m_version = v;
//}
//
//void te::xsd::Schema::addNamespace(const std::string& ns, const std::string& nsprefix)
//{
//  if(m_xmlns == 0)
//    m_xmlns = new std::map<std::string, std::string>;
//
//  m_xmlns->insert(std::pair<std::string, std::string>(nsprefix, ns));
//}
//
//void te::xsd::Schema::addInclude(Include* i)
//{
//  if(m_includes == 0)
//    m_includes = new std::vector<te::xsd::Include*>;
//
//  m_includes->push_back(i);
//}
//
//void te::xsd::Schema::addImport(te::xsd::Import* i)
//{
//  if(m_imports == 0)
//    m_imports = new std::vector<te::xsd::Import*>;
//
//  m_imports->push_back(i);
//}
//
//void te::xsd::Schema::addRedefine(te::xsd::Redefine* r)
//{
//  if(m_redefines == 0)
//    m_redefines = new std::vector<te::xsd::Redefine*>;
//
//  m_redefines->push_back(r);
//}
//
//void te::xsd::Schema::addSimpleType(te::xsd::SimpleType* s)
//{
//  if(m_simpleTypes == 0)
//    m_simpleTypes = new std::vector<te::xsd::SimpleType*>;
//
//   m_simpleTypes->push_back(s);
//}
//
//void te::xsd::Schema::addComplexType(te::xsd::ComplexType* c)
//{
//  if(m_complexTypes == 0)
//    m_complexTypes = new std::vector<te::xsd::ComplexType*>;
//
//  m_complexTypes->push_back(c);
//}
//
//void te::xsd::Schema::addGroup(Group* g)
//{
//  if(m_groups == 0)
//    m_groups = new std::vector<te::xsd::Group*>;
//
//  m_groups->push_back(g);
//}
//
//void te::xsd::Schema::addAttribute(AbstractAttribute* a)
//{
//  if(m_attributes == 0)
//    m_attributes = new std::vector<te::xsd::AbstractAttribute*>;
//
//  m_attributes->push_back(a);
//}
//
//void te::xsd::Schema::addElement(Element* e)
//{
//  if(m_elements == 0)
//    m_elements = new std::vector<te::xsd::Element*>;
//
//  m_elements->push_back(e);
//}
//
//void te::xsd::Schema::addNotation(Notation* n)
//{
//  if(m_notations == 0)
//    m_notations = new std::vector<te::xsd::Notation*>;
//
//  m_notations->push_back(n);
//}



