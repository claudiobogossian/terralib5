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
  \file terralib/dataaccess/query/Min.h

  \brief Min statistical function.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_MIN_H
#define __TERRALIB_DATAACCESS_INTERNAL_MIN_H

// TerraLib
#include "FunctionNames.h"
#include "UnaryFunction.h"

namespace te
{
  namespace da
  {
    /*!
      \class Min
      
      \brief Min statistical function.

      This function is just a syntatic-suggar.

      \sa UnaryFunction
    */
    class TEDATAACCESSEXPORT Min : public UnaryFunction
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param arg The single function argument.

          \note The Min function will take the ownership of the given argument.
        */
        Min(Expression* arg) : UnaryFunction(FunctionNames::sm_Min, arg) {}

        /*!
          \brief Constructor.

          \param arg The single function argument.
        */
        Min(const Expression& arg) : UnaryFunction(FunctionNames::sm_Min, arg) {}

        /*! \brief Copy constructor. */
        Min(const Min& rhs);

        /*! \brief Destructor. */
        ~Min() {}

        /*! Assignment operator.  */
        Min& operator=(const Min& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_MIN_H

