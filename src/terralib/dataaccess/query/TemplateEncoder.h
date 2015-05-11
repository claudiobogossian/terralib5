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
  \file terralib/dataaccess/query/TemplateEncoder.h

  \brief A query encoder for functions that needs special translation.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_TEMPLATEENCODER_H
#define __TERRALIB_DATAACCESS_INTERNAL_TEMPLATEENCODER_H

// TerraLib
#include "SQLFunctionEncoder.h"

// STL
#include <vector>

namespace te
{
  namespace da
  {
    /*!
      \class TemplateEncoder
      
      \brief A query encoder for binary operator expressions.

      You can use a special notation in order to translate the function arguments to a specific dialect convetion.

      You can write an expression like: ($1 from $2 for $3) in order to write
      a function with arguments in differnt positions.
      The $n numbers will be used to map the arguments to its right place.

      \sa Function, TemplateEncoder, AbstractOperatorEncoder, UnaryOperatorEncoder, FunctionEncoder, FunctionNoArgsEncoder
     */
    class TEDATAACCESSEXPORT TemplateEncoder : public SQLFunctionEncoder
    {
      public:

        /*!
          \brief Constructor.
        
          \param name The function alias name.
          \param t    The expression template.
        */
        TemplateEncoder(const std::string& name, const std::string& t);

        /*! \brief Destructor. */
        ~TemplateEncoder() {}

        /*!
          \brief It encodes the function to a SQL notation.

          \param f     The function to be translated to a SQL notation.
          \param buff  The destination string buffer where the translation will be written.
          \param v     The SQL visitor that will be used to visit the arguments expression.
        */
        void toSQL(const Function& f,
                   std::string& buff,
                   SQLVisitor& v) const;

      private:

        std::vector<std::string> m_chuncks; //!< We split the input template into some pieces and make them prepared to concatenate.
        std::vector<std::size_t> m_argIdx;  //!< The arguments index.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_TEMPLATEENCODER_H

