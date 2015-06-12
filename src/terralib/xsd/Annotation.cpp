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
  \file Annotation.cpp

  \brief A class that models a XSD annotation element.
*/

// TerraLib
#include "Annotation.h"
#include "AnnotationItem.h"
#include "Identifiable.h"

// STL
#include <cassert>

te::xsd::Annotation::Annotation(std::string* id)
  : Identifiable(id)/*,
    m_otherAttributes(0)*/
{
}

te::xsd::Annotation::Annotation(const Annotation& rhs)
  : Identifiable(rhs)
{
  for(std::size_t i = 0; i < rhs.m_itemVec.size(); ++i)
    m_itemVec.push_back(rhs.m_itemVec[i].clone());
}

te::xsd::Annotation::~Annotation()
{
}

te::xsd::Annotation& te::xsd::Annotation::operator=(const Annotation& rhs)
{
  if(this != &rhs)
  {
    Identifiable::operator=(rhs);

    for(std::size_t i = 0; i < rhs.m_itemVec.size(); ++i)
      m_itemVec.push_back(rhs.m_itemVec[i].clone());
  }

  return *this;
}

void te::xsd::Annotation::add(AnnotationItem* item)
{
  assert(item);

  m_itemVec.push_back(item);
}

const boost::ptr_vector<te::xsd::AnnotationItem>& te::xsd::Annotation::getItems() const
{
  return m_itemVec;
}

//std::map<std::string, std::string>* te::xsd::Annotation::getOtherAttributes() const
//{
//  return m_otherAttributes;
//}

//void te::xsd::Annotation::addOtherAttribute(std::string key, std::string value)
//{
//  if(m_otherAttributes == 0)
//    m_otherAttributes = new std::map<std::string, std::string>;
//
//  m_otherAttributes->insert(std::pair<std::string, std::string>(key, value));
//}
