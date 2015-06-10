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
  \file Geometry.h
   
  \brief JavaScript exporting routine for the TerraLib Geometry module.
 */

#ifndef __TERRALIB_BINDING_V8_JSI_DATATYPE_INTERNAL_DATATYPE_H
#define __TERRALIB_BINDING_V8_JSI_DATATYPE_INTERNAL_DATATYPE_H

// TerraLib
#include "../Config.h"

// Google V8
#include <v8.h>

namespace te
{
  namespace v8
  {
    namespace jsi
    {
      /*!
        \brief It returns a reference to the persistent template of an AbstractData object.

        \return A reference to the persistent template of am AbstractData object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetAbstractDataTemplate();

      /*!
        \brief It register the Envelope class.

        You can use the Envelope class like:
        \code
        var e = new TeEnvelope(1.0, 1.0, 5.0, 5.0);
        \endcode       

        \param global The global object that will be used to register the Envelope class

        \exception Exception It may throws an exception if it is not possible to register the class.
       */
      //void RegisterDateTime(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a Envelope object.

        \return A reference to the persistent template of a Envelope object.
       */
      //::v8::Persistent<::v8::FunctionTemplate>& GetDateTimeTemplate();

    } // end namespace jsi
  }   // end namespace v8
}     // end namespace te

#endif  // __TERRALIB_BINDING_V8_JSI_DATATYPE_INTERNAL_DATATYPE_H

