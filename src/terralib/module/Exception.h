/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file te/module/Exception.h

  \brief Module Manager specific exception types.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __TERRALIB_MODULE_EXCEPTION_H__
#define __TERRALIB_MODULE_EXCEPTION_H__

// TerraLib
#include "../Exception.h"

namespace te
{
  //! Module Manager namespace.
  namespace module
  {
    //! The base exception type in Module Manager.
    struct Exception: virtual te::Exception { };
    
    //! An exception indicating an error when loading a shared library.
    struct LibraryLoadException: virtual Exception { };

    //! An exception indicating an error when releasing a shared library.
    struct LibraryUnloadException: virtual Exception { };

    //! An exception indicating an error when searching for a given symbol in a shared library.
    struct LibrarySymbolNotFoundException: virtual Exception { };

    //! An exception indicating an error when adding a given path to the operational system search library path.
    struct LibraryInvalidSearchPathException: virtual Exception { };

    //! An exception indicating an error when reseting the operational system search library path.
    struct LibraryResetSearchPathException: virtual Exception { };

    //! An exception indicating the operational system search library path was not found.
    struct LibrarySearchPathException: virtual Exception { };

  }  // end namespace plugin
}    // end namespace te

#endif  // __TERRALIB_MODULE_EXCEPTION_H__
