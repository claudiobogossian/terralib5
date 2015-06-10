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
  \file terralib/geometry/CompoundCurve.h

  \brief CompoundCurve is a curve that may have circular and linear segments.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_COMPOUNDCURVE_H
#define __TERRALIB_GEOMETRY_INTERNAL_COMPOUNDCURVE_H

// TerraLib
#include "Curve.h"

// STL
#include <vector>

namespace te
{
  namespace gm
  {
    /*!
      \class CompoundCurve

      \brief CompoundCurve is a curve that may have circular and linear segments.

      \ingroup geometry

      \sa Geometry,
          AbstractPoint, Point, PointM, PointZ, PointZM, PointKd,
          Curve, Line, LineString, LinearRing, CircularString,
          Surface, Polygon, Triangle, CurvePolygon, PolyhedralSurface, TIN,
          GeometryCollection, MultiSurface, MultiCurve,
          MultiPoint, MultiLineString, MultiPolygon
    */
    class TEGEOMEXPORT CompoundCurve : public Curve
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer methods on geometric objects
         *  Methods for initializing a geometric object.
         */
        //@{

        /*!
          \brief It initializes the compound curve with the specified spatial reference system id and envelope.
       
          \param t    The internal type of the compound curve.
          \param srid The Spatial Reference System ID associated to the compound curve.
          \param mbr  The minimum bounding rectangle of this geometry (i.e., its envelope).

          \note The compound curve will take the ownership of the given mbr.
        */
        CompoundCurve(GeomType t, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief It initializes the compound curve with the specified spatial reference system id and envelope.
       
          \param size The number of elements in the CompoundCurve. It must be a value greater than 0.
          \param t    The internal type of the compound curve.
          \param srid The Spatial Reference System ID associated to the compound curve.
          \param mbr  The minimum bounding rectangle of this geometry (i.e., its envelope).

          \note The compound curve will take the ownership of the given mbr.
        */
        CompoundCurve(std::size_t size, GeomType t, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief Copy constructor.

          \param rhs The other geometry.
        */
        CompoundCurve(const CompoundCurve& rhs);

        /*! \brief Virtual destructor. */
        ~CompoundCurve();

        /*!
          \brief Assignment operator.

          \param rhs The other geometry.

          \return A reference for this.
        */
        CompoundCurve& operator=(const CompoundCurve& rhs);

        //@}

        /** @name Re-Implementation from AbstractData
         *  Methods re-Implementated from AbstractData.
         */
        //@{

        /*!
          \brief It clones the compound curve.

          \return A copy of the given compound curve.

          \note The caller of this method will take the ownership of the returned compound curve.

          \note The cloned compound curve will not have the
                MBR computed. This will save time when you
                are just cloning a geometry and don't intend
                to waste time computing the bounding box.
                If you have another suggestion, please, let me know.
        */
        te::dt::AbstractData* clone() const;

        //@}

        /** @name Re-Implmentation of methods from Geometry class
         *  Re-Implmentation of basic methods from Geometry class.
         */
        //@{

        /*!
          \brief The name of instantiable subtype is: CompoundCurve.

          \return The name of instantiable subtype is: CompoundCurve.
        */
        const std::string& getGeometryType() const throw();

        /*!
          \brief It sets the Spatial Reference System ID of the compound curve.

          \param srid The Spatial Reference System ID to be associated to the compound curve.

          \note TerraLib extended method.
        */
        void setSRID(int srid) throw();

        /*!
          \brief It converts the coordinate values of the compound curve to the new spatial reference system.

          After calling this method the compound curve will be associated to the new SRID.

          \param srid The new Spatial Reference System ID used to transform the coordinates of the compound curve.

          \exception Exception It will throw an exception if it can not do the transformation.

          \note The compound curve must be associated to a valid SRID before calling this method.

          \note If the compound curve already has an associated MBR, this method will automatically update it (i. e. automatically recompute it).
        */
        void transform(int srid) throw(te::common::Exception);

        /*!
          \brief It computes the minimum bounding rectangle for the compound curve.

          \param cascade For compound curve this flag doesn't have effect.

          \note You can use this method in order to update the MBR of the compound curve.

          \note TerraLib extended method.
        */
        void computeMBR(bool cascade) const throw();

        /*!
          \brief It returns the number of points (vertexes) in the compound curve.

          \return The number of points (vertexes) in the compound curve.

          \note TerraLib extended method.
        */
        std::size_t getNPoints() const throw();

        /*!
          \brief It returns a derived geometry collection value according to the range of coordinate values inclusively.

          \param mStart The initial coordinate value.
          \param mEnd   The final coordinate value.

          \return A GeometryCollection value.

          \note The caller of this method will take the ownership of geometry.
        */
        Geometry* locateBetween(const double& mStart, const double& mEnd) const throw(Exception);

        //@}

        /** @name Re-Implementation from Curve
         *  Methods re-impleented from Curve.
         */
        //@{

        /*!
          \brief The length of this Curve in its associated spatial reference.

          \return The length of this Curve in its associated spatial reference.
        */
        double getLength() const;

        /*!
          \brief It returns the curve start point.

          \return The curve start point.

          \note The caller of this method will take the ownership of the point geometry.
        */
        Point* getStartPoint() const;

        /*!
          \brief It returns the curve end point.

          \return The curve end point.

          \note The caller of this method will take the ownership of the point geometry.
        */
        Point* getEndPoint() const;

        /*!
          \brief It returns true if the curve is closed (startPoint = endPoint).

          \return True if the curve is closed (startPoint = endPoint).

          \warning The line must have at least 2 points.
        */
        bool isClosed() const;

        //@}

        /** @name CompoundCurve Specific Methods
         *  Specific methods for a CompoundCurve.
         */
        //@{

        /*!
          \brief It returns the number of elements in the compound geometry.

          \return The number of elements in the compound geometry.

          \note TerraLib extended method.
        */
        std::size_t size() const { return m_curves.size(); }

        /*!
          \brief It clears all the segments.

          \note TerraLib extended method.
        */
        void makeEmpty();

        /*!
          \brief It returns the i-th curve.

          \return The i-th curve.
        */
        Curve* getCurve(std::size_t i) const;

        /*!
          \brief It adds the curve to the compound.

          \param c The curve to be added to the compound. The compound will take its ownership.
        */
        void add(Curve* c);

        /*!
          \brief It returns a pointer to the internal array of segments.

          \return A pointer to the internal array of segments.

          \warning Don't use this method unless you know exactly what you're doing!

          \note TerraLib extended method.
        */
        const std::vector<Curve*>& getCurves() const
        {
          return m_curves;
        }

        /*!
          \brief It returns a pointer to the internal array of segments.

          \return A pointer to the internal array of segments.

          \warning Don't use this method unless you know exactly what you're doing!

          \note TerraLib extended method.
        */
        std::vector<Curve*>& getCurves()
        {
          return m_curves;
        }

        //@}

      protected:
        
        std::vector<Curve*> m_curves;   //!< The list of segments of the compund curve.

      private:

        static const std::string sm_typeName; //! Geometry type name for CompoundCurve.
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_COMPOUNDCURVE_H

