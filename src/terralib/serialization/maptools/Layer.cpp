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
#include "../../maptools/Layer.h"
#include "../../maptools/QueryLayer.h"
#include "../geometry/Envelope.h"
#include "../Exception.h"
#include "Layer.h"

// STL
#include <cassert>

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/format.hpp>

te::map::AbstractLayer* LayerReader(te::xml::Reader& reader);
te::map::AbstractLayer* QueryLayerReader(te::xml::Reader& reader);
void LayerWriter(const te::map::AbstractLayer* layer, te::xml::Writer& writer);
void QueryLayerWriter(const te::map::AbstractLayer* layer, te::xml::Writer& writer);

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
  m_fncts["LAYER"] = std::make_pair(LayerReadFnctType(&LayerReader), LayerWriteFnctType(&LayerWriter));
  m_fncts["QUERYLAYER"] = std::make_pair(LayerReadFnctType(&QueryLayerReader), LayerWriteFnctType(&QueryLayerWriter));
}

te::map::AbstractLayer* LayerReader(te::xml::Reader& reader)
{
  std::string id = reader.getAttr(0);

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Title");

  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string title = reader.getElementValue();

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "SRID");

  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  int srid = reader.getElementValueAsInt32();

  reader.next();
  assert(reader.getElementLocalName() == "Extent");
  std::auto_ptr<te::gm::Envelope> mbr(te::serialize::ReadExtent(reader));

  reader.next();
  assert(reader.getElementLocalName() == "Visible");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  bool visible = reader.getElementValueAsBoolean();

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "DataSet");

  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string dataset = reader.getElementValue();

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "DataSourceId");

  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string datasourceId = reader.getElementValue();

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Renderer");

  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string rendererId = reader.getElementValue();

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Style");

  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string style = reader.getElementValue();

  reader.next();

  std::auto_ptr<te::map::Layer> layer(new te::map::Layer(id, title, 0));
  layer->setSRID(srid);
  layer->setExtent(mbr.release());
  layer->setVisibility(visible == true ? te::map::VISIBLE : te::map::NOT_VISIBLE);
  layer->setDataSetName(dataset);
  layer->setDataSource(datasourceId);
  layer->setRenderer(rendererId);

  return layer.release();
}

te::map::AbstractLayer* QueryLayerReader(te::xml::Reader& reader)
{
  std::string id = reader.getAttr(0);

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Title");

  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string title = reader.getElementValue();

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "SRID");

  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  int srid = reader.getElementValueAsInt32();

  reader.next();
  assert(reader.getElementLocalName() == "Extent");
  std::auto_ptr<te::gm::Envelope> mbr(te::serialize::ReadExtent(reader));

  reader.next();
  assert(reader.getElementLocalName() == "Visible");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  bool visible = reader.getElementValueAsBoolean();

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Query");

  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string query = reader.getElementValue();

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "DataSourceId");

  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string datasourceId = reader.getElementValue();

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Renderer");

  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string rendererId = reader.getElementValue();

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Style");

  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string style = reader.getElementValue();

  reader.next();

  std::auto_ptr<te::map::QueryLayer> layer(new te::map::QueryLayer(id, title, 0));
  layer->setSRID(srid);
  layer->setExtent(mbr.release());
  layer->setVisibility(visible == true ? te::map::VISIBLE : te::map::NOT_VISIBLE);
  layer->setQuery(query);
  layer->setDataSource(datasourceId);
  layer->setRenderer(rendererId);

  return layer.release();
}

void LayerWriter(const te::map::AbstractLayer* alayer, te::xml::Writer& writer)
{
  const te::map::Layer* layer = dynamic_cast<const te::map::Layer*>(alayer);

  if(layer == 0)
    return;

  writer.writeStartElement("Layer");

  writer.writeAttribute("id", layer->getId());
  writer.writeElement("Title", layer->getTitle());
  writer.writeElement("SRID", layer->getSRID());
  te::serialize::SaveExtent(layer->getExtent(), writer);
  writer.writeElement("Visible", layer->getVisibility() == te::map::VISIBLE);
  writer.writeElement("DataSet", layer->getDataSetName());
  writer.writeElement("DataSourceId", layer->getDataSource());
  writer.writeElement("Renderer", "k");
  writer.writeElement("Style", "x");

  writer.writeEndElement("Layer");
}

void QueryLayerWriter(const te::map::AbstractLayer* alayer, te::xml::Writer& writer)
{
  const te::map::QueryLayer* layer = dynamic_cast<const te::map::QueryLayer*>(alayer);

  if(layer == 0)
    return;

  writer.writeStartElement("QueryLayer");

  writer.writeAttribute("id", layer->getId());
  writer.writeElement("Title", layer->getTitle());
  writer.writeElement("SRID", layer->getSRID());
  te::serialize::SaveExtent(layer->getExtent(), writer);
  writer.writeElement("Visible", layer->getVisibility() == te::map::VISIBLE);
  writer.writeElement("Query", layer->getQuery());
  writer.writeElement("DataSourceId", layer->getDataSource());
  writer.writeElement("Renderer", "x");
  writer.writeElement("Style", "z");

  writer.writeEndElement("QueryLayer");
}

