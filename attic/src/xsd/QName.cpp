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
  \file QName.cpp

  \brief A class to deal with XML qualified names.
*/

// TerraLib
#include "QName.h"

te::xsd::QName::QName()
{
}

te::xsd::QName::QName(const std::string& prefix, const std::string& localPart)
  : m_prefix(prefix),
    m_localPart(localPart)
{
}

te::xsd::QName::QName(const QName& rhs)
  : m_prefix(rhs.m_prefix),
    m_localPart(rhs.m_localPart)
{
}

te::xsd::QName::~QName()
{
}

te::xsd::QName& te::xsd::QName::operator=(const QName& rhs)
{
  if(this != &rhs)
  {
    m_prefix = rhs.m_prefix;
    m_localPart = rhs.m_localPart;
  }

  return *this;
}

std::string te::xsd::QName::getQualifiedName() const
{
  return m_prefix + ":" + m_localPart;
}

const std::string& te::xsd::QName::getPrefix() const
{
  return m_prefix;
}

const std::string& te::xsd::QName::getLocalPart() const
{
  return m_localPart;
}

void te::xsd::QName::setPrefix(const std::string& prefix)
{
  m_prefix = prefix;
}

void te::xsd::QName::setLocalPart(const std::string& localPart)
{
  m_localPart = localPart;
}
