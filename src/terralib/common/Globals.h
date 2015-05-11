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
  \file terralib/common/Globals.h

  \brief An static class with global definitions.

  \ingroup common
*/

#ifndef __TERRALIB_COMMON_INTERNAL_GLOBALS_H
#define __TERRALIB_COMMON_INTERNAL_GLOBALS_H

// TerraLib
#include "Config.h"
#include "Enums.h"

// STL
#include <string>

namespace te
{
  namespace common
  {
    /*!
      \class Globals

      \brief An static class with global definitions.

      You can access this class to get some specific values
      defined on TerraLib startup.
    */
    class TECOMMONEXPORT Globals
    {
      public:

        static const MachineByteOrder sm_machineByteOrder;  //!< A flag that indicates the machine byte order (Big Endian or Little Endian).
        static const std::string sm_dummyStr;               //!< A dummy string value.
        static const std::string sm_nanStr;                 //!< Not a number string value.
      
      private:
      
        Globals();
        ~Globals();
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_GLOBALS_H

