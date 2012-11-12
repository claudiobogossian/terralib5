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
  \file terralib/serialization/se/ShadedRelief.h
 
  \brief Support for ShadedRelief serialization.
*/

#ifndef __TERRALIB_SERIALIZATION_SE_INTERNAL_SHADEDRELIEF_H
#define __TERRALIB_SERIALIZATION_SE_INTERNAL_SHADEDRELIEF_H

// TerraLib
#include "../Config.h"

namespace te
{
  namespace se { class ShadedRelief; }

  namespace xml
  {
    class Reader;
    class Writer;
  }

  namespace serialize
  {
    TESERIALIZATIONEXPORT te::se::ShadedRelief* ReadShadedRelief(te::xml::Reader& reader);

    TESERIALIZATIONEXPORT void Save(const te::se::ShadedRelief* sr, te::xml::Writer& writer);

  } // end namespace serialize
}   // end namespace te

#endif  // __TERRALIB_SERIALIZATION_SE_INTERNAL_SHADEDRELIEF_H

