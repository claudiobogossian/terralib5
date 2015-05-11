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
  \file terralib/dataaccess/query/BinaryFunction.h

  \brief A base class for binary functions.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_BINARYFUNCTION_H
#define __TERRALIB_DATAACCESS_INTERNAL_BINARYFUNCTION_H

// TerraLib
#include "Function.h"

namespace te
{
  namespace da
  {
    /*!
      \class BinaryFunction
      
      \brief A base class for binary functions.

      \sa Function, BinaryOp
    */
    class TEDATAACCESSEXPORT BinaryFunction : public Function
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param name The function name.
          \param arg1 The first argument.
          \param arg2 The second argument.

          \note The BinaryFunction will take the ownership of the given arguments.
        */
        BinaryFunction(const std::string& name, Expression* arg1, Expression* arg2);

        /*!
          \brief Constructor.

          \param name The function name.
          \param arg1 The first argument.
          \param arg2 The second argument.
        */
        BinaryFunction(const std::string& name, const Expression& arg1, const Expression& arg2);

        /*! \brief Copy constructor. */
        BinaryFunction(const BinaryFunction& rhs);

        /*! \brief Virtual Destructor. */
        virtual ~BinaryFunction();

        /*! Assignment operator.  */
        BinaryFunction& operator=(const BinaryFunction& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;

        /*!
          \brief It returns the first function argument.

          \return The first function argument.
        */
        Expression* getFirst() const;

        /*!
          \brief It sets the first function argument.

          \param arg The first function argument.

          \note The BinaryFunction will take the ownership of the given argument.
        */
        void setFirst(Expression* arg);

        /*!
          \brief It returns the second function argument.

          \return The second function argument.
        */
        Expression* getSecond() const;

        /*!
          \brief It sets the second function argument.

          \param arg The second function argument.

          \note The BinaryFunction will take the ownership of the given argument.
        */
        void setSecond(Expression* arg);
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_BINARYFUNCTION_H

