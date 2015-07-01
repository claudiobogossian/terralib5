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
  \file terralib/terralib4/GeomReader.h
   
  \brief An utility class for converting a TerraLib 4.x geometry to a TerraLib 5.
*/

#ifndef __TERRALIB_TERRALIB4_INTERNAL_GEOMREADER_H
#define __TERRALIB_TERRALIB4_INTERNAL_GEOMREADER_H

// TerraLib 5
#include "../common/Static.h"
#include "../geometry/Enums.h"
#include "Config.h"

// STL
#include <memory>

// Forward declarations
#include "../geometry_fw.h"

class TeCell;
class TeGeometry;
class TeLine2D;
class TeLinearRing;
class TeLineSet;
class TePoint;
class TePolygon;
class TePolygonSet;
class TePointSet;

namespace terralib4
{
  /*!
    \class EWKBReader

    \brief An utility class for reading a TerraLib 4.x geometry.
  */
  class GeomReader : public te::common::Static
  {
    public:

      static std::auto_ptr<te::gm::Point> getPoint(const TePoint& pt);

      static std::auto_ptr<te::gm::LineString> getLineString(const TeLine2D& line);

      static std::auto_ptr<te::gm::LinearRing> getLinearRing(const TeLinearRing& ring);

      static std::auto_ptr<te::gm::Polygon> getPolygon(const TePolygon& poly);

      static std::auto_ptr<te::gm::MultiPolygon> getMultiPolygon(const TePolygonSet& polySet);

      static std::auto_ptr<te::gm::MultiLineString> getMultiLineString(const TeLineSet& lineSet);

      static std::auto_ptr<te::gm::MultiPoint> getMultiPoint(const TePointSet& pointSet);

      static std::auto_ptr<te::gm::Polygon> getPolygon(const TeCell& cell);

      static std::auto_ptr<te::gm::Geometry> getGeometry(const TeGeometry& geom);
  };
}  // end namespace terralib4

#endif  // __TERRALIB_TERRALIB4_INTERNAL_GEOMREADER_H
