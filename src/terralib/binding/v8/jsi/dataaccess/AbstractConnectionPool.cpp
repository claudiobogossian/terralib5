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
  \file AbstractConnectionPool.cpp
   
  \brief Utility functions to register the AbstractConnectionPool class into Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../dataaccess/datasource/AbstractConnectionPool.h"
#include "../../common/Utils.h"
#include "DataAccess.h"

// Boost
#include <boost/cstdint.hpp>

::v8::Handle<::v8::Value> AbstractConnectionPool_Initialize(const ::v8::Arguments& args)
{
  te::da::AbstractConnectionPool* pool = te::v8::common::Unwrap<te::da::AbstractConnectionPool>(args.Holder());

  if(pool == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid connection pool in the initialize method!"));

  pool->initialize();

  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> AbstractConnectionPool_Finalize(const ::v8::Arguments& args)
{
  te::da::AbstractConnectionPool* pool = te::v8::common::Unwrap<te::da::AbstractConnectionPool>(args.Holder());

  if(pool == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid connection pool in the finalize method!"));

  pool->finalize();

  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> AbstractConnectionPool_Idle(const ::v8::Arguments& args)
{
  te::da::AbstractConnectionPool* pool = te::v8::common::Unwrap<te::da::AbstractConnectionPool>(args.Holder());

  if(pool == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid connection pool in the idle method!"));

  pool->idle();

  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> AbstractConnectionPool_IsValid(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  te::da::AbstractConnectionPool* pool = te::v8::common::Unwrap<te::da::AbstractConnectionPool>(args.Holder());

  if(pool == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid connection pool in the isValid method!"));

  bool isValid = pool->isValid();

  ::v8::Handle<::v8::Boolean> jisValid = ::v8::Boolean::New(isValid);

  return hs.Close(jisValid);
}

::v8::Handle<::v8::Value> AbstractConnectionPool_IsInitialized(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  te::da::AbstractConnectionPool* pool = te::v8::common::Unwrap<te::da::AbstractConnectionPool>(args.Holder());

  if(pool == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid connection pool in the isInitialized method!"));

  bool isInitialized = pool->isInitialized();

  ::v8::Handle<::v8::Boolean> jisInitialized = ::v8::Boolean::New(isInitialized);

  return hs.Close(jisInitialized);
}

::v8::Handle<::v8::Value> AbstractConnectionPool_GetPoolSize(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  te::da::AbstractConnectionPool* pool = te::v8::common::Unwrap<te::da::AbstractConnectionPool>(args.Holder());

  if(pool == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid connection pool in the getPoolsize method!"));

  std::size_t poolSize = pool->getPoolSize();

  ::v8::Handle<::v8::Integer> jpoolSize = ::v8::Integer::New(static_cast<boost::int32_t>(poolSize));

  return hs.Close(jpoolSize);
}

::v8::Handle<::v8::Value> AbstractConnectionPool_GetInitialPoolSize(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  te::da::AbstractConnectionPool* pool = te::v8::common::Unwrap<te::da::AbstractConnectionPool>(args.Holder());

  if(pool == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid connection pool in the getInitialPoolsize method!"));

  std::size_t initialPoolSize = pool->getInitialPoolSize();

  ::v8::Handle<::v8::Integer> jinitialPoolSize = ::v8::Integer::New(static_cast<boost::int32_t>(initialPoolSize));

  return hs.Close(jinitialPoolSize);
}

::v8::Handle<::v8::Value> AbstractConnectionPool_SetInitialPoolSize(const ::v8::Arguments& args)
{
  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use the setInitialPoolSize method you must use the object notation: \"obj.setInitialPoolSize(size);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsInt32())
    return ::v8::ThrowException(::v8::String::New("In order to use the setInitialPoolSize method you must use the object notation: \"obj.setInitialPoolSize(size);\""));

  te::da::AbstractConnectionPool* pool = te::v8::common::Unwrap<te::da::AbstractConnectionPool>(args.Holder());

  if(pool == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid connection pool in the setInitialPoolSize method!"));

  int initialPoolSize = args[0]->ToInt32()->Value();
  
  pool->setInitialPoolSize(initialPoolSize);

  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> AbstractConnectionPool_GetMinPoolSize(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  te::da::AbstractConnectionPool* pool = te::v8::common::Unwrap<te::da::AbstractConnectionPool>(args.Holder());

  if(pool == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid connection pool in the getMinPoolsize method!"));

  std::size_t minPoolSize = pool->getMinPoolSize();

  ::v8::Handle<::v8::Integer> jminPoolSize = ::v8::Integer::New(static_cast<boost::int32_t>(minPoolSize));

  return hs.Close(jminPoolSize);
}

::v8::Handle<::v8::Value> AbstractConnectionPool_SetMinPoolSize(const ::v8::Arguments& args)
{
  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use the setMinPoolSize method you must use the object notation: \"obj.setMinPoolSize(size);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsInt32())
    return ::v8::ThrowException(::v8::String::New("In order to use the setMinPoolSize method you must use the object notation: \"obj.setMinPoolSize(size);\""));

  te::da::AbstractConnectionPool* pool = te::v8::common::Unwrap<te::da::AbstractConnectionPool>(args.Holder());

  if(pool == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid connection pool in the setMinPoolSize method!"));

  int minPoolSize = args[0]->ToInt32()->Value();
  
  pool->setMinPoolSize(minPoolSize);

  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> AbstractConnectionPool_GetMaxPoolSize(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  te::da::AbstractConnectionPool* pool = te::v8::common::Unwrap<te::da::AbstractConnectionPool>(args.Holder());

  if(pool == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid connection pool in the getMaxPoolsize method!"));

  std::size_t maxPoolSize = pool->getMaxPoolSize();

  ::v8::Handle<::v8::Integer> jmaxPoolSize = ::v8::Integer::New(static_cast<boost::int32_t>(maxPoolSize));

  return hs.Close(jmaxPoolSize);
}

::v8::Handle<::v8::Value> AbstractConnectionPool_SetMaxPoolSize(const ::v8::Arguments& args)
{
  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use the setMaxPoolSize method you must use the object notation: \"obj.setMaxPoolSize(size);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsInt32())
    return ::v8::ThrowException(::v8::String::New("In order to use the setMaxPoolSize method you must use the object notation: \"obj.setMaxPoolSize(size);\""));

  te::da::AbstractConnectionPool* pool = te::v8::common::Unwrap<te::da::AbstractConnectionPool>(args.Holder());

  if(pool == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid connection pool in the setMaxPoolSize method!"));

  int maxPoolSize = args[0]->ToInt32()->Value();
  
  pool->setMaxPoolSize(maxPoolSize);

  return ::v8::Undefined();
}

static ::v8::Persistent<::v8::FunctionTemplate> sg_abstractconnectionpool_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetAbstractConnectionPoolTemplate()
{
  if(sg_abstractconnectionpool_template.IsEmpty())
  {
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("initialize"), ::v8::FunctionTemplate::New(AbstractConnectionPool_Initialize));
    prototype->Set(::v8::String::NewSymbol("finalize"), ::v8::FunctionTemplate::New(AbstractConnectionPool_Finalize));
    prototype->Set(::v8::String::NewSymbol("idle"), ::v8::FunctionTemplate::New(AbstractConnectionPool_Idle));
    prototype->Set(::v8::String::NewSymbol("isValid"), ::v8::FunctionTemplate::New(AbstractConnectionPool_IsValid));
    prototype->Set(::v8::String::NewSymbol("isInitialized"), ::v8::FunctionTemplate::New(AbstractConnectionPool_IsInitialized));
    prototype->Set(::v8::String::NewSymbol("getPoolSize"), ::v8::FunctionTemplate::New(AbstractConnectionPool_GetPoolSize));
    prototype->Set(::v8::String::NewSymbol("getInitialPoolSize"), ::v8::FunctionTemplate::New(AbstractConnectionPool_GetInitialPoolSize));
    prototype->Set(::v8::String::NewSymbol("setInitialPoolSize"), ::v8::FunctionTemplate::New(AbstractConnectionPool_SetInitialPoolSize));
    prototype->Set(::v8::String::NewSymbol("getMinPoolSize"), ::v8::FunctionTemplate::New(AbstractConnectionPool_GetMinPoolSize));
    prototype->Set(::v8::String::NewSymbol("setMinPoolSize"), ::v8::FunctionTemplate::New(AbstractConnectionPool_SetMinPoolSize));
    prototype->Set(::v8::String::NewSymbol("getMaxPoolSize"), ::v8::FunctionTemplate::New(AbstractConnectionPool_GetMaxPoolSize));
    prototype->Set(::v8::String::NewSymbol("setMaxPoolSize"), ::v8::FunctionTemplate::New(AbstractConnectionPool_SetMaxPoolSize));

    sg_abstractconnectionpool_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sg_abstractconnectionpool_template;
}

