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
  \file Geometry.cpp
   
  \brief Utility functions to register the Geometry class into Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../common/Exception.h"
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../common/Utils.h"
#include "DataAccess.h"

// STL
#include <cassert>

// Boost
#include <boost/cstdint.hpp>

::v8::Handle<::v8::Value> DataSource_GetType(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("In order to use the getType method you must use the object notation: \"t = obj.getType();\"")));

  te::da::DataSource* ds = te::v8::common::Unwrap<te::da::DataSource>(args.Holder());

  if(ds == 0)
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Invalid data source in getType method!")));

  const std::string& dst = ds->getType();

  ::v8::Local<::v8::String> jdst = ::v8::String::New(dst.c_str());

  return hs.Close(jdst);
}

::v8::Handle<::v8::Value> DataSource_GetConnectionInfo(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("In order to use the getConnectionInfo method you must use the object notation: \"conninfo = obj.getConnectionInfo();\"")));

  te::da::DataSource* ds = te::v8::common::Unwrap<te::da::DataSource>(args.Holder());

  if(ds == 0)
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Invalid data source in getConnectionInfo method!")));

  const std::map<std::string, std::string>& conninfo = ds->getConnectionInfo();

  ::v8::Local<::v8::Object> jsmap = te::v8::common::Convert2Js(conninfo);

  return hs.Close(jsmap);
}

::v8::Handle<::v8::Value> DataSource_SetConnectionInfo(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("In order to use the setConnectionInfo method you must use the object notation: \"obj.setConnectionInfo(conninfo);\"")));

  te::da::DataSource* ds = te::v8::common::Unwrap<te::da::DataSource>(args.Holder());

  if(ds == 0)
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Invalid data source in setConnectionInfo method!")));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsArray())
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("In order to use setConnectionInfo method you must use object notation: \"obj.setConnectionInfo(connfinfo);\"")));
 
  ::v8::Local<::v8::Object> aconninfo = args[0]->ToObject();

  std::map<std::string, std::string> conninfo;

  te::v8::common::Convert2Cpp(aconninfo, conninfo);

  ds->setConnectionInfo(conninfo);

  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSource_GetConnectionStr(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("In order to use the getConnectionStr method you must use the object notation: \"connstr = obj.getConnectionStr();\"")));

  te::da::DataSource* ds = te::v8::common::Unwrap<te::da::DataSource>(args.Holder());

  if(ds == 0)
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Invalid data source in the getConnectionStr method!")));

  const std::string& connStr = ds->getConnectionStr();

  ::v8::Local<::v8::String> jconnStr = ::v8::String::New(connStr.c_str());

  return hs.Close(jconnStr);
}

::v8::Handle<::v8::Value> DataSource_SetConnectionStr(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("In order to use the setConnectionStr method you must use the object notation: \"obj.setConnectionStr(connstr);\"")));

  if(args.Length() != 1 || args[0].IsEmpty() || !args[0]->IsString())
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("In order to use the setConnectionStr method, you must use the object notation: \"obj.setConnectionStr(\"connStr\");\"")));

  te::da::DataSource* ds = te::v8::common::Unwrap<te::da::DataSource>(args.Holder());

  if(ds == 0)
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Invalid data source in setConnectionStr method!")));

  ::v8::String::Utf8Value jconnStr(args[0]->ToString());

  ds->setConnectionStr(*jconnStr);

  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSource_GetCapabilities(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSource_GetDialect(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSource_Open(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("In order to use the open method, you must use the object notation: \"obj.open();\" or \"obj.open(conn_string);\" or \"obj.open(conninfo);\"")));

  te::da::DataSource* ds = te::v8::common::Unwrap<te::da::DataSource>(args.Holder());

  if(ds == 0)
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Invalid data source in open method!")));

  try
  {
    if(args.Length() == 0)
    {
      ds->open();
    }
    else
    {
      if((args.Length() == 1) && (!args[0].IsEmpty()) && (args[0]->IsString()))
      {
        v8::String::Utf8Value jconnInfo(args[0]->ToString());
        ds->open(*jconnInfo);
      }
      else if((args.Length() == 1) && (!args[0].IsEmpty()) && (args[0]->IsArray()))
      {
        ::v8::Local<::v8::Object> aconninfo = args[0]->ToObject();
        std::map<std::string, std::string> conninfo;
        te::v8::common::Convert2Cpp(aconninfo, conninfo);
        ds->open(conninfo);
      }
      
      throw te::common::Exception(TR_V8JSI("Wrong parameters in open method!"));
    }
  }
  catch(const std::exception& e)
  {
    return ::v8::ThrowException(::v8::String::New(e.what()));
  }
  catch(...)
  {
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Unexpected exception in open method!")));
  }

  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSource_Close(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use the close method, you must use the object notation: \"obj.close();\""));

  te::da::DataSource* ds = te::v8::common::Unwrap<te::da::DataSource>(args.Holder());

  if(ds == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid data source in the close method!"));

  try
  {
    ds->close();
  }
  catch(const std::exception& e)
  {
    return ::v8::ThrowException(::v8::String::New(e.what()));
  }
  catch(...)
  {
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Unexpected exception in close method!")));
  }

  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSource_IsOpened(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use the isOpened method, you must use the object notation: \"result = obj.isOpened();\""));

  te::da::DataSource* ds = te::v8::common::Unwrap<te::da::DataSource>(args.Holder());

  if(ds == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid data source in isOpened method!"));

  bool result = ds->isOpened();

  ::v8::Handle<::v8::Boolean> jresult = ::v8::Boolean::New(result);

  return hs.Close(jresult);
}

::v8::Handle<::v8::Value> DataSource_IsValid(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use the isValid method, you must use the object notation: \"result = obj.isValid();\""));

  te::da::DataSource* ds = te::v8::common::Unwrap<te::da::DataSource>(args.Holder());

  if(ds == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid data source in isValid method!"));

  bool result = ds->isValid();

  ::v8::Handle<::v8::Boolean> jresult = ::v8::Boolean::New(result);

  return hs.Close(jresult);
}

::v8::Handle<::v8::Value> DataSource_GetCatalog(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use the getCatalog method you must use the object notation: \"c = obj.getCatalog();\""));

  te::da::DataSource* ds = te::v8::common::Unwrap<te::da::DataSource>(args.Holder());

  if(ds == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid data source in the getCatalog method!"));

  std::auto_ptr<te::da::DataSourceCatalog> catalog(ds->getCatalog());
  
  ::v8::Local<::v8::Object> jcatalog = te::v8::common::Make(catalog.get(), te::v8::jsi::GetDataSourceCatalogTemplate, false);

  catalog.release();

  return hs.Close(jcatalog);
}

::v8::Handle<::v8::Value> DataSource_GetTransactor(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use the getTransactor method you must use the object notation: \"c = obj.getTransactor();\""));

  te::da::DataSource* ds = te::v8::common::Unwrap<te::da::DataSource>(args.Holder());

  if(ds == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid data source in the getTransactor method!"));

  try
  {
    std::auto_ptr<te::da::DataSourceTransactor> t(ds->getTransactor());
    ::v8::Local<::v8::Object> jt = te::v8::common::Make(t.get(), te::v8::jsi::GetDataSourceTransactorTemplate, true);
    t.release();
    return hs.Close(jt);
  }
  catch(const std::exception& e)
  {
    return ::v8::ThrowException(::v8::String::New(e.what()));
  }
  catch(...)
  {
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Unexpected exception in getTransactor method!")));
  }
}

::v8::Handle<::v8::Value> DataSource_Create(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSource_Drop(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Length() != 1 || args[0].IsEmpty() || !args[0]->IsObject() )
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Wrong parameters drop method!")));
  
  te::da::DataSource* ds = te::v8::common::Unwrap<te::da::DataSource>(args[0]->ToObject());

  try
  {
    te::da::DataSource::drop(ds);
    te::v8::common::LooseOwnership<te::da::DataSource>(args[0]->ToObject());
  }
  catch(const std::exception& e)
  {
    return ::v8::ThrowException(::v8::String::New(e.what()));
  }
  catch(...)
  {
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Unexpected exception in drop method!")));
  }

  return ::v8::Undefined();
}

void te::v8::jsi::RegisterDataSource(::v8::Local<::v8::Object>& global)
{
  ::v8::HandleScope hs;

  ::v8::Handle<::v8::ObjectTemplate> objTpl = ::v8::ObjectTemplate::New();

  objTpl->Set(::v8::String::NewSymbol("create"), ::v8::FunctionTemplate::New(DataSource_Create));
  objTpl->Set(::v8::String::NewSymbol("drop"), ::v8::FunctionTemplate::New(DataSource_Drop));

  global->Set(::v8::String::New("TeDataSource"), objTpl->NewInstance());
}

static ::v8::Persistent<::v8::FunctionTemplate> sg_datasource_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetDataSourceTemplate()
{
  if(sg_datasource_template.IsEmpty())
  {
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("getType"), ::v8::FunctionTemplate::New(DataSource_GetType));
    prototype->Set(::v8::String::NewSymbol("getConnectionInfo"), ::v8::FunctionTemplate::New(DataSource_GetConnectionInfo));
    prototype->Set(::v8::String::NewSymbol("setConnectionInfo"), ::v8::FunctionTemplate::New(DataSource_SetConnectionInfo));
    prototype->Set(::v8::String::NewSymbol("getConnectionStr"), ::v8::FunctionTemplate::New(DataSource_GetConnectionStr));
    prototype->Set(::v8::String::NewSymbol("setConnectionStr"), ::v8::FunctionTemplate::New(DataSource_SetConnectionStr));
    prototype->Set(::v8::String::NewSymbol("getCapabilities"), ::v8::FunctionTemplate::New(DataSource_GetCapabilities));
    prototype->Set(::v8::String::NewSymbol("getDialect"), ::v8::FunctionTemplate::New(DataSource_GetDialect));
    prototype->Set(::v8::String::NewSymbol("open"), ::v8::FunctionTemplate::New(DataSource_Open));
    prototype->Set(::v8::String::NewSymbol("close"), ::v8::FunctionTemplate::New(DataSource_Close));
    prototype->Set(::v8::String::NewSymbol("isOpened"), ::v8::FunctionTemplate::New(DataSource_IsOpened));
    prototype->Set(::v8::String::NewSymbol("isValid"), ::v8::FunctionTemplate::New(DataSource_IsValid));
    prototype->Set(::v8::String::NewSymbol("getCatalog"), ::v8::FunctionTemplate::New(DataSource_GetCatalog));
    prototype->Set(::v8::String::NewSymbol("getTransactor"), ::v8::FunctionTemplate::New(DataSource_GetTransactor));

    sg_datasource_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sg_datasource_template;
}

