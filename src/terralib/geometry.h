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
  \file geometry.h

  \brief This file contains include headers for the Vector Geometry model of TerraLib.
 */

#ifndef __TERRALIB_INTERNAL_GEOMETRY_H
#define __TERRALIB_INTERNAL_GEOMETRY_H

// TerraLib
#include "geometry/AbstractPoint.h"
#include "geometry/CircularString.h"
#include "geometry/CompoundCurve.h"
#include "geometry/Config.h"
#include "geometry/Coord2D.h"
#include "geometry/Curve.h"
#include "geometry/CurvePolygon.h"
#include "geometry/Enums.h"
#include "geometry/Envelope.h"
#include "geometry/Exception.h"
#include "geometry/Geometry.h"
#include "geometry/GeometryCollection.h"
#include "geometry/GeometryFactory.h"
#include "geometry/GeometryProperty.h"
#include "geometry/GEOSReader.h"
#include "geometry/GEOSWriter.h"
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
#include "geometry/Utils.h"
#include "geometry/Visitor.h"
#include "geometry/WKBReader.h"
#include "geometry/WKBSize.h"
#include "geometry/WKBWriter.h"
#include "geometry/WKTReader.h"
#include "geometry/WKTWriter.h"

namespace te
{
  /*!
    \brief Namespace for the Vector Geometry module of TerraLib.

    The Vector Geometry module of TerraLib is OGC compliant.
    So, the majority documentation of this module was extracted from
    the Open Geospatial Consortium specification called
    Simple Feature Specification - Part 1: Common Architecture.
    This specification can be accessed at: http://www.opengeospatial.org/standards/sfa.
  */
  namespace gm
  {
  }	// end namespace gm
}   // end namespace te

#endif  // __TERRALIB_INTERNAL_GEOMETRY_H

