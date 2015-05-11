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
  \file terralib/dataaccess/query/UnaryOp.h

  \brief A base class for unary operators.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_UNARYOP_H
#define __TERRALIB_DATAACCESS_INTERNAL_UNARYOP_H

// TerraLib
#include "UnaryFunction.h"

namespace te
{
  namespace da
  {
    /*!
      \class UnaryOp
      
      \brief A base class for unary operators.

      \sa UnaryFunction
    */
    class TEDATAACCESSEXPORT UnaryOp : public UnaryFunction
    {
      TE_DEFINE_VISITABLE

      public:

        /*!
          \brief Constructor.

          \param name The operator name.
          \param arg  The single operator argument.

          \note The UnaryOp will take the ownership of the given argument.
        */
        UnaryOp(const std::string& name, Expression* arg) : UnaryFunction(name, arg) { }

        /*!
          \brief Constructor.

          \param name The operator name.
          \param arg  The single operator argument.
        */
        UnaryOp(const std::string& name, const Expression& arg) : UnaryFunction(name, arg) { }

        /*! \brief Copy constructor. */
        UnaryOp(const UnaryOp& rhs);

        /*! \brief Virtual Destructor. */
        virtual ~UnaryOp() {}

        /*! Assignment operator.  */
        UnaryOp& operator=(const UnaryOp& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_UNARYOP_H

