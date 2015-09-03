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
  \file terralib/geometry/TIN.h

  \brief TIN (triangulated irregular network) is a PolyhedralSurface consisting only of Triangle patches.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_TIN_H
#define __TERRALIB_GEOMETRY_INTERNAL_TIN_H

// TerraLib
#include "PolyhedralSurface.h"

namespace te
{
  namespace gm
  {
    /*!
      \class TIN
      
      \brief TIN (triangulated irregular network) is a PolyhedralSurface consisting only of Triangle patches.

      \ingroup geometry

      \sa Geometry,
          Point,
          Curve, LineString, LinearRing,
          Surface, Polygon, Triangle, PolyhedralSurface
          GeometryCollection, MultiSurface, MultiCurve,
          MultiPoint, MultiLineString, MultiPolygon
    */
    class TEGEOMEXPORT TIN : public PolyhedralSurface
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer methods on geometric objects
         *  Methods for initializing a geometric object.
         */
        //@{

        /*!
          \brief It initializes the TIN with the specified spatial reference system id and envelope.
       
          \param nPolygons The number of polygons forming the polyhedral.
          \param t         The internal type of the TIN.
          \param srid      The Spatial Reference System ID associated to the Geometry.
          \param mbr       The minimum bounding rectangle of this geometry (i.e., its envelope).

          \note The TIN will take the ownership of the given mbr.
        */
        TIN(std::size_t nPolygons, GeomType t, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief Copy constructor.

          \param rhs The other geometry.
        */
        TIN(const TIN& rhs);

        /*! \brief Destructor. */
        ~TIN() { }

        /*!
          \brief Assignment operator.

          \param rhs The other geometry.

          \return A reference for this.
        */
        TIN& operator=(const TIN& rhs);

        //@}

        /** @name Re-Implmentation of methods from Geometry class
         *  Re-Implmentation of basic methods from Geometry class.
         */
        //@{

        /*!
          \brief It returns the name of the instantiable subtype of Geometry of which this geometric object is an instantiable member.

          The name of the Geometry subtype for PolyhedralSurface is: Tin.

          \return The name of the instantiable subtype of Geometry of which this geometric object is an instantiable member. In this case, it returns Polygon.
         */
        const std::string& getGeometryType() const throw();

        //@}

		/*!
		\brief It sets the n-th triangle in this TIN.

		\param i The index of the given polygon element.
		\param g The polygon that will be placed on the position indicated by index.

		\note It doesn't check the index range.

		\note TerraLib extended method.
		*/
		void setGeometryN(std::size_t i, te::gm::Polygon* g);
		
		/*!
		\brief It adds the triangle into the TIN.

		\param g The geometry you want to add to the TIN.

		\note TerraLib extended method.
		*/
		void add(te::gm::Polygon* g);

      private:

        static const std::string sm_typeName; //!< Geometry type name for TIN.
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_TIN_H

