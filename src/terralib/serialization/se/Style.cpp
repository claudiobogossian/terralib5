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
  \file terralib/serialization/se/Style.cpp
   
  \brief Auxiliary classes and functions to serialize style informations from a XML document.
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../se/CoverageStyle.h"
#include "../../se/FeatureTypeStyle.h"
#include "../Exception.h"
#include "Description.h"
#include "Rule.h"
#include "Style.h"
#include "Utils.h"

// STL
#include <cassert>

// Boost
#include <boost/format.hpp>

te::se::Style* FeatureTypeStyleReader(te::xml::Reader& reader);
te::se::Style* CoverageStyleReader(te::xml::Reader& reader);

void FeatureTypeStyleWriter(const te::se::Style* style, te::xml::Writer& writer);
void CoverageStyleWriter(const te::se::Style* layer, te::xml::Writer& writer);

void te::serialize::Style::reg(const std::string& styleType, const StyleFnctSerializeType& fncts)
{
  m_fncts[styleType] = fncts;
}

te::se::Style* te::serialize::Style::read(te::xml::Reader& reader) const
{
  std::string styleType = reader.getElementLocalName();

  StyleFnctIdxType::const_iterator it = m_fncts.find(styleType);

  if(it == m_fncts.end())
    throw Exception((boost::format(TR_SERIALIZATION("Could not find a reader for the following style type: %1%.")) % styleType).str());

  assert(it->second.second);

  return it->second.first(reader);
}

void te::serialize::Style::write(const te::se::Style* style, te::xml::Writer& writer) const
{
  assert(style);

  StyleFnctIdxType::const_iterator it = m_fncts.find(style->getType());

  if(it == m_fncts.end())
    throw Exception((boost::format(TR_SERIALIZATION("Could not find a writer for the following style type: %1%.")) % style->getType()).str());

  assert(it->second.second);

  return it->second.second(style, writer);
}

te::serialize::Style::~Style()
{
}

te::serialize::Style::Style()
{
  m_fncts["FeatureTypeStyle"] = std::make_pair(StyleReadFnctType(&FeatureTypeStyleReader), StyleWriteFnctType(&FeatureTypeStyleWriter));
  m_fncts["CoverageStyle"] = std::make_pair(StyleReadFnctType(&CoverageStyleReader), StyleWriteFnctType(&CoverageStyleWriter));
}

te::se::Style* FeatureTypeStyleReader(te::xml::Reader& reader)
{
  return 0;
}

te::se::Style* CoverageStyleReader(te::xml::Reader& reader)
{
  return 0;
}

void FeatureTypeStyleWriter(const te::se::Style* style, te::xml::Writer& writer)
{
  const te::se::FeatureTypeStyle* fts = dynamic_cast<const te::se::FeatureTypeStyle*>(style);

  if(fts == 0)
    return;

  writer.writeStartElement("FeatureTypeStyle");
  // Version
  writer.writeAttribute("version", fts->getVersion());
  // Name
  te::serialize::WriteStringPtrHelper("Name", fts->getName(), writer);
  // Description
  te::serialize::Save(fts->getDescription(), writer);
  // FeatureTypeName
  te::serialize::WriteStringPtrHelper("FeatureTypeName", fts->getFeatureTypeName(), writer);
  // Semantic Type Identifiers
  const std::vector<std::string>* semantics = fts->getSemanticTypeIdentifiers();
  if(semantics != 0)
  {
    for(std::size_t i = 0; i < semantics->size(); ++i)
      writer.writeElement("SemanticTypeIdentifier", semantics->at(i));
  }
  // Rules
  for(std::size_t i = 0; i < fts->getNRules(); ++i)
    te::serialize::Save(fts->getRule(i), writer);

  // Online Resources (...)

  writer.writeEndElement("FeatureTypeStyle");
}

void CoverageStyleWriter(const te::se::Style* style, te::xml::Writer& writer)
{
  const te::se::CoverageStyle* cs = dynamic_cast<const te::se::CoverageStyle*>(style);

  if(cs == 0)
    return;

  writer.writeStartElement("CoverageStyle");

  // Version
  writer.writeAttribute("version", cs->getVersion());
  // Name
  te::serialize::WriteStringPtrHelper("Name", cs->getName(), writer);
  // Description
  te::serialize::Save(cs->getDescription(), writer);
  // CoverageName
  te::serialize::WriteStringPtrHelper("CoverageName", cs->getCoverageName(), writer);
  // Semantic Type Identifiers
  const std::vector<std::string>* semantics = cs->getSemanticTypeIdentifiers();
  if(semantics != 0)
  {
    for(std::size_t i = 0; i < semantics->size(); ++i)
      writer.writeElement("SemanticTypeIdentifier", semantics->at(i));
  }
  // Rules
  for(std::size_t i = 0; i < cs->getNRules(); ++i)
    te::serialize::Save(cs->getRule(i), writer);

  // Online Resources (...)

  writer.writeEndElement("CoverageStyle");
}
