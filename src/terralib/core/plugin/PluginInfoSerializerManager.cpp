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
  \file terralib/core/plugin/PluginInfoSerializerManager.cpp

  \brief A singleton for managing PluginInfo serializers.

  \author Gilberto Ribeiro de Queiroz
  \author Matheus Cavassan Zaglia
 */

// TerraLib
#include "PluginInfoSerializerManager.h"
#include "../translator/Translator.h"
#include "Exception.h"

// STL
#include <map>

// Boost
#include <boost/format.hpp>

struct te::core::PluginInfoSerializerManager::Impl
{
  std::map<std::string, PluginInfoSerializerFnct> serializers;
};

std::vector<std::string>
te::core::PluginInfoSerializerManager::getSerializers() const
{
  typedef std::map<std::string, PluginInfoSerializerFnct>::value_type serializer_t;

  std::vector<std::string> serializers;

  for(const serializer_t& ps : m_pimpl->serializers)
    serializers.push_back(ps.first);
  
  return serializers;
}

te::core::PluginInfoSerializerFnct&
te::core::PluginInfoSerializerManager::get(const std::string& serializer_name)
{
  std::map<std::string, PluginInfoSerializerFnct>::iterator it = m_pimpl->serializers.find(serializer_name);

  if(it == m_pimpl->serializers.end())
  {
    boost::format err_msg(TE_TR("Could not find plugin serializer: '%1%'."));

    throw OutOfRangeException() << ErrorDescription((err_msg % serializer_name).str());
  }

  return it->second;
}

void
te::core::PluginInfoSerializerManager::insert(const std::string& serializer_name,
                                                PluginInfoSerializerFnct serializer)
{
  if(exists(serializer_name))
  {
    boost::format err_msg(TE_TR("There is already a plugin serializer registered with the name: '%1%'"));
    
    throw InvalidArgumentException() << ErrorDescription((err_msg % serializer_name).str());
  }
  
  m_pimpl->serializers.insert(std::make_pair(serializer_name, serializer));
}

void
te::core::PluginInfoSerializerManager::remove(const std::string& serializer_name)
{
  std::map<std::string, PluginInfoSerializerFnct>::iterator it = m_pimpl->serializers.find(serializer_name);
  
  if(it == m_pimpl->serializers.end())
  {
    boost::format err_msg(TE_TR("Could not find plugin serializer: '%1%', in order to remove it."));

    throw OutOfRangeException() << ErrorDescription((err_msg % serializer_name).str());
  }

  m_pimpl->serializers.erase(it);
}

bool
te::core::PluginInfoSerializerManager::exists(const std::string& serializer_name) const
{
  return (m_pimpl->serializers.find(serializer_name) != m_pimpl->serializers.end());
}

te::core::PluginInfoSerializerManager&
te::core::PluginInfoSerializerManager::instance()
{
  static PluginInfoSerializerManager inst;
  
  return inst;
}

te::core::PluginInfoSerializerManager::PluginInfoSerializerManager()
  : m_pimpl(nullptr)
{
  m_pimpl = new Impl;
}

te::core::PluginInfoSerializerManager::~PluginInfoSerializerManager()
{
  delete m_pimpl;
}
