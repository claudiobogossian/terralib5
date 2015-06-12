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
  \file DataSetPersistence.cpp
   
  \brief Utility functions to register the DataSetPersistence class into the Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../dataaccess/dataset/DataSetPersistence.h"
#include "../../common/Utils.h"
#include "DataAccess.h"

// STL
//#include <memory>

// Boost
#include <boost/cstdint.hpp>


::v8::Handle<::v8::Value> DataSetPersistence_Create(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetPersistence_Remove(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetPersistence_Add(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetPersistence_Update(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetPersistence_GetTransactor(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

static ::v8::Persistent<::v8::FunctionTemplate> sdset_datasetpersistence_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetDataSetPersistenceTemplate()
{
  if(sdset_datasetpersistence_template.IsEmpty())
  {
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate();

    prototype->Set(::v8::String::NewSymbol("create"), ::v8::FunctionTemplate::New(DataSetPersistence_Create));
    prototype->Set(::v8::String::NewSymbol("remove"), ::v8::FunctionTemplate::New(DataSetPersistence_Remove));
    prototype->Set(::v8::String::NewSymbol("add"), ::v8::FunctionTemplate::New(DataSetPersistence_Add));
    prototype->Set(::v8::String::NewSymbol("update"), ::v8::FunctionTemplate::New(DataSetPersistence_Update));
    prototype->Set(::v8::String::NewSymbol("getTransactor"), ::v8::FunctionTemplate::New(DataSetPersistence_GetTransactor));

    sdset_datasetpersistence_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sdset_datasetpersistence_template;
}
