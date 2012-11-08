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
  \file terralib/serialization/se/Symbolizer.cpp
   
  \brief Auxiliary classes and functions to serialize symbolizer informations from a XML document.
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../se/LineSymbolizer.h"
#include "../../se/PointSymbolizer.h"
#include "../../se/PolygonSymbolizer.h"
#include "../../se/RasterSymbolizer.h"
#include "../../se/TextSymbolizer.h"
#include "../Exception.h"
#include "Symbolizer.h"
#include "Utils.h"

// STL
#include <cassert>

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/format.hpp>

te::se::Symbolizer* LineSymbolizerReader(te::xml::Reader& reader);
te::se::Symbolizer* PointSymbolizerReader(te::xml::Reader& reader);
te::se::Symbolizer* PolygonSymbolizerReader(te::xml::Reader& reader);
te::se::Symbolizer* RasterSymbolizerReader(te::xml::Reader& reader);
te::se::Symbolizer* TextSymbolizerReader(te::xml::Reader& reader);

void LineSymbolizerWriter(const te::se::Symbolizer* symbolizer, te::xml::Writer& writer);
void PointSymbolizerWriter(const te::se::Symbolizer* symbolizer, te::xml::Writer& writer);
void PolygonSymbolizerWriter(const te::se::Symbolizer* symbolizer, te::xml::Writer& writer);
void RasterSymbolizerWriter(const te::se::Symbolizer* symbolizer, te::xml::Writer& writer);
void TextSymbolizerWriter(const te::se::Symbolizer* symbolizer, te::xml::Writer& writer);

void te::serialize::Symbolizer::reg(const std::string& symbolizerType, const SymbolizerFnctSerializeType& fncts)
{
  m_fncts[symbolizerType] = fncts;
}

te::se::Symbolizer* te::serialize::Symbolizer::read(te::xml::Reader& reader) const
{
  std::string symbolizerType = boost::to_upper_copy(reader.getElementLocalName());

  SymbolizerFnctIdxType::const_iterator it = m_fncts.find(symbolizerType);

  if(it == m_fncts.end())
    throw Exception((boost::format(TR_SERIALIZATION("Could not find a reader for the following symbolizer type: %1%.")) % symbolizerType).str());

  assert(it->second.second);

  return it->second.first(reader);
}

void te::serialize::Symbolizer::write(const te::se::Symbolizer* symbolizer, te::xml::Writer& writer) const
{
  assert(symbolizer);

  SymbolizerFnctIdxType::const_iterator it = m_fncts.find(symbolizer->getType());

  if(it == m_fncts.end())
    throw Exception((boost::format(TR_SERIALIZATION("Could not find a writer for the following symbolizer type: %1%.")) % symbolizer->getType()).str());

  assert(it->second.second);

  return it->second.second(symbolizer, writer);
}

te::serialize::Symbolizer::~Symbolizer()
{
}

te::serialize::Symbolizer::Symbolizer()
{
  m_fncts["LINESYMBOLIZER"] = std::make_pair(SymbolizerReadFnctType(&LineSymbolizerReader), SymbolizerWriteFnctType(&LineSymbolizerWriter));
  m_fncts["POINTSYMBOLIZER"] = std::make_pair(SymbolizerReadFnctType(&PointSymbolizerReader), SymbolizerWriteFnctType(&PointSymbolizerWriter));
  m_fncts["POLYGONSYMBOLIZER"] = std::make_pair(SymbolizerReadFnctType(&PolygonSymbolizerReader), SymbolizerWriteFnctType(&PolygonSymbolizerWriter));
  m_fncts["RASTERSYMBOLIZER"] = std::make_pair(SymbolizerReadFnctType(&RasterSymbolizerReader), SymbolizerWriteFnctType(&RasterSymbolizerWriter));
  m_fncts["TEXTSYMBOLIZER"] = std::make_pair(SymbolizerReadFnctType(&TextSymbolizerReader), SymbolizerWriteFnctType(&TextSymbolizerWriter));
}

te::se::Symbolizer* LineSymbolizerReader(te::xml::Reader& reader)
{
  return 0;
}

te::se::Symbolizer* PointSymbolizerReader(te::xml::Reader& reader)
{
  return 0;
}

te::se::Symbolizer* PolygonSymbolizerReader(te::xml::Reader& reader)
{
  return 0;
}

te::se::Symbolizer* RasterSymbolizerReader(te::xml::Reader& reader)
{
  return 0;
}

te::se::Symbolizer* TextSymbolizerReader(te::xml::Reader& reader)
{
  return 0;
}

void LineSymbolizerWriter(const te::se::Symbolizer* symbolizer, te::xml::Writer& writer)
{
  const te::se::LineSymbolizer* ls = dynamic_cast<const te::se::LineSymbolizer*>(symbolizer);

  if(ls == 0)
    return;
}

void PointSymbolizerWriter(const te::se::Symbolizer* symbolizer, te::xml::Writer& writer)
{
  const te::se::PointSymbolizer* ps = dynamic_cast<const te::se::PointSymbolizer*>(symbolizer);

  if(ps == 0)
    return;
}

void PolygonSymbolizerWriter(const te::se::Symbolizer* symbolizer, te::xml::Writer& writer)
{
  const te::se::PolygonSymbolizer* ps = dynamic_cast<const te::se::PolygonSymbolizer*>(symbolizer);

  if(ps == 0)
    return;
}

void RasterSymbolizerWriter(const te::se::Symbolizer* symbolizer, te::xml::Writer& writer)
{
  const te::se::RasterSymbolizer* rs = dynamic_cast<const te::se::RasterSymbolizer*>(symbolizer);

  if(rs == 0)
    return;
}

void TextSymbolizerWriter(const te::se::Symbolizer* symbolizer, te::xml::Writer& writer)
{
  const te::se::TextSymbolizer* ts = dynamic_cast<const te::se::TextSymbolizer*>(symbolizer);

  if(ts == 0)
    return;
}
