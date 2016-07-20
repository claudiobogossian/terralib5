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
  \file ComplexContent.h

  \brief It models an XML Schema complexContent element.
*/

#ifndef __TERRALIB_XSD_INTERNAL_COMPLEXCONTENT_H
#define __TERRALIB_XSD_INTERNAL_COMPLEXCONTENT_H

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
      \class ComplexContent

      \brief It models an XML Schema complexContent element.

      The ComplexContent element defines extensions or restrictions on a complex type that contains mixed content or elements only.

      \note Parent elements: complexType
    */
    class TEXSDEXPORT ComplexContent : public Identifiable, public Annotated
    {
      public:

        /*!
          \brief Constructor.
          
          \param dev It specifies the element content and can be: restriction | extension.
          \param ann An annotation.
          \param id  It specifies a unique ID for the element.

          \note The ComplexContent object will take the ownership of the pointers.
        */
        ComplexContent(TypeDerivation* dev = 0, Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.
        */
        ComplexContent(const ComplexContent& rhs);

        /*! \brief Destructor. */
        ~ComplexContent();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.
        */
        ComplexContent& operator=(const ComplexContent& rhs);

        /*!
          \brief If the ComplexContent is mixed or not.

          \return True if the ComplexContent is mixed and false if not.
        */
        bool isMixed();

        /*!
          \brief It returns the derivation type of this ComplexContent.

          \return The derivation type: restriction | extension.
        */
        TypeDerivation* getTypeDerivation() const;

        /*!
          \brief It specifies whether character data is allowed to appear between the child elements of this complexType element.

          \param v True indicates that character data is allowed to appear between the child elements of this complexType element.
        */
        void setAsMixed(bool v);

        /*!
          \brief It sets the derivation type of this ComplexContent: (restriction | extension).

          \param t The derivation type of this ComplexContent.

          \note The ComplexContent object will take the ownership of the given TypeDerivation.
        */
        void setTypeDerivation(TypeDerivation* t);

      private:

        bool m_mixed;                   //!< It specifies whether character data is allowed to appear between the child elements of this complexType element. Default is false. (Optional)
        TypeDerivation* m_derivation;   //!< It can be: restriction (Restriction4ComplexContent) | extension. (Required)
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_COMPLEXCONTENT_H
