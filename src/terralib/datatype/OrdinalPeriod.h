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
  \file terralib/datatype/OrdinalPeriod.h

  \brief A class to represent ordinal time period. 
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_ORDINALPERIOD_H
#define __TERRALIB_DATATYPE_INTERNAL_ORDINALPERIOD_H

// TerraLib
#include "DateTimePeriod.h"
#include "SimpleData.h"
#include "OrdinalInstant.h"

namespace te
{
  namespace dt
  {        
    /*!
      \class OrdinalPeriod

      \brief A class to represent ordinal time period.

      A time period represented by two ordinal numbers: the first one
      to represent the inital time and the second one the final time.
      Examples: a time period whose initial time is the first 
      epidemiological week and the final time is the fifht epidemiological
      week.
      
      To understand the meaning of the ordinal numbers, a user has to use
      extra information, such as the temporal resolution and the starting time 
      in the DateTimeProperty.

      \sa DateTime
    */
    class TEDATATYPEEXPORT OrdinalPeriod : public DateTimePeriod
    {
      public:
         
        /*! \brief Empty constructor. */
        OrdinalPeriod();

        /*!
          \brief Constructor.

          \param it  The initial time instant.
          \param ft  The final time instant.
        */
        OrdinalPeriod(Int64 it, Int64 ft);
                
        /*!
          \brief It returns the inital time instant.

          \return The initial time instant.
        */
        Int64 getInitialTime() const { return m_initialInstant.getTimeInstant(); }

        /*!
          \brief It returns the final time instant.

          \return The final time instant.
        */
        Int64 getFinalTime() const { return m_finalInstant.getTimeInstant(); }

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
          \brief It sets the inital time instant.

          \param it The initial time instant.
        */
        void setInitialInstant(Int64 it) { m_initialInstant = it; }

        /*!
          \brief It sets the final time instant.

          \param ft The final time instant.
        */
        void setFinalInstant(Int64 ft) { m_finalInstant = ft; }
        
        /*!
          \brief Operator ==

          \param rhs The time period to be compared.
          
          \return It returns true if the two time periods are equal. Otherwise, it returns false.
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
          \brief It returns the time period in a textual format (initialTimeInstant-finalTimeinstant) 

          \return The time period in a textual format (initialTimeInstant-finalTimeinstant) 
        */
        std::string toString() const;
        
        /*!
          \brief It returns the subtype of the date and time type.

          \return The subtype of the date and time type. 
        */
        DateTimeType getDateTimeType() const { return ORDINAL_TIME_PERIOD; }

      protected:

        OrdinalInstant    m_initialInstant;   //!< The initial time instant information. 
        OrdinalInstant    m_finalInstant;     //!< The final time instant information. 
     };    

  } // end namespace dt
}   // end namespace te

#endif // __TERRALIB_DATATYPE_INTERNAL_ORDINALPERIOD_H

