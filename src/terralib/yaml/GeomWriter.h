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
  \file terralib/yaml/GeomWriter.h

  \brief A class that converts a TerraLib Geometry to YAML.
*/

#ifndef __TERRALIB_YAML_INTERNAL_GEOMWRITER_H
#define __TERRALIB_YAML_INTERNAL_GEOMWRITER_H

// TerraLib
#include "../common/Static.h"
#include "Config.h"

#ifdef TE_USE_LIB_YAMLCPP

// yaml-cpp
#include <yaml-cpp/yaml.h>

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

  namespace yaml
  {
    /*!
      \class GeomWriter

      \brief A class that converts a TerraLib Geometry to YAML.

      This implementation is based on yaml-cpp.

      \sa GeomReader
    */
    class TEYAMLEXPORT GeomWriter : public te::common::Static
    {
      public:

        static void write(const te::gm::Geometry* g, YAML::Emitter& emitter);

        static void write(const te::gm::Point* pt, YAML::Emitter& emitter, bool onlyCoords = false);

        static void write(const te::gm::LineString* line, YAML::Emitter& emitter, bool onlyCoords = false);

        static void write(const te::gm::Polygon* poly, YAML::Emitter& emitter, bool onlyCoords = false);

        static void write(const te::gm::MultiPolygon* mpoly, YAML::Emitter& emitter);

        static void write(const te::gm::MultiLineString* mline, YAML::Emitter& emitter);

        static void write(const te::gm::MultiPoint* mpt, YAML::Emitter& emitter);

        static void write(const te::gm::GeometryCollection* geomcoll, YAML::Emitter& emitter);

      private:

        static void write(const te::gm::Coord2D* coords, std::size_t npts, YAML::Emitter& emitter);
    };

  }   // end namespace yaml
}     // end namespace te

#endif  // TE_USE_LIB_YAMLCPP

#endif  // __TERRALIB_YAML_INTERNAL_GEOMWRITER_H

