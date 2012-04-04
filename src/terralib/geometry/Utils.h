/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/geometry/Utils.h

  \brief Utility functions for the Geometry Module.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_GEOMUTILS_H
#define __TERRALIB_GEOMETRY_INTERNAL_GEOMUTILS_H

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
    
    /*!
      \brief It returns the number of measurements or axes needed to describe a position in a coordinate system.

      It returns:
      <ul>
      <li>2 for a coordinate with x, y;</li>
      <li>3 for a coordinate with x, y and z or x, y and m;</li>
      <li>4 for a coordinate with x, y, z and m.</li>
      </ul>

      \param t The geomeytric type.

      \return The number of measurements or axes needed to describe a position in a coordinate system.
    */
    inline int GetCoordDimension(GeomType t)
    {
      if(t & 0x100)   // may be z (0x300), m (0x700) or zm (0x800)
      {
        if(t & 0x800) // it must be zm
          return 4;

        return 3; // it can be z (gType & 0x300) or m (gType & 0x700)
      }

      return 2;
    }

    /*!
      \brief It creates a Geometry (a polygon) from the given envelope.

      \param e    The envelope to extract the coordinates. Don't call with a NULL envelope.
      \param srid The Spatial Reference System ID to be associated to the polygon.

      \return A polygon (in counter-clock-wise) with rectangle coordinates: [(MINX, MINY), (MAXX, MINY), (MAXX, MAXY), (MINX, MAXY), (MINX, MINY)].

      \note The caller of this method will take the ownership of the returned geometry.
    */
    TEGEOMEXPORT Geometry* GetGeomFromEnvelope(const Envelope* const e, int srid);
    
    /*!
      \brief It returns if two geometries satisfy a given spatial relation.
     
      \param g1 The first geometry
      \param g2 The second geometry
      \param r  A given spatial relation to be tested
      
      \return It returns true if the given two geometries satisfy the spatial relation. Otherwise, it returns false.  

      \exception Exception It throws an exception if the spatial relation is not valid or if the test can not be evaluated.
    */
    TEGEOMEXPORT bool SatisfySpatialRelation(const Geometry* g1, const Geometry* g2, SpatialRelation relation);      

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_GEOMUTILS_H

