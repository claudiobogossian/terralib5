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
  \file terralib/se/CoverageStyle.cpp
  
  \brief The CoverageStyle defines the styling that is to be applied to a subset of Coverage data.
*/

// TerraLib
#include "../xlink/SimpleLink.h"
#include "CoverageStyle.h"
#include "Description.h"
#include "Rule.h"

const std::string te::se::CoverageStyle::sm_type("CoverageStyle");

te::se::CoverageStyle::CoverageStyle()
  : m_coverageName(0)
{
}

te::se::CoverageStyle::~CoverageStyle()
{
  delete m_coverageName;
}

void te::se::CoverageStyle::setCoverageName(std::string* name)
{
  delete m_coverageName;
  m_coverageName = name;
}

const std::string* te::se::CoverageStyle::getCoverageName() const
{
  return m_coverageName;
}

const std::string& te::se::CoverageStyle::getType() const
{
  return sm_type;
}

te::se::Style* te::se::CoverageStyle::clone() const
{
  CoverageStyle* style = new CoverageStyle();

  if(m_name)
    style->setName(new std::string(*m_name));

  if(m_description)
    style->setDescription(m_description->clone());

  if(m_semanticTypeIdentifiers)
  {
    for(std::size_t i = 0; i < m_semanticTypeIdentifiers->size(); ++i)
    {
      style->m_semanticTypeIdentifiers->push_back(m_semanticTypeIdentifiers->at(i));
    }
  }

  for(std::size_t i = 0; i < m_rules.size(); ++i)
  {
    if(m_rules[i])
      style->m_rules.push_back(m_rules[i]->clone());
  }

  for(std::size_t i = 0; i < m_onlineResources.size(); ++i)
  {
    if(m_onlineResources[i])
      style->m_onlineResources.push_back(m_onlineResources[i]->clone());
  }

  style->m_version = m_version;

  if(m_coverageName)
    style->setCoverageName(new std::string(*m_coverageName));

  return style;
}