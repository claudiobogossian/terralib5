/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/datatype/OrdinalInstant.h

  \brief A class to represent ordinal time instant. 
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_ORDINALINSTANT_H
#define __TERRALIB_DATATYPE_INTERNAL_ORDINALINSTANT_H

// TerraLib
#include "DateTimeInstant.h"
#include "SimpleData.h"

namespace te
{
  namespace dt
  {        
    /*!
      \class OrdinalInstant

      \brief A class to represent ordinal time instant.

      An ordinal number to represent a time instant. Examples: 
      the first/second/etc epidemiological week, the first/second/etc
      day, etc. 

      To understand the meaning of the ordinal numbers, a user has to use
      extra information, such as the temporal resolution and the starting time 
      in the DateTimeProperty.

      \sa DateTimeInstant
    */
    class TEDATATYPEEXPORT OrdinalInstant : public DateTimeInstant
    {
      public:
         
        /*! \brief Empty constructor. */
        OrdinalInstant();

        /*!
          \brief Constructor.

          \param t  The time instant represented by an integer type.
        */
        OrdinalInstant(Int64 t);
                
        /*!
          \brief It returns the time instant.

          \return The time instant.
        */
        Int64 getTimeInstant() const { return m_timeInstant; }

        /*!
          \brief It sets the time instant.

          \param t The time instant.
        */
        void setTimeInstant(Int64 t) { m_timeInstant = t; }
        
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
          \brief It returns a clone of this object.
          
          \return A clone of this object.
        */
        AbstractData* clone() const;

        /*!
          \brief It returns the time instant in a textual format.

          \return The time instant in a textual format.
        */
        std::string toString() const;

        /*!
          \brief It returns the subtype of the date and time type.

          \return The subtype of the date and time type. 
        */
        DateTimeType getDateTimeType() const { return ORDINAL_TIME_INSTANT; }

      protected:

        Int64 m_timeInstant;  //!< The internal time instant information. 
     };    

  } // end namespace dt
}   // end namespace te

#endif // __TERRALIB_DATATYPE_INTERNAL_ORDINALINSTANT_H

