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
  \file All.cpp

  \brief This class models an XML Schema all element.
*/

// TerraLib
#include "All.h"
#include "Annotation.h"
#include "Element.h"

// STL
#include <cassert>

te::xsd::All::All(unsigned int minOccurs, unsigned int maxOccurs, Annotation* ann, std::string* id)
  : Occurs(minOccurs, maxOccurs),
    Identifiable(id),
    Annotated(ann)
{
}

te::xsd::All::All(const All& rhs)
  : Occurs(rhs),
    Identifiable(rhs),
    Annotated(rhs)
{
  for(std::size_t i = 0; i < rhs.m_elementVec.size(); ++i)
    m_elementVec.push_back(new Element(m_elementVec[i]));
}

te::xsd::All::~All()
{
}

te::xsd::All& te::xsd::All::operator=(const All& rhs)
{
  if(this != &rhs)
  {
    Occurs::operator=(rhs);

    Identifiable::operator=(rhs);

    Annotated::operator=(rhs);

    for(std::size_t i = 0; i < rhs.m_elementVec.size(); ++i)
      m_elementVec.push_back(new Element(rhs.m_elementVec[i]));
  }

  return *this;
}

const boost::ptr_vector<te::xsd::Element>& te::xsd::All::getElements() const
{
  return m_elementVec;
}

void te::xsd::All::addElement(te::xsd::Element* e)
{
  m_elementVec.push_back(e);
}

te::xsd::Content* te::xsd::All::clone() const
{
  return new All(*this);
}
