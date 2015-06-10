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
  \file GeometryCollection.cpp
   
  \brief Utility functions to register the GeometryCollection class into Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../geometry/GeometryCollection.h"
#include "../../common/Utils.h"
#include "Geometry.h"

// Boost
#include <boost/cstdint.hpp>

::v8::Handle<::v8::Value> GeometryCollection_GetNumGeometries(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getNumGeometries method you must use object notation: \"ng = obj.getNumGeometries();\""));

  te::gm::GeometryCollection* g = te::v8::common::Unwrap<te::gm::GeometryCollection>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getNumGeometries method!"));

  std::size_t n = g->getNumGeometries();

  ::v8::Handle<::v8::Integer> jn = ::v8::Integer::New(static_cast<boost::int32_t>(n));

  return hs.Close(jn);
}

::v8::Handle<::v8::Value> GeometryCollection_SetNumGeometries(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use setNumGeometries method you must use object notation: \"obj.setNumGeometries(2);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsInt32())
    return ::v8::ThrowException(::v8::String::New("In order to use setNumGeometries method you must use object notation: \"obj.setNumGeometries(2);\""));

  te::gm::GeometryCollection* g = te::v8::common::Unwrap<te::gm::GeometryCollection>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in setNumGeometries method!"));

  int n = args[0]->ToInt32()->Value();
  
  g->setNumGeometries(n);

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> GeometryCollection_GetGeometryN(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getGeometryN method you must use object notation: \"gn = obj.getGeometryN(2);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsInt32())
    return ::v8::ThrowException(::v8::String::New("In order to use getGeometryN method you must use object notation: \"gn = obj.getGeometryN(2);\""));

  te::gm::GeometryCollection* thisGeom = te::v8::common::Unwrap<te::gm::GeometryCollection>(args.Holder());

  if(thisGeom == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getGeometryN method!"));

  int i = args[0]->ToInt32()->Value();

  te::gm::Geometry* g = thisGeom->getGeometryN(i);
  
  ::v8::Local<::v8::Object> jg = te::v8::jsi::Geometry_Make(g, false);

  return hs.Close(jg);
}

::v8::Handle<::v8::Value> GeometryCollection_SetGeometryN(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use setGeometryN method you must use object notation: \"obj.setGeometryN(i, new_geom);\""));

  if((args.Length() != 2) || args[0].IsEmpty() || !args[0]->IsInt32() || args[1].IsEmpty() || !args[1]->IsObject())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in setGeometryN method!"));

  te::gm::GeometryCollection* thisGeom = te::v8::common::Unwrap<te::gm::GeometryCollection>(args.Holder());

  if(thisGeom == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in setGeometryN method!"));

  int i = args[0]->ToInt32()->Value();
  te::gm::Geometry* rhsGeom = te::v8::common::UnwrapAndLooseOwnership<te::gm::Geometry>(args[1]->ToObject());

  if(rhsGeom == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in setGeometryN method!"));

  thisGeom->setGeometryN(i, rhsGeom);

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> GeometryCollection_RemoveGeometryN(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use removeGeometryN method you must use object notation: \"obj.removeGeometryN(2);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsInt32())
    return ::v8::ThrowException(::v8::String::New("In order to use removeGeometryN method you must use object notation: \"obj.removeGeometryN(2);\""));

  te::gm::GeometryCollection* g = te::v8::common::Unwrap<te::gm::GeometryCollection>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in removeGeometryN method!"));

  int n = args[0]->ToInt32()->Value();
  
  g->removeGeometryN(n);

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> GeometryCollection_Add(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use add method you must use object notation: \"obj.add(new_geom);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsObject())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in add method!"));

  te::gm::GeometryCollection* thisGeom = te::v8::common::Unwrap<te::gm::GeometryCollection>(args.Holder());

  if(thisGeom == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in add method!"));

  te::gm::Geometry* rhsGeom = te::v8::common::UnwrapAndLooseOwnership<te::gm::Geometry>(args[0]->ToObject());

  if(rhsGeom == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in add method!"));

  thisGeom->add(rhsGeom);

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> GeometryCollection_Clear(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use clear method you must use object notation: \"obj.clear();\""));

  te::gm::GeometryCollection* g = te::v8::common::Unwrap<te::gm::GeometryCollection>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in clear method!"));

  g->clear();

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> GeometryCollection_Constructor(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(!args.IsConstructCall())
    return ::v8::ThrowException(::v8::String::New("In order to create a GeometryCollection you need to call its constructor like: var mygc = new TeGeometryCollection(2, TE_OGC_GEOMETRYCOLLECTION, 4326)."));

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("GeometryCollection constructor must use object notation!"));

  if((args.Length() != 3) ||
      args[0].IsEmpty() || !args[0]->IsInt32() ||
      args[1].IsEmpty() || !args[1]->IsInt32() ||
      args[2].IsEmpty() || !args[2]->IsInt32())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in GeometryCollection constructor method!!"));

  int ngeom = args[0]->Int32Value();
  int geomType = args[1]->Int32Value();
  int srid = args[2]->ToInt32()->Int32Value();

  std::auto_ptr<te::gm::GeometryCollection> g(new te::gm::GeometryCollection(ngeom, static_cast<te::gm::GeomType>(geomType), srid));

  ::v8::Local<::v8::Object> jg = te::v8::common::Make(g.get(), te::v8::jsi::GetGeometryCollectionTemplate, true);

  g.release();

  return hs.Close(jg);
}

void te::v8::jsi::RegisterGeometryCollection(::v8::Local<::v8::Object>& global)
{
  ::v8::HandleScope hs;

  ::v8::Local<::v8::FunctionTemplate> jsgc = ::v8::FunctionTemplate::New(GeometryCollection_Constructor);

  global->Set(::v8::String::New("TeGeometryCollection"), jsgc->GetFunction());
}

static ::v8::Persistent<::v8::FunctionTemplate> sg_gc_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetGeometryCollectionTemplate()
{
  if(sg_gc_template.IsEmpty())
  {
    ::v8::Persistent<::v8::FunctionTemplate>& gTpl = GetGeometryTemplate();
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    result->Inherit(gTpl);

    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("getNumGeometries"), ::v8::FunctionTemplate::New(GeometryCollection_GetNumGeometries));
    prototype->Set(::v8::String::NewSymbol("setNumGeometries"), ::v8::FunctionTemplate::New(GeometryCollection_SetNumGeometries));
    prototype->Set(::v8::String::NewSymbol("getGeometryN"), ::v8::FunctionTemplate::New(GeometryCollection_GetGeometryN));
    prototype->Set(::v8::String::NewSymbol("setGeometryN"), ::v8::FunctionTemplate::New(GeometryCollection_SetGeometryN));
    prototype->Set(::v8::String::NewSymbol("removeGeometryN"), ::v8::FunctionTemplate::New(GeometryCollection_RemoveGeometryN));
    prototype->Set(::v8::String::NewSymbol("add"), ::v8::FunctionTemplate::New(GeometryCollection_Add));
    prototype->Set(::v8::String::NewSymbol("clear"), ::v8::FunctionTemplate::New(GeometryCollection_Clear));

    sg_gc_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sg_gc_template;
}

