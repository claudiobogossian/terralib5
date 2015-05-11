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
  \file Restriction4SimpleType.h

  \brief The restriction element defines restrictions on a simpleType definition.
*/

#ifndef __TERRALIB_XSD_INTERNAL_RESTRICTION4SIMPLETYPE_H
#define __TERRALIB_XSD_INTERNAL_RESTRICTION4SIMPLETYPE_H

// TerraLib
#include "Enums.h"
#include "SimpleTypeConstructor.h"

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{
  namespace xsd
  {
// Forward declarations
    class Facet;
    class QName;
    class SimpleType;

    /*!
      \class Restriction4SimpleType

      \brief The restriction element defines restrictions on a simpleType definition.

      \note Parent elements: simpleType.
    */
    class TEXSDEXPORT Restriction4SimpleType : public SimpleTypeConstructor
    {
      public:

        /*!
          \brief Constructor.
          
          \param base The name of a built-in data type simpleType element defined in a schema.
          \param ann  An annotation.
          \param id   It specifies a unique ID for the element.
          
          \note The List object will take the ownership of the given pointers.
        */
        Restriction4SimpleType(QName* base = 0, Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.

          \todo need review!!!!!
        */
        Restriction4SimpleType(const Restriction4SimpleType& rhs);

        /*! \brief Destructor. */
        ~Restriction4SimpleType();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.

          \todo need review!!!!!
        */
        Restriction4SimpleType& operator=(const Restriction4SimpleType& rhs);

        /*!
          \brief It returns the name of the built-in data type simpleType element that will be restricted.

          \return The name of the built-in data type simpleType element that will be restricted.
        */
        QName* getBase() const;

        /*!
          \brief It returns the list of facets defined on this restriction.

          \return The list of facets defined on this restriction.
        */
        const boost::ptr_vector<Facet>& getFacets() const;

        /*!
          \brief It sets the name of the built-in data type simpleType element that will be restricted.

          \param base The name of the built-in data type simpleType element that will be restricted.
        */
        void setBase(QName* base);

        /*!
          \brief It sets the simpleType element that will be restricted.

          \param base The name of the built-in data type simpleType element that will be restricted.
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
          
          \note The Restriction4SimpleType object will take the ownership of the given pointer.
        */
        void addFacet(Facet* f);

        SimpleTypeConstructor* clone() const;

      private:

        QName* m_base;                       //!< It specifies the name of the built-in data type simpleType element that will be restricted. (Required)
        SimpleType* m_simpleType;             //!< Optional.
        boost::ptr_vector<Facet> m_facetVec; //!< The list of facets. (Optional)
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_RESTRICTION4SIMPLETYPE_H
