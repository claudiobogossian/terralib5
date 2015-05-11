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
  \file Restriction4SimpleContent.h

  \brief This restriction class defines restrictions on a simpleContent.
*/

// TerraLib
#include "AbstractAttribute.h"
#include "AnyAttribute.h"
#include "Facet.h"
#include "Restriction4SimpleContent.h"
#include "SimpleType.h"
#include "QName.h"

// STL
#include <cassert>

te::xsd::Restriction4SimpleContent::Restriction4SimpleContent(std::string* id, te::xsd::QName* base)
  : Identifiable(id),
    Annotated(0),
    m_base(base),
    m_simpleType(0),
    m_anyAttr(0)
{
  assert(base);
}

te::xsd::Restriction4SimpleContent::~Restriction4SimpleContent()
{
  delete m_base;
  delete m_simpleType;
  delete m_anyAttr;
}

te::xsd::QName* te::xsd::Restriction4SimpleContent::getBase() const
{
  return m_base;
}

const boost::ptr_vector<te::xsd::Facet>& te::xsd::Restriction4SimpleContent::getFacets() const
{
  return m_facetVec;
}

const boost::ptr_vector<te::xsd::AbstractAttribute>& te::xsd::Restriction4SimpleContent::getAttributes() const
{
  return m_attributeVec;
}

te::xsd::AnyAttribute* te::xsd::Restriction4SimpleContent::getAnyAttribute() const
{
  return m_anyAttr;
}

void te::xsd::Restriction4SimpleContent::setBase(te::xsd::QName* base)
{
  delete m_base;
  m_base = base;
}

void te::xsd::Restriction4SimpleContent::setSimpleType(te::xsd::SimpleType* simpleType)
{
  delete m_simpleType;
  m_simpleType = simpleType;
}

void te::xsd::Restriction4SimpleContent::addFacet(te::xsd::FacetType fType, const std::string& value)
{
  te::xsd::Facet* f = new te::xsd::Facet(fType, value);
  addFacet(f);
}

void te::xsd::Restriction4SimpleContent::addFacet(te::xsd::Facet* f)
{
  m_facetVec.push_back(f);
}

void te::xsd::Restriction4SimpleContent::addAttribute(te::xsd::AbstractAttribute* a)
{
  m_attributeVec.push_back(a);
}

void te::xsd::Restriction4SimpleContent::setAnyAttribute(te::xsd::AnyAttribute* a)
{
  delete m_anyAttr;
  m_anyAttr = a;
}
