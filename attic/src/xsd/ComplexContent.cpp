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
#include "Annotation.h"
#include "ComplexContent.h"
#include "TypeDerivation.h"

// STL
#include <cassert>

te::xsd::ComplexContent::ComplexContent(TypeDerivation* dev, Annotation* ann, std::string* id)
  : Identifiable(id),
    Annotated(ann),
    m_mixed(false),
    m_derivation(dev)
{
}

te::xsd::ComplexContent::ComplexContent(const ComplexContent& rhs)
  : Identifiable(rhs),
    Annotated(rhs),
    m_mixed(rhs.m_mixed),
    m_derivation(0)
{
  m_derivation = rhs.m_derivation ? m_derivation->clone() : 0;
}

te::xsd::ComplexContent::~ComplexContent()
{
  delete m_derivation;
}

te::xsd::ComplexContent& te::xsd::ComplexContent::operator=(const ComplexContent& rhs)
{
  if(this != &rhs)
  {
    Identifiable::operator=(rhs);

    Annotated::operator=(rhs);

    m_mixed = rhs.m_mixed;

    delete m_derivation;

    m_derivation = rhs.m_derivation ? m_derivation->clone() : 0;
  }

  return *this;
}

bool te::xsd::ComplexContent::isMixed()
{
  return m_mixed;
}

te::xsd::TypeDerivation* te::xsd::ComplexContent::getTypeDerivation() const
{
  return m_derivation;
}

void te::xsd::ComplexContent::setAsMixed(bool v)
{
  m_mixed = v;
}

void te::xsd::ComplexContent::setTypeDerivation(TypeDerivation* t)
{
  delete m_derivation;
  m_derivation = t;
}
