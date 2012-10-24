/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/sqlite/EWKBWriter.h

  \brief A class that serializes a geometry to the SpatiaLite EWKB format.
*/

#ifndef __TERRALIB_SQLITE_INTERNAL_EWKBWRITER_H
#define __TERRALIB_SQLITE_INTERNAL_EWKBWRITER_H

// TerraLib
#include "../common/Enums.h"
#include "../geometry/Visitor.h"
#include "Config.h"

#ifdef TE_ENABLE_SPATIALITE

namespace te
{
  namespace sqlite
  {
    /*!
      \class EWKBWriter
      
      \brief A class that serializes a geometry to the SpatiaLite EWKB format.

      \sa EWKBReader
    */
    class TESQLITEEXPORT EWKBWriter : public te::gm::Visitor
    {
      public:

        /*!
          \brief It constructs a new EWKB writer.

          \param ewkb      A pointer to a buffer to output the ewkb. It must have at least the size in bytes necessary to write the EWKB.
          \param byteOrder The byte order to be used to encode the EWKB.
        */
        EWKBWriter(unsigned char* ewkb, te::common::MachineByteOrder byteOrder = te::common::NDR);

        /*! \brief Destructor. */
        ~EWKBWriter();

        /*!
          \brief It serializes the geometry to a EWKB representation into the specified buffer.

          \param geom The geometry to be serialized.

          \note You must call reset in successive calls to this method.
        */
        void write(const te::gm::Geometry* geom);

        /*!
          \brief It serializes the geometry to a EWKB representation into the specified buffer.

          The ewkb parameter must have at least geom.getWkbSize() in order to be used. Don't
          pass a NULL pointer or a buffer smaller than the size needed. Note that
          the WKB will be on the specified byte order.

          \param geom      The geometry to be serialized.
          \param ewkb      The buffer where the geometry will be serialized.
          \param byteOrder The byte order used to store/serialize the geometry.
        */
        static void write(const te::gm::Geometry* geom, unsigned char* ewkb, te::common::MachineByteOrder byteOrder = te::common::NDR);

        /*!
          \brief It resets the WKB writer an allows a new geometry to be seriealized to a new buffer.

          \param ewkb       A pointer to a buffer to output the wkb. It must have at least the size in bytes necessary to write the WKB.
          \param byteOrder The byte order to be used to encode the WKB.
        */
        void reset(unsigned char* ewkb, te::common::MachineByteOrder byteOrder = te::common::NDR);

      protected:

        void visit(const te::gm::Curve& /*visited*/) {}
        void visit(const te::gm::Geometry& /*visited*/) {}
        void visit(const te::gm::GeometryCollection& visited);
        void visit(const te::gm::LinearRing& visited);
        void visit(const te::gm::LineString& visited);
        void visit(const te::gm::MultiCurve& /*visited*/) {}
        void visit(const te::gm::MultiLineString& visited);
        void visit(const te::gm::MultiPoint& visited);
        void visit(const te::gm::MultiPolygon& visited);
        void visit(const te::gm::MultiSurface& /*visited*/) {}
        void visit(const te::gm::AbstractPoint& /*visited*/) {}
        void visit(const te::gm::Point& visited);
        void visit(const te::gm::PointM& visited);
        void visit(const te::gm::PointZ& visited);
        void visit(const te::gm::PointZM& visited);
        void visit(const te::gm::PointKd& /*visited*/) {}
        void visit(const te::gm::Polygon& visited);
        void visit(const te::gm::PolyhedralSurface& /*visited*/) {}
        void visit(const te::gm::Surface& /*visited*/) {}
        void visit(const te::gm::TIN& /*visited*/) {}
        void visit(const te::gm::Triangle& /*visited*/) {}
        void visit(const te::gm::CircularString& /*visited*/) {}
        void visit(const te::gm::CompoundCurve& /*visited*/) {}
        void visit(const te::gm::CurvePolygon& /*visited*/) {}

      private:

        unsigned char* m_ewkb;                      //!< A pointer that marks the current position in a buffer to be write when parsing the geometry.
        te::common::MachineByteOrder m_byteOrder;   //!< The byte order used to encode the WKB.
    };

  } // namespace sqlite
}   // namespace te

#endif  // TE_ENABLE_SPATIALITE

#endif  // __TERRALIB_SQLITE_INTERNAL_EWKBWRITER_H
