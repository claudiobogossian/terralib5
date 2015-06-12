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
  \file terralib/dataaccess/query/Expression.h
  
  \brief This is an abstract class that models a query expression.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_EXPRESSION_H
#define __TERRALIB_DATAACCESS_INTERNAL_EXPRESSION_H

// TerraLib
#include "../../common/BaseVisitable.h"
#include "QueryVisitor.h"

namespace te
{
  namespace da
  {
    /*!
      \class Expression
      
      \brief This is an abstract class that models a query expression.

      An expression is a combination of one or more
      functions, literals or property names that can be evaluated.

      \sa PropertyName, Function, Literal
    */
    class TEDATAACCESSEXPORT Expression : public te::common::BaseVisitable<QueryVisitor>
    {
      public:

        TE_DEFINE_VISITABLE

        /*! \brief Default constructor. */
        Expression() {}

        /*! \brief Virtual destructor. */
        virtual ~Expression() {}

        /*! \brief It creates a new copy of this expression. */
        virtual Expression* clone() const = 0;
    };

    /*! \brief For use with boost conteiners. */
    inline Expression* new_clone(const Expression& a)
    {
      return a.clone();
    }

  } // end namespace da
}   // end namespace te

TEDATAACCESSEXPORT te::da::Expression* operator==(const te::da::Expression& e1, const te::da::Expression& e2);
TEDATAACCESSEXPORT te::da::Expression* operator!=(const te::da::Expression& e1, const te::da::Expression& e2);
TEDATAACCESSEXPORT te::da::Expression* operator&&(const te::da::Expression& e1, const te::da::Expression& e2);
TEDATAACCESSEXPORT te::da::Expression* operator||(const te::da::Expression& e1, const te::da::Expression& e2);
TEDATAACCESSEXPORT te::da::Expression* operator~(const te::da::Expression& e);
TEDATAACCESSEXPORT te::da::Expression* operator>(const te::da::Expression& e1, const te::da::Expression& e2);
TEDATAACCESSEXPORT te::da::Expression* operator>=(const te::da::Expression& e1, const te::da::Expression& e2);
TEDATAACCESSEXPORT te::da::Expression* operator<(const te::da::Expression& e1, const te::da::Expression& e2);
TEDATAACCESSEXPORT te::da::Expression* operator<=(const te::da::Expression& e1, const te::da::Expression& e2);

#endif  // __TERRALIB_DATAACCESS_INTERNAL_EXPRESSION_H

