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
  \file terralib/common/WinUtils.h

  \brief This file contains several utility functions when dealing with Microsoft Windows specific API.

  \ingroup common
*/

#ifndef __TERRALIB_COMMON_INTERNAL_WINUTILS_H
#define __TERRALIB_COMMON_INTERNAL_WINUTILS_H

// TerraLib
#include "Config.h"

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS

// STL
#include <string>

namespace te
{
  namespace common
  {
    namespace win
    {
      /*!
        \brief It obtains the last thread's error message in a Windows System.

        \return The last thread's error message in a Windows System.
       */
      TECOMMONEXPORT const std::string GetLastError();
    }   // end namespace win
  }     // end namespace common
}       // end namespace te

#endif  // TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS

#endif  //__TERRALIB_COMMON_INTERNAL_WINUTILS_H