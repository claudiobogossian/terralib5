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
  \file terralib/json/cpp/GeomReader.h

  \brief A class that converts a GeoJSON geometry to a TerraLib geometry.
*/

#ifndef __TERRALIB_JSON_CPP_INTERNAL_GEOMREADER_H
#define __TERRALIB_JSON_CPP_INTERNAL_GEOMREADER_H

// TerraLib
#include "../../common/Static.h"
#include "../Config.h"

#ifdef TE_USE_LIB_JSONCPP

// JSONCpp
#include <json/forwards.h>

namespace te
{
// Forward declaration
  namespace gm
  {
    class Geometry;
    class GeometryCollection;
    class LinearRing;
    class LineString;
    class MultiLineString;
    class MultiPoint;
    class MultiPolygon;
    class Point;
    class Polygon;
  }

  namespace json
  {
    namespace cpp
    {
      /*!
        \class GeomReader

        \brief A class that converts a GeoJSON geometry to a TerraLib geometry.

        This implementation is based on cppjson.

        \sa GeomWriter
      */
      class TEJSONEXPORT GeomReader : public te::common::Static
      {
        public:

          static te::gm::Geometry* read(const Json::Value& jgeom);

        private:

          static te::gm::Point* readPoint(const Json::Value& jpt);

          static te::gm::LineString* readLineString(const Json::Value& jline);

          static void readLineString(te::gm::LineString* line, const Json::Value& jline);

          static te::gm::Polygon* readPolygon(const Json::Value& jpoly);

          static te::gm::MultiPolygon* readMPolygon(const Json::Value& jmpoly);

          static te::gm::MultiLineString* readMLineString(const Json::Value& jmline);

          static te::gm::MultiPoint* readMPoint(const Json::Value& jmpt);

          static te::gm::GeometryCollection* readGeomCollection(const Json::Value& jgeomcoll);
      };

    } // end namespace cpp
  }   // end namespace json
}     // end namespace te

#endif  // TE_USE_LIB_JSONCPP

#endif  // __TERRALIB_JSON_CPP_INTERNAL_GEOMREADER_H
