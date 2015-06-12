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
  \file terralib/color/ColorSchemeGroup.cpp

  \brief Implementation of the color scheme group class.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "ColorScheme.h"
#include "ColorSchemeCatalog.h"
#include "ColorSchemeGroup.h"

// STL
#include <algorithm>
#include <cassert>

te::color::ColorSchemeGroup::ColorSchemeGroup(const std::string& name)
  : m_parent(0),
    m_name(name)
{
}

te::color::ColorSchemeGroup::~ColorSchemeGroup()
{
  te::common::FreeContents(m_colorSchemes);
}

const std::string& te::color::ColorSchemeGroup::getName() const
{
  return m_name;
}

void te::color::ColorSchemeGroup::setName(const std::string& name)
{
  assert(name.empty() == false);
  
  if(m_parent == 0)
  {
    m_name = name;
    return;
  }
// disconnect from the parent, change name and re-connect
  m_parent->disconnect(this);
  m_name = name;
  m_parent->push_back(this);
}

const std::string& te::color::ColorSchemeGroup::getDescription() const
{
  return m_description;
}

void te::color::ColorSchemeGroup::setDescription(const std::string& d)
{
  m_description = d;
}

void te::color::ColorSchemeGroup::push_back(ColorScheme* colorScheme)
{
  assert(colorScheme && (colorScheme->getParent() == 0));
  m_colorSchemes.push_back(colorScheme);
  colorScheme->setParent(this);
}

void te::color::ColorSchemeGroup::disconnect(ColorScheme* colorScheme)
{
  std::vector<ColorScheme*>::iterator it = std::find(m_colorSchemes.begin(), m_colorSchemes.end(), colorScheme);

  if(it != m_colorSchemes.end())
    m_colorSchemes.erase(it);
}

const std::vector<te::color::ColorScheme*>& te::color::ColorSchemeGroup::getColorSchemes() const
{
  return m_colorSchemes;
}

te::color::ColorSchemeCatalog* te::color::ColorSchemeGroup::getParent() const
{
  return m_parent;
}

void te::color::ColorSchemeGroup::setParent(ColorSchemeCatalog* parent)
{
  m_parent = parent;
}

