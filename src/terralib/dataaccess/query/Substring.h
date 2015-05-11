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
  \file terralib/dataaccess/query/Substring.h

  \brief Allows the extraction of substrings.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_SUBSTRING_H
#define __TERRALIB_DATAACCESS_INTERNAL_SUBSTRING_H

// TerraLib
#include "Function.h"

namespace te
{
  namespace da
  {
    /*!
      \class Substring
      
      \brief Allows the extraction of substrings.

      \sa Function
    */
    class TEDATAACCESSEXPORT Substring : public Function
    {
      public:

        /*! \brief Default constructor. */
        Substring(Expression* str, Expression* start, Expression* len);

        /*! \brief Copy constructor. */
        Substring(const Substring& rhs);

        /*! \brief Destructor. */
        ~Substring() {}

        /*! Assignment operator.  */
        Substring& operator=(const Substring& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;

        Expression* getString() const;

        void setString(Expression* str);

        Expression* getStartPosition();

        void setStartPosition(Expression* start);

        Expression* getLength();

        void setLength(Expression* len);
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_SUBSTRING_H

