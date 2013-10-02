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
      \brief This function tests if v1 is less than or equal to v2
    */
    TESTEXPORT bool LessOrEqual(te::dt::AbstractData* v1, te::dt::AbstractData* v2);

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
            
    /*!
      \brief It adjusts a DataSetType (dtTo) and a ObservationSetType (obstTo) 
          
      It adjusts a DataSetType (dtTo) and a ObservationSetType (obstTo) with a subset
      of properties (properties) coming from a DataSetType (dtFrom) that has 
      an associated ObservationSetType (obstFrom). 
                    
      \param dtTo       The DataSetType to be adjusted.
      \param dtFrom     The DataSetType that provides the information to adjust dtTo. 
      \param obstTo     The ObservationSetType to be adjusted.
      \param obstFrom   The ObservationSetType that provides the information to adjust obstTo. 
      \param properties The subset of DataSetType "dtFrom" properties to be considered. 
    */
    //TESTEXPORT void AdjustDataSetType( te::da::DataSetType* dtTo, const te::da::DataSetType* dtFrom, 
    //                    te::st::ObservationSetType* obstTo, const te::st::ObservationSetType* obstFrom, 
    //                    const std::vector<te::dt::Property*>& properties);
    /*!
      \brief It normalizes a given time series, considering a given date and time resolution.
          
      During the normalization, it uses the time series internal interpolator in order to 
      estimate values to be associated to non-observed date and time.
          
      \param ts     The given time series.
      \param tRes   The date and time resolution to be considered in the normalization.

      \return A new normalized time series.
         
      \note The caller WILL take the ownership of the returned pointer.
    */
    /*TESTEXPORT*/ te::st::TimeSeries* Normalize(TimeSeries* ts, te::dt::DateTimeResolution tRes);

    /*!
      \brief It normalizes a given time series, considering a given date and time resolution.
          
      During the normalization, it uses a default value, passed by parameter, 
      to be associated to non-observed date and time.
          
      \param ts       The given time series.
      \param tRes     The date and time resolution to be considered in the normalization.
      \param defvalue The default value to be used at non-observed date and time.

      \return A new normalized time series.
         
      \note The caller WILL take the ownership of the returned pointer.
    */
    /*TESTEXPORT*/ te::st::TimeSeries* Normalize(TimeSeries* ts, te::dt::DateTimeResolution tRes, te::dt::AbstractData* defvalue);    

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_UTILS_H
