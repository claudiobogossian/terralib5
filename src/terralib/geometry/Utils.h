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
  \file terralib/geometry/Utils.h

  \brief Utility functions for the Geometry Module.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_GEOMUTILS_H
#define __TERRALIB_GEOMETRY_INTERNAL_GEOMUTILS_H

// TerraLib
#include "Config.h"
#include "Coord2D.h"
#include "Enums.h"

// STL
#include <vector>

namespace te
{
  namespace gm
  {
// Forward declarations
    class Envelope;
    class Geometry;
    class Point;
    class Polygon;
    class LineString;
    
    /*!
    \struct TopologyValidationError

    \brief This struct contains informations about GEOS TopologyValidationError
    */
    struct TopologyValidationError
    {
      public:

        Coord2D     m_coordinate;
        std::string m_message;
    };

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

    /*!
      \brief  Finds the correspondent smallest box that allows a box to be cut in blocks of a given size
     
      \param env     Reference envelope
      \param bWidth  Block width
      \param bHeight Block height
      
      \return It returns a adjusted envelope
    */
    TEGEOMEXPORT Envelope AdjustToCut(const Envelope & env, double bWidth, double bHeight);

    /* \brief It checks if one object intersects another object. */
    template<class T1, class T2> bool Intersects(const T1& o1, const T2& o2);

    /* \brief Specialized function that checks if point intersects envelope. */
    template<> TEGEOMEXPORT bool Intersects(const te::gm::Point& point, const te::gm::Envelope& e);

    /*!
      \brief Make the line interpolation to find a target
     
      \param line LineString to make the interpolation
      \param initial Initial value
      \param final Final value
      \param target Target value
      
      \return It returns a target Coord2D in the line.
    */
    TEGEOMEXPORT Coord2D* locateAlong(const LineString* line, double initial, double final, double target);
    
    /*!
    \brief It will get a GeometryCollection and distribute in a vector.

    \param g      Input GeometryCollection.
    \param geoms  Output Geometry Vector.

    \note If the geomSource is not a te::gm::GeometryCollectionType it will return an empty vector.
    */
    TEGEOMEXPORT void Multi2Single(te::gm::Geometry* g, std::vector<te::gm::Geometry*>& geoms);

    /*!
    \brief It will fix geometries with self-intersection.

    \param g Input Geometry to fix.
    
    \return a vector of geometries.

    \note It will not check if the geometry is invalid.

    \exception Exception It throws an exception if the geometry is not Polygon or MultiPolygon.
    */
    TEGEOMEXPORT std::vector<te::gm::Geometry*> FixSelfIntersection(const te::gm::Geometry* g);

    /*!
    \brief It will get the union of the input geometries.

    \param geom   Input Geometry.

    \return a Geometry containing the union.

    \note If no input geometries were provided, a POINT EMPTY is returned.
    */
    TEGEOMEXPORT te::gm::Geometry* UnaryUnion(te::gm::Geometry* geom);

    /*!
    \brief It will get a list of polygons formed by the polygonization.

    \param g      Input Polygon.
    \param pols   Output Polygon Vector.
    */
    TEGEOMEXPORT void Polygonizer(te::gm::Geometry* g, std::vector<te::gm::Polygon*>& pols);

    /*!
    \brief It check geometry validity using GEOS.

    \param geom  Geometry that will be verified.
    \param error TopologyValidationError struct.

    \return True if geometry is valid.
    */
    TEGEOMEXPORT bool CheckValidity(const te::gm::Geometry* geom, te::gm::TopologyValidationError& error);

    /*!
    \brief Get/create a valid version of the geometry given. If the geometry is a polygon or multi polygon, self intersections /
           inconsistencies are fixed. Otherwise the geometry is returned.

    \param geom
    \return a geometry
    */
    TEGEOMEXPORT te::gm::Geometry* validate(te::gm::Geometry* geom);

    /*!
    \brief Add all line strings from the polygon given to the vector given.

    \param polygon polygon from which to extract line strings
    \param pAdd A reference to a vector of geometries.
    */
    TEGEOMEXPORT void addPolygon(te::gm::Polygon* polygon, std::vector<te::gm::Geometry*>& pAdd);

    /*!
    \brief Add the linestring given to the vector.

    \param linestring line string
    \param pAdd A reference to a vector of geometries.
    */
    TEGEOMEXPORT void addLineString(te::gm::LineString* lineString, std::vector<te::gm::Geometry*>& pAdd);

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_GEOMUTILS_H

