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
  \file Extension.h

  \brief This class models an extension element that can be used to extend an existing simpleType or complexType element.
*/

#ifndef __TERRALIB_XSD_INTERNAL_EXTENSION_H
#define __TERRALIB_XSD_INTERNAL_EXTENSION_H

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
    class AbstractAttribute;
    class AnyAttribute;
    class Content;
    class QName;

    /*!
      \class Extension

      \brief This class models an extension element that can be used to extend an existing simpleType or complexType element.
    */
    class TEXSDEXPORT Extension : public TypeDerivation, public Identifiable, public Annotated
    {
      public:

        /*!
          \brief Constructor.
          
          \param id   It specifies a unique ID for the element. It can be a NULL value.
          \param base It specifies the name of a built-in data type, a SimpleType element or a ComplexType element. It can not be a NULL value.
        */
        Extension(QName* base = 0, Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.

          \todo Implement!
        */
        Extension(const Extension& rhs);

        /*! \brief Destructor. */
        ~Extension();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.

          \todo Implement!
        */
        Extension& operator=(const Extension& rhs);

        /*!
          \brief It returns the name of a built-in data type, simple type or complex type that will be extended.

          \return The name of a built-in data type, simple type or complex type that will be extended.
        */
        QName* getBase() const;

        /*!
          \brief It returns the content of Extension element.

          \return The content of this Extension element.
        */
        Content* getContent() const;

        /*!
          \brief It returns the list of attributes of this Extension element.

          \return The list of attributes.
        */
        const boost::ptr_vector<AbstractAttribute>& getAttributes() const;

        /*!
          \brief It returns the anyAttribute defined on this Extension element.

          \return The anyAttribute.
        */
        AnyAttribute* getAnyAttribute() const;

        /*!
          \brief It sets the name of a built-in data type, simple type or complex type that will be extended.

          \param base The name of a built-in data type, simple type or complex type that will be extended.
          
          \note The Extension object will take the ownership of the given pointer.
        */
        void setBase(QName* base);

        /*!
          \brief It sets the content of this Extension element. It can be one of: group | all | choice | sequence.

          \param c The appropriate content to Extension element.
  
          \note The Extension object will take the ownership of the given pointer.
         */
        void setContent(Content* c);

        /*!
          \brief It adds an attribute to this Extension element.

          \param a The attribute that will be added.
  
          \note The Extension object will take the ownership of the given pointer.
        */
        void addAttribute(AbstractAttribute* a);

        /*!
          \brief It sets the anyAttribute to Extension element.

          \param a It specifies the anyAttribute.
  
          \note The Extension object will take the ownership of the given pointer.
        */
        void setAnyAttribute(AnyAttribute* a);

        // TODO
        TypeDerivation* clone() const { return 0; }

      private:

        QName* m_base;                                       //!< It specifies the name of a built-in data type, a SimpleType element or a ComplexType element. (Required)
        Content* m_content;                                  //!< It can be: group | all | choice | sequence. (Optional)
        boost::ptr_vector<AbstractAttribute> m_attributeVec; //!< The list of attributes (attributes and attributeGroups). (Optional)
        AnyAttribute* m_anyAttr;                             //!< The anyAttribute element. (Optional)
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_EXTENSION_H
