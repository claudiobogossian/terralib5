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
  \file BatchExecutor.cpp
   
  \brief Utility functions to register the BatchExecutor class into Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../dataaccess/datasource/BatchExecutor.h"
#include "../../common/Utils.h"
#include "DataAccess.h"

::v8::Handle<::v8::Value> BatchExecutor_Add(const ::v8::Arguments& /*args*/)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> BatchExecutor_Execute(const ::v8::Arguments& args)
{
  te::da::BatchExecutor* bexec = te::v8::common::Unwrap<te::da::BatchExecutor>(args.Holder());

  if(bexec == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid batch executor in the execute method!"));

  bexec->execute();

  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> BatchExecutor_GetTransactor(const ::v8::Arguments& /*args*/)
{
  return ::v8::Undefined();
}

static ::v8::Persistent<::v8::FunctionTemplate> sg_batchexecutor_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetBatchExecutorTemplate()
{
  if(sg_batchexecutor_template.IsEmpty())
  {
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("add"), ::v8::FunctionTemplate::New(BatchExecutor_Add));
    prototype->Set(::v8::String::NewSymbol("execute"), ::v8::FunctionTemplate::New(BatchExecutor_Execute));
    prototype->Set(::v8::String::NewSymbol("getTransactor"), ::v8::FunctionTemplate::New(BatchExecutor_GetTransactor));

    sg_batchexecutor_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sg_batchexecutor_template;
}

