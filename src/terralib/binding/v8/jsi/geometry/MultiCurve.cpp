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
  \file MultiCurve.cpp
   
  \brief Utility functions to register the MultiCurve class into Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../geometry/MultiCurve.h"
#include "../../common/Utils.h"
#include "Geometry.h"

::v8::Handle<::v8::Value> MultiCurve_IsClosed(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use isClosed method you must use object notation: \"isclosed = obj.isClosed();\""));

  te::gm::MultiCurve* g = te::v8::common::Unwrap<te::gm::MultiCurve>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in isClosed method!"));

  bool is = g->isClosed();

  ::v8::Handle<::v8::Boolean> jis = ::v8::Boolean::New(is);

  return hs.Close(jis);
}

::v8::Handle<::v8::Value> MultiCurve_GetLength(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getLength method you must use object notation: \"len = obj.getLength();\""));

  te::gm::MultiCurve* g = te::v8::common::Unwrap<te::gm::MultiCurve>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getLength method!"));

  double len = g->getLength();

  ::v8::Local<::v8::Number> jlen = ::v8::Number::New(len);

  return hs.Close(jlen);
}

static ::v8::Persistent<::v8::FunctionTemplate> sg_mcurve_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetMultiCurveTemplate()
{
  if(sg_mcurve_template.IsEmpty())
  {
    ::v8::Persistent<::v8::FunctionTemplate>& geomTpl = GetGeometryCollectionTemplate();
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    result->Inherit(geomTpl);

    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("isClosed"), ::v8::FunctionTemplate::New(MultiCurve_IsClosed));
    prototype->Set(::v8::String::NewSymbol("getLength"), ::v8::FunctionTemplate::New(MultiCurve_GetLength));

    sg_mcurve_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sg_mcurve_template;
}

