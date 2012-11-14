/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file SerializationExample.h

  \brief Several examples declarations.
*/

#ifndef __TERRALIB_EXAMPLES_SERIALIZATION_INTERNAL_SERIALIZATIONEXAMPLES_H
#define __TERRALIB_EXAMPLES_SERIALIZATION_INTERNAL_SERIALIZATIONEXAMPLES_H

// STL
#include <string>

/*! \brief It creates an OGC Filter Enconding and encode it to XML format. */
void EncodeFilter(const std::string& path = "");

/*! \brief It creates an OGC Symbology Enconding Style and encode it to XML format. */
void EncodeStyle(const std::string& path = "");

#endif  // __TERRALIB_EXAMPLES_SERIALIZATION_INTERNAL_SERIALIZATIONEXAMPLES_H
