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
  \file NumericProperty.cpp
   
  \brief Utility functions to register the NumericProperty class into the Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../dataaccess/property/NumericProperty.h"
#include "../../common/Utils.h"
#include "DataAccess.h"

// Boost
#include <boost/cstdint.hpp>

::v8::Handle<::v8::Value> NumericProperty_GetPrecision(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> NumericProperty_GetScale(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> NumericProperty_Clone(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> NumericProperty_Constructor(const ::v8::Arguments& args)
{
  //::v8::HandleScope hs;

  //if(!args.IsConstructCall())
  //  return ::v8::ThrowException(::v8::String::New("In order to create a check constraint, you need to call its constructor like: var c = new TeNumericProperty(dt, id);"));

  //if(args.Holder().IsEmpty())
  //  return ::v8::ThrowException(::v8::String::New("The NumericProperty constructor must use object notation!"));

  //if(args.Length() != 0)
  //  return ::v8::ThrowException(::v8::String::New("The NumericProperty constructor has no parameters!"));

  //std::auto_ptr<te::da::NumericProperty> catalog(new te::da::NumericProperty());

  //::v8::Local<::v8::Object> jcatalog = te::v8::common::Make(catalog.get(), te::v8::jsi::GetNumericPropertyTemplate, true);

  //catalog.release();

  //return hs.Close(jcatalog);

  return ::v8::Undefined();
}

void te::v8::jsi::RegisterNumericProperty(::v8::Local<::v8::Object>& global)
{
  ::v8::Local<::v8::FunctionTemplate> jnumericproperty = ::v8::FunctionTemplate::New(NumericProperty_Constructor);

  global->Set(::v8::String::New("TeNumericProperty"), jnumericproperty->GetFunction());
}

static ::v8::Persistent<::v8::FunctionTemplate> sdset_numericproperty_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetNumericPropertyTemplate()
{
  if(sdset_numericproperty_template.IsEmpty())
  {
    ::v8::Persistent<::v8::FunctionTemplate>& pTpl = GetSimplePropertyTemplate();
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    result->Inherit(pTpl);

    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("getPrecision"), ::v8::FunctionTemplate::New(NumericProperty_GetPrecision));
    prototype->Set(::v8::String::NewSymbol("getScale"), ::v8::FunctionTemplate::New(NumericProperty_GetScale));
    prototype->Set(::v8::String::NewSymbol("clone"), ::v8::FunctionTemplate::New(NumericProperty_Clone));

    sdset_numericproperty_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sdset_numericproperty_template;
}
