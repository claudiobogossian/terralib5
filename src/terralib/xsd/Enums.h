/*  Copyright (C) 2010-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file Enums.h

  \brief Several enumerations related to XSD.
*/

#ifndef __TERRALIB_XSD_INTERNAL_ENUMS_H
#define __TERRALIB_XSD_INTERNAL_ENUMS_H

// TerraLib
#include "../common/Static.h"

namespace te
{
  namespace xsd
  {
    //class BlockDefault : public te::common::Static
    //{
    //  enum
    //  {
    //    EXTENSION    = 1,
    //    RESTRICTION  = 2,
    //    SUBSTITUTION = 4,
    //    ALL          = 7
    //  };

    //};

    /*!
      \enum DerivationType

      \brief It specifies the list of possible derivation types.
    */
    enum DerivationType
    {
      UNKNOWN      = 0,
      EXTENSION    = 1,
      RESTRICTION  = 2,
      SUBSTITUTION = 4,
      LIST         = 8,
      UNION        = 16,
      ALL          = 32
    };

    /*!
      \enum AttributeUse

      \brief It specifies how the attribute is used.
    */
    enum AttributeUse
    {
      REQUIRED,     //!< It indicates that the attribute use is required.
      OPTIONAL,     //!< It indicates that the attribute use is optional. This is the default.
      PROHIBITED    //!< It indicates that the attribute cannot be used.
    };

    /*!
      \enum BuiltInType

      \brief It enumerates the available built-in types.
    */
    enum BuiltInType
    {
      TODO //!< TODO
    };

    /*!
      \enum FacetType

      \brief It enumerates the available facet types.
    */
    enum FacetType
    {
      Enumeration,    //!< Defines a list of acceptable values.
      FractionDigits, //!< Specifies the maximum number of decimal places allowed.
      Length,         //!< Specifies the exact number of characters or list items allowed.
      MaxExclusive,   //!< Specifies the upper bounds for numeric values. (<)
      MaxInclusive,   //!< Specifies the upper bounds for numeric values. (<=)
      MaxLength,      //!< Specifies the maximum number of characters or list items allowed.
      MinExclusive,   //!< Specifies the lower bounds for numeric values. (>)
      MinInclusive,   //!< Specifies the lower bounds for numeric values. (>=)
      MinLength,      //!< Specifies the minimum number of characters or list items allowed.
      Pattern,        //!< Defines the exact sequence of characters that are acceptable
      TotalDigits,    //!< Specifies the maximum number of digits allowed.
      WhiteSpace      //!< Specifies how white space (line feeds, tabs, spaces and carriage returns) is handled.
    };

    /*!
      \enum Form

      \brief It specifies the form for the attribute.
    */
    enum Form
    {
      Qualified,    //!< It indicates that the attribute attribute must be qualified with the namespace prefix and the no-colon-name (NCName) of the attribute.
      Unqualified   //!< It indicates that the attribute attribute is not required to be qualified with the namespace prefix and is matched against the (NCName) of the attribute.
    };

    /*!
      \enum ProcessContents

      \brief It enumerates the possible ways that the XML processor should handle validation against the elements specified by an any element.
    */
    enum ProcessContents
    {
      STRICT, //!< The XML processor must obtain the schema for the required namespaces and validate the elements (this is default).
      LAX,    //!< Same as strict but; if the schema cannot be obtained, no errors will occur.
      SKIP   //!< The XML processor does not attempt to validate any elements from the specified namespaces.
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_ENUMS_H

