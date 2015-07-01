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
  \file terralib/geometry/AbstractPoint.h

  \brief A base abstract class for 0-dimensional geometric objects that represents a single location in coordinate space.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_ABSTRACTPOINT_H
#define __TERRALIB_GEOMETRY_INTERNAL_ABSTRACTPOINT_H

// TerraLib
#include "Geometry.h"

namespace te
{
  namespace gm
  {
    /*!
      \class AbstractPoint

      \brief A base abstract class for 0-dimensional geometric objects that represents a single location in coordinate space.

      \ingroup geometry

      \sa Geometry,
          Point, PointM, PointZ, PointZM, PointKd,
          Curve, LineString, LinearRing, Line, CircularString, CompoundCurve,
          Surface, Polygon, Triangle, CurvePolygon, PolyhedralSurface, TIN,
          GeometryCollection, MultiSurface, MultiCurve,
          MultiPoint, MultiLineString, MultiPolygon
    */
    class TEGEOMEXPORT AbstractPoint: public Geometry
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief It initializes the point with the specified spatial reference system id and envelope.       
          
          \param t    The internal point type.
          \param srid The Spatial Reference System ID associated to the point.
          \param mbr  The minimum bounding rectangle of this point (i.e., its envelope).

          \note The point will take the ownership of the given mbr.
        */
        AbstractPoint(GeomType t, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief Copy constructor.

          \param rhs The other geometry.
        */
        AbstractPoint(const AbstractPoint& rhs);

        /*! \brief Virtual destructor. */
        virtual ~AbstractPoint() { }

        /*!
          \brief Assignment operator.

          \param rhs The other geometry.

          \return A reference for this.
        */
        virtual AbstractPoint& operator=(const AbstractPoint& rhs);

        //@}

        /** @name Point Specific Methods
         *  Specific methods that a point subclass must implement.
         */
        //@{

        /*!
          \brief It returns the point x-coordinate value.

          \return The x-coordinate value for this point.
        */
        virtual const double& getX() const = 0;

        /*!
          \brief It sets the point x-coordinate value.

          \param x The x-coordinate value for this point.
        */
        virtual void setX(const double& x) = 0;

        /*!
          \brief It returns the point y-coordinate value.

          \return The y-coordinate value for this point.
        */
        virtual const double& getY() const = 0;

        /*!
          \brief It sets the point y-coordinate value.

          \param y The y-coordinate value for this point.
        */
        virtual void setY(const double& y) = 0;

        /*!
          \brief It returns the point z-coordinate value, if it has one or TE_DOUBLE_NOT_A_NUMBER otherwise.

          \return The z-coordinate value for this point.
         */
        virtual const double& getZ() const = 0;

        /*!
          \brief It sets the point z-coordinate value.

          \param z The z-coordinate value for this point.
         */
        virtual void setZ(const double& z) = 0;

        /*!
          \brief It returns the point m-coordinate value, if it has one or TE_DOUBLE_NOT_A_NUMBER otherwise.

          \return The m-coordinate value for this point.
        */
        virtual const double& getM() const = 0;

        /*!
          \brief It sets the point m-coordinate value.

          \param m The m-coordinate value for this point.
        */
        virtual void setM(const double& m) = 0;

        //@}

        /** @name Re-Implmentation of methods from Geometry class
         *  Re-Implmentation of basic methods from Geometry class.
         */
        //@{

        /*!
          \brief Points are 0-dimensional objects.

          \return te::gm::P (0-dimensional)
        */
        Dimensionality getDimension() const throw();

        /*!
          \brief The name of instantiable subtype is: Point.

          \return The name of instantiable subtype is: Point.
        */
        const std::string& getGeometryType() const throw();

        /*!
          \brief It sets the Spatial Reference System ID of the Point.

          \param srid The Spatial Reference System ID to be associated to the geometric object.

          \note TerraLib extended method.
        */
        void setSRID(int srid) throw();

        /*!
          \brief It converts the coordinate values of the point to the new spatial reference system.

          After calling this method the point will be associated to the new SRID.

          \param srid The new Spatial Reference System ID used to transform the coordinates of the point.

          \exception Exception It will throw an exception if it can not do the transformation.

          \note The point must be associated to a valid SRID before calling this method.

          \note If the point already has an associated MBR, this method will automatically update it (i. e. automatically recompute it).
        */
        void transform(int srid) throw(te::common::Exception);

        /*!
          \brief It computes the minimum bounding rectangle for the point.

          \param cascade For points this flag doesn't have effect.

          \note You can use this method in order to update the MBR of the point.

          \note TerraLib extended method.
        */
        void computeMBR(bool cascade) const throw();

        /*!
          \brief it returns the number of points (vertexes) in the geometry.

          \return The number of points (vertexes) in the geometry.

          \note TerraLib extended method.
        */
        std::size_t getNPoints() const throw() { return 1; }

        //@}

      protected:

        static const std::string sm_typeName; //!< Geometry type name for AbstractPoint.

        static const double sm_notNumber;     //!< Just a special value to return in the case of an invalid geometry.
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_ABSTRACTPOINT_H

