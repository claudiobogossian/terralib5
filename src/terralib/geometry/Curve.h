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
  \file terralib/geometry/Curve.h

  \brief Curve is an abstract class that represents 1-dimensional geometric objects stored as a sequence of coordinates.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_CURVE_H
#define __TERRALIB_GEOMETRY_INTERNAL_CURVE_H

// TerraLib
#include "Config.h"
#include "Geometry.h"

namespace te
{
  namespace gm
  {
// Forward declaration
    class Point;

    /*!
      \class Curve

      \brief Curve is an abstract class that represents 1-dimensional geometric objects stored as a sequence of coordinates.

      The OGC SFS specification defines only one subclass of
      Curve which uses linear interpolation between Points: LineString, Line, LinearRing.
      The ISO SQL/MM additionaly specifies CircularString and CompoundCurve.

      \ingroup geometry
      
      \sa Geometry,
          AbstractPoint, Point, PointM, PointZ, PointZM, PointKd,
          LineString, LinearRing,
          Surface, Polygon, Triangle, PolyhedralSurface, TIN,
          GeometryCollection, MultiSurface, MultiCurve,
          MultiPoint, MultiLineString, MultiPolygon
    */
    class TEGEOMEXPORT Curve : public Geometry
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer methods on geometric objects
         *  Methods for initializing a geometric object.
         */
        //@{

        /*!
          \brief It initializes the curve with the specified spatial reference system id and envelope.
       
          \param t    The internal type of the curve.
          \param srid The Spatial Reference System ID associated to the curve.
          \param mbr  The minimum bounding rectangle of this geometry (i.e., its envelope). May be a NULL value.

          \note The curve will take the ownership of the given mbr.
         */
        Curve(GeomType t, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief Copy constructor.

          \param rhs The other geometry.
        */
        Curve(const Curve& rhs);

        /*! \brief Virtual destructor. */
        virtual ~Curve() { }

        /*!
          \brief Assignment operator.

          \param rhs The other geometry.

          \return A reference for this.
        */
        virtual Curve& operator=(const Curve& rhs);

        //@}

        /** @name Curve Specific Methods
         *  Specific methods for a Curve.
         */
        //@{

        /*!
          \brief The length of this curve in the unit associated to its spatial reference system.

          \return The length of this Curve in the unit associated to its spatial reference system.
        */
        virtual double getLength() const;

        /*!
          \brief It returns the curve start point.

          \return The curve start point.

          \note The caller of this method will take the ownership of the point geometry.
        */
        virtual Point* getStartPoint() const = 0;

        /*!
          \brief It returns the curve end point.

          \return The curve end point.

          \note The caller of this method will take the ownership of the point geometry.
        */
        virtual Point* getEndPoint() const = 0;

        /*!
          \brief It returns true if the curve is closed (startPoint = endPoint).

          \return True if the curve is closed (startPoint = endPoint).
        */
        virtual bool isClosed() const = 0;

        /*!
          \brief It returns true if the curve is closed and simple.

          \return True if the curve is closed and simple.

          \warning If you just want to know if the first and last points are the same, call isClosed() it is computationally less intensive.
        */
        bool isRing() const;

        //@}

        /** @name Re-Implmentation of methods from Geometry class
         *  Re-Implmentation of basic methods from Geometry class.
         */
        //@{

        /*!
          \brief Curves are 1-dimensional objects.

          \return 1-dimensional.
        */
        Dimensionality getDimension() const throw();

        //@}
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_CURVE_H

