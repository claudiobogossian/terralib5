/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/common/plugin/LibraryManager.cpp

  \brief A singleton that can be used to observe the available libraries in the system.

  \author Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
 */

// TerraLib
#include "LibraryManager.h"
#include "Library.h"

// STL
#include <cassert>
#include <map>

struct te::module::LibraryManager::Impl
{
  std::map<std::string, std::weak_ptr<Library> > libraries;
};

void
te::module::LibraryManager::insert(const std::string& id, const std::shared_ptr<Library>& l)
{
  m_pimpl->libraries[id] = l;
}

std::shared_ptr<te::module::Library>
te::module::LibraryManager::get(const std::string& id)
{
  std::map<std::string, std::weak_ptr<Library> >::const_iterator it = m_pimpl->libraries.find(id);

  if(it != m_pimpl->libraries.end())
    return it->second.lock();

  return std::shared_ptr<te::module::Library>();
}

te::module::LibraryManager&
te::module::LibraryManager::instance()
{
  static LibraryManager inst;

  return inst;
}

te::module::LibraryManager::LibraryManager()
  : m_pimpl(nullptr)
{
  m_pimpl = new Impl;
}

te::module::LibraryManager::~LibraryManager()
{
  delete m_pimpl;
}

