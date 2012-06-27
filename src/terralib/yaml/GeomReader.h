
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
  \file terralib/yaml/GeomReader.h

  \brief A class that converts a YAML geometry to a TerraLib geometry.
*/

#ifndef __TERRALIB_YAML_INTERNAL_GEOMREADER_H
#define __TERRALIB_YAML_INTERNAL_GEOMREADER_H

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
      \class GeomReader

      \brief A class that converts a YAML geometry to a TerraLib geometry.

      This implementation is based on yaml-cpp.

      \sa GeomWriter
    */
    class TEYAMLEXPORT GeomReader : public te::common::Static
    {
      public:

        static te::gm::Geometry* read(const YAML::Node& ygeom);

      private:

        static te::gm::Point* readPoint(const YAML::Node& ypt);

        static te::gm::LineString* readLineString(const YAML::Node& yline);

        static void readLineString(te::gm::LineString* line, const YAML::Node& yline);

        static te::gm::Polygon* readPolygon(const YAML::Node& ypoly);

        static te::gm::MultiPolygon* readMPolygon(const YAML::Node& ympoly);

        static te::gm::MultiLineString* readMLineString(const YAML::Node& ymline);

        static te::gm::MultiPoint* readMPoint(const YAML::Node& ympt);

        static te::gm::GeometryCollection* readGeomCollection(const YAML::Node& ygeomcoll);
    };

  }   // end namespace yaml
}     // end namespace te

#endif  // TE_USE_LIB_YAMLCPP

#endif  // __TERRALIB_YAML_INTERNAL_GEOMREADER_H
