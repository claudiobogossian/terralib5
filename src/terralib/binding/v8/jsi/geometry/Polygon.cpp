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
  \file Polygon.cpp
   
  \brief Utility functions to register the Polygon class into Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../geometry/LinearRing.h"
#include "../../../../geometry/Polygon.h"
#include "../../common/Utils.h"
#include "Geometry.h"

::v8::Handle<::v8::Value> Polygon_GetExteriorRing(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getExteriorRing method you must use object notation: \"ring = poly.getExteriorRing();\""));

  te::gm::Polygon* g = te::v8::common::Unwrap<te::gm::Polygon>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getExteriorRing method!"));

  te::gm::LinearRing* r = g->getExteriorRing();

  ::v8::Local<::v8::Object> jsgeom = te::v8::common::Make(r, te::v8::jsi::GetLinearRingTemplate, false);

  return hs.Close(jsgeom);
}

::v8::Handle<::v8::Value> Polygon_GetNumInteriorRings(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getNumInteriorRings method you must use object notation: \"n = poly.getNumInteriorRings();\""));

  te::gm::Polygon* g = te::v8::common::Unwrap<te::gm::Polygon>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getNumInteriorRings method!"));

  std::size_t n = g->getNumInteriorRings();

  ::v8::Local<::v8::Integer> jn = ::v8::Integer::New(static_cast<int>(n));

  return hs.Close(jn);
}

::v8::Handle<::v8::Value> Polygon_GetNumRings(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getNumRings method you must use object notation: \"n = poly.getNumRings();\""));

  te::gm::Polygon* g = te::v8::common::Unwrap<te::gm::Polygon>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getNumRings method!"));

  std::size_t n = g->getNumRings();

  ::v8::Handle<::v8::Integer> jn = ::v8::Integer::New(static_cast<int>(n));

  return hs.Close(jn);
}

::v8::Handle<::v8::Value> Polygon_SetNumRings(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use setNumRings method you must use object notation: \"poly.setNumRings(n);\""));

  te::gm::Polygon* g = te::v8::common::Unwrap<te::gm::Polygon>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in setNumRings method!"));

  if(args.Length() != 1 || args[0].IsEmpty() || !args[0]->IsInt32())
    return ::v8::ThrowException(::v8::String::New("Invalid parameter for setNumRings!"));

  int n = args[0]->ToInt32()->Value();

  g->setNumRings(n);

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> Polygon_GetInteriorRingN(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getInteriorRingN method you must use object notation: \"r = poly.getInteriorRingN(i);\""));

  te::gm::Polygon* g = te::v8::common::Unwrap<te::gm::Polygon>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getInteriorRingN method!"));

  if(args.Length() != 1 || args[0].IsEmpty() || !args[0]->IsInt32())
    return ::v8::ThrowException(::v8::String::New("Invalid parameter for getInteriorRingN!"));

  int n = args[0]->ToInt32()->Value();

  te::gm::LinearRing* ring = g->getInteriorRingN(n);

  ::v8::Local<::v8::Object> jsgeom = te::v8::common::Make(ring, te::v8::jsi::GetLinearRingTemplate, false);

  return hs.Close(jsgeom);
}

::v8::Handle<::v8::Value> Polygon_GetRingN(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getRingN method you must use object notation: \"r = poly.getRingN(i);\""));

  te::gm::Polygon* g = te::v8::common::Unwrap<te::gm::Polygon>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getRingN method!"));

  if(args.Length() != 1 || args[0].IsEmpty() || !args[0]->IsInt32())
    return ::v8::ThrowException(::v8::String::New("Invalid parameter for getRingN!"));

  int n = args[0]->ToInt32()->Value();

  te::gm::LinearRing* ring = g->getRingN(n);

  ::v8::Local<::v8::Object> jsgeom = te::v8::common::Make(ring, te::v8::jsi::GetLinearRingTemplate, false);

  return hs.Close(jsgeom);
}

::v8::Handle<::v8::Value> Polygon_SetRingN(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use setRingN method you must use object notation: \"poly.setRingN(n, ring);\""));

  te::gm::Polygon* g = te::v8::common::Unwrap<te::gm::Polygon>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in setRingN method!"));

  if(args.Length() != 2 || args[0].IsEmpty() || !args[0]->IsInt32() || args[1].IsEmpty() || !args[1]->IsObject())
    return ::v8::ThrowException(::v8::String::New("Invalid parameter for setNumRings!"));

  int n = args[0]->ToInt32()->Value();

  std::auto_ptr<te::gm::LinearRing> ring(te::v8::common::UnwrapAndLooseOwnership<te::gm::LinearRing>(args[1]->ToObject()));

  g->setRingN(n, ring.get());

  ring.release();

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> Polygon_RemoveRingN(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use removeRingN method you must use object notation: \"poly.removeRingN(i);\""));

  te::gm::Polygon* g = te::v8::common::Unwrap<te::gm::Polygon>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in removeRingN method!"));

  if(args.Length() != 1 || args[0].IsEmpty() || !args[0]->IsInt32())
    return ::v8::ThrowException(::v8::String::New("Invalid parameter for removeRingN!"));

  int n = args[0]->ToInt32()->Value();

  g->removeRingN(n);

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> Polygon_Add(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use add method you must use object notation: \"poly.add(ring);\""));

  te::gm::Polygon* g = te::v8::common::Unwrap<te::gm::Polygon>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in add method!"));

  if(args.Length() != 1 || args[0].IsEmpty() || !args[0]->IsObject())
    return ::v8::ThrowException(::v8::String::New("Invalid parameter for add!"));

  std::auto_ptr<te::gm::LinearRing> ring(te::v8::common::UnwrapAndLooseOwnership<te::gm::LinearRing>(args[0]->ToObject()));

  g->add(ring.get());

  ring.release();

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> Polygon_Clear(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use clear method you must use object notation: \"poly.clear();\""));

  te::gm::Polygon* g = te::v8::common::Unwrap<te::gm::Polygon>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getNumRings method!"));

  g->clear();

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> Polygon_GetRings(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getRings method you must use object notation: \"rings = poly.getRings();\""));

  te::gm::Polygon* g = te::v8::common::Unwrap<te::gm::Polygon>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getRings method!"));

  const std::vector<te::gm::LinearRing*>& rings = g->getRings();

  int len = static_cast<int>(rings.size());

  ::v8::Handle<::v8::Array> jrings = ::v8::Array::New(len);

  for(int i = 0; i < len; ++i)
    jrings->Set(i, te::v8::common::Make(rings[i], te::v8::jsi::GetLinearRingTemplate, false));

  return hs.Close(jrings);
}

::v8::Handle<::v8::Value> Polygon_Constructor(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(!args.IsConstructCall())
    return ::v8::ThrowException(::v8::String::New("In order to create a Polygon you need to call its constructor like: var mygc = new TePolygon(3, TE_OGC_POLYGON, 4326)."));

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("Polygon constructor must use object notation!"));

  if((args.Length() != 3) ||
      args[0].IsEmpty() || !args[0]->IsInt32() ||
      args[1].IsEmpty() || !args[1]->IsInt32() ||
      args[2].IsEmpty() || !args[2]->IsInt32())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in Polygon constructor method!!"));


  int nrings = args[0]->Int32Value();
  int geomType = args[1]->Int32Value();
  int srid = args[2]->ToInt32()->Int32Value();

  std::auto_ptr<te::gm::Polygon> g(new te::gm::Polygon(nrings, static_cast<te::gm::GeomType>(geomType), srid));

  ::v8::Local<::v8::Object> jg = te::v8::common::Make(g.get(), te::v8::jsi::GetPolygonTemplate, true);

  g.release();

  return hs.Close(jg);
}

void te::v8::jsi::RegisterPolygon(::v8::Local<::v8::Object>& global)
{
  ::v8::HandleScope hs;

  ::v8::Local<::v8::FunctionTemplate> jsgc = ::v8::FunctionTemplate::New(Polygon_Constructor);

  global->Set(::v8::String::New("TePolygon"), jsgc->GetFunction());
}

static ::v8::Persistent<::v8::FunctionTemplate> sg_polygon_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetPolygonTemplate()
{
  if(sg_polygon_template.IsEmpty())
  {
    ::v8::Persistent<::v8::FunctionTemplate>& surfaceTpl = GetSurfaceTemplate();
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    result->Inherit(surfaceTpl);

    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("getExteriorRing"), ::v8::FunctionTemplate::New(Polygon_GetExteriorRing));
    prototype->Set(::v8::String::NewSymbol("getNumInteriorRings"), ::v8::FunctionTemplate::New(Polygon_GetNumInteriorRings));
    prototype->Set(::v8::String::NewSymbol("getNumRings"), ::v8::FunctionTemplate::New(Polygon_GetNumRings));
    prototype->Set(::v8::String::NewSymbol("setNumRings"), ::v8::FunctionTemplate::New(Polygon_SetNumRings));
    prototype->Set(::v8::String::NewSymbol("getInteriorRingN"), ::v8::FunctionTemplate::New(Polygon_GetInteriorRingN));
    prototype->Set(::v8::String::NewSymbol("getRingN"), ::v8::FunctionTemplate::New(Polygon_GetRingN));
    prototype->Set(::v8::String::NewSymbol("setRingN"), ::v8::FunctionTemplate::New(Polygon_SetRingN));
    prototype->Set(::v8::String::NewSymbol("removeRingN"), ::v8::FunctionTemplate::New(Polygon_RemoveRingN));
    prototype->Set(::v8::String::NewSymbol("add"), ::v8::FunctionTemplate::New(Polygon_Add));
    prototype->Set(::v8::String::NewSymbol("push_back"), ::v8::FunctionTemplate::New(Polygon_Add));
    prototype->Set(::v8::String::NewSymbol("clear"), ::v8::FunctionTemplate::New(Polygon_Clear));
    prototype->Set(::v8::String::NewSymbol("getRings"), ::v8::FunctionTemplate::New(Polygon_GetRings));

    sg_polygon_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sg_polygon_template;
}


