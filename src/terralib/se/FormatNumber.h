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
  \file terralib/se/FormatNumber.h

  \brief Numeric formatting function.
*/

#ifndef __TERRALIB_SE_INTERNAL_FORMATNUMBER_H
#define __TERRALIB_SE_INTERNAL_FORMATNUMBER_H

// TerraLib
#include "Function.h"

namespace te
{
  namespace se
  {
// Forward declaration
    class ParameterValue;

    /*!
      \class FormatNumber
      
      \brief Numeric formatting function.

      Function for formatting numbers to make them human readable.
      One of the most needed is a function for formatting numbers
      to make them human readable. You need such a function
      whenever a TextSymbolizer is employed to output a
      numeric property value.

      \sa Function, ParameterValue
    */
    class TESEEXPORT FormatNumber : public Function
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new FormatNumber. */
        FormatNumber();

        /*! \brief Destructor. */
        ~FormatNumber();

        //@}

        /*
          The Pattern is build-up of characters as follows:
          <ul>
          <li><b>#:</b> digit, which is omitted if irrelevant (leading or trailing zero);</li>
          <li><b>0:</b> digit;</li>
          <li><b>.:</b> decimal point;</li>
          <li><b>,:</b> grouping separator;</li>
          <li><b>E:</b> exponent indicator;</li>
          <li><b>-:</b> minus sign;</li>
          <li><b>':</b> apostrophe (for quoting).</li>
          </ul>

          Due to required distinctions in the notation of numbers,
          two of the pattern characters can be defined as attributes
          of the FormatNumber element:
          decimalPoint="."
          groupingSeperator=","

          If there is no NegativePattern, "-" is prefixed to the Pattern.
        */
        void setNumericValue(ParameterValue* v);
        void setPattern(const std::string& p);
        void setNegativePattern(const std::string& np);
        void setDecimalPoint(const std::string& dp);
        void setGroupingSeparator(const std::string& gs);

      private:

        /** @name Not Allowed Methods
         *  No copy allowed. 
         */
        //@{

        /*!
          \brief No copy constructor allowed.

          \param rhs The other object.
        */
        FormatNumber(const FormatNumber& rhs);

        /*!
          \brief No assignment operator allowed.

          \param rhs The other object.

          \return A reference for this.
        */
        FormatNumber& operator=(const FormatNumber& rhs);

        //@}

      private:

        ParameterValue* m_numericValue;    //!< Mandatory.
        std::string m_pattern;             //!< Mandatory.
        std::string m_negativePattern;     //!< Optional.
        std::string m_decimalPoint;        //!< Optional. Default: ".".
        std::string m_groupingSeparator;   //!< Optional. Default: ",".
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_FORMATNUMBER_H
