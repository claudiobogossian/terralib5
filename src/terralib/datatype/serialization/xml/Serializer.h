/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/datatype/serialization/xml/Serializer.h

  \brief Support for Property serialization.
*/

#ifndef __TERRALIB_DATATYPE_SERIALIZATION_XML_INTERNAL_SERIALIZER_H
#define __TERRALIB_DATATYPE_SERIALIZATION_XML_INTERNAL_SERIALIZER_H

// TerraLib
#include "../../../common/Singleton.h"
#include "../../Config.h"

#ifdef TERRALIB_MOD_XML_ENABLED

// STL
#include <memory>

namespace te
{
  namespace xml
  {
    class Reader;
    class Writer;
  }

  namespace dt
  {
    class Property;
  }

  namespace serialize
  {
    namespace xml
    {
      TEDATATYPEEXPORT std::auto_ptr<te::dt::Property> ReadProperty(te::xml::Reader& reader);

      TEDATATYPEEXPORT void Save(const te::dt::Property& p, te::xml::Writer& writer);

    } // end namespace xml
  }   // end namespace serialize
}     // end namespace te

#endif // TERRALIB_MOD_XML_ENABLED

#endif  // __TERRALIB_DATATYPE_SERIALIZATION_XML_INTERNAL_SERIALIZER_H

