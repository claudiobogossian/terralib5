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
  \file terralib/fe/BinaryOperator.h
  
  \brief This class can be used to represent binary operation expressions.  
 */

#ifndef __TERRALIB_FE_INTERNAL_BINARYOPERATOR_H
#define __TERRALIB_FE_INTERNAL_BINARYOPERATOR_H

// TerraLib
#include "Expression.h"

// STL
#include <set>

namespace te
{
  namespace fe
  {
    /*!
      \class BinaryOperator
      
      \brief This class can be used to represent binary operation expressions.

      The BinaryOperator class can be used to encode
      the fundamental arithmetic operations of addition,
      subtraction, multiplication and division. Arithmetic
      operators are binary operators meaning that they
      accept two arguments and evaluate to a single result.
      <br>
      The following operators are defined in the Filter
      Encoding specification:
      <ul>
      <li>Add</li>
      <li>Sub</li>
      <li>Mul</li>
      <li>Div</li>
      </ul>
      Of course, you can extend the list of supported binary operators.

      \ingroup fe

      \sa Expression

      \todo Mostrar como a pessoa pode estender a lista de operadores binarios!
     */
    class TEFEEXPORT BinaryOperator : public Expression
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes a new BinaryOperator.

          \param opName The operator name.

          \note The BinaryOperator will not take the ownership of the given name.
         */
        BinaryOperator(const char* opName = 0);

        /*!
          \brief It initializes a new BinaryOperator.

          \param opName The operator name.
          \param first  The <i>first</i> operand. 
          \param second The <i>second</i> operand.

          \note The BinaryOperator will not take the ownership of the given name.

          \note It will take the ownership of the <i>first</i> and <i>second</i> operands.
         */
        BinaryOperator(const char* opName, Expression* first, Expression* second);

        /*! \brief Virtual destructor. */
        virtual ~BinaryOperator();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It returns the operator name.

          \return The operator name.
         */
        const char* getName() const { return m_opName; }

        /*!
          \brief It sets the <i>first operand</i>.

          \param first The <i>first operand</i>.

          \note The binary operator will take the ownership of the <i>first operand</i>.
         */
        void setFirst(Expression* first);

        /*!
          \brief It returns the <i>first operand</i>.

          \return The <i>first operand</i>.
         */
        Expression* getFirst() const;

        /*!
          \brief It sets the <i>second operand</i>.

          \param second The <i>second</i> operand.

          \note It will take the ownership of the <i>second operand</i>.
         */
        void setSecond(Expression* second);

        /*!
          \brief It returns the <i>second operand</i>.

          \return The <i>second operand</i>.
         */
        Expression* getSecond() const;

        //@}

        /** @name Expression Re-implementation
         *  Methods re-implemented from Expression.
         */
        //@{

        virtual Expression* clone() const;

        //@}

      private:

        const char* m_opName;  //!< The operator name. (Mandatory)
        Expression* m_first;   //!< First operand (an expression). (Mandatory)
        Expression* m_second;  //!< Second operand (an expression). (Mandatory)
    };

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_BINARYOPERATOR_H
