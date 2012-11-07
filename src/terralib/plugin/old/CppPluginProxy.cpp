/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file CppPluginProxy.cpp
  
  \brief An abstract interface for Plugins written in C++.
 */

// TerraLib
#include "CppPlugin.h"
#include "CppPluginProxy.h"

#if TE_PLATFORM == TE_PLATFORMCODE_LINUX || TE_PLATFORM == TE_PLATFORMCODE_APPLE
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#endif

te::plugin::CppPluginProxy::CppPluginProxy(CppPlugin* plugin)
  : m_plugin(plugin)
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
    , m_module(NULL)
#endif
#if TE_PLATFORM == TE_PLATFORMCODE_LINUX || TE_PLATFORM == TE_PLATFORMCODE_APPLE
    , m_module(NULL)
#endif
{
}

te::plugin::CppPluginProxy::~CppPluginProxy()
{
  delete m_plugin;

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
  if(m_module)
    FreeLibrary(m_module);
#endif

#if TE_PLATFORM == TE_PLATFORMCODE_LINUX || TE_PLATFORM == TE_PLATFORMCODE_APPLE
  if(m_module)
    dlclose(m_module);
#endif


}

void te::plugin::CppPluginProxy::startup()
{
  m_plugin->startup();
}

void te::plugin::CppPluginProxy::shutdown()
{
  m_plugin->shutdown();
}

const te::plugin::PluginInfo& te::plugin::CppPluginProxy::getInfo() const
{
  return m_plugin->getInfo();
}

