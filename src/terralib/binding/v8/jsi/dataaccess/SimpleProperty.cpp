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
  \file SimpleProperty.cpp
   
  \brief Utility functions to register the SimpleProperty class into the Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../dataaccess/property/SimpleProperty.h"
#include "../../common/Utils.h"
#include "DataAccess.h"

// Boost
#include <boost/cstdint.hpp>

::v8::Handle<::v8::Value> SimpleProperty_IsRequired(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> SimpleProperty_SetRequired(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> SimpleProperty_IsAutoNumber(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> SimpleProperty_SetAutoNumber(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> SimpleProperty_GetDefaultValue(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> SimpleProperty_SetDefaultValue(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> SimpleProperty_Has(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> SimpleProperty_Clone(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> SimpleProperty_Constructor(const ::v8::Arguments& args)
{
  //::v8::HandleScope hs;

  //if(!args.IsConstructCall())
  //  return ::v8::ThrowException(::v8::String::New("In order to create a check constraint, you need to call its constructor like: var c = new TeSimpleProperty(dt, id);"));

  //if(args.Holder().IsEmpty())
  //  return ::v8::ThrowException(::v8::String::New("The SimpleProperty constructor must use object notation!"));

  //if(args.Length() != 0)
  //  return ::v8::ThrowException(::v8::String::New("The SimpleProperty constructor has no parameters!"));

  //std::auto_ptr<te::da::SimpleProperty> catalog(new te::da::SimpleProperty());

  //::v8::Local<::v8::Object> jcatalog = te::v8::common::Make(catalog.get(), te::v8::jsi::GetSimplePropertyTemplate, true);

  //catalog.release();

  //return hs.Close(jcatalog);

  return ::v8::Undefined();
}

void te::v8::jsi::RegisterSimpleProperty(::v8::Local<::v8::Object>& global)
{
  ::v8::Local<::v8::FunctionTemplate> jsimpleproperty = ::v8::FunctionTemplate::New(SimpleProperty_Constructor);

  global->Set(::v8::String::New("TeSimpleProperty"), jsimpleproperty->GetFunction());
}

static ::v8::Persistent<::v8::FunctionTemplate> sdset_simpleproperty_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetSimplePropertyTemplate()
{
  if(sdset_simpleproperty_template.IsEmpty())
  {
    ::v8::Persistent<::v8::FunctionTemplate>& pTpl = GetPropertyTemplate();
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    result->Inherit(pTpl);

    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("isRequired"), ::v8::FunctionTemplate::New(SimpleProperty_IsRequired));
    prototype->Set(::v8::String::NewSymbol("setRequired"), ::v8::FunctionTemplate::New(SimpleProperty_SetRequired));
    prototype->Set(::v8::String::NewSymbol("isAutoNumber"), ::v8::FunctionTemplate::New(SimpleProperty_IsAutoNumber));
    prototype->Set(::v8::String::NewSymbol("setAutoNumber"), ::v8::FunctionTemplate::New(SimpleProperty_SetAutoNumber));
    prototype->Set(::v8::String::NewSymbol("getDefaultValue"), ::v8::FunctionTemplate::New(SimpleProperty_GetDefaultValue));
    prototype->Set(::v8::String::NewSymbol("setDefaultValue"), ::v8::FunctionTemplate::New(SimpleProperty_SetDefaultValue));
    prototype->Set(::v8::String::NewSymbol("has"), ::v8::FunctionTemplate::New(SimpleProperty_Has));
    prototype->Set(::v8::String::NewSymbol("clone"), ::v8::FunctionTemplate::New(SimpleProperty_Clone));

    sdset_simpleproperty_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sdset_simpleproperty_template;
}
