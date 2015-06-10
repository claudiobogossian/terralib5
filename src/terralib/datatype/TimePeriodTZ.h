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
  \file terralib/datatype/TimePeriodTZ.h

  \brief A class to represent time period with time zone. 
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_TIMEPERIODTZ_H
#define __TERRALIB_DATATYPE_INTERNAL_TIMEPERIODTZ_H

// TerraLib
#include "DateTimePeriod.h"

// Boost
#include <boost/date_time/local_time/local_time.hpp>

namespace te
{
  namespace dt
  {
    class TimeInstantTZ;

    /*!
      \class TimePeriodTZ

      \brief A class to represent time period with time zone.

      A time period is a part of the continous time axis limited by two 
      time instants with time zone. Each time instant with time zone is composed 
      of a date that uses the Gregorian calendar and of a time with an associated time zone.
      Internally, it uses the datetime boost library to represent time 
      period with time zone, by using the data type boost::local_time::local_time_period.

      \sa DateTime, Date, DatePeriod, DateDuration, TimeInstant, TimeInstantTZ, TimePeriod, TimeDuration
    */
    class TEDATATYPEEXPORT TimePeriodTZ : public DateTimePeriod
    {
      public:
         
         /*!
          \brief Constructor.

          \param initialTime  A initial time instant.
          \param finalTime    A final time instant.
        */
        TimePeriodTZ(const TimeInstantTZ& initialTime, const TimeInstantTZ& finalTime);        
        
        /*!
          \brief Constructor.

          \param t A time period with time zone.
        */
        TimePeriodTZ(const boost::local_time::local_time_period& t);

        /*!
          \brief It returns the boost time period with time zone type.

          \return The boost time period with time zone type.
        */
        const boost::local_time::local_time_period& getTimePeriodTZ() const { return m_timePeriodTZ; } 

        /*!
          \brief It returns the boost time period with time zone type.

          \return The boost time period with time zone type.
        */
        boost::local_time::local_time_period& getTimePeriodTZ() { return m_timePeriodTZ; }
        
        /*!
          \brief It gets the initial time instant. 

          \return The initial time instant
        */
        TimeInstantTZ getInitialTimeInstant() const;  

        /*!
          \brief It gets the final time instant. 

          \return The final time instant
        */
        TimeInstantTZ getFinalTimeInstant() const;

        /*!
          \brief It gets the initial date time instant. 

          \return The initial time instant

          \note The caller will take the ownership of the returned pointer. 
        */
        DateTimeInstant* getInitialInstant() const; 

        /*!
          \brief It gets the final date time instant. 

          \return The final time instant

          \note The caller will take the ownership of the returned pointer.
        */
        DateTimeInstant* getFinalInstant() const;  
        
        /*!
          \brief Operator ==

          \param rhs The time period to be compared.
          
          \return It returns true if the two time period are equal. Otherwise, it returns false.
        */
        bool operator==(const DateTime& rhs) const;
        
        /*!
          \brief Operator !=

          \param rhs The time period to be compared.
          
          \return It returns true if the two time periods are not equal. Otherwise, it returns false.
        */
        bool operator!=(const DateTime& rhs) const;
       
        /*!
          \brief Operator <

          \param rhs The right-hand-side time period to be compared.
          
          \return It returns true if the right-hand-side time period is greater than the lefth side one. Otherwise, it returns false.
        */
        bool operator<(const DateTime& rhs) const;
        
        /*!
          \brief Operator >

          \param rhs The right-hand-side time period to be compared.
          
          \return It returns true if the right-hand-side time period is less than the lefth side one. Otherwise, it returns false.
        */
        bool operator>(const DateTime& rhs) const;

        /*!
          \brief It returns a clone of this object.
          
          \return A clone of this object.
        */
        AbstractData* clone() const;

        /*!
          \brief It returns the time period with time zone in the ISO textual format (YYYYMMDDThhmmss±hhmm/YYYYMMDDThhmmss±hhmm).

          \return The time period with time zone in the ISO textual format (YYYYMMDDThhmmss±hhmm/YYYYMMDDThhmmss±hhmm).
        */
        std::string toString() const;

        /*!
          \brief It returns the subtype of the date and time type.

          \return The subtype of the date and time type. 
        */
        DateTimeType getDateTimeType() const { return TIME_PERIOD_TZ; }

        /*!
          \brief Destructor.
        */
        virtual ~TimePeriodTZ();
       
      protected:

         boost::local_time::local_time_period  m_timePeriodTZ;  //!< Time period with time zone information.
     };

  } // end namespace dt
}   // end namespace te

#endif // __TERRALIB_DATATYPE_INTERNAL_TIMEPERIODTZ_H

