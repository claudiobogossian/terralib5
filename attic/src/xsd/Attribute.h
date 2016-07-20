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
  \file Attribute.h

  \brief It models a XML Schema attribute.
*/

#ifndef __TERRALIB_XSD_INTERNAL_ATTRIBUTE_H
#define __TERRALIB_XSD_INTERNAL_ATTRIBUTE_H

// TerraLib
#include "AbstractAttribute.h"
#include "Annotated.h"
#include "Enums.h"
#include "Identifiable.h"

// STL
#include <map>

namespace te
{
  namespace xsd
  {
// Forward declarations
    class QName;
    class SimpleType;

    /*!
      \class Attribute

      \brief It models a XML Schema attribute.

      Some notes:
      <ul>
      <li>Attributes can not contain other elements or other attributes.</li>
      <li>The default and fixed attributes can not both be present.</li>
      <li>Name and ref attributes cannot both be present. If ref is present the named type, form, and SimpleType cannot be present.</li>
      <li>If a named type is used, ref and SimpleType cannot be present.</li>
      </ul>

      \note Parent elements: attributeGroup, schema, complexType, restriction (both simpleContent and complexContent), extension (both simpleContent and complexContent).
    */
    class TEXSDEXPORT Attribute : public AbstractAttribute, public Identifiable, public Annotated
    {
      public:

        /*!
          \brief Constructor.
          
          \param ann An annotation.
          \param id  It specifies a unique ID for the element.

          \note The Attribute object will take the ownership of the annotation and id.
        */
        Attribute(Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.
        */
        Attribute(const Attribute& rhs);

        /*! \brief Destructor. */
        ~Attribute();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.
        */
        Attribute& operator=(const Attribute& rhs);

        /*!
          \brief It returns the default value for the attribute.

          \return The default value.
        */
        std::string* getDefault() const;

        /*!
          \brief It returns the fixed value for the attribute.

          \return The fixed value.
        */
        std::string* getFixed() const;

        /*!
          \brief It returns the form property value of attribute.

          \return Form property value of attribute: Qualified or Unqualified.
        */
        Form getForm() const;

        /*!
          \brief It returns the name of the attribute.

          \return The name of the object.
        */
        std::string* getName() const;

        /*!
          \brief It returns the reference to a named attribute element.

          \return The reference to a named attribute element.
        */
        QName* getRef() const;

        /*!
          \brief It returns a built-in data type or simple type for the attribute element.

          \return A built-in data type or simple type for the attribute element.
        */
        QName* getType() const;

        /*!
          \brief It returns a AttributeUse object that specifies how how the attribute is used.

          \return An AttributeUse object that specifies how the attribute is used.
        */
        AttributeUse getUse() const;

        /*!
          \brief It returns the simpleType element of the attribute.

          \return A simpleType element.
        */
        SimpleType* getInnerType() const;

        /*!
          \brief It returns a map with other properties of the attribute element.

          \return Other properties of the attribute element as a key-value map.
        */
        //std::map<std::string, std::string>* getOtherAttributes() const;

        /*!
          \brief It sets a default value for the attribute.

          \param def A default value.
  
          \note Setting the default property will turn to NULL the fixed property.
          \note The Attribute object will take the ownership of the given pointer.
        */
        void setDefault(std::string* def);

        /*!
          \brief It sets a fixed value for the attribute.

          \param fix A fixed value.
  
          \note Setting the fixed property will turn to NULL the default property.
          \note The Attribute object will take the ownership of the given pointer.
        */
        void setFixed(std::string* fix);

        /*!
          \brief It sets the form property of the attribute.

          \param f The value for form property of the attribute: Qualified or Unqualified.
        */
        void setForm(Form f);

        /*!
          \brief It sets a name for the attribute.

          \param name A name for this attribute.
  
          \note Setting a name will turn to NULL the reference property.
          \note The Attribute object will take the ownership of the given pointer.
        */
        void setName(std::string* name);

        /*!
          \brief It sets a reference to a named attribute.

          \param ref A reference to a named attribute as a QName object.
  
          \note Setting a reference will turn to NULL the name and type properties and the SimpleType element.
          \note The Attribute object will take the ownership of the given pointer.
        */
        void setRef(QName* ref);

        /*!
          \brief It sets a built-in data type or simple type for the attribute.

          \param type A built-in data type or simple type as a QName object.
  
          \note Setting a reference will turn to NULL the reference property and the inner SimpleType element.
          \note The Attribute object will take the ownership of the given pointer.
        */
        void setType(QName* type);

        /*!
          \brief It sets how the attribute is used.

          \param use A AttributeUse object that specifies how the attribute is used.
        */
        void setUse(AttributeUse use);

        /*!
          \brief It sets a inner simpleType element for the attribute.

          \param iType A SimpleType object that represents a inner simpleType element for the attribute.
  
          \note Setting a simpleType element for the attribute will turn to NULL the reference and type properties.
          \note The Attribute object will take the ownership of the given pointer.
        */
        void setInnerType(SimpleType* iType);

        /*!
          \brief Add a property with non-schema namespace to the attribute.

          \param key The name of the property.
          \param value The value of the property.
        */
        //void addOtherAttribute(std::string key, std::string value);

        AbstractAttribute* clone() const;

      private:

        std::string* m_default;                                 //!< It specifies a default value for the attribute (Optional).
        std::string* m_fixed;                                   //!< It specifies a fixed value for the attribute (Optional).
        Form m_form;                                            //!< It specifies if the attribute must be qualified or not. Default = unqualified. (Optional)
        std::string* m_name;                                    //!< It specifies a name for the attribute. It will be NULL if m_ref is present. (Optional)
        QName* m_ref;                                           //!< It specifies a reference to a named attribute. NULL if m_name is present. (Optional)
        QName* m_type;                                          //!< It specifies a built-in data type or simple type for the attribute. It is NULL if m_ref is present or m_innerType. (Optional)
        AttributeUse m_use;                                     //!< It specifies how the attribute is used. (Optional)
        SimpleType* m_innerType;                                //!< It specifies the attribute type as a simple type. It will be NULL if m_ref is present or if m_type is present. (Optional)
        //std::map<std::string, std::string>* m_otherAttributes;  //!< It specifies a list of other attributes with non-schema namespace (Optional).
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_ATTRIBUTE_H

