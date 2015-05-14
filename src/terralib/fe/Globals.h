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
  \file terralib/fe/Globals.h
   
  \brief An static class with global definitions.

  \ingroup fe
 */

#ifndef __TERRALIB_FE_INTERNAL_GLOBAL_H
#define __TERRALIB_FE_INTERNAL_GLOBAL_H

// TerraLib
#include "Config.h"

namespace te
{
  namespace fe
  {
    /*!
      \class Globals

      \brief An static class with global definitions.
     */
    class TEFEEXPORT Globals
    {
      private:

        /*! \brief Not instantiable. */
        Globals();

      public:

        static const char* sm_propertyIsEqualTo;
        static const char* sm_propertyIsNotEqualTo;
        static const char* sm_propertyIsLessThan;
        static const char* sm_propertyIsGreaterThan;
        static const char* sm_propertyIsLessThanOrEqualTo;
        static const char* sm_propertyIsGreaterThanOrEqualTo;
        static const char* sm_propertyIsLike;
        static const char* sm_propertyIsNull;
        static const char* sm_propertyIsBetween;

        static const char* sm_and;
        static const char* sm_or;

        static const char* sm_not;

        static const char* sm_add;
        static const char* sm_sub;
        static const char* sm_mul;
        static const char* sm_div;

        static const char* sm_bbox;

        static const char* sm_equals;
        static const char* sm_disjoint;
        static const char* sm_touches;
        static const char* sm_within;
        static const char* sm_overlaps;
        static const char* sm_crosses;
        static const char* sm_intersects;
        static const char* sm_contains;

        static const char* sm_dWithin;
        static const char* sm_beyond;

        static const char* sm_lessThan;
        static const char* sm_greaterThan;
        static const char* sm_lessThanEqualTo;
        static const char* sm_greaterThanEqualTo;
        static const char* sm_equalTo;
        static const char* sm_notEqualTo;
        static const char* sm_like;
        static const char* sm_between;
        static const char* sm_nullCheck;

        static const char* sm_envelope;
        static const char* sm_point;
        static const char* sm_lineString;
        static const char* sm_polygon;
        static const char* sm_arcByCenterPoint;
        static const char* sm_circleByCenterPoint;
        static const char* sm_arc;
        static const char* sm_circle;
        static const char* sm_arcByBulge;
        static const char* sm_bezier;
        static const char* sm_clothoid;
        static const char* sm_cubicSpline;
        static const char* sm_geodesic;
        static const char* sm_offsetCurve;
        static const char* sm_triangle;
        static const char* sm_polyhedralSurface;
        static const char* sm_triangulatedSurface;
        static const char* sm_tin;
        static const char* sm_solid;
    };

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_GLOBAL_H

