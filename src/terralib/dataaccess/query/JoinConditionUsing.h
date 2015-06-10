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
  \file terralib/dataaccess/query/JoinConditionUsing.h

  \brief JoinConditionUsing class can be used to model a USING clause in a Join.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_JOINCONDITIONUSING_H
#define __TERRALIB_DATAACCESS_INTERNAL_JOINCONDITIONUSING_H

// TerraLib
#include "JoinCondition.h"

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{
  namespace da
  {
    /*!
      \class JoinConditionUsing
      
      \brief JoinConditionUsing class can be used to model a USING clause in a Join.

      \sa JoinCondition, JoinConditionOn
    */
    class TEDATAACCESSEXPORT JoinConditionUsing : public JoinCondition
    {
      public:

        TE_DEFINE_VISITABLE

        /*! \brief Default constructor. */
        JoinConditionUsing() {}

        /*! \brief Copy constructor. */
        JoinConditionUsing(const JoinConditionUsing& rhs);

        /*! \brief Destructor. */
        ~JoinConditionUsing();

        /*! Assignment operator.  */
        JoinConditionUsing& operator=(const JoinConditionUsing& rhs);

        /*! \brief It creates a new copy of this JoinCondition. */
        JoinCondition* clone() const;

        /*!
          \brief It returns the number of fields in this join condition.

          \return The number of fields in this join condition.
        */
        std::size_t getNumFields() const { return m_fields.size(); }

        /*!
          \brief It adds the condition to the conteiner.

          \param e The condition to be added to the join.
        */
        void push_back(Expression* e);

        /*!
          \brief It returns the i-th condition.

          \param i The condition position.

          \return The i-th condition.
        */
        const Expression* operator[](std::size_t i) const;

        /*!
          \brief It releases the i-th condition expression and trim the conteiner.

          \param i The condition to be removed.
        */
        void erase(std::size_t i);

        /*! \brief It will release all condition expressions and then it will clear the conteiner. */
        void clear();

      private:

        boost::ptr_vector<Expression> m_fields;  //!< The join column list.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_JOINCONDITIONUSING_H

