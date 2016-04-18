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
  \file terralib/core/lib/LibraryManager.cpp

  \brief A singleton that can be used to observe the available libraries in the system.

  \author Gilberto Ribeiro de Queiroz
 */

// TerraLib
#include "LibraryManager.h"
#include "Library.h"

// STL
#include <cassert>
#include <map>

struct te::core::LibraryManager::Impl
{
  std::map<std::string, std::weak_ptr<Library> > libraries;
};

void
te::core::LibraryManager::insert(const std::string& id, const std::shared_ptr<Library>& l)
{
  std::map<std::string, std::weak_ptr<Library> >::const_iterator it = m_pimpl->libraries.find(id);
  if(it != m_pimpl->libraries.end())
  {
    boost::format err_msg("A library named '%1%' is already registered with identifier: %2%.");

    std::shared_ptr<Library> lib = it->second.lock();

    std::string lib_name = lib ? lib->getFileName() : std::string();
  
    throw te::InvalidArgumentException() << te::ErrorDescription((errs_msg % lib_name % it->first).str());
  }

  m_pimpl->libraries.insert(std::make_pair(id, l));
}

std::shared_ptr<te::core::Library>
te::core::LibraryManager::get(const std::string& id)
{
  std::map<std::string, std::weak_ptr<Library> >::const_iterator it = m_pimpl->libraries.find(id);

  if(it == m_pimpl->libraries.end())
  {
    boost::format err_msg("There is no library registered with identifier: %1%.");

    throw te::OutOfRangeException() << te::ErrorDescription((errs_msg % id).str());
  }

  return it->second.lock();
}

te::core::LibraryManager&
te::core::LibraryManager::instance()
{
  static LibraryManager inst;

  return inst;
}

te::core::LibraryManager::LibraryManager()
  : m_pimpl(nullptr)
{
  m_pimpl = new Impl;
}

te::core::LibraryManager::~LibraryManager()
{
  delete m_pimpl;
}

