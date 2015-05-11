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

// TerraLib
#include "Facet.h"
#include "Restriction4SimpleType.h"
#include "SimpleType.h"
#include "QName.h"

// STL
#include <cassert>

te::xsd::Restriction4SimpleType::Restriction4SimpleType(QName* base, Annotation* ann, std::string* id)
  : SimpleTypeConstructor(ann, id),
    m_base(base)
{
}

te::xsd::Restriction4SimpleType::Restriction4SimpleType(const Restriction4SimpleType& rhs)
  : SimpleTypeConstructor(rhs),
    m_base(0)
{
  m_base = rhs.m_base ? new QName(*rhs.m_base) : 0;
  //m_facets??
}

te::xsd::Restriction4SimpleType::~Restriction4SimpleType()
{
  delete m_base;
}

te::xsd::Restriction4SimpleType& te::xsd::Restriction4SimpleType::operator=(const Restriction4SimpleType& rhs)
{
  if(this != &rhs)
  {
    SimpleTypeConstructor::operator=(rhs);

    delete m_base;

    m_base = rhs.m_base ? new QName(*rhs.m_base) : 0;

    //m_facets??
  }

  return *this;
}

te::xsd::QName* te::xsd::Restriction4SimpleType::getBase() const
{
  return m_base;
}

const boost::ptr_vector<te::xsd::Facet>& te::xsd::Restriction4SimpleType::getFacets() const
{
  return m_facetVec;
}

void te::xsd::Restriction4SimpleType::setBase(te::xsd::QName* base)
{
  delete m_base;
  m_base = base;
}

void te::xsd::Restriction4SimpleType::setSimpleType(te::xsd::SimpleType* simpleType)
{
  delete m_simpleType;
  m_simpleType = simpleType;
}

void te::xsd::Restriction4SimpleType::addFacet(te::xsd::FacetType fType, const std::string& value)
{
  te::xsd::Facet* f = new te::xsd::Facet(fType, value);
  addFacet(f);
}

void te::xsd::Restriction4SimpleType::addFacet(te::xsd::Facet* f)
{
  m_facetVec.push_back(f);
}

te::xsd::SimpleTypeConstructor* te::xsd::Restriction4SimpleType::clone() const
{
  return new Restriction4SimpleType(*this);
}
