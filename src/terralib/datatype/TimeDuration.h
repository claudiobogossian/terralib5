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
  \file terralib/datatype/TimeDuration.h

  \brief A class to represent time duration. 
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_TIMEDURATION_H
#define __TERRALIB_DATATYPE_INTERNAL_TIMEDURATION_H

// TerraLib
#include "DateTime.h"

// Boost
#include <boost/cstdint.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace te
{
  namespace dt
  {
    /*!
      \class TimeDuration

      \brief A class to represent time duration with nano-second/micro-second resolution.

      A class to represent time duration with nano-second/micro-second resolution (hh:mm:ss,ss). 
      Internally, it uses the datetime boost library to represent time duration, by using
      the data type boost::posix_time::time_duration.

      \sa DateTime, Date, DatePeriod, DateDuration, TimeInstant, TimeInstantTZ, TimePeriod, TimePeriodTZ
    */
    class TEDATATYPEEXPORT TimeDuration : public DateTime
    {
      public:
         
        /*!
          \brief Constructor.

          \param hours        A number of hours of a day - from 0 to 23.
          \param minutes      A number of minutes of a hour - from 0 to 59.
          \param seconds      A number of seconds of a minute - from 0 to 59.
          \param fracSeconds  A fractional seconds.
        */
        TimeDuration(long hours, long minutes, long seconds, boost::int64_t fracSeconds = 0);
        
        /*!
          \brief Constructor.

          \param t A time duration.
        */
        TimeDuration(boost::posix_time::time_duration& t);        

        /*!
          \brief It returns the boost time duration type.

          \return The boost time duration type.
        */
        const boost::posix_time::time_duration& getTimeDuration() const { return m_timeDuration; }

        /*!
          \brief It returns the boost time duration type.

          \return The boost time duration type.
        */
        boost::posix_time::time_duration& getTimeDuration() { return m_timeDuration; }
        
        /*!
          \brief It returns the hours of a day - from 0 to 23.

          \return The hour of a day.
        */
        long getHours() const { return m_timeDuration.hours(); } 

        /*!
          \brief It returns the minutes of a hour - from 0 to 59.

          \return The the minutes of a hour.
        */
        long getMinutes() const { return m_timeDuration.minutes(); } 

        /*!
          \brief It returns the seconds of a minute - from 0 to 59.

          \return The the seconds of a minute.
        */
        long getSeconds() const { return m_timeDuration.seconds(); } 

        /*!
          \brief It returns the fractional seconds

          \return The the fractional seconds
        */
        boost::int64_t getFractionalSeconds() const { return m_timeDuration.fractional_seconds(); }

        /*!
          \brief Operator ==

          \param rhs The time duration to be compared.
          
          \return It returns true if the two time durations are equal. Otherwise, it returns false.
        */
        bool operator==(const DateTime& rhs) const;
        
        /*!
          \brief Operator !=

          \param rhs The time duration to be compared.
          
          \return It returns true if the two time durations are not equal. Otherwise, it returns false.
        */
        bool operator!=(const DateTime& rhs) const;
       
        /*!
          \brief Operator <

          \param rhs The right-hand-side time duration to be compared.
          
          \return It returns true if the right-hand-side time duration is greater than the lefth side one. Otherwise, it returns false.
        */
        bool operator<(const DateTime& rhs) const;
        
        /*!
          \brief Operator <

          \param rhs The right-hand-side time duration to be compared.
          
          \return It returns true if the right-hand-side time duration is less than the lefth side one. Otherwise, it returns false.
        */
        bool operator>(const DateTime& rhs) const;

        /*!
          \brief Operator -

          \param rhs The right-hand-side time instant.

          \return It returns the number of seconds between the two time durations.

          \todo This method should return time duration!
        */
        long operator-(const TimeDuration& rhs) const;

        /*!
          \brief It returns a clone of this object.
          
          \return A clone of this object.
        */
        AbstractData* clone() const;

        /*!
          \brief It returns the time duration in the ISO textual format (hh:mm:ss,ss).

          \return The date in the ISO textual format (hh:mm:ss,ss).
        */
        std::string toString() const;
        
        /*!
          \brief It returns the subtype of the date and time type.

          \return The subtype of the date and time type. 
        */
        DateTimeType getDateTimeType() const { return TIME_DURATION; }

        /*!
          \brief Destructor.
        */
        virtual ~TimeDuration();

      protected:

        boost::posix_time::time_duration m_timeDuration;  //!< The internal time duration information. 
     };

   }  // end namespace dt
}     // end namespace te

#endif // __TERRALIB_DATATYPE_INTERNAL_TIMEDURATION_H

