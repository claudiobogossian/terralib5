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
  \file terralib/dataaccess/query/LiteralByteArray.h

  \brief A class that models a literal for ByteArray values.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_LITERALBYTEARRAY_H
#define __TERRALIB_DATAACCESS_INTERNAL_LITERALBYTEARRAY_H

// TerraLib
#include "Literal.h"

namespace te
{
// Forward declaratons
  namespace dt { class ByteArray; }

  namespace da
  {
    /*!
      \class LiteralByteArray
      
      \brief A class that models a literal for ByteArray values.

      This class is just a syntatic-suggar.

      \sa Literal
    */
    class TEDATAACCESSEXPORT LiteralByteArray : public Literal
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param b The ByteArray value.

          \note The LiteralByteArray will take the ownership of the given byte array.
        */
        LiteralByteArray(te::dt::ByteArray* b);

        /*!
          \brief Constructor.

          \param b The ByteArray value.
        */
        LiteralByteArray(const te::dt::ByteArray& b);

        /*! \brief Copy constructor. */
        LiteralByteArray(const LiteralByteArray& rhs);

        /*! \brief Destructor. */
        ~LiteralByteArray();

        /*! Assignment operator.  */
        LiteralByteArray& operator=(const LiteralByteArray& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_LITERALBYTEARRAY_H

