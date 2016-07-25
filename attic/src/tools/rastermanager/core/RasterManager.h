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
  \file terralib/tools/rastermanager/core/RasterManager.h

  \brief Terralib Raster Manager Tool
*/

#ifndef __TERRALIB_TOOLS_RASTERMANAGER_RASTERMANAGER_H
#define __TERRALIB_TOOLS_RASTERMANAGER_RASTERMANAGER_H

// STL
#include <string>
#include <vector>

namespace te
{
  namespace tools
  {
    namespace rastermanager
    {
      /*!
        \class RasterManager

        \brief Class with methods to manage and manipulate rasters
      */
      class RasterManager
      {
        public:

          RasterManager();

          /*!
            \brief Init the application

            \param input Input raster path
            \param errorMessage String that receive methods errors description

            \return True if successfully
          */
          bool init(std::string input, std::string & errorMessage);

          /*!
            \brief Copy a raster

            \param output Output raster path
            \param bandVec Integer vector with the bands to copy
            \param bandType Define witch type the output raster will have
            \param errorMessage String that receive methods errors description

            \return True if successfully
          */
          bool copyRaster(std::string output, std::vector<int> bandVec, int bandType, std::string & errorMessage);    

          /*!
            \brief Show raster informations
      
            \param output Buffer with the informations
            \param errorMessage String that receive methods errors description

            \return True if successfully
          */
          bool getRasterInfo(std::ostringstream & output, std::string & errorMessage);

          /*!
            \brief Reproject a raster

            \param output Output raster path
            \param srid Srid code to resproject      
            \param errorMessage String that receive methods errors description

            \return True if successfully
          */
          bool reproject(std::string output, int srid, std::string & errorMessage);

          /*!
            \brief Convert a raster

            \param output Output raster path
            \param extension String with the extrenson to convert
            \param bandVec Integer vector with the bands to copy
            \param errorMessage String that receive methods errors description

            \return True if successfully
          */
          bool convert(std::string output, std::string extension, std::vector<int> bandVec, std::string & errorMessage);

          /*!
            \brief Change Raster resolution

            \param output Output raster path
            \param method Integer with the method used to change the resolution
                          1 for Nearest Neighbor method
                          2 for Bilinear method
                          3 for Bicubic method
            \param errorMessage String that receive methods errors description

            \return True if successfully
          */
          bool changeResolution(std::string output, int method,int scale, std::string & errorMessage);

          /*!
            \brief Trim a raster

            \param output Output raster path
            \param env Double vector with the new envelope (box)
            \param errorMessage String that receive methods errors description

            \return True if successfully
          */
          bool trim(std::string output, std::vector<double> env, std::string & errorMessage);

          /*!
            \brief Show Supported Extensions

            \param errorMessage String that receive methods errors description

            \return True if successfully
          */
          bool showSupportedExtensions(std::string & errorMessage);

          /*!
            \brief Get Supported Extensions

            \param extents String vector with the supported extensions
            \param errorMessage String that receive methods errors description

            \return True if successfully
          */
          bool getSupportedExtensions(std::vector<std::string> & extents, std::string & errorMessage);

        private:

          std::string m_input;  //!< The input raster path
          bool m_loadPlugin;

      };
    }
  }
}

#endif // __TERRALIB_TOOLS_RASTERMANAGER_RASTERMANAGER_H