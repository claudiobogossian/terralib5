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
  \file terralib/dataaccess/query_fw.h
   
  \brief Forward declarations for the Query module.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_QUERY_FW_H
#define __TERRALIB_DATAACCESS_INTERNAL_QUERY_FW_H

// Forward declarations
namespace te
{
  namespace da
  {
    //class AbstractFunction;
    class Add;
    class And;
    class Avg;
    class BinaryFunction;
    class BinaryOp;
    class BinaryOpEncoder;
    class Count;
    class DataSetName;
    class Div;
    class EqualTo;
    class Expression;
    class Field;
    class FromFunctionCall;
    class FromItem;
    class Function;
    class FunctionCatalog;
    class FunctionCatalogManager;
    class FunctionDefn;
    class FunctionEncoder;
    class FunctionNames;
    class FunctionNoArgsEncoder;
    class FunctionParameter;
    class GreaterThan;
    class GreaterThanOrEqualTo;
    class GroupByItem;
    class Having;
    class In;
    class IsNull;
    class Join;
    class JoinCondition;
    class JoinConditionOn;
    class JoinConditionUsing;
    class LessThan;
    class LessThanOrEqualTo;
    class Like;
    class Literal;
    class LiteralBool;
    class LiteralByteArray;
    class LiteralDateTime;
    class LiteralDouble;
    class LiteralEnvelope;
    class LiteralGeom;
    class LiteralInt16;
    class LiteralInt32;
    class LiteralInt64;
    class LiteralString;
    class Max;
    class Min;
    class Mul;
    class Not;
    class NotEqualTo;
    class Or;
    class OrderByItem;
    class PropertyName;
    class Query;
    class QueryVisitor;
    class Select;
    class SQLDialect;
    class SQLFunctionEncoder;
    class SQLVisitor;
    class ST_Area;
    class ST_Beyond;
    class ST_Boundary;
    class ST_Buffer;
    class ST_Centroid;
    class ST_Contains;
    class ST_ConvexHull;
    class ST_Crosses;
    class ST_Difference;
    class ST_Disjoint;
    class ST_DistanceBuffer;
    class ST_Dump;
    class ST_DumpRings;
    class ST_DWithin;
    class ST_Equals;
    class ST_Intersection;
    class ST_Intersects;
    class ST_Length;
    class ST_NumGeometries;
    class ST_Overlaps;
    class ST_Perimeter;
    class ST_Relate;
    class ST_Touches;
    class ST_Transform;
    class ST_Union;
    class ST_Within;
    class StdDev;
    class Sub;
    class SubSelect;
    class Substring;
    class Sum;
    class TemplateEncoder;
    class UnaryFunction;
    class UnaryOp;
    class UnaryOpEncoder;
    class Variance;
    class Where;
  }    // end namespace da
}      // end namespace te

#endif // __TERRALIB_DATAACCESS_INTERNAL_QUERY_FW_H
