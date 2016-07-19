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
  \file SimpleContent.cpp

  \brief A class that models an XML Schema simpleContent element.
*/

// TerraLib
#include "SimpleContent.h"
#include "TypeDerivation.h"

// STL
#include <cassert>

te::xsd::SimpleContent::SimpleContent(TypeDerivation* dev, Annotation* ann, std::string* id)
  : Identifiable(id),
    Annotated(ann),
    m_derivation(dev)
{
}

te::xsd::SimpleContent::SimpleContent(const SimpleContent& rhs)
  : Identifiable(rhs),
    Annotated(rhs)
{
}

te::xsd::SimpleContent::~SimpleContent()
{
  delete m_derivation;
}

te::xsd::SimpleContent& te::xsd::SimpleContent::operator=(const SimpleContent& rhs)
{
  if(this != &rhs)
  {
    Identifiable::operator=(rhs);

    Annotated::operator=(rhs);

    delete m_derivation;

    m_derivation = rhs.m_derivation ? rhs.m_derivation->clone() : 0;
  }

  return *this;
}

te::xsd::TypeDerivation* te::xsd::SimpleContent::getTypeDerivation() const
{
  return m_derivation;
}

void te::xsd::SimpleContent::setTypeDerivation(TypeDerivation* dev)
{
  delete m_derivation;
  m_derivation = dev;
}
