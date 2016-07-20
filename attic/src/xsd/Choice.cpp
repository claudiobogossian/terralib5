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
  \file Choice.cpp

  \brief This class models the XML Schema choice element.
*/

// TerraLib
#include "Any.h"
#include "Choice.h"
#include "Element.h"

te::xsd::Choice::Choice(unsigned int minOccurs, unsigned int maxOccurs, Annotation* ann, std::string* id)
  : Occurs(minOccurs, maxOccurs), 
    Identifiable(id),
    Annotated(ann)
{
}

te::xsd::Choice::Choice(const Choice& rhs)
  : Occurs(rhs), 
    Identifiable(rhs),
    Annotated(rhs)
{
  for(std::size_t i = 0; i < rhs.m_elementVec.size(); ++i)
    m_elementVec.push_back(new Element(rhs.m_elementVec[i]));

  for(std::size_t i = 0; i < rhs.m_contentVec.size(); ++i)
    m_contentVec.push_back(rhs.m_contentVec[i].clone());

  for(std::size_t i = 0; i < rhs.m_anyVec.size(); ++i)
    m_anyVec.push_back(new Any(rhs.m_anyVec[i]));
}

te::xsd::Choice::~Choice()
{
}

te::xsd::Choice& te::xsd::Choice::operator=(const Choice& rhs)
{
  if(this != &rhs)
  {
    Content::operator=(rhs);

    Occurs::operator=(rhs);

    Identifiable::operator=(rhs);

    Annotated::operator=(rhs);

    for(std::size_t i = 0; i < rhs.m_elementVec.size(); ++i)
      m_elementVec.push_back(new Element(rhs.m_elementVec[i]));

    for(std::size_t i = 0; i < rhs.m_contentVec.size(); ++i)
      m_contentVec.push_back(rhs.m_contentVec[i].clone());

    for(std::size_t i = 0; i < rhs.m_anyVec.size(); ++i)
      m_anyVec.push_back(new Any(rhs.m_anyVec[i]));
  }

  return *this;
}

const boost::ptr_vector<te::xsd::Element>& te::xsd::Choice::getElements() const
{
  return m_elementVec;
}

const boost::ptr_vector<te::xsd::Content>& te::xsd::Choice::getContents() const
{
  return m_contentVec;
}

const boost::ptr_vector<te::xsd::Any>& te::xsd::Choice::getAnys() const
{
  return m_anyVec;
}

void te::xsd::Choice::addElement(te::xsd::Element* e)
{
  m_elementVec.push_back(e);
}

void te::xsd::Choice::addContent(te::xsd::Content* c)
{
  m_contentVec.push_back(c);
}

void te::xsd::Choice::addAny(te::xsd::Any* a)
{
  m_anyVec.push_back(a);
}

te::xsd::Content* te::xsd::Choice::clone() const
{
  return new Choice(*this);
}
