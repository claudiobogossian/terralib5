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
  \file terralib/fe/BinaryComparisonOp.h
  
  \brief A class for binary comparison operators.  
 */

#ifndef __TERRALIB_FE_INTERNAL_BINARYCOMPARISONOP_H
#define __TERRALIB_FE_INTERNAL_BINARYCOMPARISONOP_H

// TerraLib
#include "ComparisonOp.h"

namespace te
{
  namespace fe
  {
// Forward declarations
    class Expression;

    /*!
      \class BinaryComparisonOp
      
      \brief A class for binary comparison operators

      The Common Catalog Query Language defines a standard
      set of comparison operators (=, <, >, >=, <=, <>). These
      comparison operators are encoded using the complex
      type BinaryComparisonOp. This type definition includes
      the matchCase attribute which is Boolean type 
      and controls whether string comparisons are
      case sensitive or not. A true value means that string
      comparisons are case sensitive. This is the default value.
      A false value means that string comparisons are
      case insensitive.<br>
      The following operators are defined in the Filter
      Encoding specification:
      <ul>
      <li>PropertyIsEqualTo</li>
      <li>PropertyIsPropertyIsNotEqualTo</li>
      <li>PropertyIsPropertyIsLessThan</li>
      <li>PropertyIsPropertyIsGreaterThan</li>
      <li>PropertyIsPropertyIsLessThanOrEqualTo</li>
      <li>PropertyIsPropertyIsGreaterThanOrEqualTo</li>
      </ul>

      \ingroup fe

      \sa ComparisonOp, Expression      
     */
    class TEFEEXPORT BinaryComparisonOp : public ComparisonOp
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes a new BinaryComparisonOp.

          \param opName The comparison operator name.

          \note The BinaryComparisonOp will not take the ownership of the given name.
         */
        BinaryComparisonOp(const char* opName = 0);

        /*!
          \brief It initializes a new BinaryComparisonOp.

          \param opName The BinaryComparisonOp operator name.
          \param f      The first expression. The BinaryComparisonOp will take the ownership of the given expression.
          \param s      The second expression. The BinaryComparisonOp will take the ownership of the given expression.

          \note The BinaryComparisonOp will not take the ownership of the given name.
         */
        BinaryComparisonOp(const char* opName, Expression* f, Expression* s);

        /*! \brief Virtual destructor. */
        virtual ~BinaryComparisonOp();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It sets the <i>first operand</i>.

          \param first The <i>first operand</i>.

          \note It will take the ownership of the <i>first operand</i>.
         */
        void setFirst(Expression* first);

        /*!
          \brief It returns the <i>first operand</i>.

          \return The <i>first operand</i>.
         */
        Expression* getFirst() const;

        /*!
          \brief It sets the <i>second operand</i>.

          \param second The <i>second operand</i>.

          \note It will take the ownership of the <i>second operand</i>.
         */
        void setSecond(Expression* second);

        /*!
          \brief It returns the <i>second operand</i>.

          \return The <i>second</i> operand.
         */
        Expression* getSecond() const;

        /*!
          \brief It enables the match case flag. So comparisons will be case sensitive.

          \note This is the default state.
         */
        void enableMatchCase();

        /*! \brief It disables the match case flag. So comparisons will not be case sensitive. */
        void disableMatchCase();

        //@}

      protected:

        Expression* m_first;   //!< first operand (mandatory).
        Expression* m_second;  //!< second operand (mandatory).
        bool m_matchCase;      //!< Optional (default: true).
    };

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_BINARYCOMPARISONOP_H

