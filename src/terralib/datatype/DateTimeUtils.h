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
  \file terralib/datatype/DateTimeUtils.h

  \brief Utilitary function for the date and time types of the data type module.
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_DATETIMEUTILS_H
#define __TERRALIB_DATATYPE_INTERNAL_DATETIMEUTILS_H

// TerraLib
#include "Config.h"

// STL
#include <vector>

namespace te
{
  namespace dt
  {
// Forward declaration
    class DateTime;
    class DateTimeInstant;
    class DateTimePeriod;    

    /*!
      \brief It returns the temporal extent of two date and time types. 

      It returns a time period composed of the lowest and the biggest
      datetime of two given date time types.
       
      \param t1   The first datetime.
      \param t2   The second datetime.
    
      \return A datetime period that represents the temporal extent of the two given types.

      \note The caller will take the ownership of the output pointer. 
      \note The two given types must be of the same type.
    */
    TEDATATYPEEXPORT DateTimePeriod* GetTemporalExtent(const DateTime* t1, const DateTime* t2);   
    
    /*!
      \brief It returns the distance between two datetime types. 

      For Date and DatePeriod, the returned distance value is in days, 
      for TimeInstant, TimePeriod, TimeInstantTZ, TimePeriodTZ, it is in seconds, and 
      for OrdinalIntant and OrdinalPeriod, it is in the same unit of the type.
       
      \param t1   The first datetime.
      \param t2   The second datetime.
    
      \return The distance between the two given datetimes.

      \note The two given types must be of the same type.
    */
    TEDATATYPEEXPORT long GetDistance(const te::dt::DateTime* t1, const te::dt::DateTime* t2);
    
    /*!
      \brief It creates a time period based on two time instants. 

      \param t1   The first instant.
      \param t2   The second instant.
    
      \return A datetime period that represents composed of the two given instants.

      \note The caller will take the ownership of the output pointer. 
      \note The two given types must be of the same type.
    */
    TEDATATYPEEXPORT DateTimePeriod* GetTimePeriod(const DateTimeInstant* t1, const DateTimeInstant* t2);                                              

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATATYPE_INTERNAL_DATETIMEUTILS_H

