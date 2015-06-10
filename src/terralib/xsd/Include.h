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
  \file Include.h

  \brief It models a XML Schema include.
*/

#ifndef __TERRALIB_XSD_INTERNAL_INCLUDE_H
#define __TERRALIB_XSD_INTERNAL_INCLUDE_H

// TerraLib
#include "Annotated.h"
#include "Identifiable.h"

namespace te
{
  namespace xsd
  {
    /*!
      \class Include

      \brief It models a XML Schema include.

      \sa Annotated, Identifiable

      \note Parent elements: schema.
    */
    class TEXSDEXPORT Include : public Identifiable, public Annotated
    {
      public:        

        /*!
          \brief Constructor.

          \param id It specifies a unique ID for the element. It can be a NULL value.
          \param schemaLocation It specifies the URI to the schema to include in the target namespace of the containing schema.
        */
        Include(const std::string& schemaLocation, Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.

          \todo Implement!
        */
        Include(const Include& rhs);

        /*! \brief Destructor. */
        ~Include();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.

          \todo Implement!
        */
        Include& operator=(const Include& rhs);

        /*!
          \brief It returns the location of the referenced schema.

          \return The schema location.
         */
        const std::string& getSchemaLocation() const;

        /*!
          \brief It sets the location of the referenced schema.

          \param schemaLocation The schema location.
         */
        void setSchemaLocation(const std::string& schemaLocation);

      private:

        std::string m_schemaLocation; //!< It specifies the URI to the schema to include in the target namespace of the containing schema. (Required)
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_INCLUDE_H
