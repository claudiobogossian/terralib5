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
  \file terralib/ws/ogc/wcs/client/XMLParser.h

  \brief Parse the WCS XML to a WCS struct

  \author Vinicius Campanha
*/

#ifndef __TERRALIB_WS_OGC_WCS_XMLPARSER_WCS_H
#define __TERRALIB_WS_OGC_WCS_XMLPARSER_WCS_H

// STL
#include <vector>
#include <cctype>
#include <memory>
#include <sstream>

//BOOST
#include <boost/algorithm/string.hpp>

// TerraLib
#include "../../../../xml.h"
#include "DataTypes.h"

namespace te
{
  namespace ws
  {
    namespace ogc
    {
      namespace wcs
      {
        /*!
          \class XMLParser

          \brief A class responsible to parse WCS XML files to WCS TerraLib data types.
        */
        class XMLParser
        {
        public:

          XMLParser();

          virtual ~XMLParser();

          Capabilities parseCapabilities(const std::string& xmlPath);

          CoverageDescription parseDescribeCoverage(const std::string& xmlPath);

        private:

          TimeDomain parseTimeDomain(const std::auto_ptr<te::xml::Reader>& reader);

        };
      } // end namespace wcs
    } // end namespace ogc
  } // end namespace ws
} // end namespace te

#endif // __TERRALIB_WS_OGC_WCS_XMLPARSER_WCS_H
