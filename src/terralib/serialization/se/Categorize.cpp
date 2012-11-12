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
  \file terralib/serialization/se/Categorize.cpp
 
  \brief Support for Categorize serialization.
*/

// TerraLib
#include "../../se/Categorize.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "Categorize.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <vector>

te::se::Categorize* te::serialize::ReadCategorize(te::xml::Reader& reader)
{
  return 0;
}

void te::serialize::Save(const te::se::Categorize* c, te::xml::Writer& writer)
{
  if(c == 0)
    return;

  writer.writeStartElement("Categorize");

  writer.writeAttribute("fallbackValue", c->getFallbackValue());

  te::se::Categorize::ThresholdsBelongToType type = c->getThresholdsBelongTo();
  switch(type)
  {
    case te::se::Categorize::NONE:
    break;

    case te::se::Categorize::PRECEDING:
      writer.writeAttribute("threshholdsBelongTo", "preceding");
    break;

    case te::se::Categorize::SUCCEEDING:
      writer.writeAttribute("threshholdsBelongTo", "succeeding");
    break;  
  }

  WriteParameterValuePtrHelper("LookupValue", c->getLookupValue(), writer);
  WriteParameterValuePtrHelper("Value", c->getValue(), writer);

  std::vector<te::se::ParameterValue*> thresholds = c->getThresholds();
  std::vector<te::se::ParameterValue*> values = c->getThresholdValues();
  assert(thresholds.size() == values.size());

  for(std::size_t i = 0; i < thresholds.size(); ++i)
  {
    WriteParameterValuePtrHelper("Thresold", thresholds[i], writer);
    WriteParameterValuePtrHelper("Value", values[i], writer);
  }

  writer.writeEndElement("Categorize");
}
