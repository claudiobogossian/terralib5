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
  \file terralib/geometry/WKBReader.h

  \brief A class that deserializes a geometry from a valid WKB.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_WKBREADER_H
#define __TERRALIB_GEOMETRY_INTERNAL_WKBREADER_H

// TerraLib
#include "../common/Enums.h"
#include "../common/Static.h"
#include "Config.h"
#include "Enums.h"

namespace te
{
  namespace gm
  {
// Forward declaration
    class Geometry;
    class GeometryCollection;
    class LinearRing;
    class LineString;
    class MultiLineString;
    class MultiPoint;
    class MultiPolygon;
    class Point;
    class PointM;
    class PointZ;
    class PointZM;
    class Polygon;
    class PolyhedralSurface;

    /*!
      \class WKBReader
      
      \brief A class that deserializes a geometry from a valid WKB.

      \ingroup geometry

      \sa WKBWriter
    */
    class TEGEOMEXPORT WKBReader : public te::common::Static
    {
      public:

        /*!
          \brief It returns a valid geometry from a given WKB.
 
          \param wkb A valid WKB geometry.

          \return A geometry created from reading the WKB. The caller of this method will take the ownership of the returned geometry.
        */
        static Geometry* read(const char* wkb);

        /*!
          \brief It returns a valid geometry from a given hex-encoded WKB.
 
          \param hwkb An hex-encoded WKB geometry.

          \return A geometry created from reading the HWKB. The caller of this method will take the ownership of the returned geometry.
        */
        static Geometry* readHex(const char* hwkb);

      private:

        static te::gm::Geometry* getGeometry(const char* wkb, const char** endptr);

        static te::gm::Point* getPoint(const char* wkb, const char** endptr);

        static te::gm::PointZ* getPointZ(const char* wkb, const char** endptr);

        static te::gm::PointM* getPointM(const char* wkb, const char** endptr);

        static te::gm::PointZM* getPointZM(const char* wkb, const char** endptr);

        static te::gm::LineString* getLineString(const char* wkb, const char** endptr);

        static te::gm::LinearRing* getLinearRing(const char* wkb, const char** endptr, te::common::MachineByteOrder byteOrder, GeomType gType);

        static te::gm::Polygon* getPolygon(const char* wkb, const char** endptr);

        static te::gm::GeometryCollection* getGeometryCollection(const char* wkb, const char** endptr);

        static te::gm::PolyhedralSurface* getPolyhedralSurface(const char* wkb, const char** endptr);
    };

  } // namespace gm
}   // namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_WKBREADER_H
