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
  \file Sequence.cpp

  \brief This class models a sequence element in a XML Schema.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "Any.h"
#include "Choice.h"
#include "Element.h"
#include "Group.h"
#include "Sequence.h"

te::xsd::Sequence::Sequence(unsigned int minOccurs, unsigned int maxOccurs, Annotation* ann, std::string* id)
  : Occurs(minOccurs, maxOccurs),
    Identifiable(id),
    Annotated(ann),
    m_elements(0),
    m_contents(0),
    m_anys(0)
{
}

te::xsd::Sequence::Sequence(const Sequence& rhs)
  : Occurs(rhs),
    Identifiable(rhs),
    Annotated(rhs),
    m_elements(0),
    m_contents(0),
    m_anys(0)
{
}

te::xsd::Sequence::~Sequence()
{
  te::common::Free(m_elements);

  te::common::Free(m_contents);

  te::common::Free(m_anys);
}

te::xsd::Sequence& te::xsd::Sequence::operator=(const Sequence& /*rhs*/)
{
  return *this;
}

std::vector<te::xsd::Element*>* te::xsd::Sequence::getElements() const
{
  return m_elements;
}

std::vector<te::xsd::Content*>* te::xsd::Sequence::getContents() const
{
  return m_contents;
}

std::vector<te::xsd::Any*>* te::xsd::Sequence::getAnys() const
{
  return m_anys;
}

void te::xsd::Sequence::addElement(te::xsd::Element* e)
{
  if(m_elements == 0)
    m_elements = new std::vector<te::xsd::Element*>;

  m_elements->push_back(e);
}

void te::xsd::Sequence::addContent(te::xsd::Content* c)
{
  if(m_contents == 0)
    m_contents = new std::vector<te::xsd::Content*>;

  m_contents->push_back(c);
}

void te::xsd::Sequence::addAny(te::xsd::Any* a)
{
  if(m_anys == 0)
    m_anys = new std::vector<te::xsd::Any*>;

  m_anys->push_back(a);
}

te::xsd::Content* te::xsd::Sequence::clone() const
{
  return new Sequence(*this);
}


