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
  \file SimpleTypeConstructor.cpp

  \brief A base class for SimpleType elements. List, Union and Restriction4SimpleType is derived from this base class.
*/

// TerraLib
#include "SimpleTypeConstructor.h"

te::xsd::SimpleTypeConstructor::SimpleTypeConstructor(Annotation* ann, std::string* id)
  : Identifiable(id),
    Annotated(ann)
{
}

te::xsd::SimpleTypeConstructor::SimpleTypeConstructor(const SimpleTypeConstructor& rhs)
  : Identifiable(rhs),
    Annotated(rhs)
{
}

te::xsd::SimpleTypeConstructor::~SimpleTypeConstructor()
{
}

te::xsd::SimpleTypeConstructor& te::xsd::SimpleTypeConstructor::operator=(const SimpleTypeConstructor& rhs)
{
  if(this != &rhs)
  {
    Identifiable::operator=(rhs);
    Annotated::operator=(rhs);
  }

  return *this;
}
