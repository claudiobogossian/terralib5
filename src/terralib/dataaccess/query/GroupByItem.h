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
  \file terralib/dataaccess/query/GroupByItem.h

  \brief A class that can be used in a GROUP BY clause.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_GROUPBYITEM_H
#define __TERRALIB_DATAACCESS_INTERNAL_GROUPBYITEM_H

// TerraLib
#include "../Config.h"

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
      \class GroupByItem
      
      \brief A class that can be used in a GROUP BY clause.

      \sa GroupBy
    */
    class TEDATAACCESSEXPORT GroupByItem
    {
      public:   

        /*!
          \brief Constructor.

          \param e     An expression to be used in an GROUP BY clause.
          \param order The sort order.
        */
        explicit GroupByItem(const Expression& e);

        /*!
          \brief Constructor.

          \param e     An expression to be used in an GROUP BY clause.
          \param order The sort order.

          \note The GroupByItem will take the ownership of Expression.
        */
        explicit GroupByItem(Expression* e);

        /*!
          \brief Constructor.

          \param propertyName  A property name.
          \param order         The sort order.
        */
        explicit GroupByItem(const std::string& propertyName);

        /*! \brief Copy constructor. */
        explicit GroupByItem(const GroupByItem& rhs);

        /*! \brief Destructor. */
        ~GroupByItem();        

        /*! Assignment operator.  */
        GroupByItem& operator=(const GroupByItem& rhs);

        /*!
          \brief It sets the expression to sort the result of a query.

          \param e The expression to be used to sort the result of a query.

          \note The GroupByItem will take the expression ownership.
        */
        void setExpression(Expression* e);

        /*!
          \brief It returns the exprsssion to be used to sort the result of a query.

          \return The exprsssion to be used to sort the result of a query.
        */
        Expression* getExpression() const;

      private:

        std::auto_ptr<Expression> m_field;  //!< A valid expression.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_GROUPBYITEM_H

