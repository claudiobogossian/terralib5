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
  \file terralib/dataaccess/core/query/Div.h

  \brief The division operator - syntatic-suggar.
*/

#ifndef __TERRALIB_DATAACCESS_CORE_QUERY_INTERNAL_DIV_H
#define __TERRALIB_DATAACCESS_CORE_QUERY_INTERNAL_DIV_H

// TerraLib
#include "BinaryOp.h"
#include "FunctionNames.h"

namespace te
{
  namespace da
  {
    namespace core
    {
      /*!
        \class Div
      
        \brief The division operator.

        This operator is just a syntatic-suggar.

        \sa BinaryOp
      */
      class TEDATAACCESSEXPORT Div : public BinaryOp
      {
        public:

          TE_DEFINE_VISITABLE

          /*!
            \brief Constructor.

            \param arg1 The first argument.
            \param arg2 The second argument.

            \note The addition operator will take the ownership of the given arguments.
          */
          Div(Expression* arg1, Expression* arg2) : BinaryOp(FunctionNames::sm_Div, arg1, arg2) {}

          /*!
            \brief Constructor.

            \param arg1 The first argument.
            \param arg2 The second argument.
          */
          Div(const Expression& arg1, const Expression& arg2) : BinaryOp(FunctionNames::sm_Div, arg1, arg2) {}

          /*! \brief Copy constructor. */
          Div(const Div& rhs);

          /*! \brief Destructor. */
          ~Div() {}

          /*! Assignment operator.  */
          Div& operator=(const Div& rhs);

          /*! \brief It creates a new copy of this expression. */
          Expression* clone() const;
      };

    }  // end namespace core
  }    // end namespace da
}      // end namespace te

#endif  // __TERRALIB_DATAACCESS_CORE_QUERY_INTERNAL_DIV_H
