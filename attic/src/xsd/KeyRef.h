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
  \file KeyRef.h

  \brief This class models a keyref element from a XML Schema element.
*/

#ifndef __TERRALIB_XSD_INTERNAL_KEYREF_H
#define __TERRALIB_XSD_INTERNAL_KEYREF_H

// TerraLib
#include "IdentityConstraint.h"

namespace te
{
  namespace xsd
  {
// Forward declarations
    class QName;

    /*!
      \class KeyRef

      \brief This class models a keyref element from a XML Schema element.

      \note Parent elements: element.
    */
    class TEXSDEXPORT KeyRef : public IdentityConstraint
    {
      public:

        /*!
          \brief Constructor.
          
          \param id    It specifies a unique ID for the element. It may be a NULL value.
          \param name  It specifies the name of the keyref element. It is required a non-NULL value.
          \param ref   It specifies the name of a key or unique element defined in this or another schema. It is required a non-NULL value.

          \note The KeyRef object will take the ownership of the given pointers.
        */
        KeyRef(QName* refer, std::string* name, Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.

          \todo Implement!
        */
        KeyRef(const KeyRef& rhs);

        /*! \brief Destructor. */
        ~KeyRef();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.

          \todo Implement!
        */
        KeyRef& operator=(const KeyRef& rhs);

        /*!
          \brief It returns the reference to a named element.

          \return The reference to a named element.
        */
        QName* getRef() const;

        /*!
          \brief It sets a reference to a named attribute.

          \param ref A reference to a named attribute. Required a non-NULL value.
  
          \note The KeyRef object will take the ownership of the given pointer.
        */
        void setRef(QName* ref);

        IdentityConstraint* clone() const;

      private:

        QName* m_ref; //!< It specifies the name of a key or unique element defined in this or another schema. (Required)
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_KEYREF_H
