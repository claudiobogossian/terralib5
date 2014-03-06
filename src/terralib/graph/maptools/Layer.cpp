/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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

  \brief A Layer is a reference to a virtual dataset with/without geometric attributes.
*/

// TerraLib
#include "../../geometry/Envelope.h"
#include "../../se/FeatureTypeStyle.h"
#include "../../se/Style.h"
#include "../core/AbstractGraph.h"
#include "../core/GraphMetadata.h"
#include "Layer.h"
#include "LayerRenderer.h"

const std::string te::graph::Layer::sm_type("GRAPHLAYER");

te::graph::Layer::Layer(const std::string& id, const std::string& title, AbstractLayer* parent) : 
  te::map::AbstractLayer(id, title, parent),
  m_mbr(0),
  m_style(0),
//  m_srid(-1),
  m_renderer(0)
{
}

te::graph::Layer::~Layer()
{ 
}

std::auto_ptr<te::da::DataSetType> te::graph::Layer::getSchema() const
{
  return std::auto_ptr<te::da::DataSetType>();
}



std::auto_ptr<te::da::DataSet> te::graph::Layer::getData(te::common::TraverseType travType, 
                                                const te::common::AccessPolicy accessPolicy) const
{
  return std::auto_ptr<te::da::DataSet>();
}


std::auto_ptr<te::da::DataSet> te::graph::Layer::getData(const std::string& propertyName,
                                                const te::gm::Envelope* e,
                                                te::gm::SpatialRelation r,
                                                te::common::TraverseType travType,
                                                const te::common::AccessPolicy accessPolicy) const
{
  return std::auto_ptr<te::da::DataSet>();
}



std::auto_ptr<te::da::DataSet> te::graph::Layer::getData(const std::string& propertyName,
                                                const te::gm::Geometry* g,
                                                te::gm::SpatialRelation r,
                                                te::common::TraverseType travType,
                                                const te::common::AccessPolicy accessPolicy) const
{
  return std::auto_ptr<te::da::DataSet>();
}



std::auto_ptr<te::da::DataSet> te::graph::Layer::getData(te::da::Expression* restriction,
                                                te::common::TraverseType travType,
                                                const te::common::AccessPolicy accessPolicy) const
{
  return std::auto_ptr<te::da::DataSet>();
}



std::auto_ptr<te::da::DataSet> te::graph::Layer::getData(const te::da::ObjectIdSet* oids,
                                                te::common::TraverseType travType,
                                                const te::common::AccessPolicy accessPolicy) const
{
  return std::auto_ptr<te::da::DataSet>();
}


const std::string& te::graph::Layer::getType() const
{
  return sm_type;
}

bool te::graph::Layer::isValid() const
{
  return true;
}

void te::graph::Layer::draw(te::map::Canvas* canvas, const te::gm::Envelope& bbox, int srid)
{
  assert(m_renderer.get());
  m_renderer->draw(this, canvas, bbox, srid);
}

te::graph::AbstractGraph* te::graph::Layer::getGraph() const
{
  return m_graph;
}

void te::graph::Layer::setGraph(te::graph::AbstractGraph* g)
{
  m_graph = g;

  //set graph parameters
  if(m_graph->getMetadata())
  {
    //this->setSRID(m_graph->getMetadata()->getSRID());
    //this->setExtent(m_graph->getMetadata()->getEnvelope());
  }
}

te::graph::LayerRenderer* te::graph::Layer::getRenderer() const
{
  return m_renderer.get();
}

void te::graph::Layer::setRenderer(LayerRenderer* renderer)
{
  m_renderer.reset(renderer);
}
