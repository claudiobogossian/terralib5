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
  \file terralib/stmemory/Globals.h
   
  \brief A static class with global definitions for the TerraLib ST In-memory driver.
*/

#ifndef __TERRALIB_STMEMORY_INTERNAL_GLOBAL_H
#define __TERRALIB_STMEMORY_INTERNAL_GLOBAL_H

// TerraLib
#include "../common/Static.h"
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace stmem
  {
    /*!
      \class Globals

      \brief An static class with global definitions for the TerraLib ST module.
    */
    class TESTMEMORYEXPORT Globals : public te::common::Static
    {
      public:

        static const std::string sm_driverIdentifier;  //!< The STMEMORY driver identifier.
    };

  } // end namespace stmem
}   // end namespace te

#endif  // __TERRALIB_STMEMORY_INTERNAL_GLOBAL_H

