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
  \file terralib/dataaccess/query/BinaryOpEncoder.h

  \brief A query encoder for binary operations.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_BINARYOPENCODER_H
#define __TERRALIB_DATAACCESS_INTERNAL_BINARYOPENCODER_H

// TerraLib
#include "SQLFunctionEncoder.h"

namespace te
{
  namespace da
  {
    /*!
      \class BinaryOpEncoder
      
      \brief A query encoder for binary operations.

      It encodes a binary operator using the following convention: (arg1 op-name arg2).

      \sa Function, TemplateEncoder, SQLFunctionEncoder, UnaryOpEncoder, FunctionEncoder, FunctionNoArgsEncoder
    */
    class TEDATAACCESSEXPORT BinaryOpEncoder : public SQLFunctionEncoder
    {
      public:

        /*!
          \brief Constructor.

          \param name An alias to the operator name.
        */
        BinaryOpEncoder(const std::string& name) : SQLFunctionEncoder(name) {}

        /*! \brief Destructor. */
        ~BinaryOpEncoder() {}

        /*!
          \brief It encodes the function to a SQL notation.

          This method will encode the function in infix binary operator notation: (firstOperand op secondOperand).
          For example, the '+' function is encoded as: (A + B).

          \param f     The function to be translated to a SQL notation.
          \param alias The function alias in the data source query language.
          \param buff  The destination string buffer where the translation will be written.
          \param v     The SQL visitor that will be used to visit the arguments expression.
        */
        void toSQL(const Function& f,
                   std::string& buff,
                   SQLVisitor& v) const;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_BINARYOPENCODER_H

