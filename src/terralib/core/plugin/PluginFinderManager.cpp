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
  \file terralib/common/plugin/PluginFinderManager.cpp

  \brief A singleton that can be used to register plugin finders.

  \author Gilberto Ribeiro de Queiroz
  \author Matheus Cavassan Zaglia
 */

// TerraLib
#include "PluginFinderManager.h"
#include "Exception.h"

// STL
#include <map>

// Boost
#include <boost/format.hpp>

struct te::core::PluginFinderManager::Impl
{
  std::map<std::string, PluginFinderFnct> finders;
};

std::vector<std::string>
te::core::PluginFinderManager::getFinders() const
{
  typedef std::map<std::string, PluginFinderFnct>::value_type plugin_finder_t;

  std::vector<std::string> finders;

  for(const plugin_finder_t& pf : m_pimpl->finders)
    finders.push_back(pf.first);
  
  return finders;
}

te::core::PluginFinderFnct&
te::core::PluginFinderManager::get(const std::string& finder_name)
{
  std::map<std::string, PluginFinderFnct>::iterator it = m_pimpl->finders.find(finder_name);

  if(it == m_pimpl->finders.end())
    throw OutOfRangeException() << ErrorDescription((boost::format("could not find plugin finder: '%1%'.") % finder_name).str());

  return it->second;
}

void
te::core::PluginFinderManager::insert(const std::string& finder_name,
                                      PluginFinderFnct finder)
{
  if(exists(finder_name))
  {
    boost::format err_msg("there is already a plugin finder registered with the name: '%1%'");

    throw InvalidArgumentException() << ErrorDescription((err_msg % finder_name).str());
  }

  m_pimpl->finders[finder_name] = finder;
}

void
te::core::PluginFinderManager::remove(const std::string& finder_name)
{
  std::map<std::string, PluginFinderFnct>::iterator it = m_pimpl->finders.find(finder_name);

  if(it == m_pimpl->finders.end())
    throw OutOfRangeException() << ErrorDescription((boost::format("could not find plugin finder: '%1%'.") % finder_name).str());

  m_pimpl->finders.erase(it);
}

bool
te::core::PluginFinderManager::exists(const std::string& finder_name) const
{
  return (m_pimpl->finders.find(finder_name) != m_pimpl->finders.end());
}

te::core::PluginFinderManager&
te::core::PluginFinderManager::instance()
{
  static PluginFinderManager inst;

  return inst;
}

te::core::PluginFinderManager::PluginFinderManager()
  : m_pimpl(nullptr)
{
  m_pimpl = new Impl;
}

te::core::PluginFinderManager::~PluginFinderManager()
{
  delete m_pimpl;
}
