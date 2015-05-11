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
  \file terralib/geometry/LinearRing.h

  \brief A LinearRing is a LineString that is both closed and simple.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_LINEARRING_H
#define __TERRALIB_GEOMETRY_INTERNAL_LINEARRING_H

// TerraLib
#include "LineString.h"

namespace te
{
  namespace gm
  {
    /*!
      \class LinearRing

      \brief A LinearRing is a LineString that is both closed and simple.

      Note that the boundary of a linear ring is empty. This class is
      the basic building block of class Polygon.

      \ingroup geometry

      \sa Geometry,
          AbstractPoint, Point, PointM, PointZ, PointZM,
          Curve, LineString, Line, CircularString, CompoundCurve,
          Surface, Polygon, Triangle, PolyhedralSurface, TIN,
          GeometryCollection, MultiSurface, MultiCurve,
          MultiPoint, MultiLineString, MultiPolygon
    */
    class TEGEOMEXPORT LinearRing : public LineString
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer methods on geometric objects
         *  Methods for initializing a geometric object.
         */
        //@{

        /*!
          \brief It initializes the linear ring with the specified spatial reference system id and envelope.
       
          \param t    The internal type of the linear ring.
          \param srid The Spatial Reference System ID associated to the linear ring.
          \param mbr  The minimum bounding rectangle of this geometry (i.e., its envelope).

          \note The Geometry will take the ownership of the given mbr.
        */
        LinearRing(GeomType t, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief It initializes the linear ring with the specified spatial reference system id and envelope.
       
          \param size The number of points in the linear ring. It must be a value greater than 0.
          \param t    The internal type of the Geometry.
          \param srid The Spatial Reference System ID associated to the linear ring.
          \param mbr  The minimum bounding rectangle of this geometry (i.e., its envelope).

          \note The linear ring will take the ownership of the given mbr.
        */
        LinearRing(std::size_t size, GeomType t, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief Copy constructor.

          \param rhs The other geometry.
        */
        LinearRing(const LinearRing& rhs);

        /*! \brief Destructor. */
        ~LinearRing() { }

        /*!
          \brief Assignment operator.

          \param rhs The other geometry.

          \return A reference for this.
        */
        LinearRing& operator=(const LinearRing& rhs);

        //@}

        /** @name Re-Implementation from AbstractData
         *  Methods re-Implementated from AbstractData.
         */
        //@{

        /*!
          \brief It clones the linear ring.

          \return A copy of the given linear ring.

          \note The caller of this method will take the ownership of the returned linear ring.

          \note The cloned linear ring will not have the
                MBR computed. This will save time when you
                are just cloning a geometry and don't intend
                to waste time computing the bounding box.
                If you have another suggestion, please, let me know.
        */
        te::dt::AbstractData* clone() const;

        //@}
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_LINEARRING_H

