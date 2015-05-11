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
  \file DataSourceFactory.cpp
   
  \brief Utility functions to register the DataSourceFactory class into Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../common/Exception.h"
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../common/Utils.h"
#include "DataAccess.h"

// STL
#include <memory>

::v8::Handle<::v8::Value> DataSourceFactory_Make(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsString())
    return ::v8::ThrowException(::v8::String::New("Missing parameter for the TeDataSourceFactory. The correct syntax is: var my_ds = TeDataSourceFactory.make(dstype);"));

  v8::String::Utf8Value jdsType(args[0]->ToString());
  std::string dsType = *jdsType;

  try
  {
    std::auto_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::make(dsType));
    ::v8::Local<::v8::Object> jsds = te::v8::common::Make(ds.get(), te::v8::jsi::GetDataSourceTemplate, true);
    ds.release();
    return hs.Close(jsds);
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

void te::v8::jsi::RegisterDataSourceFactory(::v8::Local<::v8::Object>& global)
{
  ::v8::HandleScope hs;

  ::v8::Handle<::v8::ObjectTemplate> dsfactory = ::v8::ObjectTemplate::New();

  dsfactory->Set(::v8::String::NewSymbol("make"), ::v8::FunctionTemplate::New(DataSourceFactory_Make));

  global->Set(::v8::String::New("TeDataSourceFactory"), dsfactory->NewInstance());
}

