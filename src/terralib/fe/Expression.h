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
  \file terralib/fe/Expression.h
  
  \brief This is an abstract class that models a Filter Encoding expression.  
 */

#ifndef __TERRALIB_FE_INTERNAL_EXPRESSION_H
#define __TERRALIB_FE_INTERNAL_EXPRESSION_H

// TerraLib
#include "../common/BaseVisitable.h"
#include "Config.h"
#include "Visitor.h"

namespace te
{
  namespace fe
  {
    /*! \class Expression
      
        \brief This is an abstract class that models a Filter Encoding expression.

        An expression is a combination of one
        or more symbols that evaluate to single boolean
        value of true or false.

        \ingroup fe

        \sa BinaryOperator, PropertyName, Function, Literal, BinaryComparisonOp, PropertyIsBetween
     */
    class TEFEEXPORT Expression : public te::common::BaseVisitable<Visitor>
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new Expression. */
        Expression() {}

        /*! \brief Virtual destructor. */
        virtual ~Expression() {}

        //@}

        /*!
          \brief It returns a clone of this object.
          
          \return A clone of this object.
        */
        virtual Expression* clone() const = 0;

      private:

        /** @name Not Allowed Methods
         *  No copy allowed. 
         */
        //@{

        /*!
          \brief No copy constructor allowed.

          \param rhs The other instance.
         */
        Expression(const Expression& rhs);

        /*!
          \brief No assignment operator allowed.

          \param rhs The other instance.

          \return A reference for this.
         */
        Expression& operator=(const Expression& rhs);

        //@}
    };

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_EXPRESSION_H

