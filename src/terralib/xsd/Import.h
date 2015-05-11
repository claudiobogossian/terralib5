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
  \file Import.h

  \brief It models a XML Schema import.
*/

#ifndef __TERRALIB_XSD_INTERNAL_IMPORT_H
#define __TERRALIB_XSD_INTERNAL_IMPORT_H

// TerraLib
#include "Annotated.h"
#include "Identifiable.h"

// STL
#include <string>

namespace te
{
  namespace xsd
  {
    /*!
      \class Import

      \brief It models a XML Schema import.

      \sa Annotated, Identifiable
    */
    class TEXSDEXPORT Import : public Identifiable, public Annotated
    {
      public:

        /*!
          \brief Constructor.

          \param id It specifies a unique ID for the element. It can be a NULL value.
          \param ns The URI of the namespace to import.
          \param schemaLocation The URI to the schema for the imported namespace.
        */
        Import(std::string* ns, std::string* schemaLocation, Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.

          \todo Implement!
        */
        Import(const Import& rhs);

        /*! \brief Destructor. */
        ~Import();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.

          \todo Implement!
        */
        Import& operator=(const Import& rhs);

        /*!
          \brief It returns the namespace URI of the imported schema.

          \return The namespace URI.
        */
        std::string* getNamespace() const;

        /*!
          \brief It returns the location of the referenced schema.

          \return The schema location.
        */
        std::string* getSchemaLocation() const;

        /*!
          \brief It sets the namespace URI of the imported schema.

          \param ns A namespace URI value.
  
          \note The Import object will take the ownership of the given pointer.
        */
        void setNamespace(std::string* ns);

        /*!
          \brief It sets the location of the referenced schema.

          \param schemaLocation A schema location.
  
          \note The Import object will take the ownership of the given pointer.
        */
        void setSchemaLocation(std::string* schemaLocation);

      private:

        std::string* m_namespace;      //!< It specifies the URI of the namespace to import. (Optional)
        std::string* m_schemaLocation; //!< It specifies the URI to the schema for the imported namespace. (Optional)
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_IMPORT_H
