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
  \file terralib/fe/Function.h
  
  \brief A function is a named procedure that performs a distinct computation.
 */

#ifndef __TERRALIB_FE_INTERNAL_FUNCTION_H
#define __TERRALIB_FE_INTERNAL_FUNCTION_H

// TerraLib
#include "Expression.h"

// STL
#include <string>
#include <vector>

namespace te
{
  namespace fe
  {
    /*!
      \class Function
      
      \brief A function is a named procedure that performs a distinct computation.

      A function may accept zero or more arguments
      as input and generates a single result.

      \ingroup fe

      \sa Expression

      \todo Remover o uso do atributo nome de forma que as funcoes sejam registradas e de forma que se possa manter um pool das funcoes com os argumentos previamente alocados para cada tipo de funcao.
     */
    class TEFEEXPORT Function : public Expression
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes a new Function.

          \param name Function name. Must be a non-empty value.
         */
        Function(const std::string& name);

        /*! \brief Destructor. */
        ~Function();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It returns the function name.

          \return The function name.
         */
        const std::string& getName() const;

        /*!
          \brief It sets the function name.

          \param n The function name.
         */
        void setName(const std::string& n);

        /*!
          \brief It returns the argument in the specified position.

          \param i The argument index.

          \return The argument in the specified position.

          \note The argument index start at 0.

          \note It doesn't check the index range.
         */
        Expression* getArgument(size_t i) const;

        /*!
          \brief It adds a new argument to the function.

          \param argument The argument to be added. The Function object will take the ownership of the given argument.
         */
        void add(Expression* argument);

        /*!
          \brief It sets the argument in the specified position.

          \param i        The argument index.
          \param argument The new argument. The Function object will take the ownership of the given argument.

          \note The argument index start at 0.

          \note Be sure that the argument list is already defined having the location
                you are specifying.

          \note It doesn't check the index range.
         */
        void setArgument(size_t i, Expression* argument);

        /*!
          \brief It removes the n-th argument.

          \param i The argument index.

          \note It doesn't check the index range.
         */
        void removeArgument(size_t i);
        
        //@}

        /** @name Expression Re-implementation
         *  Methods re-implemented from Expression.
         */
        //@{

        virtual Expression* clone() const;

        //@}

      protected:

        std::string m_name;                       //!< Function name. (Mandatory)
        std::vector<Expression*> m_argumentList;  //!< Argument list (actually, a list of expressions). (Optional)
    };

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_FUNCTION_H
