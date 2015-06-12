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
  \file DataSourceTransactor.cpp
   
  \brief Utility functions to register the DataSourceTransactor class into Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../common/Exception.h"
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/dataset/DataSet.h"
#include "../../../../dataaccess/dataset/DataSetPersistence.h"
#include "../../../../dataaccess/dataset/DataSetTypePersistence.h"
#include "../../../../dataaccess/datasource/BatchExecutor.h"
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "../../../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../../../dataaccess/datasource/PreparedQuery.h"
#include "../../common/Utils.h"
#include "DataAccess.h"

// STL
#include <memory>

::v8::Handle<::v8::Value> DataSourceTransactor_Begin(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceTransactor_Commit(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceTransactor_RollBack(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceTransactor_GetDataSet(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("In order to use the getDataSet method you must use the object notation: \"dataset = obj.getDataSet(\"mydataset-name\");\"")));

  if(args.Length() != 1 || args[0].IsEmpty() || !args[0]->IsString())
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Wrong parameters in getDataSet method!")));

  te::da::DataSourceTransactor* t = te::v8::common::Unwrap<te::da::DataSourceTransactor>(args.Holder());

  if(t == 0)
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Invalid data source transactor in getDataSet method!")));

  try
  {
    v8::String::Utf8Value jdatasetname(args[0]->ToString());
    std::auto_ptr<te::da::DataSet> dataset(t->getDataSet(*jdatasetname));
    ::v8::Local<::v8::Object> jsdataset = te::v8::common::Make(dataset.get(), te::v8::jsi::GetDataSetTemplate, true);
    dataset.release();
    return hs.Close(jsdataset);
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

::v8::Handle<::v8::Value> DataSourceTransactor_GetDataSetByEnvelope(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceTransactor_GetDataSetByGeom(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceTransactor_Query(const ::v8::Arguments& /*args*/)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceTransactor_Execute(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceTransactor_GetPreparedStmt(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceTransactor_GetBatchExecutor(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceTransactor_GetCatalogLoader(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceTransactor_GetDataSetTypePersistence(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceTransactor_GetDataSetPersistence(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceTransactor_Cancel(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceTransactor_GetDataSource(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

static ::v8::Persistent<::v8::FunctionTemplate> sds_datasourcetransactor_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetDataSourceTransactorTemplate()
{
  if(sds_datasourcetransactor_template.IsEmpty())
  {
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("begin"), ::v8::FunctionTemplate::New(DataSourceTransactor_Begin));
    prototype->Set(::v8::String::NewSymbol("commit"), ::v8::FunctionTemplate::New(DataSourceTransactor_Commit));
    prototype->Set(::v8::String::NewSymbol("rollBack"), ::v8::FunctionTemplate::New(DataSourceTransactor_RollBack));
    prototype->Set(::v8::String::NewSymbol("getDataSet"), ::v8::FunctionTemplate::New(DataSourceTransactor_GetDataSet));
    prototype->Set(::v8::String::NewSymbol("getDataSetByEnvelope"), ::v8::FunctionTemplate::New(DataSourceTransactor_GetDataSetByEnvelope));
    prototype->Set(::v8::String::NewSymbol("getDataSetByGeom"), ::v8::FunctionTemplate::New(DataSourceTransactor_GetDataSetByGeom));
    prototype->Set(::v8::String::NewSymbol("query"), ::v8::FunctionTemplate::New(DataSourceTransactor_Query));
    prototype->Set(::v8::String::NewSymbol("execute"), ::v8::FunctionTemplate::New(DataSourceTransactor_Execute));
    prototype->Set(::v8::String::NewSymbol("getPrepared"), ::v8::FunctionTemplate::New(DataSourceTransactor_GetPreparedStmt));
    prototype->Set(::v8::String::NewSymbol("getBatchExecutor"), ::v8::FunctionTemplate::New(DataSourceTransactor_GetBatchExecutor));
    prototype->Set(::v8::String::NewSymbol("getCatalogLoader"), ::v8::FunctionTemplate::New(DataSourceTransactor_GetCatalogLoader));
    prototype->Set(::v8::String::NewSymbol("getDataSetTypePersistence"), ::v8::FunctionTemplate::New(DataSourceTransactor_GetDataSetTypePersistence));
    prototype->Set(::v8::String::NewSymbol("getDataSetPersistence"), ::v8::FunctionTemplate::New(DataSourceTransactor_GetDataSetPersistence));
    prototype->Set(::v8::String::NewSymbol("cancel"), ::v8::FunctionTemplate::New(DataSourceTransactor_Cancel));
    prototype->Set(::v8::String::NewSymbol("getDataSource"), ::v8::FunctionTemplate::New(DataSourceTransactor_GetDataSource));

    sds_datasourcetransactor_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sds_datasourcetransactor_template;
}

