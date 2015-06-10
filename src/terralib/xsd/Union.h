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
  \file Union.h

  \brief It defines a simple type as a collection (union) of values from specified simple data types.
*/

#ifndef __TERRALIB_XSD_INTERNAL_UNION_H
#define __TERRALIB_XSD_INTERNAL_UNION_H

// TerraLib
#include "SimpleTypeConstructor.h"

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{
  namespace xsd
  {
// Forward declarations
    class QName;
    class SimpleType;

    /*!
      \class Union

      \brief It defines a simple type as a collection (union) of values from specified simple data types.

      \note Parent elements: simpleType.
    */
    class TEXSDEXPORT Union : public SimpleTypeConstructor
    {
      public:

       /*!
          \brief Constructor.
          
          \param ann          An annotation.
          \param              id It specifies a unique ID for the element.          

          \note The Union object will take the ownership of the given pointers.
        */
        Union(Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.
        */
        Union(const Union& rhs);

        /*! \brief Destructor. */
        ~Union();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.
        */
        Union& operator=(const Union& rhs);

        /*!
          \brief It returns the list of member types.

          \return The list list of member types.
        */
        const boost::ptr_vector<QName>& getMemberTypes() const;

         /*!
          \brief It returns the list of simple types defined on this union element.

          \return The the list of simple types defined on this union element.
        */
        const boost::ptr_vector<SimpleType>& getSimpleTypes() const;

        /*!
          \brief It adds a member type to this Union element.

          \param name The name of a built-in data type or simpleType element defined in this or another schema.
  
          \note The Union object will take the ownership of the given pointer.
        */
        void addMemberType(QName* name);

        /*!
          \brief It adds an internal simple type element to this Union element.

          \param a The simple type that will be added.
  
          \note The Union object will take the ownership of the given pointer.
        */
        void addSimpleType(SimpleType* a);

        SimpleTypeConstructor* clone() const;

      private:

        boost::ptr_vector<QName> m_memberTypeVec;              //!< It specifies a list of built-in data types or simpleType elements defined in a schema. (Optional)
        boost::ptr_vector<SimpleType> m_internalSimpleTypeVec; //!< Used when the union has internal simple types.
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_UNION_H
