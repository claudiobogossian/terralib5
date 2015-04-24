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
  \file terralib/raster/serialization/xml/Raster.h
 
  \brief Support for serialization of Raster information.
*/

#ifndef __TERRALIB_RASTER_SERIALIZATION_XML_INTERNAL_SERIALIZER_H
#define __TERRALIB_RASTER_SERIALIZATION_XML_INTERNAL_SERIALIZER_H

// TerraLib
#include "../../Config.h"

// STL
#include <iosfwd>
#include <map>
#include <string>

namespace te
{
  namespace rst
  {
    class BandProperty;
    class Grid;
    class Raster;
  }

  namespace xml
  {
    class AbstractWriter;
    class Reader;
  }

  namespace serialize
  {
    namespace xml
    {
      TERASTEREXPORT void ReadRasterInfo(std::map<std::string, std::string>& rinfo, te::xml::Reader& reader);

      TERASTEREXPORT void SaveRasterInfo(std::map<std::string, std::string>& rinfo, te::xml::AbstractWriter& writer);

      TERASTEREXPORT void Save(const te::rst::Raster* raster, const std::string& fileName);

      TERASTEREXPORT void Save(const te::rst::Raster* raster, std::ostream& ostr);

      TERASTEREXPORT void Save(const te::rst::Raster* raster, te::xml::AbstractWriter& writer);

      TERASTEREXPORT double* ReadGeoTransform(te::xml::Reader& reader);

      TERASTEREXPORT te::rst::Grid* ReadGrid(te::xml::Reader& reader);

      TERASTEREXPORT void Save(const te::rst::Grid* grid, te::xml::AbstractWriter& writer);

      TERASTEREXPORT std::vector<te::rst::BandProperty*> ReadBandPropertyVector(te::xml::Reader& reader);

      TERASTEREXPORT te::rst::BandProperty* ReadBandProperty(te::xml::Reader& reader);

      TERASTEREXPORT void Save(const te::rst::BandProperty* bp, te::xml::AbstractWriter& writer);

    } // end namespace xml
  }   // end namespace serialize
}     // end namespace te

#endif  // __TERRALIB_RASTER_SERIALIZATION_XML_INTERNAL_SERIALIZER_H

