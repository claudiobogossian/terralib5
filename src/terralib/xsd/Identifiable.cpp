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
  \file Identifiable.cpp

  \brief A base class for XSD classes that must provide a unique ID property.
*/

// TerraLib
#include "Identifiable.h"

void te::xsd::Identifiable::setId(std::string* id)
{
  delete m_id;
  m_id = id;
}

std::string* te::xsd::Identifiable::getId() const
{
  return m_id;
}

te::xsd::Identifiable::Identifiable(std::string* id)
  : m_id(id)
{
}

te::xsd::Identifiable::Identifiable(const Identifiable& rhs)
  : m_id(0)
{
  m_id = rhs.m_id ? new std::string(*rhs.m_id) : 0;
}

te::xsd::Identifiable::~Identifiable()
{
  delete m_id;
}

te::xsd::Identifiable& te::xsd::Identifiable::operator=(const Identifiable& rhs)
{
  if(this != &rhs)
  {
    delete m_id;

    m_id = rhs.m_id ? new std::string(*rhs.m_id) : 0;
  }

  return *this;
}
