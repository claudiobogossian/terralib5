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
  \file terralib/ws/ogc/wcs/client/WCSClient.h

  \brief WS Client for OGC WCS

  \author Vinicius Campanha
*/

#ifndef __TERRALIB_WS_OGC_WCS_CLIENT_WCS_H
#define __TERRALIB_WS_OGC_WCS_CLIENT_WCS_H

// STL
#include <memory>
#include <vector>
#include <map>
#include <string>

// Qt
#include <QXmlStreamReader>

// TerraLib
#include "../../../../dataaccess/dataset/DataSet.h"
#include "../../../../common/progress/TaskProgress.h"
#include "../../../../core/uri/URI.h"
#include "../../../core/CurlWrapper.h"
#include "XMLParser.h"
#include "DataTypes.h"


namespace te
{
  namespace ws
  {
    namespace ogc
    {

      /*!
        \class WCSClient

        \brief A class to retrieve information and data from a Web Coverage Service.
      */
      class TEOGCWCSEXPORT WCSClient
      {
      public:

        /*!
          \brief Class constructor. It initializes the m_uri and m_version class members

          \param usrDataDir  Directory to store used temporary XML and images from WCS server.
          \param uri         The adress of WCS server.
          \param version     The WCS version.
        */
        WCSClient(const std::string usrDataDir = "",const std::string uri = "", const std::string version = "2.0.1");

        /*!
          \brief Default destructor.
        */
        ~WCSClient();

        /*!
          \brief Method to get the capabilities from a WCS server and store in m_capabilities member

                 To access the information contained in the m_capabilities, use the getCapabilities() method.

        */
        void updateCapabilities();

        /*!
          \brief Method to get the information about a coverage in the WCS server

          \param coverage The name of the coverage to describe

          \return Return the information of the coverage in the WCS
        */
        te::ws::ogc::wcs::CoverageDescription describeCoverage(const std::string coverage);

        /*!
          \brief Method to get the coverage from the WCS server

          \param coverage The struct with the desire coverage parameters

          \return Returns a DataSet that contains the coverage
        */
        std::string getCoverage(const te::ws::ogc::wcs::CoverageRequest coverageRequest, te::common::TaskProgress* taskProgress = 0) const;

        /*!
          \brief Executes a request on a WCS server

          \param url The complete url of request

          \return Returns a path to a file
        */
        std::string makeFileRequest(const std::string url, const std::string fileName, te::common::TaskProgress* taskProgress = 0) const;


        /*!
          \brief Return the m_capabilities member.

                 The m_capabilities will be empty until the updateCapabilities() method its called.

          \return Returns a path to a file
        */
        const te::ws::ogc::wcs::Capabilities& getCapabilities() const;

        /*!
         * \brief Sets the TerraLib CurlWrapper to be used internally.
         *
         *        By default it is an instance of te::ws::core::CurlWrapper class.
         *
         * \param curlWrapper Wrapper to be used.
         */
        void setCurlWrapper(te::ws::core::CurlWrapper* curlWrapper);

      private:

        std::string getFileExtension(const std::string& format) const;

      private:
        std::string m_version;
        std::string m_dataDir;
        te::core::URI m_uri;
        te::ws::ogc::wcs::Capabilities m_capabilities;
        std::shared_ptr<te::ws::core::CurlWrapper> m_curl;
        std::map<std::string, te::ws::ogc::wcs::CoverageDescription> m_descriptionMap;
        te::ws::ogc::wcs::XMLParser                m_parser;
      };
    }
  }
}

#endif // __TERRALIB_WS_OGC_WCS_CLIENT_WCS_H

