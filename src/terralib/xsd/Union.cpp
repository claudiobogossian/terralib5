/*  Copyright (C) 2010-2012 National Institute For Space Research (INPE) - Brazil.

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

te::xsd::Union::Union(std::vector<QName*>* memberTypes, Annotation* ann, std::string* id)
  : SimpleTypeConstructor(ann, id),
    m_memberTypes(memberTypes),
    m_internalSimpleTypes(0)
{
}

te::xsd::Union::Union(const Union& rhs)
  : SimpleTypeConstructor(rhs),
    m_memberTypes(0),
    m_internalSimpleTypes(0)
{
  if(rhs.m_memberTypes)
  {
    m_memberTypes = new std::vector<QName*>;

    te::common::Copy<QName>(*rhs.m_memberTypes, *m_memberTypes);
  }

  if(rhs.m_internalSimpleTypes)
  {
    m_internalSimpleTypes = new std::vector<SimpleType*>;

    te::common::Clone<SimpleType>(*rhs.m_internalSimpleTypes, *m_internalSimpleTypes);
  }
}

te::xsd::Union::~Union()
{
  te::common::Free(m_memberTypes);

  te::common::Free(m_internalSimpleTypes);
}

te::xsd::Union& te::xsd::Union::operator=(const Union& rhs)
{
  if(this != &rhs)
  {
    SimpleTypeConstructor::operator=(rhs);

    te::common::Free(m_memberTypes);
    m_memberTypes = 0;

    if(rhs.m_memberTypes)
    {
      m_memberTypes = new std::vector<QName*>;

      te::common::Copy<QName>(*rhs.m_memberTypes, *m_memberTypes);
    }

    te::common::Free(m_internalSimpleTypes);
    m_internalSimpleTypes = 0;

    if(rhs.m_internalSimpleTypes)
    {
      m_internalSimpleTypes = new std::vector<SimpleType*>;

      te::common::Clone<SimpleType>(*rhs.m_internalSimpleTypes, *m_internalSimpleTypes);
    }
  }

  return *this;
}

std::vector<te::xsd::QName*>* te::xsd::Union::getMemberTypes() const
{
  return m_memberTypes;
}

std::vector<te::xsd::SimpleType*>* te::xsd::Union::getSimpleTypes() const
{
  return m_internalSimpleTypes;
}

void te::xsd::Union::addMemberType(te::xsd::QName* name)
{
  if(m_memberTypes == 0)
    m_memberTypes = new std::vector<te::xsd::QName*>;

  m_memberTypes->push_back(name);
}

void te::xsd::Union::addSimpleType(te::xsd::SimpleType* a)
{
  if(m_internalSimpleTypes == 0)
    m_internalSimpleTypes = new std::vector<te::xsd::SimpleType*>;

  m_internalSimpleTypes->push_back(a);
}

te::xsd::SimpleTypeConstructor* te::xsd::Union::clone() const
{
  return new Union(*this);
}


