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
  \file terralib/geometry/WKBWriter.h

  \brief A class that serializes a geometry to the WKB format.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_WKBWRITER_H
#define __TERRALIB_GEOMETRY_INTERNAL_WKBWRITER_H

// TerraLib
#include "../common/Enums.h"
#include "Visitor.h"

namespace te
{
  namespace gm
  {
    /*!
      \class WKBWriter
      
      \brief A class that serializes a geometry to the WKB format.

      \ingroup geometry

      \sa WKBReader
    */
    class TEGEOMEXPORT WKBWriter : public Visitor
    {
      public:

        /*!
          \brief It constructs a new WKB writer.

          \param wkb       A pointer to a buffer to output the wkb. It must have at least the size in bytes necessary to write the WKB.
          \param byteOrder The byte order to be used to encode the WKB.
        */
        WKBWriter(char* wkb, te::common::MachineByteOrder byteOrder = te::common::NDR);

        /*! \brief Destructor. */
        ~WKBWriter();

        /*!
          \brief It serializes the geometry to a WKB representation into the specified buffer.

          \param geom The geometry to be serialized.

          \note You must call reset in successive calls to this method.
        */
        void write(const Geometry* geom);

        /*!
          \brief It serializes the geometry to a WKB representation into the specified buffer.

          The wkb parameter must have at least geom.getWkbSize() in order to be used. Don't
          pass a NULL pointer or a buffer smaller than the size needed. Note that
          the WKB will be on the specified byte order.

          \param geom      The geometry to be serialized.
          \param wkb       The buffer where the geometry will be serialized.
          \param byteOrder The byte order used to store/serialize the geometry.
        */
        static void write(const Geometry* geom, char* wkb, te::common::MachineByteOrder byteOrder = te::common::NDR);

        /*!
          \brief It resets the WKB writer an allows a new geometry to be seriealized to a new buffer.

          \param wkb       A pointer to a buffer to output the wkb. It must have at least the size in bytes necessary to write the WKB.
          \param byteOrder The byte order to be used to encode the WKB.
        */
        void reset(char* wkb, te::common::MachineByteOrder byteOrder = te::common::NDR);

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

        char* m_wkb;                                //!< A pointer that marks the current position in a buffer to be write when parsing the geometry.
        te::common::MachineByteOrder m_byteOrder;   //!< The byte order used to encode the WKB.
    };

  } // namespace gm
}   // namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_WKBWRITER_H
