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
  \file terralib/json/rapid/GeomWriter.h

  \brief A class that converts a TerraLib Geometry to GeoJSON.
*/

#ifndef __TERRALIB_JSON_RAPID_INTERNAL_GEOMWRITER_H
#define __TERRALIB_JSON_RAPID_INTERNAL_GEOMWRITER_H

// TerraLib
#include "../../common/Static.h"
#include "../Config.h"

#ifdef TE_USE_LIB_RAPIDJSON

// RapidJSON
#include <rapidjson/document.h>

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
    namespace rapid
    {
      /*!
        \class GeomWriter

        \brief A class that converts a TerraLib Geometry to GeoJSON.

        This implementation is based on RapidJSON.

        \sa GeomReader

        \todo Como a biblioteca RapidJSON trabalha com templates seria legal tornar esta classe um template tambem para tornar o uso desta classe mais amplo.
      */
      class TEJSONEXPORT GeomWriter : public te::common::Static
      {
        public:

          static void write(const te::gm::Geometry* g, ::rapidjson::Value& jgeom, ::rapidjson::Document::AllocatorType& allocator);

          static void write(const te::gm::Point* pt, ::rapidjson::Value& jpt, ::rapidjson::Document::AllocatorType& allocator);

          static void write(const te::gm::LineString* line, ::rapidjson::Value& jline, ::rapidjson::Document::AllocatorType& allocator);

          static void write(const te::gm::Polygon* poly, ::rapidjson::Value& jpoly, ::rapidjson::Document::AllocatorType& allocator);

          static void write(const te::gm::MultiPolygon* mpoly, ::rapidjson::Value& jmpoly, ::rapidjson::Document::AllocatorType& allocator);

          static void write(const te::gm::MultiLineString* mline, ::rapidjson::Value& jmline, ::rapidjson::Document::AllocatorType& allocator);

          static void write(const te::gm::MultiPoint* mpt, ::rapidjson::Value& jmpt, ::rapidjson::Document::AllocatorType& allocator);

          static void write(const te::gm::GeometryCollection* geomcoll, ::rapidjson::Value& jgeomcoll, ::rapidjson::Document::AllocatorType& allocator);

        private:

          static void copy(const te::gm::Point* pt, ::rapidjson::Value& jpt, ::rapidjson::Document::AllocatorType& allocator);

          static void copy(const te::gm::Polygon* poly, ::rapidjson::Value& rings, ::rapidjson::Document::AllocatorType& allocator);

          static void copy(const te::gm::Coord2D* coords, std::size_t npts, ::rapidjson::Value& jcoords, ::rapidjson::Document::AllocatorType& allocator);
      };

    } // end namespace rapid
  }   // end namespace json
}     // end namespace te

#endif // TE_USE_LIB_RAPIDJSON

#endif  // __TERRALIB_JSON_RAPID_INTERNAL_GEOMWRITER_H

