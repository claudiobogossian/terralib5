/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/se/SymbologyManager.cpp
 
  \brief The SymbologyManager is a singleton that can be used to manage all loaded symbols in TerraLib.
*/

// TerraLib
#include "../../../common/Exception.h"
#include "../../../common/STLUtils.h"
#include "../../../common/Translator.h"
#include "Symbol.h"
#include "SymbologyManager.h"

// STL
#include <cassert>

void te::qt::widgets::SymbologyManager::insert(te::qt::widgets::Symbol* symbol)
{
  assert(symbol);

  std::size_t id = symbol->getInfo().m_id;
  if(findById(id))
    throw te::common::Exception(TR_QT_WIDGETS("There is already a symbol with the given id."));

  m_symbolMap[id] = symbol;
}

void te::qt::widgets::SymbologyManager::remove(te::qt::widgets::Symbol* symbol)
{
  assert(symbol);

  std::size_t id = symbol->getInfo().m_id;
  std::map<std::size_t, Symbol*>::iterator it = m_symbolMap.find(id);

  if(it == m_symbolMap.end())
    throw te::common::Exception(TR_QT_WIDGETS("Could not find the symbol with the given id."));

  m_symbolMap.erase(it);
}

te::qt::widgets::Symbol* te::qt::widgets::SymbologyManager::findById(const std::size_t& id) const
{
  std::map<std::size_t, Symbol*>::const_iterator it = m_symbolMap.find(id);

  if(it != m_symbolMap.end())
    return it->second;

  return 0;
}

std::pair<std::map<std::size_t, te::qt::widgets::Symbol*>::const_iterator,
          std::map<std::size_t, te::qt::widgets::Symbol*>::const_iterator> te::qt::widgets::SymbologyManager::getIterator() const
{
  return std::pair<std::map<std::size_t, Symbol*>::const_iterator,
                   std::map<std::size_t, Symbol*>::const_iterator>(m_symbolMap.begin(), m_symbolMap.end());
}

void te::qt::widgets::SymbologyManager::clear()
{
  te::common::FreeContents(m_symbolMap);
  m_symbolMap.clear();
}

te::qt::widgets::SymbologyManager::~SymbologyManager()
{
}

te::qt::widgets::SymbologyManager::SymbologyManager()
{
}
