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
  \file terralib/geometry/MultiLineString.h

  \brief MultiLineString is a MultiCurve whose elements are LineStrings.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_MULTILINESTRING_H
#define __TERRALIB_GEOMETRY_INTERNAL_MULTILINESTRING_H

// TerraLib
#include "MultiCurve.h"

namespace te
{
  namespace gm
  {
    /*!
      \class MultiLineString
      
      \brief MultiLineString is a MultiCurve whose elements are LineStrings.

      \ingroup geometry

      \sa Geometry,
          AbstractPoint, Point, PointM, PointZ, PointZM, PointKd,
          Curve, LineString, LinearRing, Line, CircularString, CompoundCurve, 
          Surface, Polygon, Triangle, CurvePolygon, PolyhedralSurface, TIN,
          GeometryCollection, MultiCurve, MultiSurface
          MultiPoint, MultiPolygon

    */
    class TEGEOMEXPORT MultiLineString : public MultiCurve
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer methods on geometric objects
         *  Methods for initializing a geometric object.
         */
        //@{

        /*!
          \brief It initializes the multi line string with the specified spatial reference system id and envelope.
       
          \param nGeom The number of geometries in the collection.
          \param t     The internal type of the multi line string.
          \param srid  The Spatial Reference System ID associated to the multi line string.
          \param mbr   The minimum bounding rectangle of this geometry (i.e., its envelope).

          \note The multi line string will take the ownership of the given mbr.
        */
        MultiLineString(std::size_t nGeom, GeomType t, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief Copy constructor.

          \param rhs The other geometry.
        */
        MultiLineString(const MultiLineString& rhs);

        /*! \brief Destructor. */
        ~MultiLineString() { }

        /*!
          \brief Assignment operator.

          \param rhs The other geometry.

          \return A reference for this.
        */
        MultiLineString& operator=(const MultiLineString& rhs);

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
        te::dt::AbstractData* clone() const;

        //@}

        /** @name Re-Implmentation of methods from Geometry class
         *  Re-Implmentation of basic methods from Geometry class.
         */
        //@{

        /*!
          \brief The name of the Geometry subtype is: MultiLineString.

          \return The name of the Geometry subtype is: MultiLineString.
        */
        const std::string& getGeometryType() const throw();

        /*!
          \brief It returns a derived geometry collection value according to the range of coordinate values inclusively.

          \param mStart The initial coordinate value.
          \param mEnd   The final coordinate value.

          \return A GeometryCollection value.

          \exception Exception It will throw an exception if the operation could not be performed.

          \note This method only applies to Point and Line geometries, including homogeneous collections of points or lines.
                For polygons this will return a NULL value.

          \note The caller of this method will take the ownership of the returned Geometry.
        */
        Geometry* locateBetween(const double& mStart, const double& mEnd) const throw(Exception);

        //@}

      private:

        static const std::string sm_typeName; //!< Geometry type name for MultiLineString.
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_MULTILINESTRING_H

