/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/query/FunctionNames.h

  \brief A static class with global function name definitions.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_FUNCTIONNAMES_H
#define __TERRALIB_DATAACCESS_INTERNAL_FUNCTIONNAMES_H

// TerraLib
#include "../../common/Static.h"
#include "../Config.h"

// STL
#include <string>

namespace te
{
  namespace da
  {
    /*!
      \class FunctionNames
      
      \brief A static class with global function name definitions.
    */
    class TEDATAACCESSEXPORT FunctionNames : public te::common::Static
    {
      public:

// logical operators
        static const std::string sm_Not;
        static const std::string sm_And;
        static const std::string sm_Or;

// relational operators
        static const std::string sm_EqualTo;
        static const std::string sm_NotEqualTo;
        static const std::string sm_GreaterThan;
        static const std::string sm_GreaterThanOrEqualTo;
        static const std::string sm_LessThan;
        static const std::string sm_LessThanOrEqualTo;

// arithmetic operators
        static const std::string sm_Add;
        static const std::string sm_Sub;
        static const std::string sm_Mul;
        static const std::string sm_Div;

// topological operators
        static const std::string sm_ST_Area;
        static const std::string sm_ST_Beyond;
        static const std::string sm_ST_Boundary;
        static const std::string sm_ST_Centroid;
        static const std::string sm_ST_Collect;
        static const std::string sm_ST_Contains;
        static const std::string sm_ST_ConvexHull;
        static const std::string sm_ST_Crosses;
        static const std::string sm_ST_Difference;
        static const std::string sm_ST_Disjoint;
        static const std::string sm_ST_Dump;
        static const std::string sm_ST_DumpRings;
        static const std::string sm_ST_DWithin;
        static const std::string sm_ST_Envelope;
        static const std::string sm_ST_EnvelopeIntersects;
        static const std::string sm_ST_Equals;
        static const std::string sm_ST_Intersection;
        static const std::string sm_ST_Intersects;
        static const std::string sm_ST_Length;
        static const std::string sm_ST_NumGeometries;
        static const std::string sm_ST_Overlaps;
        static const std::string sm_ST_Perimeter;
        static const std::string sm_ST_Relate;
        static const std::string sm_ST_Touches;
        static const std::string sm_ST_Union;
        static const std::string sm_ST_Within;

// srs operators
        static const std::string sm_ST_Transform;

// string functions
        static const std::string sm_Substring;
        static const std::string sm_Like;

// aggregate
        static const std::string sm_Sum;

//statistical funcations
        static const std::string sm_Avg;
        static const std::string sm_Count;
        static const std::string sm_Max;
        static const std::string sm_Min;
        static const std::string sm_StdDev;
        static const std::string sm_Variance;

// others
        static const std::string sm_IsNull;
        static const std::string sm_In;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_FUNCTIONNAMES_H

