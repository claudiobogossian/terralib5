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
  \file UniqueKey.cpp
   
  \brief Utility functions to register the UniqueKey class into the Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../dataaccess/dataset/UniqueKey.h"
#include "../../common/Utils.h"
#include "DataAccess.h"

// Boost
#include <boost/cstdint.hpp>

::v8::Handle<::v8::Value> UniqueKey_GetProperties(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> UniqueKey_SetProperties(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> UniqueKey_Add(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> UniqueKey_GetAssociatedIndex(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> UniqueKey_SetAssociatedIndex(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> UniqueKey_Replace(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> UniqueKey_GetType(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> UniqueKey_Clone(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use the clone method, you must use the object notation: \"uk = obj.clone();\""));

  te::da::UniqueKey* uk = te::v8::common::Unwrap<te::da::UniqueKey>(args.Holder());

  if(uk == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid unique key in the clone method!"));

  te::da::Constraint* ukclone = uk->clone();

  ::v8::Local<::v8::Object> jukclone = te::v8::common::Make(ukclone, te::v8::jsi::GetUniqueKeyTemplate, true);

  return hs.Close(jukclone);
}

::v8::Handle<::v8::Value> UniqueKey_Constructor(const ::v8::Arguments& args)
{
  //::v8::HandleScope hs;

  //if(!args.IsConstructCall())
  //  return ::v8::ThrowException(::v8::String::New("In order to create a check constraint, you need to call its constructor like: var c = new TeUniqueKey(dt, id);"));

  //if(args.Holder().IsEmpty())
  //  return ::v8::ThrowException(::v8::String::New("The UniqueKey constructor must use object notation!"));

  //if(args.Length() != 0)
  //  return ::v8::ThrowException(::v8::String::New("The UniqueKey constructor has no parameters!"));

  //std::auto_ptr<te::da::UniqueKey> catalog(new te::da::UniqueKey());

  //::v8::Local<::v8::Object> jcatalog = te::v8::common::Make(catalog.get(), te::v8::jsi::GetUniqueKeyTemplate, true);

  //catalog.release();

  //return hs.Close(jcatalog);

  return ::v8::Undefined();
}

void te::v8::jsi::RegisterUniqueKey(::v8::Local<::v8::Object>& global)
{
  ::v8::Local<::v8::FunctionTemplate> juk = ::v8::FunctionTemplate::New(UniqueKey_Constructor);

  global->Set(::v8::String::New("TeUniqueKey"), juk->GetFunction());
}

static ::v8::Persistent<::v8::FunctionTemplate> sdset_uniquekey_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetUniqueKeyTemplate()
{
  if(sdset_uniquekey_template.IsEmpty())
  {
    ::v8::Persistent<::v8::FunctionTemplate>& cTpl = GetConstraintTemplate();
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    result->Inherit(cTpl);

    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("getProperties"), ::v8::FunctionTemplate::New(UniqueKey_GetProperties));
    prototype->Set(::v8::String::NewSymbol("setProperties"), ::v8::FunctionTemplate::New(UniqueKey_SetProperties));
    prototype->Set(::v8::String::NewSymbol("add"), ::v8::FunctionTemplate::New(UniqueKey_Add));
    prototype->Set(::v8::String::NewSymbol("getAssociatedIndex"), ::v8::FunctionTemplate::New(UniqueKey_GetAssociatedIndex));
    prototype->Set(::v8::String::NewSymbol("setAssociatedIndex"), ::v8::FunctionTemplate::New(UniqueKey_SetAssociatedIndex));
    prototype->Set(::v8::String::NewSymbol("replace"), ::v8::FunctionTemplate::New(UniqueKey_Replace));
    prototype->Set(::v8::String::NewSymbol("getType"), ::v8::FunctionTemplate::New(UniqueKey_GetType));
    prototype->Set(::v8::String::NewSymbol("clone"), ::v8::FunctionTemplate::New(UniqueKey_Clone));

    sdset_uniquekey_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sdset_uniquekey_template;
}
