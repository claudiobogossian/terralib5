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
  \file AbstractData.cpp
   
  \brief Utility functions to register the AbstractData class into Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../datatype/AbstractData.h"
#include "../../common/Utils.h"
#include "DataType.h"

::v8::Handle<::v8::Value> AbstractData_GetTypeCode(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getTypeCode method you must use object notation: \"c = obj.getTypeCode();\""));

  te::dt::AbstractData* ad = te::v8::common::Unwrap<te::dt::AbstractData>(args.Holder());

  if(ad == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid data in getTypeCode method!"));

  int c = ad->getTypeCode();

  ::v8::Local<::v8::Integer> jc = ::v8::Integer::New(c);

  return hs.Close(jc);
}

::v8::Handle<::v8::Value> AbstractData_ToString(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use toString method you must use object notation: \"s = obj.toString();\""));

  te::dt::AbstractData* ad = te::v8::common::Unwrap<te::dt::AbstractData>(args.Holder());

  if(ad == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid data in toString method!"));

  std::string s = ad->toString();

  ::v8::Local<::v8::String> js = ::v8::String::New(s.c_str());

  return hs.Close(js);
}

static ::v8::Persistent<::v8::FunctionTemplate> sg_absractdata_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetAbstractDataTemplate()
{
  if(sg_absractdata_template.IsEmpty())
  {
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("getTypeCode"), ::v8::FunctionTemplate::New(AbstractData_GetTypeCode));
    prototype->Set(::v8::String::NewSymbol("toString"), ::v8::FunctionTemplate::New(AbstractData_ToString));

    sg_absractdata_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sg_absractdata_template;
}


