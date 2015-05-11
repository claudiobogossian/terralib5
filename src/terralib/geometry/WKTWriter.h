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
  \file terralib/geometry/WKTWriter.h

  \brief A class that serializes a geometry to the WKT format.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_WKTWRITER_H
#define __TERRALIB_GEOMETRY_INTERNAL_WKTWRITER_H

// TerraLib
#include "Visitor.h"

// STL
#include <iosfwd>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace gm
  {
// Forward declaration
    class Geometry;

    /*!
      \class WKTWriter
      
      \brief A class that serializes a geometry to the WKT format.

      \ingroup geometry

      \sa WKTReader
    */
    class TEGEOMEXPORT WKTWriter : public Visitor, public boost::noncopyable
    {
      public:

        /*!
          \brief It constructs a new WKT writer.

          \param o An output stream where we will write the wkt.
        */
        WKTWriter(std::ostream& o);

        /*! \brief Destructor. */
        ~WKTWriter();

        /*!
          \brief It serializes the geometry to a WKT representation.

          \param geom The geometry to be serialized.

          \note You must call reset in successive calls to this method.
        */
        void write(const Geometry* geom);

        /*!
          \brief It serializes the geometry to a WKT representation.

          \param geom The geometry to be serialized.

          \note You must call reset in successive calls to this method.
        */
        void write(const Geometry& geom);

        /*!
          \brief It serializes the geometry to a WKT representation.

          \param geom  The geometry to be serialized.
          \param o     An output stream where we will write the wkt.
        */
        static void write(const Geometry* geom, std::ostream& o);

        /*!
          \brief It serializes the geometry to a WKT representation.

          \param geom  The geometry to be serialized.
          \param o     An output stream where we will write the wkt.
        */
        static void write(const Geometry& geom, std::ostream& o);

      protected:

        virtual void visit(const Curve& /*visited*/) {}
        virtual void visit(const Geometry& /*visited*/) {}
        void visit(const GeometryCollection& visited);
        void visit(const LinearRing& visited);
        void visit(const LineString& visited);
        virtual void visit(const MultiCurve& /*visited*/) {}
        void visit(const MultiLineString& visited);
        void visit(const MultiPoint& visited);
        void visit(const MultiPolygon& visited);
        virtual void visit(const MultiSurface& /*visited*/) {}
        void visit(const AbstractPoint& /*visited*/) {}
        void visit(const Point& visited);
        void visit(const PointM& visited);
        void visit(const PointZ& visited);
        void visit(const PointZM& visited);
        void visit(const PointKd& /*visited*/) {}
        void visit(const Polygon& visited);
        void visit(const PolyhedralSurface& visited);        
        virtual void visit(const Surface& /*visited*/) {}
        void visit(const TIN& visited);
        void visit(const Triangle& visited);
        void visit(const CircularString& /*visited*/) {}
        void visit(const CompoundCurve& /*visited*/) {}
        void visit(const CurvePolygon& /*visited*/) {}

      private:

        std::ostream& m_ostream;  //!< The output stream used to serialize the geometry to WKT.
        bool m_tagged;            //!< This flag controls how geometry are write to the output stream.
    };

  } // namespace gm
}   // namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_WKTWRITER_H
