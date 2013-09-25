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
  \file Chart.cpp
   
  \brief Auxiliary classes and functions to read Chart information from a XML document.
*/

// TerraLib
#include "../../color/RGBAColor.h"
#include "../../common/Translator.h"
#include "../../dataaccess/serialization/xml/Serializer.h"
#include "../../maptools/Chart.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../Exception.h"
#include "Chart.h"

// STL
#include <cassert>
#include <memory>

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

std::auto_ptr<te::map::Chart> te::serialize::ReadChart(te::xml::Reader& reader)
{
  if(reader.getElementLocalName() != "Chart")
    return std::auto_ptr<te::map::Chart>();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Chart");

  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Type");

  reader.next();

  assert(reader.getNodeType() == te::xml::VALUE);

  te::map::ChartType type = reader.getElementValue() == "PIE" ? te::map::Pie : te::map::Bar;

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // Type

  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Properties");

  reader.next();

  std::vector<std::string> properties;

  while(reader.getNodeType() == te::xml::START_ELEMENT && 
        reader.getElementLocalName() == "PropertyName")
  {
    reader.next();

    assert(reader.getNodeType() == te::xml::VALUE);

    properties.push_back(reader.getElementValue());

    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT); // PropertyName

    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // Properties

  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Colors");

  reader.next();

  std::vector<te::color::RGBAColor> colors;
  while(reader.getNodeType() == te::xml::START_ELEMENT && 
        reader.getElementLocalName() == "RGBAColor")
  {
    colors.push_back(ReadRGBAColor(reader));
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // Colors

  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "ContourColor");

  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "RGBAColor");

  te::color::RGBAColor contourColor = ReadRGBAColor(reader);

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // ContourColor

  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "ContourWidth");

  reader.next();

  assert(reader.getNodeType() == te::xml::VALUE);

  std::size_t contourWidth = reader.getElementValueAsInt32();

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // ContourWidth

  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Height");

  reader.next();

  assert(reader.getNodeType() == te::xml::VALUE);

  std::size_t height = reader.getElementValueAsInt32();

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // Height

  reader.next();

  std::size_t barWidth = -1;

  if(reader.getNodeType() == te::xml::START_ELEMENT &&
     reader.getElementLocalName() == "BarWidth")
  {
    reader.next();

    assert(reader.getNodeType() == te::xml::VALUE);

    barWidth = reader.getElementValueAsInt32();

    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT); // BarWidth

    reader.next();
  }

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "IsVisible");

  reader.next();

  assert(reader.getNodeType() == te::xml::VALUE);

  bool isVisible = reader.getElementValueAsBoolean();

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // IsVisible

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // Chart

  reader.next();

  std::auto_ptr<te::map::Chart> chart(new te::map::Chart(type, properties, colors));
  chart->setContourColor(contourColor);
  chart->setContourWidth(contourWidth);
  chart->setHeight(height);
  chart->setVisibility(isVisible);
  if(barWidth != -1)
    chart->setBarWidth(barWidth);

  return chart;
}

te::color::RGBAColor te::serialize::ReadRGBAColor(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "RGBAColor");

  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "R");

  reader.next();

  assert(reader.getNodeType() == te::xml::VALUE);

  std::size_t r = reader.getElementValueAsInt32();

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // R
  
  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "G");

  reader.next();

  assert(reader.getNodeType() == te::xml::VALUE);

  std::size_t g = reader.getElementValueAsInt32();

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // G
  
  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "B");

  reader.next();

  assert(reader.getNodeType() == te::xml::VALUE);

  std::size_t b = reader.getElementValueAsInt32();

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // B
  
  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "A");

  reader.next();

  assert(reader.getNodeType() == te::xml::VALUE);

  std::size_t a = reader.getElementValueAsInt32();

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // A
  
  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // RGBAColor

  reader.next();

  te::color::RGBAColor color(r, g, b, a);

  return color;
}

void te::serialize::Save(const te::map::Chart* chart, te::xml::Writer& writer)
{
  writer.writeStartElement("te_map:Chart");

  te::map::ChartType type = chart->getType();

  writer.writeElement("te_map:Type", (type == te::map::Pie ? "PIE" : "BAR"));

  std::vector<std::string> properties = chart->getProperties();

  writer.writeStartElement("te_map:Properties");

  for(std::size_t i = 0; i < properties.size(); ++i)
  {
    writer.writeElement("te_map:PropertyName", properties[i]);
  }

  writer.writeEndElement("te_map:Properties");

  writer.writeStartElement("te_map:Colors");

  for(std::size_t i = 0; i < properties.size(); ++i)
  {
    te::color::RGBAColor color = chart->getColor(i);
    Save(color, writer);
  }

  writer.writeEndElement("te_map:Colors");

  writer.writeStartElement("te_map:ContourColor");
  Save(chart->getContourColor(), writer);
  writer.writeEndElement("te_map:ContourColor");

  writer.writeElement("te_map:ContourWidth", boost::lexical_cast<int>(chart->getContourWidth()));

  writer.writeElement("te_map:Height", boost::lexical_cast<double>(chart->getHeight()));

  if(chart->getType() == te::map::Bar)
    writer.writeElement("te_map:BarWidth", boost::lexical_cast<int>(chart->getBarWidth()));

  writer.writeElement("te_map:IsVisible", (chart->isVisible() ? "TRUE" : "FALSE"));

  writer.writeEndElement("te_map:Chart");
}

void te::serialize::Save(const te::color::RGBAColor color, te::xml::Writer& writer)
{
  writer.writeStartElement("te_map:RGBAColor");

  writer.writeElement("R", color.getRed());
  writer.writeElement("G", color.getGreen());
  writer.writeElement("B", color.getBlue());
  writer.writeElement("A", color.getAlpha());

  writer.writeEndElement("te_map:RGBAColor");
}