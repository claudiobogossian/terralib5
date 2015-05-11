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
  \file terralib/geometry/Visitor.h

  \brief A visitor interface for the Geometry hierarchy.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_VISITOR_H
#define __TERRALIB_GEOMETRY_INTERNAL_VISITOR_H

// TerraLib
#include "Config.h"

// Forward declarations
#include "../geometry_fw.h"

namespace te
{
  namespace gm
  {
    /*!
      \class Visitor
      
      \brief A visitor interface for the Geometry hierarchy.
     */
    class TEGEOMEXPORT Visitor
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief Default constructor. */
        Visitor() {}

        /*! \brief Virtual destructor. */
        virtual ~Visitor() {}

        //@}

        /** @name Select
         *  All concrete visitors must implement these methods.
         */
        //@{

        virtual void visit(const Curve& visited) = 0;
        virtual void visit(const Geometry& visited) = 0;        
        virtual void visit(const GeometryCollection& visited) = 0;
        virtual void visit(const LinearRing& visited) = 0;
        virtual void visit(const LineString& visited) = 0;
        virtual void visit(const MultiCurve& visited) = 0;
        virtual void visit(const MultiLineString& visited) = 0;
        virtual void visit(const MultiPoint& visited) = 0;
        virtual void visit(const MultiPolygon& visited) = 0;
        virtual void visit(const MultiSurface& visited) = 0;
        virtual void visit(const AbstractPoint& visited) = 0;
        virtual void visit(const Point& visited) = 0;
        virtual void visit(const PointM& visited) = 0;
        virtual void visit(const PointZ& visited) = 0;
        virtual void visit(const PointZM& visited) = 0;
        virtual void visit(const PointKd& visited) = 0;
        virtual void visit(const Polygon& visited) = 0;
        virtual void visit(const PolyhedralSurface& visited) = 0;        
        virtual void visit(const Surface& visited) = 0;
        virtual void visit(const TIN& visited) = 0;
        virtual void visit(const Triangle& visited) = 0;
        virtual void visit(const CircularString& visited) = 0;
        virtual void visit(const CompoundCurve& visited) = 0;
        virtual void visit(const CurvePolygon& visited) = 0;

        //@}
    };

  } // end namespace gm
}   // end namespace te


#endif  // __TERRALIB_GEOMETRY_INTERNAL_VISITOR_H

