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
  \file terralib/datatype/TimeInstantTZ.h

  \brief A class to represent time instant with time zone. 
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_TIMEINSTANTTZ_H
#define __TERRALIB_DATATYPE_INTERNAL_TIMEINSTANTTZ_H

// TerraLib
#include "DateTimeInstant.h"

// Boost
#include <boost/date_time/local_time/local_time.hpp>

namespace te
{
  namespace dt
  {
    /*!
      \class TimeInstantTZ

      \brief A class to represent time instant with time zone.

      A time instant is a point on the continuous time axis. A time instant
      with time zone is composed of a date that uses the Gregorian calendar and 
      of a time with an associated time zone. Internally, it uses the datetime boost 
      library to represent time instant with time zone, by using the data type 
      boost::local_time::local_date_time.

      \sa DateTime, Date, DatePeriod, DateDuration, TimeInstant, TimePeriod, TimeDuration, TimePeriodTZ
    */
    class TEDATATYPEEXPORT TimeInstantTZ : public DateTimeInstant
    {
      public:

        /*!
          \brief Constructor.

          \param t A time instant with time zone.
        */
        TimeInstantTZ(const boost::local_time::local_date_time& t)
          : m_timeInstantTZ(t)
        {
        }

        /*!
          \brief It returns the boost time instant with time zone type.

          \return The boost time instant with time zone type.
        */
        const boost::local_time::local_date_time& getTimeInstantTZ() const { return m_timeInstantTZ; }

        /*!
          \brief It returns the boost time instant with time zone type.

          \return The boost time instant with time zone type.
        */
        boost::local_time::local_date_time& getTimeInstantTZ() { return m_timeInstantTZ; }

        /*!
          \brief Operator ==

          \param rhs The time instant to be compared.
          
          \return It returns true if the two time instants are equal. Otherwise, it returns false.
        */
        bool operator==(const DateTime& rhs) const;
        
        /*!
          \brief Operator !=

          \param rhs The time instant to be compared.
          
          \return It returns true if the two time instants are not equal. Otherwise, it returns false.
        */
        bool operator!=(const DateTime& rhs) const;
       
        /*!
          \brief Operator <

          \param rhs The right-hand-side time instant to be compared.
          
          \return It returns true if the right-hand-side time instant is greater than the lefth side one. Otherwise, it returns false.
        */
        bool operator<(const DateTime& rhs) const;
        
        /*!
          \brief Operator >

          \param rhs The right-hand-side time instant to be compared.
          
          \return It returns true if the right-hand-side time instant is less than the lefth side one. Otherwise, it returns false.
        */
        bool operator>(const DateTime& rhs) const;

        /*!
          \brief Operator -

          \param rhs  The right-hand-side time instant.

          \return It returns the number of seconds between the two time instants.
        */
        long operator-(const TimeInstantTZ& rhs) const;

        /*!
          \brief It returns a clone of this object.
          
          \return A clone of this object.
        */
        AbstractData* clone() const;

        /*!
          \brief It returns the time instant with time zone in the ISO textual format (YYYYMMDDThhmmss±hhmm).

          \return The time instant with time zone in the ISO textual format (YYYYMMDDThhmmss±hhmm).
        */
        std::string toString() const;

        /*!
          \brief It returns the subtype of the date and time type.

          \return The subtype of the date and time type. 
        */
        DateTimeType getDateTimeType() const { return TIME_INSTANT_TZ; }

        /*! \brief Constructor. */
        virtual ~TimeInstantTZ();

      protected:

        boost::local_time::local_date_time m_timeInstantTZ; //!< Time instant with time zone information.
     };     

  } // end namespace dt
}   // end namespace te

#endif // __TERRALIB_DATATYPE_INTERNAL_TIMEINSTANTTZ_H

