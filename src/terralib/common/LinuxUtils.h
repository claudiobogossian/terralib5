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
  \file terralib/common/LinuxUtils.h

  \brief This file contains several utility functions when dealing with Linux specific API.

  \ingroup common
*/

#ifndef __TERRALIB_COMMON_INTERNAL_LINUXUTILS_H
#define __TERRALIB_COMMON_INTERNAL_LINUXUTILS_H

// TerraLib
#include "Config.h"

#if (TE_PLATFORM == TE_PLATFORMCODE_LINUX) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE)
// STL
#include <string>

namespace te
{
  namespace common
  {
    namespace lx
    {
      /*!
        \brief It returns a human readable string describing the most recent error that occurred from dlopen(), dlsym() or dlclose() since the last call to dlerror().
        
        It returns an empty string if no errors have occurred.

        \return The most recent error that occurred from shared library handling.
       */
      const std::string GetDlError();
      
    }   // end namespace lx
  }     // end namespace common
}       // end namespace te

#endif  // (TE_PLATFORM == TE_PLATFORMCODE_LINUX) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE)

#endif  //__TERRALIB_COMMON_INTERNAL_LINUXUTILS_H