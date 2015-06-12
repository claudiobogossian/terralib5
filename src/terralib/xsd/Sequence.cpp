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
  \file Sequence.cpp

  \brief This class models a sequence element in a XML Schema.
*/

// TerraLib
#include "Any.h"
#include "Choice.h"
#include "Element.h"
#include "Group.h"
#include "Sequence.h"

te::xsd::Sequence::Sequence(unsigned int minOccurs, unsigned int maxOccurs, Annotation* ann, std::string* id)
  : Occurs(minOccurs, maxOccurs),
    Identifiable(id),
    Annotated(ann)
{
}

te::xsd::Sequence::Sequence(const Sequence& rhs)
  : Occurs(rhs),
    Identifiable(rhs),
    Annotated(rhs)
{
}

te::xsd::Sequence::~Sequence()
{
}

te::xsd::Sequence& te::xsd::Sequence::operator=(const Sequence& /*rhs*/)
{
  return *this;
}

const boost::ptr_vector<te::xsd::Element>& te::xsd::Sequence::getElements() const
{
  return m_elementVec;
}

const boost::ptr_vector<te::xsd::Content>& te::xsd::Sequence::getContents() const
{
  return m_contentVec;
}

const boost::ptr_vector<te::xsd::Any>& te::xsd::Sequence::getAnys() const
{
  return m_anyVec;
}

void te::xsd::Sequence::addElement(te::xsd::Element* e)
{
  m_elementVec.push_back(e);
}

void te::xsd::Sequence::addContent(te::xsd::Content* c)
{
  m_contentVec.push_back(c);
}

void te::xsd::Sequence::addAny(te::xsd::Any* a)
{
  m_anyVec.push_back(a);
}

te::xsd::Content* te::xsd::Sequence::clone() const
{
  return new Sequence(*this);
}
