/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/geometry/GEOSWriter.h

  \brief A class that converts a TerraLib geometry to a GEOS geometry.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_GEOSWRITER_H
#define __TERRALIB_GEOMETRY_INTERNAL_GEOSWRITER_H

// TerraLib
#include "../common/Static.h"
#include "Config.h"
#include "terralib_config.h"

#ifdef TERRALIB_GEOS_ENABLED

// STL
#include <vector>

// Forward declaration
namespace geos
{
  namespace geom
  {
    class CoordinateSequence;
    class Geometry;
    class GeometryCollection;
    class LinearRing;
    class LineString;
    class MultiLineString;
    class MultiPoint;
    class MultiPolygon;
    class Point;
    class Polygon;
  } // end namespace geom
}   // end namespace geos

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

    /*!
      \class GEOSWriter

      \brief A class that converts a TerraLib geometry to a GEOS geometry.

      \sa GEOSReader
    */
    class TEGEOMEXPORT GEOSWriter : public te::common::Static
    {
      public:

        /*!
          \brief It reads a TerraLib geometry and make a GEOS geometry.

          \param teGeom The TerraLib geometry to be used to create the GEOS geometry.

          \return A GEOS geometry. The client will take its ownership.

          \exception It throws an exception if a conversion is not possible.
        */
        static geos::geom::Geometry* write(const Geometry* teGeom);

        /*!
          \brief It converts the TerraLib point to a GEOS point.

          \param tePt The TerraLib point to be used to create the GEOS point.

          \return A GEOS point geometry. The client will take it ownership.

          \exception It throws an exception if a conversion is not possible.
        */
        static geos::geom::Point* write(const Point* tePt);

        /*!
          \brief It converts the TerraLib point to a GEOS point.

          \param tePt The TerraLib point to be used to create the GEOS point.

          \return A GEOS point geometry. The client will take it ownership.

          \exception It throws an exception if a conversion is not possible.
        */
        static geos::geom::Point* write(const PointM* tePt);

        /*!
          \brief It converts the TerraLib point to a GEOS point.

          \param tePt The TerraLib point to be used to create the GEOS point.

          \return A GEOS point geometry. The client will take it ownership.

          \exception It throws an exception if a conversion is not possible.
        */
        static geos::geom::Point* write(const PointZ* tePt);

        /*!
          \brief It converts the TerraLib point to a GEOS point.

          \param tePt The TerraLib point to be used to create the GEOS point.

          \return A GEOS point geometry. The client will take it ownership.

          \exception It throws an exception if a conversion is not possible.
        */
        static geos::geom::Point* write(const PointZM* tePt);

        /*!
          \brief It converts the TerraLib line string to a GEOS line string.

          \param teLine The TerraLib line string to be used to create the GEOS line string.

          \return A GEOS line string geometry. The client will take its ownership.

          \exception It throws an exception if a conversion is not possible.
        */
        static geos::geom::LineString* write(const LineString* teLine);

        /*!
          \brief It converts the TerraLib linear ring to a GEOS linear ring.

          \param teRing The TerraLib linear ring to be used to create the GEOS linear ring.

          \return A GEOS linear ring geometry. The client will take its ownership.

          \exception It throws an exception if a conversion is not possible.
        */
        static geos::geom::LinearRing* write(const LinearRing* teRing);

        /*!
          \brief It converts the TerraLib polygon to a GEOS polygon.

          \param tePoly The TerraLib polygon to be used to create the GEOS polygon.

          \return A GEOS polygon geometry. The client will take its ownership.

          \exception It throws an exception if a conversion is not possible.
        */
        static geos::geom::Polygon* write(const Polygon* tePoly);

        /*!
          \brief It converts the TerraLib multi polygon to a GEOS multi polygon.

          \param teMPoly The TerraLib multi polygon to be used to create the GEOS multi polygon.

          \return A GEOS multi polygon geometry. The client will take its ownership.

          \exception It throws an exception if a conversion is not possible.
        */
        static geos::geom::MultiPolygon* write(const MultiPolygon* teMPoly);

        /*!
          \brief It converts the TerraLib multi line string to a GEOS multi line string.

          \param teMLine The TerraLib multi line string to be used to create the GEOS multi line string.

          \return A GEOS multi line string geometry. The client will take its ownership.

          \exception It throws an exception if a conversion is not possible.
        */
        static geos::geom::MultiLineString* write(const MultiLineString* teMLine);

        /*!
          \brief It converts the TerraLib multi point to a GEOS multi point.

          \param teMPt The TerraLib multi point to be used to create the GEOS multi point.

          \return A GEOS multi point geometry. The client will take its ownership.

          \exception It throws an exception if a conversion is not possible.
        */
        static geos::geom::MultiPoint* write(const MultiPoint* teMPt);

        /*!
          \brief It converts the TerraLib geometry collection to a GEOS geometry collection.

          \param teGeomColl The TerraLib geometry collection to be used to create the GEOS geometry collection.

          \return A GEOS geometry collection. The client will take its ownership.

          \exception It throws an exception if a conversion is not possible.
        */
        static geos::geom::GeometryCollection* write(const GeometryCollection* teGeomColl);

      protected:

        /*!
          \brief It creates a coordinate sequence from the input TerraLib line string (or TerraLib  linear ring).

          \param teLine The TerraLib line string to be used to create the GEOS coordinate sequence.

          \return A GEOS coordinate sequence. The caller will take its ownership.
        */
        static geos::geom::CoordinateSequence* getCoordinateSequence(const LineString* teLine);

        /*!
          \brief It creates a vector of GEOS geometry from the input TerraLib geometry collection.

          \param teGeomColl The TerraLib line string to be used to create the GEOS coordinate sequence.

          \return A vector of GEOS geometry. The caller will take its ownership.
        */
        static std::vector<geos::geom::Geometry*>* getGeometries(const GeometryCollection* teGeomColl);
    };
    
  } // end namespace gm
}   // end namespace te

#endif  // TERRALIB_GEOS_ENABLED

#endif  // __TERRALIB_GEOMETRY_INTERNAL_GEOSWRITER_H



