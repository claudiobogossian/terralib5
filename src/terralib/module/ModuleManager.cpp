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
  \file terralib/module/ModuleManager.cpp

  \brief A singleton for managing modules.

  \author Gilberto Ribeiro de Queiroz
  \author Frederico Augusto Bede
 */

// TerraLib
#include "ModuleManager.h"
#include "Module.h"
#include "Exception.h"

// STL
#include <algorithm>
#include <map>

// Boost
#include <boost/format.hpp>

struct te::module::ModuleManager::Impl
{
  std::vector<std::shared_ptr<Module> > modules;    //!< The list of managed modules: this will be needed to unload accordingly the modules!
};

std::vector<std::string>
te::module::ModuleManager::listModules() const
{
  std::vector<std::string> modules;
  
  for(const std::shared_ptr<Module>& m : m_pimpl->modules)
  {
    modules.push_back(m->info.name);
  }

  return modules;
}

std::shared_ptr<te::module::Module>
te::module::ModuleManager::getModule(const std::string& name) const
{
  std::vector<std::shared_ptr<Module> >::const_iterator it = std::find_if(m_pimpl->modules.begin(), m_pimpl->modules.end(), [&name](const std::shared_ptr<te::module::Module>& m) { return m->info.name == name;} );

  if(it == m_pimpl->modules.end())
    throw te::ItemNotFoundException() << te::ErrorDescription((boost::format("could not find module: '%1%'.") % name).str());

  return *it;
}

bool
te::module::ModuleManager::exists(const std::string& module_name) const
{
  return true;
}

void
te::module::ModuleManager::insert(const std::shared_ptr<Module>& m)
{

}

void
te::module::ModuleManager::remove(const std::string& module_name)
{
}

te::module::ModuleManager::ModuleManager()
  : m_pimpl(nullptr)
{
  m_pimpl = new Impl;
}

te::module::ModuleManager::~ModuleManager()
{
  delete m_pimpl;
}

