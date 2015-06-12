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
  \file terralib/qt/widgets/se/SymbolLibraryManager.cpp
 
  \brief The SymbolLibraryManager is a singleton that can be used to manage all loaded symbol libraries in TerraLib.
*/

// TerraLib
#include "../../../common/Exception.h"
#include "../../../common/STLUtils.h"
#include "../../../common/Translator.h"
#include "SymbolLibrary.h"
#include "SymbolLibraryManager.h"

// STL
#include <cassert>

void te::qt::widgets::SymbolLibraryManager::insert(te::qt::widgets::SymbolLibrary* library)
{
  assert(library);

  std::string name = library->getName();
  if(findByName(name))
    throw te::common::Exception(TE_TR("There is already a symbol library with the given name."));

  m_symbolLibraryMap[name] = library;
}

void te::qt::widgets::SymbolLibraryManager::remove(te::qt::widgets::SymbolLibrary* library)
{
  assert(library);

  std::string name = library->getName();
  std::map<std::string, SymbolLibrary*>::iterator it = m_symbolLibraryMap.find(name);

  if(it == m_symbolLibraryMap.end())
    throw te::common::Exception(TE_TR("Could not find the symbol library with the given name."));

  m_symbolLibraryMap.erase(it);
  
  delete it->second;
}

te::qt::widgets::SymbolLibrary* te::qt::widgets::SymbolLibraryManager::findByName(const std::string& name) const
{
  std::map<std::string, SymbolLibrary*>::const_iterator it = m_symbolLibraryMap.find(name);

  if(it != m_symbolLibraryMap.end())
    return it->second;

  return 0;
}

std::pair<std::map<std::string, te::qt::widgets::SymbolLibrary*>::const_iterator,
          std::map<std::string, te::qt::widgets::SymbolLibrary*>::const_iterator> te::qt::widgets::SymbolLibraryManager::getIterator() const
{
  return std::pair<std::map<std::string, SymbolLibrary*>::const_iterator,
                   std::map<std::string, SymbolLibrary*>::const_iterator>(m_symbolLibraryMap.begin(), m_symbolLibraryMap.end());
}

void te::qt::widgets::SymbolLibraryManager::clear()
{
  te::common::FreeContents(m_symbolLibraryMap);
  m_symbolLibraryMap.clear();
}

te::qt::widgets::SymbolLibraryManager::~SymbolLibraryManager()
{
}

te::qt::widgets::SymbolLibraryManager::SymbolLibraryManager()
{
}
