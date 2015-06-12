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
  \file terralib/datatype/Date.h

  \brief A base class for date data types. 
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_DATE_H
#define __TERRALIB_DATATYPE_INTERNAL_DATE_H

// TerraLib
#include "DateTimeInstant.h"

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>

namespace te
{
  namespace dt
  {
    /*!
      \class Date

      \brief A base class for date data types.

      A class to represent dates based on the Gregorian Calendar. Internally,
      it uses boost datetime library to represent gregorian date, by using
      the data type boost::gregorian::date. For more information see the date
      representation defined in ISO 8601:2004 specification.

      \ingroup datatype

      \sa DateTime, DateDuration, DatePeriod, TimeDuration, TimeInstant, TimeInstantTZ, TimePeriod, TimePeriodTZ
    */
    class TEDATATYPEEXPORT Date : public DateTimeInstant
    {
      public:
         
        /*! \brief Empty constructor. */
        Date(); 
        
        /*!
          \brief Constructor.

          \param year   A gregorian year in the range: [1400, 9999].
          \param month  A gregorian month in the range: [1, 12].
          \param day    A gregorian day in the range: [1,31].
        */
        Date(boost::gregorian::greg_year year,
             boost::gregorian::greg_month month,
             boost::gregorian::greg_day day);
        
        /*!
          \brief Constructor.

          \param d A date based on the Gregorian Calendar.
        */
        Date(const boost::gregorian::date& d);

        /*!
          \brief It returns the internal boost date type.

          \return The internal boost date type.
        */
        const boost::gregorian::date& getDate() const { return m_date; } 

        /*!
          \brief It returns the internal boost date type.

          \return The internal boost date type.
        */
        boost::gregorian::date& getDate() { return m_date; } 

        /*!
          \brief It returns the gregorian day - from 1 to 31. 

          \return The gregorian day.
        */
        boost::gregorian::greg_day getDay() const { return m_date.day(); } 

        /*!
          \brief It returns the gregorian month - from 1 to 12. 

          \return The gregorian month.
        */
        boost::gregorian::greg_month getMonth() const { return m_date.month(); } 

        /*!
          \brief It returns the gregorian year.

          \return The gregorian year.
        */
        boost::gregorian::greg_year getYear() const { return m_date.year(); } 
        
        /*!
          \brief Operator ==

          \param rhs The date to be compared.
          
          \return It returns true if the two dates are equal. Otherwise, it returns false.
        */
        bool operator==(const DateTime& rhs) const;
        
        /*!
          \brief Operator !=

          \param rhs The date to be compared.
          
          \return It returns true if the two dates are not equal. Otherwise, it returns false.
        */
        bool operator!=(const DateTime& rhs) const;
       
        /*!
          \brief Operator <

          \param rhs The right side date to be compared.
          
          \return It returns true if the right side date is greater than the lefth side one. Otherwise, it returns false.
        */
        bool operator<(const DateTime& rhs) const;
        
        /*!
          \brief Operator >

          \param rhs The right side date to be compared.
          
          \return It returns true if the right side date is less than the lefth side one. Otherwise, it returns false.
        */
        bool operator>(const DateTime& rhs) const;

        /*!
          \brief Operator -

          \param rhs The right side datetime.

          \return It returns the number of the days between two dates. 

          \todo This operator must return DateDuration instead of long!
        */
        long operator-(const Date& rhs) const;

        /*!
          \brief It returns a clone of this object.
          
          \return A clone of this object.
        */
        AbstractData* clone() const;

        /*!
          \brief It returns the date in the ISO textual format (YYYYMMDD).

          \return The date in the ISO textual format (YYYYMMDD).
        */
        std::string toString() const;

        /*!
          \brief It returns the subtype of the date and time type.

          \return The subtype of the date and time type. 
        */
        DateTimeType getDateTimeType() const { return DATE; }

        /*!
          \brief Destructor.
        */
        virtual ~Date();

      protected:

        boost::gregorian::date m_date;  //!< Internal date information.
     };   

  } // end namespace dt
}   // end namespace te

#endif // __TERRALIB_DATATYPE_INTERNAL_DATE_H

