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
  \file terralib/datatype/DateDuration.h

  \brief A class to represent date duration. 
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_DATEDURATION_H
#define __TERRALIB_DATATYPE_INTERNAL_DATEDURATION_H

// TerraLib
#include "DateTime.h"

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>

namespace te
{
  namespace dt
  {
    /*!
      \class DateDuration

      \brief A class to represent date duration.

      A date duration is a simple day count used for arithmetic with date,
      based on the Gregorian Calendar. Internally, it uses the datetime boost 
      library to represent gregorian date duration, 
      by using the data type boost::gregorian::date_duration.

      \sa DateTime, Date, DatePeriod, TimeDuration, TimeInstant, TimeInstantTZ, TimePeriod, TimePeriodTZ
    */
    class TEDATATYPEEXPORT DateDuration : public DateTime
    {
      public:
         
        /*! \brief Empty constructor. */
        DateDuration();

        /*!
          \brief Constructor.

          \param dd A date duration in days.
        */
        DateDuration(long dd);
        
        /*!
          \brief Constructor.

          \param dd A date duration.
        */
        DateDuration(const boost::gregorian::date_duration& dd);

        /*!
          \brief It returns the boost date duration type.

          \return The boost date duration type.
        */
        const boost::gregorian::date_duration& getDateDuration() const { return m_dateDuration; } 

        /*!
          \brief It returns the boost date duration type.

          \return The boost date duration type.
        */
        boost::gregorian::date_duration& getDateDuration() { return m_dateDuration; } 

        /*!
          \brief Operator ==

          \param rhs The date duration to be compared.
          
          \return It returns true if the two date durations are equal. Otherwise, it returns false.
        */
        bool operator==(const DateTime& rhs) const;
        
        /*!
          \brief Operator !=

          \param rhs The date duration to be compared.
          
          \return It returns true if the two date durations are not equal. Otherwise, it returns false.
        */
        bool operator!=(const DateTime& rhs) const;
       
        /*!
          \brief Operator <

          \param rhs  The right-hand-side date duration to be compared.
          
          \return It returns true if the right-hand-side date duration is greater than the lefth side one. Otherwise, it returns false.
        */
        bool operator<(const DateTime& rhs) const;
        
        /*!
          \brief Operator >

          \param rhs The right-hand-side date duration to be compared.
          
          \return It returns true if the right-hand-side date duration is less than the lefth side one. Otherwise, it returns false.
        */
        bool operator>(const DateTime& rhs) const;

        /*!
          \brief It returns a clone of this object.
          
          \return A clone of this object.
        */
        AbstractData* clone() const;

        /*!
          \brief It returns the date duration in a textual format.

          \return The date duration in a textual format.
        */
        std::string toString() const;

        /*!
          \brief It returns the subtype of the date and time type.

          \return The subtype of the date and time type. 
        */
        DateTimeType getDateTimeType() const { return DATE_DURATION; }

        /*!
          \brief Destructor
        */
        virtual ~DateDuration();

      protected:

        boost::gregorian::date_duration m_dateDuration;  //!< Date duration information.      
     };    

  } // end namespace dt
}   // end namespace te

#endif // __TERRALIB_DATATYPE_INTERNAL_DATEDURATION_H

