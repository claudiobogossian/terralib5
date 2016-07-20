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
  \file Union.cpp

  \brief It defines a simple type as a collection (union) of values from specified simple data types.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "QName.h"
#include "SimpleType.h"
#include "Union.h"

// STL
#include <cassert>

te::xsd::Union::Union(Annotation* ann, std::string* id)
  : SimpleTypeConstructor(ann, id)
{
}

te::xsd::Union::Union(const Union& rhs)
  : SimpleTypeConstructor(rhs)
{
  for(std::size_t i = 0; i < rhs.m_memberTypeVec.size(); ++i)
    m_memberTypeVec.push_back(new QName(rhs.m_memberTypeVec[i]));

  for(std::size_t i = 0; i < rhs.m_internalSimpleTypeVec.size(); ++i)
    m_internalSimpleTypeVec.push_back(static_cast<SimpleType*>(rhs.m_internalSimpleTypeVec[i].clone()));
}

te::xsd::Union::~Union()
{
}

te::xsd::Union& te::xsd::Union::operator=(const Union& rhs)
{
  if(this != &rhs)
  {
    SimpleTypeConstructor::operator=(rhs);

    for(std::size_t i = 0; i < rhs.m_memberTypeVec.size(); ++i)
      m_memberTypeVec.push_back(new QName(rhs.m_memberTypeVec[i]));

    for(std::size_t i = 0; i < rhs.m_internalSimpleTypeVec.size(); ++i)
      m_internalSimpleTypeVec.push_back(static_cast<SimpleType*>(rhs.m_internalSimpleTypeVec[i].clone()));
  }

  return *this;
}

const boost::ptr_vector<te::xsd::QName>& te::xsd::Union::getMemberTypes() const
{
  return m_memberTypeVec;
}

const boost::ptr_vector<te::xsd::SimpleType>& te::xsd::Union::getSimpleTypes() const
{
  return m_internalSimpleTypeVec;
}

void te::xsd::Union::addMemberType(te::xsd::QName* name)
{
  m_memberTypeVec.push_back(name);
}

void te::xsd::Union::addSimpleType(te::xsd::SimpleType* a)
{
  m_internalSimpleTypeVec.push_back(a);
}

te::xsd::SimpleTypeConstructor* te::xsd::Union::clone() const
{
  return new Union(*this);
}
