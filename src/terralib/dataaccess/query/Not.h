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
  \file terralib/dataaccess/query/Not.h

  \brief Boolean NOT operator - syntatic-suggar.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_NOT_H
#define __TERRALIB_DATAACCESS_INTERNAL_NOT_H

// TerraLib
#include "UnaryOp.h"
#include "FunctionNames.h"

namespace te
{
  namespace da
  {
    /*!
      \class Not
      
      \brief Boolean NOT operator.

      This operator is just a syntatic-suggar.

      \sa UnaryOp
    */
    class TEDATAACCESSEXPORT Not : public UnaryOp
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param arg The single function argument.

          \note The Not function will take the ownership of the given argument.
         */
        Not(Expression* arg) : UnaryOp(FunctionNames::sm_Not, arg) {}

        /*!
          \brief Constructor.

          \param arg The single function argument.

          \note The Not function will take the ownership of the given argument.
        */
        Not(const Expression& arg) : UnaryOp(FunctionNames::sm_Not, arg) {}

        /*! \brief Copy constructor. */
        Not(const Not& rhs);

        /*! \brief Destructor. */
        ~Not() {}

        /*! Assignment operator.  */
        Not& operator=(const Not& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_NOT_H

