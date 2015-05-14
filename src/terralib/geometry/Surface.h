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
  \file terralib/geometry/Surface.h

  \brief Surface is an abstract class that represents a 2-dimensional geometric objects.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_SURFACE_H
#define __TERRALIB_GEOMETRY_INTERNAL_SURFACE_H

// TerraLib
#include "Geometry.h"

namespace te
{
  namespace gm
  {
// Forward declarations
    class Point;
    struct Coord2D;

    /*!
      \class Surface
      
      \brief Surface is an abstract class that represents a 2-dimensional geometric objects.

      \ingroup geometry

      \sa Geometry,
          AbstractPoint, Point, PointM, PointZ, PointZM, PointKd,
          Curve, LineString, LinearRing, Line, CircularString, CompoundCurve,
          Polygon, Triangle, CurvePolygon, PolyhedralSurface, TIN,
          GeometryCollection, MultiSurface, MultiCurve,
          MultiPoint, MultiLineString, MultiPolygon
    */
    class TEGEOMEXPORT Surface : public Geometry
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer methods on geometric objects
         *  Methods for initializing a geometric object.
         */
        //@{

        /*!
          \brief It initializes the surface with the specified spatial reference system id and envelope.
       
          \param t    The internal type of the surface.
          \param srid The Spatial Reference System ID associated to the surface.
          \param mbr  The minimum bounding rectangle of this geometry (i.e., its envelope).

          \note The surface will take the ownership of the given mbr.
        */
        Surface(GeomType t, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief Copy constructor.

          \param rhs The other geometry.
        */
        Surface(const Surface& rhs);

        /*! \brief Virtual destructor. */
        virtual ~Surface() { }

        /*!
          \brief Assignment operator.

          \param rhs The other geometry.

          \return A reference for this.
        */
        virtual Surface& operator=(const Surface& rhs);

        //@}

        /** @name Re-Implmentation of methods from Geometry class
         *  Re-Implmentation of basic methods from Geometry class.
         */
        //@{

        /*!
          \brief Surfaces are 2-dimensional objects.

          \return 2-dimensional.
        */
        Dimensionality getDimension() const throw();

        //@}

        /** @name Surface Specific Methods
         *  Specific methods for a Surface.
         */
        //@{

        /*!
          \brief It returns the area of the surface measured in the spatial reference system of the surface.

          \return The area of the surface.
        */
        virtual double getArea() const = 0;

        /*!
          \brief It returns the mathematical centroid for the surface as a point.

          \return The mathematical centroid for the surface.

          \note The caller of this method will take the ownership of the returned point.

          \note The result is not guaranteed to be on this Surface.
        */
        virtual Point* getCentroid() const = 0;

        /*!
          \brief It returns the mathematical centroid for this surface as a coordinate.

          \return The mathematical centroid for this surface.

          \note The caller of this method will take the ownership of the returned coordinate.

          \note The result is not guaranteed to be on this Surface.

          \note TerraLib extended method.
        */
        virtual Coord2D* getCentroidCoord() const = 0;

        /*!
          \brief It returns a point guaranteed to be on this surface.

          \return A point guaranteed to be on this surface.

          \note The caller of this method will take the ownership of the returned point.
        */
        virtual Point* getPointOnSurface() const = 0;

        /*!
          \brief It returns a coordinate guaranteed to be on this surface.

          \return A point guaranteed to be on this surface.

          \note The caller of this method will take the ownership of the returned coordinate.

          \note TerraLib extended method.
         */
        virtual Coord2D* getCoordOnSurface() const = 0;

        /*!
          \brief It returns the length of the boundary for the surface.

          \return The length of the boundary for the surface.
        */
        virtual double getPerimeter() const = 0;

        //@}
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_SURFACE_H

