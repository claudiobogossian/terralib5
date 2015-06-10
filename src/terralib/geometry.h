/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file geometry.h

  \brief This file contains include headers for the Vector Geometry model of TerraLib.
 */

#ifndef __TERRALIB_INTERNAL_GEOMETRY_H
#define __TERRALIB_INTERNAL_GEOMETRY_H

// TerraLib
#include "geometry/AbstractPoint.h"
#include "geometry/AffineGT.h"
#include "geometry/AffineGTFactory.h"
#include "geometry/CircularString.h"
#include "geometry/CompoundCurve.h"
#include "geometry/Config.h"
#include "geometry/Coord2D.h"
#include "geometry/Curve.h"
#include "geometry/CurvePolygon.h"
#include "geometry/Enums.h"
#include "geometry/Envelope.h"
#include "geometry/Exception.h"
#include "geometry/GeometricTransformation.h"
#include "geometry/Geometry.h"
#include "geometry/GeometryCollection.h"
#include "geometry/GeometryFactory.h"
#include "geometry/GeometryProperty.h"
#include "geometry/GEOSReader.h"
#include "geometry/GEOSWriter.h"
#include "geometry/GTFactory.h"
#include "geometry/GTFilter.h"
#include "geometry/GTModelParameters.h"
#include "geometry/GTParameters.h"
#include "geometry/Line.h"
#include "geometry/LinearRing.h"
#include "geometry/LineString.h"
#include "geometry/MultiCurve.h"
#include "geometry/MultiLineString.h"
#include "geometry/MultiPoint.h"
#include "geometry/MultiPolygon.h"
#include "geometry/MultiSurface.h"
#include "geometry/Point.h"
#include "geometry/PointKd.h"
#include "geometry/PointM.h"
#include "geometry/PointZ.h"
#include "geometry/PointZM.h"
#include "geometry/Polygon.h"
#include "geometry/PolyhedralSurface.h"
#include "geometry/Surface.h"
#include "geometry/TIN.h"
#include "geometry/Triangle.h"
#include "geometry/ProjectiveGT.h"
#include "geometry/ProjectiveGTFactory.h"
#include "geometry/RSTGT.h"
#include "geometry/RSTGTFactory.h"
#include "geometry/SecondDegreePolynomialGT.h"
#include "geometry/SecondDegreePolynomialGTFactory.h"
#include "geometry/ThirdDegreePolynomialGT.h"
#include "geometry/ThirdDegreePolynomialGTFactory.h"
#include "geometry/Utils.h"
#include "geometry/Visitor.h"
#include "geometry/WKBReader.h"
#include "geometry/WKBSize.h"
#include "geometry/WKBWriter.h"
#include "geometry/WKTReader.h"
#include "geometry/WKTWriter.h"

/*!
  \defgroup geometry Geometry

  \brief This module implements the vector geometry support of TerraLib.

  The vector geometry module of TerraLib is OGC compliant.
  So, the majority documentation of this module was extracted from
  the Open Geospatial Consortium specification called
  Simple Feature Specification - Part 1: Common Architecture.
  This specification can be accessed at: http://www.opengeospatial.org/standards/sfa.

  It is important to notice that this module refers to a geometry model to be used in main memory
  and it doesn't assume any kind of persistence or data storage management.

  Basically, the geometry module is responsible for providing:

  <ul>
    <li>The primitive geometric objects like points, lines and polygons;</li>
    <li>Homogeneous and heterogeneous collections of geometries;</li>
    <li>Curved geometries;</li>
    <li>Polyhedral surfaces and Triangular Irregular Networks (TIN);</li>
    <li>Spatial operations: topological relationship tests, metric, set operations;</li>
    <li>Serialization routines for common data formats: WKB, WKT, ...</li>
  </ul>

  The main classes/concepts in this module are listed here. The namespace associated to the Geometry module is te::gm.
  To know more about it, see the te::gm namespace documentation.
*/

namespace te
{
  /*!
    \brief Namespace for the Vector Geometry module of TerraLib.
  */
  namespace gm
  {
  }	// end namespace gm
}   // end namespace te

#endif  // __TERRALIB_INTERNAL_GEOMETRY_H

