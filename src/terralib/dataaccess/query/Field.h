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
  \file terralib/dataaccess/query/Field.h

  \brief The Field class can be used to model an expression that takes part of the output items of a SELECT.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_FIELD_H
#define __TERRALIB_DATAACCESS_INTERNAL_FIELD_H

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
      \class Field
      
      \brief The Field class can be used to model an expression that takes part of the output items of a SELECT.

      \sa Fields
    */
    class TEDATAACCESSEXPORT Field
    {
      public:

        /*!
          \brief Constructor.

          \param e     The output expression to be used in a Select clause.
          \param alias An alias to the output expression.
        */
        Field(const Expression& e, const std::string& alias = std::string(""));

        /*!
          \brief Constructor.

          \param e     The output expression to be used in a Select clause.
          \param alias An alias to the output expression.

          \note The Field will take the ownership of Expression and alias.
        */
        Field(Expression* e, std::string* alias = 0);

        /*!
          \brief Constructor.

          \param propertyName The property name.
          \param alias        The property alias.
        */
        Field(const std::string& propertyName, const std::string& alias = std::string(""));

        /*! \brief Copy constructor. */
        Field(const Field& rhs);

        /*! \brief Destructor. */
        ~Field();

        /*! Assignment operator.  */
        Field& operator=(const Field& rhs);

        /*!
          \brief It sets the an expression that can be used in a Select query.

          \param e An expression that can be used in a Select query.

          \note The Field will take the ownership of the given expression.
        */
        void setExpression(Expression* e);

        /*!
          \brief It returns the expression set for an output select query.

          \return The expression set for an output select query.
        */
        Expression* getExpression() const;

        /*!
          \brief As you know, sometimes is better to give an alias to an output expression.

          \param alias An aliast fo the output expression.

          \note The Field object will take the ownership of the alias.
        */
        void setAlias(std::string* alias);

        /*!
          \brief It returns the alias for the outout expression.

          \return The alias for the outout expression.
        */
        std::string* getAlias() const;

      private:

        std::auto_ptr<Expression> m_field;  //!< A valid expression.
        std::auto_ptr<std::string> m_alias; //!< An alias for the output name.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_FIELD_H

