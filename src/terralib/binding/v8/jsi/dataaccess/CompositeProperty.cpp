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
  \file CompositeProperty.cpp
   
  \brief Utility functions to register the CompositeProperty class into the Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../dataaccess/property/CompositeProperty.h"
#include "../../common/Utils.h"
#include "DataAccess.h"

// Boost
#include <boost/cstdint.hpp>

::v8::Handle<::v8::Value> CompositeProperty_GetCompositeName(const ::v8::Arguments& /*args*/)
{
    return ::v8::Undefined();
}

::v8::Handle<::v8::Value> CompositeProperty_SetCompositeName(const ::v8::Arguments& /*args*/)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> CompositeProperty_Add(const ::v8::Arguments& /*args*/)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> CompositeProperty_Remove(const ::v8::Arguments& /*args*/)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> CompositeProperty_GetProperties(const ::v8::Arguments& /*args*/)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> CompositeProperty_Size(const ::v8::Arguments& /*args*/)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> CompositeProperty_GetProperty(const ::v8::Arguments& /*args*/)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> CompositeProperty_GetPropertyPosition(const ::v8::Arguments& /*args*/)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> CompositeProperty_GetPropertyById(const ::v8::Arguments& /*args*/)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> CompositeProperty_Copy(const ::v8::Arguments& /*args*/)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> CompositeProperty_Clear(const ::v8::Arguments& /*args*/)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> CompositeProperty_Constructor(const ::v8::Arguments& /*args*/)
{
  return ::v8::Undefined();
}

void te::v8::jsi::RegisterCompositeProperty(::v8::Local<::v8::Object>& global)
{
  ::v8::Local<::v8::FunctionTemplate> jcompositeproperty = ::v8::FunctionTemplate::New(CompositeProperty_Constructor);

  global->Set(::v8::String::New("TeCompositeProperty"), jcompositeproperty->GetFunction());
}

static ::v8::Persistent<::v8::FunctionTemplate> sg_compositeproperty_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetCompositePropertyTemplate()
{
  if(sg_compositeproperty_template.IsEmpty())
  {
    ::v8::Persistent<::v8::FunctionTemplate>& propertyTpl = GetPropertyTemplate();
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    result->Inherit(propertyTpl);

    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("getCompositeName"), ::v8::FunctionTemplate::New(CompositeProperty_GetCompositeName));
    prototype->Set(::v8::String::NewSymbol("setCompositeName"), ::v8::FunctionTemplate::New(CompositeProperty_SetCompositeName));
    prototype->Set(::v8::String::NewSymbol("add"), ::v8::FunctionTemplate::New(CompositeProperty_Add));
    prototype->Set(::v8::String::NewSymbol("remove"), ::v8::FunctionTemplate::New(CompositeProperty_Remove));
    prototype->Set(::v8::String::NewSymbol("getProperties"), ::v8::FunctionTemplate::New(CompositeProperty_GetProperties));
    prototype->Set(::v8::String::NewSymbol("size"), ::v8::FunctionTemplate::New(CompositeProperty_Size));
    prototype->Set(::v8::String::NewSymbol("getProperty"), ::v8::FunctionTemplate::New(CompositeProperty_GetProperty));
    prototype->Set(::v8::String::NewSymbol("getPropertyPosition"), ::v8::FunctionTemplate::New(CompositeProperty_GetPropertyPosition));
    prototype->Set(::v8::String::NewSymbol("getPropertyById"), ::v8::FunctionTemplate::New(CompositeProperty_GetPropertyById));
    prototype->Set(::v8::String::NewSymbol("copy"), ::v8::FunctionTemplate::New(CompositeProperty_Copy));
    prototype->Set(::v8::String::NewSymbol("clear"), ::v8::FunctionTemplate::New(CompositeProperty_Clear));

    sg_compositeproperty_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sg_compositeproperty_template;
}
