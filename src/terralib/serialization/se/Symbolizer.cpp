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
#include "../xlink/SimpleLink.h"
#include "../Exception.h"
#include "ChannelSelection.h"
#include "ColorMap.h"
#include "ContrastEnhancement.h"
#include "Description.h"
#include "Displacement.h"
#include "Fill.h"
#include "Font.h"
#include "Graphic.h"
#include "Halo.h"
#include "ImageOutline.h"
#include "LabelPlacement.h"
#include "OverlapBehavior.h"
#include "ShadedRelief.h"
#include "Stroke.h"
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

  writer.writeStartElement("LineSymbolizer");

  // Common elements & attributes of Symbolizers
  te::serialize::WriterSymbolizerHelper(ls, writer);

  // Specific elements of LineSymbolizer
  //te::serialize::Save(ls->getGeometry(), writer); // TODO: fe serialize!
  te::serialize::Save(ls->getStroke(), writer);
  te::serialize::WriteParameterValuePtrHelper("PerpendicularOffset", ls->getPerpendicularOffset(), writer);

  writer.writeEndElement("LineSymbolizer");
}

void PointSymbolizerWriter(const te::se::Symbolizer* symbolizer, te::xml::Writer& writer)
{
  const te::se::PointSymbolizer* ps = dynamic_cast<const te::se::PointSymbolizer*>(symbolizer);

  if(ps == 0)
    return;

  writer.writeStartElement("PointSymbolizer");

  // Common elements & attributes of Symbolizers
  te::serialize::WriterSymbolizerHelper(ps, writer);

  // Specific elements of PointSymbolizer
  //te::serialize::Save(ps->getGeometry(), writer); // TODO: fe serialize!
  te::serialize::Save(ps->getGraphic(), writer);

  writer.writeEndElement("PointSymbolizer");
}

void PolygonSymbolizerWriter(const te::se::Symbolizer* symbolizer, te::xml::Writer& writer)
{
  const te::se::PolygonSymbolizer* ps = dynamic_cast<const te::se::PolygonSymbolizer*>(symbolizer);

  if(ps == 0)
    return;

  writer.writeStartElement("PolygonSymbolizer");

  // Common elements & attributes of Symbolizers
  te::serialize::WriterSymbolizerHelper(ps, writer);

  // Specific elements of PolygonSymbolizer
  //te::serialize::Save(ps->getGeometry(), writer); // TODO: fe serialize!
  te::serialize::Save(ps->getFill(), writer);
  te::serialize::Save(ps->getStroke(), writer);
  te::serialize::Save(ps->getDisplacement(), writer);
  te::serialize::WriteParameterValuePtrHelper("PerpendicularOffset", ps->getPerpendicularOffset(), writer);

  writer.writeEndElement("PolygonSymbolizer");
}

void RasterSymbolizerWriter(const te::se::Symbolizer* symbolizer, te::xml::Writer& writer)
{
  const te::se::RasterSymbolizer* rs = dynamic_cast<const te::se::RasterSymbolizer*>(symbolizer);

  if(rs == 0)
    return;

  writer.writeStartElement("RasterSymbolizer");

  // Common elements & attributes of Symbolizers
  te::serialize::WriterSymbolizerHelper(rs, writer);

  // Specific elements of RasterSymbolizer
  //te::serialize::Save(rs->getGeometry(), writer); // TODO: fe serialize!
  te::serialize::WriteParameterValuePtrHelper("Opacity", rs->getOpacity(), writer);
  te::serialize::WriteParameterValuePtrHelper("Gain", rs->getGain(), writer);
  te::serialize::WriteParameterValuePtrHelper("Offset", rs->getOffset(), writer);
  te::serialize::Save(rs->getChannelSelection(), writer);
  te::serialize::Save(rs->getOverlapBehavior(), writer);
  te::serialize::Save(rs->getColorMap(), writer);
  te::serialize::Save(rs->getContrastEnhancement(), writer);
  te::serialize::Save(rs->getShadedRelief(), writer);
  te::serialize::Save(rs->getImageOutline(), writer);

  writer.writeEndElement("RasterSymbolizer");
}

void TextSymbolizerWriter(const te::se::Symbolizer* symbolizer, te::xml::Writer& writer)
{
  const te::se::TextSymbolizer* ts = dynamic_cast<const te::se::TextSymbolizer*>(symbolizer);

  if(ts == 0)
    return;

  writer.writeStartElement("TextSymbolizer");

  // Common elements & attributes of Symbolizers
  te::serialize::WriterSymbolizerHelper(ts, writer);

  //te::serialize::Save(ts->getGeometry(), writer); // TODO: fe serialize!
  te::serialize::WriteParameterValuePtrHelper("Label", ts->getLabel(), writer);
  te::serialize::Save(ts->getFont(), writer);
  te::serialize::Save(ts->getLabelPlacement(), writer);
  te::serialize::Save(ts->getHalo(), writer);
  te::serialize::Save(ts->getFill(), writer);

  writer.writeEndElement("TextSymbolizer");
}
