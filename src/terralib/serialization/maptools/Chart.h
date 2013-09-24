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
  \file Chart.h
   
  \brief Auxiliary classes and functions to read Chart information from a XML document.
*/

#ifndef __TERRALIB_SERIALIZATION_MAPTOOLS_INTERNAL_CHART_H
#define __TERRALIB_SERIALIZATION_MAPTOOLS_INTERNAL_CHART_H

// TerraLib
#include "../../common/Singleton.h"
#include "../Config.h"

// STL
#include <map>
#include <string>

// Boost
#include <boost/function.hpp>

namespace te
{
  namespace color { class RGBAColor; }

  namespace map { class Chart; }

  namespace xml
  {
    class Reader;
    class Writer;
  }

  namespace serialize
  {
    std::auto_ptr<te::map::Chart> ReadChart(te::xml::Reader& reader);

    te::color::RGBAColor ReadRGBAColor(te::xml::Reader& reader);

    void Save(const te::map::Chart* chart, te::xml::Writer& writer);

    void Save(const te::color::RGBAColor color, te::xml::Writer& writer);

  } // end namespace serialize
}   // end namespace te

#endif  // __TERRALIB_SERIALIZATION_MAPTOOLS_INTERNAL_CHART_H

