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
  \file Utils.h
   
  \brief Utility functions for ST module.  
*/

#ifndef __TERRALIB_ST_INTERNAL_UTILS_H
#define __TERRALIB_ST_INTERNAL_UTILS_H

// TerraLib
#include "../datatype/Enums.h"
#include "../datatype/DateTime.h"

// ST
#include "Config.h"

// STL
#include <map>
#include <vector>

// Boost
#include <boost/shared_ptr.hpp>

// Forward declarations
namespace te { namespace dt { class AbstractData; class Property; } }
namespace te { namespace da { class DataSetType; } }

namespace te
{
  namespace st
  {
    // Forward declarations
    class ObservationDataSetInfo;
    class ObservationDataSetType;
    
    /*! 
      \brief An auxiliary function that transform ObservationDataSetInfo into ObservationDataSetType. 
    */   
    TESTEXPORT ObservationDataSetType GetType(const ObservationDataSetInfo& info);
    
    /*! 
      \brief An auxiliary struct to compare two datetime shared pointers 
    */
    struct TESTEXPORT CompareShrDateTime
    {
      bool operator()(const te::dt::DateTimeShrPtr& t1, const te::dt::DateTimeShrPtr& t2) const 
      {
        return t1->operator<(*t2);
      }
    };   

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_UTILS_H
