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
  \file CheckConstraint.cpp
   
  \brief Utility functions to register the CheckConstraint class into the Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../dataaccess/dataset/CheckConstraint.h"
#include "../../common/Utils.h"
#include "DataAccess.h"

// STL
#include <memory>

// Boost
#include <boost/cstdint.hpp>

::v8::Handle<::v8::Value> CheckConstraint_GetExpression(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use the getExpression method you must use the object notation: \"exp = obj.getExpression();\""));

  te::da::CheckConstraint* cc = te::v8::common::Unwrap<te::da::CheckConstraint>(args.Holder());

  if(cc == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid check constraint in the getExpression method!"));

  const std::string& exp = cc->getExpression();

  ::v8::Local<::v8::String> jexp = ::v8::String::New(exp.c_str());

  return hs.Close(jexp);
}

::v8::Handle<::v8::Value> CheckConstraint_SetExpression(const ::v8::Arguments& args)
{
  if(args.Length() != 1 || args[0].IsEmpty() || !args[0]->IsString() || args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use the setExpression method, you must use the object notation: \"obj.setExpression(\"x > 20\");\""));

  te::da::CheckConstraint* cc = te::v8::common::Unwrap<te::da::CheckConstraint>(args.Holder());

  if(cc == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid check constraint in the setExpression method!"));

  v8::String::Utf8Value jexp(args[0]->ToString());

  cc->setExpression(*jexp);

  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> CheckConstraint_Constructor(const ::v8::Arguments& args)
{
  //::v8::HandleScope hs;

  //if(!args.IsConstructCall())
  //  return ::v8::ThrowException(::v8::String::New("In order to create a check constraint, you need to call its constructor like: var c = new TeCheckConstraint(dt, id);"));

  //if(args.Holder().IsEmpty())
  //  return ::v8::ThrowException(::v8::String::New("The CheckConstraint constructor must use object notation!"));

  //if(args.Length() != 0)
  //  return ::v8::ThrowException(::v8::String::New("The CheckConstraint constructor has no parameters!"));

  //std::auto_ptr<te::da::CheckConstraint> catalog(new te::da::CheckConstraint());

  //::v8::Local<::v8::Object> jcatalog = te::v8::common::Make(catalog.get(), te::v8::jsi::GetCheckConstraintTemplate, true);

  //catalog.release();

  //return hs.Close(jcatalog);

  return ::v8::Undefined();
}

void te::v8::jsi::RegisterCheckConstraint(::v8::Local<::v8::Object>& global)
{
  ::v8::Local<::v8::FunctionTemplate> jscheckconstraint = ::v8::FunctionTemplate::New(CheckConstraint_Constructor);

  global->Set(::v8::String::New("TeCheckConstraint"), jscheckconstraint->GetFunction());
}

static ::v8::Persistent<::v8::FunctionTemplate> sg_checkconstraint_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetCheckConstraintTemplate()
{
  if(sg_checkconstraint_template.IsEmpty())
  {
    ::v8::Persistent<::v8::FunctionTemplate>& cTpl = GetConstraintTemplate();
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    result->Inherit(cTpl);

    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("getExpression"), ::v8::FunctionTemplate::New(CheckConstraint_GetExpression));
    prototype->Set(::v8::String::NewSymbol("setExpression"), ::v8::FunctionTemplate::New(CheckConstraint_SetExpression));

    sg_checkconstraint_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sg_checkconstraint_template;
}
