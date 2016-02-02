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
  \file src/terralib/maptools/serialization/xml/Utils.cpp
   
  \brief Auxiliary functions to read layer information from a XML document.
*/

// TerraLib
#include "../../../se/serialization/xml/Symbolizer.h"
#include "../../../xml/AbstractWriter.h"
#include "../../../xml/Reader.h"
#include "../../AbstractLayer.h"
#include "../../Chart.h"
#include "../../Grouping.h"
#include "../../GroupingItem.h"
#include "Utils.h"

// Boost
#include <boost/lexical_cast.hpp>

// STL
#include <cassert>

te::map::Visibility te::map::serialize::GetVisibility(const std::string& visible)
{
  return (visible == "VISIBLE") ? te::map::VISIBLE :
         (visible == "NOT_VISIBLE") ? te::map::NOT_VISIBLE :
         te::map::PARTIALLY_VISIBLE;
}

std::string te::map::serialize::GetVisibility(const te::map::Visibility& visible)
{
  return (visible == te::map::VISIBLE) ? "VISIBLE" :
         (visible == te::map::NOT_VISIBLE) ? "NOT_VISIBLE" :
         "PARTIALLY_VISIBLE";
}

te::map::GroupingType te::map::serialize::GetGroupingType(const std::string& type)
{
  if(type == "EQUAL_STEPS")
    return te::map::EQUAL_STEPS;

  if(type == "QUANTIL")
    return te::map::QUANTIL;

  if(type == "STD_DEVIATION")
    return te::map::STD_DEVIATION;

  assert(type == "UNIQUE_VALUE");

  return te::map::UNIQUE_VALUE;
}

std::string te::map::serialize::GetGroupingType(const te::map::GroupingType& type)
{
  switch(type)
  {
    case te::map::EQUAL_STEPS:
      return "EQUAL_STEPS";

    case te::map::QUANTIL:
      return "QUANTIL";

    case te::map::STD_DEVIATION:
      return "STD_DEVIATION";

    default:
    {
      assert(type == te::map::UNIQUE_VALUE);
      return "UNIQUE_VALUE";
    }
  }
}

std::string te::map::serialize::ReadLayerTitle(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Title");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string title = reader.getElementValue();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  return title;
}

std::string te::map::serialize::ReadDataSetName(te::xml::Reader& reader)
{
  std::string dataset = "";

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "DataSetName");
  reader.next();
  if (reader.getNodeType() == te::xml::VALUE)
  {
    dataset = reader.getElementValue();
    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  return dataset;
}

std::string te::map::serialize::ReadDataSourceId(te::xml::Reader& reader)
{
  std::string datasourceId = "";

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "DataSourceId");
  reader.next();
  if (reader.getNodeType() == te::xml::VALUE)
  {
    datasourceId = reader.getElementValue();
    reader.next();
  }
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  return datasourceId;
}

std::string te::map::serialize::ReadLayerVisibility(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Visible");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string visible = reader.getElementValue();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  return visible;
}

te::map::Grouping* te::map::serialize::ReadLayerGrouping(te::xml::Reader& reader)
{
  if(reader.getElementLocalName() != "Grouping")
    return 0;

  /* Property Name */
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "PropertyName");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string propertyName = reader.getElementValue();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  /* Property Data Type */
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "PropertyDataType");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  int propertyType = reader.getElementValueAsInt32();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  /* Grouping Type */
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Type");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string type = reader.getElementValue();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  /* Precision */
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Precision");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::size_t precision = static_cast<std::size_t>(reader.getElementValueAsInt32());
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  std::auto_ptr<te::map::Grouping> g(new te::map::Grouping(propertyName, GetGroupingType(type), precision));
  g->setPropertyType(propertyType);

  /* Smmary */
  reader.next();
  if(reader.getElementLocalName() == "Summary")
  {
    assert(reader.getNodeType() == te::xml::START_ELEMENT);
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    std::string summary = reader.getElementValue();
    reader.next();
    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    g->setSummary(summary);
    reader.next();
  }

  if(reader.getElementLocalName() == "StandardDeviation")
  {
    assert(reader.getNodeType() == te::xml::START_ELEMENT);
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    double stdDeviation = reader.getElementValueAsDouble();
    reader.next();
    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    
    g->setStdDeviation(stdDeviation);

    reader.next();
  }

  /* Grouping Items */
  std::vector<te::map::GroupingItem*> items;
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
        reader.getElementLocalName() == "GroupingItem")
  {
    items.push_back(ReadGroupingItem(reader));
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  g->setGroupingItems(items);

  return g.release();
}

te::map::GroupingItem* te::map::serialize::ReadGroupingItem(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "GroupingItem");

  std::auto_ptr<te::map::GroupingItem> item(new te::map::GroupingItem);

  reader.next();

  /* Title */
  assert(reader.getElementLocalName() == "Title");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string title = reader.getElementValue();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  item->setTitle(title);

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);

  if(reader.getElementLocalName() == "From")
  {
    /* From */
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    std::string fromValue = reader.getElementValue();
    reader.next();
    assert(reader.getNodeType() == te::xml::END_ELEMENT);

    item->setLowerLimit(fromValue);

    reader.next();

    /* To */
    assert(reader.getNodeType() == te::xml::START_ELEMENT);
    assert(reader.getElementLocalName() == "To");
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    std::string toValue = reader.getElementValue();
    reader.next();
    assert(reader.getNodeType() == te::xml::END_ELEMENT);

    item->setUpperLimit(toValue);

    reader.next();
  }
  else
  {
    /* Value */
    assert(reader.getElementLocalName() == "Value");
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    std::string value = reader.getElementValue();
    reader.next();
    assert(reader.getNodeType() == te::xml::END_ELEMENT);

    item->setValue(value);

    reader.next();
  }

  /* Symbolizers */
  std::vector<te::se::Symbolizer*> symbs;
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
        reader.getElementLocalName().find("Symbolizer") != std::string::npos)
  {
    symbs.push_back(te::se::serialize::Symbolizer::getInstance().read(reader));
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  item->setSymbolizers(symbs);

  return item.release();
}

std::auto_ptr<te::map::Chart> te::map::serialize::ReadLayerChart(te::xml::Reader& reader)
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
  assert(reader.getElementLocalName() == "ContourColor");

  reader.next();

  assert(reader.getNodeType() == te::xml::VALUE);
  
  std::string hexColor = reader.getElementValue();

  te::color::RGBAColor contourColor(hexColor);

  reader.next();

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

  double maxValue = 0;
  if(type == te::map::Bar)
  {
    assert(reader.getNodeType() == te::xml::START_ELEMENT);
    assert(reader.getElementLocalName() == "MaxValue");

    reader.next();

    assert(reader.getNodeType() == te::xml::VALUE);

    maxValue = reader.getElementValueAsDouble();

    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT); // MaxValue

    reader.next();
  }

  std::string summary  = "NONE";
  if(reader.getNodeType() == te::xml::START_ELEMENT &&
     reader.getElementLocalName() == "Summary")
  {
    reader.next();

    assert(reader.getNodeType() == te::xml::VALUE);

    summary = reader.getElementValue();

    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT); // Summary

    reader.next();
  }

  std::vector<std::string> properties;
  std::vector<te::color::RGBAColor> colors;

  while(reader.getNodeType() == te::xml::START_ELEMENT &&
        reader.getElementLocalName() == "Slice")
  {
    reader.next();

    assert(reader.getNodeType() == te::xml::START_ELEMENT);
    assert(reader.getElementLocalName() == "PropertyName");

    reader.next();

    assert(reader.getNodeType() == te::xml::VALUE);

    std::string propName = reader.getElementValue();

    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT); // PropertyName

    reader.next();

    assert(reader.getNodeType() == te::xml::START_ELEMENT);
    assert(reader.getElementLocalName() == "Color");

    reader.next();

    assert(reader.getNodeType() == te::xml::VALUE);

    std::string hColor = reader.getElementValue();
    te::color::RGBAColor color(hColor);

    reader.next();
    assert(reader.getNodeType() == te::xml::END_ELEMENT); // Color

    reader.next();
    assert(reader.getNodeType() == te::xml::END_ELEMENT); // Slice

    reader.next();

    properties.push_back(propName);
    colors.push_back(hColor);
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // Chart

  reader.next();

  std::auto_ptr<te::map::Chart> chart(new te::map::Chart(type, properties, colors));
  chart->setContourColor(contourColor);
  chart->setContourWidth(contourWidth);
  chart->setHeight(height);
  chart->setVisibility(isVisible);
  chart->setSummary(summary);
  if(barWidth != -1)
    chart->setBarWidth(barWidth);
  if(type == te::map::Bar)
    chart->setMaxValue(maxValue);

  return chart;
}

void te::map::serialize::WriteLayerChart(te::map::Chart* chart, te::xml::AbstractWriter& writer)
{
  writer.writeStartElement("te_map:Chart");

  te::map::ChartType type = chart->getType();

  writer.writeElement("te_map:Type", (type == te::map::Pie ? "PIE" : "BAR"));

  writer.writeElement("te_map:ContourColor", chart->getContourColor().getColor());

  writer.writeElement("te_map:ContourWidth", boost::lexical_cast<int>(chart->getContourWidth()));

  writer.writeElement("te_map:Height", boost::lexical_cast<int>(chart->getHeight()));

  if(chart->getType() == te::map::Bar)
    writer.writeElement("te_map:BarWidth", boost::lexical_cast<int>(chart->getBarWidth()));

  writer.writeElement("te_map:IsVisible", (chart->isVisible() ? "true" : "false"));

  if(chart->getType() == te::map::Bar)
    writer.writeElement("te_map:MaxValue", chart->getMaxValue());

  if(chart->getSummary() != "NONE")
    writer.writeElement("te_map:Summary", chart->getSummary());

  std::vector<std::string> properties = chart->getProperties();

  for(std::size_t i = 0; i < properties.size(); ++i)
  {
    writer.writeStartElement("te_map:Slice");

    writer.writeElement("te_map:PropertyName", properties[i]);

    writer.writeElement("te_map:Color", chart->getColor(i).getColor());

    writer.writeEndElement("te_map:Slice");
  }

  writer.writeEndElement("te_map:Chart");
}

void te::map::serialize::WriteLayerGrouping(te::map::Grouping* g, te::xml::AbstractWriter& writer)
{
  writer.writeStartElement("te_map:Grouping");

  te::map::GroupingType type = g->getType();

  writer.writeElement("te_map:PropertyName", g->getPropertyName());
  writer.writeElement("te_map:PropertyDataType", g->getPropertyType());
  writer.writeElement("te_map:Type", GetGroupingType(type));
  writer.writeElement("te_map:Precision", static_cast<unsigned int>(g->getPrecision()));

  if(g->getSummary() != "NONE")
    writer.writeElement("te_map:Summary", g->getSummary());

  if(type == te::map::STD_DEVIATION)
    writer.writeElement("te_map:StandardDeviation", g->getStdDeviation());

  const std::vector<te::map::GroupingItem*>& items = g->getGroupingItems();

  for(std::size_t i = 0; i < items.size(); ++i)
  {
    te::map::GroupingItem* item = items[i];
    assert(item);

    writer.writeStartElement("te_map:GroupingItem");

    writer.writeElement("te_map:Title", item->getTitle());

    if(type != te::map::UNIQUE_VALUE)
    {
      writer.writeElement("te_map:From", item->getLowerLimit());
      writer.writeElement("te_map:To", item->getUpperLimit());
    }
    else
      writer.writeElement("te_map:Value", item->getValue());

    const std::vector<te::se::Symbolizer*>& symbs = item->getSymbolizers();

    for(std::size_t j = 0; j < symbs.size(); ++j)
      te::se::serialize::Symbolizer::getInstance().write(symbs[j], writer);

    writer.writeEndElement("te_map:GroupingItem");
  }

  writer.writeEndElement("te_map:Grouping");
}

void te::map::serialize::WriteAbstractLayer(const te::map::AbstractLayer* layer, te::xml::AbstractWriter& writer)
{
  writer.writeAttribute("id", layer->getId());
  writer.writeElement("te_map:Title", layer->getTitle());
  writer.writeElement("te_map:Visible", GetVisibility(layer->getVisibility()));
  writer.writeElement("te_map:DataSetName", layer->getDataSetName());
  writer.writeElement("te_map:DataSourceId", layer->getDataSourceId());

  te::map::Grouping* g = layer->getGrouping();

  if(g != 0)
    WriteLayerGrouping(g, writer);

  te::map::Chart* chart = layer->getChart();

  if(chart != 0)
    WriteLayerChart(chart, writer);
}
