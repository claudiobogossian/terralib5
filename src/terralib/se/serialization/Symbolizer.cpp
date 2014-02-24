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
  \file terralib/se/serialization/Symbolizer.cpp
   
  \brief Auxiliary classes and functions to serialize symbolizer informations from a XML document.
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../fe/Expression.h"
#include "../../maptools/Utils.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../se/LineSymbolizer.h"
#include "../../se/ParameterValue.h"
#include "../../se/PointSymbolizer.h"
#include "../../se/PolygonSymbolizer.h"
#include "../../se/RasterSymbolizer.h"
#include "../../se/TextSymbolizer.h"
#include "../../xlink/XLinkSerializer.h"
#include "../Exception.h"
#include "../Utils.h"
#include "Symbolizer.h"
#include "SymbologySerializer.h"
#include "Utils.h"

// STL
#include <cassert>

// Boost
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

void te::se::serialize::Symbolizer::reg(const std::string& symbolizerType, const SymbolizerFnctSerializeType& fncts)
{
  m_fncts[symbolizerType] = fncts;
}

te::se::Symbolizer* te::se::serialize::Symbolizer::read(te::xml::Reader& reader) const
{
  std::string symbolizerType = reader.getElementLocalName();

  SymbolizerFnctIdxType::const_iterator it = m_fncts.find(symbolizerType);

  if(it == m_fncts.end())
    throw Exception((boost::format(TR_SE("Could not find a reader for the following symbolizer type: %1%.")) % symbolizerType).str());

  assert(it->second.second);

  return it->second.first(reader);
}

void te::se::serialize::Symbolizer::write(const te::se::Symbolizer* symbolizer, te::xml::Writer& writer) const
{
  assert(symbolizer);

  SymbolizerFnctIdxType::const_iterator it = m_fncts.find(symbolizer->getType());

  if(it == m_fncts.end())
    throw Exception((boost::format(TR_SE("Could not find a writer for the following symbolizer type: %1%.")) % symbolizer->getType()).str());

  assert(it->second.second);

  return it->second.second(symbolizer, writer);
}

te::se::serialize::Symbolizer::~Symbolizer()
{
}

te::se::serialize::Symbolizer::Symbolizer()
{
  m_fncts["LineSymbolizer"] = std::make_pair(SymbolizerReadFnctType(&LineSymbolizerReader), SymbolizerWriteFnctType(&LineSymbolizerWriter));
  m_fncts["PointSymbolizer"] = std::make_pair(SymbolizerReadFnctType(&PointSymbolizerReader), SymbolizerWriteFnctType(&PointSymbolizerWriter));
  m_fncts["PolygonSymbolizer"] = std::make_pair(SymbolizerReadFnctType(&PolygonSymbolizerReader), SymbolizerWriteFnctType(&PolygonSymbolizerWriter));
  m_fncts["RasterSymbolizer"] = std::make_pair(SymbolizerReadFnctType(&RasterSymbolizerReader), SymbolizerWriteFnctType(&RasterSymbolizerWriter));
  m_fncts["TextSymbolizer"] = std::make_pair(SymbolizerReadFnctType(&TextSymbolizerReader), SymbolizerWriteFnctType(&TextSymbolizerWriter));
}

te::se::Symbolizer* LineSymbolizerReader(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "LineSymbolizer");

  std::auto_ptr<te::se::LineSymbolizer> ls(new te::se::LineSymbolizer);

  // Common elements & attributes of Symbolizers
  te::se::serialize::ReadSymbolizerHelper(ls.get(), reader);

  // Geometry
  if(reader.getElementLocalName() == "Geometry")
    ls->setGeometry(te::se::serialize::ReadGeometryPropertyHelper(reader));

  // Stroke
  if(reader.getElementLocalName() == "Stroke")
    ls->setStroke(te::se::serialize::ReadStroke(reader));

  // PerpendicularOffset
  if(reader.getElementLocalName() == "PerpendicularOffset")
  {
    reader.next();
    ls->setPerpendicularOffset(te::se::serialize::ReadParameterValue(reader));
    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return ls.release();
}

te::se::Symbolizer* PointSymbolizerReader(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "PointSymbolizer");

  std::auto_ptr<te::se::PointSymbolizer> ps(new te::se::PointSymbolizer);

  // Common elements & attributes of Symbolizers
  te::se::serialize::ReadSymbolizerHelper(ps.get(), reader);

  // Geometry
  if(reader.getElementLocalName() == "Geometry")
    ps->setGeometry(te::se::serialize::ReadGeometryPropertyHelper(reader));

  // Graphic
  if(reader.getElementLocalName() == "Graphic")
    ps->setGraphic(te::se::serialize::ReadGraphic(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return ps.release();
}

te::se::Symbolizer* PolygonSymbolizerReader(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "PolygonSymbolizer");

  std::auto_ptr<te::se::PolygonSymbolizer> ps(new te::se::PolygonSymbolizer);

  // Common elements & attributes of Symbolizers
  te::se::serialize::ReadSymbolizerHelper(ps.get(), reader);

  // Geometry
  if(reader.getElementLocalName() == "Geometry")
    ps->setGeometry(te::se::serialize::ReadGeometryPropertyHelper(reader));

  // Fill
  if(reader.getElementLocalName() == "Fill")
    ps->setFill( te::se::serialize::ReadFill(reader));

  // Stroke
  if(reader.getElementLocalName() == "Stroke")
    ps->setStroke(te::se::serialize::ReadStroke(reader));

  // Displacement
  if(reader.getElementLocalName() == "Displacement")
    ps->setDisplacement(te::se::serialize::ReadDisplacement(reader));

  // PerpendicularOffset
  if(reader.getElementLocalName() == "PerpendicularOffset")
  {
    reader.next();
    ps->setPerpendicularOffset(te::se::serialize::ReadParameterValue(reader));
    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return ps.release();
}

te::se::Symbolizer* RasterSymbolizerReader(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "RasterSymbolizer");

  std::auto_ptr<te::se::RasterSymbolizer> rs(new te::se::RasterSymbolizer);

  // Common elements & attributes of Symbolizers
  te::se::serialize::ReadSymbolizerHelper(rs.get(), reader);

  // TODO: Missing <Geometry> - <ogc:PropertyName> attribute on te::se::RasterSymbolizer

  // Opacity
  if(reader.getElementLocalName() == "Opacity")
  {
    reader.next();
    rs->setOpacity(te::se::serialize::ReadParameterValue(reader));
  }

  // ChannelSelection
  if(reader.getElementLocalName() == "ChannelSelection")
    rs->setChannelSelection(te::se::serialize::ReadChannelSelection(reader));

  // OverlapBehavior
  if(reader.getElementLocalName() == "OverlapBehavior")
    rs->setOverlapBehavior(te::se::serialize::ReadOverlapBehavior(reader));

  // ColorMap
  if(reader.getElementLocalName() == "ColorMap")
    rs->setColorMap(te::se::serialize::ReadColorMap(reader));

  // ContrastEnhancement
  if(reader.getElementLocalName() == "ContrastEnhancement")
    rs->setContrastEnhancement(te::se::serialize::ReadContrastEnhancement(reader));

  // ShadedRelief
  if(reader.getElementLocalName() == "ShadedRelief")
    rs->setShadedRelief(te::se::serialize::ReadShadedRelief(reader));

  // ImageOutline
  if(reader.getElementLocalName() == "ImageOutline")
    rs->setImageOutline(te::se::serialize::ReadImageOutline(reader));

  // Gain (TerraLib extension)
  if(reader.getElementLocalName() == "Gain")
  {
    reader.next();
    std::string gain = reader.getElementValue();
    rs->setGain(new te::se::ParameterValue(gain));
    reader.next();
    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();
  }

  std::string aaaa = reader.getElementLocalName();

  // Offset (TerraLib extension)
  if(reader.getElementLocalName() == "Offset")
  {
    reader.next();
    std::string offset = reader.getElementValue();
    rs->setOffset(new te::se::ParameterValue(offset));
    reader.next();
    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return rs.release();
}

te::se::Symbolizer* TextSymbolizerReader(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "TextSymbolizer");

  std::auto_ptr<te::se::TextSymbolizer> ts(new te::se::TextSymbolizer);

  // Common elements & attributes of Symbolizers
  te::se::serialize::ReadSymbolizerHelper(ts.get(), reader);

  // Geometry
  if(reader.getElementLocalName() == "Geometry")
    ts->setGeometry(te::se::serialize::ReadGeometryPropertyHelper(reader));

  // Label
  if(reader.getElementLocalName() == "Label")
  {
    reader.next();
    ts->setLabel(te::se::serialize::ReadParameterValue(reader));
  }

  // Font
  if(reader.getElementLocalName() == "Font")
    ts->setFont(te::se::serialize::ReadFont(reader));

  // LabelPlacement
  if(reader.getElementLocalName() == "LabelPlacement")
    ts->setLabelPlacement(te::se::serialize::ReadLabelPlacement(reader));

  // Halo
  if(reader.getElementLocalName() == "Halo")
    ts->setHalo(te::se::serialize::ReadHalo(reader));

  // Fill
  if(reader.getElementLocalName() == "Fill")
    ts->setFill(te::se::serialize::ReadFill(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return ts.release();
}

void LineSymbolizerWriter(const te::se::Symbolizer* symbolizer, te::xml::Writer& writer)
{
  const te::se::LineSymbolizer* ls = dynamic_cast<const te::se::LineSymbolizer*>(symbolizer);

  if(ls == 0)
    return;

  writer.writeStartElement("se:LineSymbolizer");

  // Common elements & attributes of Symbolizers
  te::se::serialize::WriteSymbolizerHelper(ls, writer);

  // Specific elements of LineSymbolizer
  te::se::serialize::WriteGeometryPropertyHelper(ls->getGeometry(), writer);
  te::se::serialize::Save(ls->getStroke(), writer);
  te::se::serialize::WriteParameterValuePtrHelper("se:PerpendicularOffset", ls->getPerpendicularOffset(), writer);

  writer.writeEndElement("se:LineSymbolizer");
}

void PointSymbolizerWriter(const te::se::Symbolizer* symbolizer, te::xml::Writer& writer)
{
  const te::se::PointSymbolizer* ps = dynamic_cast<const te::se::PointSymbolizer*>(symbolizer);

  if(ps == 0)
    return;

  writer.writeStartElement("se:PointSymbolizer");

  // Common elements & attributes of Symbolizers
  te::se::serialize::WriteSymbolizerHelper(ps, writer);

  // Specific elements of PointSymbolizer
  te::se::serialize::WriteGeometryPropertyHelper(ps->getGeometry(), writer);
  te::se::serialize::Save(ps->getGraphic(), writer);

  writer.writeEndElement("se:PointSymbolizer");
}

void PolygonSymbolizerWriter(const te::se::Symbolizer* symbolizer, te::xml::Writer& writer)
{
  const te::se::PolygonSymbolizer* ps = dynamic_cast<const te::se::PolygonSymbolizer*>(symbolizer);

  if(ps == 0)
    return;

  writer.writeStartElement("se:PolygonSymbolizer");

  // Common elements & attributes of Symbolizers
  te::se::serialize::WriteSymbolizerHelper(ps, writer);

  // Specific elements of PolygonSymbolizer
  te::se::serialize::WriteGeometryPropertyHelper(ps->getGeometry(), writer);
  te::se::serialize::Save(ps->getFill(), writer);
  te::se::serialize::Save(ps->getStroke(), writer);
  te::se::serialize::Save(ps->getDisplacement(), writer);
  te::se::serialize::WriteParameterValuePtrHelper("se:PerpendicularOffset", ps->getPerpendicularOffset(), writer);

  writer.writeEndElement("se:PolygonSymbolizer");
}

void RasterSymbolizerWriter(const te::se::Symbolizer* symbolizer, te::xml::Writer& writer)
{
  const te::se::RasterSymbolizer* rs = dynamic_cast<const te::se::RasterSymbolizer*>(symbolizer);

  if(rs == 0)
    return;

  writer.writeStartElement("se:RasterSymbolizer");

  // Common elements & attributes of Symbolizers
  te::se::serialize::WriteSymbolizerHelper(rs, writer);

  // Specific elements of RasterSymbolizer
  // TODO: Missing <Geometry> - <ogc:PropertyName> attribute on te::se::RasterSymbolizer
  te::se::serialize::WriteParameterValuePtrHelper("se:Opacity", rs->getOpacity(), writer);
  //te::serialize::WriteParameterValuePtrHelper("Gain", rs->getGain(), writer); // Offset (TerraLib extension) TODO: In this case, how deal with xsd?!
  //te::serialize::WriteParameterValuePtrHelper("Offset", rs->getOffset(), writer); // Offset (TerraLib extension) TODO: In this case, how deal with xsd?!
  te::se::serialize::Save(rs->getChannelSelection(), writer);
  te::se::serialize::Save(rs->getOverlapBehavior(), writer);
  te::se::serialize::Save(rs->getColorMap(), writer);
  te::se::serialize::Save(rs->getContrastEnhancement(), writer);
  te::se::serialize::Save(rs->getShadedRelief(), writer);
  te::se::serialize::Save(rs->getImageOutline(), writer);

  double gain = GetDouble(rs->getGain());
  writer.writeElement("se:Gain", gain);

  double offset = GetDouble(rs->getOffset());
  writer.writeElement("se:Offset", offset);

  writer.writeEndElement("se:RasterSymbolizer");
}

void TextSymbolizerWriter(const te::se::Symbolizer* symbolizer, te::xml::Writer& writer)
{
  const te::se::TextSymbolizer* ts = dynamic_cast<const te::se::TextSymbolizer*>(symbolizer);

  if(ts == 0)
    return;

  writer.writeStartElement("se:TextSymbolizer");

  // Common elements & attributes of Symbolizers
  te::se::serialize::WriteSymbolizerHelper(ts, writer);

  // Specific elements of TextSymbolizer
  te::se::serialize::WriteParameterValuePtrHelper("se:Label", ts->getLabel(), writer);
  te::se::serialize::Save(ts->getFont(), writer);
  te::se::serialize::Save(ts->getLabelPlacement(), writer);
  te::se::serialize::Save(ts->getHalo(), writer);
  te::se::serialize::Save(ts->getFill(), writer);

  writer.writeEndElement("se:TextSymbolizer");
}
