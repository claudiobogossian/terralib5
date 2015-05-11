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
  \file terralib/geometry/MultiSurface.h

  \brief MultiSurface is a class that represents a 2-dimensional GeometryCollection whose elements are surfaces.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_MULTISURFACE_H
#define __TERRALIB_GEOMETRY_INTERNAL_MULTISURFACE_H

// TerraLib
#include "Coord2D.h"
#include "GeometryCollection.h"

namespace te
{
  namespace gm
  {
// Forward declarations
    class Point;

    /*!
      \class MultiSurface

      \brief MultiSurface is a class that represents a 2-dimensional GeometryCollection whose elements are surfaces.

      \ingroup geometry

      \sa Geometry,
          Point,
          Curve, LineString, LinearRing,
          Surface, Polygon, Triangle, PolyhedralSurface, TIN,
          GeometryCollection, MultiCurve,
          MultiPoint, MultiLineString, MultiPolygon
    */
    class TEGEOMEXPORT MultiSurface : public GeometryCollection
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief It initializes the Geometry with the specified spatial reference system id and envelope.
       
          \param nGeom The number of geometries in the collection.
          \param t     The internal type of the Geometry.
          \param srid  The Spatial Reference System ID associated to the Geometry.
          \param mbr   The minimum bounding rectangle of this geometry (i.e., its envelope).

          \note The Geometry will take the ownership of the given mbr.
        */
        MultiSurface(std::size_t nGeom, GeomType t, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief Copy constructor.

          \param rhs The other geometry.
        */
        MultiSurface(const MultiSurface& rhs);

        /*! \brief Virtual destructor. */
        virtual ~MultiSurface() { }

        /*!
          \brief Assignment operator.

          \param rhs The other geometry.

          \return A reference for this.
        */
        virtual MultiSurface& operator=(const MultiSurface& rhs);

        /** @name Re-Implementation from AbstractData
         *  Methods re-Implementated from AbstractData.
         */
        //@{

        /*!
          \brief It clones the multi surface.

          \return A copy of the given multi surface.

          \note The caller of this method will take the ownership of the returned multi surface.

          \note The cloned multi surface will not have the
                MBR computed. This will save time when you
                are just cloning a geometry and don't intend
                to waste time computing the bounding box.
                If you have another suggestion, please, let me know.
        */
        virtual te::dt::AbstractData* clone() const;

        //@}

        /** @name MultiSurface Specific Methods
         *  Specific methods for a MultiSurface.
         */
        //@{

        /*!
          \brief It returns the area of this MultiSurface, as measured in the spatial reference system of this multisurface.

          \return The area of this multisurface.
        */
        double getArea() const;

        /*!
          \brief It returns the mathematical centroid for this MultiSurface as a point.

          \return The mathematical centroid for this multisurface.

          \note The caller of this method will take the ownership of the returned point.

          \note The result is not guaranteed to be on this multisurface.
        */
        Point* getCentroid() const;

        /*!
          \brief It returns the mathematical centroid for this MultiSurface as a coordinate.

          \return The mathematical centroid for this multisurface.

          \note The caller of this method will take the ownership of the returned coordinate.

          \note The result is not guaranteed to be on this multisurface.

          \note TerraLib extended method.
        */
        Coord2D* getCentroidCoord() const;

        /*!
          \brief It returns a point guaranteed to be on this MultiSurface.

          \return A point guaranteed to be on this MultiSurface.

          \note The caller of this method will take the ownership of the returned point.
        */
        Point* getPointOnSurface() const;

        /*!
          \brief It returns a coordinate guaranteed to be on this MultiSurface.

          \return A point guaranteed to be on this MultiSurface.

          \note The caller of this method will take the ownership of the returned coordinate.

          \note TerraLib extended method.
        */
        Coord2D* getCoordOnSurface() const;

        /*!
          \brief It returns the length of the boundary for the surface.

          \return The length of the boundary for the surface.
        */
        double getPerimeter() const;

        //@}

        /** @name Re-Implmentation of methods from Geometry class
         *  Re-Implmentation of basic methods from Geometry class.
         */
        //@{

        /*!
          \brief MultiSurface is 2-dimenional.

          \return 2-dimenional.
         */
        Dimensionality getDimension() const throw();

        //@}
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_MULTISURFACE_H

