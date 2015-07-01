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
  \file terralib/dataaccess/query/Function.h

  \brief A class that models a Function expression.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_FUNCTION_H
#define __TERRALIB_DATAACCESS_INTERNAL_FUNCTION_H

// TerraLib
#include "Expression.h"

// STL
#include <string>
#include <vector>

namespace te
{
  namespace da
  {
    /*!
      \class Function
      
      \brief A class that models a Function expression.

      \sa Expression
    */
    class TEDATAACCESSEXPORT Function : public Expression
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.
          
          \param name The function name.

          \note See FunctionNames.h for a list of common names.
        */
        Function(const std::string& name) : m_name(name) {}

        /*! \brief Copy constructor. */
        Function(const Function& rhs);

        /*! \brief Virtual Destructor. */
        virtual ~Function();

        /*! Assignment operator.  */
        Function& operator=(const Function& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;

        /*!
          \brief It returns the function name.

          \return The function name.
        */
        const std::string& getName() const { return m_name; };

        /*!
          \brief It returns the number of arguments informed to the function.

          \return The number of parameters.
        */
        std::size_t getNumArgs() const;

        /*!
          \brief It returns the i-th function argument.

          \param i The argument position.

          \return The it-th function argument.
        */
        Expression* getArg(std::size_t i) const;

        /*!
          \brief It returns the i-th function argument.

          \param i The argument position.

          \return The it-th function argument.
        */
        Expression* operator[](std::size_t i) const;

        /*!
          \brief It adds the argument to the function list of arguments.

          \param arg The argument to be added.

          \note The Function will take the ownership of the given argument.
        */
        void add(Expression* arg);

      protected:

        std::string m_name;                //!< The function name. May be an operator symbol or just a regular name like ST_Intersects.
        std::vector<Expression*> m_args;   //!< The list of arguments.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_FUNCTION_H

