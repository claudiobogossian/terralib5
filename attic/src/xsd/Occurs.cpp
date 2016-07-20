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
  \file Occurs.cpp

  \brief A base class for XSD classes that have occurs attributes.
*/

// TerraLib
#include "Occurs.h"

// STL
#include <cassert>
#include <limits>

const unsigned int te::xsd::Occurs::unbounded = std::numeric_limits<unsigned int>::max();

const unsigned int te::xsd::Occurs::getMinOccurs() const
{
  return m_minOccurs;
}

const unsigned int te::xsd::Occurs::getMaxOccurs() const
{
  return m_maxOccurs;
}

void te::xsd::Occurs::setMinOccurs(unsigned int minOccurs)
{
  m_minOccurs = minOccurs;
}

void te::xsd::Occurs::setMaxOccurs(unsigned int maxOccurs)
{
  m_maxOccurs = maxOccurs;
}

te::xsd::Occurs::Occurs(unsigned int minOccurs, unsigned int maxOccurs)
  : m_minOccurs(minOccurs),
    m_maxOccurs(maxOccurs)
{
}

te::xsd::Occurs::Occurs(const Occurs& rhs)
  : m_minOccurs(rhs.m_minOccurs),
    m_maxOccurs(rhs.m_maxOccurs)
{
}

te::xsd::Occurs::~Occurs()
{
}

te::xsd::Occurs& te::xsd::Occurs::operator=(const Occurs& rhs)
{
  if(this != &rhs)
  {
    m_minOccurs = rhs.m_minOccurs;
    m_maxOccurs = rhs.m_maxOccurs;
  }

  return *this;
}
