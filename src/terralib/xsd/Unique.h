/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file Unique.h

  \brief It models the unique element in an XML Schema.
*/

#ifndef __TERRALIB_XSD_INTERNAL_UNIQUE_H
#define __TERRALIB_XSD_INTERNAL_UNIQUE_H

// TerraLib
#include "IdentityConstraint.h"

namespace te
{
  namespace xsd
  {
    /*!
      \class Unique

      \brief It models the unique element in an XML Schema.
      
      The unique element defines that an element or an attribute value must be unique within the scope.
    */
    class TEXSDEXPORT Unique : public IdentityConstraint
    {
      public:

        /*!
          \brief Constructor.
          
          \param id   It specifies a unique ID for the element. It may be a NULL value.
          \param name It specifies the name of the unique element. It is required a non-NULL value.

          \note The Unique object will take the ownership of the id and name pointers.
        */
        Unique(std::string* name, Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.

          \todo Implement!
        */
        Unique(const Unique& rhs);

        /*! \brief Destructor. */
        ~Unique();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.

          \todo Implement!
        */
        Unique& operator=(const Unique& rhs);

        IdentityConstraint* clone() const;
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_UNIQUE_H
