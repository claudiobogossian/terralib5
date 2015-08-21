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
  \file terralib/geometry/Enums.h

  \brief Enumerations related to Geometry module.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_ENUMS_H
#define __TERRALIB_GEOMETRY_INTERNAL_ENUMS_H

namespace te
{
  namespace gm
  {
    /*!
      \enum GeomType

      \brief Each enumerated type is compatible with a Well-known Binary (WKB) type code.

      These are all WKB geometry types that TerraLib knows how to
      encode or decode. The values follows OGC Simple Feature Specification (SFS).
    */
    enum GeomType
    {
        GeometryType   =    0,              /*!< Geometry is not instantiable but this is the general type for geometries in R2 (x, y).        */
        GeometryZType  = 1000,              /*!< Geometry is not instantiable but this is the general type for geometries in R3 (x, y, z).     */
        GeometryMType  = 2000,              /*!< Geometry is not instantiable but this is the general type for geometries in R3 (x, y, m).     */
        GeometryZMType = 3000,              /*!< Geometry is not instantiable but this is the general type for geometries in R4 (x, y, z, m).  */

        PointType   =    1,                 /*!< A point in R2 with coordinate values for x and y.       */
        PointZType  = 1001,                 /*!< A point in R3 with coordinate values for x, y and z.    */
        PointMType  = 2001,                 /*!< A point in R3 with coordinate values for x, y and m.    */
        PointZMType = 3001,                 /*!< A point in R4 with coordinate values for x, y, z and m. */
        PointKdType = 0xFFFFFFFD,           /*!< A point in Rn with k coordinate values.                 */

        LineStringType   =    2,            /*!< A LineString in R2 with coordinate values for x and y.       */
        LineStringZType  = 1002,            /*!< A LineString in R3 with coordinate values for x, y and z.    */
        LineStringMType  = 2002,            /*!< A LineString in R3 with coordinate values for x, y and m.    */
        LineStringZMType = 3002,            /*!< A LineString in R4 with coordinate values for x, y, z and m. */

        CircularStringType   =    8,        /*!< A CircularString in R2 with coordinate values for x and y.       */
        CircularStringZType  = 1008,        /*!< A CircularString in R3 with coordinate values for x, y and z.    */
        CircularStringMType  = 2008,        /*!< A CircularString in R3 with coordinate values for x, y and m.    */
        CircularStringZMType = 3008,        /*!< A CircularString in R4 with coordinate values for x, y, z and m. */

        CompoundCurveType   =    9,        /*!< A CompoundCurve in R2 with coordinate values for x and y.       */
        CompoundCurveZType  = 1009,        /*!< A CompoundCurve in R3 with coordinate values for x, y and z.    */
        CompoundCurveMType  = 2009,        /*!< A CompoundCurve in R3 with coordinate values for x, y and m.    */
        CompoundCurveZMType = 3009,        /*!< A CompoundCurve in R4 with coordinate values for x, y, z and m. */

        PolygonType   =    3,               /*!< A Polygon in R2 with coordinate values for x and y.       */
        PolygonZType  = 1003,               /*!< A Polygon in R3 with coordinate values for x, y and z.    */
        PolygonMType  = 2003,               /*!< A Polygon in R3 with coordinate values for x, y and m.    */
        PolygonZMType = 3003,               /*!< A Polygon in R4 with coordinate values for x, y, z and m. */

        CurvePolygonType   =    10,         /*!< A CurvePolygon in R2 with coordinate values for x and y.       */
        CurvePolygonZType  = 1010,          /*!< A CurvePolygon in R3 with coordinate values for x, y and z.    */
        CurvePolygonMType  = 2010,          /*!< A CurvePolygon in R3 with coordinate values for x, y and m.    */
        CurvePolygonZMType = 3010,          /*!< A CurvePolygon in R4 with coordinate values for x, y, z and m. */

        GeometryCollectionType   =    7,    /*!< A GeometryCollection in R2 with coordinate values for x and y.       */
        GeometryCollectionZType  = 1007,    /*!< A GeometryCollection in R3 with coordinate values for x, y and z.    */
        GeometryCollectionMType  = 2007,    /*!< A GeometryCollection in R3 with coordinate values for x, y and m.    */
        GeometryCollectionZMType = 3007,    /*!< A GeometryCollection in R4 with coordinate values for x, y, z and m. */

        MultiPointType   =    4,            /*!< A MultiPoint in R2 with coordinate values for x and y.       */
        MultiPointZType  = 1004,            /*!< A MultiPoint in R3 with coordinate values for x, y and z.    */
        MultiPointMType  = 2004,            /*!< A MultiPoint in R3 with coordinate values for x, y and m.    */
        MultiPointZMType = 3004,            /*!< A MultiPoint in R4 with coordinate values for x, y, z and m. */

        MultiLineStringType   =    5,       /*!< A MultiLineString in R2 with coordinate values for x and y.       */
        MultiLineStringZType  = 1005,       /*!< A MultiLineString in R3 with coordinate values for x, y and z.    */
        MultiLineStringMType  = 2005,       /*!< A MultiLineString in R3 with coordinate values for x, y and m.    */
        MultiLineStringZMType = 3005,       /*!< A MultiLineString in R4 with coordinate values for x, y, z and m. */

        MultiPolygonType   =    6,          /*!< A MultiPolygon in R2 with coordinate values for x and y.       */
        MultiPolygonZType  = 1006,          /*!< A MultiPolygon in R3 with coordinate values for x, y and z.    */
        MultiPolygonMType  = 2006,          /*!< A MultiPolygon in R3 with coordinate values for x, y and m.    */
        MultiPolygonZMType = 3006,          /*!< A MultiPolygon in R4 with coordinate values for x, y, z and m. */

        PolyhedralSurfaceType   =   15,     /*!< A PolyhedralSurface in R2 with coordinate values for x and y.       */
        PolyhedralSurfaceZType  = 1015,     /*!< A PolyhedralSurface in R3 with coordinate values for x, y and z.    */
        PolyhedralSurfaceMType  = 2015,     /*!< A PolyhedralSurface in R3 with coordinate values for x, y and m.    */
        PolyhedralSurfaceZMType = 3015,     /*!< A PolyhedralSurface in R4 with coordinate values for x, y, z and m. */

        TINType   =   16,                   /*!< A TIN in R2 with coordinate values for x and y.       */
        TINZType  = 1016,                   /*!< A TIN in R3 with coordinate values for x, y and z.    */
        TINMType  = 2016,                   /*!< A TIN in R3 with coordinate values for x, y and m.    */
        TINZMType = 3016,                   /*!< A TIN in R4 with coordinate values for x, y, z and m. */

        TriangleType   =   17,              /*!< A Triangle in R2 with coordinate values for x and y.       */
        TriangleZType  = 1017,              /*!< A Triangle in R3 with coordinate values for x, y and z.    */
        TriangleMType  = 2017,              /*!< A Triangle in R3 with coordinate values for x, y and m.    */
        TriangleZMType = 3017,              /*!< A Triangle in R4 with coordinate values for x, y, z and m. */

        MultiSurfaceType = 12,               /*!< A MultiSurface in R2 with coordinate values for x and y.       */
        MultiSurfaceZType = 1012,       /*!< A MultiSurface in R3 with coordinate values for x, y and z.    */
        MultiSurfaceMType = 2012,       /*!< A MultiSurface in R3 with coordinate values for x, y and m.    */
        MultiSurfaceZMType = 3012,      /*!< A MultiSurface in R4 with coordinate values for x, y, z and m. */

        UnknownGeometryType = 0xFFFFFFFF    /*!< Just a marker for an unknown geometry type. */
    };

    /*!
      \enum SpatialRelation

      \brief Spatial relations between geometric objects.
    */
    enum SpatialRelation
    {  
      UNKNOWN_SPATIAL_RELATION = 0, /*!< */
      INTERSECTS = 1,               /*!< */
      DISJOINT = 2,                 /*!< */
      TOUCHES = 4,                  /*!< */
      OVERLAPS = 8,                 /*!< */
      CROSSES = 16,                 /*!< */
      WITHIN = 32,                  /*!< */
      CONTAINS = 64,                /*!< */
      COVERS = 128,                 /*!< */
      COVEREDBY = 256,              /*!< */
      EQUALS = 512                  /*!< */
    };

    /*!
      \enum Dimensionality

      \brief From Wikipedia: "in mathematics, the dimension of an object is an intrinsic property, independent of the space in which the object may happen to be embedded".
    */
    enum Dimensionality
    {  
      P = 0, /*!< Points are 0-dimensional.   */
      L = 1, /*!< Lines are 1-dimensional.    */
      A = 2  /*!< Polygons are 2-dimenional.  */
    };

    /*!
      \enum BufferCapStyle
      
      \brief Buffer end cap style.

      The end cap style specifies the buffer geometry that will be
      created at the ends of LineStrings.
    */
    enum BufferCapStyle
    {
      CapRoundType, /*!< A semi-circle (default).                         */
      CapButtType,  /*!< A straight line perpendicular to the end segment */
      CapSquareType /*!< A half-square                                    */
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_ENUMS_H

