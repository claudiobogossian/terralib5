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
  \file terralib/datatype/DateTime.h

  \brief An abstract class for date and time types. 
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_DATETIME_H
#define __TERRALIB_DATATYPE_INTERNAL_DATETIME_H

// TerraLib
#include "AbstractData.h"
#include "Enums.h"

// Boost
#include <boost/shared_ptr.hpp>

namespace te
{
  namespace dt
  {    
    /*!
      \class DataTime

      \brief An abstract class for date and time types. 

      It is a base class for different kinds of date and time representations,
      such as, date and date period, time instant and period, 
      time with and without time zone, etc.

      For more information see ISO 8601:2004 and ISO 19108:2002.

      \note The DateTime classes can throw exceptions derived from std::out_of_range.

      \sa AbstractData, DateTimeInstant, DateDuration, TimeDuration
    */
    class TEDATATYPEEXPORT DateTime : public AbstractData
    {
      public:

        /*! \brief Default constructor. */
        DateTime() {}

        /*!
          \brief Operator ==

          \param rhs The date and time to be compared.
          
          \return It returns true if the two date and time are equal. Otherwise, it returns false.
        */
        virtual bool operator==(const DateTime&) const = 0;

        /*!
          \brief Operator !=

          \param rhs The date and time to be compared.
          
          \return It returns true if the two date and time are not equal. Otherwise, it returns false.
        */
        virtual bool operator!=(const DateTime&) const = 0;

        /*!
          \brief Operator <

          \param rhs The date and time to be compared.
          
          \return It returns true if the right-hand-side time is greater than the lefth side one. Otherwise, it returns false.
        */
        virtual bool operator<(const DateTime&) const = 0;

        /*!
          \brief Operator >

          \param rhs The right-hand-side time duration to be compared.
          
          \return It returns true if the right-hand-side time duration is less than the lefth side one. Otherwise, it returns false.
        */
        virtual bool operator>(const DateTime&) const = 0;
                
        /*!
          \brief It returns the data type code associated to date and time values: DATETIME_TYPE.

          \return The data type code associated to the date and time values: DATETIME_TYPE.
        */
        int getTypeCode() const { return DATETIME_TYPE; }
        
        /*!
          \brief It returns the subtype of the date and time type.

          \return The subtype of the date and time type. 
        */
        virtual DateTimeType getDateTimeType() const = 0;

        /*! \brief Virtual destructor. */
        virtual ~DateTime() { }
    };

    /*! \brief An auxiliary struct to compare two datetime pointers */
    struct TEDATATYPEEXPORT CompareDateTime
    {
      bool operator()(const te::dt::DateTime* t1, const te::dt::DateTime* t2) const
      {
        return t1->operator<(*t2);
      }
    };
	
	//Typedef 
    typedef boost::shared_ptr<DateTime>       DateTimeShrPtr;

   } // end namespace dt
}    // end namespace te

#endif // __TERRALIB_DATATYPE_INTERNAL_DATETIME_H

