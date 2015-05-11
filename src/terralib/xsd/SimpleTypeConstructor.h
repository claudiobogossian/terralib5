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
  \file SimpleTypeConstructor.h

  \brief A base class for SimpleType elements. List, Union and Restriction4SimpleType is derived from this base class.
*/

#ifndef __TERRALIB_XSD_INTERNAL_SIMPLETYPECONSTRUCTOR_H
#define __TERRALIB_XSD_INTERNAL_SIMPLETYPECONSTRUCTOR_H

// TerraLib
#include "Annotated.h"
#include "Identifiable.h"

namespace te
{
  namespace xsd
  {
    /*!
      \class SimpleTypeConstructor

      \brief A base class for SympleType elements. List, Union and Restriction4SimpleType is derived from this base class.
    */
    class TEXSDEXPORT SimpleTypeConstructor : public Identifiable, public Annotated
    {
      public:

        /*!
          \brief Constructor.

          \param ann An annotation.
          \param id  It specifies a unique ID for the element. It can be a NULL value.

          \note The SimpleTypeConstructor object will take the ownership of the annotation and id.
        */
        SimpleTypeConstructor(Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.
        */
        SimpleTypeConstructor(const SimpleTypeConstructor& rhs);

        /*! \brief Virtual destructor. */
        virtual ~SimpleTypeConstructor();

        /*!
          \brief Assignment operator not allowed.

          \param rhs Right-hand-side object.

          \return A reference to this object.
        */
        SimpleTypeConstructor& operator=(const SimpleTypeConstructor& rhs);

        /*!
          \brief It creates a clone of the object.

          \return A clone of the object. The caller will take the ownership of the returned pointer.
        */
        virtual SimpleTypeConstructor* clone() const = 0;
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_SIMPLETYPECONSTRUCTOR_H
