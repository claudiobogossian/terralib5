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
  \file terralib/se/Substring.h
  
  \brief It returns the substring (String formatting functions).
*/

#ifndef __TERRALIB_SE_INTERNAL_SUBSTRING_H
#define __TERRALIB_SE_INTERNAL_SUBSTRING_H

// TerraLib
#include "Function.h"

namespace te
{
  namespace se
  {
// Forward declaration
    class ParameterValue;

    /*!
      \class Substring
      
      \brief It returns the substring (String formatting functions).

      It returns the substring at position
      Position (counting from 1) with length Length.<br>
      The first argument StringValue is converted to
      a string value before applying the substring operation.
      If Position is not specified it is assumed as 1. The
      default value for Length is the remaining length
      starting at Position.
      <br>
      The function shall react friendly on invalid Position
      and Length contents. Positions and Lengths less or equal 0
      shall yield the empty string. If the actual string length
      is less the defined substring the existing part of the
      substring shall be returned.

      \sa Function, ParameterValue
     */
    class TESEEXPORT Substring : public Function
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new Substring. */
        Substring();

        /*! \brief Destructor. */
        ~Substring();

        //@}

        void setStringValue(ParameterValue* v);
        void setPosition(ParameterValue* v);
        void setLength(ParameterValue* v);

      private:

        /** @name Not Allowed Methods
         *  No copy allowed. 
         */
        //@{

        /*!
          \brief No copy constructor allowed.

          \param rhs The other object.
         */
        Substring(const Substring& rhs);

        /*!
          \brief No assignment operator allowed.

          \param rhs The other object.

          \return A reference for this.
         */
        Substring& operator=(const Substring& rhs);

        //@}

      private:

        ParameterValue* m_stringValue; //!< Mandatory.
        ParameterValue* m_position;    //!< Optional.
        ParameterValue* m_length;      //!< Optional.
    };    

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_SUBSTRING_H
