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
  \file terralib/geometry/PointM.h

  \brief A point with an associated measure.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_POINTM_H
#define __TERRALIB_GEOMETRY_INTERNAL_POINTM_H

// TerraLib
#include "Point.h"

namespace te
{
  namespace gm
  {

    /*!
      \class PointM

      \brief A point with an associated measure.

      \ingroup geometry

      \sa Geometry,
          AbstractPoint, Point, PointZ, PointZM, PointKd,
          Curve, LineString, LinearRing,
          Surface, Polygon, Triangle, PolyhedralSurface, TIN,
          GeometryCollection, MultiSurface, MultiCurve,
          MultiPoint, MultiLineString, MultiPolygon
    */
    class TEGEOMEXPORT PointM : public Point
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
          \param m    The Point m-coordinate value.
          \param srid The Spatial Reference System ID associated to the Geometry.
          \param mbr  The minimum bounding rectangle of this geometry (i.e., its envelope). It may be a NULL value.

          \note The point will take the ownership of the given mbr.
        */
        PointM(const double& x, const double& y, const double & m, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief It initializes the point with the specified spatial reference system id and envelope.
       
          \param srid The Spatial Reference System ID associated to the Geometry.
          \param mbr  The minimum bounding rectangle of this geometry (i.e., its envelope).

          \note The point will take the ownership of the given mbr.
        */
        PointM(int srid = 0, Envelope* mbr = 0);

        /*!
          \brief Copy constructor.

          \param rhs The other geometry.
        */
        PointM(const PointM& rhs);

        /*! \brief Destructor. */
        ~PointM() { }

        /*!
          \brief Assignment operator.

          \param rhs The other geometry.

          \return A reference for this.
        */
        PointM& operator=(const PointM& rhs);

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

        /** @name Re-Implementation from Geometry
         *  Methods re-Implementated from Geometry.
         */
        //@{

        /*!
          \brief It returns a derived geometry collection value according to the range of coordinate values inclusively.

          \param mStart The initial coordinate value.
          \param mEnd The final coordinate value.

          \return A GeometryCollection value.

          \note The caller of this method will take the ownership of geometry.

          \note This method only applies to Point and Line geometries, including homogeneu collections of points or lines.
                For polygons this will return a NULL value.
         */
        Geometry* locateBetween(const double& mStart, const double& mEnd) const throw(Exception);

        //@}

        /** @name Re-Implementation from AbstractPoint
         *  Methods re-implemeneted from AbstractPoint.
         */
        //@{

        /*!
          \brief It returns the Point z-coordinate value.

          \return The z-coordinate value for this Point.
        */
        const double& getM() const { return m_m; }

        /*!
          \brief It sets the Point z-coordinate value.

          \param z The z-coordinate value for this Point.
        */
        void setM(const double& m) { m_m = m; }

        //@}

      private:

        double m_m;   //!< The Point m-coordinate value.
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_POINTM_H


