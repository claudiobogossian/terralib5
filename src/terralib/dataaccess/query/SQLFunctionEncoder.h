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
  \file terralib/dataaccess/query/SQLFunctionEncoder.h

  \brief A base class for encoders of SQL function expressions.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_SQLFUNCTIONENCODER_H
#define __TERRALIB_DATAACCESS_INTERNAL_SQLFUNCTIONENCODER_H

// TerraLib
#include "../Config.h"

// STL
#include <string>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace da
  {
// Forward declarations
    class Function;
    class SQLVisitor;

    /*!
      \class SQLFunctionEncoder
      
      \brief A base class for encoders of SQL function expressions.

      A Function is a named procedure that performs a distinct computation.

      \sa Function, TemplateEncoder, BinaryOpEncoder, UnaryOpEncoder, FunctionEncoder
    */
    class TEDATAACCESSEXPORT SQLFunctionEncoder : public boost::noncopyable
    {
      public:

        /*!
          \brief Constructor.

          \param name The function name.
        */
        SQLFunctionEncoder(const std::string& name) : m_name(name) {}

        /*! \brief Virtual Destructor. */
        virtual ~SQLFunctionEncoder() {}

        /*!
          \brief It encodes the function to a SQL notation.

          Subclasses must provide implementations for the many ways a function
          can be called in a database native language. For example, the '+' operator
          is called A + B, while the ST_Intersects function is called ST_Intersects(A, B).

          Notice that this method will visit the expression arguments of a function.

          \param f    The function to be translated to a SQL notation.
          \param buff The destination string buffer where the translation will be written.
          \param v    The SQL visitor that will be used to visit the arguments expression.
        */
        virtual void toSQL(const Function& f,
                           std::string& buff,
                           SQLVisitor& v) const = 0;

      protected:

        std::string m_name; //!< The alias to the function.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_SQLFUNCTIONENCODER_H

