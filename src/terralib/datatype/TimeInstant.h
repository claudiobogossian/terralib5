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
  \file terralib/datatype/TimeInstant.h

  \brief A class to represent time instant. 
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_TIMEINSTANT_H
#define __TERRALIB_DATATYPE_INTERNAL_TIMEINSTANT_H

// TerraLib
#include "DateTimeInstant.h"
#include "Date.h"
#include "TimeDuration.h"

// Boost
#include <boost/date_time/posix_time/posix_time.hpp>

namespace te
{
  namespace dt
  {        
    class Date;
    
    /*!
      \class TimeInstant

      \brief A class to represent time instant.

      A time instant is a point on the continuous time axis. It is composed 
      of a date that uses the Gregorian calendar and of a local time. 
      Internally, it uses the datetime boost library to represent time instant, 
      by using the data type boost::posix_time::ptime.

      \sa Date, DateTime, TimeInstantTZ, TimeDuration, TimePeriod, TimePeriodTZ, DatePeriod, DateDuration
    */
    class TEDATATYPEEXPORT TimeInstant : public DateTimeInstant
    {
      public:
         
        /*! \brief Constructor. */
        TimeInstant();

        /*!
          \brief Constructor from a date and time offset.

          \param d  The date.
          \param td Time offset.
        */
        TimeInstant(const Date& d, const TimeDuration& td);
                
        /*!
          \brief Constructor.

          \param t A time instant.
        */
        TimeInstant(const boost::posix_time::ptime& t);

        /*!
          \brief Constructor.

          It constructs a time instant from a non delimited ISO form string.
          Example: 20020131T235959

          \param dtime ISO form string
        */
        TimeInstant(const std::string& dtime);

        /*!
          \brief It returns the boost time instant type.

          \return The boost time instant type.
        */
        const boost::posix_time::ptime& getTimeInstant() const { return m_timeInstant; }

        /*!
          \brief It returns the boost time instant type.

          \return The boost time instant type.
        */
        boost::posix_time::ptime& getTimeInstant() { return m_timeInstant; }

        /*!
          \brief It returns the date associated to time instant. 

          \return The date
        */
        Date getDate() const { return Date(m_timeInstant.date()); }
        
        /*!
          \brief It returns the time duration associated to time instant. 

          \return The time duration
        */
        TimeDuration getTime() const;

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
        long operator-(const TimeInstant& rhs) const;

        /*!
          \brief It returns a clone of this object.
          
          \return A clone of this object.
        */
        AbstractData* clone() const;

        /*!
          \brief It returns the time instant in the ISO textual format (YYYYMMDDTHHMMSS,fffffffff) where T is the date-time separator.

          \return The time instant in the ISO textual format (YYYYMMDDTHHMMSS,fffffffff) where T is the date-time separator.
         */
        std::string toString() const;

        /*!
          \brief It returns the subtype of the date and time type.

          \return The subtype of the date and time type. 
        */
        DateTimeType getDateTimeType() const { return TIME_INSTANT; }

        /*! \brief Constructor. */
        virtual ~TimeInstant();

      protected:

        boost::posix_time::ptime m_timeInstant;  //!< The internal time instant information. 
     };    

  } // end namespace dt
}   // end namespace te

#endif // __TERRALIB_DATATYPE_INTERNAL_TIMEINSTANT_H

