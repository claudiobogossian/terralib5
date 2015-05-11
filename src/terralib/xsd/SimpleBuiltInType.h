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
  \file SimpleBuiltInType.h

  \brief A class for representing built-in XML Schema types.
*/

#ifndef __TERRALIB_XSD_INTERNAL_SIMPLEBUILTINTYPE_H
#define __TERRALIB_XSD_INTERNAL_SIMPLEBUILTINTYPE_H

// TerraLib
#include "Enums.h"
#include "Type.h"

namespace te
{
  namespace xsd
  {
    /*!
      \class SimpleBuiltInType

      \brief A class for representing built-in XML Schema types.
      
      It represents atomic types like: string, int, byte and others.
     */
    class TEXSDEXPORT SimpleBuiltInType : public Type
    {
      public:

        /*!
          \brief Constructor.

          \param t The built-in type code.
        */
        SimpleBuiltInType(BuiltInType type);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.
        */
        SimpleBuiltInType(const SimpleBuiltInType& rhs);

        /*! \brief Destructor. */
        ~SimpleBuiltInType();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.
        */
        SimpleBuiltInType& operator=(const SimpleBuiltInType& rhs);

        /*!
          \brief It returns the built-in type code.

          \return The built-in type code.
        */
        BuiltInType getType() const;

        /*!
          \brief It returns the built-in type name.

          \return The built-in type name.
        */
        const std::string& getName() const;

        Type* clone() const;

      private:

        BuiltInType m_type;                //!< The built-in type.
        static std::string m_typeNames[1]; //!< The list of names for available types.
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_SIMPLEBUILTINTYPE_H
