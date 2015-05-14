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
  \file PropertyIsBetween.h
  
  \brief The PropertyIsBetween element is defined as a compact way of encoding a range check.  
 */

#ifndef __TERRALIB_FE_INTERNAL_PROPERTYISBETWEEN_H
#define __TERRALIB_FE_INTERNAL_PROPERTYISBETWEEN_H

// TerraLib
#include "ComparisonOp.h"

namespace te
{
  namespace fe
  {
    /*!
      \class PropertyIsBetween
      
      \brief The PropertyIsBetween element is defined as a compact way of encoding a range check.

      The lower and upper boundary values are inclusive.

      \ingroup fe

      \sa ComparisonOp, Expression
     */
    class TEFEEXPORT PropertyIsBetween : public ComparisonOp
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new PropertyIsBetween. */
        PropertyIsBetween();

        /*! \brief Destructor. */
        ~PropertyIsBetween();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It sets the between expression.

          \param e The between expression.

          \note It will take the ownership of the between expression.

         */
        void setExpression(Expression* e);

        /*!
          \brief It returns the between expression.

          \return The between expression.
         */
        Expression* getExpression() const;

        /*!
          \brief It sets the lower boundary expression.

          \param e The lower boundary expression.

          \note It will take the ownership of the lower boundary expression.
         */
        void setLowerBoundary(Expression* e);

        /*!
          \brief It returns the lower boundary expression.

          \return The lower boundary expression.
         */
        Expression* getLowerBoundary() const;

        /*!
          \brief It sets the lower boundary expression.

          \param e The upper boundary expression.

          \note It will take the ownership of the upper boundary expression.
         */
        void setUpperBoundary(Expression* e);

        /*!
          \brief It returns the upper boundary expression.

          \return The upper boundary expression.
         */
        Expression* getUpperBoundary() const;

        //@}

      private:

        Expression* m_expression;    //!< Mandatory.
        Expression* m_lowerBoundary; //!< Mandatory.
        Expression* m_upperBoundary; //!< Mandatory.
    };

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_PROPERTYISBETWEEN_H
