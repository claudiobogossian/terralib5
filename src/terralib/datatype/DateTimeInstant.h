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
  \file terralib/datatype/DateTimeInstant.h

  \brief An abstract class to represent an instant of date and time. 
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_DATETIMEINSTANT_H
#define __TERRALIB_DATATYPE_INTERNAL_DATETIMEINSTANT_H

// TerraLib
#include "DateTime.h"

namespace te
{
  namespace dt
  {    
    /*!
      \class DateTimeInstant

      \brief An abstract class to represent an instant of date and time. 
      
      \sa DateTime
    */
    class TEDATATYPEEXPORT DateTimeInstant : public DateTime
    {
      public:
         
        /*! \brief Default constructor. */
        DateTimeInstant() { }

        /*! \brief Virtual destructor. */
        virtual ~DateTimeInstant() { }           
    };    

  } // end namespace dt
}   // end namespace te

#endif // __TERRALIB_DATATYPE_INTERNAL_DATETIMEINSTANT_H

