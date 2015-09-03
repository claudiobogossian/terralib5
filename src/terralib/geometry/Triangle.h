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
  \file terralib/geometry/Triangle.h

  \brief Triangle is a polygon with 3 distinct, non-collinear vertices and no interior boundary.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_TRIANGLE_H
#define __TERRALIB_GEOMETRY_INTERNAL_TRIANGLE_H

// TerraLib
#include "Polygon.h"

namespace te
{
  namespace gm
  {
    /*!
      \class Triangle
      
      \brief Triangle is a polygon with 3 distinct, non-collinear vertices and no interior boundary.

      \ingroup geometry

      \sa Geometry,
          AbstractPoint, Point, PointM, PointZ, PointZM, PointKd,
          Curve, LineString, LinearRing, Line, CircularString, CompoundCurve,
          Surface, CurvePolygon, Polygon, PolyhedralSurface, TIN,
          GeometryCollection, MultiSurface, MultiCurve,
          MultiPoint, MultiLineString, MultiPolygon
    */
    class TEGEOMEXPORT Triangle : public Polygon
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer methods on geometric objects
         *  Methods for initializing a geometric object.
         */
        //@{

        /*!
          \brief It initializes the Geometry with the specified spatial reference system id and envelope.
       
          \param nRings The number of rings forming the triangle. 
          \param t      The internal type of the Geometry.
          \param srid   The Spatial Reference System ID associated to the Geometry.
          \param mbr    The minimum bounding rectangle of this geometry (i.e., its envelope). It may be a NULL value.

          \note The Geometry will take the ownership of the given mbr.
          \note The nRing informed should be 0 or 1.

          \warning Set all nRing informed, otherwise you can not use methods like computeMBR().
         */
        Triangle(std::size_t nRings, GeomType t, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief It initializes the Geometry with the specified spatial reference system id and envelope.
       
          \param t      The internal type of the Geometry.
          \param srid   The Spatial Reference System ID associated to the Geometry.
          \param mbr    The minimum bounding rectangle of this geometry (i.e., its envelope).

          \note The Geometry will take the ownership of the given mbr.
         */
        Triangle(GeomType t, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief Copy constructor.

          \param rhs The other geometry.
        */
        Triangle(const Triangle& rhs);

        /*! \brief Destructor. */
        ~Triangle() {}

        /*!
          \brief Assignment operator.

          \param rhs The other geometry.

          \return A reference for this.
        */
        Triangle& operator=(const Triangle& rhs);

        //@}

	public:
		std::size_t number_of_children; //!< number of children of the triangle
		std::size_t number_of_parents;  //!< number of parents of the triangle
		std::vector<Triangle> children; //!< references to the children of the triangle
		Triangle *adjacent[3]; //!< references to the adjacentof the triangle
		Triangle *parents[2]; //!< references to the parents of the triangle
		int triangle_index;	//!< triangle index in vector
		double xc, yc; 	//!< circumcenter coordinates of the circunference defined from the 3 points of the triangle
		double r; //!< circunference radius
		bool processed, visited; //!< flags
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_TRIANGLE_H


