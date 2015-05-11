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
  \file PluginEngine.cpp
  
  \brief A plugin engine for plugins written in JavaScript using Google's V8 engine.
 */

// TerraLib
#include "../../../common/Exception.h"
#include "../../../common/Logger.h"
#include "../../../common/Translator.h"
#include "../common/JsContext.h"
#include "../common/JsContextManager.h"
#include "../common/Utils.h"
#include "../jsi/platform/OpenModules.h"
#include "Plugin.h"
#include "PluginEngine.h"

// STL
#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>

// Boost
#include <boost/filesystem.hpp>

// Google V8
#include <v8.h>

te::plugin::AbstractPlugin* te::v8::plugin::PluginEngine::load(const te::plugin::PluginInfo& pInfo) throw(...)
{
// let's create the JavaScript plugin and then find out all information needed to execute it
  std::auto_ptr<Plugin> jsplugin(new Plugin(pInfo));

// find the context to use for plugin
  std::vector<te::plugin::PluginInfo::Parameter>::const_iterator itP = std::find_if(pInfo.m_parameters.begin(),
                                                                                    pInfo.m_parameters.end(),
                                                                                    te::plugin::PluginInfo::Finder1st("CONTEXT_ID"));

  std::string ctxid = "DEFAULT";

  if(itP != pInfo.m_parameters.end())
    ctxid = itP->second;

  te::v8::common::JsContext* ctx = te::v8::common::JsContextManager::getInstance().getCtx(ctxid);

  if(ctx == 0)
  {
// create the new context
    std::auto_ptr<te::v8::common::JsContext> jsctx(new te::v8::common::JsContext);
    te::v8::common::JsContextManager::getInstance().add(ctxid, jsctx.get());
    ctx = jsctx.release();

// and then register the C++ code for JavaScript users!
    ::v8::Persistent<::v8::Context> context = ctx->getCtx();
    ::v8::Context::Scope contextScope(context);
    ::v8::HandleScope handleScope;
    ::v8::Local<::v8::FunctionTemplate> jsPrintFunction = ::v8::FunctionTemplate::New(te::v8::common::Print);
    context->Global()->Set(::v8::String::New("print"), jsPrintFunction->GetFunction());
    te::v8::jsi::RegisterModule(context->Global());
  }

  jsplugin->m_ctx = ctx;

// let's create the required V8 environment in order to run plugin's code
  ::v8::Persistent<::v8::Context> context = ctx->getCtx();
  ::v8::Context::Scope contextScope(context);
  ::v8::HandleScope handleScope;

// the plugin file name must have the path with its relative directory from the application execution path
  std::vector<te::plugin::PluginInfo::Resource>::const_iterator itR = std::find_if(pInfo.m_resources.begin(),
                                                                                   pInfo.m_resources.end(),
                                                                                   te::plugin::PluginInfo::Finder1st("PluginFileName"));

  if(itR == pInfo.m_resources.end())
    throw te::common::Exception(TR_V8PLUGIN("Could not find JavaScript plugin file!"));

  boost::filesystem::path pluginFile(pInfo.m_folder);
  pluginFile /= itR->second;

  ::v8::Handle<::v8::String> source = te::v8::common::ReadFile(pluginFile.file_string());

  ::v8::Handle<::v8::Script> script = ::v8::Script::Compile(source);

  ::v8::TryCatch trycatch;

  ::v8::Handle<::v8::Value> result = script->Run();

  if(result.IsEmpty())
  {
    ::v8::Handle<::v8::Value> e = trycatch.Exception();
    ::v8::String::AsciiValue estr(e);
    throw te::common::Exception(*estr);
  }

// find plugin class name in plugin parameter info
  itP = std::find_if(pInfo.m_parameters.begin(),
                     pInfo.m_parameters.end(),
                     te::plugin::PluginInfo::Finder1st("PLUGIN_CLASS_NAME"));

  if(itP == pInfo.m_parameters.end())
    return jsplugin.release();  // ok: the user doesn't want to execute nothing... just go away!

  const std::string& pluginClassName = itP->second;

// find plugin class in global context and caches its constructor
  ::v8::Local<::v8::String> jsPluginClassName = ::v8::String::New(pluginClassName.c_str());
  ::v8::Local<::v8::Value> jsPluginClass = context->Global()->Get(jsPluginClassName);

  if(jsPluginClass.IsEmpty())
    throw te::common::Exception(TR_V8PLUGIN("Could not find Plugin's constructor using the informed JavaScript code!")); 

  if(!jsPluginClass->IsFunction())
    throw te::common::Exception(TR_V8PLUGIN("Plugin's constructor must be a function!"));

  ::v8::Handle<::v8::Function> jsPluginClassFtor = ::v8::Handle<::v8::Function>::Cast(jsPluginClass);
  jsplugin->m_jspluginFtor = ::v8::Persistent<::v8::Function>::New(jsPluginClassFtor);

// then caches an instance of the plugin
  ::v8::Local<::v8::Object> jsPlugin = jsPluginClassFtor->NewInstance();

  if(jsPlugin.IsEmpty())
    throw te::common::Exception(TR_V8PLUGIN("Could not create a plugin using the informed JavaScript code!")); 

  jsplugin->m_jsplugin = ::v8::Persistent<::v8::Object>::New(jsPlugin);

// cache plugin's startup method
  te::v8::common::Cache(jsPlugin, "startup", jsplugin->m_startup);

// cache plugin's shutdown method
  te::v8::common::Cache(jsPlugin, "shutdown", jsplugin->m_shutdown);

  return jsplugin.release();
}

void te::v8::plugin::PluginEngine::unload(te::plugin::AbstractPlugin* plugin) throw(...)
{
  assert(plugin && plugin->getInfo().m_engine == TE_V8_PLUGIN_ENGINE_CODE);

  delete plugin;
}

