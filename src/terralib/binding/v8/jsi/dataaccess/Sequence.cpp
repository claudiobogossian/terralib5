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
  \file Sequence.cpp
   
  \brief Utility functions to register the Sequence class into the Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../dataaccess/dataset/Sequence.h"
#include "../../common/Utils.h"
#include "DataAccess.h"

// Boost
#include <boost/cstdint.hpp>

::v8::Handle<::v8::Value> Sequence_GetId(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Sequence_SetId(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Sequence_GetName(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Sequence_SetName(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Sequence_GetIncrement(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Sequence_SetIncrement(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Sequence_GetMinValue(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Sequence_SetMinvalue(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Sequence_GetMaxValue(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Sequence_SetMaxValue(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Sequence_GetStartValue(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Sequence_SetStartValue(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Sequence_GetCachedValues(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Sequence_SetCachedValues(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Sequence_IsCycled(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Sequence_SetAsNoCycle(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Sequence_GetOwner(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Sequence_SetOwner(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Sequence_GetCatalog(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Sequence_SetCatalog(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Sequence_Constructor(const ::v8::Arguments& args)
{
  //::v8::HandleScope hs;

  //if(!args.IsConstructCall())
  //  return ::v8::ThrowException(::v8::String::New("In order to create a check constraint, you need to call its constructor like: var c = new TeSequence(dt, id);"));

  //if(args.Holder().IsEmpty())
  //  return ::v8::ThrowException(::v8::String::New("The Sequence constructor must use object notation!"));

  //if(args.Length() != 0)
  //  return ::v8::ThrowException(::v8::String::New("The Sequence constructor has no parameters!"));

  //std::auto_ptr<te::da::Sequence> catalog(new te::da::Sequence());

  //::v8::Local<::v8::Object> jcatalog = te::v8::common::Make(catalog.get(), te::v8::jsi::GetSequenceTemplate, true);

  //catalog.release();

  //return hs.Close(jcatalog);

  return ::v8::Undefined();
}

void te::v8::jsi::RegisterSequence(::v8::Local<::v8::Object>& global)
{
  ::v8::Local<::v8::FunctionTemplate> jsequence = ::v8::FunctionTemplate::New(Sequence_Constructor);

  global->Set(::v8::String::New("TeSequence"), jsequence->GetFunction());
}

static ::v8::Persistent<::v8::FunctionTemplate> sdset_sequence_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetSequenceTemplate()
{
  if(sdset_sequence_template.IsEmpty())
  {
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("getId"), ::v8::FunctionTemplate::New(Sequence_GetId));
    prototype->Set(::v8::String::NewSymbol("setId"), ::v8::FunctionTemplate::New(Sequence_SetId));
    prototype->Set(::v8::String::NewSymbol("getName"), ::v8::FunctionTemplate::New(Sequence_GetName));
    prototype->Set(::v8::String::NewSymbol("setName"), ::v8::FunctionTemplate::New(Sequence_SetName));
    prototype->Set(::v8::String::NewSymbol("getIncrement"), ::v8::FunctionTemplate::New(Sequence_GetIncrement));
    prototype->Set(::v8::String::NewSymbol("setIncrement"), ::v8::FunctionTemplate::New(Sequence_SetIncrement));
    prototype->Set(::v8::String::NewSymbol("getMinValue"), ::v8::FunctionTemplate::New(Sequence_GetMinValue));
    prototype->Set(::v8::String::NewSymbol("setMinValue"), ::v8::FunctionTemplate::New(Sequence_SetMinvalue));
    prototype->Set(::v8::String::NewSymbol("getMaxValue"), ::v8::FunctionTemplate::New(Sequence_GetMaxValue));
    prototype->Set(::v8::String::NewSymbol("setMaxValue"), ::v8::FunctionTemplate::New(Sequence_SetMaxValue));
    prototype->Set(::v8::String::NewSymbol("getStartValue"), ::v8::FunctionTemplate::New(Sequence_GetStartValue));
    prototype->Set(::v8::String::NewSymbol("setStartValue"), ::v8::FunctionTemplate::New(Sequence_SetStartValue));
    prototype->Set(::v8::String::NewSymbol("getCachedValues"), ::v8::FunctionTemplate::New(Sequence_GetCachedValues));
    prototype->Set(::v8::String::NewSymbol("setCachedValues"), ::v8::FunctionTemplate::New(Sequence_SetCachedValues));
    prototype->Set(::v8::String::NewSymbol("isCycled"), ::v8::FunctionTemplate::New(Sequence_IsCycled));
    prototype->Set(::v8::String::NewSymbol("setAsNoCycle"), ::v8::FunctionTemplate::New(Sequence_SetAsNoCycle));
    prototype->Set(::v8::String::NewSymbol("getOwner"), ::v8::FunctionTemplate::New(Sequence_GetOwner));
    prototype->Set(::v8::String::NewSymbol("setOwner"), ::v8::FunctionTemplate::New(Sequence_SetOwner));
    prototype->Set(::v8::String::NewSymbol("getCatalog"), ::v8::FunctionTemplate::New(Sequence_GetCatalog));
    prototype->Set(::v8::String::NewSymbol("setCatalog"), ::v8::FunctionTemplate::New(Sequence_SetCatalog));

    sdset_sequence_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sdset_sequence_template;
}
