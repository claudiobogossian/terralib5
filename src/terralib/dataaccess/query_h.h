/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file query_h.h

  \brief This file contains include headers for the Query module.
 */

#ifndef __TERRALIB_DATAACCESS_INTERNAL_QUERY_H_H
#define __TERRALIB_DATAACCESS_INTERNAL_QUERY_H_H

// TerraLib
#include "query/Add.h"
#include "query/And.h"
#include "query/Avg.h"
#include "query/BinaryFunction.h"
#include "query/BinaryOp.h"
#include "query/BinaryOpEncoder.h"
#include "query/Count.h"
#include "query/DataSetName.h"
#include "query/Distinct.h"
#include "query/Div.h"
#include "query/EqualTo.h"
#include "query/Expression.h"
#include "query/Field.h"
#include "query/Fields.h"
#include "query/From.h"
#include "query/FromFunctionCall.h"
#include "query/FromItem.h"
#include "query/Function.h"
#include "query/FunctionCatalog.h"
#include "query/FunctionCatalogManager.h"
#include "query/FunctionDefn.h"
#include "query/FunctionEncoder.h"
#include "query/FunctionNames.h"
#include "query/FunctionNoArgsEncoder.h"
#include "query/FunctionParameter.h"
#include "query/GreaterThan.h"
#include "query/GreaterThanOrEqualTo.h"
#include "query/GroupBy.h"
#include "query/GroupByItem.h"
#include "query/Having.h"
#include "query/In.h"
#include "query/IsNull.h"
#include "query/Join.h"
#include "query/JoinCondition.h"
#include "query/JoinConditionOn.h"
#include "query/JoinConditionUsing.h"
#include "query/LessThan.h"
#include "query/LessThanOrEqualTo.h"
#include "query/Like.h"
#include "query/Literal.h"
#include "query/LiteralBool.h"
#include "query/LiteralByteArray.h"
#include "query/LiteralDateTime.h"
#include "query/LiteralDouble.h"
#include "query/LiteralEnvelope.h"
#include "query/LiteralGeom.h"
#include "query/LiteralInt16.h"
#include "query/LiteralInt32.h"
#include "query/LiteralInt64.h"
#include "query/LiteralString.h"
#include "query/Max.h"
#include "query/Min.h"
#include "query/Mul.h"
#include "query/Not.h"
#include "query/NotEqualTo.h"
#include "query/Or.h"
#include "query/OrderBy.h"
#include "query/OrderByItem.h"
#include "query/PropertyName.h"
#include "query/Query.h"
#include "query/QueryVisitor.h"
#include "query/Select.h"
#include "query/SQLDialect.h"
#include "query/SQLFunctionEncoder.h"
#include "query/SQLVisitor.h"
#include "query/ST_Area.h"
#include "query/ST_Beyond.h"
#include "query/ST_Boundary.h"
#include "query/ST_Buffer.h"
#include "query/ST_Centroid.h"
#include "query/ST_Contains.h"
#include "query/ST_ConvexHull.h"
#include "query/ST_Crosses.h"
#include "query/ST_Difference.h"
#include "query/ST_Disjoint.h"
#include "query/ST_DistanceBuffer.h"
#include "query/ST_Dump.h"
#include "query/ST_DumpRings.h"
#include "query/ST_DWithin.h"
#include "query/ST_Equals.h"
#include "query/ST_Intersection.h"
#include "query/ST_Intersects.h"
#include "query/ST_IsValid.h"
#include "query/ST_Length.h"
#include "query/ST_NumGeometries.h"
#include "query/ST_Overlaps.h"
#include "query/ST_Perimeter.h"
#include "query/ST_Relate.h"
#include "query/ST_Touches.h"
#include "query/ST_Transform.h"
#include "query/ST_Union.h"
#include "query/ST_Within.h"
#include "query/StdDev.h"
#include "query/Sub.h"
#include "query/SubSelect.h"
#include "query/Substring.h"
#include "query/Sum.h"
#include "query/TemplateEncoder.h"
#include "query/UnaryFunction.h"
#include "query/UnaryOp.h"
#include "query/UnaryOpEncoder.h"
#include "query/Variance.h"
#include "query/Where.h"

#endif  // __TERRALIB_DATAACCESS_INTERNAL_QUERY_H_H

