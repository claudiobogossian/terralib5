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
  \file DataSet.cpp
   
  \brief Utility functions to register the DataSet class into the Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../common/Exception.h"
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/dataset/DataSet.h"
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../geometry/Geometry.h"
#include "../../common/Utils.h"
#include "../datatype/DataType.h"
#include "../geometry/Geometry.h"
#include "DataAccess.h"

// STL
#include <memory>

// Boost
#include <boost/cstdint.hpp>


::v8::Handle<::v8::Value> DataSet_GetTraverseType(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_GetAccessPolicy(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_GetType(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("In order to use the getType method you must use the object notation: \"dt = dataset.getType();\"")));

  te::da::DataSet* d = te::v8::common::Unwrap<te::da::DataSet>(args.Holder());

  if(d == 0)
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Invalid dataset in getType method!")));

  try
  {
    te::da::DataSetType* dt = d->getType();
    ::v8::Local<::v8::Object> jsdt = te::v8::common::Make(dt, te::v8::jsi::GetDataSetTypeTemplate, false);
    return hs.Close(jsdt);
  }
  catch(const std::exception& e)
  {
    return ::v8::ThrowException(::v8::String::New(e.what()));
  }
  catch(...)
  {
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Unexpected exception in getType method!")));
  }
}

::v8::Handle<::v8::Value> DataSet_GetTransactor(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_LoadTypeInfo(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_GetParent(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_GetExtent(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_SetFilter(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_GetItem(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_Add(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_IsEmpty(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_Size(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_MoveNext(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("In order to use the moveNext method you must use the object notation: \"result = dataset.moveNext();\"")));

  te::da::DataSet* d = te::v8::common::Unwrap<te::da::DataSet>(args.Holder());

  if(d == 0)
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Invalid dataset in moveNext method!")));

  try
  {
    bool result = d->moveNext();
    ::v8::Handle<::v8::Boolean> jsresult = ::v8::Boolean::New(result);
    return hs.Close(jsresult);
  }
  catch(const std::exception& e)
  {
    return ::v8::ThrowException(::v8::String::New(e.what()));
  }
  catch(...)
  {
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Unexpected exception in moveNext method!")));
  }
}

::v8::Handle<::v8::Value> DataSet_MovePrevious(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_MoveFirst(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_MoveBeforeFirst(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_MoveLast(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_MoveAfterLast(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_Move(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_IsAtBegin(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_IsBeforeBegin(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_IsAtEnd(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_IsAfterLast(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_GetChar(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_SetChar(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_GetUChar(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_SetUChar(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_GetInt16(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_SetInt16(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_GetInt32(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_SetInt32(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_GetInt64(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_SetInt64(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_GetBool(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_SetBool(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_GetFloat(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_SetFloat(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_GetDouble(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_SetDouble(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_GetNumeric(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_SetNumeric(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_GetString(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_SetString(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_GetByteArray(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_SetByteArray(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_GetGeometry(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("In order to use the getGeometry method you must use the object notation: \"geom = obj.getGeometry(i);\"")));

  if(args.Length() != 1 || args[0].IsEmpty())
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Wrong parameters in getGeometry method!")));

  te::da::DataSet* d = te::v8::common::Unwrap<te::da::DataSet>(args.Holder());

  if(d == 0)
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Invalid dataset in getGeometry method!")));

  try
  {
    if(args[0]->IsInt32())
    {
      int col = args[0]->ToInt32()->Value();
      std::auto_ptr<te::gm::Geometry> geom(d->getGeometry(col));
      ::v8::Local<::v8::Object> jsgeom = te::v8::jsi::Geometry_Make(geom.get(), true);
      geom.release();
      return hs.Close(jsgeom);
    }
    else if(args[0]->IsString())
    {
      v8::String::Utf8Value jscolname(args[0]->ToString());
      std::auto_ptr<te::gm::Geometry> geom(d->getGeometry(*jscolname));
      ::v8::Local<::v8::Object> jsgeom = te::v8::jsi::Geometry_Make(geom.get(), true);
      geom.release();
      return hs.Close(jsgeom);
    }

    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Wrong parameter in getGeometry method!")));
  }
  catch(const std::exception& e)
  {
    return ::v8::ThrowException(::v8::String::New(e.what()));
  }
  catch(...)
  {
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Unexpected exception in getDataSet method!")));
  }
}

::v8::Handle<::v8::Value> DataSet_SetGeometry(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_GetRaster(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_SetRaster(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_GetDateTime(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_SetDateTime(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_GetArray(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_GetWKB(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_GetDataSet(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_SetDataSet(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_GetValue(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_SetValue(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_GetAsString(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSet_IsNull(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

static ::v8::Persistent<::v8::FunctionTemplate> sg_dataset_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetDataSetTemplate()
{
  if(sg_dataset_template.IsEmpty())
  {
    ::v8::Persistent<::v8::FunctionTemplate>& abstractDataTpl = GetAbstractDataTemplate();
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    result->Inherit(abstractDataTpl);

    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("getTraverseType"), ::v8::FunctionTemplate::New(DataSet_GetTraverseType));
    prototype->Set(::v8::String::NewSymbol("getAccessPolicy"), ::v8::FunctionTemplate::New(DataSet_GetAccessPolicy));
    prototype->Set(::v8::String::NewSymbol("getType"), ::v8::FunctionTemplate::New(DataSet_GetType));
    prototype->Set(::v8::String::NewSymbol("getTransactor"), ::v8::FunctionTemplate::New(DataSet_GetTransactor));
    prototype->Set(::v8::String::NewSymbol("loadTypeInfo"), ::v8::FunctionTemplate::New(DataSet_LoadTypeInfo));
    prototype->Set(::v8::String::NewSymbol("getParent"), ::v8::FunctionTemplate::New(DataSet_GetParent));
    prototype->Set(::v8::String::NewSymbol("getExtent"), ::v8::FunctionTemplate::New(DataSet_GetExtent));
    prototype->Set(::v8::String::NewSymbol("setFilter"), ::v8::FunctionTemplate::New(DataSet_SetFilter));
    prototype->Set(::v8::String::NewSymbol("getItem"), ::v8::FunctionTemplate::New(DataSet_GetItem));
    prototype->Set(::v8::String::NewSymbol("add"), ::v8::FunctionTemplate::New(DataSet_Add));
    prototype->Set(::v8::String::NewSymbol("isEmpty"), ::v8::FunctionTemplate::New(DataSet_IsEmpty));
    prototype->Set(::v8::String::NewSymbol("size"), ::v8::FunctionTemplate::New(DataSet_Size));
    prototype->Set(::v8::String::NewSymbol("moveNext"), ::v8::FunctionTemplate::New(DataSet_MoveNext));
    prototype->Set(::v8::String::NewSymbol("movePrevious"), ::v8::FunctionTemplate::New(DataSet_MovePrevious));
    prototype->Set(::v8::String::NewSymbol("moveFirst"), ::v8::FunctionTemplate::New(DataSet_MoveFirst));
    prototype->Set(::v8::String::NewSymbol("moveBeforeFirst"), ::v8::FunctionTemplate::New(DataSet_MoveBeforeFirst));
    prototype->Set(::v8::String::NewSymbol("moveLast"), ::v8::FunctionTemplate::New(DataSet_MoveLast));
    prototype->Set(::v8::String::NewSymbol("moveAfterLast"), ::v8::FunctionTemplate::New(DataSet_MoveAfterLast));
    prototype->Set(::v8::String::NewSymbol("move"), ::v8::FunctionTemplate::New(DataSet_Move));
    prototype->Set(::v8::String::NewSymbol("isAtBegin"), ::v8::FunctionTemplate::New(DataSet_IsAtBegin));
    prototype->Set(::v8::String::NewSymbol("isBeforeBegin"), ::v8::FunctionTemplate::New(DataSet_IsBeforeBegin));
    prototype->Set(::v8::String::NewSymbol("isAtEnd"), ::v8::FunctionTemplate::New(DataSet_IsAtEnd));
    prototype->Set(::v8::String::NewSymbol("isAfterLast"), ::v8::FunctionTemplate::New(DataSet_IsAfterLast));
    prototype->Set(::v8::String::NewSymbol("getChar"), ::v8::FunctionTemplate::New(DataSet_GetChar));
    prototype->Set(::v8::String::NewSymbol("setChar"), ::v8::FunctionTemplate::New(DataSet_SetChar));
    prototype->Set(::v8::String::NewSymbol("getUChar"), ::v8::FunctionTemplate::New(DataSet_GetUChar));
    prototype->Set(::v8::String::NewSymbol("setUChar"), ::v8::FunctionTemplate::New(DataSet_SetUChar));
    prototype->Set(::v8::String::NewSymbol("getInt16"), ::v8::FunctionTemplate::New(DataSet_GetInt16));
    prototype->Set(::v8::String::NewSymbol("setInt16"), ::v8::FunctionTemplate::New(DataSet_SetInt16));
    prototype->Set(::v8::String::NewSymbol("getInt32"), ::v8::FunctionTemplate::New(DataSet_GetInt32));
    prototype->Set(::v8::String::NewSymbol("setInt32"), ::v8::FunctionTemplate::New(DataSet_SetInt32));
    prototype->Set(::v8::String::NewSymbol("getInt64"), ::v8::FunctionTemplate::New(DataSet_GetInt64));
    prototype->Set(::v8::String::NewSymbol("setInt64"), ::v8::FunctionTemplate::New(DataSet_SetInt64));
    prototype->Set(::v8::String::NewSymbol("getBool"), ::v8::FunctionTemplate::New(DataSet_GetBool));
    prototype->Set(::v8::String::NewSymbol("setBool"), ::v8::FunctionTemplate::New(DataSet_SetBool));
    prototype->Set(::v8::String::NewSymbol("getFloat"), ::v8::FunctionTemplate::New(DataSet_GetFloat));
    prototype->Set(::v8::String::NewSymbol("setFloat"), ::v8::FunctionTemplate::New(DataSet_SetFloat));
    prototype->Set(::v8::String::NewSymbol("getDouble"), ::v8::FunctionTemplate::New(DataSet_GetDouble));
    prototype->Set(::v8::String::NewSymbol("setDouble"), ::v8::FunctionTemplate::New(DataSet_SetDouble));
    prototype->Set(::v8::String::NewSymbol("getNumeric"), ::v8::FunctionTemplate::New(DataSet_GetNumeric));
    prototype->Set(::v8::String::NewSymbol("setNumeric"), ::v8::FunctionTemplate::New(DataSet_SetNumeric));
    prototype->Set(::v8::String::NewSymbol("getString"), ::v8::FunctionTemplate::New(DataSet_GetString));
    prototype->Set(::v8::String::NewSymbol("setString"), ::v8::FunctionTemplate::New(DataSet_SetString));
    prototype->Set(::v8::String::NewSymbol("getByteArray"), ::v8::FunctionTemplate::New(DataSet_GetByteArray));
    prototype->Set(::v8::String::NewSymbol("setByteArray"), ::v8::FunctionTemplate::New(DataSet_SetByteArray));
    prototype->Set(::v8::String::NewSymbol("getGeometry"), ::v8::FunctionTemplate::New(DataSet_GetGeometry));
    prototype->Set(::v8::String::NewSymbol("setGeometry"), ::v8::FunctionTemplate::New(DataSet_SetGeometry));
    prototype->Set(::v8::String::NewSymbol("getRaster"), ::v8::FunctionTemplate::New(DataSet_GetRaster));
    prototype->Set(::v8::String::NewSymbol("setRaster"), ::v8::FunctionTemplate::New(DataSet_SetRaster));
    prototype->Set(::v8::String::NewSymbol("getDateTime"), ::v8::FunctionTemplate::New(DataSet_GetDateTime));
    prototype->Set(::v8::String::NewSymbol("setDateTime"), ::v8::FunctionTemplate::New(DataSet_SetDateTime));
    prototype->Set(::v8::String::NewSymbol("getArray"), ::v8::FunctionTemplate::New(DataSet_GetArray));
    prototype->Set(::v8::String::NewSymbol("getWKB"), ::v8::FunctionTemplate::New(DataSet_GetWKB));
    prototype->Set(::v8::String::NewSymbol("getDataSet"), ::v8::FunctionTemplate::New(DataSet_GetDataSet));
    prototype->Set(::v8::String::NewSymbol("setDataSet"), ::v8::FunctionTemplate::New(DataSet_SetDataSet));
    prototype->Set(::v8::String::NewSymbol("getValue"), ::v8::FunctionTemplate::New(DataSet_GetValue));
    prototype->Set(::v8::String::NewSymbol("setValue"), ::v8::FunctionTemplate::New(DataSet_SetValue));
    prototype->Set(::v8::String::NewSymbol("getAsString"), ::v8::FunctionTemplate::New(DataSet_GetAsString));
    prototype->Set(::v8::String::NewSymbol("isNull"), ::v8::FunctionTemplate::New(DataSet_IsNull));

    sg_dataset_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sg_dataset_template;
}
