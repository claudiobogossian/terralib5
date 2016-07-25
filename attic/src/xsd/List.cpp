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
#include "List.h"
#include "QName.h"
#include "SimpleType.h"

te::xsd::List::List(Annotation* ann, std::string* id)
  : SimpleTypeConstructor(ann, id),
    m_itemType(0),
    m_internalSimpleType(0)
{
}

te::xsd::List::List(const List& rhs)
  : SimpleTypeConstructor(rhs)
{
  m_itemType = rhs.m_itemType ? new QName(*rhs.m_itemType) : 0;
  m_internalSimpleType = rhs.m_internalSimpleType ? static_cast<SimpleType*>(rhs.m_internalSimpleType->clone()) : 0;
}

te::xsd::List::~List()
{
  delete m_itemType;
  delete m_internalSimpleType;
}

te::xsd::List& te::xsd::List::operator=(const List& rhs)
{
  if(this != &rhs)
  {
    SimpleTypeConstructor::operator=(rhs);

    delete m_itemType;

    m_itemType = rhs.m_itemType ? new QName(*rhs.m_itemType) : 0;
  
    delete m_internalSimpleType;

    m_internalSimpleType = rhs.m_internalSimpleType ? static_cast<SimpleType*>(rhs.m_internalSimpleType->clone()) : 0;
  }

  return *this;
}

te::xsd::QName* te::xsd::List::getItemType() const
{
  return m_itemType;
}

te::xsd::SimpleType* te::xsd::List::getSimpleType() const
{
  return m_internalSimpleType;
}

void te::xsd::List::setItemType(te::xsd::QName* name)
{
  delete m_internalSimpleType;
  m_internalSimpleType = 0;
  
  delete m_itemType;
  m_itemType = name;
}

void te::xsd::List::setSimpleType(SimpleType* a)
{
   delete m_itemType;
   m_itemType = 0;
   
   delete m_internalSimpleType;
   m_internalSimpleType = a;
}

te::xsd::SimpleTypeConstructor* te::xsd::List::clone() const
{
  return new List(*this);
}
