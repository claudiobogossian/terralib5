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
  \file fe_fw.h
   
  \brief Forward declarations for the Filter Encoding module.
 */

#ifndef __TERRALIB_INTERNAL_FE_FW_H
#define __TERRALIB_INTERNAL_FE_FW_H

// Forward declarations
namespace te
{
  namespace fe
  {
    class AbstractOp;
    class ArithmeticOperators;
    class BBOXOp;
    class BinaryComparisonOp;
    class BinaryLogicOp;
    class BinaryOperator;
    class BinarySpatialOp;
    class ComparisonOperators;
    class ComparisonOp;
    class Distance;
    class DistanceBuffer;
    class Expression;
    class Filter;
    class FilterCapabilities;
    class FilterVisitor;
    class Function;
    class FunctionName;
    class GeometryOperands;
    class Globals;
    class IdCapabilities;
    class Literal;
    class LogicOp;
    class ObjectId;
    class ObjectIdList;
    class PropertyIsBetween;
    class PropertyIsLike;
    class PropertyIsNull;
    class PropertyName;
    class ScalarCapabilities;
    class SortProperty;
    class SpatialCapabilities;
    class SpatialOp;
    class SpatialOperator;
    class Visitor;
    class UnaryLogicOp;
  }
}

#endif // __TERRALIB_INTERNAL_FE_FW_H

