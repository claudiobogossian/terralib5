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
  \file MultiLineString.cpp
   
  \brief Utility functions to register the MultiLineString class into Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../geometry/MultiLineString.h"
#include "../../common/Utils.h"
#include "Geometry.h"

::v8::Handle<::v8::Value> MultiLineString_Constructor(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(!args.IsConstructCall())
    return ::v8::ThrowException(::v8::String::New("In order to create a MultiLineString you need to call its constructor like: var mygc = new TeMultiLineString(2, TE_OGC_MULTILINESTRING, 4326)."));

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("MultiLineString constructor must use object notation!"));

  if((args.Length() != 3) ||
      args[0].IsEmpty() || !args[0]->IsInt32() ||
      args[1].IsEmpty() || !args[1]->IsInt32() ||
      args[2].IsEmpty() || !args[2]->IsInt32())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in MultiLineString constructor method!!"));

  int ngeom = args[0]->Int32Value();
  int geomType = args[1]->Int32Value();
  int srid = args[2]->ToInt32()->Int32Value();

  std::auto_ptr<te::gm::MultiLineString> g(new te::gm::MultiLineString(ngeom, static_cast<te::gm::GeomType>(geomType), srid));

  ::v8::Local<::v8::Object> jg = te::v8::common::Make(g.get(), te::v8::jsi::GetMultiLineStringTemplate, true);

  g.release();

  return hs.Close(jg);
}

void te::v8::jsi::RegisterMultiLineString(::v8::Local<::v8::Object>& global)
{
  ::v8::HandleScope hs;

  ::v8::Local<::v8::FunctionTemplate> jsgc = ::v8::FunctionTemplate::New(MultiLineString_Constructor);

  global->Set(::v8::String::New("TeMultiLineString"), jsgc->GetFunction());
}

static ::v8::Persistent<::v8::FunctionTemplate> sg_mline_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetMultiLineStringTemplate()
{
  if(sg_mline_template.IsEmpty())
  {
    ::v8::Persistent<::v8::FunctionTemplate>& mcurveTpl = GetMultiCurveTemplate();
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    result->Inherit(mcurveTpl);

    sg_mline_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sg_mline_template;
}

