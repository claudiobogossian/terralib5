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
  \file terralib/json/cajun/GeomWriter.h

  \brief A class that converts a TerraLib Geometry to GeoJSON.
*/

#ifndef __TERRALIB_JSON_CAJUN_INTERNAL_GEOMWRITER_H
#define __TERRALIB_JSON_CAJUN_INTERNAL_GEOMWRITER_H

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
        \class GeomWriter

        \brief A class that converts a TerraLib Geometry to GeoJSON.

        This implementation is based on CAJUN.

        \sa GeomReader
      */
      class TEJSONEXPORT GeomWriter : public te::common::Static
      {
        public:

          static void write(const te::gm::Geometry* g, ::json::Object& jgeom);

          static void write(const te::gm::Point* pt, ::json::Object& jpt);

          static void write(const te::gm::LineString* line, ::json::Object& jline);

          static void write(const te::gm::Polygon* poly, ::json::Object& jpoly);

          static void write(const te::gm::MultiPolygon* mpoly, ::json::Object& jmpoly);

          static void write(const te::gm::MultiLineString* mline, ::json::Object& jmline);

          static void write(const te::gm::MultiPoint* mpt, ::json::Object& jmpt);

          static void write(const te::gm::GeometryCollection* geomcoll, ::json::Object& jgeomcoll);

        private:

          static void write(const te::gm::Point* pt, ::json::Array& jpt);

          static void write(const te::gm::Polygon* poly, ::json::Array& rings);

          static void write(const te::gm::Coord2D* coords, std::size_t npts, ::json::Array& jcoords);
      };

    }   // end namespace cajun
  }     // end namespace json
}       // end namespace te

#endif // TE_USE_LIB_CAJUN

#endif  // __TERRALIB_JSON_CAJUN_INTERNAL_GEOMWRITER_H

