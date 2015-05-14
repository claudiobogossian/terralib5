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
  \file Global.h
   
  \brief An static class with global definitions.
 */

#ifndef __TERRALIB_XLINK_INTERNAL_GLOBALS_H
#define __TERRALIB_XLINK_INTERNAL_GLOBALS_H

// TerraLib
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace xl
  {
    /*!
      \class Global

      \brief An static class with global definitions.
     */
    class TEXLEXPORT Globals
    {
      private:
        
        /* \brief Not instantiable. */
        Globals();

      public:

        static const std::string sm_xlinkType;  //!< Fixed value for SimpleLink: "xlink:simple".
    };

  } // end namespace color
}   // end namespace te

#endif  // __TERRALIB_XLINK_INTERNAL_GLOBALS_H

