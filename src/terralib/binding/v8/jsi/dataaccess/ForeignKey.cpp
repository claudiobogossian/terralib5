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
  \file ForeignKey.cpp
   
  \brief Utility functions to register the ForeignKey class into the Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../dataaccess/dataset/ForeignKey.h"
#include "../../common/Utils.h"
#include "DataAccess.h"

// Boost
#include <boost/cstdint.hpp>

::v8::Handle<::v8::Value> ForeignKey_GetProperties(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> ForeignKey_Add(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> ForeignKey_SetProperties(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> ForeignKey_GetReferencedProperties(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> ForeignKey_AddRefProperty(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> ForeignKey_SetReferencedProperties(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> ForeignKey_GetReferencedDataSetType(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> ForeignKey_SetReferencedDataSetType(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> ForeignKey_GetOnDeleteAction(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> ForeignKey_SetOnDeleteAction(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> ForeignKey_GetOnUpdateAction(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> ForeignKey_SetOnUpdateAction(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> ForeignKey_Has(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> ForeignKey_IsReferenced(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> ForeignKey_Replace(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> ForeignKey_GetType(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> ForeignKey_Clone(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use the clone method, you must use the object notation: \"fk = obj.clone();\""));

  te::da::ForeignKey* fk = te::v8::common::Unwrap<te::da::ForeignKey>(args.Holder());

  if(fk == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid foreign key in the clone method!"));

  te::da::Constraint* fkclone = fk->clone();

  ::v8::Local<::v8::Object> jfkclone = te::v8::common::Make(fkclone, te::v8::jsi::GetForeignKeyTemplate, true);

  return hs.Close(jfkclone);
}

::v8::Handle<::v8::Value> ForeignKey_Constructor(const ::v8::Arguments& args)
{
  //::v8::HandleScope hs;

  //if(!args.IsConstructCall())
  //  return ::v8::ThrowException(::v8::String::New("In order to create a check constraint, you need to call its constructor like: var c = new TeForeignKey(dt, id);"));

  //if(args.Holder().IsEmpty())
  //  return ::v8::ThrowException(::v8::String::New("The ForeignKey constructor must use object notation!"));

  //if(args.Length() != 0)
  //  return ::v8::ThrowException(::v8::String::New("The ForeignKey constructor has no parameters!"));

  //std::auto_ptr<te::da::ForeignKey> catalog(new te::da::ForeignKey());

  //::v8::Local<::v8::Object> jcatalog = te::v8::common::Make(catalog.get(), te::v8::jsi::GetForeignKeyTemplate, true);

  //catalog.release();

  //return hs.Close(jcatalog);

  return ::v8::Undefined();
}

void te::v8::jsi::RegisterForeignKey(::v8::Local<::v8::Object>& global)
{
  ::v8::Local<::v8::FunctionTemplate> jfk = ::v8::FunctionTemplate::New(ForeignKey_Constructor);

  global->Set(::v8::String::New("TeForeignKey"), jfk->GetFunction());
}

static ::v8::Persistent<::v8::FunctionTemplate> sdset_foreignkey_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetForeignKeyTemplate()
{
  if(sdset_foreignkey_template.IsEmpty())
  {
    ::v8::Persistent<::v8::FunctionTemplate>& cTpl = GetConstraintTemplate();
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    result->Inherit(cTpl);

    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("getProperties"), ::v8::FunctionTemplate::New(ForeignKey_GetProperties));
    prototype->Set(::v8::String::NewSymbol("add"), ::v8::FunctionTemplate::New(ForeignKey_Add));
    prototype->Set(::v8::String::NewSymbol("setProperties"), ::v8::FunctionTemplate::New(ForeignKey_SetProperties));
    prototype->Set(::v8::String::NewSymbol("getReferencedProperties"), ::v8::FunctionTemplate::New(ForeignKey_GetReferencedProperties));
    prototype->Set(::v8::String::NewSymbol("addRefProperty"), ::v8::FunctionTemplate::New(ForeignKey_AddRefProperty));
    prototype->Set(::v8::String::NewSymbol("setReferencedProperties"), ::v8::FunctionTemplate::New(ForeignKey_SetReferencedProperties));
    prototype->Set(::v8::String::NewSymbol("getReferencedDataSetType"), ::v8::FunctionTemplate::New(ForeignKey_GetReferencedDataSetType));
    prototype->Set(::v8::String::NewSymbol("setReferencedDataSetType"), ::v8::FunctionTemplate::New(ForeignKey_SetReferencedDataSetType));
    prototype->Set(::v8::String::NewSymbol("getOnDeleteAction"), ::v8::FunctionTemplate::New(ForeignKey_GetOnDeleteAction));
    prototype->Set(::v8::String::NewSymbol("setOnDeleteAction"), ::v8::FunctionTemplate::New(ForeignKey_SetOnDeleteAction));
    prototype->Set(::v8::String::NewSymbol("getOnUpdateAction"), ::v8::FunctionTemplate::New(ForeignKey_GetOnUpdateAction));
    prototype->Set(::v8::String::NewSymbol("setOnUpdateAction"), ::v8::FunctionTemplate::New(ForeignKey_SetOnUpdateAction));
    prototype->Set(::v8::String::NewSymbol("has"), ::v8::FunctionTemplate::New(ForeignKey_Has));
    prototype->Set(::v8::String::NewSymbol("isReferenced"), ::v8::FunctionTemplate::New(ForeignKey_IsReferenced));
    prototype->Set(::v8::String::NewSymbol("replace"), ::v8::FunctionTemplate::New(ForeignKey_Replace));
    prototype->Set(::v8::String::NewSymbol("getType"), ::v8::FunctionTemplate::New(ForeignKey_GetType));
    prototype->Set(::v8::String::NewSymbol("clone"), ::v8::FunctionTemplate::New(ForeignKey_Clone));

    sdset_foreignkey_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sdset_foreignkey_template;
}
