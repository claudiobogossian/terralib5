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
  \file AttributeGroup.h

  \brief It models a XML Schema attribute group.
*/

#ifndef __TERRALIB_XSD_INTERNAL_ATTRIBUTEGROUP_H
#define __TERRALIB_XSD_INTERNAL_ATTRIBUTEGROUP_H

// TerraLib
#include "AbstractAttribute.h"
#include "Annotated.h"
#include "Identifiable.h"

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{
  namespace xsd
  {
// Forward declarations
    class AnyAttribute;
    class QName;

    /*!
      \class Attribute

      \brief It models a XML Schema attribute group.

      An attribute group can be used to group a set of attribute declarations
      to be incorporated as a group into a complex type definition.

      \note Parent elements: attributeGroup, complexType, schema, restriction (both simpleContent and complexContent), extension (both simpleContent and complexContent)
    */
    class TEXSDEXPORT AttributeGroup : public AbstractAttribute, public Identifiable, public Annotated
    {
      public:

        /*!
          \brief Constructor.
          
          \param ann An annotation.
          \param id  It specifies a unique ID for the element.

          \note The AttributeGroup object will take the ownership of the annotation and id.
        */
        AttributeGroup(Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.
        */
        AttributeGroup(const AttributeGroup& rhs);

        /*! \brief Destructor. */
        ~AttributeGroup();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.
        */
        AttributeGroup& operator=(const AttributeGroup& rhs);

        /*!
          \brief It returns the name of the attribute group.

          \return The name of the attribute group.
        */
        std::string* getName() const;

        /*!
          \brief It returns the reference to a named attribute element.

          \return The reference to a named attribute element.
        */
        QName* getRef() const;

        /*!
          \brief It returns the anyAttribute defined on this attribute group element.

          \return The anyAttribute.
        */
        AnyAttribute* getAnyAttribute() const;

        /*!
          \brief It sets a name for the attribute group.

          \param name The attribute group name.
  
          \note Setting a name will turn to NULL the reference property.
          \note The AttributeGroup object will take the ownership of the given pointer.
        */
        void setName(std::string* name);

        /*!
          \brief It sets a reference to a named attribute.

          \param ref A reference to a named attribute.
  
          \note Setting a reference property will turn to NULL the name.
          \note The AttributeGroup object will take the ownership of the given pointer.
        */
        void setRef(QName* ref);

        /*!
          \brief It adds an attrbitue to this attribute group element.

          \param a The attribute that will be added.
  
          \note The AttributeGroup object will take the ownership of the given pointer.
        */
        void addAttribute(AbstractAttribute* a);

         /*!
          \brief It sets the anyAttribute to this attribute group element.

          \param a It specifies the anyAttribute.
  
          \note The AttributeGroup object will take the ownership of the given pointer.
        */
        void setAnyAttribute(AnyAttribute* a);

        AbstractAttribute* clone() const;

      private:

        std::string* m_name;                                 //!< It specifies a name for the attribute group. (Optional)
        QName* m_ref;                                        //!< It specifies a reference to a named attribute group. Name and ref attributes cannot both be present. (Optional)
        boost::ptr_vector<AbstractAttribute> m_attributeVec; //!< The list of attributes, references or nested attribute groups. (Optional)        
        AnyAttribute* m_anyAttr;                             //!< It enables the author to extend the XML document with attributes not specified by the schema. (Optional)
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_ATTRIBUTEGROUP_H
