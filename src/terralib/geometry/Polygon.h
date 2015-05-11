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
  \file terralib/geometry/Polygon.h

  \brief Polygon is a subclass of CurvePolygon whose rings are defined by linear rings.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_POLYGON_H
#define __TERRALIB_GEOMETRY_INTERNAL_POLYGON_H

// TerraLib
#include "CurvePolygon.h"

namespace te
{
  namespace gm
  {
    /*!
      \class Polygon

      \brief Polygon is a subclass of CurvePolygon whose rings are defined by linear rings.

      \ingroup geometry
      
      \sa Geometry,
          AbstractPoint, Point, PointM, PointZ, PointZM, PointKd,
          Curve, LineString, LinearRing, Line, CircularString, CompoundCurve,
          Surface, CurvePolygon, Triangle, PolyhedralSurface, TIN,
          GeometryCollection, MultiSurface, MultiCurve,
          MultiPoint, MultiLineString, MultiPolygon
    */
    class TEGEOMEXPORT Polygon : public CurvePolygon
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer methods on geometric objects
         *  Methods for initializing a geometric object.
         */
        //@{

        /*!
          \brief It initializes the Geometry with the specified spatial reference system id and envelope.
       
          \param nRings The number of rings forming the polygon.
          \param t      The internal type of the Geometry.
          \param srid   The Spatial Reference System ID associated to the Geometry.
          \param mbr    The minimum bounding rectangle of this geometry (i.e., its envelope). It may be a NULL value.

          \note The Geometry will take the ownership of the given mbr.

          \warning Set all nRing informed, otherwise you can not use methods like computeMBR().
         */
        Polygon(std::size_t nRings, GeomType t, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief Copy constructor.

          \param rhs The other geometry.
        */
        Polygon(const Polygon& rhs);

        /*! \brief Virtual destructor. */
        virtual ~Polygon();

        /*!
          \brief Assignment operator.

          \param rhs The other geometry.

          \return A reference for this.
        */
        virtual Polygon& operator=(const Polygon& rhs);

        //@}

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

        /** @name Re-Implmentation of methods from Geometry class
         *  Re-Implmentation of basic methods from Geometry class.
         */
        //@{

        /*!
          \brief It returns the name of the instantiable subtype of Geometry of which this geometric object is an instantiable member.

          The name of the Geometry subtype for Polygons is: Polygon.

          \return The name of the instantiable subtype of Geometry of which this geometric object is an instantiable member. In this case, it returns Polygon.
         */
        virtual const std::string& getGeometryType() const throw();

        //@}

      private:

        static const std::string sm_typeName; //! Geometry type name for Polygon.
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_POLYGON_H




