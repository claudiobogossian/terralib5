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
  \file terralib/geometry/CurvePolygon.h

  \brief CurvePolygon is a planar surface defined by 1 exterior boundary and 0 or more interior boundaries.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_CURVEPOLYGON_H
#define __TERRALIB_GEOMETRY_INTERNAL_CURVEPOLYGON_H

// TerraLib
#include "Surface.h"

// STL
#include <cassert>
#include <vector>

namespace te
{
  namespace gm
  {
// Forward declaration
    class Curve;

    /*!
      \class CurvePolygon

      \brief CurvePolygon is a planar surface defined by 1 exterior boundary and 0 or more interior boundaries.

      \ingroup geometry
      
      \sa Geometry,
          AbstractPoint, Point, PointM, PointZ, PointZM, PointKd,
          Curve, LineString, LinearRing, Line, CircularString, CompoundCurve,
          Surface, Triangle, Polygon, PolyhedralSurface, TIN,
          GeometryCollection, MultiSurface, MultiCurve,
          MultiPoint, MultiLineString, MultiPolygon
    */
    class TEGEOMEXPORT CurvePolygon : public Surface
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer methods on geometric objects
         *  Methods for initializing a geometric object.
         */
        //@{

        /*!
          \brief It initializes the curve polygon with the specified spatial reference system id and envelope.
       
          \param nRings The number of rings forming the curve polygon.
          \param t      The internal type of the curve polygon.
          \param srid   The Spatial Reference System ID associated to the curve polygon.
          \param mbr    The minimum bounding rectangle of this geometry (i.e., its envelope). It may be a NULL value.

          \note The curve polygon will take the ownership of the given mbr.

          \warning Set all nRing informed, otherwise you can not use methods like computeMBR().
        */
        CurvePolygon(std::size_t nRings, GeomType t, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief Copy constructor.

          \param rhs The other geometry.
        */
        CurvePolygon(const CurvePolygon& rhs);

        /*! \brief Virtual destructor. */
        virtual ~CurvePolygon();

        /*!
          \brief Assignment operator.

          \param rhs The other geometry.

          \return A reference for this.
        */
        virtual CurvePolygon& operator=(const CurvePolygon& rhs);

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

        /** @name CurvePolygon Specific Methods
         *  Specific methods for a CurvePolygon.
         */
        //@{

        /*!
          \brief It returns the exterior ring of this CurvePolygon.

          \return The exterior ring of this CurvePolygon.

          \note Don't call this method for a empty polygon.
        */
        Curve* getExteriorRing() const;

        /*!
          \brief It returns the number of interior rings in this CurvePolygon.

          \return The number of interior rings in this CurvePolygon.
        */
        std::size_t getNumInteriorRings() const;

        /*!
          \brief It returns the number of rings in this CurvePolygon.

          \return The number of rings in this CurvePolygon.

          \note TerraLib extended method.
        */
        std::size_t getNumRings() const
        {
          return m_rings.size();
        }

        /*!
          \brief It sets the number of rings in this curve polygon.

          If the new size is less than the old it will drop the geometries.

          \param size The new number of rings for the curve polygon.

          \note TerraLib extended method.
        */
        void setNumRings(std::size_t size);

        /*!
          \brief It returns the n-th interior ring for this curve polygon as a curve.

          \param i The ring index.

          \note The interior ring index start at 0.

          \note It doesn't check the index range.
        */
        Curve* getInteriorRingN(std::size_t i) const;

        /*!
          \brief It returns the n-th ring for this curve polygon as a curve.

          \param i The ring index.

          \return The n-th ring.

          \note The ring index start at 0.

          \note It doesn't check the index range.

          \note TerraLib extended method.
        */
        Curve* getRingN(std::size_t i) const
        {
          assert(i < m_rings.size());
          return m_rings[i];
        }

        /*!
          \brief It returns the n-th ring.

          \param i The coordinate index.

          \return The n-th ring.

          \note The ring index start at 0.
        */
        Curve* operator[](std::size_t i) const
        {
          assert(i < m_rings.size());
          return m_rings[i];
        }

        /*!
          \brief It returns the n-th ring.

          \param i The coordinate index.

          \note The ring index start at 0.

          \return The n-th ring.
        */
        Curve* operator[](std::size_t i)
        {
          assert(i < m_rings.size());
          return m_rings[i];
        }

        /*!
          \brief It sets the informed position ring to the new one.

          \param i The ring index.
          \param r The new ring to be placed in the informed position.

          \note The ring index start at 0.

          \note If the informed position contains a ring, it will be released.

          \note It doesn't check the index range.

          \note TerraLib extended method.
        */
        void setRingN(std::size_t i, Curve* r);

        /*!
          \brief It removes the n-th ring in this CurvePolygon.

          \param i The index of the ring we want to remove.

          \note The ring index start at 0.

          \note The memory pointed by ring will be released.

          \note It doesn't check the index range.

          \note TerraLib extended method.
        */
        void removeRingN(std::size_t i);

        /*!
          \brief It adds the ring to the curve polygon.

          \param ring The ring to be added.

          \note TerraLib extended method.
        */
        void add(Curve* ring)
        {
          m_rings.push_back(ring);
        }

        /*!
          \brief It adds the curve to the curve polygon.

          \param ring The ring to be added.

          \note TerraLib extended method.
        */
        void push_back(Curve* ring)
        {
          m_rings.push_back(ring);
        }

        /*!
          \brief It deletes all the rings of the CurvePolygon and clear it.

          \note TerraLib extended method.
        */
        void clear();

        /*!
          \brief It returns the polygon rings.

          \return A reference to the list of rings.

          \warning Don't use this method unless you know exactly what you're doing!

          \note TerraLib extended method.
        */
        std::vector<Curve*>& getRings() { return m_rings; }

        /*!
          \brief It returns the polygon rings.

          \return A reference to the list of rings.

          \warning Don't use this method unless you know exactly what you're doing!

          \note TerraLib extended method.
         */
        const std::vector<Curve*>& getRings() const { return m_rings; }

        //@}

        /** @name Surface Specific Methods
         *  Specific methods for a Surface.
         */
        //@{

        /*!
          \brief It returns the area of this surface, as measured in the spatial reference system of this surface.

          \return The area of this surface.
        */
        double getArea() const;

        /*!
          \brief It returns the mathematical centroid for this surface as a point.

          \return The mathematical centroid for this surface.

          \note The caller of this method will take the ownership of the returned point.
          \note The result is not guaranteed to be on this Surface.
        */
        Point* getCentroid() const;

        /*!
          \brief It returns the mathematical centroid for this surface as a coordinate.

          \return The mathematical centroid for this surface.

          \note The caller of this method will take the ownership of the returned coordinate.
          \note The result is not guaranteed to be on this Surface.
          \note TerraLib extended method.
        */
        Coord2D* getCentroidCoord() const;

        /*!
          \brief It returns a point guaranteed to be on this surface.
 
          \return A point guaranteed to be on this surface.

          \note The caller of this method will take the ownership of the returned point.
        */
        Point* getPointOnSurface() const;

        /*!
          \brief It returns a coordinate guaranteed to be on this surface.

          \return A point guaranteed to be on this surface.

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
          \brief The name of the geometry subtype for curve polygons is: CurvePolygon.

          \return The name of the geometry subtype for curve polygons is: CurvePolygon.
        */
        virtual const std::string& getGeometryType() const throw();

        /*!
          \brief It sets the Spatial Reference System ID of the geometry and all its parts if it is a GeometryCollection (or a Multi).

          \param srid The Spatial Reference System ID to be associated to the geometric object.

          \note This method just set the srid, it doesn't perform conversions over coordinate values.

          \note TerraLib extended method.
        */
        void setSRID(int srid) throw();

        /*!
          \brief It converts the coordinate values of the geometry to the new spatial reference system.

          After calling this method the geometry will be associated to the new SRID.

          \param srid The new Spatial Reference System ID used to transform the coordinates of the geometry.

          \exception Exception It will throw an exception if it can not do the transformation.

          \note The geometry must be associated to a valid SRID before calling this method.

          \note If the geometry already has an associated MBR, this method will automatically update it (i. e. automatically recompute it).
        */
        void transform(int srid) throw(te::common::Exception);

        /*!
          \brief It computes the minimum bounding rectangle for the curve polygon.

          \param cascade If true, it will update the MBR of its parts.

          \note You can use this method in order to update the MBR of the curve polygon.
          .
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

      private:

        std::vector<Curve*> m_rings;     //!< An array with the ring list.

        static const std::string sm_typeName; //! Geometry type name for CurvePolygon.
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_CURVEPOLYGON_H




