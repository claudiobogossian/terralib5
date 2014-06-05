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
  \file terralib/geometry/MultiPolygon.h

  \brief MultiPolygon is a MultiSurface whose elements are Polygons.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_MULTIPOLYGON_H
#define __TERRALIB_GEOMETRY_INTERNAL_MULTIPOLYGON_H

// TerraLib
#include "MultiSurface.h"

namespace te
{
  namespace gm
  {
    /*!
      \class MultiPolygon
      
      \brief MultiPolygon is a MultiSurface whose elements are Polygons.

      \ingroup geometry

      \sa Geometry,
          Point,
          Curve, LineString, LinearRing,
          Surface, Polygon, Triangle, PolyhedralSurface, TIN,
          GeometryCollection, MultiCurve, MultiSurface,
          MultiPoint, MultiLineString
    */
    class TEGEOMEXPORT MultiPolygon : public MultiSurface
    {
      public:

        TE_DEFINE_VISITABLE

        using Geometry::asText;

        /** @name Initializer methods on geometric objects
         *  Methods for initializing a geometric object.
         */
        //@{

        /*!
          \brief It initializes the Geometry with the specified spatial reference system id and envelope.
       
          \param nGeom The number of geometries in the collection.
          \param t     The internal type of the Geometry.
          \param srid  The Spatial Reference System ID associated to the Geometry.
          \param mbr   The minimum bounding rectangle of this geometry (i.e., its envelope). It may be a NULL value.

          \note The Geometry will take the ownership of the given mbr.
         */
        MultiPolygon(std::size_t nGeom, GeomType t, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief Copy constructor.

          \param rhs The other geometry.
        */
        MultiPolygon(const MultiPolygon& rhs);

        /*! \brief Destructor. */
        ~MultiPolygon() { }

        /*!
          \brief Assignment operator.

          \param rhs The other geometry.

          \return A reference for this.
        */
        MultiPolygon& operator=(const MultiPolygon& rhs);

        //@}


        /** @name Re-Implementation from MultiSurface.
         *  Methods re-Implementated from MultiSurface.
         */
        //@{

        Point* getCentroid() const;

        Coord2D* getCentroidCoord() const;

        //@}

        /** @name Re-Implementation from AbstractData
         *  Methods re-Implementated from AbstractData.
         */
        //@{

        /*!
          \brief It clones the multi polygon.

          \return A copy of the given multi polygon.

          \note The caller of this method will take the ownership of the returned multi polygon.

          \note The cloned multi polygon will not have the
                MBR computed. This will save time when you
                are just cloning a geometry and don't intend
                to waste time computing the bounding box.
                If you have another suggestion, please, let me know.
        */
        te::dt::AbstractData* clone() const;

        //@}

        /** @name Basic methods on geometric objects
         *  Basic methods on geometric objects.
         */
        //@{

        /*!
          \brief The name of the Geometry subtype is: MultiPolygon.

          \return The name of the Geometry subtype is: MultiPolygon.
        */
        const std::string& getGeometryType() const throw();

        //@}      

      private:

        static const std::string sm_typeName;     //!< Geometry type name for MultiPolygon.
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_MULTIPOLYGON_H

