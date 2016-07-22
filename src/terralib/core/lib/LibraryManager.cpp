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
  \author Matheus Cavassan Zaglia
 */

// TerraLib
#include "LibraryManager.h"
#include "Exception.h"
#include "../translator/Translator.h"

// STL
#include <vector>

// Boost
#include <boost/format.hpp>

struct te::core::LibraryManager::Impl
{
  std::vector<te::core::LibraryEntry > library_entries;
};

void
te::core::LibraryManager::insert(const te::core::LibraryEntry &entry)
{
  if(exists(entry.m_name))
  {
    boost::format err_msg(TE_TR("There is already a library registered with the name: %1%."));

    throw te::InvalidArgumentException() << te::ErrorDescription((err_msg % entry.m_name).str());
  }

  m_pimpl->library_entries.push_back(entry);
}

void
te::core::LibraryManager::remove(const std::string &name)
{
  std::vector<LibraryEntry>::iterator it = std::find_if(m_pimpl->library_entries.begin(),
                                                              m_pimpl->library_entries.end(),
                                                              [&name](const LibraryEntry& le)
                                                              { return le.m_name == name; });

  if(it != m_pimpl->library_entries.end())
  {
    boost::format err_msg(TE_TR("There is no library registered with the name: %1%."));

    throw te::OutOfRangeException() << te::ErrorDescription((err_msg % name).str());
  }
  if(it->m_initialized)
  {
    boost::format err_msg(TE_TR("The library cannot be initialized in order to remove it."));

    throw te::Exception() << te::ErrorDescription((err_msg % name).str());
  }

  m_pimpl->library_entries.erase(it);
}


const te::core::LibraryEntry&
te::core::LibraryManager::get(const std::string& name)
{
  for(const LibraryEntry& le: m_pimpl->library_entries)
  {
    if(le.m_name == name)
      return le;
  }

  boost::format err_msg(TE_TR("There is no library registered with the name: %1%."));

  throw te::OutOfRangeException() << te::ErrorDescription((err_msg % name).str());
}

bool
te::core::LibraryManager::isInitialized(const std::string& name)
{
  for(const LibraryEntry& le: m_pimpl->library_entries)
  {
    if(le.m_name == name)
      return le.m_initialized;
  }

  boost::format err_msg(TE_TR("There is no library registered with the name: %1%."));

  throw te::OutOfRangeException() << te::ErrorDescription((err_msg % name).str());
}

bool te::core::LibraryManager::exists(const std::string &name)
{
  return std::find_if(m_pimpl->library_entries.begin(),
                      m_pimpl->library_entries.end(),
                      [&name](const LibraryEntry& le)
                      { return le.m_name == name; }) != m_pimpl->library_entries.end();
}

te::core::LibraryManager&
te::core::LibraryManager::instance()
{
  static LibraryManager instance;

  return instance;
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

