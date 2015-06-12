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
  \file Utils.cpp

  \brief Utility functions for dealing with JNI.
 */

// TerraLib
#include "../../../common/Exception.h"
#include "../../../common/Translator.h"
#include "Utils.h"

// STL
#include <cassert>
#include <cstdio>

::v8::Handle<::v8::String> te::v8::common::ReadFile(const std::string& fileName)
{
  FILE* file = fopen(fileName.c_str(), "rb");

  if(file == 0)
    throw te::common::Exception(TR_V8COMMON("Could not read the informed file"));

  fseek(file, 0, SEEK_END);

  std::size_t fileSize = ftell(file);

  rewind(file);

  char* buff = new char[fileSize + 1]; // add room for trailing '\0'

  buff[fileSize] = '\0';

  std::size_t read = 0;

  for(std::size_t i = 0; i < fileSize; i += read)
    read = fread(&buff[i], sizeof(char), fileSize - i, file); // we need to check the return value

  fclose(file);

  ::v8::Handle<::v8::String> result = ::v8::String::New(buff, static_cast<int>(fileSize));

  delete [] buff;

  return result;
}

void te::v8::common::Cache(::v8::Local<::v8::Object>& obj, const std::string& methodName, ::v8::Persistent<::v8::Function>& outFtor)
{
  ::v8::Local<::v8::String> jsMethodName = ::v8::String::New(methodName.c_str());

  ::v8::Local<::v8::Value> jsMethod = obj->Get(jsMethodName);

  if(jsMethod.IsEmpty())
    throw te::common::Exception(TR_V8COMMON("Could not find the informed method in the JavaScript object!")); 

  if(!jsMethod->IsFunction())
    throw te::common::Exception(TR_V8COMMON("A method from a JavaScript object must be a function!"));
  
  ::v8::Handle<::v8::Function> jsMethodFtor = ::v8::Handle<::v8::Function>::Cast(jsMethod);

  outFtor = ::v8::Persistent<::v8::Function>::New(jsMethodFtor);
}

::v8::Handle<::v8::Value> te::v8::common::Print(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  bool first = true;

  for(int i = 0; i < args.Length(); ++i)
  {
    if(first)
      first = false;
    else
      printf(" ");

    ::v8::String::Utf8Value str(args[i]);

    const char* cstr = *str;

    printf("%s", cstr);
  }

  printf("\n");

  fflush(stdout);

  return ::v8::Undefined();
}

