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
  \file terralib/ws/ogc/wcs-client/ClientWCS.h

  \brief WS Client for OGC WCS
*/

#ifndef __TERRALIB_WS_OGC_WCSCLIENT_H
#define __TERRALIB_WS_OGC_WCSCLIENT_H

// STL
#include <memory>

// Qt
#include <QXmlStreamReader>

// TerraLib
#include "../../../dataaccess/dataset/DataSet.h"


namespace te
{
  namespace ws
  {
    namespace ogc
    {

      /*! \brief A struct to set the parameters of wanted coverage */
      struct CoverageRequest
      {
        std::string coverageID;
        std::string format;
        std::string mediaType;
        std::map< std::string, std::string > subset; //!< The subset axe(key) and the limits(values). The limits should be separated by comma.
        std::map< std::string, std::string > parameters;
      };

      /*!
        \class ClientWCS

        \brief A class to retrieve information and data from a Web Coverage Service.
      */
      class ClientWCS
      {
      public:

        /*!
          \brief Class constructor. It initializes the uri_ and version_ class members

          \param uri      The adress of WCS server.
          \param version  The WCS version.
        */
        ClientWCS(const std::string uri, const std::string version = "2.0.1");

        ~ClientWCS();


        /*!
          \brief Method to get the capabilities from a WCS server

          \return A XML with the WCS capabilities of the WCS server
        */
        QXmlStreamReader*  getCapabilities();

        /*!
          \brief Method to get the information about a coverage in the WCS server

          \param coverage The name of the coverage to describe

          \return Return the information of the coverage in the WCS
        */
        QXmlStreamReader* describeCoverage(const std::string coverage) const;

        /*!
          \brief Method to get the coverage from the WCS server

          \param coverage The struct with the desire coverage parameters

          \return Returns a DataSet that contains the coverage
        */
        te::da::DataSet* getCoverage(const struct CoverageRequest coverage) const;

        /*!
          \brief Executes a request on a WCS server

          \param url The complete url of request

          \return Returns a XML with info requested
        */
        QXmlStreamReader* makeRequest(const std::string url) const;

      private:

        ClientWCS(const ClientWCS&);

        ClientWCS& operator=(const ClientWCS&);


      private:
        std::string uri_;
        std::string version_;
      };
    }
  }
}

#endif // __TERRALIB_WS_OGC_WCSCLIENT_H

