/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/geometry/Line.h

  \brief A Line is LineString with 2 points.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_LINE_H
#define __TERRALIB_GEOMETRY_INTERNAL_LINE_H

// TerraLib
#include "LineString.h"

namespace te
{
  namespace gm
  {
    /*!
      \class Line

      \brief A Line is LineString with 2 points.

      \sa Geometry,
          AbstractPoint, Point, PointM, PointZ, PointZM, PointKd,
          Curve, LineString, LinearRing, CircularString, CompoundCurve,
          Surface, Polygon, Triangle, CurvePolygon, PolyhedralSurface, TIN,
          GeometryCollection, MultiSurface, MultiCurve,
          MultiPoint, MultiLine, MultiPolygon
    */
    class TEGEOMEXPORT Line : public LineString
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer methods on geometric objects
         *  Methods for initializing a geometric object.
         */
        //@{

        /*!
          \brief It initializes the line with the specified spatial reference system id and envelope.
       
          \param t    The internal type of the linestring.
          \param srid The Spatial Reference System ID associated to the linestring.
          \param mbr  The minimum bounding rectangle of this geometry (i.e., its envelope).

          \note The linestring will take the ownership of the given mbr.
        */
        Line(GeomType t, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief It initializes the linestring with the specified spatial reference system id and envelope.
       
          \param size The number of points in the Line. It must be a value greater than 0.
          \param t    The internal type of the linestring.
          \param srid The Spatial Reference System ID associated to the linestring.
          \param mbr  The minimum bounding rectangle of this geometry (i.e., its envelope).

          \note The linestring will take the ownership of the given mbr.
        */
        Line(const Point& first, const Point& second, GeomType t, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief Copy constructor.

          \param rhs The other geometry.
        */
        Line(const Line& rhs);

        /*! \brief Virtual destructor. */
        ~Line();

        /*!
          \brief Assignment operator.

          \param rhs The other geometry.

          \return A reference for this.
        */
        Line& operator=(const Line& rhs);

        //@}

        /** @name Re-Implementation from AbstractData
         *  Methods re-Implementated from AbstractData.
         */
        //@{

        /*!
          \brief It clones the line.

          \return A copy of the given line.

          \note The caller of this method will take the ownership of the returned line.

          \note The cloned linestring will not have the
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

#endif  // __TERRALIB_GEOMETRY_INTERNAL_LINE_H

