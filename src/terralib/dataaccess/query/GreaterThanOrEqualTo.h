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
  \file terralib/dataaccess/query/GreaterThanOrEqualTo.h

  \brief It models the inequality operator greater than or equal to (>=) - syntatic-suggar.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_GREATERTHANOREQUALTO_H
#define __TERRALIB_DATAACCESS_INTERNAL_GREATERTHANOREQUALTO_H

// TerraLib
#include "BinaryOp.h"
#include "FunctionNames.h"

namespace te
{
  namespace da
  {
    /*!
      \class GreaterThanOrEqualTo
      
      \brief It models the inequality operator greater than or equal to (>=).

      This operator is just a syntatic-suggar.

      \sa BinaryOp
    */
    class TEDATAACCESSEXPORT GreaterThanOrEqualTo : public BinaryOp
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param arg1 The first argument.
          \param arg2 The second argument.

          \note The addition operator will take the ownership of the given arguments.
        */
        GreaterThanOrEqualTo(Expression* arg1, Expression* arg2) : BinaryOp(te::da::FunctionNames::sm_GreaterThanOrEqualTo, arg1, arg2) {}

        /*!
          \brief Constructor.

          \param arg1 The first argument.
          \param arg2 The second argument.
        */
        GreaterThanOrEqualTo(const Expression& arg1, const Expression& arg2) : BinaryOp(te::da::FunctionNames::sm_GreaterThanOrEqualTo, arg1, arg2) {}

        /*! \brief Copy constructor. */
        GreaterThanOrEqualTo(const GreaterThanOrEqualTo& rhs);

        /*! \brief Destructor. */
        ~GreaterThanOrEqualTo() {}

        /*! Assignment operator.  */
        GreaterThanOrEqualTo& operator=(const GreaterThanOrEqualTo& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_GREATERTHANOREQUALTO_H

