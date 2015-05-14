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
  \file terralib/geometry/PointZ.h

  \brief A point with z-coordinate value.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_POINTZ_H
#define __TERRALIB_GEOMETRY_INTERNAL_POINTZ_H

// TerraLib
#include "Point.h"

namespace te
{
  namespace gm
  {

    /*!
      \class PointZ

      \brief A point with z-coordinate value.

      \ingroup geometry

      \sa Geometry,
          AbstractPoint, Point, PointM, PointZM, PointKd,
          Curve, LineString, LinearRing,
          Surface, Polygon, Triangle, PolyhedralSurface, TIN,
          GeometryCollection, MultiSurface, MultiCurve,
          MultiPoint, MultiLineString, MultiPolygon 
    */
    class TEGEOMEXPORT PointZ : public Point
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer methods on geometric objects
         *  Methods for initializing a geometric object.
         */
        //@{

        /*!
          \brief It initializes the point with the specified spatial reference system id and envelope.       
          
          \param x    The Point x-coordinate value.
          \param y    The Point y-coordinate value.
          \param z    The Point z-coordinate value.
          \param srid The Spatial Reference System ID associated to the Geometry.
          \param mbr  The minimum bounding rectangle of this geometry (i.e., its envelope). It may be a NULL value.

          \note The point will take the ownership of the given mbr.
        */
        PointZ(const double& x, const double& y, const double & z, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief It initializes the Geometry with the specified spatial reference system id and envelope.
       
          \param srid The Spatial Reference System ID associated to the Geometry.
          \param mbr  The minimum bounding rectangle of this geometry (i.e., its envelope). It may be a NULL value.

          \note The point will take the ownership of the given mbr.
        */
        PointZ(int srid = 0, Envelope* mbr = 0);

        /*!
          \brief Copy constructor.

          \param rhs The other geometry.
        */
        PointZ(const PointZ& rhs);

        /*! \brief Destructor. */
        ~PointZ() { }

        /*!
          \brief Assignment operator.

          \param rhs The other geometry.

          \return A reference for this.
        */
        PointZ& operator=(const PointZ& rhs);

        //@}

        /** @name Re-Implementation from AbstractData
         *  Methods re-Implementated from AbstractData.
         */
        //@{

        /*!
          \brief It clones the point.

          \return A copy of the given point.

          \note The caller of this method will take the ownership of the returned point.

          \note The cloned point will not have the
                MBR computed. This will save time when you
                are just cloning a geometry and don't intend
                to waste time computing the bounding box.
                If you have another suggestion, please, let me know.
        */
        te::dt::AbstractData* clone() const;

        //@}

        /** @name Re-Implementation from AbstractPoint
         *  Methods re-implemeneted from AbstractPoint.
         */
        //@{

        /*!
          \brief It returns the Point z-coordinate value.

          \return The z-coordinate value for this Point.
        */
        const double& getZ() const { return m_z; }

        /*!
          \brief It sets the Point z-coordinate value.

          \param z The z-coordinate value for this Point.
        */
        void setZ(const double& z) { m_z = z; }

        //@}

      private:

        double m_z;   //!< The Point z-coordinate value.
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_POINTZ_H


