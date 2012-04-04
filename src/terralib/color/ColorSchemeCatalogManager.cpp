/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/color/ColorSchemeCatalogManager.cpp

  \brief Implementation of the singleton to manage color scheme catalogs.
*/

// TerraLib
#include "../common/Exception.h"
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "ColorSchemeCatalog.h"
#include "ColorSchemeCatalogManager.h"

// STL
#include <cassert>

void te::color::ColorSchemeCatalogManager::insert(ColorSchemeCatalog* c)
{
  assert(c);

  if(findByName(c->getName()))
    throw te::common::Exception(TR_COLOR("There is already a color scheme catalog with the given name!"));

  m_catalogs.push_back(c);
  m_catalogIdxByName.insert(std::map<std::string, ColorSchemeCatalog*>::value_type(c->getName(), c));
}

void te::color::ColorSchemeCatalogManager::disconnect(ColorSchemeCatalog* c)
{
  assert(c);

// first, find candidates for deletion... if one of then is not found, raise an exception
  std::map<std::string, ColorSchemeCatalog*>::iterator itProjectIdxByName = m_catalogIdxByName.find(c->getName());

  if(itProjectIdxByName != m_catalogIdxByName.end())
    throw te::common::Exception(TR_COLOR("Couldn't find the catalog with the given name!"));

  size_t i = 0;

  for(; i < m_catalogs.size(); ++i)
  {
    if(m_catalogs[i]->getName() == c->getName())
      break;
  }

  if(i == m_catalogs.size())
    throw te::common::Exception(TR_COLOR("Couldn't find the catalog with the given name!"));
  
// if we are here, so all entries are ok... just remove them
  m_catalogs.erase(m_catalogs.begin() + i);
  m_catalogIdxByName.erase(itProjectIdxByName);
}

void te::color::ColorSchemeCatalogManager::erase(ColorSchemeCatalog* c)
{
  disconnect(c);

// and delete the projet from main memory
  delete (c);
}

te::color::ColorSchemeCatalog* te::color::ColorSchemeCatalogManager::findByName(const std::string& name) const
{
  std::map<std::string, ColorSchemeCatalog*>::const_iterator it = m_catalogIdxByName.find(name);

  if(it != m_catalogIdxByName.end())
    return it->second;

  return 0;
}

std::pair<std::vector<te::color::ColorSchemeCatalog*>::const_iterator,
          std::vector<te::color::ColorSchemeCatalog*>::const_iterator> te::color::ColorSchemeCatalogManager::getIterator() const
{
  return std::pair<std::vector<te::color::ColorSchemeCatalog*>::const_iterator,
                   std::vector<te::color::ColorSchemeCatalog*>::const_iterator>(m_catalogs.begin(), m_catalogs.end());
}

const std::vector<te::color::ColorSchemeCatalog*>& te::color::ColorSchemeCatalogManager::getCatalogs() const
{
  return m_catalogs;
}

bool te::color::ColorSchemeCatalogManager::isEmpty() const
{
  return m_catalogs.empty();
}

void te::color::ColorSchemeCatalogManager::clear()
{
  te::common::FreeContents(m_catalogs);

  m_catalogs.clear();

  m_catalogIdxByName.clear();
}

te::color::ColorSchemeCatalogManager::~ColorSchemeCatalogManager()
{  
  clear();
}

te::color::ColorSchemeCatalogManager::ColorSchemeCatalogManager()
{
}

