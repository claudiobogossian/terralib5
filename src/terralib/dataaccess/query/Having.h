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
  \file terralib/dataaccess/query/Having.h

  \brief A Having is a filter expression that can be applied to a query with a group by clause.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_HAVING_H
#define __TERRALIB_DATAACCESS_INTERNAL_HAVING_H

// TerraLib
#include "../Config.h"

// STL
#include <memory>

namespace te
{
  namespace da
  {
// Forward declaration
    class Expression;

    /*!
      \class Having

      \brief A class that can be used to model a filter expression that can be applied to a query.
    */
    class TEDATAACCESSEXPORT Having
    {
      public:

        /*!
          \brief Constructor.

          \param e The where expression.

          \note The Having clause will take the ownership of the expression.
        */
        explicit Having(Expression* e);

        explicit Having(const Expression& e);

        Having(const Having& rhs);

        ~Having();
        
        Having& operator=(const Having& rhs);

        Expression* getExp() const;

        /*!
          \brief Sets the expression.

          \param e The where expression.

          \note The Having clause will take the ownership of the expression.
        */
        void setExp(Expression* exp);

      private:

        std::auto_ptr<Expression> m_exp;  //!< The expression that defines the where clause.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_HAVING_H

