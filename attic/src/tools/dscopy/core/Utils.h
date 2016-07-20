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
  \file terralib/tools/dscopy/core/Utils.h

  \brief Terralib Raster Manager Tool Utils 
*/

#ifndef __TERRALIB_TOOLS_DSCOPY_UTILS_H
#define __TERRALIB_TOOLS_DSCOPY_UTILS_H

// STL
#include <string>

namespace te
{
  namespace tools
  {
    namespace dscopy
    {
      /*!
        \class Utils

        \brief Class with help methods for the application
      */
      class Utils
      {
        public:   

          /*!
           \brief Load Terralib modules

            \param errorMessage String that receive errors description

            \return True if successfully
          */
          static bool loadModules(std::string & errorMessage);


      };
    }
  }
}

#endif // __TERRALIB_TOOLS_DSCOPY_UTILS_H