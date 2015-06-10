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
  \file terralib/se/ChangeCase.h

  \brief Changes the case of strings (String formatting functions).
*/

#ifndef __TERRALIB_SE_INTERNAL_CHANGECASE_H
#define __TERRALIB_SE_INTERNAL_CHANGECASE_H

// TerraLib
#include "Function.h"

namespace te
{
  namespace se
  {
// Forward declaration
    class ParameterValue;

    /*!
      \class ChangeCase

      \brief Changes the case of strings (String formatting functions).

      The function changes the case of the
      StringValue as indicated by the attribute
      direction. Possible values of the latter are
      "toUpper" and "toLower", where the former is
      the default value.

      \sa Function, ParameterValue
    */
    class TESEEXPORT ChangeCase : public Function
    {
      public:

        /*!
          \enum CaseDirectionType

          \brief The type of case.
        */
        enum CaseDirectionType
        {
          TOUPPER,  /*!< It means to change case to upper case. */
          TOLOWER   /*!< It means to change case to lower case. */
        };

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new ChangeCase. */
        ChangeCase();

        /*! \brief Destructor. */
        ~ChangeCase();

        //@}

        void setStringValue(ParameterValue* v);
        void setDirection(CaseDirectionType cd);

      private:

        ParameterValue* m_stringValue; //!< Mandatory.
        CaseDirectionType m_direction; //!< Mandatory.
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_CHANGECASE_H
