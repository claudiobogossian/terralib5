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
  \file terralib/geometry/MultiCurve.h

  \brief MultiCurve is a class that represents a 1-dimensional GeometryCollection whose elements are curves.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_MULTICURVE_H
#define __TERRALIB_GEOMETRY_INTERNAL_MULTICURVE_H

// TerraLib
#include "GeometryCollection.h"

namespace te
{
  namespace gm
  {
    /*!
      \class MultiCurve
      
      \brief MultiCurve is a class that represents a 1-dimensional GeometryCollection whose elements are curves.

      \ingroup geometry

      \sa Geometry,
          AbstractPoint, Point, PointM, PointZ, PointZM, PointKd,
          Curve, LineString, Line, LinearRing, CircularString, CompoundCurve,
          Surface, Polygon, Triangle, CurvePolygon, PolyhedralSurface, TIN,
          GeometryCollection, MultiSurface,
          MultiPoint, MultiLineString, MultiPolygon
    */
    class TEGEOMEXPORT MultiCurve : public GeometryCollection
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief It initializes the multi curve with the specified spatial reference system id and envelope.
       
          \param nGeom The number of geometries in the collection.
          \param t     The internal type of the multi curve.
          \param srid  The Spatial Reference System ID associated to the Geometry.
          \param mbr   The minimum bounding rectangle of this geometry (i.e., its envelope).

          \note The multi curve will take the ownership of the given mbr.
        */
        MultiCurve(std::size_t nGeom, GeomType t, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief Copy constructor.

          \param rhs The other geometry.
        */
        MultiCurve(const MultiCurve& rhs);

        /*! \brief Virtual destructor. */
        virtual ~MultiCurve() { }

        /*!
          \brief Assignment operator.

          \param rhs The other geometry.

          \return A reference for this.
        */
        virtual MultiCurve& operator=(const MultiCurve& rhs);

        /** @name Re-Implementation from AbstractData
         *  Methods re-Implementated from AbstractData.
         */
        //@{

        /*!
          \brief It clones the linestring.

          \return A copy of the given linestring.

          \note The caller of this method will take the ownership of the returned linestring.

          \note The cloned linestring will not have the
                MBR computed. This will save time when you
                are just cloning a geometry and don't intend
                to waste time computing the bounding box.
                If you have another suggestion, please, let me know.
        */
        virtual te::dt::AbstractData* clone() const;

        //@}

        /** @name MultiCurve Specific Methods
         *  Specific methods for a MultiCurve.
         */
        //@{

        /*!
          \brief It returns true if this MultiCurve is closed [StartPoint ( ) = EndPoint ( ) for each Curve in this MultiCurve].

          \return true if this MultiCurve is closed [StartPoint ( ) = EndPoint ( ) for each Curve in this MultiCurve].
         */
        bool isClosed() const;

        /*!
          \brief It returns the Length of this MultiCurve which is equal to the sum of the lengths of the element Curves.

          \return The Length of this MultiCurve which is equal to the sum of the lengths of the element Curves.
         */
        double getLength() const;

        //@}

        /** @name Re-Implmentation of methods from Geometry class
         *  Re-Implmentation of basic methods from Geometry class.
         */
        //@{

        /*!
          \brief MultiCurves are 1-dimensional.

          \return 1-dimensional.
        */
        Dimensionality getDimension() const throw();

        //@}
      
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_MULTICURVE_H

