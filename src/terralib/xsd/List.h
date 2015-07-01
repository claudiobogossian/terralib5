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
  \file List.h

  \brief It is used to define a simple type element as a list of values.
*/

#ifndef __TERRALIB_XSD_INTERNAL_LIST_H
#define __TERRALIB_XSD_INTERNAL_LIST_H

// TerraLib
#include "SimpleTypeConstructor.h"

namespace te
{
  namespace xsd
  {
// Forward declarations
    class QName;
    class SimpleType;

    /*!
      \class List

      \brief It is used to define a simple type element as a list of values.
    */
    class TEXSDEXPORT List : public SimpleTypeConstructor
    {
      public:

        /*!
          \brief Constructor.
          
          \param ann An annotation.
          \param id  It specifies a unique ID for the element.

          \note The List object will take the ownership of the annotation and id.
        */
        List(Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.
        */
        List(const List& rhs);

        /*! \brief Destructor. */
        ~List();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.
        */
        List& operator=(const List& rhs);

        /*!
          \brief It returns name of a built-in data type or simpleType element defined in this or another schema.

          \return The name of a built-in data type or simpleType element defined in this or another schema.
        */
        QName* getItemType() const;

         /*!
          \brief It returns list internal simple type.

          \return The list internal simple type.
        */
        SimpleType* getSimpleType() const;

        /*!
          \brief It sets the name of a built-in data type or simpleType element defined in this or another schema.

          \param name The name of a built-in data type or simpleType element defined in this or another schema.
  
          \note Setting a reference will turn to NULL the internal simpleType.
          \note The List object will take the ownership of the given pointer.
        */
        void setItemType(QName* name);

        /*!
          \brief It sets the list internal simple type.

          \param a The list internal simple type.
  
          \note Setting a reference will turn to NULL the itemType.
          \note The List object will take the ownership of the given pointer.
        */
        void setSimpleType(SimpleType* a);

        SimpleTypeConstructor* clone() const;

      private:

        QName* m_itemType;                 //!< It specifies the name of a built-in data type or simpleType element defined in this or another schema. This attribute is not allowed if the content contains a simpleType element, otherwise it is required
        SimpleType* m_internalSimpleType;  //!< Used when the list has an internal simple type. (Optional if m_itemType is present)
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_LIST_H
