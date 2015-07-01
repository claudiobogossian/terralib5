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
  \file terralib/datatype/DateTimePeriod.h

  \brief An abstract class to represent a period of date and time. 
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_DATETIMEPERIOD_H
#define __TERRALIB_DATATYPE_INTERNAL_DATETIMEPERIOD_H

// TerraLib
#include "DateTime.h"

namespace te
{
  namespace dt
  {    
    class DateTimeInstant;
    
    /*!
      \class DateTimePeriod

      \brief An abstract class to represent a period of date and time. 

      A time period is a part of the continous time axis limited by two 
      time instants or points. 

      \sa DateTime
    */
    class TEDATATYPEEXPORT DateTimePeriod : public DateTime
    {
      public:
         
        /*! \brief Default constructor. */
        DateTimePeriod();

        /*!
          \brief It gets the initial date time instant. 

          \return The initial time instant

          \note The caller will take the ownership of the returned pointer. 
        */
        virtual DateTimeInstant* getInitialInstant() const = 0; 

        /*!
          \brief It gets the final date time instant. 

          \return The final time instant

          \note The caller will take the ownership of the returned pointer.
        */
        virtual DateTimeInstant* getFinalInstant() const = 0;  
        
        /*! \brief Virtual destructor. */
        virtual ~DateTimePeriod();    
    };    

  } // end namespace dt
}   // end namespace te

#endif // __TERRALIB_DATATYPE_INTERNAL_DATETIMEPERIOD_H

