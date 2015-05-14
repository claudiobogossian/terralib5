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
  \file terralib/se/Trim.h

  \brief Strips off chars from a string value (String formatting functions).
*/

#ifndef __TERRALIB_SE_INTERNAL_TRIM_H
#define __TERRALIB_SE_INTERNAL_TRIM_H

// TerraLib
#include "Function.h"

namespace te
{
// Forward declaration
  class ParameterValue;

  namespace se
  {
    /*!
      \class Trim
      
      \brief Strips off chars from a string value (String formatting functions).

      The function strips off "leading", "trailing",
      or "both" chars from a string value. Attributes
      control the mode of stripping and the character
      stripped. Defaults are "leading" and blank.

      \sa Function, ParameterValue, StringLength, StringPosition
    */
    class TESEEXPORT Trim : public Function
    {
      public:

        /*!
          \enum StripOffPositionType

          \brief It controls the mode of stripping characters.
        */
        enum StripOffPositionType
        {
          LEADING,  /*!< It means to strips off leading chars from a string value.                     */
          TRAILING, /*!< It means to strips off trailing chars from a string value.                    */
          BOTH      /*!< It means to strips off both (leading and trailing) chars from a string value. */
        };

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new Trim. */
        Trim();

        /*! \brief Destructor. */
        ~Trim();

        //@}

        void setStringValue(ParameterValue* v);
        void setStripOffPosition(StripOffPositionType p);
        void setStripOffChar(const std::string& v);

      private:

        ParameterValue* m_stringValue;           //!< Mandatory.
        StripOffPositionType m_stripOffPosition; //!< Mandatory.
        std::string m_stripOffChar;              //!< Mandatory.
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_TRIM_H
