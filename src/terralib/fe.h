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
  \file fe.h
  
  \brief This file contains include headers for TerraLib Filter Encoding module.
 */
#ifndef __TERRALIB_INTERNAL_FE_H
#define __TERRALIB_INTERNAL_FE_H

// TerraLib
#include "fe/AbstractOp.h"
#include "fe/ArithmeticOperators.h"
#include "fe/BBOXOp.h"
#include "fe/BinaryComparisonOp.h"
#include "fe/BinaryLogicOp.h"
#include "fe/BinaryOperator.h"
#include "fe/BinarySpatialOp.h"
#include "fe/ComparisonOperators.h"
#include "fe/ComparisonOp.h"
#include "fe/Config.h"
#include "fe/DistanceBuffer.h"
#include "fe/Expression.h"
#include "fe/Filter.h"
#include "fe/FilterCapabilities.h"
#include "fe/Globals.h"
#include "fe/Visitor.h"
#include "fe/Function.h"
#include "fe/FunctionName.h"
#include "fe/GeometryOperands.h"
#include "fe/IdCapabilities.h"
#include "fe/Literal.h"
#include "fe/LogicOp.h"
#include "fe/ObjectId.h"
#include "fe/PropertyIsBetween.h"
#include "fe/PropertyIsLike.h"
#include "fe/PropertyIsNull.h"
#include "fe/PropertyName.h"
#include "fe/ScalarCapabilities.h"
#include "fe/SortBy.h"
#include "fe/SortProperty.h"
#include "fe/SpatialCapabilities.h"
#include "fe/SpatialOp.h"
#include "fe/SpatialOperators.h"
#include "fe/UnaryLogicOp.h"

/*!
  \defgroup fe Filter Encoding

  \brief This module implements the OGC Filter Encoding specification.

    A Filter is any valid predicate expression that can
    be formed using the elements defined in the OGC Filter Encoding specification.
    Most of the documentation used in Filter classes were extracted
    from the specificaton available at: http://www.opengeospatial.org/standards/filter.

    The main classes/concepts in this module are listed here. The namespace associated to the Filter Encoding module is te::fe.
    To know more about it, see the te::fe namespace documentation.
*/

namespace te
{
  /*!
    \brief Namespace for the Filter Encoding module of TerraLib.
  */
  namespace fe
  {
  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_INTERNAL_FE_H
