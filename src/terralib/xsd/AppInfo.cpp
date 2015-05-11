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
  \file AppInfo.cpp

  \brief This class models an object that specifies information to be used by applications.
*/

// TerraLib
#include "AppInfo.h"

te::xsd::AppInfo::AppInfo(std::string* source, std::string* value)
  : m_source(source),
    m_value(value)
{
}

te::xsd::AppInfo::AppInfo(const AppInfo& rhs)
  : m_source(0),
    m_value(0)
{
  m_source = rhs.m_source ? new std::string(*rhs.m_source) : 0;
  m_value = rhs.m_value ? new std::string(*rhs.m_value) : 0;
}

te::xsd::AppInfo::~AppInfo()
{
  delete m_source;
  delete m_value;
}

te::xsd::AppInfo& te::xsd::AppInfo::operator=(const AppInfo& rhs)
{
  if(this != &rhs)
  {
    delete m_source;

    m_source = rhs.m_source ? new std::string(*rhs.m_source) : 0;

    delete m_value;

    m_value = rhs.m_value ? new std::string(*rhs.m_value) : 0;
  }

  return *this;
}

void te::xsd::AppInfo::setValue(std::string* value)
{
  delete m_value;
  m_value = value;
}

void te::xsd::AppInfo::setSource(std::string* source)
{
  delete m_source;
  m_source = source;
}

std::string* te::xsd::AppInfo::getValue() const
{
  return m_value;
}

std::string* te::xsd::AppInfo::getSource() const
{
  return m_source;
}

te::xsd::AnnotationItem* te::xsd::AppInfo::clone() const
{
  return new AppInfo(*this);
}


