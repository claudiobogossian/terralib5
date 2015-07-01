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
  \file terralib/geometry/PolyhedralSurface.h

  \brief PolyhedralSurface is a contiguous collection of polygons, which share common boundary segments.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_POLYHEDRALSURFACE_H
#define __TERRALIB_GEOMETRY_INTERNAL_POLYHEDRALSURFACE_H

// TerraLib
#include "Surface.h"

// STL
#include <vector>

namespace te
{
  namespace gm
  {
// Forward declarations
    class MultiPolygon;
    class Polygon;

    /*!
      \class PolyhedralSurface
      
      \brief PolyhedralSurface is a contiguous collection of polygons, which share common boundary segments.

      \ingroup geometry

      \sa Geometry,
          Point,
          Curve, LineString, LinearRing,
          Surface, Polygon, Triangle, TIN,
          GeometryCollection, MultiSurface, MultiCurve,
          MultiPoint, MultiLineString, MultiPolygon
    */
    class TEGEOMEXPORT PolyhedralSurface : public Surface
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer methods on geometric objects
         *  Methods for initializing a geometric object.
         */
        //@{

        /*!
          \brief It initializes the Geometry with the specified spatial reference system id and envelope.
       
          \param nPolygons The number of polygons forming the polyhedral.
          \param t         The internal type of the Geometry.
          \param srid      The Spatial Reference System ID associated to the Geometry.
          \param mbr       The minimum bounding rectangle of this geometry (i.e., its envelope). It may be a NULL value.

          \note The Geometry will take the ownership of the given mbr.
        */
        PolyhedralSurface(std::size_t nPolygons, GeomType t, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief Copy constructor.

          \param rhs The other geometry.
        */
        PolyhedralSurface(const PolyhedralSurface& rhs);

        /*! \brief Virtual destructor. */
        virtual ~PolyhedralSurface();

        /*!
          \brief Assignment operator.

          \param rhs The other geometry.

          \return A reference for this.
        */
        virtual PolyhedralSurface& operator=(const PolyhedralSurface& rhs);

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

        /** @name PolyhedralSurface Specific Methods
         *  Specific methods for a PolyhedralSurface.
         */
        //@{

        /*!
          \brief It returns the number of including polygons.

          \return The number of including polygons.
        */
        std::size_t getNumPatches() const
        {
          return m_polygons.size();
        }

        /*!
          \brief It sets the number of including polygons.

          If the new size is less than the old it will drop the geometries.

          \param size The number of polygons for the PolyhedralSurface.

          \note TerraLib extended method.
        */
        void setNumPatches(std::size_t size);

        /*!
          \brief It returns a polygon in this surface, the order is arbitrary.

          \param i The index of the polygon component.

          \return A polygon in this surface, the order is arbitrary.

          \note It doesn't check the index range.
        */
        Polygon* getPatchN(std::size_t i) const;

        /*!
          \brief It sets the informed position polygon to the new one.

          \param i The polygon index.
          \param p The new polygon to be placed in the informed position.

          \note The polygon index start at 0.

          \note It doesn't check the index range.

          \note TerraLib extended method.
        */
        void setPatchN(std::size_t i, Polygon* p);

        /*!
          \brief It returns the collection of polygons in this surface that bounds the given polygon "p" for any polygon "p" in the surface.

          \param p The bounding Polygon.

          \return The collection of polygons in this surface that bounds the given polygon "p" for any polygon "p" in the surface.

          \note The caller of this method will take the ownership of the returned geometry.
        */
        MultiPolygon* getBoundingPolygons(const Polygon* p) const;

        /*!
          \brief It returns true if the polygon closes on itself, and thus has no boundary and encloses a solid.

          \return True if the polygon closes on itself, and thus has no boundary and encloses a solid.
        */
        bool isClosed() const
        {
          return false;
        }

        //@}

        /** @name Re-Implmentation of methods from surface class
         *  Re-Implmentation of basic methods from surface class.
         */
        //@{

        /*!
          \brief It returns the area of this surface, as measured in the spatial reference system of this surface.

          \return The area of this surface.
         */
        double getArea() const
        {
          return 0.0;
        }

        /*!
          \brief It returns the mathematical centroid for this surface as a point.

          \return The mathematical centroid for this surface.

          \note The caller of this method will take the ownership of the returned point.

          \note The result is not guaranteed to be on this Surface.
         */
        Point* getCentroid() const
        {
          return 0;
        }

        /*!
          \brief It returns the mathematical centroid for this surface as a coordinate.

          \return The mathematical centroid for this surface.

          \note The caller of this method will take the ownership of the returned coordinate.

          \note The result is not guaranteed to be on this Surface.

          \note TerraLib extended method.
         */
        Coord2D* getCentroidCoord() const
        {
          return 0;
        }

        /*!
          \brief It returns a point guaranteed to be on this surface.

          \return A point guaranteed to be on this surface.

          \note The caller of this method will take the ownership of the returned point.
         */
        Point* getPointOnSurface() const
        {
          return 0;
        }

        /*!
          \brief It returns a coordinate guaranteed to be on this surface.

          \return A point guaranteed to be on this surface.

          \note The caller of this method will take the ownership of the returned coordinate.

          \note TerraLib extended method.
         */
        Coord2D* getCoordOnSurface() const
        {
          return 0;
        }

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
          \brief The name of the geometry subtype for PolyhedralSurface is: PolyhedralSurface.          

          \return The name of the geometry subtype for PolyhedralSurface is: PolyhedralSurface.
         */
        virtual const std::string& getGeometryType() const throw();

        /*!
          \brief It sets the Spatial Reference System ID of the Geometry and all its parts if it is a GeometryCollection (or a Multi).

          \param srid The Spatial Reference System ID to be associated to the geometric object.

          \note TerraLib extended method.
        */
        void setSRID(int srid) throw();

        /*!
          \brief It will transform the coordinates of the Geometry to the new one.

          After calling this method the Geometry will be associated to the new SRID.

          \param srid The new Spatial Reference System ID used to transform the coordinates of the Geometry.

          \exception Exception It will throw an exception if it can not do the transformation.

          \note The Geometry must be associated to a valid SRID before calling this method.

          \note If the geometry already has an associated MBR, it will automatically update it (i. e. automatically recompute it).

          \note TerraLib extended method.
        */
        void transform(int srid) throw(te::common::Exception);

        /*!
          \brief It computes the minimum bounding rectangle for the Geometry.

          \param cascade If true, it will update the MBR of its parts.

          \note You can use this method in order to update the MBR of the Geometry.

          \note TerraLib extended method.
        */
        void computeMBR(bool cascade) const throw();

        /*!
          \brief it returns the number of points (vertexes) in the geometry.

          \return The number of points (vertexes) in the geometry.

          \note TerraLib extended method.
         */
        std::size_t getNPoints() const throw();

        //@}

      protected:

        std::vector<Polygon*> m_polygons;     //!< An array with the polygon list.

      private:

        static const std::string sm_typeName; //!< Geometry type name for PolyhedralSurface.
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_POLYHEDRALSURFACE_H


