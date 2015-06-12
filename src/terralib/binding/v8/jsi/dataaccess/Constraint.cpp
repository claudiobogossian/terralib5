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
  \file Constraint.cpp
   
  \brief Utility functions to register the Constraint class into the Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../dataaccess/dataset/Constraint.h"
#include "../../common/Utils.h"
#include "DataAccess.h"

// Boost
#include <boost/cstdint.hpp>

::v8::Handle<::v8::Value> Constraint_GetId(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use the getId method, you must use the object notation: \"id = obj.getId();\""));

  te::da::Constraint* c = te::v8::common::Unwrap<te::da::Constraint>(args.Holder());

  if(c == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid constraint in the getId method!"));

  unsigned int id = c->getId();

  ::v8::Local<::v8::Integer> jid = ::v8::Uint32::New(static_cast<boost::uint32_t>(id));

  return hs.Close(jid);
}

::v8::Handle<::v8::Value> Constraint_SetId(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use setId method, you must use the object notation: \"obj.setId();\""));

  te::da::Constraint* c = te::v8::common::Unwrap<te::da::Constraint>(args.Holder());

  if(c == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid id in setId method!"));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsUint32())
    return ::v8::ThrowException(::v8::String::New("Invalid id in setId method!"));

  unsigned int id = args[0]->ToUint32()->Value();
  c->setId(id);

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> Constraint_GetName(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use the getName method you must use the object notation: \"name = obj.getName();\""));

  te::da::Constraint* c = te::v8::common::Unwrap<te::da::Constraint>(args.Holder());

  if(c == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid constraint in the getName method!"));

  const std::string& name = c->getName();

  ::v8::Local<::v8::String> jname = ::v8::String::New(name.c_str());

  return hs.Close(jname);
}

::v8::Handle<::v8::Value> Constraint_SetName(const ::v8::Arguments& args)
{
  if(args.Length() != 1 || args[0].IsEmpty() || args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use the setName method, you must use the object notation: \"obj.setName(\"name\");\""));

  te::da::Constraint* c = te::v8::common::Unwrap<te::da::Constraint>(args.Holder());

  if(c == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid constraint in the setName method!"));

  v8::String::Utf8Value jconnStr(args[0]->ToString());

  c->setName(*jconnStr);

  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Constraint_GetDataSetType(const ::v8::Arguments& /*args*/)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Constraint_SetDataSetType(const ::v8::Arguments& /*args*/)
{
  return ::v8::Undefined();
}

::v8::Handle<::v8::Value> Constraint_GetType(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use the getType method you must use the object notation: \"t = obj.getType();\""));

  te::da::Constraint* c = te::v8::common::Unwrap<te::da::Constraint>(args.Holder());

  if(c == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid constraint in the getType method!"));

  te::da::Constraint::ConstraintType type = c->getType();

  ::v8::Local<::v8::Integer> jtype = ::v8::Integer::New(static_cast<boost::int32_t>(type));

  return hs.Close(jtype);
}

::v8::Handle<::v8::Value> Constraint_Clone(const ::v8::Arguments& /*args*/)
{
  return ::v8::Undefined();
}

static ::v8::Persistent<::v8::FunctionTemplate> sg_constraint_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetConstraintTemplate()
{
  if(sg_constraint_template.IsEmpty())
  {
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();

    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("getId"), ::v8::FunctionTemplate::New(Constraint_GetId));
    prototype->Set(::v8::String::NewSymbol("setId"), ::v8::FunctionTemplate::New(Constraint_SetId));
    prototype->Set(::v8::String::NewSymbol("getName"), ::v8::FunctionTemplate::New(Constraint_GetName));
    prototype->Set(::v8::String::NewSymbol("setName"), ::v8::FunctionTemplate::New(Constraint_SetName));
    prototype->Set(::v8::String::NewSymbol("getDataSetType"), ::v8::FunctionTemplate::New(Constraint_GetDataSetType));
    prototype->Set(::v8::String::NewSymbol("setDataSetType"), ::v8::FunctionTemplate::New(Constraint_SetDataSetType));
    prototype->Set(::v8::String::NewSymbol("getType"), ::v8::FunctionTemplate::New(Constraint_GetType));
    prototype->Set(::v8::String::NewSymbol("clone"), ::v8::FunctionTemplate::New(Constraint_Clone));

    sg_constraint_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sg_constraint_template;
}

