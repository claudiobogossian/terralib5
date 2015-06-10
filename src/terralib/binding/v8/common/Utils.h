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
  \file Utils.h

  \brief Utility functions for dealing with JNI.
 */

#ifndef __TERRALIB_BINDING_V8_COMMON_INTERNAL_UTILS_H
#define __TERRALIB_BINDING_V8_COMMON_INTERNAL_UTILS_H

// TerraLib
#include "Config.h"
#include "JsObject.h"

// STL
#include <cassert>
#include <map>
#include <string>

// Google V8
#include <v8.h>

namespace te
{
  namespace v8
  {
    namespace common
    {
      /*!
        \brief It reads a file into a v8 string.

        \param fileName The name of the file to be read.

        \return A v8 string with the file content.

        \exception Exception It throws an exception if it can not read the file.

        \pre A HandleScope must exist before calling this method.

        \todo Check the implementaion for exceptions in the read loop!
       */
      TEV8COMMONEXPORT ::v8::Handle<::v8::String> ReadFile(const std::string& fileName);

      /*!
        \brief It caches the object method into the persistent output functor.

        \param obj        The object that must have a method called methodName.
        \param methodName The name of a method belonging to obj.
        \param outFtor    A persistent function where the reference to the method will me cached.

        \exception Exception It throws an exception if it can not find the method or something goes wrong.

        \pre A HandleScope must exist before calling this method.
       */
      TEV8COMMONEXPORT void Cache(::v8::Local<::v8::Object>& obj, const std::string& methodName, ::v8::Persistent<::v8::Function>& outFtor);

      /*!
        \brief This function will print all the arguments to the standard output.

        \param args The arguments to be printed.
        
        \return An undefinded V8 value just to allow the function to be exported to JavaScript environment!
       */
      TEV8COMMONEXPORT ::v8::Handle<::v8::Value> Print(const ::v8::Arguments& args);

      /*!
        \brief An utility function that extracts the C++ object from a wrapped object.

        \param obj The object holding the C++ pointer.

        \return The C++ pointer casted to the right type.

        \pre There must be a valid HandleScope before calling this routine.
       */
      template<class T> inline T* Unwrap(::v8::Handle<::v8::Object> obj)
      {
        ::v8::Handle<::v8::External> field = ::v8::Handle<::v8::External>::Cast(obj->GetInternalField(0));
        void* p = field->Value();
        JsObject<T>* jsObj = static_cast<JsObject<T>*>(p);
        return static_cast<T*>(jsObj->m_handle);
      }

      /*!
        \brief An utility function that extracts the C++ object from a wrapped object.

        \param obj The object holding the C++ pointer.

        \return The C++ pointer casted to the right type.

        \pre There must be a valid HandleScope before calling this routine.
       */
      template<class T> inline T* UnwrapAndLooseOwnership(::v8::Handle<::v8::Object> obj)
      {
        ::v8::Handle<::v8::External> field = ::v8::Handle<::v8::External>::Cast(obj->GetInternalField(0));
        void* p = field->Value();
        JsObject<T>* jsObj = static_cast<JsObject<T>*>(p);
        jsObj->m_isOwner = false;
        return static_cast<T*>(jsObj->m_handle);
      }

      /*!
        \brief An utility function that extracts the C++ object from a wrapped object.

        \param obj The object holding the C++ pointer.

        \return The C++ pointer casted to the right type.

        \pre There must be a valid HandleScope before calling this routine.
       */
      template<class T> inline void LooseOwnership(::v8::Handle<::v8::Object> obj)
      {
        ::v8::Handle<::v8::External> field = ::v8::Handle<::v8::External>::Cast(obj->GetInternalField(0));
        void* p = field->Value();
        JsObject<T>* jsObj = static_cast<JsObject<T>*>(p);
        jsObj->m_isOwner = false;
        return;
      }

      /*!
        \brief The call-back function for releasing objects.

        \param obj       The object to be disposed.
        \param parameter The C++ pointer to be released.

        \pre There must be a valid HandleScope before calling this routine.
       */
      template<class T> void JsObjectRelease(::v8::Persistent<::v8::Value> obj, void* parameter)
      {
        //::v8::HandleScope hs;

        ::v8::Handle<::v8::Object> oobj = obj->ToObject();

        ::v8::Handle<::v8::External> field = ::v8::Handle<::v8::External>::Cast(oobj->GetInternalField(0));

        void* p = field->Value();

        JsObject<T>* cobj = static_cast<JsObject<T>*>(p);

        assert(cobj == static_cast<JsObject<T>*>(parameter));

        delete cobj;

        obj.Dispose();
      }

      /*!
        \brief It creates a new JavaScript object from a C++ object (obj).

        The new object will have tfunc as its object template.
        If isOwner is true the JavaScript object will be the owner of the given C++ object pointer.

        \param obj     A valid pointer to a C++ object.
        \param tfunc   A function with no arguments that returns the object template to be applied.
        \param isOwner If true when the JavaSript object goes out if the scope it automatically release the C++ object, otherwise, it doesn't.

        \pre There must be a valid HandleScope before calling this routine.
       */
      template<class T, class TF> inline ::v8::Local<::v8::Object> Make(T* obj, TF tfunc, const bool isOwner)
      {
        //::v8::HandleScope hs;

        ::v8::Persistent<::v8::FunctionTemplate>& objFuncTemplate = tfunc();
        ::v8::Local<::v8::ObjectTemplate> objTemplate = objFuncTemplate->InstanceTemplate();
        objTemplate->SetInternalFieldCount(1);
        ::v8::Local<::v8::Object> jsObj = objTemplate->NewInstance();
  
// store the C++ object pointer in the JavaScript object
        JsObject<T>* o = new JsObject<T>(obj, isOwner);

        ::v8::Local<::v8::External> objPtr = ::v8::External::New(o);
        jsObj->SetInternalField(0, objPtr);

// set the release call back function for the object
        ::v8::Persistent<::v8::Object> pjsObj = ::v8::Persistent<::v8::Object>::New(jsObj);

        pjsObj.MakeWeak(o, JsObjectRelease<T>);

        //return hs.Close(jsObj);
        return jsObj;
      }

      /*!
        \brief It converts the input map to an object representing the associative conteiner.

        \param m An input map to be converted to a JavsScript object.

        \pre There must be a valid HandleScope before calling this routine.

        \return An object representing the associative conteiner.
       */
      inline ::v8::Local<::v8::Object> Convert2Js(const std::map<std::string, std::string>& m)
      {
        ::v8::Local<::v8::Object> jsObj = ::v8::Object::New();

        std::map<std::string, std::string>::const_iterator it = m.begin();
        std::map<std::string, std::string>::const_iterator itend = m.begin();

        while(it != itend)
        {
          ::v8::Local<::v8::String> key = ::v8::String::New(it->first.c_str());
          ::v8::Local<::v8::String> value = ::v8::String::New(it->second.c_str());

          jsObj->Set(key, value);

          ++it;
        }

        return jsObj;        
      }

      /*!
        \brief It converts the input map to an object representing the associative conteiner.

        \param m An input map to be converted to a JavsScript object.

        \pre There must be a valid HandleScope before calling this routine.

        \return An object representing the associative conteiner.
       */
      inline void Convert2Cpp(const ::v8::Local<::v8::Object>& jsmap, std::map<std::string, std::string>& cppmap)
      {
        assert(!jsmap.IsEmpty());

        ::v8::Local<::v8::Array> amap = jsmap->GetPropertyNames();

        unsigned int size = amap->Length();
        
        for(unsigned int i = 0; i < size; ++i)
        {
          ::v8::Local<::v8::Value> key = amap->Get(i);
          ::v8::Local<::v8::Value> value = jsmap->Get(key);

          ::v8::String::Utf8Value ukey(key->ToString());
          ::v8::String::Utf8Value uvalue(value->ToString());

          cppmap[*ukey] = *uvalue;
        }

        return;
      }


    } // end namespace common
  }   // end namespace v8
}     // end namespace te

#endif  // __TERRALIB_BINDING_V8_COMMON_INTERNAL_UTILS_H

