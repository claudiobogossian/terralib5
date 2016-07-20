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
  \file Restriction4SimpleContent.h

  \brief This restriction class defines restrictions on a simpleContent.
*/

#ifndef __TERRALIB_XSD_INTERNAL_RESTRICTION4SIMPLECONTENT_H
#define __TERRALIB_XSD_INTERNAL_RESTRICTION4SIMPLECONTENT_H

// TerraLib
#include "Annotated.h"
#include "Enums.h"
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
    class Facet;
    class QName;
    class SimpleType;

    /*!
      \class Restriction4SimpleContent

      \brief This restriction class defines restrictions on a simpleContent.
     */
    class TEXSDEXPORT Restriction4SimpleContent : public TypeDerivation, public Identifiable, public Annotated
    {
      public:

        /*!
          \brief Constructor.
          
          \param id   It specifies a unique ID for the element. It can be a NULL value.
          \param base The name of a built-in data type simpleType element defined in a schema. It can not be a NULL value.

          \note The Restriction4SimpleContent object will take the ownership of the given pointers.
        */
        Restriction4SimpleContent(std::string* id, QName* base);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.

          \todo Implement!
        */
        Restriction4SimpleContent(const Restriction4SimpleContent& rhs);

        /*! \brief Destructor. */
        ~Restriction4SimpleContent();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.

          \todo Implement!
        */
        Restriction4SimpleContent& operator=(const Restriction4SimpleContent& rhs);

        /*!
          \brief It returns the name of a built-in data type, simple type or complex type that will be restricted.

          \return The name of a built-in data type, simple type or complex type that will be restricted.
         */
        QName* getBase() const;

        /*!
          \brief It returns the list of facets defined on this restriction.

          \return The list of facets defined on this restriction.
         */
        const boost::ptr_vector<Facet>& getFacets() const;

        /*!
          \brief It returns the list of attributes of this restriction.

          \return The list of attributes of this restriction.
         */
        const boost::ptr_vector<AbstractAttribute>& getAttributes() const;

        /*!
          \brief It returns the anyAttribute defined on this restriction.

          \return The anyAttribute.
         */
        AnyAttribute* getAnyAttribute() const;

        /*!
          \brief It sets the name of a built-in data type, simple type or complex type that will be restricted.

          \param base The name of a built-in data type, simple type or complex type that will be restricted.
          
          \note The Restriction4SimpleContent object will take the ownership of the given pointer.
         */
        void setBase(QName* base);

        /*!
          \brief It sets the restriction internal simple type.

          \param simpleType The restriction internal simple type.

          \note The Restriction4SimpleContent object will take the ownership of the given pointer.
        */
        void setSimpleType(SimpleType* simpleType);

        /*!
          \brief Adds a new facet to this restriction.

          \param fType The facet type.
          \param value The facet associated value.
         */
        void addFacet(FacetType fType, const std::string& value);

        /*!
          \brief Adds a new facet to this restriction.

          \param f The facet that will be added.
          
          \note The Restriction4SimpleContent object will take the ownership of the given pointer.
         */
        void addFacet(Facet* f);

        /*!
          \brief It adds an attribute to this restriction.

          \param a The attribute that will be added.
  
          \note The Restriction4SimpleContent object will take the ownership of the given pointer.
         */
        void addAttribute(AbstractAttribute* a);

        /*!
          \brief It sets the anyAttribute to this restriction.

          \param a It specifies the anyAttribute.
  
          \note The Restriction4SimpleContent object will take the ownership of the given pointer.
         */
        void setAnyAttribute(AnyAttribute* a);

        // TODO
        TypeDerivation* clone() const { return 0; }

      private:

        QName* m_base;                                       //!< It specifies the name of a built-in data type, simpleType element or complexType element defined in this schema or another schema. (Required)
        SimpleType* m_simpleType;                            //!< Optional.
        boost::ptr_vector<Facet> m_facetVec;                 //!< The list of facets. (Optional)
        boost::ptr_vector<AbstractAttribute> m_attributeVec; //!< The list of attributes (attributes and attributeGroups). (Optional)
        AnyAttribute* m_anyAttr;                             //!< The anyAttribute element. (Optional)
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_RESTRICTION4SIMPLECONTENT_H

