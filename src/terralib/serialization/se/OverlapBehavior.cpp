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
  \file terralib/serialization/se/OverlapBehavior.cpp
 
  \brief Support for OverlapBehavior serialization.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "OverlapBehavior.h"

// STL
#include <cassert>

te::se::RasterSymbolizer::OverlapBehavior te::serialize::ReadOverlapBehavior(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "OverlapBehavior");

  std::string type = reader.getElementValue();

  if(type == "AVERAGE")
    return te::se::RasterSymbolizer::AVERAGE;

  if(type == "EARLIEST_ON_TOP")
    return te::se::RasterSymbolizer::EARLIEST_ON_TOP;

  if(type == "LATEST_ON_TOP")
    return te::se::RasterSymbolizer::LATEST_ON_TOP;

  if(type == "RANDOM")
    return te::se::RasterSymbolizer::RANDOM;

  return te::se::RasterSymbolizer::NO_BEHAVIOR;
}

void te::serialize::Save(const te::se::RasterSymbolizer::OverlapBehavior& type, te::xml::Writer& writer)
{
  switch(type)
  {
    case te::se::RasterSymbolizer::NO_BEHAVIOR:
    break;

    case te::se::RasterSymbolizer::AVERAGE:
      writer.writeElement("OverlapBehavior", "AVERAGE");
    break;

    case te::se::RasterSymbolizer::EARLIEST_ON_TOP:
      writer.writeElement("OverlapBehavior", "EARLIEST_ON_TOP");
    break;

    case te::se::RasterSymbolizer::LATEST_ON_TOP:
      writer.writeElement("OverlapBehavior", "LATEST_ON_TOP");
    break;

    case te::se::RasterSymbolizer::RANDOM:
      writer.writeElement("OverlapBehavior", "RANDOM");
    break;
  }
}
