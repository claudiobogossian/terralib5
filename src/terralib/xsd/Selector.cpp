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
  \file Selector.cpp

  \brief It models the selector element of an XML Schema.
*/

// TerraLib
#include "Selector.h"

// STL
#include <cassert>

te::xsd::Selector::Selector(std::string* xpath, Annotation* ann, std::string* id)
  : Identifiable(id),
    Annotated(ann),
    m_xpath(xpath)
{
  assert(m_xpath);
}

te::xsd::Selector::Selector(const Selector& rhs)
  : Identifiable(rhs),
    Annotated(rhs),
    m_xpath(0)
{
  m_xpath = rhs.m_xpath ? new std::string(*(rhs.m_xpath)) : 0;
}

te::xsd::Selector::~Selector()
{
  delete m_xpath;
}

te::xsd::Selector& te::xsd::Selector::operator=(const Selector& rhs)
{
  return *this;
}

std::string* te::xsd::Selector::getXPath() const
{
  return m_xpath;
}

void te::xsd::Selector::setXPath(std::string* xpath)
{
  delete m_xpath;
  m_xpath = xpath;
}
