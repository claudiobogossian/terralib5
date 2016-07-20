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
  \file terralib/sqlite/EWKBSize.h

  \brief A class that helps to determine the size of a SpatiaLite geometry.
*/

#ifndef __TERRALIB_SQLITE_INTERNAL_EWKBSIZE_H
#define __TERRALIB_SQLITE_INTERNAL_EWKBSIZE_H

// TerraLib
#include "../common/Enums.h"
#include "../geometry/Visitor.h"

namespace te
{
  namespace sqlite
  {
    class EWKBSize : public te::gm::Visitor
    {
      public:

        EWKBSize();

        ~EWKBSize();

        static std::size_t getEWKBSize(const te::gm::Geometry* g);

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
        void visit(const te::gm::Triangle& /*visited*/) { }
        void visit(const te::gm::CircularString& /*visited*/) {}
        void visit(const te::gm::CompoundCurve& /*visited*/) {}
        void visit(const te::gm::CurvePolygon& /*visited*/) {}

      private:

        std::size_t m_size;
    };

  } // namespace sqlite
}   // namespace te

#endif  // __TERRALIB_SQLITE_INTERNAL_EWKBSIZE_H
