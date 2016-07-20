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
  \file Restriction4ComplexContent.h

  \brief The restriction class can be used to define restrictions on a complexContent.
*/

#ifndef __TERRALIB_XSD_INTERNAL_RESTRICTION4COMPLEXCONTENT_H
#define __TERRALIB_XSD_INTERNAL_RESTRICTION4COMPLEXCONTENT_H

// TerraLib
#include "Annotated.h"
#include "Identifiable.h"
#include "TypeDerivation.h"

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{
  namespace xsd
  {
// Forward declarations
    class AnyAttribute;
    class AbstractAttribute;
    class Content;
    class QName;

    /*!
      \class Restriction4ComplexContent

      \brief The restriction class can be used to define restrictions on a complexContent.
    */
    class TEXSDEXPORT Restriction4ComplexContent : public TypeDerivation, public Identifiable, public Annotated
    {
      public:

        /*!
          \brief Constructor.
          
          \param id   It specifies a unique ID for the element. It can be a NULL value.
          \param base It specifies the name of a built-in data type, simpleType element, or complexType element defined in this schema or another schema. It can not be a NULL value.

          \note The Restriction4ComplexContent object will take the ownership of the given pointers.
        */
        Restriction4ComplexContent(std::string* id, QName* base);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.

          \todo Implement!
        */
        Restriction4ComplexContent(const Restriction4ComplexContent& rhs);

        /*! \brief Destructor. */
        ~Restriction4ComplexContent();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.

          \todo Implement!
        */
        Restriction4ComplexContent& operator=(const Restriction4ComplexContent& rhs);

        /*!
          \brief It returns the name of a built-in data type, simple type or complex type that will be restricted.

          \return The name of a built-in data type, simple type or complex type that will be restricted.
        */
        QName* getBase() const;

        /*!
          \brief It returns the content of this restriction.

          \return The content of this restriction.
        */
        Content* getContent() const;

        /*!
          \brief It returns the anyAttribute defined on this restriction.

          \return The anyAttribute.
        */
        AnyAttribute* getAnyAttribute() const;

        /*!
          \brief It sets the name of a built-in data type, simple type or complex type that will be restricted.

          \param base The name of a built-in data type, simple type or complex type that will be restricted.
          
          \note The Restriction4ComplexContent object will take the ownership of the given pointer.
        */
        void setBase(QName* base);

        /*!
          \brief It sets the group content. It can be one of: group |all | choice | sequence.

          \param c The appropriate content to this restriction.
  
          \note The Restriction4ComplexContent object will take the ownership of the given pointer.
        */
        void setContent(Content* c);

        /*!
          \brief It adds an attribute to this restriction.

          \param a The attribute that will be added.
  
          \note The Restriction4ComplexContent object will take the ownership of the given pointer.
        */
        void addAttribute(AbstractAttribute* a);

        /*!
          \brief It sets the anyAttribute to this restriction.

          \param a It specifies the anyAttribute.
  
          \note The Restriction4ComplexContent object will take the ownership of the given pointer.
        */
        void setAnyAttribute(AnyAttribute* a);

        // TODO
        TypeDerivation* clone() const { return 0; }

      private:

        QName* m_base;                                       //!< It specifies the name of a built-in data type, simpleType element, or complexType element defined in this schema or another schema. (Required)
        Content* m_content;                                  //!< It can be: group |all | choice | sequence. (Optional)
        boost::ptr_vector<AbstractAttribute> m_attributeVec; //!< The list of attributes (attributes and attributeGroups). (Optional)
        AnyAttribute* m_anyAttr;                             //!< The anyAttribute element. (Optional)
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_RESTRICTION4COMPLEXCONTENT_H
