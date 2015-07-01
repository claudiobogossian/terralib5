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
  \file DataSetTypePersistence.cpp
   
  \brief Utility functions to register the DataSetTypePersistence class into the Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../dataaccess/dataset/DataSetTypePersistence.h"
#include "../../common/Utils.h"
#include "DataAccess.h"

// Boost
#include <boost/cstdint.hpp>


::v8::Handle<::v8::Value> DataSetTypePersistence_Create(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetTypePersistence_Remove(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetTypePersistence_Drop(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetTypePersistence_Rename(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetTypePersistence_Add(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetTypePersistence_Update(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

static ::v8::Persistent<::v8::FunctionTemplate> sg_datasettypepersistence_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetDataSetTypePersistenceTemplate()
{
  if(sg_datasettypepersistence_template.IsEmpty())
  {
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate();

    prototype->Set(::v8::String::NewSymbol("create"), ::v8::FunctionTemplate::New(DataSetTypePersistence_Create));
    prototype->Set(::v8::String::NewSymbol("drop"), ::v8::FunctionTemplate::New(DataSetTypePersistence_Drop));
    prototype->Set(::v8::String::NewSymbol("rename"), ::v8::FunctionTemplate::New(DataSetTypePersistence_Rename));
    prototype->Set(::v8::String::NewSymbol("add"), ::v8::FunctionTemplate::New(DataSetTypePersistence_Add));
    prototype->Set(::v8::String::NewSymbol("update"), ::v8::FunctionTemplate::New(DataSetTypePersistence_Update));

    sg_datasettypepersistence_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sg_datasettypepersistence_template;
}
