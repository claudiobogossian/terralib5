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
  \file Notation.h

  \brief This class models a notation element from a XML Schema.
*/

#ifndef __TERRALIB_XSD_INTERNAL_NOTATION_H
#define __TERRALIB_XSD_INTERNAL_NOTATION_H

// TerraLib
#include "Annotated.h"
#include "Identifiable.h"

namespace te
{
  namespace xsd
  {
    /*!
      \class Notation

      \brief This class models a notation element from a XML Schema.

      \note Parent elements: schema.
    */
    class TEXSDEXPORT Notation : public Identifiable, public Annotated
    {
      public:

        /*!
          \brief Constructor.
          
          \param id   It specifies a unique ID for the element. It can be a NULL value.
          \param name It specifies a name for the element. It can not be a NULL value.
          \param pub  It specifies a URI corresponding to the public identifier. It can not be a NULL value.

          \note The Notation object will take the ownership of the pointers id, name and pub.
        */
        Notation(std::string* name, std::string* pub, Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.

          \todo Implement!
        */
        Notation(const Notation& rhs);

        /*! \brief Destructor. */
        ~Notation();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.

          \todo Implement!
        */
        Notation& operator=(const Notation& rhs);

        /*!
          \brief It returns the name of this element.

          \return The name of this element.
        */
        std::string* getName() const;

        /*!
          \brief It returns the URI corresponding to the public identifier.

          \return The URI corresponding to the public identifier.
        */
        std::string* getPublic() const;

        /*!
          \brief It returns the URI corresponding to the system identifier.

          \return The URI corresponding to the system identifier.
        */
        std::string* getSystem() const;

        /*!
          \brief It sets the name of this element.

          \param name The name of this element.
  
          \note The Notation object will take the ownership of the given pointer.
        */
        void setName(std::string* name);

        /*!
          \brief It sets the URI corresponding to the public identifier.

          \param pub The URI corresponding to the public identifier.
  
          \note The Notation object will take the ownership of the given pointer.
        */
        void setPublic(std::string* pub);

        /*!
          \brief It sets the URI corresponding to the system identifier.

          \param sys The URI corresponding to the system identifier.
  
          \note The Notation object will take the ownership of the given pointer.
        */
        void setSystem(std::string* sys);

      private:

        std::string* m_name;       //!< It specifies a name for the element. (Required)
        std::string* m_public;     //!< It specifies a URI corresponding to the public identifier. (Required)
        std::string* m_system;     //!< It specifies a URI corresponding to the system identifier. (Optional)
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_NOTATION_H
