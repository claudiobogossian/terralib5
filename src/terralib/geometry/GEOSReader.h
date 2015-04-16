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
  \file terralib/geometry/GEOSReader.h

  \brief A class that converts a GEOS geometry to a TerraLib geometry.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_GEOSREADER_H
#define __TERRALIB_GEOMETRY_INTERNAL_GEOSREADER_H

// TerraLib
#include "terralib_config.h"
#include "../common/Static.h"
#include "Config.h"

#ifdef TERRALIB_GEOS_ENABLED

// Forward declaration
namespace geos
{
  namespace geom
  {
    //class CoordinateSequence;
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
    class Polygon;    

    /*!
      \class GEOSReader

      \brief A class that converts a GEOS geometry to a TerraLib geometry.

      \sa GEOSWriter
    */
    class TEGEOMEXPORT GEOSReader : public te::common::Static
    {
      public:

        /*!
          \brief It reads a GEOS geometry and make a TerraLib geometry.

          \param geosGeom The GEOS geometry to be used to create the TerraLib geometry.

          \return A TerraLib geometry. The client will take its ownership.

          \exception It throws an exception if a conversion is not possible.
        */
        static Geometry* read(const geos::geom::Geometry* geosGeom);

        /*!
          \brief It converts the GEOS point to a TerraLib point.

          \param geosPt The GEOS point to be used to create the TerraLib point.

          \return A TerraLib point geometry. The client will take its ownership.

          \exception It throws an exception if a conversion is not possible.
        */
        static Point* read(const geos::geom::Point* geosPt);

        /*!
          \brief It converts the GEOS line string to a TerraLib line string.

          \param geosLine The GEOS line string to be used to create the TerraLib line string.

          \return A TerraLib line string geometry. The client will take its ownership.

          \exception It throws an exception if a conversion is not possible.
        */
        static LineString* read(const geos::geom::LineString* geosLine);

        /*!
          \brief It converts the GEOS linear ring to a TerraLib linear ring.

          \param geosRing The GEOS linear ring to be used to create the TerraLib linear ring.

          \return A TerraLib linear ring geometry. The client will take its ownership.

          \exception It throws an exception if a conversion is not possible.
        */
        static LinearRing* read(const geos::geom::LinearRing* geosRing);

        /*!
          \brief It converts the GEOS polygon to a TerraLib polygon.

          \param geosPoly The GEOS polygon to be used to create the TerraLib polygon.

          \return A TerraLib polygon geometry. The client will take its ownership.

          \exception It throws an exception if a conversion is not possible.
        */
        static Polygon* read(const geos::geom::Polygon* geosPoly);

        /*!
          \brief It converts the GEOS multi polygon to a TerraLib multi polygon.

          \param geosMPoly The GEOS multi polygon to be used to create the TerraLib multi polygon.

          \return A TerraLib multi polygon geometry. The client will take its ownership.

          \exception It throws an exception if a conversion is not possible.
        */
        static MultiPolygon* read(const geos::geom::MultiPolygon* geosMPoly);

        /*!
          \brief It converts the GEOS multi line string to a TerraLib multi line string.

          \param geosMLine The GEOS multi line string to be used to create the TerraLib multi line string.

          \return A TerraLib multi line string geometry. The client will take its ownership.

          \exception It throws an exception if a conversion is not possible.
        */
        static MultiLineString* read(const geos::geom::MultiLineString* geosMLine);

        /*!
          \brief It converts the GEOS multi point to a TerraLib multi point.

          \param geosMPt The GEOS multi point to be used to create the TerraLib multi point.

          \return A TerraLib multi point geometry. The client will take its ownership.

          \exception It throws an exception if a conversion is not possible.
        */
        static MultiPoint* read(const geos::geom::MultiPoint* geosMPt);

        /*!
          \brief It converts the GEOS geometry collection to a TerraLib geometry collection.

          \param geosGeomColl The GEOS geometry collection to be used to create the TerraLib geometry collection.

          \return A TerraLib geometry collection. The client will take its ownership.

          \exception It throws an exception if a conversion is not possible.
        */
        static GeometryCollection* read(const geos::geom::GeometryCollection* geosGeomColl);

      //protected:

        /*!
          \brief It sets the TerraLib line string coordinates using a GEOS coordinate sequence.

          \param teLine The TerraLib line string to be used to create the GEOS coordinate sequence.
          \param cs     A GEOS coordinate sequence to be used to extract the TerraLib line string coordinates.
          \param nPts   The number of coordinates to be extracted from the coordinate sequence.

          \note The TerraLib line string must have the same capacity (number of vertex) as the input GEOS coordinate sequence.
        */
        //static void setCoords(const LineString* teLine, geos::geom::CoordinateSequence* cs, std::size_t nPts) const;
    };
    
  } // end namespace gm
}   // end namespace te

#endif  // TERRALIB_GEOS_ENABLED

#endif  // __TERRALIB_GEOMETRY_INTERNAL_GEOSREADER_H



