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
  \file Layer.cpp
   
  \brief Auxiliary classes and functions to read layer information from a XML document.
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../geometry/Envelope.h"
#include "../../se/CoverageStyle.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../maptools/AbstractLayer.h"
#include "../../maptools/DataSetLayer.h"
#include "../../maptools/FolderLayer.h"
#include "../../maptools/Grouping.h"
#include "../../maptools/GroupingItem.h"
#include "../../maptools/QueryLayer.h"
#include "../../maptools/RasterLayer.h"
#include "../../se/RasterSymbolizer.h"
#include "../geometry/Envelope.h"
#include "../se/Style.h"
#include "../se/Symbolizer.h"
#include "../Exception.h"
#include "Layer.h"

// STL
#include <cassert>
#include <memory>

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/format.hpp>

te::map::AbstractLayer* DataSetLayerReader(te::xml::Reader& reader);
te::map::AbstractLayer* QueryLayerReader(te::xml::Reader& reader);
te::map::AbstractLayer* FolderLayerReader(te::xml::Reader& reader);
te::map::AbstractLayer* RasterLayerReader(te::xml::Reader& reader);
void DataSetLayerWriter(const te::map::AbstractLayer* layer, te::xml::Writer& writer);
void QueryLayerWriter(const te::map::AbstractLayer* layer, te::xml::Writer& writer);
void FolderLayerWriter(const te::map::AbstractLayer* layer, te::xml::Writer& writer);
void RasterLayerWriter(const te::map::AbstractLayer* layer, te::xml::Writer& writer);

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
    symbs.push_back(te::serialize::Symbolizer::getInstance().read(reader));
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

void WriteAbstractLayer(const te::map::AbstractLayer* layer, te::xml::Writer& writer)
{
  writer.writeAttribute("id", layer->getId());
  writer.writeElement("te_map:Title", layer->getTitle());
  writer.writeElement("te_map:Visible", GetVisibility(layer->getVisibility()));

  te::map::Grouping* g = layer->getGrouping();

  if(g == 0)
    return;

  writer.writeStartElement("te_map:Grouping");

  te::map::GroupingType type = g->getType();

  writer.writeElement("te_map:PropertyName", g->getPropertyName());
  writer.writeElement("te_map:Type", GetGroupingType(type));
  writer.writeElement("te_map:Precision", g->getPrecision());
  
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

    std::vector<te::se::Symbolizer*> symbs = item->getSymbolizers();

    for(std::size_t j = 0; j < symbs.size(); ++j)
      te::serialize::Symbolizer::getInstance().write(symbs[j], writer);

    writer.writeEndElement("te_map:GroupingItem");
  }

  writer.writeEndElement("te_map:Grouping");
}

void te::serialize::Layer::reg(const std::string& layerType, const LayerFnctSerializeType& fncts)
{
  m_fncts[layerType] = fncts;
}

te::map::AbstractLayer* te::serialize::Layer::read(te::xml::Reader& reader) const
{
  std::string layerType = boost::to_upper_copy(reader.getElementLocalName());

  LayerFnctIdxType::const_iterator it = m_fncts.find(layerType);

  if(it == m_fncts.end())
    throw Exception((boost::format(TR_SERIALIZATION("Could not find a reader for the following layer type: %1%.")) % layerType).str());

  assert(it->second.second);

  return it->second.first(reader);
}

void te::serialize::Layer::write(const te::map::AbstractLayer* alayer, te::xml::Writer& writer) const
{
  assert(alayer);

  LayerFnctIdxType::const_iterator it = m_fncts.find(alayer->getType());

  if(it == m_fncts.end())
    throw Exception((boost::format(TR_SERIALIZATION("Could not find a writer for the following layer type: %1%.")) % alayer->getType()).str());

  assert(it->second.second);

  return it->second.second(alayer, writer);
}

te::serialize::Layer::~Layer()
{
}

te::serialize::Layer::Layer()
{
  m_fncts["DATASETLAYER"] = std::make_pair(LayerReadFnctType(&DataSetLayerReader), LayerWriteFnctType(&DataSetLayerWriter));
  m_fncts["QUERYLAYER"] = std::make_pair(LayerReadFnctType(&QueryLayerReader), LayerWriteFnctType(&QueryLayerWriter));
  m_fncts["FOLDERLAYER"] = std::make_pair(LayerReadFnctType(&FolderLayerReader), LayerWriteFnctType(&FolderLayerWriter));
  m_fncts["RASTERLAYER"] = std::make_pair(LayerReadFnctType(&RasterLayerReader), LayerWriteFnctType(&RasterLayerWriter));
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
  std::auto_ptr<te::gm::Envelope> mbr(te::serialize::ReadExtent(reader));

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

    style.reset(te::serialize::Style::getInstance().read(reader));

    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    assert(reader.getElementLocalName() == "Style");

    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  assert(reader.getElementLocalName() == "DataSetLayer");

  reader.next();

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

  return layer.release();
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

  /* Query Element */
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Query");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string query = reader.getElementValue();
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
  std::auto_ptr<te::gm::Envelope> mbr(te::serialize::ReadExtent(reader));

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

    style.reset(te::serialize::Style::getInstance().read(reader));

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
  //layer->setQuery(query); Uba
  layer->setDataSourceId(datasourceId);
  layer->setRendererType(rendererId);
  layer->setStyle(style.release());

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

  const te::serialize::Layer& lserial = te::serialize::Layer::getInstance();

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
  std::auto_ptr<te::gm::Envelope> mbr(te::serialize::ReadExtent(reader));

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

    style.reset(te::serialize::Style::getInstance().read(reader));

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

void DataSetLayerWriter(const te::map::AbstractLayer* alayer, te::xml::Writer& writer)
{
  const te::map::DataSetLayer* layer = dynamic_cast<const te::map::DataSetLayer*>(alayer);

  if(layer == 0)
    return;

  writer.writeStartElement("te_map:DataSetLayer");

  WriteAbstractLayer(layer, writer);

  writer.writeElement("te_map:DataSetName", layer->getDataSetName());
  writer.writeElement("te_map:DataSourceId", layer->getDataSourceId());
  writer.writeElement("te_map:SRID", layer->getSRID());
  te::serialize::SaveExtent(&layer->getExtent(), writer);
  writer.writeElement("te_map:RendererId", layer->getRendererType());

  if(layer->getStyle())
  {
    writer.writeStartElement("te_map:Style");

    te::serialize::Style::getInstance().write(layer->getStyle(), writer);

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

  writer.writeElement("te_map:Query", "");
  writer.writeElement("te_map:DataSourceId", layer->getDataSourceId());
  writer.writeElement("te_map:SRID", layer->getSRID());
  te::serialize::SaveExtent(&layer->getExtent(), writer);
  writer.writeElement("te_map:RendererId", layer->getRendererType());

  if(layer->getStyle())
  {
    writer.writeStartElement("te_map:Style");

    te::serialize::Style::getInstance().write(layer->getStyle(), writer);

    writer.writeEndElement("te_map:Style");
  }

  writer.writeEndElement("te_map:QueryLayer");
}

void FolderLayerWriter(const te::map::AbstractLayer* /*alayer*/, te::xml::Writer& /*writer*/)
{
  throw te::serialize::Exception("Not implemented yet!");
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
  te::serialize::SaveExtent(&layer->getExtent(), writer);
  writer.writeElement("te_map:RendererId", layer->getRendererType());

  if(layer->getStyle())
  {
    writer.writeStartElement("te_map:Style");

    te::serialize::Style::getInstance().write(layer->getStyle(), writer);

    writer.writeEndElement("te_map:Style");
  }

  writer.writeEndElement("te_map:RasterLayer");
}
