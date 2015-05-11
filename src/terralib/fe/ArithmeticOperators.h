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
  \file terralib/fe/ArithmeticOperators.h
  
  \brief It is used to indicate what arithmetic operators the a service can support.  
 */

#ifndef __TERRALIB_FE_INTERNAL_ARITHMETICOPERATORS_H
#define __TERRALIB_FE_INTERNAL_ARITHMETICOPERATORS_H

// TerraLib
#include "../common/STLUtils.h"
#include "Config.h"

// STL
#include <vector>

namespace te
{
  namespace fe
  {
// Forward declarations
    class FunctionName;

    /*!
      \class ArithmeticOperators
      
      \brief It is used to indicate the arithmetic operators that a service can support.

      \sa ScalarCapabilities, FunctionName         
     */
    class TEFEEXPORT ArithmeticOperators
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new ArithmeticOperators. */
        ArithmeticOperators();

        /*! \brief Destructor. */
        ~ArithmeticOperators();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*! \brief It marks the Simple Arithmetic as supported by the filter. */
        void enableSimpleArithmetic();

        /*! \brief It marks the Simple Arithmetic as not supported by the filter. */
        void disableSimpleArithmetic();

        /*!
          \brief It adds the function name to the Arithmetic operator list.
         
          \param f The function name to be added to the Arithmetic operator list.

          \note The ArithmeticOperators object will take the ownership of the function name, so you must not free it.
         */
        void push_back(FunctionName* f);

        /*!
          \brief It returns a specified function name.

          \param i The index of desired function name.

          \return A specified function name.

          \note You must not clear the returned function name.

          \note The method will not check the index range.
         */
        const FunctionName* operator[](std::size_t i) const;

        //@}

      private:

        bool m_simpleArithmetic;                            //!< It is used to indicate that the service can support addition, subtraction, multiplication and division. Default: false. (Optional)
        std::vector<FunctionName*> m_arithmeticOperators;   //!< It is used to list the function names that are supported, and the number of arguments each function requires. (Optional)
    };    

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_ARITHMETICOPERATORS_H
