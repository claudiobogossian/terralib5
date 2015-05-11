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
 
  \brief This file contains several utility functions for XSD support.
*/

#ifndef __TERRALIB_XSD_INTERNAL_UTILS_H
#define __TERRALIB_XSD_INTERNAL_UTILS_H

// TerraLib 
#include "Enums.h"

// STL
#include <cassert>
#include <string>

namespace te
{
  namespace xsd
  {
    /*!
      \brief It extracts an integer value that is a combination of: #all|list of (extension|restriction|substitution).

      \return An integer value that is a combination of: #all|list of (extension|restriction|substitution).

      \sa DerivationType
    */
    inline int ExtractExtension(const char* value)
    {
      assert(value);

      int i = 0;

// while we didn't reach the end of the string...
      while(*value != '\0')
      {
        if(strcmp(value, "#all") == 0)
          return ALL;          
        else if(strcmp(value, "extension") == 0)
        {
          value += 9;
          i |= EXTENSION;
        }
        else if(strcmp(value, "restriction") == 0)
        {
          value += 11;
          i |= RESTRICTION;
        }
        else if(strcmp(value, "substitution") == 0)
        {
          value += 12;
          i |= SUBSTITUTION;
        }

// skip white spaces
        while(*value == ' ')
          ++value;
      }

      return i;
    }

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_XSDUTILS_H
