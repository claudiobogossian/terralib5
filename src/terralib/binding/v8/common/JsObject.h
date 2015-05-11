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
  \file JsObject.h

  \brief An auxiliary data structure for helping to control the garbage collection of wrapped C++ objects associated to JavaScript objects in Google's V8 engine.
 */

#ifndef __TERRALIB_BINDING_V8_COMMON_INTERNAL_JSOBJECT_H
#define __TERRALIB_BINDING_V8_COMMON_INTERNAL_JSOBJECT_H

// TerraLib
#include "Config.h"

namespace te
{
  namespace v8
  {
    namespace common
    {
      /*!
        \struct JsObject

        \brief An auxiliary data structure for helping to control the garbage collection of wrapped C++ objects associated to JavaScript objects in Google's V8 engine.
       */
      template<class T> struct JsObject
      {
        T* m_handle;      //!< A pointer to a C++ object.
        bool m_isOwner;   //!< If true it specifies that JsObject has the ownership of the C++ handle.

        /*!
          \brief Initializes a new garbage collection helper object.

          \param handle  A pointer to a C++ object.
          \param isOwner If true the JsObject will have the ownership of the pointer.
         */
        JsObject(T* handle, bool isOwner)
          : m_handle(handle),
            m_isOwner(isOwner)
        {
        }

        /*! \brief The destructor will check if it is necessary to release the C++ object handle. */
        ~JsObject()
        {
          if(m_isOwner)
            delete m_handle;
        }
      };

    } // end namespace common
  }   // end namespace v8
}     // end namespace te

#endif  // __TERRALIB_BINDING_V8_COMMON_INTERNAL_JSOBJECT_H
