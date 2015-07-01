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
  \file terralib/common/LoggedException.h

  \brief A class that automatically logs the exception message.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_LOGGEDEXCEPTION_H
#define __TERRALIB_COMMON_INTERNAL_LOGGEDEXCEPTION_H

// TerraLib
#include "Exception.h"

namespace te
{
  namespace common
  {
    /*!
      \class LoggedException

      \brief A class that automatically logs the exception message.

      \ingroup common
     */
    class TECOMMONEXPORT LoggedException : public virtual Exception
    {
      public:

        /*!
          \brief It initializes a new LoggedException.

          \param what A brief description of what has raised the exception.
         */
        explicit LoggedException(const std::string& what) throw();

        /*!
          \brief It initializes a new LoggedException.

          \param what A brief description of what has raised the exception.
         */
        explicit LoggedException(const char* const what) throw();

        /*! \brief Destructor. */
        virtual ~LoggedException() throw();

        virtual const char* getClassName() const throw();
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_LOGGEDEXCEPTION_H

