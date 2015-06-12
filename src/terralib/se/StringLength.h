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
  \file terralib/se/StringLength.h

  \brief Returns length of string (String formatting functions).
*/

#ifndef __TERRALIB_SE_INTERNAL_STRINGLENGTH_H
#define __TERRALIB_SE_INTERNAL_STRINGLENGTH_H

// TerraLib
#include "Function.h"

namespace te
{
// Forward declaration
  class ParameterValue;

  namespace se
  {

    /*!
      \class StringLength
      
      \brief Returns length of string (String formatting functions).

      The function returns the number of
      characters in a string. The argument
      is converted to a string before
      computing its length.

      \sa Function, ParameterValue, StringPosition, Trim
    */
    class TESEEXPORT StringLength : public Function
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new StringLength. */
        StringLength();

        /*! \brief Destructor. */
        ~StringLength();

        //@}

        void setStringValue(ParameterValue* v);

      private:

        ParameterValue* m_stringValue;  //!< Mandatory.
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_STRINGLENGTH_H
