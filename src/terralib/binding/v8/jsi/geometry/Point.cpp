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
  \file Point.cpp
   
  \brief Utility functions to register the Point class into Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../common/Logger.h"
#include "../../../../common/Translator.h"
#include "../../../../geometry/Point.h"
#include "../../../../geometry/PointM.h"
#include "../../../../geometry/PointZ.h"
#include "../../../../geometry/PointZM.h"
#include "../../common/Utils.h"
#include "Geometry.h"

::v8::Handle<::v8::Value> Point_GetX(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("Point getX method must use object notation!"));

  te::gm::Point* pt = te::v8::common::Unwrap<te::gm::Point>(args.Holder());

  return ::v8::Number::New(pt->getX());
}

::v8::Handle<::v8::Value> Point_SetX(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("Point setX method must use object notation!"));

  if((args.Length() != 1) || !args[0]->IsNumber())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong type in Point setX method!"));

  te::gm::Point* pt = te::v8::common::Unwrap<te::gm::Point>(args.Holder());

  pt->setX(args[0]->ToNumber()->Value());

  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Point_GetY(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("Point getY method must use object notation!"));

  te::gm::Point* pt = te::v8::common::Unwrap<te::gm::Point>(args.Holder());

  return ::v8::Number::New(pt->getY());
}

::v8::Handle<::v8::Value> Point_SetY(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("Point setY method must use object notation!"));

  if((args.Length() != 1) || !args[0]->IsNumber())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong type in Point setX method!"));

  te::gm::Point* pt = te::v8::common::Unwrap<te::gm::Point>(args.Holder());

  pt->setY(args[0]->ToNumber()->Value());

  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Point_GetZ(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("Point getZ method must use object notation!"));

  te::gm::Point* pt = te::v8::common::Unwrap<te::gm::Point>(args.Holder());

  return ::v8::Number::New(pt->getZ());
}

::v8::Handle<::v8::Value> Point_SetZ(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("Point setZ method must use object notation!"));

  if((args.Length() != 1) || !args[0]->IsNumber())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong type in Point setZ method!"));

  te::gm::Point* pt = te::v8::common::Unwrap<te::gm::Point>(args.Holder());

  pt->setZ(args[0]->ToNumber()->Value());

  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Point_GetM(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("Point getX method must use object notation!"));

  te::gm::Point* pt = te::v8::common::Unwrap<te::gm::Point>(args.Holder());

  return ::v8::Number::New(pt->getM());
}

::v8::Handle<::v8::Value> Point_SetM(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("Point setM method must use object notation!"));

  if((args.Length() != 1) || !args[0]->IsNumber())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong type in Point setM method!"));

  te::gm::Point* pt = te::v8::common::Unwrap<te::gm::Point>(args.Holder());

  pt->setM(args[0]->ToNumber()->Value());

  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Point_Constructor(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(!args.IsConstructCall())
    return ::v8::ThrowException(::v8::String::New("In order to create a point you need call its constructor like: var mypt = new TePoint(1.0, 2.0, 4326)."));

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("Point constructor must use object notation!"));

  double x = args[0]->ToNumber()->Value();
  double y = args[1]->ToNumber()->Value();
  int srid = args[2]->ToInt32()->Int32Value();

  std::auto_ptr<te::gm::Point> pt(new te::gm::Point(x, y, srid));

  ::v8::Local<::v8::Object> jpt = te::v8::common::Make(pt.get(), te::v8::jsi::GetPointTemplate, true);

  pt.release();

  return hs.Close(jpt);
}

::v8::Handle<::v8::Value> PointZ_Constructor(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(!args.IsConstructCall())
    return ::v8::ThrowException(::v8::String::New("In order to create a point you need call its constructor like: var mypt = new TePointZ(1.0, 2.0, 3.0, 4326)."));

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("Point constructor must use object notation!"));

  double x = args[0]->ToNumber()->Value();
  double y = args[1]->ToNumber()->Value();
  double z = args[2]->ToNumber()->Value();
  int srid = args[3]->ToInt32()->Int32Value();

  std::auto_ptr<te::gm::PointZ> pt(new te::gm::PointZ(x, y, z, srid));

  ::v8::Local<::v8::Object> jpt = te::v8::common::Make(pt.get(), te::v8::jsi::GetPointTemplate, true);

  pt.release();

  return hs.Close(jpt);
}

::v8::Handle<::v8::Value> PointM_Constructor(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(!args.IsConstructCall())
    return ::v8::ThrowException(::v8::String::New("In order to create a point you need call its constructor like: var mypt = new TePointM(1.0, 2.0, 3.0, 4326)."));

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("Point constructor must use object notation!"));

  double x = args[0]->ToNumber()->Value();
  double y = args[1]->ToNumber()->Value();
  double m = args[2]->ToNumber()->Value();
  int srid = args[3]->ToInt32()->Int32Value();

  std::auto_ptr<te::gm::PointM> pt(new te::gm::PointM(x, y, m, srid));

  ::v8::Local<::v8::Object> jpt = te::v8::common::Make(pt.get(), te::v8::jsi::GetPointTemplate, true);

  pt.release();

  return hs.Close(jpt);
}

::v8::Handle<::v8::Value> PointZM_Constructor(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(!args.IsConstructCall())
    return ::v8::ThrowException(::v8::String::New("In order to create a point you need call its constructor like: var mypt = new TePointZM(1.0, 2.0, 3.0, 4.0, 4326)."));

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("Point constructor must use object notation!"));

  double x = args[0]->ToNumber()->Value();
  double y = args[1]->ToNumber()->Value();
  double z = args[2]->ToNumber()->Value();
  double m = args[3]->ToNumber()->Value();
  int srid = args[4]->ToInt32()->Int32Value();

  std::auto_ptr<te::gm::PointZM> pt(new te::gm::PointZM(x, y, z, m, srid));

  ::v8::Local<::v8::Object> jpt = te::v8::common::Make(pt.get(), te::v8::jsi::GetPointTemplate, true);

  pt.release();

  return hs.Close(jpt);
}

void te::v8::jsi::RegisterPoint(::v8::Local<::v8::Object>& global)
{
  ::v8::HandleScope hs;

  ::v8::Local<::v8::FunctionTemplate> jspt = ::v8::FunctionTemplate::New(Point_Constructor);
  ::v8::Local<::v8::FunctionTemplate> jsptz = ::v8::FunctionTemplate::New(PointZ_Constructor);
  ::v8::Local<::v8::FunctionTemplate> jsptm = ::v8::FunctionTemplate::New(PointM_Constructor);
  ::v8::Local<::v8::FunctionTemplate> jsptzm = ::v8::FunctionTemplate::New(PointZM_Constructor);

  global->Set(::v8::String::New("TePoint"), jspt->GetFunction());
  global->Set(::v8::String::New("TePointZ"), jsptz->GetFunction());
  global->Set(::v8::String::New("TePointM"), jsptm->GetFunction());
  global->Set(::v8::String::New("TePointZM"), jsptzm->GetFunction());
}

static ::v8::Persistent<::v8::FunctionTemplate> sg_point_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetPointTemplate()
{
  if(sg_point_template.IsEmpty())
  {
    ::v8::Persistent<::v8::FunctionTemplate>& geomTpl = GetGeometryTemplate();
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    result->Inherit(geomTpl);

    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("getX"), ::v8::FunctionTemplate::New(Point_GetX));
    prototype->Set(::v8::String::NewSymbol("setX"), ::v8::FunctionTemplate::New(Point_SetX));
    prototype->Set(::v8::String::NewSymbol("getY"), ::v8::FunctionTemplate::New(Point_GetY));
    prototype->Set(::v8::String::NewSymbol("setY"), ::v8::FunctionTemplate::New(Point_SetY));
    prototype->Set(::v8::String::NewSymbol("getZ"), ::v8::FunctionTemplate::New(Point_GetZ));
    prototype->Set(::v8::String::NewSymbol("setZ"), ::v8::FunctionTemplate::New(Point_SetZ));
    prototype->Set(::v8::String::NewSymbol("getM"), ::v8::FunctionTemplate::New(Point_GetM));
    prototype->Set(::v8::String::NewSymbol("setM"), ::v8::FunctionTemplate::New(Point_SetM));

    sg_point_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sg_point_template;
}

