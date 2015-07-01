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
  \file DataSourceManager.cpp
   
  \brief Utility functions to register the DataSourceManager class into Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../common/Exception.h"
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../common/Utils.h"
#include "DataAccess.h"

// Boost
#include <boost/cstdint.hpp>

::v8::Handle<::v8::Value> DataSourceManager_Make(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if((args.Length() != 2) || args[0].IsEmpty() || !args[0]->IsString() || args[1].IsEmpty() || !args[1]->IsString())
    return ::v8::ThrowException(::v8::String::New("In order to use the DataSourceManager make method you must use object notation: \"ds = TeDataSourceManager.make(\"id1\", \"POSTGIS\");\""));

  v8::String::Utf8Value jid(args[0]->ToString());
  v8::String::Utf8Value jdstype(args[1]->ToString());

  try
  {
    te::da::DataSource* ds = te::da::DataSourceManager::getInstance().make(*jid, *jdstype);
    ::v8::Local<::v8::Object> jds = te::v8::common::Make(ds, te::v8::jsi::GetDataSourceTemplate, false);
    return hs.Close(jds);
  }
  catch(const std::exception& e)
  {
    return ::v8::ThrowException(::v8::String::New(e.what()));
  }
  catch(...)
  {
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Unexpected exception in make method!")));
  }
}

::v8::Handle<::v8::Value> DataSourceManager_Open(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Length() != 3)
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Wrong number of parameters for open method!")));

  if(args[0].IsEmpty() || !args[0]->IsString() || args[1].IsEmpty() || !args[1]->IsString() || args[2].IsEmpty())
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Wrong parameters type for open method!")));

  v8::String::Utf8Value jid(args[0]->ToString());
  v8::String::Utf8Value jdstype(args[1]->ToString());

  try
  {
    if(args[2]->IsString())
    {    
      v8::String::Utf8Value jconninfo(args[2]->ToString());
      te::da::DataSource* ds = te::da::DataSourceManager::getInstance().open(*jid, *jdstype, *jconninfo);
      ::v8::Local<::v8::Object> jds = te::v8::common::Make(ds, te::v8::jsi::GetDataSourceTemplate, false);
      return hs.Close(jds);
    }
    else if(args[2]->IsArray())
    {
      ::v8::Local<::v8::Object> aconninfo = args[2]->ToObject();
      std::map<std::string, std::string> conninfo;
      te::v8::common::Convert2Cpp(aconninfo, conninfo);
      te::da::DataSource* ds = te::da::DataSourceManager::getInstance().open(*jid, *jdstype, conninfo);
      ::v8::Local<::v8::Object> jds = te::v8::common::Make(ds, te::v8::jsi::GetDataSourceTemplate, false);
      return hs.Close(jds);
    }

    throw te::common::Exception(TR_V8JSI("Wrong parameters in open method!"));
  }
  catch(const std::exception& e)
  {
    return ::v8::ThrowException(::v8::String::New(e.what()));
  }
  catch(...)
  {
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Unexpected exception in open method!")));
  }
}

::v8::Handle<::v8::Value> DataSourceManager_Size(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;
  std::size_t size = te::da::DataSourceManager::getInstance().size();
  ::v8::Local<::v8::Integer> jsize = ::v8::Integer::New(static_cast<boost::int32_t>(size));
  return hs.Close(jsize);
}

::v8::Handle<::v8::Value> DataSourceManager_Find(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Length() != 1 || args[0].IsEmpty())
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("In order to use the find method, you must use the object notation: \"ds = obj.find(\"ds1_id1\");\"")));

  if(args[0]->IsString())
  {
    v8::String::Utf8Value jid(args[0]->ToString());
    te::da::DataSource* ds = te::da::DataSourceManager::getInstance().find(*jid);
    ::v8::Local<::v8::Object> jds = te::v8::common::Make(ds, te::v8::jsi::GetDataSourceTemplate, false);
    return hs.Close(jds);
  }
  else if(args[0]->IsObject())
  {
    te::da::DataSource* ds = te::v8::common::Unwrap<te::da::DataSource>(args[0]->ToObject());
    std::string id = te::da::DataSourceManager::getInstance().find(ds);
    ::v8::Local<::v8::String> jid = ::v8::String::New(id.c_str());
    return hs.Close(jid);
  }

  return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Invalid parameter type in find method!")));
}

::v8::Handle<::v8::Value> DataSourceManager_Insert(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceManager_Detach(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;

  /*if(args.Length() != 1 || args[0].IsEmpty())
    return ::v8::ThrowException(::v8::String::New(TR_V8JSI("In order to use the detach method, you must use the object notation: \"obj.detach(\"ds1_id1\");\"")));

  if(args[0]->IsString())
  {
    v8::String::Utf8Value jid(args[0]->ToString());
    te::da::DataSource* ds = te::da::DataSourceManager::getInstance().find(*jid);
    ::v8::Local<::v8::Object> jds = te::v8::common::Make(ds, te::v8::jsi::GetDataSourceTemplate, true);
    return hs.Close(jds);
  }
  else if(args[0]->IsObject())
  {
    te::da::DataSource* ds = te::v8::common::Unwrap<te::da::DataSource>(args[0]->ToObject());
    te::da::DataSourceManager::getInstance().detach(ds);
    return ::v8::Undefined();
  }*/

  return ::v8::ThrowException(::v8::String::New(TR_V8JSI("Invalid parameter type in the detach method!")));
}

::v8::Handle<::v8::Value> DataSourceManager_DetachAll(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  /*te::da::DataSourceManager::getInstance().detachAll();*/
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceManager_Remove(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;

  //if(args.Length() != 1 || args[0].IsEmpty())
  //  return ::v8::ThrowException(::v8::String::New("In order to use the detach method, you must use the object notation: \"obj.remove(\"ds1_id1\");\""));

  //if(args[0]->IsString())
  //{
  //  v8::String::Utf8Value jid(args[0]->ToString());
  //  te::da::DataSourceManager::getInstance().remove(*jid);

  //  return ::v8::Undefined();
  //}
  //else if(args[0]->IsObject())
  //{
  //  te::da::DataSource* ds = te::v8::common::Unwrap<te::da::DataSource>(args[0]->ToObject());

  //  te::da::DataSourceManager::getInstance().remove(ds);

  //  return ::v8::Undefined();
  //}

  return ::v8::ThrowException(::v8::String::New("Invalid parameter type in the remove method!"));
}

::v8::Handle<::v8::Value> DataSourceManager_RemoveAll(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;

  /*if(args.Length() != 0)
    return ::v8::ThrowException(::v8::String::New("In order to use the removeAll method, you must use the object notation: \"obj.removeAll();\""));

  te::da::DataSourceManager::getInstance().removeAll();*/

  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceManager_Begin(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceManager_End(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> DataSourceManager_GetOpenedList(const ::v8::Arguments& /*args*/)
{
  ::v8::HandleScope hs;
  return ::v8::Undefined();
}

void te::v8::jsi::RegisterDataSourceManager(::v8::Local<::v8::Object>& global)
{
  ::v8::HandleScope hs;

  ::v8::Handle<::v8::ObjectTemplate> objTpl = ::v8::ObjectTemplate::New();

  objTpl->Set(::v8::String::NewSymbol("make"), ::v8::FunctionTemplate::New(DataSourceManager_Make));
  objTpl->Set(::v8::String::NewSymbol("open"), ::v8::FunctionTemplate::New(DataSourceManager_Open));
  objTpl->Set(::v8::String::NewSymbol("size"), ::v8::FunctionTemplate::New(DataSourceManager_Size));
  objTpl->Set(::v8::String::NewSymbol("find"), ::v8::FunctionTemplate::New(DataSourceManager_Find));
  objTpl->Set(::v8::String::NewSymbol("insert"), ::v8::FunctionTemplate::New(DataSourceManager_Insert));
  objTpl->Set(::v8::String::NewSymbol("detach"), ::v8::FunctionTemplate::New(DataSourceManager_Detach));
  objTpl->Set(::v8::String::NewSymbol("detachAll"), ::v8::FunctionTemplate::New(DataSourceManager_DetachAll));
  objTpl->Set(::v8::String::NewSymbol("remove"), ::v8::FunctionTemplate::New(DataSourceManager_Remove));
  objTpl->Set(::v8::String::NewSymbol("removeAll"), ::v8::FunctionTemplate::New(DataSourceManager_RemoveAll));
  objTpl->Set(::v8::String::NewSymbol("begin"), ::v8::FunctionTemplate::New(DataSourceManager_Begin));
  objTpl->Set(::v8::String::NewSymbol("end"), ::v8::FunctionTemplate::New(DataSourceManager_End));
  objTpl->Set(::v8::String::NewSymbol("getOpenedList"), ::v8::FunctionTemplate::New(DataSourceManager_GetOpenedList));

  global->Set(::v8::String::New("TeDataSourceManager"), objTpl->NewInstance());
}

