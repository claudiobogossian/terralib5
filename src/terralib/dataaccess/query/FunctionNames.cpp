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
  \file terralib/dataaccess/query/FunctionNames.cpp

  \brief A static class with global function name definitions.
*/

// TerraLib
#include "FunctionNames.h"

const std::string te::da::FunctionNames::sm_Not("not");
const std::string te::da::FunctionNames::sm_And("and");
const std::string te::da::FunctionNames::sm_Or("or");
const std::string te::da::FunctionNames::sm_EqualTo("=");
const std::string te::da::FunctionNames::sm_NotEqualTo("<>");
const std::string te::da::FunctionNames::sm_GreaterThan(">");
const std::string te::da::FunctionNames::sm_GreaterThanOrEqualTo(">=");
const std::string te::da::FunctionNames::sm_LessThan("<");
const std::string te::da::FunctionNames::sm_LessThanOrEqualTo("<=");
const std::string te::da::FunctionNames::sm_Add("+");
const std::string te::da::FunctionNames::sm_Sub("-");
const std::string te::da::FunctionNames::sm_Mul("*");
const std::string te::da::FunctionNames::sm_Div("/");
const std::string te::da::FunctionNames::sm_ST_Area("st_area");
const std::string te::da::FunctionNames::sm_ST_Equals("st_equals");
const std::string te::da::FunctionNames::sm_ST_Disjoint("st_disjoint");
const std::string te::da::FunctionNames::sm_ST_DWithin("st_dwithin");
const std::string te::da::FunctionNames::sm_ST_Beyond("st_beyond");
const std::string te::da::FunctionNames::sm_ST_Touches("st_touches");
const std::string te::da::FunctionNames::sm_ST_Within("st_within");
const std::string te::da::FunctionNames::sm_ST_Overlaps("st_overlaps");
const std::string te::da::FunctionNames::sm_ST_Crosses("st_crosses");
const std::string te::da::FunctionNames::sm_ST_Centroid("st_centroid");
const std::string te::da::FunctionNames::sm_ST_ConvexHull("st_convexhull");
const std::string te::da::FunctionNames::sm_ST_Contains("st_contains");
const std::string te::da::FunctionNames::sm_ST_Difference("st_difference");
const std::string te::da::FunctionNames::sm_ST_Envelope("st_eenvelope");
const std::string te::da::FunctionNames::sm_ST_Intersection("st_intersection");
const std::string te::da::FunctionNames::sm_ST_Intersects("st_intersects");
const std::string te::da::FunctionNames::sm_ST_Length("st_length");
const std::string te::da::FunctionNames::sm_ST_EnvelopeIntersects("st_envelopeintersects");
const std::string te::da::FunctionNames::sm_ST_Perimeter("st_perimeter");
const std::string te::da::FunctionNames::sm_ST_Relate("st_relate");
const std::string te::da::FunctionNames::sm_ST_Union("st_union");
const std::string te::da::FunctionNames::sm_ST_Transform("st_transform");
const std::string te::da::FunctionNames::sm_Substring("substring");
const std::string te::da::FunctionNames::sm_Like("like");
const std::string te::da::FunctionNames::sm_Sum("sum");
const std::string te::da::FunctionNames::sm_Avg("avg");
const std::string te::da::FunctionNames::sm_Max("max");
const std::string te::da::FunctionNames::sm_Min("min");
const std::string te::da::FunctionNames::sm_StdDev("stddev");
const std::string te::da::FunctionNames::sm_Variance("variance");
const std::string te::da::FunctionNames::sm_Count("count");
const std::string te::da::FunctionNames::sm_IsNull("isnull");
const std::string te::da::FunctionNames::sm_In("in");
