/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file geometry_fw.h

  \brief This file contains forward declarations for the Vector Geometry model of TerraLib.
 */

#ifndef __TERRALIB_FW_INTERNAL_GEOMETRY_FW_H
#define __TERRALIB_FW_INTERNAL_GEOMETRY_FW_H

namespace te
{
  namespace gm
  {    
    class AbstractPoint;
    class AffineGT;
    class AffineGTFactory;
    class CircularString;
    class CompoundCurve;
    class Curve;
    class CurvePolygon;
    class Envelope;
    class GeometricTransformation;
    class Geometry;
    class GeometryCollection;
    class GeometryFactory;
    class GeometryProperty;
    class GTFactory;
    class GTFilter;
    class GTModelParameters;
    class GTParameters;
    class LinearRing;
    class LineString;
    class MultiCurve;
    class MultiLineString;
    class MultiPoint;
    class MultiPolygon;
    class MultiSurface;
    class Platform;
    class Point;
    class PointM;
    class PointZ;
    class PointZM;
    class PointKd;
    class Polygon;
    class PolyhedralSurface;
    class Surface;
    class TIN;
    class Triangle;
    class ProjectiveGT;
    class ProjectiveGTFactory;
    class RSTGT;
    class RSTGTFactory;
    class SecondDegreePolynomialGT;
    class SecondDegreePolynomialGTFactory;
    class ThirdDegreePolynomialGT;
    class ThirdDegreePolynomialGTFactory;
    class WKBReader;
    class WKBSize;
    class WKBWriter;
    class WKTReader;
    class WKTWriter;

    struct Coord2D;

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_FW_INTERNAL_GEOMETRY_FW_H

