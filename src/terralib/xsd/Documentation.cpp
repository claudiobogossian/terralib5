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
  \file Documentation.cpp

  \brief A class that models a documentation element used to enter text comments in annotations.
*/

// TerraLib
#include "Documentation.h"

te::xsd::Documentation::Documentation(std::string* value, std::string* source, std::string* lang)
  : m_value(value),
    m_source(source),
    m_lang(lang)
{
}

te::xsd::Documentation::Documentation(const Documentation& rhs)
  : m_value(0),
    m_source(0),
    m_lang(0)
{
  m_value = rhs.m_value ? new std::string(*rhs.m_value) : 0;
  m_source = rhs.m_source ? new std::string(*rhs.m_source) : 0;
  m_lang = rhs.m_lang ? new std::string(*rhs.m_lang) : 0;
}

te::xsd::Documentation::~Documentation()
{
  delete m_value;
  delete m_source;
  delete m_lang;
}

te::xsd::Documentation& te::xsd::Documentation::operator=(const Documentation& rhs)
{
  if(this != &rhs)
  {
    delete m_value;

    m_value = rhs.m_value ? new std::string(*rhs.m_value) : 0;

    delete m_source;

    m_source = rhs.m_source ? new std::string(*rhs.m_source) : 0;

    delete m_lang;

    m_lang = rhs.m_lang ? new std::string(*rhs.m_lang) : 0;
  }

  return *this;
}

std::string* te::xsd::Documentation::getLang() const
{
  return m_lang;
}

std::string* te::xsd::Documentation::getValue() const
{
  return m_value;
}

std::string* te::xsd::Documentation::getSource() const
{
  return m_source;
}

void te::xsd::Documentation::setLang(std::string* lang)
{
  delete m_lang;
  m_lang = lang;
}

void te::xsd::Documentation::setValue(std::string* doc)
{
  delete m_value;
  m_value = doc;
}

void te::xsd::Documentation::setSource(std::string* source)
{
  delete m_source;
  m_source = source;
}

te::xsd::AnnotationItem* te::xsd::Documentation::clone() const
{
  return new Documentation(*this);
}
