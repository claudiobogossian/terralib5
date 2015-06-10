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
  \file Key.h

  \brief This class models a key element from an XML Schema.
*/

#ifndef __TERRALIB_XSD_INTERNAL_KEY_H
#define __TERRALIB_XSD_INTERNAL_KEY_H

// TerraLib
#include "IdentityConstraint.h"

namespace te
{
  namespace xsd
  {
    /*!
      \class Key

      \brief This class models a key element from an XML Schema.

      \note Parent elements: element.
    */
    class TEXSDEXPORT Key : public IdentityConstraint
    {
      public:

        /*!
          \brief Constructor.
          
          \param id   It specifies a unique ID for the element. It may be a NULL value.
          \param name It specifies the name of the key element. It is required a non-NULL value.

          \note The Key object will take the ownership of the id and name pointers.
        */
        Key(std::string* name, Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.

          \todo Implement!
        */
        Key(const Key& rhs);

        /*! \brief Destructor. */
        ~Key();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.

          \todo Implement!
        */
        Key& operator=(const Key& rhs);

        /*!
          \brief It returns the key name.

          \return The key name.
        */
        std::string* getName() const;

        /*!
          \brief It sets the key name.

          \param name The key name.

          \note The key will take the ownership of the given name.
        */
        void setName(std::string* name);

        IdentityConstraint* clone() const;
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_KEY_H
