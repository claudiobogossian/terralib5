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
  \file ComplexType.h

  \brief It models a XML Schema Complex Type definition.
*/

#ifndef __TERRALIB_XSD_INTERNAL_COMPLEXTYPE_H
#define __TERRALIB_XSD_INTERNAL_COMPLEXTYPE_H

// TerraLib
#include "Annotated.h"
#include "Enums.h"
#include "Identifiable.h"
#include "Type.h"

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{
  namespace xsd
  {
// Forward declaration
    class AbstractAttribute;
    class AnyAttribute;
    class ComplexContent;
    class Content;
    class SimpleContent;

    /*!
      \class ComplexType

      \brief It models a XML Schema Complex Type definition.

      \note Parent elements: element, redefine, schema
    */
    class TEXSDEXPORT ComplexType : public Type, public Identifiable, public Annotated
    {
      public:

        /*!
          \brief Constructor.

          \param id It specifies a unique ID for the element. It can be a NULL value.
         */
        ComplexType(Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.

          \todo Implement!
        */
        ComplexType(const ComplexType& rhs);

        /*! \brief Destructor. */
        ~ComplexType();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.

          \todo Implement!
        */
        ComplexType& operator=(const ComplexType& rhs);

        /*!
          \brief It returns the element name or NULL if it doesn't have one.

          \return The element name or NULL if it doesn't have one.
        */
        std::string* getName() const;

        /*!
          \brief If the CompleType is "abstract" or not.

          \return True if the CompleType is "abstract" and false if not.
        */
        bool isAbstract();

        /*!
          \brief If the CompleType is "mixed" or not.

          \return True if the CompleType is "mixed" and false if not.
        */
        bool isMixed();

        /*!
          \brief It returns the block value for the CompleType.

          \return The block value. It can be: #all | list of (extension|restriction).
        */
        int getBlock();

        /*!
          \brief It returns the final value for the CompleType.

          \return The final value. It can be: #all | list of (extension|restriction).
        */
        int getFinal();

        /*!
          \brief It returns the SimpleContent of this ComplexType.

          \return The SimpleContent of this ComplexType.
        */
        SimpleContent* getSimpleContent() const;

        /*!
          \brief It returns the ComplexContent of this ComplexType.

          \return The ComplexContent of this ComplexType.
        */
        ComplexContent* getComplexContent() const;

        /*!
          \brief It returns the content element of this ComplexType.

          \return The content element of this ComplexType. It can be: group | all | choice | sequence.
        */
        Content* getContent();

        /*!
          \brief It returns the list of attributes of this ComplexType.

          \return The list of attributes of this ComplexType.
        */
        const boost::ptr_vector<AbstractAttribute>& getAttributes() const;

        /*!
          \brief It returns the anyAttribute defined on this ComplexType.

          \return The anyAttribute defined on this ComplexType.
        */
        AnyAttribute* getAnyAttribute() const;

        /*!
          \brief It sets the element name.

          \param name The element name.

          \note The ComplexType object will take the ownership of the given name.
        */
        void setName(std::string* name);

        /*!
          \brief It specifies whether the element can be used in an instance of the document.

          \param v True indicates that the element cannot appear in the instance document. Instead, another element whose substitutionGroup attribute contains the qualified name (QName) of this element must appear in this element's place.
        */
        void setAsAbstract(bool v);

        /*!
          \brief It specifies whether character data is allowed to appear between the child elements of this complexType element.

          \param v True indicates that character data is allowed to appear between the child elements of this complexType element.
        */
        void setAsMixed(bool v);

        /*!
          \brief It prevents an element with a specified type of derivation from being used in place of this element.

          \param v It can be: #all|list of (extension|restriction).
        */
        void setBlock(int v);

        /*!
          \brief It sets the default value of the final attribute on the element element.

          \param v It can be: #all|list of (extension|restriction).
        */
        void setFinal(int v);

        /*!
          \brief It sets the SimpleContent to this ComplexType.

          \param sc The SimpleContent to this ComplexType.
  
          \note Setting the SimpleContent element will turn to NULL the ComplexContent, Content, Attributes and AnyAttribute.
          \note The ComplexType object will take the ownership of the given pointer.
        */
        void setSimpleContent(SimpleContent* sc);

        /*!
          \brief It sets the ComplexContent to this ComplexType.

          \param cc The ComplexContent to this ComplexType.
  
          \note Setting the ComplexContent element will turn to NULL the SimpleContent, Content, Attributes and AnyAttribute.
          \note The ComplexType object will take the ownership of the given pointer.
        */
        void setComplexContent(ComplexContent* cc);

        /*!
          \brief It sets the ComplexType content. It can be one of: group | all | choice | sequence.

          \param c The content to this ComplexType.
  
          \note Setting the content element will turn to NULL the SimpleContent and the ComplexContent.
          \note The ComplexType object will take the ownership of the given pointer.
        */
        void setContent(Content* c);

        /*!
          \brief It adds an attribute to this ComplexType.

          \param a The attribute that will be added. It can be: attribute | attributeGroup.
  
          \note Adding a attribute will turn to NULL the SimpleContent and the ComplexContent.
          \note The ComplexType object will take the ownership of the given pointer.
        */
        void addAttribute(AbstractAttribute* a);

        /*!
          \brief It sets the anyAttribute to this ComplexType.

          \param a It specifies the anyAttribute.
  
          \note Setting the anyAttribute will turn to NULL the SimpleContent and the ComplexContent.
          \note The ComplexType object will take the ownership of the given pointer.
        */
        void setAnyAttribute(AnyAttribute* a);

        Type* clone() const;

      private:
      
        std::string* m_name;         //!< The type name if it is not an unonoymous type (declared inside a element definition).
        bool m_abstract;             //!< It specifies whether the complex type can be used in an instance document (optional). True indicates that an element cannot use this complex type directly but must use a complex type derived from this complex type. Default is false.
        bool m_mixed;                //!< It specifies whether character data is allowed to appear between the child elements of this complexType element (optional). Default is false. If a simpleContent element is a child element, the mixed attribute is not allowed.
        int m_block;                 //!< It prevents an element with a specified type of derivation from being used in place of this element. (Optional)
        int m_final;                 //!< It sets the default value of the final attribute on the element element. This attribute cannot be used if the parent element is not the schema element. (Optional)
        SimpleContent* m_sContent;   //!< SimpleContent. If m_cContent is present this must be NULL. Likewise if m_content or m_attributes or m_anyAttr is present this must be also NULL.
        ComplexContent* m_cContent;  //!< ComplexContent. If m_sContent is present this must be NULL. Likewise if m_content or m_attributes or m_anyAttr is present this must be also NULL.
        Content* m_content;          //!< It can be: group|all|choice|sequence. If m_sContent or m_cContent is present, this must be NULL.
        boost::ptr_vector<AbstractAttribute> m_attributeVec; //!< It can be: (attribute|attributeGroup)*. If m_sContent or m_cContent is present, this must be NULL.
        AnyAttribute* m_anyAttr;     //!< A pointer to a anyAttribute. If m_sContent or m_cContent is present, this must be NULL.
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_COMPLEXTYPE_H

