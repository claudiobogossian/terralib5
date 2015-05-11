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
  \file terralib/dataaccess/query/Or.h

  \brief Boolean logic operator: OR - syntatic-suggar.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_OR_H
#define __TERRALIB_DATAACCESS_INTERNAL_OR_H

// TerraLib
#include "BinaryOp.h"
#include "FunctionNames.h"

namespace te
{
  namespace da
  {
    /*!
      \class BinaryOp
      
      \brief Boolean logic operator: OR.

      This operator is just a syntatic-suggar.

      \sa BinaryOp
    */
    class TEDATAACCESSEXPORT Or : public BinaryOp
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param arg1 The first argument.
          \param arg2 The second argument.

          \note The addition operator will take the ownership of the given arguments.
        */
        Or(Expression* arg1, Expression* arg2) : BinaryOp(FunctionNames::sm_Or, arg1, arg2) {}

        /*!
          \brief Constructor.

          \param arg1 The first argument.
          \param arg2 The second argument.
        */
        Or(const Expression& arg1, const Expression& arg2) : BinaryOp(FunctionNames::sm_Or, arg1, arg2) {}

        /*! \brief Copy constructor. */
        Or(const Or& rhs);

        /*! \brief Destructor. */
        ~Or() {}

        /*! Assignment operator.  */
        Or& operator=(const Or& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_OR_H

