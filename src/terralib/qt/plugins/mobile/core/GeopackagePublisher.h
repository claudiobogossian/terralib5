/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
\file src/terraMobilePlugin/core/GeopackagePublisher.h

\brief This file is used to Publisher operation.
*/

#ifndef __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_GEOPACKAGEPUBLISHER_H
#define __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_GEOPACKAGEPUBLISHER_H

#include "../Config.h"


// STL
#include <string>
#include <vector>

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace terramobile
      {

        struct GeopackageFile
        {
          std::string m_name;
          std::string m_status;
          std::string m_objId;
          std::string m_desc;
        };

        typedef std::vector<GeopackageFile> GeopackageFiles;

        /*!
        \class GeoPackagePublisher

        \brief This file is used to Publisher operation.
        */
        class GeopackagePublisher
        {

          public:

            GeopackagePublisher();

            ~GeopackagePublisher();

          public:

            std::string getErrorMessage();

            GeopackageFiles getGeopackageFiles(std::string url);

            void downloadGeopackageFile(std::string url, GeopackageFile gpkgFile, std::string pathDir);

            void uploadGeopackageFile(std::string url, std::string filePath);

          protected:

            GeopackageFiles readJSONInfo(std::string stream);

          protected:

            std::string m_errorMessage;

        };
      }   // end namespace thirdParty
    }     // end namespace plugins
  }       // end namespace qt
}         // end namespace te

#endif  // __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_GEOPACKAGEPUBLISHER_H

