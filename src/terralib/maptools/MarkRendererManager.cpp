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
  \file terralib/maptools/MarkRendererManager.cpp

  \brief This is a singleton for managing all mark renderers instances available in the system.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../se/Mark.h"
#include "AbstractMarkRenderer.h"
#include "Exception.h"
#include "MarkRendererManager.h"

// Boost
#include <boost/format.hpp>

// STL
#include <cassert>
#include <vector>

te::map::MarkRendererManager::MarkRendererManager()
{
}

te::map::MarkRendererManager::~MarkRendererManager()
{
  clear();
}

te::color::RGBAColor** te::map::MarkRendererManager::render(const te::se::Mark* mark, std::size_t size)
{
  LockWrite l;

  if(size == 0)
    throw Exception(TE_TR("Requested size is invalid!"));

  if(mark == 0)
    throw Exception(TE_TR("The given mark is invalid!"));

  const std::string* name = mark->getWellKnownName();
  if(name == 0)
    throw Exception(TE_TR("The given mark not have a valid well known name!"));

  if(name->empty())
    throw Exception(TE_TR("The mark well known name is empty!"));

  // Default renderer key
  std::string key("");

  // Try find the renderer key
  std::vector<std::string> tokens;
  te::common::Tokenize(*name, tokens, "://");

  if(tokens.size() > 1)
    key = tokens[0];

  std::map<std::string, AbstractMarkRenderer*>::iterator it = m_renderers.find(key);

  if(it == m_renderers.end())
    throw Exception((boost::format(TE_TR("There is not a mark renderer registered for the given mark %1%.")) % *name).str());

  return it->second->render(mark, size);
}

void te::map::MarkRendererManager::getAllSupportedMarks(std::vector<std::string>& marks) const
{
  std::map<std::string, AbstractMarkRenderer*>::const_iterator it;
  for(it = m_renderers.begin(); it != m_renderers.end(); ++it)
    it->second->getSupportedMarks(marks);
}

void te::map::MarkRendererManager::add(const std::string& key, AbstractMarkRenderer* renderer)
{
  std::map<std::string, AbstractMarkRenderer*>::iterator it = m_renderers.find(key);

  if(it != m_renderers.end())
    throw Exception((boost::format(TE_TR("There is already a mark renderer registered with the given key %1%.")) % key).str());

  m_renderers[key] = renderer;
}

void te::map::MarkRendererManager::clear()
{
  te::common::FreeContents(m_renderers);
  m_renderers.clear();
}
