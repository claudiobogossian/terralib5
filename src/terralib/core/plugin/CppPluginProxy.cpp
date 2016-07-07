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
  \file terralib/core/plugin/CppPluginProxy.cpp

  \brief A proxy class for C++ plugins.

  \author Gilberto Ribeiro de Queiroz
  \author Matheus Cavassan Zaglia
 */

// TerraLib
#include "CppPluginProxy.h"
#include "../lib/Library.h"
#include "CppPlugin.h"

struct te::core::CppPluginProxy::Impl
{
  std::shared_ptr<Library> slib;
  std::unique_ptr<CppPlugin> real_plugin;
};

te::core::CppPluginProxy::CppPluginProxy(const std::shared_ptr<Library>& slib,
                                           std::unique_ptr<CppPlugin> real_plugin)
  : m_pimpl(nullptr)
{
  m_pimpl = new Impl;
  m_pimpl->slib = slib;
  m_pimpl->real_plugin = std::move(real_plugin);
}

te::core::CppPluginProxy::~CppPluginProxy()
{
  delete m_pimpl;
}

const te::core::PluginInfo&
te::core::CppPluginProxy::info() const
{
  return m_pimpl->real_plugin->info();
}

bool
te::core::CppPluginProxy::initialized() const
{
  return m_pimpl->real_plugin->initialized();
}

void
te::core::CppPluginProxy::startup()
{
  m_pimpl->real_plugin->startup();
}

void
te::core::CppPluginProxy::shutdown()
{
  m_pimpl->real_plugin->shutdown();
}
