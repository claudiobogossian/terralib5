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
  \file terralib/geometry/FixGeometries.h

  \brief This class contains attributes and functions necessary to fix and report unconsistencies of geometries.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_FIXGEOMETRIES_H
#define __TERRALIB_GEOMETRY_INTERNAL_FIXGEOMETRIES_H

// TerraLib
#include "Config.h"
#include "Enums.h"

namespace te
{
  namespace gm
  {
// Forward declarations
    class Envelope;
    class Geometry;
    class Point;
    struct Coord2D;
    class LineString;
    
    TEGEOMEXPORT Geometry* FixGeometry(te::gm::Geometry* inputGeom);

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_FIXGEOMETRIES_H

