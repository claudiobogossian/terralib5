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
  \file terralib/se/Style.cpp
  
  \brief The Style defines the styling that is to be applied to a single feature (also applies to coverage).
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../xlink/SimpleLink.h"
#include "Description.h"
#include "Rule.h"
#include "Style.h"

// STL
#include <cassert>

te::se::Style::Style()
  : m_name(0),
    m_description(0),
    m_semanticTypeIdentifiers(0),
  m_version(TE_SE_DEFAULT_VERSION)
{
}

te::se::Style::~Style()
{
  delete m_name;
  delete m_description;
  delete m_semanticTypeIdentifiers;
  te::common::FreeContents(m_rules);
  te::common::FreeContents(m_onlineResources);
}

void te::se::Style::setName(std::string* name)
{
  delete m_name;
  m_name = name;
}

const std::string* te::se::Style::getName() const
{
  return m_name;
}

void te::se::Style::setDescription(Description* d)
{
  delete m_description;
  m_description = d;
}

const te::se::Description* te::se::Style::getDescription() const
{
  return m_description;
}

void te::se::Style::push_back(const std::string& semanticTypeIdentifier)
{
  if(m_semanticTypeIdentifiers == 0)
    m_semanticTypeIdentifiers = new std::vector<std::string>;

  m_semanticTypeIdentifiers->push_back(semanticTypeIdentifier);
}

const std::string& te::se::Style::getSemanticTypeIdentifier(std::size_t i)
{
  assert(m_semanticTypeIdentifiers && (i < m_semanticTypeIdentifiers->size()));
  return (*m_semanticTypeIdentifiers)[i];
}

const std::vector<std::string>* te::se::Style::getSemanticTypeIdentifiers() const
{
  return m_semanticTypeIdentifiers;
}

const std::vector<te::se::Rule*>& te::se::Style::getRules() const
{
  return m_rules;
}

void te::se::Style::push_back(Rule* rule)
{
  assert(rule);
  m_rules.push_back(rule);
}

te::se::Rule* te::se::Style::getRule(std::size_t i) const
{
  assert(i < m_rules.size());
  return m_rules[i];
}

void te::se::Style::push_back(te::xl::SimpleLink* onlineResource)
{
  assert(onlineResource);
  m_onlineResources.push_back(onlineResource);
}

const std::vector<te::xl::SimpleLink*>& te::se::Style::getOnlineResources() const
{
  return m_onlineResources;
}

const te::xl::SimpleLink* te::se::Style::getOnlineResource(std::size_t i) const
{
  assert(i < m_onlineResources.size());
  return m_onlineResources[i];
}

void te::se::Style::setVersion(const std::string& v)
{
  m_version = v;
}

const std::string& te::se::Style::getVersion() const
{
  return m_version;
}

