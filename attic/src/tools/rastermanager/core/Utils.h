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
  \file terralib/tools/rastermanager/core/Utils.h

  \brief Terralib Raster Manager Tool Utils 
*/

#ifndef __TERRALIB_TOOLS_RASTERMANAGER_UTILS_H
#define __TERRALIB_TOOLS_RASTERMANAGER_UTILS_H

// STL
#include <string>

namespace te
{
  // Forward declarations
  namespace rst
  {
    class Raster;
  }

  namespace tools
  {
    namespace rastermanager
    {
      /*!
        \class Utils

        \brief Class with help methods for the application
      */
      class Utils
      {
        public:

          /*!
           \brief Check input raster path

            \param inputPath String with the input raster path to check
            \param errorMessage String that receive methods errors description

            \return True if successfully
          */
          static bool checkInputPath(std::string inputPath, std::string & errorMessage);

          /*!
           \brief Check output raster path

            \param outputPath String with the output raster path to check
            \param errorMessage String that receive methods errors description

            \return True if successfully
          */
          static bool checkOutputPath(std::string outputPath, std::string & errorMessage);

          /*!
           \brief Get a raster based in the path

            \param path String with the input raster path
            \param raster Raster pointer reference that gives the raster for the caller
            \param errorMessage String that receive methods errors description

            \return True if successfully
          */
          static bool getRaster(std::string path, te::rst::Raster* & raster, std::string & errorMessage);

          /*!
           \brief Load Terralib modules

            \param errorMessage String that receive methods errors description

            \return True if successfully
          */
          static bool loadModules(std::string & errorMessage);
      }; 
    }
  }
}



#endif // __TERRALIB_TOOLS_RASTERMANAGER_UTILS_H