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
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../maptools/AbstractLayer.h"
#include "../../maptools/DataSetLayer.h"
#include "../../maptools/FolderLayer.h"
#include "../../maptools/QueryLayer.h"
#include "../../se/Style.h"
#include "../geometry/Envelope.h"
#include "../se/Style.h"
#include "../Exception.h"
#include "Layer.h"

// STL
#include <cassert>

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/format.hpp>

te::map::AbstractLayer* DataSetLayerReader(te::xml::Reader& reader);
te::map::AbstractLayer* QueryLayerReader(te::xml::Reader& reader);
te::map::AbstractLayer* FolderLayerReader(te::xml::Reader& reader);
void DataSetLayerWriter(const te::map::AbstractLayer* layer, te::xml::Writer& writer);
void QueryLayerWriter(const te::map::AbstractLayer* layer, te::xml::Writer& writer);
void FolderLayerWriter(const te::map::AbstractLayer* layer, te::xml::Writer& writer);

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
}

te::map::AbstractLayer* DataSetLayerReader(te::xml::Reader& reader)
{
  std::string id = reader.getAttr(0);

  /* Title Element */
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Title");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string title = reader.getElementValue();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  /* Visible Element */
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Visible");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  bool visible = reader.getElementValueAsBoolean();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  /* DataSetName Element */
  reader.next();
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
  layer->setVisibility(visible == true ? te::map::VISIBLE : te::map::NOT_VISIBLE);
  layer->setDataSetName(dataset);
  layer->setDataSourceId(datasourceId);
  layer->setRendererType(rendererId);
  layer->setStyle(style.release());

  return layer.release();
}

te::map::AbstractLayer* QueryLayerReader(te::xml::Reader& reader)
{
  std::string id = reader.getAttr(0);

 /* Title Element */
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Title");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string title = reader.getElementValue();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  /* Visible Element */
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Visible");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  bool visible = reader.getElementValueAsBoolean();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

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
  layer->setVisibility(visible == true ? te::map::VISIBLE : te::map::NOT_VISIBLE);
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
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Title");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string title = reader.getElementValue();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  /* Visible Element */
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Visible");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  bool visible = reader.getElementValueAsBoolean();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "LayerList");

  reader.next();

  std::auto_ptr<te::map::FolderLayer> flayer(new te::map::FolderLayer(id, title, 0));

  flayer->setVisibility(visible == true ? te::map::VISIBLE : te::map::NOT_VISIBLE);

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

  return flayer.release();
}

void DataSetLayerWriter(const te::map::AbstractLayer* alayer, te::xml::Writer& writer)
{
  const te::map::DataSetLayer* layer = dynamic_cast<const te::map::DataSetLayer*>(alayer);

  if(layer == 0)
    return;

  writer.writeStartElement("DataSetLayer");

  writer.writeAttribute("id", layer->getId());
  writer.writeElement("Title", layer->getTitle());
  writer.writeElement("Visible", layer->getVisibility() == te::map::VISIBLE);
  writer.writeElement("DataSetName", layer->getDataSetName());
  writer.writeElement("DataSourceId", layer->getDataSourceId());
  writer.writeElement("SRID", layer->getSRID());
  te::serialize::SaveExtent(&layer->getExtent(), writer);
  writer.writeElement("RendererId", layer->getRendererType());

  if(layer->getStyle())
  {
    writer.writeStartElement("Style");

    te::serialize::Style::getInstance().write(layer->getStyle(), writer);

    writer.writeEndElement("Style");
  }

  writer.writeEndElement("DataSetLayer");
}

void QueryLayerWriter(const te::map::AbstractLayer* alayer, te::xml::Writer& writer)
{
  const te::map::QueryLayer* layer = dynamic_cast<const te::map::QueryLayer*>(alayer);

  if(layer == 0)
    return;

  writer.writeStartElement("QueryLayer");

  writer.writeAttribute("id", layer->getId());
  writer.writeElement("Title", layer->getTitle());
  writer.writeElement("Visible", layer->getVisibility() == te::map::VISIBLE);
  writer.writeElement("Query", "");
  writer.writeElement("DataSourceId", layer->getDataSourceId());
  writer.writeElement("SRID", layer->getSRID());
  te::serialize::SaveExtent(&layer->getExtent(), writer);
  writer.writeElement("RendererId", layer->getRendererType());

  if(layer->getStyle())
  {
    writer.writeStartElement("Style");

    te::serialize::Style::getInstance().write(layer->getStyle(), writer);

    writer.writeEndElement("Style");
  }

  writer.writeEndElement("QueryLayer");
}

void FolderLayerWriter(const te::map::AbstractLayer* /*alayer*/, te::xml::Writer& /*writer*/)
{
  throw te::serialize::Exception("Not implemented yet!");
  //const te::map::DataSetLayer* layer = dynamic_cast<const te::map::DataSetLayer*>(alayer);

  //if(layer == 0)
  //  return;

  //writer.writeStartElement("DataSetLayer");

  //writer.writeAttribute("id", layer->getId());
  //writer.writeElement("Title", layer->getTitle());
  //writer.writeElement("Visible", layer->getVisibility() == te::map::VISIBLE);
  //writer.writeElement("DataSetName", layer->getDataSetName());
  //writer.writeElement("DataSourceId", layer->getDataSourceId());
  //writer.writeElement("SRID", layer->getSRID());
  //te::serialize::SaveExtent(&layer->getExtent(), writer);
  //writer.writeElement("RendererId", layer->getRendererType());
  //writer.writeElement("StyleId", "x");

  //writer.writeEndElement("DataSetLayer");
}

