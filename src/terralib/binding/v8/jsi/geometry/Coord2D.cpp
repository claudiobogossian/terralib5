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
  \file Coord2D.cpp
   
  \brief Utility functions to register the Coord2D class into Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../geometry/Coord2D.h"
#include "../../common/Utils.h"
#include "Geometry.h"

// STL
#include <memory>

::v8::Handle<::v8::Value> Coord2D_XGetter(::v8::Local<::v8::String> /*prop*/, const ::v8::AccessorInfo& info)
{
  ::v8::HandleScope hs;

  if(info.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("Missing object in Coord2D x getter!"));

  te::gm::Coord2D* c = te::v8::common::Unwrap<te::gm::Coord2D>(info.Holder());

  return ::v8::Number::New(c->x);
}

void Coord2D_XSetter(::v8::Local<::v8::String> /*prop*/, ::v8::Local<::v8::Value> value, const ::v8::AccessorInfo& info)
{
  ::v8::HandleScope hs;

  if(info.Holder().IsEmpty())
  {
    ::v8::ThrowException(::v8::String::New("Missing object in Coord2D x setter!"));
    return;
  }

  if(value.IsEmpty() || !value->IsNumber())
  {
    ::v8::ThrowException(::v8::String::New("Missing value or wrong type in Coord2D x setter!"));
    return;
  }

  te::gm::Coord2D* c = te::v8::common::Unwrap<te::gm::Coord2D>(info.Holder());

  double x = value->ToNumber()->Value();

  c->x = x;

  return;
}

::v8::Handle<::v8::Value> Coord2D_YGetter(::v8::Local<::v8::String> /*prop*/, const ::v8::AccessorInfo& info)
{
  ::v8::HandleScope hs;

  if(info.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("Missing object in Coord2D y getter!"));

  te::gm::Coord2D* c = te::v8::common::Unwrap<te::gm::Coord2D>(info.Holder());

  return ::v8::Number::New(c->y);
}

void Coord2D_YSetter(::v8::Local<::v8::String> /*prop*/, ::v8::Local<::v8::Value> value, const ::v8::AccessorInfo& info)
{
  ::v8::HandleScope hs;

  if(info.Holder().IsEmpty())
  {
    ::v8::ThrowException(::v8::String::New("Missing object in Coord2D y setter!"));
    return;
  }

  if(value.IsEmpty() || !value->IsNumber())
  {
    ::v8::ThrowException(::v8::String::New("Missing value or wrong type in Coord2D y setter!"));
    return;
  }

  te::gm::Coord2D* c = te::v8::common::Unwrap<te::gm::Coord2D>(info.Holder());

  double y = value->ToNumber()->Value();

  c->y = y;

  return;
}

::v8::Handle<::v8::Value> Coord2D_Constructor(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(!args.IsConstructCall())
    return ::v8::ThrowException(::v8::String::New("In order to create a coordinate you need call its constructor like: var c = new TeCoord2D(1.0, 2.0);"));

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("Coord2D constructor must use object notation!"));

  if((args.Length() != 2) || !args[0]->IsNumber() || !args[1]->IsNumber())
    return ::v8::ThrowException(::v8::String::New("Missing parameter in Coord2D constructor or wrong parameter type!"));

  double x = args[0]->ToNumber()->Value();
  double y = args[1]->ToNumber()->Value();

  std::auto_ptr<te::gm::Coord2D> c(new te::gm::Coord2D(x, y));

  ::v8::Local<::v8::Object> jc = te::v8::common::Make(c.get(), te::v8::jsi::GetCoord2DTemplate, true);

  c.release();

  return hs.Close(jc);
}

void te::v8::jsi::RegisterCoord2D(::v8::Local<::v8::Object>& global)
{
  ::v8::HandleScope hs;

  ::v8::Local<::v8::FunctionTemplate> jsc = ::v8::FunctionTemplate::New(Coord2D_Constructor);

  global->Set(::v8::String::New("TeCoord2D"), jsc->GetFunction());
}

static ::v8::Persistent<::v8::FunctionTemplate> sg_coord2d_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetCoord2DTemplate()
{
  if(sg_coord2d_template.IsEmpty())
  {
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();

    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->SetAccessor(::v8::String::NewSymbol("x"), Coord2D_XGetter, Coord2D_XSetter);
    prototype->SetAccessor(::v8::String::NewSymbol("y"), Coord2D_YGetter, Coord2D_YSetter);

    sg_coord2d_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sg_coord2d_template;
}

