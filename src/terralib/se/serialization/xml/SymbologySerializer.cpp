/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/se/serialization/xml/SymbologySerializer.cpp

  \brief Data serialization for the Symbology Encoding module.
*/

// TerraLib
#include "../../../fe/serialization/xml/Filter.h"
#include "../../../fe/serialization/xml/Expression.h"
#include "../../../xlink/serialization/xml/XLinkSerializer.h"
#include "../../../xml/AbstractWriter.h"
#include "../../../xml/Reader.h"
#include "../../AnchorPoint.h"
#include "../../Categorize.h"
#include "../../ChannelSelection.h"
#include "../../ColorMap.h"
#include "../../ContrastEnhancement.h"
#include "../../Description.h"
#include "../../Displacement.h"
#include "../../ExternalGraphic.h"
#include "../../Fill.h"
#include "../../Font.h"
#include "../../Graphic.h"
#include "../../GraphicStroke.h"
#include "../../Halo.h"
#include "../../ImageOutline.h"
#include "../../InlineContent.h"
#include "../../Interpolate.h"
#include "../../InterpolationPoint.h"
#include "../../LabelPlacement.h"
#include "../../LinePlacement.h"
#include "../../MapItem.h"
#include "../../Mark.h"
#include "../../ParameterValue.h"
#include "../../PointPlacement.h"
#include "../../RasterSymbolizer.h"
#include "../../Recode.h"
#include "../../Rule.h"
#include "../../SelectedChannel.h"
#include "../../ShadedRelief.h"
#include "../../Stroke.h"
#include "../../SvgParameter.h"
#include "../../Symbolizer.h"
#include "SymbologySerializer.h"
#include "Symbolizer.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

//#ifdef TE_ENABLED_XML

te::se::AnchorPoint* te::se::serialize::ReadAnchorPoint(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "AnchorPoint");

  reader.next();

  std::auto_ptr<te::se::AnchorPoint> ap(new te::se::AnchorPoint);

  assert(reader.getElementLocalName() == "AnchorPointX");
  reader.next();
  ap->setAnchorPointX(ReadParameterValue(reader));

  assert(reader.getElementLocalName() == "AnchorPointY");
  reader.next();
  ap->setAnchorPointY(ReadParameterValue(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return ap.release();
}

void te::se::serialize::Save(const te::se::AnchorPoint* ap, te::xml::AbstractWriter& writer)
{
  if(ap == 0)
    return;

  writer.writeStartElement("se:AnchorPoint");

  WriteParameterValuePtrHelper("se:AnchorPointX", ap->getAnchorPointX(), writer);
  WriteParameterValuePtrHelper("se:AnchorPointY", ap->getAnchorPointY(), writer);

  writer.writeEndElement("se:AnchorPoint");
}

te::se::Categorize* te::se::serialize::ReadCategorize(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Categorize");

  assert(reader.hasAttrs());

  std::auto_ptr<te::se::Categorize> c(new te::se::Categorize);

  // FallBackValue Attribute
  std::string fbv = reader.getAttr("fallbackValue");
  assert(!fbv.empty());
  c->setFallbackValue(fbv);

  // ThreshholdsBelongTo Attribute
  std::string tbt = reader.getAttr("threshholdsBelongTo");
  assert(!tbt.empty() && (tbt == "preceding" || tbt == "succeeding"));
  tbt == "preceding" ? c->setThresholdsBelongTo(te::se::Categorize::PRECEDING) : c->setThresholdsBelongTo(te::se::Categorize::SUCCEEDING);

  reader.next();

  // LookupValue
  assert(reader.getElementLocalName() == "LookupValue");
  reader.next();
  c->setLookupValue(ReadParameterValue(reader));

  // Value
  assert(reader.getElementLocalName() == "Value");
  reader.next();
  c->addValue(ReadParameterValue(reader));

  // Threshold + Value
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
        reader.getElementLocalName() == "Threshold")
  {
    reader.next();
    c->addThreshold(ReadParameterValue(reader));

    assert(reader.getElementLocalName() == "Value");
    reader.next();
    c->addValue(ReadParameterValue(reader));
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return c.release();
}

void te::se::serialize::Save(const te::se::Categorize* c, te::xml::AbstractWriter& writer)
{
  if(c == 0)
    return;

  writer.writeStartElement("se:Categorize");

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

  WriteParameterValuePtrHelper("se:LookupValue", c->getLookupValue(), writer);
  WriteParameterValuePtrHelper("se:Value", c->getValue(), writer);

  std::vector<te::se::ParameterValue*> thresholds = c->getThresholds();
  std::vector<te::se::ParameterValue*> values = c->getThresholdValues();
  assert(thresholds.size() == values.size()-1);

  for(std::size_t i = 0; i < values.size(); ++i)
  {
    WriteParameterValuePtrHelper("se:Value", values[i], writer);

    if(i != values.size()-1)
      WriteParameterValuePtrHelper("se:Threshold", thresholds[i], writer);
  }

  writer.writeEndElement("se:Categorize");
}

te::se::ChannelSelection* te::se::serialize::ReadChannelSelection(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "ChannelSelection");

  reader.next();

  std::auto_ptr<te::se::ChannelSelection> cs(new te::se::ChannelSelection);
  cs->setColorCompositionType(te::se::UNKNOWN_COMPOSITION);

  // GrayChannel
  if(reader.getElementLocalName() == "GrayChannel")
  {
    cs->setGrayChannel(ReadSelectedChannel(reader));
    cs->setColorCompositionType(te::se::GRAY_COMPOSITION);

    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();

    return cs.release();
  }

  std::size_t nChannels = 0; // To count the number of channels

  // RedChannel
  if(reader.getElementLocalName() == "RedChannel")
  {
    cs->setRedChannel(ReadSelectedChannel(reader));
    cs->setColorCompositionType(te::se::RED_COMPOSITION);
    nChannels++;
  }

  // GreenChannel
  if(reader.getElementLocalName() == "GreenChannel")
  {
    cs->setGreenChannel(ReadSelectedChannel(reader));
    cs->setColorCompositionType(te::se::GREEN_COMPOSITION);
    nChannels++;
  }

  // BlueChannel
  if(reader.getElementLocalName() == "BlueChannel")
  {
    cs->setBlueChannel(ReadSelectedChannel(reader));
    cs->setColorCompositionType(te::se::BLUE_COMPOSITION);
    nChannels++;
  }

  assert(nChannels > 0);

  // Adjusting...
  if(nChannels > 1)
    nChannels == 3 ? cs->setColorCompositionType(te::se::RGB_COMPOSITION) : cs->setColorCompositionType(te::se::UNKNOWN_COMPOSITION);

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return cs.release();
}

void te::se::serialize::Save(const te::se::ChannelSelection* cs, te::xml::AbstractWriter& writer)
{
  if(cs == 0)
    return;

  writer.writeStartElement("se:ChannelSelection");

  if(cs->getColorCompositionType() == te::se::GRAY_COMPOSITION)
  {
    WriteSelectedChannelHelper("se:GrayChannel", cs->getGrayChannel(), writer);
  }
  else if(cs->getColorCompositionType() == te::se::RED_COMPOSITION)
  {
    WriteSelectedChannelHelper("se:RedChannel", cs->getRedChannel(), writer);
  }
  else if(cs->getColorCompositionType() == te::se::GREEN_COMPOSITION)
  {
    WriteSelectedChannelHelper("se:GreenChannel", cs->getGreenChannel(), writer);
  }
  else if(cs->getColorCompositionType() == te::se::BLUE_COMPOSITION)
  {
    WriteSelectedChannelHelper("se:BlueChannel", cs->getBlueChannel(), writer);
  }
  else if(cs->getColorCompositionType() == te::se::RGB_COMPOSITION)
  {
    WriteSelectedChannelHelper("se:RedChannel", cs->getRedChannel(), writer);
    WriteSelectedChannelHelper("se:GreenChannel", cs->getGreenChannel(), writer);
    WriteSelectedChannelHelper("se:BlueChannel", cs->getBlueChannel(), writer);
  }

  writer.writeEndElement("se:ChannelSelection");
}

te::se::ColorMap* te::se::serialize::ReadColorMap(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "ColorMap");

  reader.next();

  std::auto_ptr<te::se::ColorMap> cm(new te::se::ColorMap);

  if(reader.getElementLocalName() == "Categorize")
    cm->setCategorize(ReadCategorize(reader));
  else if (reader.getElementLocalName() == "Interpolate")
    cm->setInterpolate(ReadInterpolate(reader));
  else
    cm->setRecode(ReadRecode(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return cm.release();
}

void te::se::serialize::Save(const te::se::ColorMap* cm, te::xml::AbstractWriter& writer)
{
  if(cm == 0)
    return;

  writer.writeStartElement("se:ColorMap");

  te::se::Categorize* categorize = cm->getCategorize();
  te::se::Interpolate* interpolate = cm->getInterpolate();
  te::se::Recode* recode = cm->getRecode();
  if(categorize)
    Save(categorize, writer);
  else if (interpolate)
  {
    assert(interpolate);
    Save(interpolate, writer);
  }
  else if (recode)
  {
    Save(recode, writer);
  }

  writer.writeEndElement("se:ColorMap");
}

te::se::ContrastEnhancement* te::se::serialize::ReadContrastEnhancement(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "ContrastEnhancement");

  reader.next();

  std::auto_ptr<te::se::ContrastEnhancement> ce(new te::se::ContrastEnhancement);
  ce->setContrastEnhancementType(te::se::ContrastEnhancement::ENHANCEMENT_NONE);

  if(reader.getElementLocalName() == "Histogram")
  {
    ce->setContrastEnhancementType(te::se::ContrastEnhancement::ENHANCEMENT_HISTOGRAM);
    reader.next();
  }
  else if(reader.getElementLocalName() == "Normalize")
  {
    ce->setContrastEnhancementType(te::se::ContrastEnhancement::ENHANCEMENT_NORMALIZE);
    reader.next();
  }

  if(reader.getElementLocalName() == "GammaValue")
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    ce->setGammaValue(reader.getElementValueAsDouble());
    reader.next();
    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return ce.release();
}

void te::se::serialize::Save(const te::se::ContrastEnhancement* ce, te::xml::AbstractWriter& writer)
{
  if(ce == 0)
    return;

  writer.writeStartElement("se:ContrastEnhancement");
  te::se::ContrastEnhancement::ContrastEnhancementType type = ce->getContrastEnhancementType();
  switch(type)
  {
    case te::se::ContrastEnhancement::ENHANCEMENT_NONE:
    break;

    case te::se::ContrastEnhancement::ENHANCEMENT_HISTOGRAM:
      writer.writeElement("se:Histogram", ""); // TODO: Writer method to writer a empty element, i.e. <Histogram/>
    break;

    case te::se::ContrastEnhancement::ENHANCEMENT_NORMALIZE:
      writer.writeElement("se:Normalize", ""); // TODO: Writer method to writer a empty element, i.e. <Normalize/>
    break;
  }

  writer.writeElement("se:GammaValue", ce->getGammaValue());

  writer.writeEndElement("se:ContrastEnhancement");
}

te::se::Description* te::se::serialize::ReadDescription(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Description");

  reader.next();

  std::auto_ptr<te::se::Description> description(new te::se::Description);

  // Title
  if(reader.getElementLocalName() == "Title")
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    std::string title = reader.getElementValue();
    description->setTitle(title);
    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();
  }

  // Abstract
  if(reader.getElementLocalName() == "Abstract")
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    std::string abs = reader.getElementValue();
    description->setAbstract(abs);
    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return description.release();
}

void te::se::serialize::Save(const te::se::Description* d, te::xml::AbstractWriter& writer)
{
  if(d == 0)
    return;

  writer.writeStartElement("se:Description");
  
  if(!d->getTitle().empty())
    writer.writeElement("se:Title", d->getTitle());
  
  if(!d->getAbstract().empty())
    writer.writeElement("se:Abstract", d->getAbstract());

  writer.writeEndElement("se:Description");
}

te::se::Displacement* te::se::serialize::ReadDisplacement(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Displacement");

  reader.next();

  std::auto_ptr<te::se::Displacement> d(new te::se::Displacement);

  assert(reader.getElementLocalName() == "DisplacementX");
  reader.next();
  d->setDisplacementX(ReadParameterValue(reader));

  assert(reader.getElementLocalName() == "DisplacementY");
  reader.next();
  d->setDisplacementY(ReadParameterValue(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return d.release();
}

void te::se::serialize::Save(const te::se::Displacement* d, te::xml::AbstractWriter& writer)
{
  if(d == 0)
    return;

  writer.writeStartElement("se:Displacement");

  WriteParameterValuePtrHelper("se:DisplacementX", d->getDisplacementX(), writer);
  WriteParameterValuePtrHelper("se:DisplacementY", d->getDisplacementY(), writer);

  writer.writeEndElement("se:Displacement");
}

te::se::ExternalGraphic* te::se::serialize::ReadExternalGraphic(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "ExternalGraphic");

  reader.next();

  std::auto_ptr<te::se::ExternalGraphic> eg(new te::se::ExternalGraphic);

  if(reader.getElementLocalName() == "OnlineResource")
    eg->setOnlineResource(te::xl::serialize::ReadSimpleLink(reader));
  else // InlineContent
    eg->setInlineContent(ReadInlineContent(reader));

  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  reader.next();

  // Format
  assert(reader.getElementLocalName() == "Format");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  eg->setFormat(reader.getElementValue());
  reader.next();

  // Recodes (...)

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return eg.release();
}

void te::se::serialize::Save(const te::se::ExternalGraphic* eg, te::xml::AbstractWriter& writer)
{
  if(eg == 0)
    return;

  writer.writeStartElement("se:ExternalGraphic");

  const te::xl::SimpleLink* link = eg->getOnlineResource();
  if(link)
    te::se::serialize::WriteOnlineResourceHelper(link, writer);
  else
  {
    const te::se::InlineContent* ic = eg->getInlineContent();
    assert(ic);
    Save(ic, writer);
  }
  const std::string& format = eg->getFormat();
  assert(!format.empty());
  writer.writeElement("se:Format", format);

  // Recodes (...)

  writer.writeEndElement("se:ExternalGraphic");
}

te::se::Fill* te::se::serialize::ReadFill(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Fill");

  reader.next();

  std::auto_ptr<te::se::Fill> fill(new te::se::Fill);

  // GraphicFill
  if(reader.getElementLocalName() == "GraphicFill")
  {
    reader.next();
    fill->setGraphicFill(ReadGraphic(reader));
    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();
  }

  // SvgParameters
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
        reader.getElementLocalName() == "SvgParameter")
    fill->add(ReadSvgParameter(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return fill.release();
}

void te::se::serialize::Save(const te::se::Fill* fill, te::xml::AbstractWriter& writer)
{
  if(fill == 0)
    return;

  writer.writeStartElement("se:Fill");

  const te::se::Graphic* g = fill->getGraphicFill();
  if(g)
  {
    writer.writeStartElement("se:GraphicFill");
    Save(g, writer);
    writer.writeEndElement("se:GraphicFill");
  }
  else
  {
    Save(fill->getColor(), writer);
    Save(fill->getOpacity(), writer);
  }

  writer.writeEndElement("se:Fill");
}

te::se::Font* te::se::serialize::ReadFont(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Font");

  reader.next();

  std::auto_ptr<te::se::Font> font(new te::se::Font);

  // SvgParameters
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
        reader.getElementLocalName() == "SvgParameter")
    font->add(ReadSvgParameter(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return font.release();
}

void te::se::serialize::Save(const te::se::Font* font, te::xml::AbstractWriter& writer)
{
  if(font == 0)
    return;

  writer.writeStartElement("se:Font");

  Save(font->getFamily(), writer);
  Save(font->getStyle(), writer);
  Save(font->getWeight(), writer);
  Save(font->getSize(), writer);

  writer.writeEndElement("se:Font");
}

te::se::Graphic* te::se::serialize::ReadGraphic(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Graphic");

  reader.next();

  std::auto_ptr<te::se::Graphic> graphic(new te::se::Graphic);

  while(reader.getNodeType() == te::xml::START_ELEMENT &&
        (reader.getElementLocalName() == "Mark" ||
        reader.getElementLocalName() == "ExternalGraphic"))
    reader.getElementLocalName() == "Mark" ? graphic->add(ReadMark(reader)) : graphic->add(ReadExternalGraphic(reader));

  // Opacity
  if(reader.getElementLocalName() == "Opacity")
  {
    reader.next();
    graphic->setOpacity(ReadParameterValue(reader));
  }

  // Size
  if(reader.getElementLocalName() == "Size")
  {
    reader.next();
    graphic->setSize(ReadParameterValue(reader));
  }

  // Rotation
  if(reader.getElementLocalName() == "Rotation")
  {
    reader.next();
    graphic->setRotation(ReadParameterValue(reader));
  }

  // AnchorPoint
  if(reader.getElementLocalName() == "AnchorPoint")
    graphic->setAnchorPoint(ReadAnchorPoint(reader));

  // Displacement
  if(reader.getElementLocalName() == "Displacement")
    graphic->setDisplacement(ReadDisplacement(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return graphic.release();
}

void te::se::serialize::Save(const te::se::Graphic* graphic, te::xml::AbstractWriter& writer)
{
  if(graphic == 0)
    return;

  writer.writeStartElement("se:Graphic");

  const std::vector<te::se::Mark*> marks = graphic->getMarks();
  for(std::size_t i = 0; i < marks.size(); ++i)
    Save(marks[i], writer);

  const std::vector<te::se::ExternalGraphic*> egs = graphic->getExternalGraphics();
  for(std::size_t i = 0; i < egs.size(); ++i)
    Save(egs[i], writer);

  WriteParameterValuePtrHelper("se:Opacity", graphic->getOpacity(), writer);
  WriteParameterValuePtrHelper("se:Size", graphic->getSize(), writer);
  WriteParameterValuePtrHelper("se:Rotation", graphic->getRotation(), writer);

  Save(graphic->getAnchorPoint(), writer);
  Save(graphic->getDisplacement(), writer);

  writer.writeEndElement("se:Graphic");
}

te::se::GraphicStroke* te::se::serialize::ReadGraphicStroke(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "GraphicStroke");

  reader.next();

  std::auto_ptr<te::se::GraphicStroke> graphicStroke(new te::se::GraphicStroke);

  assert(reader.getElementLocalName() == "Graphic");
  graphicStroke->setGraphic(ReadGraphic(reader));

  // InitialGap
  if(reader.getElementLocalName() == "InitialGap")
  {
    reader.next();
    graphicStroke->setInitialGap(ReadParameterValue(reader));
  }

  // Gap
  if(reader.getElementLocalName() == "Gap")
  {
    reader.next();
    graphicStroke->setGap(ReadParameterValue(reader));
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return graphicStroke.release();
}

void te::se::serialize::Save(const te::se::GraphicStroke* graphicStroke, te::xml::AbstractWriter& writer)
{
  if(graphicStroke == 0)
    return;

  writer.writeStartElement("se:GraphicStroke");

  const te::se::Graphic* graphic = graphicStroke->getGraphic();
  assert(graphic);
  Save(graphic, writer);

  WriteParameterValuePtrHelper("se:InitialGap", graphicStroke->getInitialGap(), writer);
  WriteParameterValuePtrHelper("se:Gap", graphicStroke->getGap(), writer);

  writer.writeEndElement("se:GraphicStroke");
}

te::se::Halo* te::se::serialize::ReadHalo(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Halo");

  reader.next();

  std::auto_ptr<te::se::Halo> halo(new te::se::Halo);

  // Radius
  if(reader.getElementLocalName() == "Radius")
  {
    reader.next();
    halo->setRadius(ReadParameterValue(reader));
  }
  
  // Fill
  if(reader.getElementLocalName() == "Fill")
    halo->setFill(ReadFill(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return halo.release();
}

void te::se::serialize::Save(const te::se::Halo* halo, te::xml::AbstractWriter& writer)
{
  if(halo == 0)
    return;

  writer.writeStartElement("se:Halo");

  WriteParameterValuePtrHelper("se:Radius", halo->getRadius(), writer);
  Save(halo->getFill(), writer);

  writer.writeEndElement("se:Halo");
}

te::se::ImageOutline* te::se::serialize::ReadImageOutline(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "ImageOutline");

  reader.next();

  std::auto_ptr<te::se::ImageOutline> imageOutline(new te::se::ImageOutline);
  imageOutline->setSymbolizer(Symbolizer::getInstance().read(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return imageOutline.release();
}

void te::se::serialize::Save(const te::se::ImageOutline* imageOutline, te::xml::AbstractWriter& writer)
{
  if(imageOutline == 0)
    return;

  writer.writeStartElement("se:ImageOutline");

  assert(imageOutline->getSymbolizer());
  Symbolizer::getInstance().write(imageOutline->getSymbolizer(), writer);

  writer.writeEndElement("se:ImageOutline");
}

te::se::InlineContent* te::se::serialize::ReadInlineContent(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "InlineContent");

  std::string encondingValue = reader.getAttr("enconding");
  assert(encondingValue == "xml" || encondingValue == "base64");

  te::se::InlineContent::EncodingType encodingType;
  encondingValue == "xml" ? encodingType = te::se::InlineContent::XML : encodingType = te::se::InlineContent::BASE64;

  std::string data = reader.getElementValue();

  std::auto_ptr<te::se::InlineContent> ic(new te::se::InlineContent(encodingType));
  ic->setData(data);

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return ic.release();
}

void te::se::serialize::Save(const te::se::InlineContent* ic, te::xml::AbstractWriter& writer)
{
  if(ic == 0)
    return;

  writer.writeStartElement("se:InlineContent");
  ic->getEncoding() == te::se::InlineContent::XML ? writer.writeAttribute("enconding", "xml") : writer.writeAttribute("enconding", "base64");
  writer.writeValue(ic->getData());
  writer.writeEndElement("se:InlineContent");
}

te::se::Interpolate* te::se::serialize::ReadInterpolate(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Interpolate");

  assert(reader.hasAttrs());

  std::auto_ptr<te::se::Interpolate> interpolate(new te::se::Interpolate);

  // FallBackValue Attribute
  std::string fbv = reader.getAttr("fallbackValue");
  assert(!fbv.empty());
  interpolate->setFallbackValue(fbv);

  // Mode Attribute
  std::string mode = reader.getAttr("mode");
  assert(!mode.empty() && (mode == "linear" || mode == "cosine" || mode == "cubic"));
  if(mode == "linear")
    interpolate->setModeType(te::se::Interpolate::LINEAR);
  else if(mode == "cosine")
    interpolate->setModeType(te::se::Interpolate::COSINE);
  else
    interpolate->setModeType(te::se::Interpolate::CUBIC);

  // Method Attribute
  std::string method = reader.getAttr("method");
  assert(!method.empty() && (method == "numeric" || method == "color"));
  method == "numeric" ? interpolate->setMethodType(te::se::Interpolate::NUMERIC) : interpolate->setMethodType(te::se::Interpolate::COLOR);

  reader.next();

  // LookupValue
  assert(reader.getElementLocalName() == "LookupValue");
  reader.next();
  interpolate->setLookupValue(ReadParameterValue(reader));

  // InterpolationPoints
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
        reader.getElementLocalName() == "InterpolationPoint")
  {
    //reader.next();
    interpolate->add(ReadInterpolationPoint(reader));
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return interpolate.release();
}

void te::se::serialize::Save(const te::se::Interpolate* interpolate, te::xml::AbstractWriter& writer)
{
  if(interpolate == 0)
    return;

  writer.writeStartElement("se:Interpolate");

  writer.writeAttribute("fallbackValue", interpolate->getFallbackValue());

  te::se::Interpolate::ModeType modeType = interpolate->getModeType();
  switch(modeType)
  {
    case te::se::Interpolate::LINEAR:
      writer.writeAttribute("mode", "linear");
    break;

    case te::se::Interpolate::COSINE:
      writer.writeAttribute("mode", "cosine");
    break;

    case te::se::Interpolate::CUBIC:
      writer.writeAttribute("mode", "cubic");
    break;  
  }

  te::se::Interpolate::MethodType methodType = interpolate->geMethodType();
  switch(methodType)
  {
    case te::se::Interpolate::NUMERIC:
      writer.writeAttribute("method", "numeric");
    break;

    case te::se::Interpolate::COLOR:
      writer.writeAttribute("method", "color");
    break;
  }

  WriteParameterValuePtrHelper("se:LookupValue", interpolate->getLookupValue(), writer);

  const std::vector<te::se::InterpolationPoint*> ipts = interpolate->getInterpolationPoints();
  assert(!ipts.empty());
  for(std::size_t i = 0; i < ipts.size(); ++i)
    Save(ipts[i], writer);

  writer.writeEndElement("se:Interpolate");
}

te::se::InterpolationPoint* te::se::serialize::ReadInterpolationPoint(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "InterpolationPoint");

  std::auto_ptr<te::se::InterpolationPoint> ip(new te::se::InterpolationPoint);

  reader.next();
  assert(reader.getElementLocalName() == "Data");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  double data = reader.getElementValueAsDouble();
  ip->setData(data);

  reader.next();
  reader.next();

  assert(reader.getElementLocalName() == "Value");
  reader.next();
  ip->setValue(ReadParameterValue(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return ip.release();
}

void te::se::serialize::Save(const te::se::InterpolationPoint* ip, te::xml::AbstractWriter& writer)
{
  if(ip == 0)
    return;

  writer.writeStartElement("se:InterpolationPoint");
  
  writer.writeElement("se:Data", ip->getData());
  assert(ip->getValue());
  WriteParameterValuePtrHelper("se:Value", ip->getValue(), writer);

  writer.writeEndElement("se:InterpolationPoint");
}

te::se::LabelPlacement* te::se::serialize::ReadLabelPlacement(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "LabelPlacement");

  reader.next();

  std::auto_ptr<te::se::LabelPlacement> lp(new te::se::LabelPlacement);

  if(reader.getElementLocalName() == "PointPlacement")
    lp->setPointPlacement(ReadPointPlacement(reader));
  else // LinePlacement
    lp->setLinePlacement(ReadLinePlacement(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return lp.release();
}

void te::se::serialize::Save(const te::se::LabelPlacement* lp, te::xml::AbstractWriter& writer)
{
  if(lp == 0)
    return;

  writer.writeStartElement("se:LabelPlacement");

  const te::se::PointPlacement* pp = lp->getPointPlacement();
  if(pp)
    Save(pp, writer);
  else
  {
    const te::se::LinePlacement* linePlacement = lp->getLinePlacement();
    assert(linePlacement);
    Save(linePlacement, writer);
  }

  writer.writeEndElement("se:LabelPlacement");
}

te::se::LinePlacement* te::se::serialize::ReadLinePlacement(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "LinePlacement");

  reader.next();

  std::auto_ptr<te::se::LinePlacement> lp(new te::se::LinePlacement);

  // PerpendicularOffset
  if(reader.getElementLocalName() == "PerpendicularOffset")
  {
    reader.next();
    lp->setPerpendicularOffset(ReadParameterValue(reader));
  }

  // IsRepeated
  if(reader.getElementLocalName() == "IsRepeated")
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    bool isRepeated = reader.getElementValueAsBoolean();
    lp->setIsRepeated(isRepeated);
    reader.next();
  }

  // InitialGap
  if(reader.getElementLocalName() == "InitialGap")
  {
    reader.next();
    lp->setInitialGap(ReadParameterValue(reader));
  }

  // Gap
  if(reader.getElementLocalName() == "Gap")
  {
    reader.next();
    lp->setGap(ReadParameterValue(reader));
  }

  // IsAligned
  if(reader.getElementLocalName() == "IsAligned")
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    bool isAligned = reader.getElementValueAsBoolean();
    lp->setIsAligned(isAligned);
    reader.next();
  }

  // GeneralizeLine
  if(reader.getElementLocalName() == "GeneralizeLine")
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    bool generalizeLine = reader.getElementValueAsBoolean();
    lp->setGeneralizeLine(generalizeLine);
    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return lp.release();
}

void te::se::serialize::Save(const te::se::LinePlacement* lp, te::xml::AbstractWriter& writer)
{
  if(lp == 0)
    return;

  writer.writeStartElement("se:LinePlacement");
  
  WriteParameterValuePtrHelper("se:PerpendicularOffset", lp->getPerpendicularOffset(), writer);

  std::string isRepeated;
  lp->isRepeated() ? isRepeated = "true" : isRepeated = "false";
  writer.writeElement("se:IsRepeated", isRepeated);

  WriteParameterValuePtrHelper("se:InitialGap", lp->getInitialGap(), writer);
  WriteParameterValuePtrHelper("se:Gap", lp->getGap(), writer);

  std::string isAligned;
  lp->isAligned() ? isAligned = "true" : isAligned = "false";
  writer.writeElement("se:IsAligned", isAligned);

  std::string generalizeLine;
  lp->getGeneralizeLine() ? generalizeLine = "true" : generalizeLine = "false";
  writer.writeElement("se:GeneralizeLine", generalizeLine);

  writer.writeEndElement("se:LinePlacement");
}

te::se::MapItem* te::se::serialize::ReadMapItem(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "MapItem");

  reader.next();

  std::auto_ptr<te::se::MapItem> mapItem(new te::se::MapItem);

  if (reader.getElementLocalName() == "Data")
  {
    reader.next();

    double data = reader.getElementValueAsDouble();
    mapItem->setData(data);

    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT);

    reader.next();
  }

  if (reader.getElementLocalName() == "Value")
  {
    reader.next();

    te::se::ParameterValue* value = ReadParameterValue(reader);

    mapItem->setValue(value);

  }

  if (reader.getElementLocalName() == "Title")
  {
    reader.next();

    if (reader.getNodeType() != te::xml::END_ELEMENT)
    {
      std::string title = reader.getElementValue();
      mapItem->setTitle(title);

      reader.next();
    }

    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  reader.next();

  return mapItem.release();
}

void te::se::serialize::Save(const te::se::MapItem* mapItem, te::xml::AbstractWriter& writer)
{
  if (!mapItem)
    return;

  double data = mapItem->getData();
  te::se::ParameterValue* value = mapItem->getValue();
  std::string title = mapItem->getTitle();

  writer.writeStartElement("se:MapItem");

  writer.writeElement("Data", data);
  writer.writeStartElement("Value");
  Save(value, writer);
  writer.writeEndElement("Value");
  writer.writeElement("Title", title);

  writer.writeEndElement("se:MapItem");
}

te::se::Mark* te::se::serialize::ReadMark(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Mark");

  reader.next();

  std::auto_ptr<te::se::Mark> mark(new te::se::Mark);

  // WellKnownName
  if(reader.getElementLocalName() == "WellKnownName")
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    mark->setWellKnownName(new std::string(reader.getElementValue()));
    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();
  }
  else if(reader.getElementLocalName() == "OnlineResource")
    mark->setOnlineResource(te::xl::serialize::ReadSimpleLink(reader));
  else // InlineContent
    mark->setInlineContent(ReadInlineContent(reader));

  // Fill
  if(reader.getElementLocalName() == "Fill")
    mark->setFill(ReadFill(reader));

  // Stroke
  if(reader.getElementLocalName() == "Stroke")
    mark->setStroke(ReadStroke(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return mark.release();
}

void te::se::serialize::Save(const te::se::Mark* mark, te::xml::AbstractWriter& writer)
{
  if(mark == 0)
    return;

  writer.writeStartElement("se:Mark");

  const std::string* wellKnownName = mark->getWellKnownName();
  if(wellKnownName)
    writer.writeElement("se:WellKnownName", *wellKnownName);
  else
  {
    const te::xl::SimpleLink* link = mark->getOnlineResource();
    if(link)
      WriteOnlineResourceHelper(link, writer);
    else
    {
      const te::se::InlineContent* ic = mark->getInlineContent();
      assert(ic);
      Save(ic, writer);
    }
    const std::string* format = mark->getFormat();
    assert(format);
    writer.writeElement("se:Format", *format);

    writer.writeElement("se:MarkIndex", mark->getMarkIndex());
  }

  Save(mark->getFill(), writer);
  Save(mark->getStroke(), writer);

  writer.writeEndElement("se:Mark");
}

te::se::RasterSymbolizer::OverlapBehavior te::se::serialize::ReadOverlapBehavior(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "OverlapBehavior");

  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string type = reader.getElementValue();
  reader.next();

  if(type == "AVERAGE")
    return te::se::RasterSymbolizer::AVERAGE;

  if(type == "EARLIEST_ON_TOP")
    return te::se::RasterSymbolizer::EARLIEST_ON_TOP;

  if(type == "LATEST_ON_TOP")
    return te::se::RasterSymbolizer::LATEST_ON_TOP;

  if(type == "RANDOM")
    return te::se::RasterSymbolizer::RANDOM;

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return te::se::RasterSymbolizer::NO_BEHAVIOR;
}

void te::se::serialize::Save(const te::se::RasterSymbolizer::OverlapBehavior& type, te::xml::AbstractWriter& writer)
{
  switch(type)
  {
    case te::se::RasterSymbolizer::NO_BEHAVIOR:
    break;

    case te::se::RasterSymbolizer::AVERAGE:
      writer.writeElement("se:OverlapBehavior", "AVERAGE");
    break;

    case te::se::RasterSymbolizer::EARLIEST_ON_TOP:
      writer.writeElement("se:OverlapBehavior", "EARLIEST_ON_TOP");
    break;

    case te::se::RasterSymbolizer::LATEST_ON_TOP:
      writer.writeElement("se:OverlapBehavior", "LATEST_ON_TOP");
    break;

    case te::se::RasterSymbolizer::RANDOM:
      writer.writeElement("se:OverlapBehavior", "RANDOM");
    break;
  }
}

te::se::ParameterValue* te::se::serialize::ReadParameterValue(te::xml::Reader& reader)
{
  std::auto_ptr<te::se::ParameterValue> param(new te::se::ParameterValue);

  // Expression TODO: (n's expressions?)
  te::se::ParameterValue::Parameter* p = new te::se::ParameterValue::Parameter;
  p->m_expression = te::fe::serialize::Expression::getInstance().read(reader);
  
  // TODO: and mixed data?!
  
  param->add(p);

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return param.release();
}

void te::se::serialize::Save(const te::se::ParameterValue* p, te::xml::AbstractWriter& writer)
{
  if(p == 0)
    return;

  std::size_t n = p->getNParameters();

  for(std::size_t i = 0; i != n; ++i)
  {
    const te::se::ParameterValue::Parameter* param = p->getParameter(i);

    if(param->m_mixedData)
      writer.writeValue(*(param->m_mixedData));
    else
      te::fe::serialize::Expression::getInstance().write(param->m_expression, writer);
  }
}

te::se::PointPlacement* te::se::serialize::ReadPointPlacement(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "PointPlacement");

  reader.next();

  std::auto_ptr<te::se::PointPlacement> pp(new te::se::PointPlacement);

  // AnchorPoint
  if(reader.getElementLocalName() == "AnchorPoint")
    pp->setAnchorPoint(ReadAnchorPoint(reader));

  // Displacement
  if(reader.getElementLocalName() == "Displacement")
    pp->setDisplacement(ReadDisplacement(reader));

  // Rotation
  if(reader.getElementLocalName() == "Rotation")
  {
    reader.next();
    pp->setRotation(ReadParameterValue(reader));
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return pp.release();
}

void te::se::serialize::Save(const te::se::PointPlacement* pp, te::xml::AbstractWriter& writer)
{
  if(pp == 0)
    return;

  writer.writeStartElement("se:PointPlacement");

  Save(pp->getAnchorPoint(), writer);
  Save(pp->getDisplacement(), writer);
  WriteParameterValuePtrHelper("se:Rotation", pp->getRotation(), writer);

  writer.writeEndElement("se:PointPlacement");
}

te::se::Recode* te::se::serialize::ReadRecode(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Recode");

  std::auto_ptr<te::se::Recode> recode(new te::se::Recode);

  reader.next();

  if (reader.getNodeType() == te::xml::END_ELEMENT && reader.getElementLocalName() == "Recode")
    return 0;

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "fallbackValue");

  reader.next();

  if (reader.getNodeType() == te::xml::VALUE)
  {
    recode->setFallbackValue(reader.getElementValue());

    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  assert(reader.getElementLocalName() == "fallbackValue");

  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "MapItem");

  while (reader.getNodeType() == te::xml::START_ELEMENT && reader.getElementLocalName() == "MapItem")
  {

    te::se::MapItem* mapItem = ReadMapItem(reader);

    recode->add(mapItem);
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  assert(reader.getElementLocalName() == "Recode");

  reader.next();

  return recode.release();
}

void te::se::serialize::Save(const te::se::Recode* recode, te::xml::AbstractWriter& writer)
{
  if (!recode)
    return;

  writer.writeStartElement("se:Recode");

  std::string fallbackValue = recode->getFallbackValue();

  writer.writeElement("fallbackValue", fallbackValue);

  std::vector<te::se::MapItem*> items = recode->getMapItems();

  for (std::size_t i = 0; i < items.size(); ++i)
  {
    Save(items[i], writer);
  }

  writer.writeEndElement("se:Recode");
}

te::se::Rule* te::se::serialize::ReadRule(te::xml::Reader& reader)
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
    rule->setFilter(te::fe::serialize::ReadFilter(reader));
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
    rule->push_back(te::se::serialize::Symbolizer::getInstance().read(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return rule.release();
}

void te::se::serialize::Save(const te::se::Rule* rule, te::xml::AbstractWriter& writer)
{
  if(rule == 0)
    return;

  writer.writeStartElement("se:Rule");

  WriteStringPtrHelper("se:Name", rule->getName(), writer);
  Save(rule->getDescription(), writer);

  const te::se::Graphic* legendGraphic = rule->getLegendGraphic();
  if(legendGraphic)
  {
    writer.writeStartElement("se:LegendGraphic");
    Save(rule->getLegendGraphic(), writer);
    writer.writeEndElement("se:LegendGraphic");
  }

  if(rule->getFilter())
    te::fe::serialize::Save(rule->getFilter(), writer);
  else if(rule->hasElseFilter())
    writer.writeElement("se:ElseFilter", "");

  //if(rule->getMinScaleDenominator() != 0.0)
    writer.writeElement("se:MinScaleDenominator", rule->getMinScaleDenominator());

  //if(rule->getMaxScaleDenominator() != std::numeric_limits<double>::infinity())
    writer.writeElement("se:MaxScaleDenominator", rule->getMaxScaleDenominator());

  const std::vector<te::se::Symbolizer*>& symbs = rule->getSymbolizers();
  for(std::size_t i = 0; i < symbs.size(); ++i)
    Symbolizer::getInstance().write(symbs[i], writer);

  writer.writeEndElement("se:Rule");
}

te::se::SelectedChannel* te::se::serialize::ReadSelectedChannel(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "GrayChannel" || reader.getElementLocalName() == "RedChannel" ||
         reader.getElementLocalName() == "GreenChannel" || reader.getElementLocalName() == "BlueChannel");

  reader.next();

  std::auto_ptr<te::se::SelectedChannel> sc(new te::se::SelectedChannel);

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "SourceChannelName");
  
  // SourceChannelName
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  sc->setSourceChannelName(reader.getElementValue());
  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  // ContrastEnhancement
  if(reader.getElementLocalName() == "ContrastEnhancement")
    sc->setContrastEnhancement(ReadContrastEnhancement(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return sc.release();
}

void te::se::serialize::Save(const te::se::SelectedChannel* sc, te::xml::AbstractWriter& writer)
{
  if(sc == 0)
    return;

  assert(!sc->getSourceChannelName().empty());
  writer.writeElement("se:SourceChannelName", sc->getSourceChannelName());
  
  Save(sc->getContrastEnhancement(), writer);
}

te::se::ShadedRelief* te::se::serialize::ReadShadedRelief(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "ShadedRelief");

  reader.next();

  std::auto_ptr<te::se::ShadedRelief> sr(new te::se::ShadedRelief);

  if(reader.getElementLocalName() == "BrightnessOnly")
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    sr->setBrightnessOnly(reader.getElementValueAsBoolean());
    reader.next();
  }

  if(reader.getElementLocalName() == "ReliefFactor")
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    sr->setReliefFactor(reader.getElementValueAsDouble());
    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return sr.release();
}

void te::se::serialize::Save(const te::se::ShadedRelief* sr, te::xml::AbstractWriter& writer)
{
  if(sr == 0)
    return;

  writer.writeStartElement("se:ShadedRelief");

  std::string brightnessOnlyValue;
  sr->isBrightnessOnly() ? brightnessOnlyValue = "true" : brightnessOnlyValue = "false";

  writer.writeElement("se:BrightnessOnly", brightnessOnlyValue);
  writer.writeElement("se:ReliefFactor", sr->getReliefFactor());

  writer.writeEndElement("se:ShadedRelief");
}

te::se::Stroke* te::se::serialize::ReadStroke(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Stroke");

  reader.next();

  std::auto_ptr<te::se::Stroke> stroke(new te::se::Stroke);

  // GraphicFill
  if(reader.getElementLocalName() == "GraphicFill")
  {
    reader.next();
    stroke->setGraphicFill(ReadGraphic(reader));

    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();
  }
  // GraphicStroke
  else if(reader.getElementLocalName() == "GraphicStroke")
    stroke->setGraphicStroke(ReadGraphicStroke(reader));

  // SvgParameters
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
        reader.getElementLocalName() == "SvgParameter")
    stroke->add(ReadSvgParameter(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return stroke.release();
}

void te::se::serialize::Save(const te::se::Stroke* stroke, te::xml::AbstractWriter& writer)
{
  if(stroke == 0)
    return;

  writer.writeStartElement("se:Stroke");

  if(stroke->getGraphicFill())
  {
    writer.writeStartElement("se:GraphicFill");
    Save(stroke->getGraphicFill(), writer);
    writer.writeEndElement("se:GraphicFill");
  }
  else if(stroke->getGraphicStroke())
    Save(stroke->getGraphicStroke(), writer);

  Save(stroke->getColor(), writer);
  Save(stroke->getOpacity(), writer);
  Save(stroke->getWidth(), writer);
  Save(stroke->getLineJoin(), writer);
  Save(stroke->getLineCap(), writer);
  Save(stroke->getDashArray(), writer);
  Save(stroke->setDashOffset(), writer);

  writer.writeEndElement("se:Stroke");
}

te::se::SvgParameter* te::se::serialize::ReadSvgParameter(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "SvgParameter");
  assert(reader.hasAttrs());

  std::string name = reader.getAttr("name");
  assert(!name.empty());

  std::auto_ptr<te::se::SvgParameter> svgParam(new te::se::SvgParameter(name));

  reader.next();

  // Expression TODO: (n's expressions?)
  te::se::ParameterValue::Parameter* p = new te::se::ParameterValue::Parameter;

  if(reader.getNodeType() == te::xml::VALUE)
  {
    p->m_mixedData = new std::string(reader.getElementValue());
    reader.next();
  }
  else
  {
    p->m_expression = te::fe::serialize::Expression::getInstance().read(reader);
  }

  svgParam->add(p);

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return svgParam.release();
}

void te::se::serialize::Save(const te::se::SvgParameter* p, te::xml::AbstractWriter& writer)
{
  if(p == 0)
    return;

  writer.writeStartElement("se:SvgParameter");

  writer.writeAttribute("name", p->getName());

  Save(static_cast<const te::se::ParameterValue*>(p), writer);

  writer.writeEndElement("se:SvgParameter");
}

//#endif
