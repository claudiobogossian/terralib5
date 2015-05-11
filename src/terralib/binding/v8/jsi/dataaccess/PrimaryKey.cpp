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
  \file PrimaryKey.cpp
   
  \brief Utility functions to register the PrimaryKey class into the Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../dataaccess/dataset/PrimaryKey.h"
#include "../../common/Utils.h"
#include "DataAccess.h"

// Boost
#include <boost/cstdint.hpp>

::v8::Handle<::v8::Value> PrimaryKey_GetProperties(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> PrimaryKey_SetProperties(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> PrimaryKey_Add(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> PrimaryKey_SetAssociatedIndex(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> PrimaryKey_GetAssociatedIndex(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> PrimaryKey_Has(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> PrimaryKey_Replace(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> PrimaryKey_GetType(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> PrimaryKey_Clone(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use the clone method, you must use the object notation: \"pk = obj.clone();\""));

  te::da::PrimaryKey* pk = te::v8::common::Unwrap<te::da::PrimaryKey>(args.Holder());

  if(pk == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid primary key in the clone method!"));

  te::da::Constraint* pkclone = pk->clone();

  ::v8::Local<::v8::Object> jpkclone = te::v8::common::Make(pkclone, te::v8::jsi::GetPrimaryKeyTemplate, true);

  return hs.Close(jpkclone);
}

::v8::Handle<::v8::Value> PrimaryKey_Constructor(const ::v8::Arguments& args)
{
  //::v8::HandleScope hs;

  //if(!args.IsConstructCall())
  //  return ::v8::ThrowException(::v8::String::New("In order to create a check constraint, you need to call its constructor like: var c = new TePrimaryKey(dt, id);"));

  //if(args.Holder().IsEmpty())
  //  return ::v8::ThrowException(::v8::String::New("The PrimaryKey constructor must use object notation!"));

  //if(args.Length() != 0)
  //  return ::v8::ThrowException(::v8::String::New("The PrimaryKey constructor has no parameters!"));

  //std::auto_ptr<te::da::PrimaryKey> catalog(new te::da::PrimaryKey());

  //::v8::Local<::v8::Object> jcatalog = te::v8::common::Make(catalog.get(), te::v8::jsi::GetPrimaryKeyTemplate, true);

  //catalog.release();

  //return hs.Close(jcatalog);

  return ::v8::Undefined();
}

void te::v8::jsi::RegisterPrimaryKey(::v8::Local<::v8::Object>& global)
{
  ::v8::Local<::v8::FunctionTemplate> jpk = ::v8::FunctionTemplate::New(PrimaryKey_Constructor);

  global->Set(::v8::String::New("TePrimaryKey"), jpk->GetFunction());
}

static ::v8::Persistent<::v8::FunctionTemplate> sdset_primarykey_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetPrimaryKeyTemplate()
{
  if(sdset_primarykey_template.IsEmpty())
  {
    ::v8::Persistent<::v8::FunctionTemplate>& cTpl = GetConstraintTemplate();
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    result->Inherit(cTpl);

    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("getProperties"), ::v8::FunctionTemplate::New(PrimaryKey_GetProperties));
    prototype->Set(::v8::String::NewSymbol("setProperties"), ::v8::FunctionTemplate::New(PrimaryKey_SetProperties));
    prototype->Set(::v8::String::NewSymbol("add"), ::v8::FunctionTemplate::New(PrimaryKey_Add));
    prototype->Set(::v8::String::NewSymbol("setAssociatedIndex"), ::v8::FunctionTemplate::New(PrimaryKey_SetAssociatedIndex));
    prototype->Set(::v8::String::NewSymbol("getAssociatedIndex"), ::v8::FunctionTemplate::New(PrimaryKey_GetAssociatedIndex));
    prototype->Set(::v8::String::NewSymbol("has"), ::v8::FunctionTemplate::New(PrimaryKey_Has));
    prototype->Set(::v8::String::NewSymbol("replace"), ::v8::FunctionTemplate::New(PrimaryKey_Replace));
    prototype->Set(::v8::String::NewSymbol("getType"), ::v8::FunctionTemplate::New(PrimaryKey_GetType));
    prototype->Set(::v8::String::NewSymbol("clone"), ::v8::FunctionTemplate::New(PrimaryKey_Clone));

    sdset_primarykey_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sdset_primarykey_template;
}
