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
  \file terralib/ws/ogc/wcs/client/DataTypes.h

  \brief Specifies containers for WCS data types.

  \author Emerson Moraes
*/

#ifndef __TERRALIB_WS_OGC_WCS_DATATYPES_H
#define __TERRALIB_WS_OGC_WCS_DATATYPES_H

namespace te
{
  namespace ws
  {
    namespace ogc
    {
      struct Capabilities
      {
        std::vector< std::string > operations;
        std::vector< std::string > coverages;
      };

      struct SubSet
      {
        std::string axis;
        std::string min;
        std::string max;
      };

      struct DomainSet
      {
        std::vector< SubSet > subSet;
      };

      struct EnvelopeWithTimePeriod
      {
        std::string srsName;
        std::string srsDimension;
        std::vector< std::string > uomLabels;

        std::string firstLabel;
        std::string secondLabel;

        std::string lowerCorner_X;
        std::string lowerCorner_Y;

        std::string upperCorner_X;
        std::string upperCorner_Y;

        std::string timeLabel;
        std::string beginPosition;
        std::string endPosition;
      };

      struct ServiceParameters
      {
        std::string coverageSubtype;
        std::string coverageSubtypeParent;
        std::string nativeFormat;
        std::string extension;
      };

      struct CoverageDescription
      {
        std::string coverageId;
        struct EnvelopeWithTimePeriod envelope;
        struct DomainSet domainSet;
        struct ServiceParameters serviceParameters;
        std::vector< std::string > fieldNames;
        std::vector< std::string > extension;
      };


      /*! \brief A struct to set the parameters of requested coverage */
      struct CoverageRequest
      {
        std::string coverageID;
        std::string format;
        std::string mediaType;
        EnvelopeWithTimePeriod envelope;
        std::string time;
        std::vector< SubSet > subSet;
        std::map< std::string, std::string > additionalParameters;
      };
    }
  }
}

#endif //__TERRALIB_WS_OGC_WCS_DATATYPES_H
