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
  \file SimpleType.h

  \brief It models a XML Schema SimpleType element.
*/

#ifndef __TERRALIB_XSD_INTERNAL_SIMPLETYPE_H
#define __TERRALIB_XSD_INTERNAL_SIMPLETYPE_H

// TerraLib
#include "Annotated.h"
#include "Identifiable.h"
#include "Type.h"

// STL
#include <string>

namespace te
{
  namespace xsd
  {
// Forward declarations
    class SimpleTypeConstructor;

    /*!
      \class SimpleType

      \brief It models a XML Schema SimpleType element.
      
      The simpleType element defines a simple type and specifies the constraints and information about 
      the values of attributes or text-only elements.

      \sa Annotated, Identifiable
    */
    class TEXSDEXPORT SimpleType : public Type, public Identifiable, public Annotated
    {
      public:        

        /*!
          \brief Constructor.

          \param ann An annotation.
          \param id  It specifies a unique ID for the element.

          \note The SimpleType object will take the ownership of the annotation and id.
        */
        SimpleType(Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.
        */
        SimpleType(const SimpleType& rhs);

        /*! \brief Destructor. */
        ~SimpleType();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.
        */
        SimpleType& operator=(const SimpleType& rhs);

        /*!
          \brief It returns the name of the element.

          \return The name of the element.
        */
        std::string* getName() const;

        /*!
          \brief It returns the constructor of the element.

          \return The constructor of the element - (restriction | list | union).
        */
        SimpleTypeConstructor* getConstructor() const;

        /*!
          \brief It sets the name of the element.

          \param name It specifies the element name.

          \note The SimpleType object will take the ownership of the given pointer.
        */
        void setName(std::string* name);

        /*!
          \brief It sets the constructor of the element.

          \param c It specifies the constructor of the element.

          \note The SimpleType object will take the ownership of the given pointer.
        */
        void setConstructor(SimpleTypeConstructor* c);

        Type* clone() const;

      private:

        std::string* m_name;                   //!< It specifies a name for the element. Required if the simpleType element is a child of the schema element, otherwise it is not allowed.
        SimpleTypeConstructor* m_constructor;  //!< restriction | list | union.
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_SIMPLETYPE_H
