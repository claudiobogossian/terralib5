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
  \file terralib/dataaccess/query/Sum.h

  \brief Sum aggregate operator.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_SUM_H
#define __TERRALIB_DATAACCESS_INTERNAL_SUM_H

// TerraLib
#include "FunctionNames.h"
#include "UnaryFunction.h"

namespace te
{
  namespace da
  {
    /*!
      \class Sum
      
      \brief Sum aggregate operator.

      This function is just a syntatic-suggar.

      \sa UnaryFunction
    */
    class TEDATAACCESSEXPORT Sum : public UnaryFunction
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param arg The single function argument.

          \note The Sum function will take the ownership of the given argument.
        */
        Sum(Expression* arg) : UnaryFunction(FunctionNames::sm_Sum, arg) {}

        /*!
          \brief Constructor.

          \param arg The single function argument.
        */
        Sum(const Expression& arg) : UnaryFunction(FunctionNames::sm_Sum, arg) {}

        /*! \brief Copy constructor. */
        Sum(const Sum& rhs);

        /*! \brief Destructor. */
        ~Sum() {}

        /*! Assignment operator.  */
        Sum& operator=(const Sum& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_SUM_H

