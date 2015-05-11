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
  \file terralib/fe/Add.h

  \brief The addition operator.
 */

#ifndef __TERRALIB_FE_INTERNAL_ADD_H
#define __TERRALIB_FE_INTERNAL_ADD_H

// TerraLib
#include "BinaryOperator.h"
#include "Globals.h"

namespace te
{
  namespace fe
  {    
    /*!
      \class Add
      
      \brief The addition operator.

      \sa BinaryOperator
     */
    class TEFEEXPORT Add : public BinaryOperator
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief Constructor.

          \param arg1 The first operand.
          \param arg2 The second operand.

          \note The addition operator will take the ownership of the given operands.
         */
        Add(Expression* arg1, Expression* arg2) : BinaryOperator(Globals::sm_add, arg1, arg2) {}

        /*! \brief Destructor. */
        ~Add() {}

        //@}
    };

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_ADD_H

