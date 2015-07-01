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
  \file DataSetItem.cpp
   
  \brief Utility functions to register the DataSetItem class into the Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../dataaccess/dataset/DataSetItem.h"
#include "../../common/Utils.h"
#include "DataAccess.h"

// STL
#include <memory>

// Boost
#include <boost/cstdint.hpp>


::v8::Handle<::v8::Value> DataSetItem_GetType(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_GetParent(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_GetChar(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_SetChar(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_GetUChar(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_SetUChar(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_GetInt16(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_SetInt16(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_GetInt32(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_SetInt32(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_GetInt64(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_SetInt64(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_GetBool(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_SetBool(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_GetFloat(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_SetFloat(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_GetDouble(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_SetDouble(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_GetNumeric(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_SetNumeric(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_GetString(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_SetString(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_GetByteArray(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_SetByteArray(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_GetGeometry(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_SetGeometry(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_GetRaster(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_SetRaster(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_GetDateTime(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_SetDateTime(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_GetArray(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_GetWKB(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_GetDataSet(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_SetDataSet(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_GetValue(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_SetValue(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_GetAsString(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetItem_IsNull(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

static ::v8::Persistent<::v8::FunctionTemplate> sg_datasetitem_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetDataSetItemTemplate()
{
  if(sg_datasetitem_template.IsEmpty())
  {
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate();

    prototype->Set(::v8::String::NewSymbol("getType"), ::v8::FunctionTemplate::New(DataSetItem_GetType));
    prototype->Set(::v8::String::NewSymbol("getParent"), ::v8::FunctionTemplate::New(DataSetItem_GetParent));
    prototype->Set(::v8::String::NewSymbol("getChar"), ::v8::FunctionTemplate::New(DataSetItem_GetChar));
    prototype->Set(::v8::String::NewSymbol("setChar"), ::v8::FunctionTemplate::New(DataSetItem_SetChar));
    prototype->Set(::v8::String::NewSymbol("getUChar"), ::v8::FunctionTemplate::New(DataSetItem_GetUChar));
    prototype->Set(::v8::String::NewSymbol("setUChar"), ::v8::FunctionTemplate::New(DataSetItem_SetUChar));
    prototype->Set(::v8::String::NewSymbol("getInt16"), ::v8::FunctionTemplate::New(DataSetItem_GetInt16));
    prototype->Set(::v8::String::NewSymbol("setInt16"), ::v8::FunctionTemplate::New(DataSetItem_SetInt16));
    prototype->Set(::v8::String::NewSymbol("getInt32"), ::v8::FunctionTemplate::New(DataSetItem_GetInt32));
    prototype->Set(::v8::String::NewSymbol("setInt32"), ::v8::FunctionTemplate::New(DataSetItem_SetInt32));
    prototype->Set(::v8::String::NewSymbol("getInt64"), ::v8::FunctionTemplate::New(DataSetItem_GetInt64));
    prototype->Set(::v8::String::NewSymbol("setInt64"), ::v8::FunctionTemplate::New(DataSetItem_SetInt64));
    prototype->Set(::v8::String::NewSymbol("getBool"), ::v8::FunctionTemplate::New(DataSetItem_GetBool));
    prototype->Set(::v8::String::NewSymbol("setBool"), ::v8::FunctionTemplate::New(DataSetItem_SetBool));
    prototype->Set(::v8::String::NewSymbol("getFloat"), ::v8::FunctionTemplate::New(DataSetItem_GetFloat));
    prototype->Set(::v8::String::NewSymbol("setFloat"), ::v8::FunctionTemplate::New(DataSetItem_SetFloat));
    prototype->Set(::v8::String::NewSymbol("getDouble"), ::v8::FunctionTemplate::New(DataSetItem_GetDouble));
    prototype->Set(::v8::String::NewSymbol("setDouble"), ::v8::FunctionTemplate::New(DataSetItem_SetDouble));
    prototype->Set(::v8::String::NewSymbol("getNumeric"), ::v8::FunctionTemplate::New(DataSetItem_GetNumeric));
    prototype->Set(::v8::String::NewSymbol("setNumeric"), ::v8::FunctionTemplate::New(DataSetItem_SetNumeric));
    prototype->Set(::v8::String::NewSymbol("getString"), ::v8::FunctionTemplate::New(DataSetItem_GetString));
    prototype->Set(::v8::String::NewSymbol("setString"), ::v8::FunctionTemplate::New(DataSetItem_SetString));
    prototype->Set(::v8::String::NewSymbol("getByteArray"), ::v8::FunctionTemplate::New(DataSetItem_GetByteArray));
    prototype->Set(::v8::String::NewSymbol("setByteArray"), ::v8::FunctionTemplate::New(DataSetItem_SetByteArray));
    prototype->Set(::v8::String::NewSymbol("getGeometry"), ::v8::FunctionTemplate::New(DataSetItem_GetGeometry));
    prototype->Set(::v8::String::NewSymbol("setGeometry"), ::v8::FunctionTemplate::New(DataSetItem_SetGeometry));
    prototype->Set(::v8::String::NewSymbol("getRaster"), ::v8::FunctionTemplate::New(DataSetItem_GetRaster));
    prototype->Set(::v8::String::NewSymbol("setRaster"), ::v8::FunctionTemplate::New(DataSetItem_SetRaster));
    prototype->Set(::v8::String::NewSymbol("getDateTime"), ::v8::FunctionTemplate::New(DataSetItem_GetDateTime));
    prototype->Set(::v8::String::NewSymbol("setDateTime"), ::v8::FunctionTemplate::New(DataSetItem_SetDateTime));
    prototype->Set(::v8::String::NewSymbol("getArray"), ::v8::FunctionTemplate::New(DataSetItem_GetArray));
    prototype->Set(::v8::String::NewSymbol("getWKB"), ::v8::FunctionTemplate::New(DataSetItem_GetWKB));
    prototype->Set(::v8::String::NewSymbol("getDataSet"), ::v8::FunctionTemplate::New(DataSetItem_GetDataSet));
    prototype->Set(::v8::String::NewSymbol("setDataSet"), ::v8::FunctionTemplate::New(DataSetItem_SetDataSet));
    prototype->Set(::v8::String::NewSymbol("getValue"), ::v8::FunctionTemplate::New(DataSetItem_GetValue));
    prototype->Set(::v8::String::NewSymbol("setValue"), ::v8::FunctionTemplate::New(DataSetItem_SetValue));
    prototype->Set(::v8::String::NewSymbol("getAsString"), ::v8::FunctionTemplate::New(DataSetItem_GetAsString));
    prototype->Set(::v8::String::NewSymbol("isNull"), ::v8::FunctionTemplate::New(DataSetItem_IsNull));

    sg_datasetitem_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sg_datasetitem_template;
}
