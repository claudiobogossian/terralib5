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
  \file GeometryFactory.cpp
   
  \brief Utility functions to register the GeometryFactory class into Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../geometry/GeometryFactory.h"
#include "../../common/Utils.h"
#include "Geometry.h"

::v8::Handle<::v8::Value> GeometryFactory_Make(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

// check and get arguments
  if((args.Length() < 2) ||
     (args[0].IsEmpty()) ||
     (args[1].IsEmpty()))
    return ::v8::ThrowException(::v8::String::New("Missing parameter for TeGeometryFactory. The correct syntax is: var my_geom = TeGeometryFactory.make(geomtype, srid)"));

  int geomType = args[0]->Int32Value();
  int srid = args[1]->Int32Value();

  std::auto_ptr<te::gm::Geometry> geom(te::gm::GeometryFactory::make(static_cast<te::gm::GeomType>(geomType), srid));

// create JavaScript object
  ::v8::Local<::v8::Object> jsgeom = te::v8::jsi::Geometry_Make(geom.get(), true);

  geom.release();
  
  return hs.Close(jsgeom);
}

void te::v8::jsi::RegisterGeometryFactory(::v8::Local<::v8::Object>& global)
{
  ::v8::HandleScope hs;

  ::v8::Handle<::v8::ObjectTemplate> gfactory = ::v8::ObjectTemplate::New();

  gfactory->Set(::v8::String::NewSymbol("make"), ::v8::FunctionTemplate::New(GeometryFactory_Make));

  global->Set(::v8::String::New("TeGeometryFactory"), gfactory->NewInstance());
}

