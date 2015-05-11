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
  \file Type.h

  \brief An abstract class for modeling the definition of a new XML Schema Type.
*/

#ifndef __TERRALIB_XSD_INTERNAL_TYPE_H
#define __TERRALIB_XSD_INTERNAL_TYPE_H

// TerraLib
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace xsd
  {
    /*!
      \class Type

      \brief An abstract class for modeling the definition of a new XML Schema Type.

      A type can be a:
      <ul>
      <li>simple built-in type: like string, integer, list-of-simple-type or union-of-simple-types</li>
      <li>simple type: user defined</li>
      <li>complex type: user defined</li>
      </ul>

      We also must remember from W3C's XML Schema Specification that a type can be unnamed, 
      i. e., it can be declared inside an element definition or another type declaration without
      an explicit name.

      Another important concept in this framework is the use of "pointer of pointers" when referencing a type. 
      This is a way of allowing the user of the API do what he wants. For example, when reading a XML schema 
      you can create a NULL pointer and insert it into the schema, so the element declaration can reference 
      this pointer before the type gets read.

      Note that all types and elements belongs to the schema object. So no other object will delete a type definition 
      or a element declaration in its destructor. The schema object will free the memory.

      \sa ComplexType, SimpleType, SimpleBuiltInType
     */
    class TEXSDEXPORT Type
    {
      public:

        Type() {}

        virtual ~Type() {}

        virtual Type* clone() const = 0;
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_TYPE_H
