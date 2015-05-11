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
  \file Unique.cpp

  \brief It models the unique element in an XML Schema.
*/

// TerraLib
#include "Unique.h"

te::xsd::Unique::Unique(std::string* name, Annotation* ann, std::string* id)
  : IdentityConstraint(name, ann, id)
{
}

te::xsd::Unique::Unique(const Unique& rhs)
  : IdentityConstraint(rhs)
{
}

te::xsd::Unique::~Unique()
{
}

te::xsd::Unique& te::xsd::Unique::operator=(const Unique& /*rhs*/)
{
  return *this;
}

te::xsd::IdentityConstraint* te::xsd::Unique::clone() const
{
  return new Unique(*this);
}
