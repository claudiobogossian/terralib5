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
  \file PreparedQuery.cpp
   
  \brief Utility functions to register the PreparedQuery class into Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../dataaccess/datasource/PreparedQuery.h"
#include "DataAccess.h"

::v8::Handle<::v8::Value> PreparedQuery_GetName(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> PreparedQuery_Prepare(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> PreparedQuery_Execute(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> PreparedQuery_Query(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> PreparedQuery_GetTransactor(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

static ::v8::Persistent<::v8::FunctionTemplate> sds_preparedstatement_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetPreparedQueryTemplate()
{
  if(sds_preparedstatement_template.IsEmpty())
  {
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("getName"), ::v8::FunctionTemplate::New(PreparedQuery_GetName));
    prototype->Set(::v8::String::NewSymbol("prepare"), ::v8::FunctionTemplate::New(PreparedQuery_Prepare));
    prototype->Set(::v8::String::NewSymbol("execute"), ::v8::FunctionTemplate::New(PreparedQuery_Execute));
    prototype->Set(::v8::String::NewSymbol("query"), ::v8::FunctionTemplate::New(PreparedQuery_Query));
    prototype->Set(::v8::String::NewSymbol("getTransactor"), ::v8::FunctionTemplate::New(PreparedQuery_GetTransactor));

    sds_preparedstatement_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sds_preparedstatement_template;
}

