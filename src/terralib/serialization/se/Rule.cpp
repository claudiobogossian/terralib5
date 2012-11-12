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
  \file terralib/serialization/se/Rule.cpp
 
  \brief Support for Rule serialization.
*/

// TerraLib
#include "../../se/Rule.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "Description.h"
#include "Graphic.h"
#include "Rule.h"
#include "Symbolizer.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

te::se::Rule* te::serialize::ReadRule(te::xml::Reader& reader)
{
  return 0;
}

void te::serialize::Save(const te::se::Rule* rule, te::xml::Writer& writer)
{
  if(rule == 0)
    return;

  writer.writeStartElement("Rule");

  WriteStringPtrHelper("Name", rule->getName(), writer);
  Save(rule->getDescription(), writer);

  const te::se::Graphic* legendGraphic = rule->getLegendGraphic();
  if(legendGraphic)
  {
    writer.writeStartElement("LegendGraphic");
    Save(rule->getLegendGraphic(), writer);
    writer.writeEndElement("LegendGraphic");
  }

  if(rule->getFilter())
    /* Save(rule->getFilter(), writer); TODO: fe serialize! */ int x = 0;
  else if(rule->hasElseFilter())
    writer.writeElement("ElseFilter", "");

  writer.writeElement("MinScaleDenominator", rule->getMinScaleDenominator());
  writer.writeElement("MaxScaleDenominator", rule->getMaxScaleDenominator());

  const std::vector<te::se::Symbolizer*> symbs = rule->getSymbolizers();
  for(std::size_t i = 0; i < symbs.size(); ++i)
    Symbolizer::getInstance().write(symbs[i], writer);

  writer.writeEndElement("Rule");
}

