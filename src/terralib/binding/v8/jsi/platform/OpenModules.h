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
  \file OpenModules.h
   
  \brief The main entry function for registering TerraLib API in a V8 environment.
 */

#ifndef __TERRALIB_BINDING_V8_JSI_INTERNAL_OPENMODULES_H
#define __TERRALIB_BINDING_V8_JSI_INTERNAL_OPENMODULES_H

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
        \brief It register the Module class.

        You can use the Module class as a singleton using the following syntax:
        \code
        TeModule.initialize();
        \endcode    

        \param global The global object that will be used to register the Module class

        \exception Exception It may throws an exception if it is not possible to register the class.
       */
      TEV8JSIEXPORT void RegisterModule(::v8::Local<::v8::Object>& global);

    } // end namespace jsi
  }   // end namespace v8
}     // end namespace te

#endif  // __TERRALIB_BINDING_V8_JSI_INTERNAL_OPENMODULES_H

