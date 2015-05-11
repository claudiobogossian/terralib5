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
  \file ScalarCapabilities.h
  
  \brief Scalar capabilities include the ability to process logical expressions, comparisons and arithmetic operations including the ability to process a list of named functions.
 */

#ifndef __TERRALIB_FE_INTERNAL_SCALARCAPABILITIES_H
#define __TERRALIB_FE_INTERNAL_SCALARCAPABILITIES_H

// TerraLib
#include "Config.h"

namespace te
{
  namespace fe
  {
// Forward declarations
    class ArithmeticOperators;
    class ComparisonOperators;

    /*! \class ScalarCapabilities
      
        \brief Scalar capabilities include the ability to process logical expressions, comparisons and arithmetic operations including the ability to process a list of named functions.

        \sa FilterCapabilities, ComparisonOperators, ArithmeticOperators
     */
    class TEFEEXPORT ScalarCapabilities
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new ScalarCapabilities. */
        ScalarCapabilities();

        /*! \brief Destructor. */
        ~ScalarCapabilities();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It returns true if it has logical operators support, otherwise, returns false.

          \return True if it has logical operators support, otherwise, returns false.
         */
        bool hasLogicalOperators() const;

        /*! \brief It enables the logical operators support. */
        void enableLogicalOperators();

        /*! brief It disables the logical operators support. */
        void disableLogicalOperators();

        /*!
          \brief It sets the comparison operators.

          \param cOps The comparison operators.

          \note It will take the ownership of the comparison operators.
                So, don't release the comparison operators resources.
         */
        void setComparisonOperators(ComparisonOperators* cOps);

        /*!
          \brief It returns the comparison operators.

          \return The comparison operators.
         */
        const ComparisonOperators* getComparisonOperators() const;        

        /*! \brief It sets the arithmetic operators.

            \param aOps The arithmetic operators.

            \note It will take the ownership of the arithmetic operators.
                  So, don't release the arithmetic operators resources.
         */
        void setArithmeticOperators(ArithmeticOperators* aOps);

        /*!
          \brief It returns the arithmetic operators.

          \return The arithmetic operators.
         */
        const ArithmeticOperators* getArithmeticOperators() const;

        //@}

      private:

        bool m_hasLogicalOperators;                  //!< It is used to indicate that the filter can process And, Or and Not operators. Default: false. (Optional)
        ComparisonOperators* m_comparisonOperators;  //!< Optional.
        ArithmeticOperators* m_arithmeticOperators;  //!< Optional.
    };    

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_SCALARCAPABILITIES_H
