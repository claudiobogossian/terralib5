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
  \file terralib/dataaccess/query/Join.h

  \brief A Join clause combines two FromItems.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_JOIN_H
#define __TERRALIB_DATAACCESS_INTERNAL_JOIN_H

// TerraLib
#include "../Enums.h"
#include "FromItem.h"

// STL
#include <memory>

namespace te
{
  namespace da
  {
// Forward declaration
    class JoinCondition;

    /*!
      \class Join
      
      \brief A Join clause combines two FromItems.

      \sa FromItem, DataSetName, FromFunctionCall, SubSelect
    */
    class TEDATAACCESSEXPORT Join : public FromItem
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param first  The first FromItem to be combined.
          \param second The second FromItem to be combined.
          \param t      The join type.
          \param c      The join condition.

          \note The Join will take the ownership of the First, Second and JoinCondition.
        */
        Join(FromItem* first, FromItem* second, JoinType t, JoinCondition* c);

        /*!
          \brief Constructor.

          \param first  The first FromItem to be combined.
          \param second The second FromItem to be combined.
          \param t      The join type.
          \param c      The join condition.
        */
        Join(const FromItem& first, const FromItem& second, JoinType t, const JoinCondition& c);

        /*! \brief Copy constructor. */
        Join(const Join& rhs);

        /*! \brief Destructor. */
        ~Join();

        /*! Assignment operator.  */
        Join& operator=(const Join& rhs);

        /*! \brief It creates a new copy of this FromItem. */
        FromItem* clone() const;

        /*!
          \brief It returns the first from item involved in the join.

          \return The first item involved in the join.
        */
        FromItem* getFirst() const;

        /*!
          \brief It sets first item involved in the join.

          \param item The first item involved in the join.

          \note The Join will take the ownership of the given item.
        */
        void setFirst(FromItem* item);

        /*!
          \brief It returns the second item involved in the join.

          \return The second item involved in the join.
        */
        FromItem* getSecond() const;

        /*!
          \brief It sets second item involved in the join.

          \param item The second item involved in the join.

          \note The Join will take the ownership of the given item.
        */
        void setSecond(FromItem* item);

        /*!
          \brief It returns the join type.

          \return The join type.
        */
        JoinType getType() const;

        /*!
          \brief It sets join type.

          \param t The join type.
        */
        void setType(JoinType t);

        /*!
          \brief It tells if the join is Natural.

          \return True if it is a Natural join, otherwise, false.
        */
        bool isNatural() const { return m_isNatural; }

        /*!
          \brief It marks if the join is a natural join.

          \param n If true this will be a natural join.
        */
        void setNatural(bool n) { m_isNatural = n; }

        /*!
          \brief It returns the join condition.

          \return The join condition.
        */
        JoinCondition* getCondition() const;

        /*!
          \brief It sets the join condition.

          \param c The join condition.

          \note The Join will take the ownership of the given condition.
        */
        void setCondition(JoinCondition* c);

      private:

        std::auto_ptr<FromItem> m_first;          //!< The first item involved in the join.
        std::auto_ptr<FromItem> m_second;         //!< The first item involved in the join.
        std::auto_ptr<JoinCondition> m_condition; //!< The join condition.
        JoinType m_type;                          //!< The join type.
        bool m_isNatural;                         //!< Natural is a shorthand for a JoinConditionUsing list that mentions all columns in the two tables that have the same names.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_JOIN_H

