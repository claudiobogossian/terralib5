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
  \file terralib/dataaccess/query/BinaryOp.h

  \brief A base class for binary operations.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_BINARYOP_H
#define __TERRALIB_DATAACCESS_INTERNAL_BINARYOP_H

// TerraLib
#include "BinaryFunction.h"

namespace te
{
  namespace da
  {
    /*!
      \class BinaryOp
      
      \brief A base class for binary operations.

      \sa BinaryFunction
    */
    class TEDATAACCESSEXPORT BinaryOp : public BinaryFunction
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param name The operator name.
          \param arg1 The first argument.
          \param arg2 The second argument.

          \note The BinaryOp will take the ownership of the given arguments.
        */
        BinaryOp(const std::string& name, Expression* arg1, Expression* arg2) : BinaryFunction(name, arg1, arg2) {}

        /*!
          \brief Constructor.

          \param name The operator name.
          \param arg1 The first argument.
          \param arg2 The second argument.

          \note The BinaryOp will take the ownership of the given arguments.
        */
        BinaryOp(const std::string& name, const Expression& arg1, const Expression& arg2) : BinaryFunction(name, arg1, arg2) {}

        /*! \brief Copy constructor. */
        BinaryOp(const BinaryOp& rhs);

        /*! \brief Virtual Destructor. */
        virtual ~BinaryOp() {}

        /*! Assignment operator.  */
        BinaryOp& operator=(const BinaryOp& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_BINARYOP_H

