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
  \file Plugin.cpp
  
  \brief A class that handles JavaScript Plugins using Google's V8 engine.
 */

// TerraLib
#include "../../../common/Exception.h"
#include "../common/JsContext.h"
#include "Plugin.h"

te::v8::plugin::Plugin::Plugin(const te::plugin::PluginInfo& pInfo)
  : te::plugin::Plugin(pInfo),
    m_ctx(0)
{
}

te::v8::plugin::Plugin::~Plugin()
{
  m_startup.Dispose();
  m_shutdown.Dispose();
  m_jsplugin.Dispose();
  m_jspluginFtor.Dispose();
}

void te::v8::plugin::Plugin::startup() throw(...) 
{
  if(m_initialized || m_jsplugin.IsEmpty() || m_startup.IsEmpty() || (m_ctx == 0))
    return;

  ::v8::HandleScope handleScope;

  ::v8::TryCatch trycatch;

  ::v8::Handle<::v8::Value> result = m_startup->Call(m_jsplugin, 0, 0);

  if(result.IsEmpty())
  {
    ::v8::Handle<::v8::Value> e = trycatch.Exception();
    ::v8::String::AsciiValue estr(e);
    throw te::common::Exception(*estr);
  }

  m_initialized = true;
}

void te::v8::plugin::Plugin::shutdown() throw(...) 
{
  if(!m_initialized || m_jsplugin.IsEmpty() || m_shutdown.IsEmpty() || (m_ctx == 0))
    return;

  ::v8::HandleScope handleScope;

  ::v8::TryCatch trycatch;

  ::v8::Handle<::v8::Value> result = m_shutdown->Call(m_jsplugin, 0, 0);

  if(result.IsEmpty())
  {
    ::v8::Handle<::v8::Value> e = trycatch.Exception();
    ::v8::String::AsciiValue estr(e);
    throw te::common::Exception(*estr);
  }

  m_initialized = false;
}

