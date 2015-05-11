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
  \file DataSetType.cpp
   
  \brief Utility functions to register the DataSetType class into the Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../common/Exception.h"
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../common/Utils.h"
#include "DataAccess.h"

// Boost
#include <boost/cstdint.hpp>

::v8::Handle<::v8::Value> DataSetType_GetTitle(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_SetTitle(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_HasGeom(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_HasDefaultGeom(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_GetDefaultGeomProperty(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_SetDefaultGeomProperty(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_FindFirstGeomProperty(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_GetDefaultGeomPropertyPos(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("In order to use the getDefaultGeomPropertyPos method you must use the object notation: \"geom_pos = dt.getDefaultGeomPropertyPos();\"")));

  te::da::DataSetType* dt = te::v8::common::Unwrap<te::da::DataSetType>(args.Holder());

  if(dt == 0)
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Invalid dataset in getDefaultGeomPropertyPos method!")));

  std::size_t pos = dt->getDefaultGeomPropertyPos();

  ::v8::Local<::v8::Integer> jspos = ::v8::Integer::New(static_cast<boost::int32_t>(pos));

  return hs.Close(jspos);
}

::v8::Handle<::v8::Value> DataSetType_GetCatalog(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_SetCatalog(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_IsFullLoaded(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_SetFullLoaded(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_SetPrimaryKey(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_GetPrimaryKey(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_Add(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_Remove(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_GetNumberOfUniqueKeys(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_GetUniqueKey(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_GetNumberOfCheckConstraints(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_GetCheckConstraint(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_ClearCheckConstraints(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_GetNumberOfIndexes(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_GetIndex(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_GetNumberOfForeignKeys(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_GetForeignKey(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_Replace(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSetType_Constructor(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

void te::v8::jsi::RegisterDataSetType(::v8::Local<::v8::Object>& global)
{
  ::v8::Local<::v8::FunctionTemplate> jdatasettype = ::v8::FunctionTemplate::New(DataSetType_Constructor);

  global->Set(::v8::String::New("TeDataSetType"), jdatasettype->GetFunction());
}

static ::v8::Persistent<::v8::FunctionTemplate> sg_datasettype_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetDataSetTypeTemplate()
{
  if(sg_datasettype_template.IsEmpty())
  {
    ::v8::Persistent<::v8::FunctionTemplate>& compositePropertyTpl = GetCompositePropertyTemplate();
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    result->Inherit(compositePropertyTpl);

    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("getTitle"), ::v8::FunctionTemplate::New(DataSetType_GetTitle));
    prototype->Set(::v8::String::NewSymbol("setTitle"), ::v8::FunctionTemplate::New(DataSetType_SetTitle));
    prototype->Set(::v8::String::NewSymbol("hasGeom"), ::v8::FunctionTemplate::New(DataSetType_HasGeom));
    prototype->Set(::v8::String::NewSymbol("hasDefaultGeom"), ::v8::FunctionTemplate::New(DataSetType_HasDefaultGeom));
    prototype->Set(::v8::String::NewSymbol("getDefaultGeomProperty"), ::v8::FunctionTemplate::New(DataSetType_GetDefaultGeomProperty));
    prototype->Set(::v8::String::NewSymbol("setDefaultGeomProperty"), ::v8::FunctionTemplate::New(DataSetType_SetDefaultGeomProperty));
    prototype->Set(::v8::String::NewSymbol("findFirstGeomProperty"), ::v8::FunctionTemplate::New(DataSetType_FindFirstGeomProperty));
    prototype->Set(::v8::String::NewSymbol("getDefaultGeomPropertyPos"), ::v8::FunctionTemplate::New(DataSetType_GetDefaultGeomPropertyPos));
    prototype->Set(::v8::String::NewSymbol("getCatalog"), ::v8::FunctionTemplate::New(DataSetType_GetCatalog));
    prototype->Set(::v8::String::NewSymbol("setCatalog"), ::v8::FunctionTemplate::New(DataSetType_SetCatalog));
    prototype->Set(::v8::String::NewSymbol("isFullLoaded"), ::v8::FunctionTemplate::New(DataSetType_IsFullLoaded));
    prototype->Set(::v8::String::NewSymbol("setFullLoaded"), ::v8::FunctionTemplate::New(DataSetType_SetFullLoaded));
    prototype->Set(::v8::String::NewSymbol("setPrimaryKey"), ::v8::FunctionTemplate::New(DataSetType_SetPrimaryKey));
    prototype->Set(::v8::String::NewSymbol("getPrimaryKey"), ::v8::FunctionTemplate::New(DataSetType_GetPrimaryKey));
    prototype->Set(::v8::String::NewSymbol("add"), ::v8::FunctionTemplate::New(DataSetType_Add));
    prototype->Set(::v8::String::NewSymbol("remove"), ::v8::FunctionTemplate::New(DataSetType_Remove));
    prototype->Set(::v8::String::NewSymbol("getNumberOfUniqueKeys"), ::v8::FunctionTemplate::New(DataSetType_GetNumberOfUniqueKeys));
    prototype->Set(::v8::String::NewSymbol("getUniqueKey"), ::v8::FunctionTemplate::New(DataSetType_GetUniqueKey));
    prototype->Set(::v8::String::NewSymbol("getNumberOfCheckConstraints"), ::v8::FunctionTemplate::New(DataSetType_GetNumberOfCheckConstraints));
    prototype->Set(::v8::String::NewSymbol("getCheckConstraint"), ::v8::FunctionTemplate::New(DataSetType_GetCheckConstraint));
    prototype->Set(::v8::String::NewSymbol("clearCheckConstraints"), ::v8::FunctionTemplate::New(DataSetType_ClearCheckConstraints));
    prototype->Set(::v8::String::NewSymbol("getNumberOfIndexes"), ::v8::FunctionTemplate::New(DataSetType_GetNumberOfIndexes));
    prototype->Set(::v8::String::NewSymbol("getIndex"), ::v8::FunctionTemplate::New(DataSetType_GetIndex));
    prototype->Set(::v8::String::NewSymbol("getNumberOfForeignKeys"), ::v8::FunctionTemplate::New(DataSetType_GetNumberOfForeignKeys));
    prototype->Set(::v8::String::NewSymbol("getForeignKey"), ::v8::FunctionTemplate::New(DataSetType_GetForeignKey));
    prototype->Set(::v8::String::NewSymbol("replace"), ::v8::FunctionTemplate::New(DataSetType_Replace));

    sg_datasettype_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sg_datasettype_template;
}
