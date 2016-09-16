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


#ifndef __TERRALIB_WS_OGC_WCS_XMLPARSER_WMS_H
#define __TERRALIB_WS_OGC_WCS_XMLPARSER_WMS_H

#include "DataTypes.h"

// TerraLib
#include "../../../../xml.h"

namespace te
{
  namespace ws
  {
    namespace ogc
    {
      namespace wms
      {
      /*!
        \class XMLParser

        \brief A class responsible to parse WMS XML files to WMS TerraLib data types.
      */
      class TEOGCWMSEXPORT XMLParser
      {
      public:

        XMLParser();

        virtual ~XMLParser();

        /*!
          \brief Method responsible to parse a WMS XML GetCapabilities file and
          create an WMSCapabilities instance with document metadata.

          \param xmlPath path of the file that will be parsed.

          \return Return the WMS Server Capabilities
        */
        virtual WMSCapabilities parseCapabilities(const std::string& xmlPath);

      private:

        Capability parseCapability(const std::auto_ptr<te::xml::Reader>& reader);

        Request parseRequest(const std::auto_ptr<te::xml::Reader>& reader);

        Layer parseLayers(const std::auto_ptr<te::xml::Reader>& reader);

        GeographicBoundingBox parseGeographicBoundingBox(const std::auto_ptr<te::xml::Reader>& reader);

        BoundingBox parseBoundingBox(const std::auto_ptr<te::xml::Reader>& reader);

        Dimension parseDimension(const std::auto_ptr<te::xml::Reader>& reader);

        Style parseStyle(const std::auto_ptr<te::xml::Reader>& reader);

        LegendURL parseLegendURL(const std::auto_ptr<te::xml::Reader>& reader);

        OnlineResource parseOnlineResource(const std::auto_ptr<te::xml::Reader>& reader);

        StyleSheetURL parseStyleSheetURL(const std::auto_ptr<te::xml::Reader>& reader);

        StyleURL parseStyleURL(const std::auto_ptr<te::xml::Reader>& reader);

        Operation parseOperation(const std::auto_ptr<te::xml::Reader>& reader, const std::string& operationName);

        HTTP parseHTTP(const std::auto_ptr<te::xml::Reader>& reader);

      };
      }
    }
  }
}

#endif //__TERRALIB_WS_OGC_WCS_XMLPARSER_WMS_H
