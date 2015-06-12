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
  \file terralib/dataaccess/query/JoinConditionOn.h

  \brief JoinConditionOn is a boolean expression and it specifies which items in a join are considered to match.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_JOINCONDITIONON_H
#define __TERRALIB_DATAACCESS_INTERNAL_JOINCONDITIONON_H

// TerraLib
#include "JoinCondition.h"

// STL
#include <memory>

namespace te
{
  namespace da
  {
// Forward declaraton
    class Expression;

    /*!
      \class JoinConditionOn
      
      \brief JoinConditionOn is a boolean expression and it specifies which items in a join are considered to match.

      \sa JoinCondition, JoinConditionUsing
    */
    class TEDATAACCESSEXPORT JoinConditionOn : public JoinCondition
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.
          
          \param e A join condition expression.

          \note The JoinConditionOn will take the ownership of the given expression.
        */
        JoinConditionOn(Expression* e);

        /*!
          \brief Constructor.
          
          \param e A join condition expression.
        */
        JoinConditionOn(const Expression& e);

        /*! \brief Copy constructor. */
        JoinConditionOn(const JoinConditionOn& rhs);

        /*! \brief Destructor. */
        ~JoinConditionOn();

        /*! Assignment operator.  */
        JoinConditionOn& operator=(const JoinConditionOn& rhs);

        /*! \brief It creates a new copy of this JoinCondition. */
        JoinCondition* clone() const;

        /*!
          \brief It sets the join condition expression.

          \param c A join condition expression.

          \note The JoinConditionOn will take the ownership of the given expression.
        */
        void setCondition(Expression* c);

        /*!
          \brief It returns a pointer to a join condition.

          \return A pointer to a join condition.
        */
        Expression* getCondition() const;

      private:

        std::auto_ptr<Expression> m_condition;  //!< The join expression.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_JOINCONDITIONON_H

