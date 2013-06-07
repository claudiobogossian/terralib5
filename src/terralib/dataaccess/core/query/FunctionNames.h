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
  \file terralib/dataaccess/core/query/FunctionNames.h

  \brief A static class with global function name definitions.
*/

#ifndef __TERRALIB_DATAACCESS_CORE_QUERY_INTERNAL_FUNCTIONNAMES_H
#define __TERRALIB_DATAACCESS_CORE_QUERY_INTERNAL_FUNCTIONNAMES_H

// TerraLib
#include "../../../common/Static.h"
#include "../Config.h"

// STL
#include <string>

namespace te
{
  namespace da
  {
    namespace core
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
          static const std::string sm_Equals;
          static const std::string sm_Disjoint;
          static const std::string sm_DWithin;
          static const std::string sm_Beyond;
          static const std::string sm_Touches;
          static const std::string sm_Within;
          static const std::string sm_Overlaps;
          static const std::string sm_Crosses;
          static const std::string sm_Intersects;
          static const std::string sm_Contains;
          static const std::string sm_ST_Intersects;
          static const std::string sm_ST_Relate;

  // srs operators
          static const std::string sm_ST_Transform;

  // string functions
          static const std::string sm_Substring;
          static const std::string sm_Like;        

  // aggregate
          static const std::string sm_Sum;

  // others
          static const std::string sm_IsNull;
          static const std::string sm_In;
      };

    } // end namespace core
  }   // end namespace da
}     // end namespace te

#endif  // __TERRALIB_DATAACCESS_CORE_QUERY_INTERNAL_FUNCTIONNAMES_H
