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
  \file Facet.cpp

  \brief This class can be used to model a facet (a constraint in the range of values).
*/

// TerraLib
#include "Facet.h"

std::string te::xsd::Facet::m_typeNames[] = {std::string("Enumeration"),
                                             std::string("FractionDigits"),
                                             std::string("Length"),
                                             std::string("MaxExclusive"),
                                             std::string("MaxInclusive"),
                                             std::string("MaxLength"),
                                             std::string("MinExclusive"),
                                             std::string("MinInclusive"),
                                             std::string("MinLength"),
                                             std::string("Pattern"),
                                             std::string("TotalDigits"),
                                             std::string("WhiteSpace")};

te::xsd::Facet::Facet(FacetType fType, const std::string& value)
  : m_type(fType),
    m_value(value)
{
}

te::xsd::Facet::Facet(const Facet& rhs)
{
}

te::xsd::Facet::~Facet()
{
}

te::xsd::Facet& te::xsd::Facet::operator=(const Facet& rhs)
{
  return *this;
}

te::xsd::FacetType te::xsd::Facet::getType() const
{
  return m_type;
}

const std::string& te::xsd::Facet::getName() const
{
  return m_typeNames[m_type];
}

const std::string& te::xsd::Facet::getValue()
{
  return m_value;
}
