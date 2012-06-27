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
  \file terralib/json/cajun/GeomReader.h

  \brief A class that converts a GeoJSON geometry to a TerraLib geometry.
*/

#ifndef __TERRALIB_JSON_CAJUN_INTERNAL_GEOMREADER_H
#define __TERRALIB_JSON_CAJUN_INTERNAL_GEOMREADER_H

// TerraLib
#include "../../common/Static.h"
#include "../Config.h"

#ifdef TE_USE_LIB_CAJUN

// CAJUN
#include <cajun/elements.h>

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
    namespace cajun
    {
      /*!
        \class GeomReader

        \brief A class that converts a GeoJSON geometry to a TerraLib geometry.

        This implementation is based on cajun.

        \sa GeomWriter
      */
      class TEJSONEXPORT GeomReader : public te::common::Static
      {
        public:

          static te::gm::Geometry* read(const ::json::Object& jgeom);

        private:

          static te::gm::Point* readPoint(const ::json::Array& jpt);

          static te::gm::LineString* readLineString(const ::json::Array& jline);

          static void readLineString(te::gm::LineString* line, const ::json::Array& jline);

          static te::gm::Polygon* readPolygon(const ::json::Array& jpoly);

          static te::gm::MultiPolygon* readMPolygon(const ::json::Array& jmpoly);

          static te::gm::MultiLineString* readMLineString(const ::json::Array& jmline);

          static te::gm::MultiPoint* readMPoint(const ::json::Array& jmpt);

          static te::gm::GeometryCollection* readGeomCollection(const ::json::Array& jgeomcoll);
      };

    } // end namespace cajun
  }   // end namespace json
}     // end namespace te

#endif  // TE_USE_LIB_CAJUN

#endif  // __TERRALIB_JSON_CAJUN_INTERNAL_GEOMREADER_H
