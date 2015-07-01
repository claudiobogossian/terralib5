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
  \file terralib/se/Concatenate.h

  \brief Concatenates strings (String formatting functions).
*/

#ifndef __TERRALIB_SE_INTERNAL_CONCATENATE_H
#define __TERRALIB_SE_INTERNAL_CONCATENATE_H

// TerraLib
#include "Function.h"

namespace te
{
  namespace se
  {
// Forward declaration
    class ParameterValue;

    /*!
      \class Concatenate
      
      \brief Concatenates strings (String formatting functions).

      The function concatenates strings.
      It is used to create concatenated strings as
      arguments of functions.

      \sa Function, ParameterValue
    */
    class TESEEXPORT Concatenate : public Function
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new Concatenate. */
        Concatenate();

        /*! \brief Destructor. */
        ~Concatenate();

        //@}

        void add(ParameterValue* v);

      private:

        std::vector<ParameterValue*> m_stringValue; //!< Mandatory at least two.
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_CONCATENATE_H
