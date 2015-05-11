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
  \file DataSourceCatalogLoader.cpp
   
  \brief Utility functions to register the DataSourceCatalogLoader class into Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "DataAccess.h"

::v8::Handle<::v8::Value> DataSourceCatalogLoader_GetDataSets(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalogLoader_GetDataSetType(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalogLoader_GetPrimaryKey(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalogLoader_GetUniqueKeys(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalogLoader_GetForeignKeys(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalogLoader_GetForeignKey(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalogLoader_GetIndexes(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalogLoader_GetCheckConstraints(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalogLoader_GetSequences(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalogLoader_GetSequence(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalogLoader_GetExtent(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalogLoader_LoadCatalog(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalogLoader_DataSetExists(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalogLoader_PrimaryKeyExists(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalogLoader_UniqueKeyExists(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalogLoader_ForeignKeyExists(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalogLoader_CheckConstraintExists(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalogLoader_IndexExists(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalogLoader_SequenceExists(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalogLoader_GetTransactor(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}


static ::v8::Persistent<::v8::FunctionTemplate> sds_datasourcecatalogloader_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetDataSourceCatalogLoaderTemplate()
{
  if(sds_datasourcecatalogloader_template.IsEmpty())
  {
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("getDataSets"), ::v8::FunctionTemplate::New(DataSourceCatalogLoader_GetDataSets));
    prototype->Set(::v8::String::NewSymbol("getDataSetType"), ::v8::FunctionTemplate::New(DataSourceCatalogLoader_GetDataSetType));
    prototype->Set(::v8::String::NewSymbol("getPrimaryKey"), ::v8::FunctionTemplate::New(DataSourceCatalogLoader_GetPrimaryKey));
    prototype->Set(::v8::String::NewSymbol("getUniqueKeys"), ::v8::FunctionTemplate::New(DataSourceCatalogLoader_GetUniqueKeys));
    prototype->Set(::v8::String::NewSymbol("getForeignKeys"), ::v8::FunctionTemplate::New(DataSourceCatalogLoader_GetForeignKeys));
    prototype->Set(::v8::String::NewSymbol("getForeignKey"), ::v8::FunctionTemplate::New(DataSourceCatalogLoader_GetForeignKey));
    prototype->Set(::v8::String::NewSymbol("getIndexes"), ::v8::FunctionTemplate::New(DataSourceCatalogLoader_GetIndexes));
    prototype->Set(::v8::String::NewSymbol("getCheckConstraints"), ::v8::FunctionTemplate::New(DataSourceCatalogLoader_GetCheckConstraints));
    prototype->Set(::v8::String::NewSymbol("getSequences"), ::v8::FunctionTemplate::New(DataSourceCatalogLoader_GetSequences));
    prototype->Set(::v8::String::NewSymbol("getSequence"), ::v8::FunctionTemplate::New(DataSourceCatalogLoader_GetSequence));
    prototype->Set(::v8::String::NewSymbol("getExtent"), ::v8::FunctionTemplate::New(DataSourceCatalogLoader_GetExtent));
    prototype->Set(::v8::String::NewSymbol("loadCatalog"), ::v8::FunctionTemplate::New(DataSourceCatalogLoader_LoadCatalog));
    prototype->Set(::v8::String::NewSymbol("datasetExists"), ::v8::FunctionTemplate::New(DataSourceCatalogLoader_DataSetExists));
    prototype->Set(::v8::String::NewSymbol("primarykeyExists"), ::v8::FunctionTemplate::New(DataSourceCatalogLoader_PrimaryKeyExists));
    prototype->Set(::v8::String::NewSymbol("uniquekeyExists"), ::v8::FunctionTemplate::New(DataSourceCatalogLoader_UniqueKeyExists));
    prototype->Set(::v8::String::NewSymbol("foreignkeyExists"), ::v8::FunctionTemplate::New(DataSourceCatalogLoader_ForeignKeyExists));
    prototype->Set(::v8::String::NewSymbol("checkConstraintExists"), ::v8::FunctionTemplate::New(DataSourceCatalogLoader_CheckConstraintExists));
    prototype->Set(::v8::String::NewSymbol("indexExists"), ::v8::FunctionTemplate::New(DataSourceCatalogLoader_IndexExists));
    prototype->Set(::v8::String::NewSymbol("sequenceExists"), ::v8::FunctionTemplate::New(DataSourceCatalogLoader_SequenceExists));
    prototype->Set(::v8::String::NewSymbol("getTransactor"), ::v8::FunctionTemplate::New(DataSourceCatalogLoader_GetTransactor));

    sds_datasourcecatalogloader_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sds_datasourcecatalogloader_template;
}

