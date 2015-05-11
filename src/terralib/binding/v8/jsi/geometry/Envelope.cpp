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
  \file Envelope.cpp
   
  \brief Utility functions to register the Envelope class into Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../geometry/Envelope.h"
#include "../../common/Utils.h"
#include "Geometry.h"

// STL
#include <memory>

::v8::Handle<::v8::Value> Envelope_LLXGetter(::v8::Local<::v8::String> /*prop*/, const ::v8::AccessorInfo& info)
{
  ::v8::HandleScope hs;

  if(info.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("Missing object in Envelope llx getter!"));

  te::gm::Envelope* e = te::v8::common::Unwrap<te::gm::Envelope>(info.Holder());

  return ::v8::Number::New(e->m_llx);
}

void Envelope_LLXSetter(::v8::Local<::v8::String> /*prop*/, ::v8::Local<::v8::Value> value, const ::v8::AccessorInfo& info)
{
  ::v8::HandleScope hs;

  if(info.Holder().IsEmpty())
  {
    ::v8::ThrowException(::v8::String::New("Missing object in Envelope llx setter!"));
    return;
  }

  if(value.IsEmpty() || !value->IsNumber())
  {
    ::v8::ThrowException(::v8::String::New("Missing value or wrong type in Envelope llx setter!"));
    return;
  }

  te::gm::Envelope* e = te::v8::common::Unwrap<te::gm::Envelope>(info.Holder());

  double llx = value->ToNumber()->Value();

  e->m_llx = llx;

  return;
}

::v8::Handle<::v8::Value> Envelope_LLYGetter(::v8::Local<::v8::String> /*prop*/, const ::v8::AccessorInfo& info)
{
  ::v8::HandleScope hs;

  if(info.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("Missing object in Envelope lly getter!"));

  te::gm::Envelope* e = te::v8::common::Unwrap<te::gm::Envelope>(info.Holder());

  return ::v8::Number::New(e->m_lly);
}

void Envelope_LLYSetter(::v8::Local<::v8::String> /*prop*/, ::v8::Local<::v8::Value> value, const ::v8::AccessorInfo& info)
{
  ::v8::HandleScope hs;

  if(info.Holder().IsEmpty())
  {
    ::v8::ThrowException(::v8::String::New("Missing object in Envelope lly setter!"));
    return;
  }

  if(value.IsEmpty() || !value->IsNumber())
  {
    ::v8::ThrowException(::v8::String::New("Missing value or wrong type in Envelope lly setter!"));
    return;
  }

  te::gm::Envelope* e = te::v8::common::Unwrap<te::gm::Envelope>(info.Holder());

  double lly = value->ToNumber()->Value();

  e->m_lly = lly;

  return;
}

::v8::Handle<::v8::Value> Envelope_URXGetter(::v8::Local<::v8::String> /*prop*/, const ::v8::AccessorInfo& info)
{
  ::v8::HandleScope hs;

  if(info.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("Missing object in Envelope urx getter!"));

  te::gm::Envelope* e = te::v8::common::Unwrap<te::gm::Envelope>(info.Holder());

  return ::v8::Number::New(e->m_urx);
}

void Envelope_URXSetter(::v8::Local<::v8::String> /*prop*/, ::v8::Local<::v8::Value> value, const ::v8::AccessorInfo& info)
{
  ::v8::HandleScope hs;

  if(info.Holder().IsEmpty())
  {
    ::v8::ThrowException(::v8::String::New("Missing object in Envelope urx setter!"));
    return;
  }

  if(value.IsEmpty() || !value->IsNumber())
  {
    ::v8::ThrowException(::v8::String::New("Missing value or wrong type in Envelope urx setter!"));
    return;
  }

  te::gm::Envelope* e = te::v8::common::Unwrap<te::gm::Envelope>(info.Holder());

  double urx = value->ToNumber()->Value();

  e->m_urx = urx;

  return;
}

::v8::Handle<::v8::Value> Envelope_URYGetter(::v8::Local<::v8::String> /*prop*/, const ::v8::AccessorInfo& info)
{
  ::v8::HandleScope hs;

  if(info.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("Missing object in Envelope ury getter!"));

  te::gm::Envelope* e = te::v8::common::Unwrap<te::gm::Envelope>(info.Holder());

  return ::v8::Number::New(e->m_ury);
}

void Envelope_URYSetter(::v8::Local<::v8::String> /*prop*/, ::v8::Local<::v8::Value> value, const ::v8::AccessorInfo& info)
{
  ::v8::HandleScope hs;

  if(info.Holder().IsEmpty())
  {
    ::v8::ThrowException(::v8::String::New("Missing object in Envelope ury setter!"));
    return;
  }

  if(value.IsEmpty() || !value->IsNumber())
  {
    ::v8::ThrowException(::v8::String::New("Missing value or wrong type in Envelope ury setter!"));
    return;
  }

  te::gm::Envelope* e = te::v8::common::Unwrap<te::gm::Envelope>(info.Holder());

  double ury = value->ToNumber()->Value();

  e->m_ury = ury;

  return;
}

::v8::Handle<::v8::Value> Envelope_Constructor(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(!args.IsConstructCall())
    return ::v8::ThrowException(::v8::String::New("In order to create an envelope you need call its constructor like: var e = new TeEnvelope(1.0, 1.0, 5.0, 5.0);"));

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("Envelope constructor must use object notation!"));

  if((args.Length() != 4) || !args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber() || !args[3]->IsNumber())
    return ::v8::ThrowException(::v8::String::New("Missing parameter in Envelope constructor or wrong parameter type!"));

  double llx = args[0]->ToNumber()->Value();
  double lly = args[1]->ToNumber()->Value();
  double urx = args[2]->ToNumber()->Value();
  double ury = args[3]->ToNumber()->Value();

  std::auto_ptr<te::gm::Envelope> e(new te::gm::Envelope(llx, lly, urx, ury));

  ::v8::Local<::v8::Object> je = te::v8::common::Make(e.get(), te::v8::jsi::GetEnvelopeTemplate, true);

  e.release();

  return hs.Close(je);
}

//prototype->Set(::v8::String::NewSymbol("init"), ::v8::FunctionTemplate::New(Envelope_));
//prototype->Set(::v8::String::NewSymbol("getLowerLeftX"), ::v8::FunctionTemplate::New(Envelope_));
//prototype->Set(::v8::String::NewSymbol("getLowerLeftY"), ::v8::FunctionTemplate::New(Envelope_));
//prototype->Set(::v8::String::NewSymbol("getUpperRightX"), ::v8::FunctionTemplate::New(Envelope_));
//prototype->Set(::v8::String::NewSymbol("getUpperRightY"), ::v8::FunctionTemplate::New(Envelope_));
//prototype->Set(::v8::String::NewSymbol("getLowerLeft"), ::v8::FunctionTemplate::New(Envelope_));
//prototype->Set(::v8::String::NewSymbol("getUpperRight"), ::v8::FunctionTemplate::New(Envelope_));
//prototype->Set(::v8::String::NewSymbol("makeInvalid"), ::v8::FunctionTemplate::New(Envelope_));
//prototype->Set(::v8::String::NewSymbol("isValid"), ::v8::FunctionTemplate::New(Envelope_));
//prototype->Set(::v8::String::NewSymbol("getWidth"), ::v8::FunctionTemplate::New(Envelope_));
//prototype->Set(::v8::String::NewSymbol("getHeight"), ::v8::FunctionTemplate::New(Envelope_));
//prototype->Set(::v8::String::NewSymbol("getArea"), ::v8::FunctionTemplate::New(Envelope_));
//prototype->Set(::v8::String::NewSymbol("equals"), ::v8::FunctionTemplate::New(Envelope_));
//prototype->Set(::v8::String::NewSymbol("disjoint"), ::v8::FunctionTemplate::New(Envelope_));
//prototype->Set(::v8::String::NewSymbol("intersects"), ::v8::FunctionTemplate::New(Envelope_));
//prototype->Set(::v8::String::NewSymbol("touches"), ::v8::FunctionTemplate::New(Envelope_));
//prototype->Set(::v8::String::NewSymbol("within"), ::v8::FunctionTemplate::New(Envelope_));
//prototype->Set(::v8::String::NewSymbol("contains"), ::v8::FunctionTemplate::New(Envelope_));
//prototype->Set(::v8::String::NewSymbol("distance"), ::v8::FunctionTemplate::New(Envelope_));
//prototype->Set(::v8::String::NewSymbol("intersection"), ::v8::FunctionTemplate::New(Envelope_));
//prototype->Set(::v8::String::NewSymbol("union"), ::v8::FunctionTemplate::New(Envelope_));
//prototype->Set(::v8::String::NewSymbol("transform"), ::v8::FunctionTemplate::New(Envelope_));

void te::v8::jsi::RegisterEnvelope(::v8::Local<::v8::Object>& global)
{
  ::v8::HandleScope hs;

  ::v8::Local<::v8::FunctionTemplate> jsc = ::v8::FunctionTemplate::New(Envelope_Constructor);

  global->Set(::v8::String::New("TeEnvelope"), jsc->GetFunction());
}

static ::v8::Persistent<::v8::FunctionTemplate> sg_envelope_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetEnvelopeTemplate()
{
  if(sg_envelope_template.IsEmpty())
  {
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();

    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->SetAccessor(::v8::String::NewSymbol("llx"), Envelope_LLXGetter, Envelope_LLXSetter);
    prototype->SetAccessor(::v8::String::NewSymbol("lly"), Envelope_LLYGetter, Envelope_LLYSetter);
    prototype->SetAccessor(::v8::String::NewSymbol("urx"), Envelope_URXGetter, Envelope_URXSetter);
    prototype->SetAccessor(::v8::String::NewSymbol("ury"), Envelope_URYGetter, Envelope_URYSetter);

    //prototype->Set(::v8::String::NewSymbol("init"), ::v8::FunctionTemplate::New(Envelope_));
    //prototype->Set(::v8::String::NewSymbol("getLowerLeftX"), ::v8::FunctionTemplate::New(Envelope_));
    //prototype->Set(::v8::String::NewSymbol("getLowerLeftY"), ::v8::FunctionTemplate::New(Envelope_));
    //prototype->Set(::v8::String::NewSymbol("getUpperRightX"), ::v8::FunctionTemplate::New(Envelope_));
    //prototype->Set(::v8::String::NewSymbol("getUpperRightY"), ::v8::FunctionTemplate::New(Envelope_));
    //prototype->Set(::v8::String::NewSymbol("getLowerLeft"), ::v8::FunctionTemplate::New(Envelope_));
    //prototype->Set(::v8::String::NewSymbol("getUpperRight"), ::v8::FunctionTemplate::New(Envelope_));
    //prototype->Set(::v8::String::NewSymbol("makeInvalid"), ::v8::FunctionTemplate::New(Envelope_));
    //prototype->Set(::v8::String::NewSymbol("isValid"), ::v8::FunctionTemplate::New(Envelope_));
    //prototype->Set(::v8::String::NewSymbol("getWidth"), ::v8::FunctionTemplate::New(Envelope_));
    //prototype->Set(::v8::String::NewSymbol("getHeight"), ::v8::FunctionTemplate::New(Envelope_));
    //prototype->Set(::v8::String::NewSymbol("getArea"), ::v8::FunctionTemplate::New(Envelope_));
    //prototype->Set(::v8::String::NewSymbol("equals"), ::v8::FunctionTemplate::New(Envelope_));
    //prototype->Set(::v8::String::NewSymbol("disjoint"), ::v8::FunctionTemplate::New(Envelope_));
    //prototype->Set(::v8::String::NewSymbol("intersects"), ::v8::FunctionTemplate::New(Envelope_));
    //prototype->Set(::v8::String::NewSymbol("touches"), ::v8::FunctionTemplate::New(Envelope_));
    //prototype->Set(::v8::String::NewSymbol("within"), ::v8::FunctionTemplate::New(Envelope_));
    //prototype->Set(::v8::String::NewSymbol("contains"), ::v8::FunctionTemplate::New(Envelope_));
    //prototype->Set(::v8::String::NewSymbol("distance"), ::v8::FunctionTemplate::New(Envelope_));
    //prototype->Set(::v8::String::NewSymbol("intersection"), ::v8::FunctionTemplate::New(Envelope_));
    //prototype->Set(::v8::String::NewSymbol("union"), ::v8::FunctionTemplate::New(Envelope_));
    //prototype->Set(::v8::String::NewSymbol("transform"), ::v8::FunctionTemplate::New(Envelope_));

    sg_envelope_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sg_envelope_template;
}

