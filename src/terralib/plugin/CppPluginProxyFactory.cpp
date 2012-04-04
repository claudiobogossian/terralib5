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
  \file PluginFactory.cpp

  \brief This is the abstract factory for Plugins.
 */

// TerraLib
#include "../common/Exception.h"
#include "../common/Logger.h"
#include "../common/Translator.h"
#include "CppPlugin.h"
#include "CppPluginProxy.h"
#include "CppPluginProxyFactory.h"

#if TE_PLATFORM == TE_PLATFORMCODE_LINUX || TE_PLATFORM == TE_PLATFORMCODE_APPLE
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#endif

te::plugin::CppPluginProxyFactory* te::plugin::CppPluginProxyFactory::sm_factory(0);

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
static const std::string smi_cppplugin("dll");
#endif

#if TE_PLATFORM == TE_PLATFORMCODE_LINUX
static const std::string smi_cppplugin("s.o.");
#endif

#if TE_PLATFORM == TE_PLATFORMCODE_APPLE
static const std::string smi_cppplugin("dylib");
#endif


void te::plugin::CppPluginProxyFactory::initialize()
{
  finalize();
  sm_factory = new CppPluginProxyFactory;
}

void te::plugin::CppPluginProxyFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

te::plugin::CppPluginProxyFactory::~CppPluginProxyFactory()
{
}

te::plugin::Plugin* te::plugin::CppPluginProxyFactory::build(PluginInfo p)
{
  CppPlugin* plugin = 0;
  CppPluginProxy* proxy = 0; 

  std::string m  = TR_PLUGIN("Trying to start C++ plugin: ");
              m += p.m_name;
              m += ", " + p.m_mainFile;

  TE_LOG_TRACE(m.c_str());

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
// try to get a handle to the DLL module
  HMODULE pluginModule = LoadLibraryA(p.m_mainFile.c_str());

  if(pluginModule == NULL)
  {
    m  = TR_PLUGIN("Could not start C++ plugin: ");
    m +=  p.m_name;
    m += ", " + p.m_mainFile;
    m += ".";

    throw te::common::Exception(m);
  }

// if the handle is valid, try to get the function address
  CppPlugin::GetPluginFPtr getPluginFptr = (CppPlugin::GetPluginFPtr) GetProcAddress(pluginModule, "CppPluginGetInstance");

  if(getPluginFptr == NULL)
  {
    BOOL result = FreeLibrary(pluginModule);

    if(result)
      m  = TR_PLUGIN("Could not find CppPluginGetInstance function into the plugin's code.");
    else
      m  = TR_PLUGIN("Could not find CppPluginGetInstance function into the plugin's code and couldn't free plugin's resources!");

    throw te::common::Exception(m);
  }

  plugin = getPluginFptr(p);

  if(plugin == 0)
  {
    BOOL result = FreeLibrary(pluginModule);

    if(result)
      m  = TR_PLUGIN("CppPluginGetInstance function is NULL.");
    else
      m  = TR_PLUGIN("CppPluginGetInstance function is NULL and couldn't free plugin's resources!");

    throw te::common::Exception(m);
  }

  proxy = new CppPluginProxy(plugin);
  proxy->m_module = pluginModule;
#endif

#if TE_PLATFORM == TE_PLATFORMCODE_LINUX || TE_PLATFORM == TE_PLATFORMCODE_APPLE
// try to get a handle to the S.O. module
  void* pluginModule = dlopen(p.m_mainFile.c_str(), RTLD_NOW);

  if(pluginModule == 0)
  {
    const char* errmsg = dlerror();

    m  = TR_PLUGIN("Could not start C++ plugin: ");
    m +=  p.m_name;
    m += ", " + p.m_mainFile;
    m += ", due to: ";
    m += errmsg;

    throw te::common::Exception(m);
  }

// if the handle is valid, try to get the function address
  CppPlugin::GetPluginFPtr getPluginFptr = (CppPlugin::GetPluginFPtr) dlsym(pluginModule, "CppPluginGetInstance");

  if(getPluginFptr == NULL)
  {
    int result = dlclose(pluginModule);

    if(result == 0)
      m  = TR_PLUGIN("Could not find CppPluginGetInstance function into the plugin's code.");
    else
      m  = TR_PLUGIN("Could not find CppPluginGetInstance function into the plugin's code and couldn't free plugin's resources!");

    throw te::common::Exception(m);
  }

  plugin = getPluginFptr(p);

  if(plugin == 0)
  {
    int result = dlclose(pluginModule);

    if(result == 0)
      m  = TR_PLUGIN("CppPluginGetInstance function is NULL.");
    else
      m  = TR_PLUGIN("CppPluginGetInstance function is NULL and couldn't free plugin's resources!");

    throw te::common::Exception(m);
  }

  proxy = new CppPluginProxy(plugin);
  proxy->m_module = pluginModule;
#endif

  m  = TR_PLUGIN("C++ plugin: ");
  m += p.m_name;
  m += "," + p.m_mainFile;
  m += TR_PLUGIN(" - loaded!");

  TE_LOG_TRACE(m.c_str());

  return proxy;
}

te::plugin::CppPluginProxyFactory::CppPluginProxyFactory()
  : PluginFactory(smi_cppplugin)
{
}

