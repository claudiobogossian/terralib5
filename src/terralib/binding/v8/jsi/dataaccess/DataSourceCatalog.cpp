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
  \file DataSourceCatalog.cpp
   
  \brief Utility functions to register the DataSourceCatalog class into the Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../dataaccess/datasource/DataSourceCatalog.h"
#include "../../common/Utils.h"
#include "DataAccess.h"

// STL
//#include <memory>

// Boost
#include <boost/cstdint.hpp>

::v8::Handle<::v8::Value> DataSourceCatalog_GetId(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use the getId method, you must use the object notation: \"id = obj.getId();\""));

  te::da::DataSourceCatalog* catalog = te::v8::common::Unwrap<te::da::DataSourceCatalog>(args.Holder());

  if(catalog == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid catalog in the getId method!"));

  unsigned int id = catalog->getId();

  ::v8::Local<::v8::Integer> jid = ::v8::Uint32::New(static_cast<boost::uint32_t>(id));

  return hs.Close(jid);
}

::v8::Handle<::v8::Value> DataSourceCatalog_SetId(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use setId method, you must use the object notation: \"obj.setId();\""));

  te::da::DataSourceCatalog* catalog = te::v8::common::Unwrap<te::da::DataSourceCatalog>(args.Holder());

  if(catalog == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid catalog in the getId method!"));

  if((args.Length() == 1) && !args[0].IsEmpty() && args[0]->IsUint32())
  {
    unsigned int id = args[0]->ToUint32()->Value();
    catalog->setId(id);
  }

  // Completar com o setId(DatasetType* dt, unsigned int id) e setId(Sequence* s, unsigned int newId)

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> DataSourceCatalog_GetDataSource(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use the getDataSource method you must use the object notation: \"ds = obj.getDataSource();\""));

  te::da::DataSourceCatalog* catalog = te::v8::common::Unwrap<te::da::DataSourceCatalog>(args.Holder());

  if(catalog == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid data source catalog in the getDataSource method!"));

  te::da::DataSource* ds = catalog->getDataSource();

  ::v8::Local<::v8::Object> jds = te::v8::common::Make(ds, te::v8::jsi::GetDataSourceTemplate, false);

  return hs.Close(jds);
}

::v8::Handle<::v8::Value> DataSourceCatalog_SetDataSource(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use the setDataSource method, you must use the object notation: \"obj.setDataSource(ds);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsObject())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in the setDataSource method!"));

  te::da::DataSourceCatalog* catalog = te::v8::common::Unwrap<te::da::DataSourceCatalog>(args.Holder());

  if(catalog == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid data source catalog in the setDataSource method!"));

  te::da::DataSource* ds = te::v8::common::Unwrap<te::da::DataSource>(args[0]->ToObject());

  catalog->setDataSource(ds);

  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalog_Clear(const ::v8::Arguments& args)
{
  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use the clear method, you must use the object notation: \"obj.clear();\""));

  te::da::DataSourceCatalog* catalog = te::v8::common::Unwrap<te::da::DataSourceCatalog>(args.Holder());

  if(catalog == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid data source catalog in the clear method!"));

  catalog->clear();

  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalog_Clone(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use the clone method, you must use the object notation: \"obj.clone();\""));

  te::da::DataSourceCatalog* catalog = te::v8::common::Unwrap<te::da::DataSourceCatalog>(args.Holder());

  if(catalog == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid data source catalog in the clone method!"));

  te::da::DataSourceCatalog* cloneCatalog = catalog->clone();

  ::v8::Local<::v8::Object> jcloneCatalog = te::v8::common::Make(cloneCatalog, te::v8::jsi::GetDataSourceCatalogTemplate, true);

  return hs.Close(jcloneCatalog);
}

::v8::Handle<::v8::Value> DataSourceCatalog_GetNumberOfDatasets(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use the getNumberOfDatasets method you must use the object notation: \"ndataSets = obj.getNumberOfDatasets();\""));

  te::da::DataSourceCatalog* catalog = te::v8::common::Unwrap<te::da::DataSourceCatalog>(args.Holder());

  if(catalog == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid data source catalog in the clone method!"));

  std::size_t ndataSets = catalog->getNumberOfDataSets();

  ::v8::Local<::v8::Integer> jndataSets = ::v8::Integer::New(static_cast<boost::int32_t>(ndataSets));

  return hs.Close(jndataSets);
}

::v8::Handle<::v8::Value> DataSourceCatalog_Add(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalog_Remove(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalog_Detach(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalog_Rename(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalog_GetDataSetType(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalog_GetDataSetTypeById(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalog_GetDataSetTypePos(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalog_GetNumberOfSequences(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use the getNumberOfSequences method you must use the object notation: \"nSequences = obj.getNumberOfSequences();\""));

  te::da::DataSourceCatalog* catalog = te::v8::common::Unwrap<te::da::DataSourceCatalog>(args.Holder());

  if(catalog == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid data source catalog in the clone method!"));

  std::size_t nSequences = catalog->getNumberOfSequences();

  ::v8::Local<::v8::Integer> jnSequences = ::v8::Integer::New(static_cast<boost::int32_t>(nSequences));

  return hs.Close(jnSequences);
}

::v8::Handle<::v8::Value> DataSourceCatalog_GetSequence(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalog_GetSequenceById(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalog_GetSequencePos(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalog_AddRef(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalog_RemoveRef(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalog_GetRefFK(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalog_DropDependentSequences(const ::v8::Arguments& args)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceCatalog_Constructor(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(!args.IsConstructCall())
    return ::v8::ThrowException(::v8::String::New("In order to create a data source catalog, you need call its constructor like: var c = new TeDataSourceCatalog();"));

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("The DataSourceCatalog constructor must use object notation!"));

  if(args.Length() != 0)
    return ::v8::ThrowException(::v8::String::New("The DataSourceCatalog constructor has no parameters!"));

  std::auto_ptr<te::da::DataSourceCatalog> catalog(new te::da::DataSourceCatalog());

  ::v8::Local<::v8::Object> jcatalog = te::v8::common::Make(catalog.get(), te::v8::jsi::GetDataSourceCatalogTemplate, true);

  catalog.release();

  return hs.Close(jcatalog);
}

void te::v8::jsi::RegisterDataSourceCatalog(::v8::Local<::v8::Object>& global)
{
  ::v8::HandleScope hs;

  ::v8::Local<::v8::FunctionTemplate> jscatalog = ::v8::FunctionTemplate::New(DataSourceCatalog_Constructor);

  global->Set(::v8::String::New("TeDataSourceCatalog"), jscatalog->GetFunction());
}

static ::v8::Persistent<::v8::FunctionTemplate> sds_datasourcecatalog_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetDataSourceCatalogTemplate()
{
  if(sds_datasourcecatalog_template.IsEmpty())
  {
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();

    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("getId"), ::v8::FunctionTemplate::New(DataSourceCatalog_GetId));
    prototype->Set(::v8::String::NewSymbol("setId"), ::v8::FunctionTemplate::New(DataSourceCatalog_SetId));
    prototype->Set(::v8::String::NewSymbol("getDataSource"), ::v8::FunctionTemplate::New(DataSourceCatalog_GetDataSource));
    prototype->Set(::v8::String::NewSymbol("setDataSource"), ::v8::FunctionTemplate::New(DataSourceCatalog_SetDataSource));
    prototype->Set(::v8::String::NewSymbol("clear"), ::v8::FunctionTemplate::New(DataSourceCatalog_Clear));
    prototype->Set(::v8::String::NewSymbol("clone"), ::v8::FunctionTemplate::New(DataSourceCatalog_Clone));
    prototype->Set(::v8::String::NewSymbol("getNumberOfDataSets"), ::v8::FunctionTemplate::New(DataSourceCatalog_GetNumberOfDatasets));
    prototype->Set(::v8::String::NewSymbol("add"), ::v8::FunctionTemplate::New(DataSourceCatalog_Add));
    prototype->Set(::v8::String::NewSymbol("remove"), ::v8::FunctionTemplate::New(DataSourceCatalog_Remove));
    prototype->Set(::v8::String::NewSymbol("detach"), ::v8::FunctionTemplate::New(DataSourceCatalog_Detach));
    prototype->Set(::v8::String::NewSymbol("rename"), ::v8::FunctionTemplate::New(DataSourceCatalog_Rename));
    prototype->Set(::v8::String::NewSymbol("getDataSetType"), ::v8::FunctionTemplate::New(DataSourceCatalog_GetDataSetType));
    prototype->Set(::v8::String::NewSymbol("getDataSetTypeById"), ::v8::FunctionTemplate::New(DataSourceCatalog_GetDataSetTypeById));
    prototype->Set(::v8::String::NewSymbol("getDataSetTypePos"), ::v8::FunctionTemplate::New(DataSourceCatalog_GetDataSetTypePos));
    prototype->Set(::v8::String::NewSymbol("getNumberOfSequences"), ::v8::FunctionTemplate::New(DataSourceCatalog_GetNumberOfSequences));
    prototype->Set(::v8::String::NewSymbol("getSequence"), ::v8::FunctionTemplate::New(DataSourceCatalog_GetSequence));
    prototype->Set(::v8::String::NewSymbol("getSequenceById"), ::v8::FunctionTemplate::New(DataSourceCatalog_GetSequenceById));
    prototype->Set(::v8::String::NewSymbol("getSequencePos"), ::v8::FunctionTemplate::New(DataSourceCatalog_GetSequencePos));
    prototype->Set(::v8::String::NewSymbol("addRef"), ::v8::FunctionTemplate::New(DataSourceCatalog_AddRef));
    prototype->Set(::v8::String::NewSymbol("removeRef"), ::v8::FunctionTemplate::New(DataSourceCatalog_RemoveRef));
    prototype->Set(::v8::String::NewSymbol("getRefFK"), ::v8::FunctionTemplate::New(DataSourceCatalog_GetRefFK));
    prototype->Set(::v8::String::NewSymbol("dropDependentSequences"), ::v8::FunctionTemplate::New(DataSourceCatalog_DropDependentSequences));

    sds_datasourcecatalog_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sds_datasourcecatalog_template;
}

