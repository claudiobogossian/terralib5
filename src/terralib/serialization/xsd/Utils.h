/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/serialization/xsd/Utils.h
 
  \brief Utility methods for Schema serialization.
*/

#ifndef __TERRALIB_SERIALIZATION_XSD_INTERNAL_UTILS_H
#define __TERRALIB_SERIALIZATION_XSD_INTERNAL_UTILS_H

// STL
#include <string>

namespace te
{
  namespace xml
  {
    class Reader;
  }

  namespace xsd
  {
    class Annotated;
    class Identifiable;
    class Occurs;
    class QName;
  }

  namespace serialize
  {
    void ReadIdentifiable(te::xsd::Identifiable* identifiable, te::xml::Reader& reader);

    void ReadAnnotated(te::xsd::Annotated* annotated, te::xml::Reader& reader);

    void ReadOccurs(te::xsd::Occurs* occurs, te::xml::Reader& reader);

    te::xsd::QName* CreateQName(const std::string& name);

  } // end namespace serialize
}   // end namespace te

#endif  // __TERRALIB_SERIALIZATION_XSD_INTERNAL_UTILS_H
