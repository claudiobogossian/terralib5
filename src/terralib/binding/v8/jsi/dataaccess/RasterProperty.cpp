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
  \file RasterProperty.cpp
   
  \brief Utility functions to register the RasterProperty class into the Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../dataaccess/property/RasterProperty.h"
#include "../../common/Utils.h"
#include "DataAccess.h"

// Boost
#include <boost/cstdint.hpp>

::v8::Handle<::v8::Value> RasterProperty_GetExtent(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> RasterProperty_GetSRID(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> RasterProperty_SetSRID(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> RasterProperty_GetNLines(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> RasterProperty_GetNColumns(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> RasterProperty_GetResX(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> RasterProperty_GetResY(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> RasterProperty_GetNBands(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> RasterProperty_GetRasterBands(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> RasterProperty_SetRasterBand(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> RasterProperty_AddRasterBand(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> RasterProperty_SetLocation(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> RasterProperty_GetLocation(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> RasterProperty_IsInvalid(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> RasterProperty_Clone(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> RasterProperty_Constructor(const ::v8::Arguments& args)
{
  //::v8::HandleScope hs;

  //if(!args.IsConstructCall())
  //  return ::v8::ThrowException(::v8::String::New("In order to create a check constraint, you need to call its constructor like: var c = new TeRasterProperty(dt, id);"));

  //if(args.Holder().IsEmpty())
  //  return ::v8::ThrowException(::v8::String::New("The RasterProperty constructor must use object notation!"));

  //if(args.Length() != 0)
  //  return ::v8::ThrowException(::v8::String::New("The RasterProperty constructor has no parameters!"));

  //std::auto_ptr<te::da::RasterProperty> catalog(new te::da::RasterProperty());

  //::v8::Local<::v8::Object> jcatalog = te::v8::common::Make(catalog.get(), te::v8::jsi::GetRasterPropertyTemplate, true);

  //catalog.release();

  //return hs.Close(jcatalog);

  return ::v8::Undefined();
}

void te::v8::jsi::RegisterRasterProperty(::v8::Local<::v8::Object>& global)
{
  ::v8::Local<::v8::FunctionTemplate> jrasterproperty = ::v8::FunctionTemplate::New(RasterProperty_Constructor);

  global->Set(::v8::String::New("TeRasterProperty"), jrasterproperty->GetFunction());
}

static ::v8::Persistent<::v8::FunctionTemplate> sdset_rasterproperty_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetRasterPropertyTemplate()
{
  if(sdset_rasterproperty_template.IsEmpty())
  {
    ::v8::Persistent<::v8::FunctionTemplate>& pTpl = GetSimplePropertyTemplate();
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    result->Inherit(pTpl);

    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("getExtent"), ::v8::FunctionTemplate::New(RasterProperty_GetExtent));
    prototype->Set(::v8::String::NewSymbol("getSRID"), ::v8::FunctionTemplate::New(RasterProperty_GetSRID));
    prototype->Set(::v8::String::NewSymbol("setSRID"), ::v8::FunctionTemplate::New(RasterProperty_SetSRID));
    prototype->Set(::v8::String::NewSymbol("getNLines"), ::v8::FunctionTemplate::New(RasterProperty_GetNLines));
    prototype->Set(::v8::String::NewSymbol("getNColumns"), ::v8::FunctionTemplate::New(RasterProperty_GetNColumns));
    prototype->Set(::v8::String::NewSymbol("getResX"), ::v8::FunctionTemplate::New(RasterProperty_GetResX));
    prototype->Set(::v8::String::NewSymbol("getResY"), ::v8::FunctionTemplate::New(RasterProperty_GetResY));
    prototype->Set(::v8::String::NewSymbol("getNBands"), ::v8::FunctionTemplate::New(RasterProperty_GetNBands));
    prototype->Set(::v8::String::NewSymbol("getRasterBands"), ::v8::FunctionTemplate::New(RasterProperty_GetRasterBands));
    prototype->Set(::v8::String::NewSymbol("setRasterBand"), ::v8::FunctionTemplate::New(RasterProperty_SetRasterBand));
    prototype->Set(::v8::String::NewSymbol("addRasterBand"), ::v8::FunctionTemplate::New(RasterProperty_AddRasterBand));
    prototype->Set(::v8::String::NewSymbol("setLocation"), ::v8::FunctionTemplate::New(RasterProperty_SetLocation));
    prototype->Set(::v8::String::NewSymbol("getLocation"), ::v8::FunctionTemplate::New(RasterProperty_GetLocation));
    prototype->Set(::v8::String::NewSymbol("isInvalid"), ::v8::FunctionTemplate::New(RasterProperty_IsInvalid));
    prototype->Set(::v8::String::NewSymbol("clone"), ::v8::FunctionTemplate::New(RasterProperty_Clone));

    sdset_rasterproperty_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sdset_rasterproperty_template;
}
