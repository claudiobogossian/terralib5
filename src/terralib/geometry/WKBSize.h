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
  \file terralib/geometry/WKBSize.h

  \brief A class that computes the number of bytes necessary to encode a geometry in WKB.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_WKBSIZE_H
#define __TERRALIB_GEOMETRY_INTERNAL_WKBSIZE_H

// TerraLib
#include "Visitor.h"

namespace te
{
  namespace gm
  {

    /*!
      \class WKBSize
      
      \brief A class that computes the number of bytes necessary to encode a geometry in WKB.

      \sa WKBWriter
    */
    class TEGEOMEXPORT WKBSize : public Visitor
    {
      public:

        /*!
          \brief It calculates the number of bytes required to encode the geometry in a WKB format.

          \param geom The geometry to calculate the WKB size.

          \return The number of bytes required to encode the geometry in a WKB format.
        */
        static std::size_t size(const Geometry* geom);        

        /*!
          \brief It calculates the number of bytes required to encode the geometry in a WKB format.

          \param geom The geometry to calculate the WKB size.

          \return The number of bytes required to encode the geometry in a WKB format.
        */
        static std::size_t size(const Geometry& geom);

      protected:

        void visit(const Curve& /*visited*/) {}
        void visit(const Geometry& /*visited*/) {}
        void visit(const GeometryCollection& visited);
        void visit(const LinearRing& visited);
        void visit(const LineString& visited);
        void visit(const MultiCurve& /*visited*/) {}
        void visit(const MultiLineString& visited);
        void visit(const MultiPoint& visited);
        void visit(const MultiPolygon& visited);
        void visit(const MultiSurface& /*visited*/) {}
        void visit(const AbstractPoint& /*visited*/) {}
        void visit(const Point& visited);
        void visit(const PointM& visited);
        void visit(const PointZ& visited);
        void visit(const PointZM& visited);
        void visit(const PointKd& /*visited*/) {}
        void visit(const Polygon& visited);
        void visit(const PolyhedralSurface& visited);        
        void visit(const Surface& /*visited*/) {}
        void visit(const TIN& visited);
        void visit(const Triangle& visited);
        void visit(const CircularString& /*visited*/) {}
        void visit(const CompoundCurve& /*visited*/) {}
        void visit(const CurvePolygon& /*visited*/) {}

      private:

        /*! \brief Not instantiable class. */
        WKBSize();

        /*! \brief Destructor. */
        ~WKBSize();

      private:

        std::size_t m_size; //!< A counter for the number of bytes required to store a given geometry in WKB.
    };

  } // namespace gm
}   // namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_WKBSIZE_H
