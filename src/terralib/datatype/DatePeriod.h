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
  \file terralib/datatype/DatePeriod.h

  \brief A class to represent date period. 
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_DATEPERIOD_H
#define __TERRALIB_DATATYPE_INTERNAL_DATEPERIOD_H

// TerraLib
#include "DateTimePeriod.h"

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>

namespace te
{
  namespace dt
  {     
     /*!
       \class DatePeriod

       \brief A class to represent date period based on the Gregorian Calendar.

       A class to represent date period based on the Gregorian Calendar. 
       A date period represents a range between two dates. Internally,
       it uses the datetime boost library to represent gregorian date period, 
       by using the data type boost::gregorian::date_period.

       \sa DateTime, Date, DateDuration, TimeDuration, TimeInstant, TimeInstantTZ, TimePeriod, TimePeriodTZ
    */
    class TEDATATYPEEXPORT DatePeriod : public DateTimePeriod
    {
      public:
         
        /*! \brief Constructor. */
        DatePeriod();
        
        /*!
          \brief Constructor.

          \param initialDate  An initial gregorian date.
          \param finalDate    An final gregorian date.
        */
        DatePeriod(const Date& initialDate, const Date& finalDate);        
        
        /*!
          \brief Constructor.

          \param dp A date period based on the Gregorian Calendar.
        */
        DatePeriod(const boost::gregorian::date_period& dp);

        /*!
          \brief It creates a period as [begin, end).

          \param begin The initial date.
          \param begin The final date.
        */
        DatePeriod(const boost::gregorian::date& begin, const boost::gregorian::date& end);

        /*!
          \brief It returns the boost date period type.

          \return The boost date period type.
        */
        const boost::gregorian::date_period& getDatePeriod() const { return m_datePeriod; }

        /*!
          \brief It returns the boost date period type.

          \return The boost date period type.
        */
        boost::gregorian::date_period& getDatePeriod() { return m_datePeriod; }
        
        /*!
          \brief It gets the initial date. 

          \return The initial date
        */
        Date getInitialDate() const; 

        /*!
          \brief It gets the final date. 

          \return The final date
        */
        Date getFinalDate() const; 
        
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

          \param rhs The date period to be compared.
          
          \return It returns true if the two date periods are equal. Otherwise, it returns false.
        */
        bool operator==(const DateTime& rhs) const;
        
        /*!
          \brief Operator !=

          \param rhs The date period to be compared.
          
          \return It returns true if the two date periods are not equal. Otherwise, it returns false.
        */
        bool operator!=(const DateTime& rhs) const;
       
        /*!
          \brief Operator <

          \param rhs The right side date period to be compared.
          
          \return It returns true if the right side date period is greater than the lefth side one. Otherwise, it returns false.
        */
        bool operator<(const DateTime& rhs) const;
        
        /*!
          \brief Operator >

          \param rhs The right side date period to be compared.
          
          \return It returns true if the right side date period is less than the lefth side one. Otherwise, it returns false.
        */
        bool operator>(const DateTime& rhs) const;

        /*!
          \brief It returns a clone of this object.
          
          \return A clone of this object.
        */
        AbstractData* clone() const;

        /*!
          \brief It returns the date period in the ISO textual format (YYYYMMDDTYYYYMMDD).

          \return The date period in the ISO textual format (YYYYMMDDTYYYYMMDD).
        */
        std::string toString() const;

        /*!
          \brief It returns the subtype of the date and time type.

          \return The subtype of the date and time type. 
        */
        DateTimeType getDateTimeType() const { return DATE_PERIOD; }

        /*!
          \brief Destructor.
        */
        virtual ~DatePeriod();

      protected:
          
        boost::gregorian::date_period m_datePeriod;  //!< The internal date period information.    
     };  
  } // end namespace dt
}   // end namespace te

#endif // __TERRALIB_DATATYPE_INTERNAL_DATEPERIOD_H

