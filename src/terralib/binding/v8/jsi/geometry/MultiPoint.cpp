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
  \file MultiPoint.cpp
   
  \brief Utility functions to register the MultiPoint class into Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../geometry/MultiPoint.h"
#include "../../common/Utils.h"
#include "Geometry.h"

::v8::Handle<::v8::Value> MultiPoint_Constructor(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(!args.IsConstructCall())
    return ::v8::ThrowException(::v8::String::New("In order to create a MultiPoint you need to call its constructor like: var mygc = new TeMultiPoint(2, TE_OGC_MULTIPOINT, 4326)."));

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("MultiPoint constructor must use object notation!"));

  if((args.Length() != 3) ||
      args[0].IsEmpty() || !args[0]->IsInt32() ||
      args[1].IsEmpty() || !args[1]->IsInt32() ||
      args[2].IsEmpty() || !args[2]->IsInt32())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in MultiPoint constructor method!!"));

  int ngeom = args[0]->Int32Value();
  int geomType = args[1]->Int32Value();
  int srid = args[2]->ToInt32()->Int32Value();

  std::auto_ptr<te::gm::MultiPoint> g(new te::gm::MultiPoint(ngeom, static_cast<te::gm::GeomType>(geomType), srid));

  ::v8::Local<::v8::Object> jg = te::v8::common::Make(g.get(), te::v8::jsi::GetMultiPointTemplate, true);

  g.release();

  return hs.Close(jg);
}

void te::v8::jsi::RegisterMultiPoint(::v8::Local<::v8::Object>& global)
{
  ::v8::HandleScope hs;

  ::v8::Local<::v8::FunctionTemplate> jsgc = ::v8::FunctionTemplate::New(MultiPoint_Constructor);

  global->Set(::v8::String::New("TeMultiPoint"), jsgc->GetFunction());
}

static ::v8::Persistent<::v8::FunctionTemplate> sg_mpoint_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetMultiPointTemplate()
{
  if(sg_mpoint_template.IsEmpty())
  {
    ::v8::Persistent<::v8::FunctionTemplate>& geomTpl = GetGeometryCollectionTemplate();
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    result->Inherit(geomTpl);

    sg_mpoint_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sg_mpoint_template;
}


