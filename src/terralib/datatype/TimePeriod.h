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
  \file terralib/datatype/TimePeriod.h

  \brief A class to represent time period. 
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_TIMEPERIOD_H
#define __TERRALIB_DATATYPE_INTERNAL_TIMEPERIOD_H

// TerraLib
#include "DateTimePeriod.h"

// Boost
#include <boost/date_time/posix_time/posix_time.hpp>

namespace te
{
  namespace dt
  {    
    class TimeInstant;
    
    /*!
      \class TimePeriod

      \brief A class to represent time period.

      A time period is a part of the continous time axis limited by two 
      time instants or points. Each time instant is composed 
      of a date that uses the Gregorian calendar and of a local time.
      Internally, it uses the datetime boost library to represent time 
      period, by using the data type boost::posix_time::time_period.

      \sa DateTime, Date, DatePeriod, DateDuration, TimePeriodTZ, TimeInstant, TimeInstantTZ, TimeDuration
    */
    class TEDATATYPEEXPORT TimePeriod : public DateTimePeriod
    {
      public:
         
        /*! \brief Constructor. */
        TimePeriod();
        
        /*!
          \brief Constructor.

          \param initialTime  A initial time instant.
          \param finalTime    A final time instant.
        */
        TimePeriod(const TimeInstant& initialTime, const TimeInstant& finalTime);
        
        /*!
          \brief Constructor.

          \param t A time period.
        */
        TimePeriod(const boost::posix_time::time_period& t); 
        
        /*! 
          \brief Assignment operator.
        */
        //const TimePeriod& operator=(const TimePeriod& rhs);               

        /*!
          \brief It returns the boost time period type.

          \return The boost time period type.
        */
        const boost::posix_time::time_period& getTimePeriod() const { return m_timePeriod; }

        /*!
          \brief It returns the boost time period type.

          \return The boost time period type.
        */
        boost::posix_time::time_period& getTimePeriod() { return m_timePeriod; }

        /*!
          \brief It gets the initial time instant. 

          \return The initial time instant
        */
        TimeInstant getInitialTimeInstant() const;

        /*!
          \brief It gets the final time instant. 

          \return The final time instant
        */
        TimeInstant getFinalTimeInstant() const;  

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
          \brief It returns the time period in the ISO textual format (YYYYMMDDThhmmss/YYYYMMDDThhmmss).

          \return The time period in the ISO textual format (YYYYMMDDThhmmss/YYYYMMDDThhmmss).
        */
        std::string toString() const;

        /*!
          \brief It returns the subtype of the date and time type.

          \return The subtype of the date and time type. 
        */
        DateTimeType getDateTimeType() const { return TIME_PERIOD; }

        /*!
          \brief Destructor
        */
        virtual ~TimePeriod();
                  
      protected:

        boost::posix_time::time_period  m_timePeriod;  //!< Time period information.
     };    

  } // end namespace dt
}   // end namespace te

#endif // __TERRALIB_DATATYPE_INTERNAL_TIMEPERIOD_H

