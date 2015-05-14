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
  \file terralib/geometry/PointKd.h

  \brief A k-dimensional point.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_POINTKD_H
#define __TERRALIB_GEOMETRY_INTERNAL_POINTKD_H

// TerraLib
#include "AbstractPoint.h"

// STL
#include <vector>

namespace te
{
  namespace gm
  {
    /*!
      \class PointKd

      \brief A k-dimensional point.

      \ingroup geometry

      \sa Geometry,
          AbstractPoint, Point, PointM, PointZ, PointZM,
          Curve, LineString, LinearRing, Line, CircularString, CompoundCurve,
          Surface, Polygon, Triangle, CurvePolygon, PolyhedralSurface, TIN,
          GeometryCollection, MultiSurface, MultiCurve,
          MultiPoint, MultiLineString, MultiPolygon
    */
    class TEGEOMEXPORT PointKd: public AbstractPoint
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief it constructs a new point with k-dimension.

          \param k    The number of dimensions.
          \param srid The Spatial Reference System ID associated to the point.
          \param mbr  The minimum bounding rectangle of this point (i.e., its envelope).
        */
        PointKd(unsigned int k, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief Copy constructor.

          \param rhs The other geometry.
        */
        PointKd(const PointKd& rhs);

        /*! \brief Destructor. */
        ~PointKd() { }

        /*!
          \brief Assignment operator.

          \param rhs The other geometry.

          \return A reference for this.
        */
        PointKd& operator=(const PointKd& rhs);

        /*!
          \brief It returns the i-th coordinate value.

          \param i The index of the desired coordinate value.

          \return The i-th coordinate value.
        */
        const double& getCoord(std::size_t i) const { return m_coords[i]; }

        /*!
          \brief It returns the i-th coordinate value.

          \param i The index of the desired coordinate value.

          \return The i-th coordinate value.
        */
        double& getCoord(std::size_t i) { return m_coords[i]; }

        /*!
          \brief It returns the number of coordinates of the point.

          \return The number of coordinates of the point.
        */
        std::size_t size() const { return m_coords.size(); }

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
         *  Methods re-Implementated from AbstractPoint.
         */
        //@{

        /*!
          \brief It returns the point x-coordinate value.

          \return The x-coordinate value for this point.
        */
        const double& getX() const;

        /*!
          \brief It sets the point x-coordinate value.

          \param x The x-coordinate value for this point.
        */
        void setX(const double& x);

        /*!
          \brief It returns the point y-coordinate value.

          \return The y-coordinate value for this point.
        */
        const double& getY() const;

        /*!
          \brief It sets the point y-coordinate value.

          \param y The y-coordinate value for this point.
        */
        void setY(const double& y);

        /*!
          \brief It returns the point z-coordinate value, if it has one or TE_DOUBLE_NOT_A_NUMBER otherwise.

          \return The z-coordinate value for this point.
         */
        const double& getZ() const;

        /*!
          \brief It sets the point z-coordinate value.

          \param z The z-coordinate value for this point.
         */
        void setZ(const double& z);

        /*!
          \brief It returns TE_DOUBLE_NOT_A_NUMBER.

          \return TE_DOUBLE_NOT_A_NUMBER.
        */
        const double& getM() const;

        /*!
          \brief It doesn't perform any operation in a Kd point.

          \param m The m-coordinate value for this point.
        */
        void setM(const double& m);

        //@}

      private:

        std::vector<double> m_coords; //!< The array of coordinates.
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_POINTKD_H

