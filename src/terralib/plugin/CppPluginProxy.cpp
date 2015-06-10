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
  \file terralib/plugin/CppPluginProxy.cpp

  \brief An abstract interface for Plugins written in C++.
*/

// TerraLib
#include "CppPluginProxy.h"
#include "Plugin.h"

te::plugin::CppPluginProxy::CppPluginProxy(const te::common::LibraryPtr& lib, Plugin* plugin)
  : m_lib(lib),
    m_plugin(plugin)
{
}

te::plugin::CppPluginProxy::~CppPluginProxy()
{
// if we need to assure that plugin goes out of scope
// before the library, we will need to remove the following comment
//  m_plugin.release();
}

const te::plugin::PluginInfo& te::plugin::CppPluginProxy::getInfo() const
{
  return m_plugin->getInfo();
}

bool te::plugin::CppPluginProxy::isStarted() const
{
  return m_plugin->isStarted();
}

void te::plugin::CppPluginProxy::startup()
{
  m_plugin->startup();
}

void te::plugin::CppPluginProxy::shutdown()
{
  m_plugin->shutdown();
}


