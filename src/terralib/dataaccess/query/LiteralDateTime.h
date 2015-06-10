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
  \file terralib/dataaccess/query/LiteralDateTime.h

  \brief A class that models a literal for Date and Time values.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_LITERALDATETIME_H
#define __TERRALIB_DATAACCESS_INTERNAL_LITERALDATETIME_H

// TerraLib
#include "Literal.h"

namespace te
{
// Forward declaratons
  namespace dt { class DateTime; }

  namespace da
  {
    /*!
      \class LiteralDateTime
      
      \brief A class that models a literal for Date and Time values.

      This class is just a syntatic-suggar.

      \sa Literal
    */
    class TEDATAACCESSEXPORT LiteralDateTime : public Literal
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param g The geometry value.

          \note The LiteralDateTime will take the ownership of the given geometry.
        */
        LiteralDateTime(te::dt::DateTime* d);

        /*!
          \brief Constructor.

          \param g The geometry value.
        */
        LiteralDateTime(const te::dt::DateTime& d);

        /*! \brief Copy constructor. */
        LiteralDateTime(const LiteralDateTime& rhs);

        /*! \brief Destructor. */
        ~LiteralDateTime();

        /*! Assignment operator.  */
        LiteralDateTime& operator=(const LiteralDateTime& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_LITERALDATETIME_H

