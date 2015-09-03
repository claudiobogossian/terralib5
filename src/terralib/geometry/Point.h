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
  \file terralib/geometry/Point.h

  \brief A point with x and y coordinate values.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_POINT_H
#define __TERRALIB_GEOMETRY_INTERNAL_POINT_H

// TerraLib
#include "AbstractPoint.h"

namespace te
{
  namespace gm
  {
    /*!
      \class Point

      \brief A point with x and y coordinate values.

      \ingroup geometry

      \sa Geometry,
          AbstractPoint, PointM, PointZ, PointZM, PointKd,
          Curve, LineString, LinearRing, Line, CircularString, CompoundCurve,
          Surface, Polygon, Triangle, CurvePolygon, PolyhedralSurface, TIN,
          GeometryCollection, MultiSurface, MultiCurve,
          MultiPoint, MultiLineString, MultiPolygon
    */
    class TEGEOMEXPORT Point: public AbstractPoint
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer methods on geometric objects
         *  Methods for initializing a geometric object.
         */
        //@{

        /*!
          \brief It initializes the Geometry with the specified spatial reference system id and envelope.       
          
          \param x    The Point x-coordinate value.
          \param y    The Point y-coordinate value.
          \param srid The Spatial Reference System ID associated to the Geometry.
          \param mbr  The minimum bounding rectangle of this geometry (i.e., its envelope).

          \note The Geometry will take the ownership of the given mbr.
        */
        Point(const double& x, const double& y, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief It initializes the Geometry with the specified spatial reference system id and envelope.
       
          \param srid The Spatial Reference System ID associated to the Geometry.
          \param mbr  The minimum bounding rectangle of this geometry (i.e., its envelope). It may be a NULL value.

          \note The Geometry will take the ownership of the given mbr.
        */
        Point(int srid = 0, Envelope* mbr = 0);

        /*!
          \brief Copy constructor.

          \param rhs The other geometry.
        */
        Point(const Point& rhs);

        /*!
          \brief Assignment operator.

          \param rhs The other geometry.

          \return A reference for this.
         */
        virtual Point& operator=(const Point& rhs);

        /*! \brief Virtual destructor. */
        virtual ~Point() { }

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
        virtual te::dt::AbstractData* clone() const;

        //@}

        /** @name Re-Implmentation from AbstractPoint
         *  Methods re-implemented from AbstractPoint.
         */
        //@{

        /*!
          \brief It returns the Point x-coordinate value.

          \return The x-coordinate value for this Point.
        */
        const double& getX() const { return m_x; }

        /*!
          \brief It sets the Point x-coordinate value.

          \param x The x-coordinate value for this Point.
        */
        void setX(const double& x) { m_x = x; }

        /*!
          \brief It returns the Point y-coordinate value.

          \return The y-coordinate value for this Point.
        */
        const double& getY() const { return m_y; }

        /*!
          \brief It sets the Point y-coordinate value.

          \param y The y-coordinate value for this Point.
        */
        void setY(const double& y) { m_y = y; }

        /*!
          \brief It returns the Point z-coordinate value, if it has one or DoubleNotANumber otherwise.

          \return The z-coordinate value for this Point.
        */
        virtual const double& getZ() const { return sm_notNumber; }

        /*!
          \brief It sets the Point z-coordinate value.

          \param z The z-coordinate value for this Point.
        */
        virtual void setZ(const double& z);

        /*!
          \brief It returns the Point m-coordinate value, if it has one or DoubleNotANumber otherwise.

          \return The m-coordinate value for this Point.
        */
        virtual const double& getM() const { return sm_notNumber; }

        /*!
          \brief It sets the Point m-coordinate value.

          \param m The m-coordinate value for this Point.
        */
        virtual void setM(const double& m);

        /*!
        \brief Less then operator.

        \param rhs The source object to be compared.

        \return True if the point have the coordinates values is less.
        */
        bool operator<(const Point& rhs);


        //@}

      public:

         /*!
          \brief It initializes the Geometry with the specified spatial reference system id and envelope.
       
          \param t    The internal type of the Geometry.
          \param srid The Spatial Reference System ID associated to the Geometry.
          \param mbr  The minimum bounding rectangle of this geometry (i.e., its envelope). It may be a NULL value.

          \note The Geometry will take the ownership of the given mbr.
         */
        Point(GeomType t, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief It initializes the Geometry with the specified spatial reference system id and envelope.
       
          \param t    The internal type of the Geometry.
          \param srid The Spatial Reference System ID associated to the Geometry.
          \param mbr  The minimum bounding rectangle of this geometry (i.e., its envelope). It may be a NULL value.
          \param x    The Point x-coordinate value.
          \param y    The Point y-coordinate value.

          \note The Geometry will take the ownership of the given mbr.
         */
        Point(GeomType t, int srid, Envelope* mbr, const double& x, const double& y);

        //@}

      protected:

        double m_x;   //!< The Point x-coordinate value.
        double m_y;   //!< The Point y-coordinate value.

        static const std::string sm_typeName; //!< Geometry type name for Point.
        static const double sm_notNumber;     //!< Just a special value to return in the case of a invalid Geometry.
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_POINT_H

