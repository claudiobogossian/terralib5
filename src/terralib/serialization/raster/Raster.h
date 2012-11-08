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
  \file terralib/serialization/raster/Raster.h
 
  \brief Support for serialization of Raster information.
*/

#ifndef __TERRALIB_SERIALIZATION_RASTER_INTERNAL_RASTER_H
#define __TERRALIB_SERIALIZATION_RASTER_INTERNAL_RASTER_H

// TerraLib
#include "../Config.h"

// STL
#include <iosfwd>
#include <map>
#include <string>

namespace te
{
  namespace rst { class Raster; }

  namespace xml
  {
    class Reader;
    class Writer;
  }

  namespace serialize
  {
    TESERIALIZATIONEXPORT void ReadRasterInfo(std::map<std::string, std::string>& rinfo, te::xml::Reader& reader);

    TESERIALIZATIONEXPORT void SaveRasterInfo(std::map<std::string, std::string>& rinfo, te::xml::Writer& writer);

    TESERIALIZATIONEXPORT void Save(const te::rst::Raster* raster, const std::string& fileName);

    TESERIALIZATIONEXPORT void Save(const te::rst::Raster* raster, std::ostream& ostr);

    TESERIALIZATIONEXPORT void Save(const te::rst::Raster* raster, te::xml::Writer& writer);

  } // end namespace serialize
}   // end namespace te

#endif  // __TERRALIB_SERIALIZATION_RASTER_INTERNAL_RASTER_H

