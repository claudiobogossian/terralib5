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
  \file src/terralib/maptools/serialization/xml/Layer.cpp
   
  \brief Auxiliary classes and functions to read layer information from a XML document.
*/

// TerraLib
#include "../../../common/BoostUtils.h"
#include "../../../common/Translator.h"
#include "../../../color/RGBAColor.h"
#include "../../../dataaccess/dataset/AttributeConverterManager.h"
#include "../../../dataaccess/dataset/DataSetTypeConverter.h"
#include "../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../dataaccess/serialization/xml/Serializer.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../datatype/DateTimeProperty.h"
#include "../../../datatype/NumericProperty.h"
#include "../../../datatype/Property.h"
#include "../../../datatype/SimpleProperty.h"
#include "../../../datatype/StringProperty.h"
#include "../../../datatype/serialization/xml/Serializer.h"
#include "../../../geometry/Envelope.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../geometry/serialization/xml/Serializer.h"
#include "../../../se/CoverageStyle.h"
#include "../../../xml/Reader.h"
#include "../../../xml/Writer.h"
#include "../../../maptools/AbstractLayer.h"
#include "../../../maptools/Chart.h"
#include "../../../maptools/DataSetAdapterLayer.h"
#include "../../../maptools/DataSetLayer.h"
#include "../../../maptools/FolderLayer.h"
#include "../../../maptools/Grouping.h"
#include "../../../maptools/GroupingItem.h"
#include "../../../maptools/QueryLayer.h"
#include "../../../maptools/RasterLayer.h"
#include "../../../se/RasterSymbolizer.h"
#include "../../../se/serialization/xml/Style.h"
#include "../../../se/serialization/xml/Symbolizer.h"
#include "../../Exception.h"
#include "Layer.h"

// STL
#include <cassert>
#include <memory>

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

te::map::AbstractLayer* DataSetLayerReader(te::xml::Reader& reader);
te::map::AbstractLayer* QueryLayerReader(te::xml::Reader& reader);
te::map::AbstractLayer* FolderLayerReader(te::xml::Reader& reader);
te::map::AbstractLayer* RasterLayerReader(te::xml::Reader& reader);
te::map::AbstractLayer* DataSetAdapterLayerReader(te::xml::Reader& reader);
void DataSetLayerWriter(const te::map::AbstractLayer* layer, te::xml::Writer& writer);
void QueryLayerWriter(const te::map::AbstractLayer* layer, te::xml::Writer& writer);
void FolderLayerWriter(const te::map::AbstractLayer* layer, te::xml::Writer& writer);
void RasterLayerWriter(const te::map::AbstractLayer* layer, te::xml::Writer& writer);
void DataSetAdapterLayerWriter(const te::map::AbstractLayer* layer, te::xml::Writer& writer);

te::map::Visibility GetVisibility(const std::string& visible)
{
  return (visible == "VISIBLE") ? te::map::VISIBLE :
         (visible == "NOT_VISIBLE") ? te::map::NOT_VISIBLE :
         te::map::PARTIALLY_VISIBLE;
}

std::string GetVisibility(const te::map::Visibility& visible)
{
  return (visible == te::map::VISIBLE) ? "VISIBLE" :
         (visible == te::map::NOT_VISIBLE) ? "NOT_VISIBLE" :
         "PARTIALLY_VISIBLE";
}

te::map::GroupingType GetGroupingType(const std::string& type)
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

 std::string GetGroupingType(const te::map::GroupingType& type)
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

te::dt::SimpleProperty* GetProperty(std::string name, int dataType, int geomType, int srid)
{
   te::dt::SimpleProperty* simpleProperty = 0;

  switch(dataType)
  {
    case te::dt::BOOLEAN_TYPE:
    case te::dt::CHAR_TYPE:
    case te::dt::DOUBLE_TYPE:
    case te::dt::FLOAT_TYPE:
    case te::dt::INT16_TYPE:
    case te::dt::INT32_TYPE:
    case te::dt::INT64_TYPE:
    case te::dt::UCHAR_TYPE:
    case te::dt::UINT16_TYPE:
    case te::dt::UINT32_TYPE:
    case te::dt::UINT64_TYPE:
    {
      simpleProperty = new te::dt::SimpleProperty(name, dataType);
      break;
    }

    case te::dt::STRING_TYPE:
    {
      simpleProperty = new te::dt::StringProperty(name);
      break;
    }

    case te::dt::NUMERIC_TYPE:
    {
      simpleProperty = new te::dt::NumericProperty(name, 0, 0);
      break;
    }

    case te::dt::DATETIME_TYPE:
    {
      simpleProperty = new te::dt::DateTimeProperty(name);
      break;
    }
        
    case te::dt::GEOMETRY_TYPE:
    {
      simpleProperty = new te::gm::GeometryProperty(name, srid, (te::gm::GeomType)geomType);
      break;
    }

    default:
    {
      simpleProperty = 0;
      return false;
    }
  }

  return simpleProperty;
}


std::string ReadLayerTitle(te::xml::Reader& reader)
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

std::string ReadLayerVisibility(te::xml::Reader& reader)
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

te::map::GroupingItem* ReadGroupingItem(te::xml::Reader& reader)
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

te::map::Grouping* ReadLayerGrouping(te::xml::Reader& reader)
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

  reader.next();
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

std::auto_ptr<te::map::Chart> ReadLayerChart(te::xml::Reader& reader)
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
  if(barWidth != -1)
    chart->setBarWidth(barWidth);
  if(type == te::map::Bar)
    chart->setMaxValue(maxValue);

  return chart;
}

void WriteLayerChart(te::map::Chart* chart, te::xml::Writer& writer)
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

void WriteLayerGrouping(te::map::Grouping* g, te::xml::Writer& writer)
{
  writer.writeStartElement("te_map:Grouping");

  te::map::GroupingType type = g->getType();

  writer.writeElement("te_map:PropertyName", g->getPropertyName());
  writer.writeElement("te_map:PropertyDataType", g->getPropertyType());
  writer.writeElement("te_map:Type", GetGroupingType(type));
  writer.writeElement("te_map:Precision", static_cast<unsigned int>(g->getPrecision()));
  
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

void WriteAbstractLayer(const te::map::AbstractLayer* layer, te::xml::Writer& writer)
{
  writer.writeAttribute("id", layer->getId());
  writer.writeElement("te_map:Title", layer->getTitle());
  writer.writeElement("te_map:Visible", GetVisibility(layer->getVisibility()));

  te::map::Grouping* g = layer->getGrouping();

  if(g != 0)
    WriteLayerGrouping(g, writer);

  te::map::Chart* chart = layer->getChart();

  if(chart != 0)
    WriteLayerChart(chart, writer);
}

void WriteOGRAbstractLayer(const te::map::AbstractLayer* layer, te::xml::Writer& writer)
{
  writer.writeAttribute("id", layer->getId());
  writer.writeElement("te_map:Title", te::common::ConvertLatin1UTFString(layer->getTitle()));
  writer.writeElement("te_map:Visible", GetVisibility(layer->getVisibility()));

  te::map::Grouping* g = layer->getGrouping();

  if(g != 0)
    WriteLayerGrouping(g, writer);

  te::map::Chart* chart = layer->getChart();

  if(chart != 0)
    WriteLayerChart(chart, writer);
}

void te::map::serialize::Layer::reg(const std::string& layerType, const LayerFnctSerializeType& fncts)
{
  m_fncts[layerType] = fncts;
}

te::map::AbstractLayer* te::map::serialize::Layer::read(te::xml::Reader& reader) const
{
  std::string layerType = boost::to_upper_copy(reader.getElementLocalName());

  LayerFnctIdxType::const_iterator it = m_fncts.find(layerType);

  if(it == m_fncts.end())
    throw Exception((boost::format(TE_TR("Could not find a reader for the following layer type: %1%.")) % layerType).str());

  assert(it->second.second);

  return it->second.first(reader);
}

void te::map::serialize::Layer::write(const te::map::AbstractLayer* alayer, te::xml::Writer& writer) const
{
  assert(alayer);

  LayerFnctIdxType::const_iterator it = m_fncts.find(alayer->getType());

  if(it == m_fncts.end())
    throw Exception((boost::format(TE_TR("Could not find a writer for the following layer type: %1%.")) % alayer->getType()).str());

  assert(it->second.second);

  return it->second.second(alayer, writer);
}

te::map::serialize::Layer::~Layer()
{
}

te::map::serialize::Layer::Layer()
{
  m_fncts["DATASETLAYER"] = std::make_pair(LayerReadFnctType(&DataSetLayerReader), LayerWriteFnctType(&DataSetLayerWriter));
  m_fncts["QUERYLAYER"] = std::make_pair(LayerReadFnctType(&QueryLayerReader), LayerWriteFnctType(&QueryLayerWriter));
  m_fncts["FOLDERLAYER"] = std::make_pair(LayerReadFnctType(&FolderLayerReader), LayerWriteFnctType(&FolderLayerWriter));
  m_fncts["RASTERLAYER"] = std::make_pair(LayerReadFnctType(&RasterLayerReader), LayerWriteFnctType(&RasterLayerWriter));
  m_fncts["DATASETADAPTERLAYER"] = std::make_pair(LayerReadFnctType(&DataSetAdapterLayerReader), LayerWriteFnctType(&DataSetAdapterLayerWriter));
}

te::map::AbstractLayer* DataSetLayerReader(te::xml::Reader& reader)
{
  std::string id = reader.getAttr(0);

  /* Title Element */
  reader.next();
  std::string title = ReadLayerTitle(reader);

  /* Visible Element */
  reader.next();
  std::string visible = ReadLayerVisibility(reader);

  reader.next();

  /* Grouping */
  te::map::Grouping* grouping = ReadLayerGrouping(reader);

  /* Chart */
  std::auto_ptr<te::map::Chart> chart(ReadLayerChart(reader));

  /* DataSetName Element */
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "DataSetName");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string dataset = reader.getElementValue();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  /* DataSourceId Element */
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "DataSourceId");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string datasourceId = reader.getElementValue();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  /* SRID Element */
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "SRID");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  int srid = reader.getElementValueAsInt32();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  /* Extent Element */
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Extent");
  std::auto_ptr<te::gm::Envelope> mbr(te::serialize::xml::ReadExtent(reader));

  /* RendererId Element */
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "RendererId");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string rendererId = reader.getElementValue();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  /* has a Style Element ? */
  reader.next();

  std::auto_ptr<te::se::Style> style;

  if((reader.getNodeType() == te::xml::START_ELEMENT) &&
     (reader.getElementLocalName() == "Style"))
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::START_ELEMENT);

    style.reset(te::se::serialize::Style::getInstance().read(reader));

    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    assert(reader.getElementLocalName() == "Style");

    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  assert(reader.getElementLocalName() == "DataSetLayer");

  reader.next();

  te::da::DataSourcePtr dsc = te::da::GetDataSource(datasourceId);
  if (dsc.get())
  {
    std::auto_ptr<te::map::DataSetLayer> layer(new te::map::DataSetLayer(id, title, 0));
    layer->setSRID(srid);
    layer->setExtent(*mbr.get());
    layer->setVisibility(GetVisibility(visible));
    layer->setDataSetName(dataset);
    layer->setDataSourceId(datasourceId);
    layer->setRendererType(rendererId);
    layer->setStyle(style.release());
    
    if(grouping)
      layer->setGrouping(grouping);
    
    if(chart.get())
      layer->setChart(chart.release());
    
    return layer.release();
  }
  return 0;
}

te::map::AbstractLayer* QueryLayerReader(te::xml::Reader& reader)
{
  std::string id = reader.getAttr(0);

 /* Title Element */
  reader.next();
  std::string title = ReadLayerTitle(reader);

  /* Visible Element */
  reader.next();
  std::string visible = ReadLayerVisibility(reader);
  reader.next();

  /* Grouping */
  te::map::Grouping* grouping = ReadLayerGrouping(reader);

  /* Query Element */
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Select");
  te::da::Select* query = te::serialize::xml::ReadSelect(reader);

  /* DataSourceId Element */
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "DataSourceId");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string datasourceId = reader.getElementValue();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  /* SRID Element */
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "SRID");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  int srid = reader.getElementValueAsInt32();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  /* Extent Element */
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Extent");
  std::auto_ptr<te::gm::Envelope> mbr(te::serialize::xml::ReadExtent(reader));

  /* RendererId Element */
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "RendererId");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string rendererId = reader.getElementValue();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

    /* has a Style Element ? */
  reader.next();

  std::auto_ptr<te::se::Style> style;

  if((reader.getNodeType() == te::xml::START_ELEMENT) &&
     (reader.getElementLocalName() == "Style"))
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::START_ELEMENT);

    style.reset(te::se::serialize::Style::getInstance().read(reader));

    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    assert(reader.getElementLocalName() == "Style");

    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  assert(reader.getElementLocalName() == "QueryLayer");

  reader.next();

  std::auto_ptr<te::map::QueryLayer> layer(new te::map::QueryLayer(id, title, 0));
  layer->setSRID(srid);
  layer->setExtent(*mbr.get());
  layer->setVisibility(GetVisibility(visible));
  layer->setQuery(query);
  layer->setDataSourceId(datasourceId);
  layer->setRendererType(rendererId);
  layer->setStyle(style.release());

  if(grouping)
    layer->setGrouping(grouping);

  return layer.release();
}

te::map::AbstractLayer* FolderLayerReader(te::xml::Reader& reader)
{
  std::string id = reader.getAttr(0);

  /* Title Element */
  reader.next();
  std::string title = ReadLayerTitle(reader);

  /* Visible Element */
  reader.next();
  std::string visible = ReadLayerVisibility(reader);

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "LayerList");

  reader.next();

  std::auto_ptr<te::map::FolderLayer> flayer(new te::map::FolderLayer(id, title, 0));

  flayer->setVisibility(GetVisibility(visible));

  const te::map::serialize::Layer& lserial = te::map::serialize::Layer::getInstance();

  while((reader.getNodeType() != te::xml::END_ELEMENT) &&
        (reader.getElementLocalName() != "LayerList"))
  {
    te::map::AbstractLayerPtr layer(lserial.read(reader));

    if(layer.get() == 0)
      break;

    flayer->add(layer);
  }

  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  assert(reader.getElementLocalName() == "FolderLayer");

  reader.next();

  return flayer.release();
}

te::map::AbstractLayer* RasterLayerReader(te::xml::Reader& reader)
{
  std::string id = reader.getAttr(0);

  /* Title Element */
  reader.next();
  std::string title = ReadLayerTitle(reader);

  /* Visible Element */
  reader.next();
  std::string visible = ReadLayerVisibility(reader);

  /* ConnectionInfo Element */
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "ConnectionInfo");

  std::map<std::string, std::string> conninfo;

  while(reader.next() &&
        (reader.getNodeType() == te::xml::START_ELEMENT) &&
        (reader.getElementLocalName() == "Parameter"))
  {
    // Parameter Name
    reader.next();
    assert(reader.getNodeType() == te::xml::START_ELEMENT);
    assert(reader.getElementLocalName() == "Name");
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    std::string paramName = reader.getElementValue();
    reader.next();
    assert(reader.getNodeType() == te::xml::END_ELEMENT);

    // Parameter Value
    reader.next();
    assert(reader.getNodeType() == te::xml::START_ELEMENT);
    assert(reader.getElementLocalName() == "Value");
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    std::string paramValue = reader.getElementValue();
    reader.next();
    assert(reader.getNodeType() == te::xml::END_ELEMENT);

    conninfo[paramName] = paramValue;

    reader.next();
    assert(reader.getNodeType() == te::xml::END_ELEMENT);
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // End of ConnectionInfo Element

  /* SRID Element */
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "SRID");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  int srid = reader.getElementValueAsInt32();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  /* Extent Element */
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Extent");
  std::auto_ptr<te::gm::Envelope> mbr(te::serialize::xml::ReadExtent(reader));

  /* RendererId Element */
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "RendererId");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string rendererId = reader.getElementValue();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  /* has a Style Element ? */
  reader.next();

  std::auto_ptr<te::se::Style> style;

  if((reader.getNodeType() == te::xml::START_ELEMENT) &&
     (reader.getElementLocalName() == "Style"))
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::START_ELEMENT);

    style.reset(te::se::serialize::Style::getInstance().read(reader));

    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    assert(reader.getElementLocalName() == "Style");

    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  assert(reader.getElementLocalName() == "RasterLayer");

  reader.next();

  std::auto_ptr<te::map::RasterLayer> layer(new te::map::RasterLayer(id, title, 0));
  layer->setSRID(srid);
  layer->setExtent(*mbr.get());
  layer->setRasterInfo(conninfo);
  layer->setVisibility(GetVisibility(visible));
  layer->setRendererType(rendererId);
  layer->setStyle(dynamic_cast<te::se::CoverageStyle*>(style.release()));

  return layer.release();
}

te::map::AbstractLayer* DataSetAdapterLayerReader(te::xml::Reader& reader)
{
  std::string id = reader.getAttr(0);

  /* Title Element */
  reader.next();
  std::string title = ReadLayerTitle(reader);

  /* Visible Element */
  reader.next();
  std::string visible = ReadLayerVisibility(reader);

  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "DataSetName");

  reader.next();

  assert(reader.getNodeType() == te::xml::VALUE);

  std::string dataSetName = reader.getElementValue();

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT);  // DataSetName

  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "DataSourceId");

  reader.next();

  assert(reader.getNodeType() == te::xml::VALUE);

  std::string dataSourceId = reader.getElementValue();

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // DataSourceId

  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "RendererType");

  reader.next();

  assert(reader.getNodeType() == te::xml::VALUE);

  std::string rendererType = reader.getElementValue();

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT); //RendererType

  /* has a Style Element ? */
  reader.next();

  std::auto_ptr<te::se::Style> style;

  if((reader.getNodeType() == te::xml::START_ELEMENT) &&
     (reader.getElementLocalName() == "Style"))
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::START_ELEMENT);

    style.reset(te::se::serialize::Style::getInstance().read(reader));

    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    assert(reader.getElementLocalName() == "Style");

    reader.next();
  }

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Converter");

  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "InDataSetTypeName");

  reader.next();

  std::string inDataSetName = reader.getElementValue();

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // InDataSetTypeName

  reader.next();

  std::vector<std::pair<std::string, std::vector<std::size_t> > > props;

  while(reader.getNodeType() == te::xml::START_ELEMENT &&
        reader.getElementLocalName() == "OutPropInfo")
  {
    reader.next();

    assert(reader.getNodeType() == te::xml::START_ELEMENT);
    assert(reader.getElementLocalName() == "type");

    reader.next();

    assert(reader.getNodeType() == te::xml::VALUE);

    int type = reader.getElementValueAsInt32();

    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT); // type

    reader.next();

    assert(reader.getNodeType() == te::xml::START_ELEMENT);
    assert(reader.getElementLocalName() == "name");

    reader.next();

    assert(reader.getNodeType() == te::xml::VALUE);

    std::string name = reader.getElementValue();

    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT); // name

    reader.next();

    int geomType;
    int srid;

    assert(reader.getNodeType() == te::xml::START_ELEMENT);
    assert(reader.getElementLocalName() == "geomType");

    reader.next();

    assert(reader.getNodeType() == te::xml::VALUE);

    geomType = reader.getElementValueAsInt32();

    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT); // geomType

    reader.next();

    assert(reader.getNodeType() == te::xml::START_ELEMENT);
    assert(reader.getElementLocalName() == "srid");

    reader.next();

    assert(reader.getNodeType() == te::xml::VALUE);

    srid = reader.getElementValueAsInt32();

    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT); // srid

    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT); // OutPropInfo

    reader.next();

    std::pair<std::string, std::vector<std::size_t> > prop;
    prop.first = name;
    std::vector<std::size_t> typeVec;
    typeVec.push_back(type);
    typeVec.push_back(geomType);
    typeVec.push_back(srid);
    prop.second = typeVec;
    props.push_back(prop);
  }

  std::vector<std::vector<std::size_t> > propertyIndexes;

  while(reader.getNodeType() == te::xml::START_ELEMENT &&
        reader.getElementLocalName() == "PropertyIndex")
  {

    int outIdx = reader.getAttrAsInt32("OutIdx");

    reader.next();

    std::vector<std::size_t> inIdxs;

    while(reader.getNodeType() == te::xml::START_ELEMENT &&
          reader.getElementLocalName() == "InIdx")
    {
      reader.next();

      assert(reader.getNodeType() == te::xml::VALUE);

      inIdxs.push_back(reader.getElementValueAsInt32());

      reader.next();

      assert(reader.getNodeType() == te::xml::END_ELEMENT); // InIdx

      reader.next();
    }

    assert(reader.getNodeType() == te::xml::END_ELEMENT); // InIdx

    propertyIndexes.push_back(inIdxs);

    reader.next();
  }

  std::vector<std::string> functionsNames;

  int teste = 0;
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
        reader.getElementLocalName() == "FunctionName")
  {
    ++teste;

    reader.next();

    assert(reader.getNodeType() == te::xml::VALUE);

    functionsNames.push_back(reader.getElementValue());

    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT); // FunctionName

    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // Converter

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // DataSetAdapterLayer

  reader.next();

  te::map::DataSetAdapterLayer* result = new te::map::DataSetAdapterLayer(id);
  result->setTitle(title);

  result->setVisibility(GetVisibility(visible));
  result->setDataSetName(dataSetName);
  result->setDataSourceId(dataSourceId);
  result->setRendererType(rendererType);
  result->setStyle(style.release());
  
  te::da::DataSourcePtr ds = te::da::GetDataSource(dataSourceId);
  std::auto_ptr<te::da::DataSetType> dst = ds->getDataSetType(dataSetName);

  std::auto_ptr<te::da::DataSetTypeConverter> converter(new te::da::DataSetTypeConverter(dst.get()));
  
  for(std::size_t i = 0; i < propertyIndexes.size(); ++i)
  {
    converter->add(propertyIndexes[i], GetProperty(props[i].first, props[i].second[0], props[i].second[1], props[i].second[2]), functionsNames[i]);
  }

  std::vector<te::dt::Property*> convertedProps = converter->getResult()->getProperties();

  te::da::PrimaryKey* pkIn = dst->getPrimaryKey();
  if(pkIn)
  {
    te::da::PrimaryKey* pk = new te::da::PrimaryKey(converter->getResult());

    std::vector<te::dt::Property*> pkProps = pkIn->getProperties();
    for(std::size_t i = 0; i < convertedProps.size(); ++i)
    {
      for(std::size_t t = 0; t < pkProps.size(); ++t)
      {
        if(convertedProps[i]->getName() == pkProps[t]->getName())
          pk->add(convertedProps[i]);
          
      }
    }
  }

  result->setConverter(converter);

  te::gm::Envelope* env = new te::gm::Envelope;

  std::auto_ptr<te::da::DataSet> dataset = result->getData();

  te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(result->getConverter()->getResult());

  if(gp)
  {
    while(dataset->moveNext())
    {
      te::gm::Geometry* geom = dataset->getGeometry(gp->getName()).release();
      assert(geom);
      env->Union(*geom->getMBR());
    }
  }

  result->setExtent(*env);

  return result;
}

void DataSetLayerWriter(const te::map::AbstractLayer* alayer, te::xml::Writer& writer)
{
  const te::map::DataSetLayer* layer = dynamic_cast<const te::map::DataSetLayer*>(alayer);

  if(layer == 0)
    return;

  writer.writeStartElement("te_map:DataSetLayer");

  bool ogrType = te::da::GetDataSource(layer->getDataSourceId())->getType() == "OGR";

  if(ogrType)
    WriteOGRAbstractLayer(layer, writer);
  else
    WriteAbstractLayer(layer, writer);

  writer.writeElement("te_map:DataSetName", (ogrType) ? te::common::ConvertLatin1UTFString(layer->getDataSetName()) : layer->getDataSetName());
  writer.writeElement("te_map:DataSourceId", layer->getDataSourceId());
  writer.writeElement("te_map:SRID", layer->getSRID());
  te::serialize::xml::SaveExtent(layer->getExtent(), writer);
  writer.writeElement("te_map:RendererId", layer->getRendererType());

  if(layer->getStyle())
  {
    writer.writeStartElement("te_map:Style");

    te::se::serialize::Style::getInstance().write(layer->getStyle(), writer);

    writer.writeEndElement("te_map:Style");
  }

  writer.writeEndElement("te_map:DataSetLayer");
}

void QueryLayerWriter(const te::map::AbstractLayer* alayer, te::xml::Writer& writer)
{
  const te::map::QueryLayer* layer = dynamic_cast<const te::map::QueryLayer*>(alayer);

  if(layer == 0)
    return;

  writer.writeStartElement("te_map:QueryLayer");

  WriteAbstractLayer(layer, writer);

  te::serialize::xml::Save(layer->getQuery(), writer);
  writer.writeElement("te_map:DataSourceId", layer->getDataSourceId());
  writer.writeElement("te_map:SRID", layer->getSRID());
  te::serialize::xml::SaveExtent(layer->getExtent(), writer);
  writer.writeElement("te_map:RendererId", layer->getRendererType());

  if(layer->getStyle())
  {
    writer.writeStartElement("te_map:Style");

    te::se::serialize::Style::getInstance().write(layer->getStyle(), writer);

    writer.writeEndElement("te_map:Style");
  }

  writer.writeEndElement("te_map:QueryLayer");
}

void FolderLayerWriter(const te::map::AbstractLayer* alayer, te::xml::Writer& writer)
{
  const te::map::FolderLayer* folderLayer = static_cast<const te::map::FolderLayer*>(alayer);

  writer.writeStartElement("te_map:FolderLayer");

  writer.writeAttribute("id", folderLayer->getId());
  writer.writeElement("te_map:Title", folderLayer->getTitle());
  writer.writeElement("te_map:Visible", GetVisibility(folderLayer->getVisibility()));

  writer.writeStartElement("te_map:LayerList");
 
  size_t count = folderLayer->getChildrenCount();

  for(size_t i=0; i<count; i++)
    te::map::serialize::Layer::getInstance().write((const te::map::AbstractLayer*) folderLayer->getChild(i).get(), writer);

  writer.writeEndElement("te_map:LayerList");

  writer.writeEndElement("te_map:FolderLayer");
}

void RasterLayerWriter(const te::map::AbstractLayer* alayer, te::xml::Writer& writer)
{
  const te::map::RasterLayer* layer = dynamic_cast<const te::map::RasterLayer*>(alayer);

  if(layer == 0)
    return;

  writer.writeStartElement("te_map:RasterLayer");

  WriteAbstractLayer(layer, writer);

  writer.writeStartElement("te_map:ConnectionInfo");
  std::map<std::string, std::string> info = layer->getRasterInfo();
  std::map<std::string, std::string>::iterator conIt;

  for(conIt=info.begin(); conIt!=info.end(); ++conIt)
  {
    writer.writeStartElement("te_map:Parameter");

    writer.writeStartElement("te_map:Name");
    writer.writeValue(conIt->first);
    writer.writeEndElement("te_map:Name");

    writer.writeStartElement("te_map:Value");
    writer.writeValue(conIt->second);
    writer.writeEndElement("te_map:Value");

    writer.writeEndElement("te_map:Parameter");
  }
  writer.writeEndElement("te_map:ConnectionInfo");

  writer.writeElement("te_map:SRID", layer->getSRID());
  te::serialize::xml::SaveExtent(layer->getExtent(), writer);
  writer.writeElement("te_map:RendererId", layer->getRendererType());

  if(layer->getStyle())
  {
    writer.writeStartElement("te_map:Style");

    te::se::serialize::Style::getInstance().write(layer->getStyle(), writer);

    writer.writeEndElement("te_map:Style");
  }

  writer.writeEndElement("te_map:RasterLayer");
}

void DataSetAdapterLayerWriter(const te::map::AbstractLayer* alayer, te::xml::Writer& writer)
{
  const te::map::DataSetAdapterLayer* layer = dynamic_cast<const te::map::DataSetAdapterLayer*>(alayer);

  if(layer == 0)
    return;

  writer.writeStartElement("te_map:DataSetAdapterLayer");

  WriteAbstractLayer(layer, writer);
  
  writer.writeElement("te_map:DataSetName", layer->getDataSetName());
  writer.writeElement("te_map:DataSourceId", layer->getDataSourceId());
  writer.writeElement("te_map:RendererType", layer->getRendererType());

  if(layer->getStyle())
  {
    writer.writeStartElement("te_map:Style");

    te::se::serialize::Style::getInstance().write(layer->getStyle(), writer);

    writer.writeEndElement("te_map:Style");
  }

  te::da::DataSetTypeConverter* converter = layer->getConverter();

  writer.writeStartElement("te_map:Converter");

  writer.writeElement("te_map:InDataSetTypeName", converter->getConvertee()->getName());

  te::da::DataSetType* resultDt = converter->getResult();

  std::vector<te::dt::Property*> outProps = resultDt->getProperties();

  for(std::size_t i = 0; i < outProps.size(); ++i)
  {
    writer.writeStartElement("te_map:OutPropInfo");

    writer.writeElement("te_map:type", outProps[i]->getType());
    writer.writeElement("te_map:name", outProps[i]->getName());

    if(outProps[i]->getType() == te::dt::GEOMETRY_TYPE)
    {
      te::gm::GeometryProperty* geomProp = dynamic_cast<te::gm::GeometryProperty*>(outProps[i]);

      writer.writeElement("te_map:geomType", geomProp->getGeometryType());
      writer.writeElement("te_map:srid", geomProp->getSRID());
    }
    else
    {
      writer.writeElement("te_map:geomType", -1);
      writer.writeElement("te_map:srid", 0);
    }

    writer.writeEndElement("te_map:OutPropInfo");
    
  }

  // PropertyIndexes
  std::vector<std::vector<std::size_t> > propertyIndexes = converter->getConvertedPropertyIndexes();

  for(std::size_t i = 0; i < propertyIndexes.size(); ++i)
  {
    writer.writeStartElement("te_map:PropertyIndex");

    writer.writeAttribute("OutIdx", (int)i);

    std::vector<std::size_t> inputPropertiesIdx = propertyIndexes[i];

    for(std::size_t j = 0; j < inputPropertiesIdx.size(); ++j)
      writer.writeElement("te_map:InIdx", (int)inputPropertiesIdx[j]);

    writer.writeEndElement("te_map:PropertyIndex");
  }

  std::vector<te::da::AttributeConverter> converters = converter->getConverters();
  for(std::size_t i = 0; i < converters.size(); ++i)
  {
    std::string convName = layer->getConverter()->getConverterName(i);

    writer.writeElement("te_map:FunctionName", convName);
  }

  writer.writeEndElement("te_map:Converter");

  writer.writeEndElement("te_map:DataSetAdapterLayer");
}
