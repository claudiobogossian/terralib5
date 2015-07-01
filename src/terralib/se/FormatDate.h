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
  \file terralib/se/FormatDate.h

  \brief Date formatting function.
*/

#ifndef __TERRALIB_SE_INTERNAL_FORMATDATE_H
#define __TERRALIB_SE_INTERNAL_FORMATDATE_H

// TerraLib
#include "Function.h"

namespace te
{
  namespace se
  {
// Forward declaration
    class ParameterValue;

    /*!
      \class FormatDate
      
      \brief Date formatting function.

      Function for dates.
      This function is used for several date types.
      The argument of the function can consist of
      one of the following ISO 8601 XML schema types:
      <ul>
      <li>dateTime</li>
      <li>time</li>
      <li>date</li>
      <li>gYearMonth</li>
      <li>gMonthDay</li>
      <li>gDay</li>
      <li>gMonth</li>
      <li>gYear</li>
      <li>gml:TimeInstant</li>
      </ul>
      Standard output is as defined in ISO 8601: <b>YYYY-MM-DDThh:mm:ss</b>.

      \sa Function, ParameterValue
    */
    class TESEEXPORT FormatDate : public Function
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new FormatDate. */
        FormatDate();

        /*! \brief Destructor. */
        ~FormatDate();

        //@}

        /*
          The Pattern is build-up of characters as follows:
          <ul>
          <li><b>YYYY:</b> Four digit year</li>
          <li><b>YY:</b> Two digit year (without century and millennium)</li>
          <li><b>MM:</b> Two digit month</li>
          <li><b>M:</b> Month, leading zero omitted</li>
          <li><b>MMM:</b> Month displayed by three letter acronym ("FEB"),
                          ISO 639 two-letter language codes as
                          defined by ISO 639 can be appended to
                          create language-dependent variants
                          (MMMde would yield "DEZ" instead of "DEC")</li>
          <li><b>MMMMM:</b> for display of full month ("February").
                            The two-letter language code can be
                            appended (MMMMMde would result in 'Februar').</li>
          <li><b>DD:</b> Two digit day</li>
          <li><b>D:</b> Day, leading zero omitted</li>
          <li><b>hh:</b> hour, h is used to omit a leading zero</li>
          <li><b>mm:</b> minute, m is used to omit a leading zero</li>
          <li><b>ss:</b> second, s is used to omit a leading zero</li>
          <li><b>.:</b> point, will appear literally in the result</li>
          <li><b>/:</b> slash, literally</li>
          <li><b>::</b> colon. literally</li>
          <li><b>-:</b> minus, literally</li>
          <li><b>\:</b> backslash is employed to quote any character, which is to appear literally in the result.</li>
          <li><b>a:</b> am/pm marker</li>
          <li><b>z:</b> z: time zone (if present e.g. Pacific Standard Time; PST; GMT-08:00)</li>
          </ul>
        */
        void setDateValue(ParameterValue* v);
        void setPattern(const std::string& p);

      private:

        /** @name Not Allowed Methods
         *  No copy allowed. 
         */
        //@{

        /*!
          \brief No copy constructor allowed.

          \param rhs The other object.
        */
        FormatDate(const FormatDate& rhs);

        /*!
          \brief No assignment operator allowed.

          \param rhs The other object.

          \return A reference for this.
        */
        FormatDate& operator=(const FormatDate& rhs);

        //@}

      private:

        ParameterValue* m_dateValue; //!< Mandatory.
        std::string m_pattern;       //!< Mandatory.
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_FORMATDATE_H
