/*  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file src/terralib/maptools/serialization/xml/Utils.h
   
  \brief Auxiliary functions to read layer information from a XML document.
*/

#ifndef __TERRALIB_MAPTOOLS_SERIALIZATION_XML_INTERNAL_UTILS_H
#define __TERRALIB_MAPTOOLS_SERIALIZATION_XML_INTERNAL_UTILS_H

// TerraLib
#include "../../Enums.h"
#include "../../Config.h"

// STL
#include <string>
#include <memory>

namespace te
{
  namespace xml
  {
    class AbstractWriter;
    class Reader;
  }

  namespace map
  {
    class AbstractLayer;
    class Chart;
    class Grouping;
    class GroupingItem;

    namespace serialize
    {
      TEMAPEXPORT te::map::Visibility GetVisibility(const std::string& visible);

      TEMAPEXPORT std::string GetVisibility(const te::map::Visibility& visible);

      TEMAPEXPORT te::map::GroupingType GetGroupingType(const std::string& type);

      TEMAPEXPORT std::string GetGroupingType(const te::map::GroupingType& type);

      TEMAPEXPORT std::string ReadLayerTitle(te::xml::Reader& reader);

      TEMAPEXPORT std::string ReadLayerVisibility(te::xml::Reader& reader);

      TEMAPEXPORT te::map::Grouping* ReadLayerGrouping(te::xml::Reader& reader);

      TEMAPEXPORT te::map::GroupingItem* ReadGroupingItem(te::xml::Reader& reader);

      TEMAPEXPORT std::auto_ptr<te::map::Chart> ReadLayerChart(te::xml::Reader& reader);

      TEMAPEXPORT void WriteLayerChart(te::map::Chart* chart, te::xml::AbstractWriter& writer);

      TEMAPEXPORT void WriteLayerGrouping(te::map::Grouping* g, te::xml::AbstractWriter& writer);

      TEMAPEXPORT void WriteAbstractLayer(const te::map::AbstractLayer* layer, te::xml::AbstractWriter& writer);

    }  //end namespace serialize
  }    // end namespace map
}      // end namespace te

#endif  // __TERRALIB_MAPTOOLS_SERIALIZATION_XML_INTERNAL_UTILS_H
