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
  \file Property.cpp
   
  \brief Utility functions to register the Property class into the Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../dataaccess/property/Property.h"
#include "../../common/Utils.h"
#include "DataAccess.h"

// Boost
#include <boost/cstdint.hpp>

::v8::Handle<::v8::Value> Property_GetId(const ::v8::Arguments& args)
{
    return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Property_SetId(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Property_GetName(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Property_SetName(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Property_GetType(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Property_GetParent(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Property_SetParent(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Property_Has(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Property_Clone(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Property_Constructor(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

void te::v8::jsi::RegisterProperty(::v8::Local<::v8::Object>& global)
{
  ::v8::Local<::v8::FunctionTemplate> jproperty = ::v8::FunctionTemplate::New(Property_Constructor);

  global->Set(::v8::String::New("TeProperty"), jproperty->GetFunction());
}

static ::v8::Persistent<::v8::FunctionTemplate> sproperty_property_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetPropertyTemplate()
{
  if(sproperty_property_template.IsEmpty())
  {
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("getId"), ::v8::FunctionTemplate::New(Property_GetId));
    prototype->Set(::v8::String::NewSymbol("setId"), ::v8::FunctionTemplate::New(Property_SetId));
    prototype->Set(::v8::String::NewSymbol("getName"), ::v8::FunctionTemplate::New(Property_GetName));
    prototype->Set(::v8::String::NewSymbol("setName"), ::v8::FunctionTemplate::New(Property_SetName));
    prototype->Set(::v8::String::NewSymbol("getType"), ::v8::FunctionTemplate::New(Property_GetType));
    prototype->Set(::v8::String::NewSymbol("getParent"), ::v8::FunctionTemplate::New(Property_GetParent));
    prototype->Set(::v8::String::NewSymbol("setParent"), ::v8::FunctionTemplate::New(Property_SetParent));
    prototype->Set(::v8::String::NewSymbol("has"), ::v8::FunctionTemplate::New(Property_Has));
    prototype->Set(::v8::String::NewSymbol("clone"), ::v8::FunctionTemplate::New(Property_Clone));

    sproperty_property_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sproperty_property_template;
}
