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
  \file terralib/color/ColorSchemeCatalog.cpp

  \brief Implementation of the color scheme catalog class.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "ColorScheme.h"
#include "ColorSchemeCatalog.h"
#include "ColorSchemeCatalogManager.h"
#include "ColorSchemeGroup.h"

// STL
#include <algorithm>
#include <cassert>

te::color::ColorSchemeCatalog::ColorSchemeCatalog(const std::string& name)
  : m_name(name)
{
}

te::color::ColorSchemeCatalog::~ColorSchemeCatalog()
{
  te::common::FreeContents(m_colorSchemeGroups);
}

const std::string& te::color::ColorSchemeCatalog::getName() const
{
  return m_name;
}

void te::color::ColorSchemeCatalog::setName(const std::string& newName)
{
  assert(newName.empty() == false);

  if(ColorSchemeCatalogManager::getInstance().findByName(m_name)) // does it relate to any manager?
  {
    ColorSchemeCatalogManager::getInstance().disconnect(this);
    m_name = newName;
    ColorSchemeCatalogManager::getInstance().insert(this);
  }
  else  //it doesn't relate to any manager
    m_name = newName;
}

const std::string& te::color::ColorSchemeCatalog::getDescription() const
{
  return m_description;
}

void te::color::ColorSchemeCatalog::setDescription(const std::string& d)
{
  m_description = d;
}

const std::string& te::color::ColorSchemeCatalog::getAuthor() const
{
  return m_author;
}

void te::color::ColorSchemeCatalog::setAuthor(const std::string& author)
{
  m_author = author;
}

const std::string& te::color::ColorSchemeCatalog::getCopyright() const
{
  return m_copyright;
}

void te::color::ColorSchemeCatalog::setCopyright(const std::string& c)
{
  m_copyright = c;
}

const std::string& te::color::ColorSchemeCatalog::getOnlineResource() const
{
  return m_onlineResource;
}

void te::color::ColorSchemeCatalog::setOnlineResource(const std::string& r)
{
  m_onlineResource = r;
}

void te::color::ColorSchemeCatalog::push_back(ColorSchemeGroup* group)
{
  assert(group);

  m_colorSchemeGroups.push_back(group);
  group->setParent(this);
}

void te::color::ColorSchemeCatalog::disconnect(ColorSchemeGroup* group)
{
  std::vector<ColorSchemeGroup*>::iterator it = std::find(m_colorSchemeGroups.begin(), m_colorSchemeGroups.end(), group);

  if(it != m_colorSchemeGroups.end())
    m_colorSchemeGroups.erase(it);
}

const std::vector<te::color::ColorSchemeGroup*>& te::color::ColorSchemeCatalog::getColorSchemeGroups() const
{
  return m_colorSchemeGroups;
}



