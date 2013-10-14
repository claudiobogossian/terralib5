/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
    class TimeSeries;
    class ObservationDataSetInfo;
    class ObservationDataSetType;
    class TrajectoryDataSetInfo;
    class TrajectoryDataSetType;
    class TimeSeriesDataSetInfo;
    class TimeSeriesDataSetType;
    //class CoverageSeriesDataSetInfo;
    //class CoverageSeriesDataSetType;
    
    /*! 
      \brief An auxiliary function that transform ObservationDataSetInfo into ObservationDataSetType. 
    */   
    TESTEXPORT ObservationDataSetType GetType(const ObservationDataSetInfo& info);

    /*! 
      \brief An auxiliary function that transform TimeSeriesDataSetInfo into TimeSeriesDataSetType. 
    */   
    TESTEXPORT TimeSeriesDataSetType GetType(const TimeSeriesDataSetInfo& info);
    
    /*! 
      \brief An auxiliary function that transform TrajectoryDataSetInfo into TrajectoryDataSetType. 
    */   
    TESTEXPORT TrajectoryDataSetType GetType(const TrajectoryDataSetInfo& info);
    
    /*! 
      \brief An auxiliary struct to compare two datetime shared pointers 
    */
    struct TESTEXPORT CompareShrDateTime
    {
      bool operator()(const boost::shared_ptr<te::dt::DateTime>& t1, const boost::shared_ptr<te::dt::DateTime>& t2)
      {
        return t1->operator<(*t2);
      }
    };   

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_UTILS_H
