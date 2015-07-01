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
  \file terralib/dataaccess/query/OrderByItem.h

  \brief A class that can be used in an ORDER BY clause to sort the items of a resulting query.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_ORDERBYITEM_H
#define __TERRALIB_DATAACCESS_INTERNAL_ORDERBYITEM_H

// TerraLib
#include "../Config.h"
#include "../Enums.h"

// STL
#include <memory>
#include <string>

namespace te
{
  namespace da
  {
// Forward declarations
    class Expression;

    /*!
      \class OrderByItem
      
      \brief A class that can be used in an ORDER BY clause to sort the items of a resulting query.

      The order default will be ascendent if none is informed.

      \sa OrderBy
    */
    class TEDATAACCESSEXPORT OrderByItem
    {
      public:   

        /*!
          \brief Constructor.

          \param e     An expression to be used in an ORDER BY clause.
          \param order The sort order.
        */
        explicit OrderByItem(const Expression& e, SortOrder order = ASC);

        /*!
          \brief Constructor.

          \param e     An expression to be used in an ORDER BY clause.
          \param order The sort order.

          \note The OrderByItem will take the ownership of Expression.
        */
        explicit OrderByItem(Expression* e, SortOrder order = ASC);

        /*!
          \brief Constructor.

          \param propertyName  A property name.
          \param order         The sort order.
        */
        explicit OrderByItem(const std::string& propertyName, SortOrder order = ASC);

        /*! \brief Copy constructor. */
        explicit OrderByItem(const OrderByItem& rhs);

        /*! \brief Destructor. */
        ~OrderByItem();        

        /*! Assignment operator.  */
        OrderByItem& operator=(const OrderByItem& rhs);

        /*!
          \brief It sets the expression to sort the result of a query.

          \param e The expression to be used to sort the result of a query.

          \note The OrderByItem will take the expression ownership.
        */
        void setExpression(Expression* e);

        /*!
          \brief It returns the exprsssion to be used to sort the result of a query.

          \return The exprsssion to be used to sort the result of a query.
        */
        Expression* getExpression() const;

        /*!
          \brief It sets the order to be used during the sorting of a query.

          \param o The order to be used during the sorting of a query.
        */
        void setSortOrder(SortOrder o);

        /*!
          \brief It returns the order of sorting: Asc or Desc.

          \return The order of sorting: Asc or Desc.
        */
        SortOrder getSortOrder() const;

      private:

        std::auto_ptr<Expression> m_field;  //!< A valid expression.
        SortOrder m_order;                  //!< Sort order.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_ORDERBYITEM_H

