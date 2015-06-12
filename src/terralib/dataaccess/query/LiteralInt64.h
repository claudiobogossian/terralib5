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
  \file terralib/dataaccess/query/LiteralInt64.h

  \brief A class that models a Literal for a integer value.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_LITERALINT64_H
#define __TERRALIB_DATAACCESS_INTERNAL_LITERALINT64_H

// TerraLib
#include "Literal.h"

// STL
#include <string>

// Boost
#include <boost/cstdint.hpp>

namespace te
{
  namespace da
  {
    /*!
      \class LiteralInt
      
      \brief A class that models a Literal for a integer value.

      \sa Literal
    */
    class TEDATAACCESSEXPORT LiteralInt64 : public Literal
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param i The integer value.
        */
        LiteralInt64(boost::int64_t i);

        /*! \brief Copy constructor. */
        LiteralInt64(const LiteralInt64& rhs);

        /*! \brief Destructor. */
        ~LiteralInt64() {}

        /*! Assignment operator.  */
        LiteralInt64& operator=(const LiteralInt64& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_LITERALINT64_H

