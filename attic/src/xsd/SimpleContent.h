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
  \file SimpleContent.h

  \brief A class that models an XML Schema simpleContent element.
*/

#ifndef __TERRALIB_XSD_INTERNAL_SIMPLECONTENT_H
#define __TERRALIB_XSD_INTERNAL_SIMPLECONTENT_H

// TerraLib
#include "Annotated.h"
#include "Identifiable.h"

namespace te
{
  namespace xsd
  {
// Forward declarations
    class TypeDerivation;

    /*!
      \class SimpleContent

      \brief A class that models an XML Schema simpleContent element.
      
      The simpleContent element contains extensions or restrictions on a text-only complex type or 
      on a simple type as content and contains no elements.

      \note Parent elements: complexType.
    */
    class TEXSDEXPORT SimpleContent : public Identifiable, public Annotated
    {
      public:

        /*!
          \brief Constructor.
          
          \param dev It specifies the element content and can be: restriction | extension.
          \param ann An annotation.
          \param id  It specifies a unique ID for the element.

          \note The SimpleContent object will take the ownership of the pointers.
        */
        SimpleContent(TypeDerivation* dev = 0, Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.
        */
        SimpleContent(const SimpleContent& rhs);

        /*! \brief Destructor. */
        ~SimpleContent();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.
        */
        SimpleContent& operator=(const SimpleContent& rhs);

        /*!
          \brief It returns the derivation type of this SimpleContent.

          \return The derivation type: restriction | extension.
        */
        TypeDerivation* getTypeDerivation() const;

        /*!
          \brief It sets the derivation type of this SimpleContent.

          \param dev The derivation type: restriction | extension.
        */
        void setTypeDerivation(TypeDerivation* dev);

      private:

        TypeDerivation* m_derivation; //!< It can be: restriction (Restriction4SimpleContent) | extension. (Required)
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_SIMPLECONTENT_H
