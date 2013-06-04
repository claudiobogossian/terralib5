/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/core/query/UnaryFunction.h

  \brief A base class for unary functions.
*/

#ifndef __TERRALIB_DATAACCESS_CORE_QUERY_INTERNAL_UNARYFUNCTION_H
#define __TERRALIB_DATAACCESS_CORE_QUERY_INTERNAL_UNARYFUNCTION_H

// TerraLib
#include "Function.h"

namespace te
{
  namespace da
  {
    namespace core
    {
      /*!
        \class UnaryFunction
      
        \brief A base class for unary functions.

        \sa Function, UnaryOp
       */
      class TEDATAACCESSEXPORT UnaryFunction : public Function
      {
        public:

          TE_DEFINE_VISITABLE

          /*!
            \brief Constructor.

            \param name The function name.
            \param arg  The single function argument.

            \note The UnaryFunction will take the ownership of the given argument.
          */
          UnaryFunction(const std::string& name, Expression* arg);

          /*!
            \brief Constructor.

            \param name The function name.
            \param arg  The single function argument.
          */
          UnaryFunction(const std::string& name, const Expression& arg);

          /*! \brief Copy constructor. */
          UnaryFunction(const UnaryFunction& rhs);

          /*! \brief Virtual Destructor. */
          virtual ~UnaryFunction();

          /*! Assignment operator.  */
          UnaryFunction& operator=(const UnaryFunction& rhs);

          /*! \brief It creates a new copy of this expression. */
          Expression* clone() const;

          /*!
            \brief It returns the function associated argument.

            \return The function associated argument.
          */
          Expression* getArgument() const;

          /*!
            \brief It sets the function argument.

            \param arg The single function argument.

            \note The UnaryFunction will take the ownership of the given argument.
          */
          void setArgument(Expression* arg);
      };

    }  // end namespace core
  }    // end namespace da
}      // end namespace te

#endif  // __TERRALIB_DATAACCESS_CORE_QUERY_INTERNAL_UNARYFUNCTION_H
