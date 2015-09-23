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
  \file terralib/geometry/GeometryFactory.h

  \brief This is the Geometry factory for TerraLib geometries.  
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_GEOMETRYFACTORY_H
#define __TERRALIB_GEOMETRY_INTERNAL_GEOMETRYFACTORY_H

// TerraLib
#include "../common/Static.h"
#include "Geometry.h"
#include "GeometryCollection.h"
#include "LineString.h"
#include "MultiLineString.h"
#include "MultiPoint.h"
#include "MultiPolygon.h"
#include "Point.h"
#include "PointM.h"
#include "PointZ.h"
#include "PointZM.h"
#include "Polygon.h"
#include "PolyhedralSurface.h"
#include "TIN.h"
#include "Triangle.h"

namespace te
{
  namespace gm
  {
    /*!
      \class GeometryFactory

      \brief This is the Geometry factory for TerraLib geometries.

      \ingroup geometry

      \sa Geometry,
          Point, PointM, PointZ, PointZM,
          Curve, LineString, LinearRing,
          Surface, Polygon, Triangle, PolyhedralSurface, TIN,
          GeometryCollection, MultiCurve, MultiSurface,
          MultiPoint, MultiLineString, MultiPolygon     
    */
    class TEGEOMEXPORT GeometryFactory : public te::common::Static
    {
      public:

        /*!
          \brief It returns an instance according to the informed geometry type.

          \param t    The geometry type to be instantiable.
          \param srid The geometry spatial reference system.

          \return A geometry object.
        */
        static Geometry* make(GeomType t, int srid);
    };

    inline Geometry* GeometryFactory::make(GeomType t, int srid)
    {
      switch(t)
      {
        case PointType:
          return new Point(srid, 0);

        case PointZType:
          return new PointZ(srid, 0);

        case PointMType:
          return new PointM(srid, 0);

        case PointZMType:
          return new PointZM(srid, 0);

        case LineStringType:
        case LineStringZType:
        case LineStringMType:
        case LineStringZMType:
          return new LineString(t, srid, 0);

        case PolygonType:
        case PolygonZType:
        case PolygonMType:
        case PolygonZMType:
          return new Polygon(0, t, srid, 0);

        case MultiPointType:
        case MultiPointZType:
        case MultiPointMType:
        case MultiPointZMType:
          return new MultiPoint(0, t, srid, 0);

        case MultiLineStringType:
        case MultiLineStringZType:
        case MultiLineStringMType:
        case MultiLineStringZMType:
          return new MultiLineString(0, t, srid, 0);

        case MultiPolygonType:
        case MultiPolygonZType:
        case MultiPolygonMType:
        case MultiPolygonZMType:
          return new MultiPolygon(0, t, srid, 0);

        case MultiSurfaceType:
        case MultiSurfaceZType:
        case MultiSurfaceMType:
        case MultiSurfaceZMType:
          return new MultiSurface(0, t, srid, 0);

        case GeometryCollectionType:
        case GeometryCollectionZType:
        case GeometryCollectionMType:
        case GeometryCollectionZMType:
          return new GeometryCollection(0, t, srid, 0);

        case TriangleType:
        case TriangleZType:
        case TriangleMType:
        case TriangleZMType:
          return new Triangle(t, srid, 0);

        case TINType:
        case TINZType:
        case TINMType:
        case TINZMType:
          return new TIN(0, t, srid, 0);

        case PolyhedralSurfaceType:
        case PolyhedralSurfaceZType:
        case PolyhedralSurfaceMType:
        case PolyhedralSurfaceZMType:
          return new PolyhedralSurface(0, t, srid, 0);

        default:
          return 0;
      }
    }

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_GEOMETRYFACTORY_H

