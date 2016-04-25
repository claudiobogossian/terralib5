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
  \file terralib/dataaccess/query/Cast.h

  \brief Cast a expression function.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_CAST_H
#define __TERRALIB_DATAACCESS_INTERNAL_CAST_H

// TerraLib
#include "FunctionNames.h"
#include "BinaryFunction.h"

namespace te
{
  namespace da
  {
    /*!
      \class Cast
      
      \brief Cast a expression function.

      This function is just a syntatic-suggar.

      \sa Function
    */
    class TEDATAACCESSEXPORT Cast : public BinaryFunction
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
        \brief Constructor.

        \param value  Value expression.
        \param type   Output cast type expression.
        */
        Cast(Expression* value, Expression* type) : BinaryFunction(FunctionNames::sm_Cast, value, type) {}

        /*!
        \brief Constructor.

        \param value  Value expression.
        \param type   Output cast type expression.
        */
        Cast(const Expression& value, const Expression& type) : BinaryFunction(FunctionNames::sm_Cast, value, type) {}

        /*! \brief Copy constructor. */
        Cast(const Cast& rhs);

        /*! \brief Destructor. */
        ~Cast() {}

        /*! Assignment operator.  */
        Cast& operator=(const Cast& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_CAST_H

