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
#include "../fe/Filter.h"
#include "Description.h"
#include "Graphic.h"
#include "Rule.h"
#include "Symbolizer.h"
#include "Utils.h"

// STL
#include <cassert>
#include <limits>
#include <memory>

te::se::Rule* te::serialize::ReadRule(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Rule");

  reader.next();

  std::auto_ptr<te::se::Rule> rule(new te::se::Rule);

  // Name
  if(reader.getElementLocalName() == "Name")
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    rule->setName(new std::string(reader.getElementValue()));
    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();
  }

  // Description
  if(reader.getElementLocalName() == "Description")
    rule->setDescription(ReadDescription(reader));

  // LegendGraphic
  if(reader.getElementLocalName() == "LegendGraphic")
  {
    reader.next();
    rule->setLegendGraphic(ReadGraphic(reader));

    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();
  }

  // Filter / ElseFilter
  if(reader.getElementLocalName() == "Filter")
    rule->setFilter(ReadFilter(reader));
  else if(reader.getElementLocalName() == "ElseFilter")
  {
    rule->enableElseFilter();
    reader.next();
    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();
  }

  // MinScaleDenominator
  if(reader.getElementLocalName() == "MinScaleDenominator")
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    double minScale = reader.getElementValueAsDouble();
    rule->setMinScaleDenominator(minScale);
    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();
  }

  // MaxScaleDenominator
  if(reader.getElementLocalName() == "MaxScaleDenominator")
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    double maxScale = reader.getElementValueAsDouble();
    rule->setMaxScaleDenominator(maxScale);
    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();
  }

  // Symbolizers
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
    reader.getElementLocalName().find("Symbolizer") != std::string::npos) // TODO: For while using find("Symbolizer")... Actually, I would like to search by the registered names of symbolizer.
    rule->push_back(te::serialize::Symbolizer::getInstance().read(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return rule.release();
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
    Save(rule->getFilter(), writer);
  else if(rule->hasElseFilter())
    writer.writeElement("ElseFilter", "");

  if(rule->getMinScaleDenominator() != 0.0)
    writer.writeElement("MinScaleDenominator", rule->getMinScaleDenominator());

  if(rule->getMaxScaleDenominator() != std::numeric_limits<double>::infinity())
    writer.writeElement("MaxScaleDenominator", rule->getMaxScaleDenominator());

  const std::vector<te::se::Symbolizer*> symbs = rule->getSymbolizers();
  for(std::size_t i = 0; i < symbs.size(); ++i)
    Symbolizer::getInstance().write(symbs[i], writer);

  writer.writeEndElement("Rule");
}

