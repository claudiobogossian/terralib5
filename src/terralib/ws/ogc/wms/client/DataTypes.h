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
  \file terralib/ws/ogc/wms/client/DataTypes.h

  \brief Specifies containers for WMS data types.

  \author Emerson Moraes
*/

#ifndef __TERRALIB_WS_OGC_WMS_DATATYPES_H
#define __TERRALIB_WS_OGC_WMS_DATATYPES_H

#include "Config.h"

#include <vector>
#include <string>

#include <boost/lexical_cast.hpp>

namespace te
{
  namespace ws
  {
    namespace ogc
    {
      namespace wms
      {

      /*!
       * \brief The OnlineResource WMS 1.3.0 struct.
       *
       * An OnlineResource is typically an HTTP URL.
       * The URL is placed in the xlink:href attribute, and the value "simple" is placed in the xlink:type attribute.
       */
      struct TEOGCWMSEXPORT OnlineResource
      {
        std::string m_href;
        std::string m_type;
      };

      struct TEOGCWMSEXPORT HTTP
      {
        OnlineResource m_get;
        OnlineResource m_post;
      };

      /*!
       * \brief The GeographicBoundingBox WMS 1.3.0 struct.
       *
       * The EX_GeographicBoundingBox attributes indicate the limits of the enclosing rectangle in longitude and latitude decimal degrees.
       */
      struct TEOGCWMSEXPORT GeographicBoundingBox
      {
        double m_westBoundLongitude;
        double m_eastBoundLongitude;
        double m_southBoundLatitude;
        double m_northBoundLatitude;

        GeographicBoundingBox() :
          m_westBoundLongitude(0.0),
          m_eastBoundLongitude(0.0),
          m_southBoundLatitude(0.0),
          m_northBoundLatitude(0.0)
        {}
      };

      /*!
       * \brief The BoundingBox WMS 1.3.0 struct.
       *
       * The BoundingBox attributes indicate the limits of the bounding box in units of the specified coordinate reference system.
       */
      struct TEOGCWMSEXPORT BoundingBox
      {
        std::string m_crs;
        double      m_minX;
        double      m_minY;
        double      m_maxX;
        double      m_maxY;
        double      m_resX;
        double      m_resY;

        BoundingBox() :
          m_minX(0.0),
          m_minY(0.0),
          m_maxX(0.0),
          m_maxY(0.0),
          m_resX(0.0),
          m_resY(0.0)
        {}
      };

      /*!
       * \brief The Dimension WMS 1.3.0 struct.
       *
       * The Dimension element declares the existence of a dimension and indicates what values along a dimension are valid.
       */
      struct TEOGCWMSEXPORT Dimension
      {
        std::string m_name;
        std::string m_units;
        std::string m_unitSymbol;
        std::string m_default;
        bool        m_multipleValues;
        bool        m_nearestValue;
        bool        m_current;

        Dimension() :
          m_multipleValues(false),
          m_nearestValue(false),
          m_current(false)
        {}
      };

      /*!
       * \brief The LegendURL WMS 1.3.0 struct.
       *
       * A Map Server may use zero or more LegendURL elements to provide an image(s) of a legend relevant to each Style of a Layer.
       * The Format element indicates the MIME type of the legend.
       * Width and height attributes may be provided to assist client applications in laying out space to display the legend.
       */
      struct TEOGCWMSEXPORT LegendURL
      {
        std::string    m_format;
        OnlineResource m_onlineResource;
        unsigned int   m_width;
        unsigned int   m_height;

        LegendURL() :
          m_width(0),
          m_height(0)
        {}
      };

      /*!
       * \brief The Operation WMS 1.3.0 struct.
       *
       * For each operation offered by the server,
       * list the available output formats and the online resource.
       */
      struct TEOGCWMSEXPORT Operation
      {
        std::vector<std::string> m_formats;
        std::vector<HTTP>        m_dcps;
      };

      /*!
       * \brief The StyleSheeetURL WMS 1.3.0 struct.
       *
       * StyleSheeetURL provides symbology information for each Style of a Layer.
       */
      struct TEOGCWMSEXPORT StyleSheetURL
      {
        std::string    m_format;
        OnlineResource m_onlineResource;
      };

      /*!
       * \brief The StyleURL WMS 1.3.0 struct.
       *
       * A Map Server may use StyleURL to offer more information about the data or symbology underlying a particular Style.
       * While the semantics are not well-defined,
       * as long as the results of an HTTP GET request against the StyleURL are properly MIME-typed,
       * Viewer Clients and Cascading Map Servers can make use of this.
       * A possible use could be to allow a Map Server to provide legend information.
       */
      struct TEOGCWMSEXPORT StyleURL
      {
        std::string    m_format;
        OnlineResource m_onlineResource;
      };

      /*!
       * \brief The Style WMS 1.3.0 struct.
       *
       * A Style element lists the name by which a style is requested and a human-readable title for pick lists,
       * optionally (and ideally) provides a human-readable description, and optionally gives a style URL.
       */
      struct TEOGCWMSEXPORT Style
      {
        std::string   m_name;
        std::string   m_title;
        std::string   m_abstract;
        LegendURL     m_legendURL;
        StyleSheetURL m_styleSheetURL;
        StyleURL      m_styleURL;
      };

      /*!
       * \brief The Layer WMS 1.3.0 struct.
       *
       * Nested list of zero or more map Layers offered by WMS server.
       */
      struct TEOGCWMSEXPORT Layer
      {
        std::string              m_name;
        std::string              m_title;
        std::string              m_abstract;
        std::vector<std::string> m_crs;
        std::vector<std::string> m_keywordList;
        GeographicBoundingBox    m_geoBoundingBox;
        std::vector<BoundingBox> m_boundingBoxes;
        std::vector<Dimension>   m_dimensions;
        std::vector<Style>       m_styles;
        double                   m_minScaleDenominator;
        double                   m_maxScaleDenominator;
        std::vector<Layer>       m_layers;
        bool                     m_queryable;
        unsigned int             m_cascaded;
        bool                     m_opaque;
        bool                     m_noSubsets;
        unsigned int             m_fixedWidth;
        unsigned int             m_fixedHeight;

        Layer() :
          m_minScaleDenominator(0.0),
          m_maxScaleDenominator(0.0),
          m_queryable(false),
          m_cascaded(0),
          m_opaque(false),
          m_noSubsets(false),
          m_fixedWidth(0),
          m_fixedHeight(0)
        {}
      };

      /*!
       * \brief The Request WMS 1.3.0 struct.
       *
       * Available WMS Operations are listed in a Request element.
       */
      struct TEOGCWMSEXPORT Request
      {
        Operation m_getCapabilities;
        Operation m_getMap;
        Operation m_getFeatureInfo;
      };

      /*!
       * \brief The Capability WMS 1.3.0 struct.
       *
       * A Capability lists available request types.
       * It also includes an optional list of map layers available from this server.
       */
      struct TEOGCWMSEXPORT Capability
      {
        Request m_request;
        Layer   m_layer;
      };

      /*!
       * \brief The Capability WMS 1.3.0 struct.
       *
       * A WMSCapabilities document is returned in response
       * to a GetCapabilities request made on a WMS.
       */
      struct TEOGCWMSEXPORT WMSCapabilities
      {
        Capability m_capability;
      };

      /*!
       * \brief The WMSGetMapRequest WMS 1.3.0 struct.
       *
       * A WMSGetMapRequest is a struct to be used on
       * WMS 1.3.0 GetMap requests.
       */
      struct TEOGCWMSEXPORT WMSGetMapRequest
      {
        std::vector<std::string> m_layers;
        std::vector<std::string> m_styles;
        std::string              m_srs;
        BoundingBox              m_boundingBox;
        int                      m_width;
        int                      m_height;
        std::string              m_format;
        bool                     m_transparent;
        std::string              m_bgColor;
        std::string              m_time;

        WMSGetMapRequest() :
          m_srs(TE_OGC_WMS_DEFAULT_SRS),
          m_width(boost::lexical_cast<int>(TE_OGC_WMS_DEFAULT_WIDTH)),
          m_height(boost::lexical_cast<int>(TE_OGC_WMS_DEFAULT_HEIGHT)),
          m_format(TE_OGC_WMS_DEFAULT_IMAGE_FORMAT),
          m_transparent(false),
          m_bgColor(""),
          m_time("")
        {}
      };

      /*!
       * \brief The WMSGetMapResponse WMS 1.3.0 struct.
       *
       * A WMSGetMapResponse is a sctruct that is used to
       * retrieve 1.3.0 GetMap images as a buffer.
       */
      struct TEOGCWMSEXPORT WMSGetMapResponse
      {
        std::string m_buffer;
        int         m_size;
        std::string m_format;
      };

      }
    }
  }
}

#endif
