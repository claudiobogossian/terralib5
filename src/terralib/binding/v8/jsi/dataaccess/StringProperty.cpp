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
  \file StringProperty.cpp
   
  \brief Utility functions to register the StringProperty class into the Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../dataaccess/property/StringProperty.h"
#include "../../common/Utils.h"
#include "DataAccess.h"

// Boost
#include <boost/cstdint.hpp>

::v8::Handle<::v8::Value> StringProperty_GetSubType(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> StringProperty_SetSubType(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> StringProperty_Size(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> StringProperty_SetSize(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> StringProperty_Clone(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> StringProperty_Constructor(const ::v8::Arguments& args)
{
  //::v8::HandleScope hs;

  //if(!args.IsConstructCall())
  //  return ::v8::ThrowException(::v8::String::New("In order to create a check constraint, you need to call its constructor like: var c = new TeStringProperty(dt, id);"));

  //if(args.Holder().IsEmpty())
  //  return ::v8::ThrowException(::v8::String::New("The StringProperty constructor must use object notation!"));

  //if(args.Length() != 0)
  //  return ::v8::ThrowException(::v8::String::New("The StringProperty constructor has no parameters!"));

  //std::auto_ptr<te::da::StringProperty> catalog(new te::da::StringProperty());

  //::v8::Local<::v8::Object> jcatalog = te::v8::common::Make(catalog.get(), te::v8::jsi::GetStringPropertyTemplate, true);

  //catalog.release();

  //return hs.Close(jcatalog);

  return ::v8::Undefined();
}

void te::v8::jsi::RegisterStringProperty(::v8::Local<::v8::Object>& global)
{
  ::v8::Local<::v8::FunctionTemplate> jstringproperty = ::v8::FunctionTemplate::New(StringProperty_Constructor);

  global->Set(::v8::String::New("TeStringProperty"), jstringproperty->GetFunction());
}

static ::v8::Persistent<::v8::FunctionTemplate> sdset_stringproperty_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetStringPropertyTemplate()
{
  if(sdset_stringproperty_template.IsEmpty())
  {
    ::v8::Persistent<::v8::FunctionTemplate>& pTpl = GetSimplePropertyTemplate();
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    result->Inherit(pTpl);

    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("getSubType"), ::v8::FunctionTemplate::New(StringProperty_GetSubType));
    prototype->Set(::v8::String::NewSymbol("setSubtype"), ::v8::FunctionTemplate::New(StringProperty_SetSubType));
    prototype->Set(::v8::String::NewSymbol("size"), ::v8::FunctionTemplate::New(StringProperty_Size));
    prototype->Set(::v8::String::NewSymbol("setSize"), ::v8::FunctionTemplate::New(StringProperty_SetSize));
    prototype->Set(::v8::String::NewSymbol("clone"), ::v8::FunctionTemplate::New(StringProperty_Clone));

    sdset_stringproperty_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sdset_stringproperty_template;
}
