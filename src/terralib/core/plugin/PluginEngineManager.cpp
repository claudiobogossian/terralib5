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
  \file terralib/core/plugin/PluginEngineManager.cpp

  \brief A singleton that can be used to register plugin engines.

  \author Gilberto Ribeiro de Queiroz
  \author Matheus Cavassan Zaglia
 */

// TerraLib
#include "PluginEngineManager.h"
#include "../translator/Translator.h"
#include "AbstractPluginEngine.h"
#include "Exception.h"

// Boost
#include <boost/format.hpp>

struct te::core::PluginEngineManager::Impl
{
  std::map<std::string, std::shared_ptr<AbstractPluginEngine> > engines;
};


void
te::core::PluginEngineManager::insert(std::unique_ptr<AbstractPluginEngine> engine)
{
  if(engine == nullptr)
    throw te::InvalidArgumentException() << ErrorDescription(TE_TR("Attempting to register a null plugin engine in PluginEngineManager."));

  const std::string& engine_id = engine->id();

  if(m_pimpl->engines.find(engine_id) != m_pimpl->engines.end())
  {
    boost::format err_msg(TE_TR("There is already a plugin engine registered with name: %1%."));

    throw InvalidArgumentException() << ErrorDescription((err_msg % engine_id).str());
  }

  std::shared_ptr<AbstractPluginEngine> new_engine(engine.release());

  m_pimpl->engines.insert(std::make_pair(engine_id, new_engine));
}

void
te::core::PluginEngineManager::remove(const std::string& engine_id)
{
  std::map<std::string, std::shared_ptr<AbstractPluginEngine> >::iterator it = m_pimpl->engines.find(engine_id);

  if(it == m_pimpl->engines.end())
  {
    boost::format err_msg(TE_TR("Could not remove plugin engine '%1%': not found."));

    throw OutOfRangeException() << ErrorDescription((err_msg % engine_id).str());
  }
  
  m_pimpl->engines.erase(it);
}

te::core::AbstractPluginEngine&
te::core::PluginEngineManager::get(const std::string& engine_id) const
{
  std::map<std::string, std::shared_ptr<AbstractPluginEngine> >::iterator it = m_pimpl->engines.find(engine_id);

  if(it == m_pimpl->engines.end())
  {
    boost::format err_msg(TE_TR("Could not retrieve plugin engine '%1%': not found."));

    throw OutOfRangeException() << ErrorDescription((err_msg % engine_id).str());
  }

  return *(it->second);
}

te::core::PluginEngineManager&
te::core::PluginEngineManager::instance()
{
  static PluginEngineManager inst;

  return inst;
}

te::core::PluginEngineManager::PluginEngineManager()
  : m_pimpl(nullptr)
{
  m_pimpl = new Impl;
}

te::core::PluginEngineManager::~PluginEngineManager()
{
  delete m_pimpl;
}
