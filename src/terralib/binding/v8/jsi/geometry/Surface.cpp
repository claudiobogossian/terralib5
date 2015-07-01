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
  \file Surface.cpp
   
  \brief Utility functions to register the Surface class into Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../geometry/Coord2D.h"
#include "../../../../geometry/Point.h"
#include "../../../../geometry/Surface.h"
#include "../../common/Utils.h"
#include "Geometry.h"

::v8::Handle<::v8::Value> Surface_GetArea(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getArea method you must use object notation: \"area = obj.getArea();\""));

  te::gm::Surface* g = te::v8::common::Unwrap<te::gm::Surface>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getArea method!"));

  double a = g->getArea();

  ::v8::Local<::v8::Number> ja = ::v8::Number::New(a);

  return hs.Close(ja);
}

::v8::Handle<::v8::Value> Surface_GetCentroid(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getCentroid method you must use object notation: \"pt = obj.getCentroid();\""));

  te::gm::Surface* g = te::v8::common::Unwrap<te::gm::Surface>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getCentroid method!"));

  std::auto_ptr<te::gm::Point> pt(g->getCentroid());

  ::v8::Local<::v8::Object> jsgeom = te::v8::common::Make(pt.get(), te::v8::jsi::GetPointTemplate, true);

  pt.release();

  return hs.Close(jsgeom);
}

::v8::Handle<::v8::Value> Surface_GetCentroidCoord(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getCentroidCoord method you must use object notation: \"c = obj.getCentroidCoord();\""));

  te::gm::Surface* g = te::v8::common::Unwrap<te::gm::Surface>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getCentroidCoord method!"));

  std::auto_ptr<te::gm::Coord2D> c(g->getCentroidCoord());

  ::v8::Local<::v8::Object> jsgeom = te::v8::common::Make(c.get(), te::v8::jsi::GetCoord2DTemplate, true);

  c.release();

  return hs.Close(jsgeom);
}

::v8::Handle<::v8::Value> Surface_GetPointOnSurface(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getPointOnSurface method you must use object notation: \"pt = obj.getPointOnSurface();\""));

  te::gm::Surface* g = te::v8::common::Unwrap<te::gm::Surface>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getPointOnSurface method!"));

  std::auto_ptr<te::gm::Point> pt(g->getPointOnSurface());

  ::v8::Local<::v8::Object> jsgeom = te::v8::common::Make(pt.get(), te::v8::jsi::GetPointTemplate, true);

  pt.release();

  return hs.Close(jsgeom);
}

::v8::Handle<::v8::Value> Surface_GetCoordOnSurface(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getCoordOnSurface method you must use object notation: \"c = obj.getCoordOnSurface();\""));

  te::gm::Surface* g = te::v8::common::Unwrap<te::gm::Surface>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getCoordOnSurface method!"));

  std::auto_ptr<te::gm::Coord2D> c(g->getCoordOnSurface());

  ::v8::Local<::v8::Object> jsgeom = te::v8::common::Make(c.get(), te::v8::jsi::GetCoord2DTemplate, true);

  c.release();

  return hs.Close(jsgeom);
}

static ::v8::Persistent<::v8::FunctionTemplate> sg_surface_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetSurfaceTemplate()
{
  if(sg_surface_template.IsEmpty())
  {
    ::v8::Persistent<::v8::FunctionTemplate>& geomTpl = GetGeometryTemplate();
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    result->Inherit(geomTpl);

    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("getArea"), ::v8::FunctionTemplate::New(Surface_GetArea));
    prototype->Set(::v8::String::NewSymbol("getCentroid"), ::v8::FunctionTemplate::New(Surface_GetCentroid));
    prototype->Set(::v8::String::NewSymbol("getCentroidCoord"), ::v8::FunctionTemplate::New(Surface_GetCentroidCoord));
    prototype->Set(::v8::String::NewSymbol("getPointOnSurface"), ::v8::FunctionTemplate::New(Surface_GetPointOnSurface));
    prototype->Set(::v8::String::NewSymbol("getCoordOnSurface"), ::v8::FunctionTemplate::New(Surface_GetCoordOnSurface));

    sg_surface_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sg_surface_template;
}

