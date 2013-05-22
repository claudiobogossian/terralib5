/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraView - A GIS Application.

    TerraView is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraView is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib Code Editor. See COPYING. If not, write to
    TerraLib Team at <terralib-team@dpi.inpe.br>.
 */

#include "LayerDecorator.h"

#include "events/LayerEvents.h"
#include "ApplicationController.h"

te::qt::af::LayerDecorator::LayerDecorator(const te::map::AbstractLayerPtr& layer) :
te::common::Decorator<te::map::AbstractLayer>(layer.get(), false),
  m_layer(layer)
{
}

te::qt::af::LayerDecorator::~LayerDecorator()
{
}

const std::string& te::qt::af::LayerDecorator::getId() const
{
  return m_decorated->getId();
}

void te::qt::af::LayerDecorator::setId(const std::string& id)
{
  m_decorated->setId(id);
}

const std::string& te::qt::af::LayerDecorator::getTitle() const
{
  return m_decorated->getTitle();
}

void te::qt::af::LayerDecorator::setTitle(const std::string& title)
{
  m_decorated->setTitle(title);
}
        
te::map::Visibility te::qt::af::LayerDecorator::getVisibility() const
{
  return m_decorated->getVisibility();
}

void te::qt::af::LayerDecorator::setVisibility(te::map::Visibility v)
{
  m_decorated->setVisibility(v);
}

void te::qt::af::LayerDecorator::adjustVisibility()
{
  m_decorated->adjustVisibility();
}

const te::gm::Envelope& te::qt::af::LayerDecorator::getExtent() const
{
  return m_decorated->getExtent();
}

void te::qt::af::LayerDecorator::setExtent(const te::gm::Envelope& mbr)
{
  m_decorated->setExtent(mbr);
}

int te::qt::af::LayerDecorator::getSRID() const
{
  return m_decorated->getSRID();
}

void te::qt::af::LayerDecorator::setSRID(int srid)
{
  m_decorated->setSRID(srid);
}

void te::qt::af::LayerDecorator::select(te::da::ObjectIdSet* oids)
{
  m_decorated->select(oids);

  te::qt::af::evt::HighlightObjects evt(this);

  ApplicationController::getInstance().broadcast(&evt);
}

void te::qt::af::LayerDecorator::deselect(const te::da::ObjectIdSet* oids)
{
  m_decorated->deselect(oids);

  te::qt::af::evt::HighlightObjects evt(this);

  ApplicationController::getInstance().broadcast(&evt);
}

const te::da::ObjectIdSet* te::qt::af::LayerDecorator::getSelected() const
{
  return m_decorated->getSelected();
}

void te::qt::af::LayerDecorator::clearSelected()
{
  m_decorated->clearSelected();
}

const te::map::LayerSchema* te::qt::af::LayerDecorator::getSchema() const
{
  return m_decorated->getSchema();
}

te::da::DataSet* te::qt::af::LayerDecorator::getData(te::common::TraverseType travType, 
                                te::common::AccessPolicy rwRole) const
{
  return m_decorated->getData(travType, rwRole);
}

te::da::DataSet* te::qt::af::LayerDecorator::getData(const te::gm::Envelope& e,
                                te::gm::SpatialRelation r,
                                te::common::TraverseType travType,
                                te::common::AccessPolicy rwRole) const
{
  return m_decorated->getData(e, r, travType, rwRole);
}

te::da::DataSet* te::qt::af::LayerDecorator::getData(const te::dt::Property& p,
                                const te::gm::Envelope& e,
                                te::gm::SpatialRelation r,
                                te::common::TraverseType travType,
                                te::common::AccessPolicy rwRole) const
{
  return m_decorated->getData(p, e, r, travType, rwRole);
}


te::da::DataSet* te::qt::af::LayerDecorator::getData(const te::gm::Geometry& g,
                                te::gm::SpatialRelation r,
                                te::common::TraverseType travType, 
                                te::common::AccessPolicy rwRole) const
{
  return m_decorated->getData(g, r, travType, rwRole);
}

te::da::DataSet* te::qt::af::LayerDecorator::getData(const te::dt::Property& p,
                                const te::gm::Geometry& g,
                                te::gm::SpatialRelation r,
                                te::common::TraverseType travType,
                                te::common::AccessPolicy rwRole) const
{
  return m_decorated->getData(p, g, r, travType, rwRole);
}

te::da::DataSet* te::qt::af::LayerDecorator::getData(const te::da::ObjectIdSet* oids,
                                te::common::TraverseType travType,
                                te::common::AccessPolicy rwRole) const
{
  return m_decorated->getData(oids, travType, rwRole);
}
          
const std::string& te::qt::af::LayerDecorator::getType() const
{
  return m_decorated->getType();
}

bool te::qt::af::LayerDecorator::isValid() const
{
  return m_decorated->isValid();
}

void te::qt::af::LayerDecorator::draw(te::map::Canvas* canvas, const te::gm::Envelope& bbox, int srid)
{
  m_decorated->draw(canvas, bbox, srid);
}
