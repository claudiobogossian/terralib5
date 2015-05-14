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
  \file Index.cpp
   
  \brief Utility functions to register the Index class into the Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../dataaccess/dataset/Index.h"
#include "../../common/Utils.h"
#include "DataAccess.h"

// Boost
#include <boost/cstdint.hpp>

::v8::Handle<::v8::Value> Index_GetId(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Index_SetId(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Index_GetName(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Index_SetName(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Index_GetIndexType(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Index_SetIndexType(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Index_GetProperties(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Index_SetProperties(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Index_Add(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Index_GetDataSetType(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Index_SetDataSetType(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}


::v8::Handle<::v8::Value> Index_Has(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Index_Replace(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Index_Constructor(const ::v8::Arguments& args)
{
  //::v8::HandleScope hs;

  //if(!args.IsConstructCall())
  //  return ::v8::ThrowException(::v8::String::New("In order to create a check constraint, you need to call its constructor like: var c = new TeIndex(dt, id);"));

  //if(args.Holder().IsEmpty())
  //  return ::v8::ThrowException(::v8::String::New("The Index constructor must use object notation!"));

  //if(args.Length() != 0)
  //  return ::v8::ThrowException(::v8::String::New("The Index constructor has no parameters!"));

  //std::auto_ptr<te::da::Index> catalog(new te::da::Index());

  //::v8::Local<::v8::Object> jcatalog = te::v8::common::Make(catalog.get(), te::v8::jsi::GetIndexTemplate, true);

  //catalog.release();

  //return hs.Close(jcatalog);

  return ::v8::Undefined();
}

void te::v8::jsi::RegisterIndex(::v8::Local<::v8::Object>& global)
{
  ::v8::Local<::v8::FunctionTemplate> jindex = ::v8::FunctionTemplate::New(Index_Constructor);

  global->Set(::v8::String::New("TeIndex"), jindex->GetFunction());
}

static ::v8::Persistent<::v8::FunctionTemplate> sdset_index_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetIndexTemplate()
{
  if(sdset_index_template.IsEmpty())
  {
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("getId"), ::v8::FunctionTemplate::New(Index_GetId));
    prototype->Set(::v8::String::NewSymbol("setId"), ::v8::FunctionTemplate::New(Index_SetId));
    prototype->Set(::v8::String::NewSymbol("getName"), ::v8::FunctionTemplate::New(Index_GetName));
    prototype->Set(::v8::String::NewSymbol("setName"), ::v8::FunctionTemplate::New(Index_SetName));
    prototype->Set(::v8::String::NewSymbol("getIndexType"), ::v8::FunctionTemplate::New(Index_GetIndexType));
    prototype->Set(::v8::String::NewSymbol("setIndexType"), ::v8::FunctionTemplate::New(Index_SetIndexType));
    prototype->Set(::v8::String::NewSymbol("getProperties"), ::v8::FunctionTemplate::New(Index_GetProperties));
    prototype->Set(::v8::String::NewSymbol("setProperties"), ::v8::FunctionTemplate::New(Index_SetProperties));
    prototype->Set(::v8::String::NewSymbol("add"), ::v8::FunctionTemplate::New(Index_Add));
    prototype->Set(::v8::String::NewSymbol("getDataSetType"), ::v8::FunctionTemplate::New(Index_GetDataSetType));
    prototype->Set(::v8::String::NewSymbol("setDataSetType"), ::v8::FunctionTemplate::New(Index_SetDataSetType));
    prototype->Set(::v8::String::NewSymbol("has"), ::v8::FunctionTemplate::New(Index_Has));
    prototype->Set(::v8::String::NewSymbol("replace"), ::v8::FunctionTemplate::New(Index_Replace));

    sdset_index_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sdset_index_template;
}
