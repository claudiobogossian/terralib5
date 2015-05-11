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
  \file LineString.cpp
   
  \brief Utility functions to register the LineString class into Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../geometry/Coord2D.h"
#include "../../../../geometry/LineString.h"
#include "../../../../geometry/Point.h"
#include "../../common/Utils.h"
#include "Geometry.h"

::v8::Handle<::v8::Value> LineString_SetNumCoordinates(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use setNumCoordinates method you must use object notation: \"line.setNumCoordinates(10);\""));

  te::gm::LineString* g = te::v8::common::Unwrap<te::gm::LineString>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in setNumCoordinates method!"));

  if(args.Length() != 1 || args[0].IsEmpty() || !args[0]->IsInt32())
    return ::v8::ThrowException(::v8::String::New("Invalid parameter for setNumCoordinates!"));

  int n = args[0]->ToInt32()->Value();

  g->setNumCoordinates(n);

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> LineString_MakeEmpty(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use makeEmpty method you must use object notation: \"line.makeEmpty();\""));

  te::gm::LineString* g = te::v8::common::Unwrap<te::gm::LineString>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in setNumCoordinates method!"));

  g->makeEmpty();

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> LineString_GetPointN(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getPointN method you must use object notation: \"pt = line.getPointN(3);\""));

  te::gm::LineString* g = te::v8::common::Unwrap<te::gm::LineString>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getPointN method!"));

  if(args.Length() != 1 || args[0].IsEmpty() || !args[0]->IsInt32())
    return ::v8::ThrowException(::v8::String::New("Invalid parameter for getPointN!"));

  int i = args[0]->ToInt32()->Value();

  std::auto_ptr<te::gm::Point> pt(g->getPointN(i));

  ::v8::Local<::v8::Object> jsgeom = te::v8::common::Make(pt.get(), te::v8::jsi::GetPointTemplate, true);

  pt.release();

  return hs.Close(jsgeom);
}

::v8::Handle<::v8::Value> LineString_SetPointN(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use setPointN method you must use object notation: \"line.setPointN(i, pt);\""));

  te::gm::LineString* g = te::v8::common::Unwrap<te::gm::LineString>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in setPointN method!"));

  if(args.Length() != 2 || args[0].IsEmpty() || !args[0]->IsInt32() || args[1].IsEmpty() || !args[1]->IsObject())
    return ::v8::ThrowException(::v8::String::New("Invalid parameter for setPointN!"));

  int i = args[0]->ToInt32()->Value();

  te::gm::Point* pt = te::v8::common::Unwrap<te::gm::Point>(args[1]->ToObject());

  g->setPointN(i, *pt);

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> LineString_SetPoint(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use setPoint method you must use object notation: \"line.setPoint(i, x, y);\""));

  te::gm::LineString* g = te::v8::common::Unwrap<te::gm::LineString>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in setPoint method!"));

  if(args.Length() != 3 ||
     args[0].IsEmpty() || !args[0]->IsInt32() ||
     args[1].IsEmpty() || !args[1]->IsNumber() ||
     args[2].IsEmpty() || !args[2]->IsNumber())
    return ::v8::ThrowException(::v8::String::New("Invalid parameter for setPoint!"));

  int i = args[0]->ToInt32()->Value();
  double x = args[1]->ToNumber()->Value();
  double y = args[2]->ToNumber()->Value();

  g->setPoint(i, x, y);

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> LineString_SetPointZ(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use setPointZ method you must use object notation: \"line.setPointZ(i, x, y, z);\""));

  te::gm::LineString* g = te::v8::common::Unwrap<te::gm::LineString>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in setPointZ method!"));

  if(args.Length() != 4 ||
     args[0].IsEmpty() || !args[0]->IsInt32() ||
     args[1].IsEmpty() || !args[1]->IsNumber() ||
     args[2].IsEmpty() || !args[2]->IsNumber() ||
     args[3].IsEmpty() || !args[3]->IsNumber())
    return ::v8::ThrowException(::v8::String::New("Invalid parameter for setPointZ!"));

  int i = args[0]->ToInt32()->Value();
  double x = args[1]->ToNumber()->Value();
  double y = args[2]->ToNumber()->Value();
  double z = args[3]->ToNumber()->Value();

  g->setPointZ(i, x, y, z);

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> LineString_SetPointM(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use setPointM method you must use object notation: \"line.setPointM(i, x, y, m);\""));

  te::gm::LineString* g = te::v8::common::Unwrap<te::gm::LineString>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in setPointM method!"));

  if(args.Length() != 4 ||
     args[0].IsEmpty() || !args[0]->IsInt32() ||
     args[1].IsEmpty() || !args[1]->IsNumber() ||
     args[2].IsEmpty() || !args[2]->IsNumber() ||
     args[3].IsEmpty() || !args[3]->IsNumber())
    return ::v8::ThrowException(::v8::String::New("Invalid parameter for setPointM!"));

  int i = args[0]->ToInt32()->Value();
  double x = args[1]->ToNumber()->Value();
  double y = args[2]->ToNumber()->Value();
  double m = args[3]->ToNumber()->Value();

  g->setPointM(i, x, y, m);

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> LineString_SetPointZM(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use setPointZM method you must use object notation: \"line.setPointZM(i, x, y, z, m);\""));

  te::gm::LineString* g = te::v8::common::Unwrap<te::gm::LineString>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in setPointZM method!"));

  if(args.Length() != 4 ||
     args[0].IsEmpty() || !args[0]->IsInt32() ||
     args[1].IsEmpty() || !args[1]->IsNumber() ||
     args[2].IsEmpty() || !args[2]->IsNumber() ||
     args[3].IsEmpty() || !args[3]->IsNumber() ||
     args[4].IsEmpty() || !args[4]->IsNumber())
    return ::v8::ThrowException(::v8::String::New("Invalid parameter for setPointZM!"));

  int i = args[0]->ToInt32()->Value();
  double x = args[1]->ToNumber()->Value();
  double y = args[2]->ToNumber()->Value();
  double z = args[3]->ToNumber()->Value();
  double m = args[4]->ToNumber()->Value();

  g->setPointZM(i, x, y, z, m);

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> LineString_GetX(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getX method you must use object notation: \"x = line.getX(i);\""));

  te::gm::LineString* g = te::v8::common::Unwrap<te::gm::LineString>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getX method!"));

  if(args.Length() != 1 || args[0].IsEmpty() || !args[0]->IsInt32())
    return ::v8::ThrowException(::v8::String::New("Invalid parameter for getX!"));

  int i = args[0]->ToInt32()->Value();

  double x = g->getX(i);

  ::v8::Handle<::v8::Number> jx = ::v8::Number::New(x);

  return hs.Close(jx);
}

::v8::Handle<::v8::Value> LineString_GetY(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getY method you must use object notation: \"y = line.getY(i);\""));

  te::gm::LineString* g = te::v8::common::Unwrap<te::gm::LineString>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getY method!"));

  if(args.Length() != 1 || args[0].IsEmpty() || !args[0]->IsInt32())
    return ::v8::ThrowException(::v8::String::New("Invalid parameter for getY!"));

  int i = args[0]->ToInt32()->Value();

  double y = g->getY(i);

  ::v8::Handle<::v8::Number> jy = ::v8::Number::New(y);

  return hs.Close(jy);
}

::v8::Handle<::v8::Value> LineString_GetZ(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getZ method you must use object notation: \"z = line.getZ(i);\""));

  te::gm::LineString* g = te::v8::common::Unwrap<te::gm::LineString>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getZ method!"));

  if(args.Length() != 1 || args[0].IsEmpty() || !args[0]->IsInt32())
    return ::v8::ThrowException(::v8::String::New("Invalid parameter for getZ!"));

  int i = args[0]->ToInt32()->Value();

  double z = g->getZ(i);

  ::v8::Handle<::v8::Number> jz = ::v8::Number::New(z);

  return hs.Close(jz);
}

::v8::Handle<::v8::Value> LineString_GetM(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getM method you must use object notation: \"m = line.getM(i);\""));

  te::gm::LineString* g = te::v8::common::Unwrap<te::gm::LineString>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getM method!"));

  if(args.Length() != 1 || args[0].IsEmpty() || !args[0]->IsInt32())
    return ::v8::ThrowException(::v8::String::New("Invalid parameter for getM!"));

  int i = args[0]->ToInt32()->Value();

  double m = g->getM(i);

  ::v8::Handle<::v8::Number> jm = ::v8::Number::New(m);

  return hs.Close(jm);
}

::v8::Handle<::v8::Value> LineString_SetX(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use setX method you must use object notation: \"line.setX(i, x);\""));

  te::gm::LineString* g = te::v8::common::Unwrap<te::gm::LineString>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in setX method!"));

  if(args.Length() != 2 || args[0].IsEmpty() || !args[0]->IsInt32() || args[1].IsEmpty() || !args[1]->IsNumber())
    return ::v8::ThrowException(::v8::String::New("Invalid parameter for setX!"));

  int i = args[0]->ToInt32()->Value();
  double x = args[1]->ToNumber()->Value();

  g->setX(i, x);

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> LineString_SetY(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use setY method you must use object notation: \"line.setY(i, y);\""));

  te::gm::LineString* g = te::v8::common::Unwrap<te::gm::LineString>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in setY method!"));

  if(args.Length() != 2 || args[0].IsEmpty() || !args[0]->IsInt32() || args[1].IsEmpty() || !args[1]->IsNumber())
    return ::v8::ThrowException(::v8::String::New("Invalid parameter for setY!"));

  int i = args[0]->ToInt32()->Value();
  double y = args[1]->ToNumber()->Value();

  g->setY(i, y);

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> LineString_SetZ(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use setZ method you must use object notation: \"line.setZ(i, z);\""));

  te::gm::LineString* g = te::v8::common::Unwrap<te::gm::LineString>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in setZ method!"));

  if(args.Length() != 2 || args[0].IsEmpty() || !args[0]->IsInt32() || args[1].IsEmpty() || !args[1]->IsNumber())
    return ::v8::ThrowException(::v8::String::New("Invalid parameter for setZ!"));

  int i = args[0]->ToInt32()->Value();
  double z = args[1]->ToNumber()->Value();

  g->setZ(i, z);

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> LineString_SetM(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use setM method you must use object notation: \"line.setM(i, m);\""));

  te::gm::LineString* g = te::v8::common::Unwrap<te::gm::LineString>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in setM method!"));

  if(args.Length() != 2 || args[0].IsEmpty() || !args[0]->IsInt32() || args[1].IsEmpty() || !args[1]->IsNumber())
    return ::v8::ThrowException(::v8::String::New("Invalid parameter for setM!"));

  int i = args[0]->ToInt32()->Value();
  double m = args[1]->ToNumber()->Value();

  g->setM(i, m);

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> LineString_GetCoordinates(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getCoordinates method you must use object notation: \"cs = line.getCoordinates();\""));

  te::gm::LineString* g = te::v8::common::Unwrap<te::gm::LineString>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getCoordinates method!"));

  te::gm::Coord2D* cs = g->getCoordinates();

  int len = static_cast<int>(g->getNPoints());

  ::v8::Handle<::v8::Array> jcs = ::v8::Array::New(len);

  for(int i = 0; i < len; ++i)
    jcs->Set(i, te::v8::common::Make(&(cs[i]), te::v8::jsi::GetCoord2DTemplate, false));

  return hs.Close(jcs);
}

::v8::Handle<::v8::Value> LineString_GetZCoords(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getZCoords method you must use object notation: \"zs = line.getZCoords();\""));

  te::gm::LineString* g = te::v8::common::Unwrap<te::gm::LineString>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getZCoords method!"));

  double* zs = g->getZ();

  int len = static_cast<int>(g->getNPoints());

  ::v8::Handle<::v8::Array> jzs = ::v8::Array::New(len);

  for(int i = 0; i < len; ++i)
    jzs->Set(i, ::v8::Number::New(zs[i]));

  return hs.Close(jzs);
}

::v8::Handle<::v8::Value> LineString_GetMCoords(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getMCoords method you must use object notation: \"ms = line.getMCoords();\""));

  te::gm::LineString* g = te::v8::common::Unwrap<te::gm::LineString>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getMCoords method!"));

  double* ms = g->getM();

  int len = static_cast<int>(g->getNPoints());

  ::v8::Handle<::v8::Array> jms = ::v8::Array::New(len);

  for(int i = 0; i < len; ++i)
    jms->Set(i, ::v8::Number::New(ms[i]));

  return hs.Close(jms);
}

::v8::Handle<::v8::Value> LineString_Constructor(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(!args.IsConstructCall())
    return ::v8::ThrowException(::v8::String::New("In order to create a LineString you need to call its constructor like: var mygc = new TeLineString(3, TE_OGC_LINESTRING, 4326)."));

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("LineString constructor must use object notation!"));

  if((args.Length() != 3) ||
      args[0].IsEmpty() || !args[0]->IsInt32() ||
      args[1].IsEmpty() || !args[1]->IsInt32() ||
      args[2].IsEmpty() || !args[2]->IsInt32())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in LineString constructor method!!"));


  int npts = args[0]->Int32Value();
  int geomType = args[1]->Int32Value();
  int srid = args[2]->ToInt32()->Int32Value();

  std::auto_ptr<te::gm::LineString> g(new te::gm::LineString(npts, static_cast<te::gm::GeomType>(geomType), srid));

  ::v8::Local<::v8::Object> jg = te::v8::common::Make(g.get(), te::v8::jsi::GetLineStringTemplate, true);

  g.release();

  return hs.Close(jg);
}

void te::v8::jsi::RegisterLineString(::v8::Local<::v8::Object>& global)
{
  ::v8::HandleScope hs;

  ::v8::Local<::v8::FunctionTemplate> jsgc = ::v8::FunctionTemplate::New(LineString_Constructor);

  global->Set(::v8::String::New("TeLineString"), jsgc->GetFunction());
}

static ::v8::Persistent<::v8::FunctionTemplate> sg_line_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetLineStringTemplate()
{
  if(sg_line_template.IsEmpty())
  {
    ::v8::Persistent<::v8::FunctionTemplate>& curveTpl = GetCurveTemplate();
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    result->Inherit(curveTpl);

    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("setNumCoordinates"), ::v8::FunctionTemplate::New(LineString_SetNumCoordinates));
    prototype->Set(::v8::String::NewSymbol("makeEmpty"), ::v8::FunctionTemplate::New(LineString_MakeEmpty));
    prototype->Set(::v8::String::NewSymbol("getPointN"), ::v8::FunctionTemplate::New(LineString_GetPointN));
    prototype->Set(::v8::String::NewSymbol("setPointN"), ::v8::FunctionTemplate::New(LineString_SetPointN));
    prototype->Set(::v8::String::NewSymbol("setPoint"), ::v8::FunctionTemplate::New(LineString_SetPoint));
    prototype->Set(::v8::String::NewSymbol("setPointZ"), ::v8::FunctionTemplate::New(LineString_SetPointZ));
    prototype->Set(::v8::String::NewSymbol("setPointM"), ::v8::FunctionTemplate::New(LineString_SetPointM));
    prototype->Set(::v8::String::NewSymbol("setPointZM"), ::v8::FunctionTemplate::New(LineString_SetPointZM));
    prototype->Set(::v8::String::NewSymbol("getX"), ::v8::FunctionTemplate::New(LineString_GetX));
    prototype->Set(::v8::String::NewSymbol("getY"), ::v8::FunctionTemplate::New(LineString_GetY));
    prototype->Set(::v8::String::NewSymbol("getZ"), ::v8::FunctionTemplate::New(LineString_GetZ));
    prototype->Set(::v8::String::NewSymbol("getM"), ::v8::FunctionTemplate::New(LineString_GetM));
    prototype->Set(::v8::String::NewSymbol("setX"), ::v8::FunctionTemplate::New(LineString_SetX));
    prototype->Set(::v8::String::NewSymbol("setY"), ::v8::FunctionTemplate::New(LineString_SetY));
    prototype->Set(::v8::String::NewSymbol("setZ"), ::v8::FunctionTemplate::New(LineString_SetZ));
    prototype->Set(::v8::String::NewSymbol("setM"), ::v8::FunctionTemplate::New(LineString_SetM));
    prototype->Set(::v8::String::NewSymbol("getCoordinates"), ::v8::FunctionTemplate::New(LineString_GetCoordinates));
    prototype->Set(::v8::String::NewSymbol("getZCoords"), ::v8::FunctionTemplate::New(LineString_GetZCoords));
    prototype->Set(::v8::String::NewSymbol("getMCoords"), ::v8::FunctionTemplate::New(LineString_GetMCoords));

    sg_line_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sg_line_template;
}


