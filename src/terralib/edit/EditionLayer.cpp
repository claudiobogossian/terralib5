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
  \file terralib/edit/EditionLayer.cpp

  \brief This class represents a layer being created from user-edition.
*/

// TerraLib
#include "../common/Exception.h"
#include "../common/Translator.h"
#include "../maptools/RendererFactory.h"
#include "EditionLayer.h"

// Boost
#include <boost/format.hpp>

// STL
#include <memory>

const std::string te::edit::EditionLayer::sm_type("EDITIONLAYER");

te::edit::EditionLayer::EditionLayer(te::map::AbstractLayer* parent)
  : te::map::AbstractLayer(parent),
    m_schema(0)
{
}

te::edit::EditionLayer::EditionLayer(const std::string& id, te::map::AbstractLayer* parent)
  : te::map::AbstractLayer(id, parent),
    m_schema(0)
{
}

te::edit::EditionLayer::EditionLayer(const std::string& id,
                                     const std::string& title,
                                     te::map::AbstractLayer* parent)
  : te::map::AbstractLayer(id, title, parent),
    m_schema(0)
{
}

te::edit::EditionLayer::~EditionLayer()
{
}

std::auto_ptr<te::map::LayerSchema> te::edit::EditionLayer::getSchema() const
{
  return std::auto_ptr<te::map::LayerSchema>(static_cast<te::map::LayerSchema*>(m_schema->clone()));
}

void te::edit::EditionLayer::setSchema(te::map::LayerSchema* schema)
{
  delete m_schema;
  m_schema = schema;
}

std::auto_ptr<te::da::DataSet> te::edit::EditionLayer::getData(te::common::TraverseType travType,
                                                               const te::common::AccessPolicy accessPolicy) const
{
  throw(te::common::Exception("Under development!"));
}

std::auto_ptr<te::da::DataSet> te::edit::EditionLayer::getData(const std::string& propertyName,
                                                               const te::gm::Envelope* e,
                                                               te::gm::SpatialRelation r,
                                                               te::common::TraverseType travType,
                                                               const te::common::AccessPolicy accessPolicy) const
{
  throw(te::common::Exception("Under development!"));
}

std::auto_ptr<te::da::DataSet> te::edit::EditionLayer::getData(const std::string& propertyName,
                                                               const te::gm::Geometry* g,
                                                               te::gm::SpatialRelation r,
                                                               te::common::TraverseType travType,
                                                               const te::common::AccessPolicy accessPolicy) const
{
  throw(te::common::Exception("Under development!"));
}

std::auto_ptr<te::da::DataSet> te::edit::EditionLayer::getData(te::da::Expression* restriction,
                                                            te::common::TraverseType travType,
                                                            const te::common::AccessPolicy accessPolicy) const
{
  throw(te::common::Exception("Under development!"));
}

std::auto_ptr<te::da::DataSet> te::edit::EditionLayer::getData(const te::da::ObjectIdSet* oids,
                                                               te::common::TraverseType travType,
                                                               const te::common::AccessPolicy accessPolicy) const
{
  throw(te::common::Exception("Under development!"));
}

bool te::edit::EditionLayer::isValid() const
{
  return false;
}

void te::edit::EditionLayer::draw(te::map::Canvas* canvas, const te::gm::Envelope& bbox, int srid)
{
  if(m_rendererType.empty())
    throw te::common::Exception((boost::format(TE_TR("Could not draw the layer %1%. The renderer type is empty!")) % getTitle()).str());

  // Try get the defined renderer
  std::auto_ptr<te::map::AbstractRenderer> renderer(te::map::RendererFactory::make(m_rendererType));
  if(renderer.get() == 0)
    throw te::common::Exception((boost::format(TE_TR("Could not draw the layer %1%. The renderer %2% could not be created!")) % getTitle() % m_rendererType).str());

  renderer->draw(this, canvas, bbox, srid);
}

const std::string& te::edit::EditionLayer::getType() const
{
  return sm_type;
}

const std::string& te::edit::EditionLayer::getRendererType() const
{
  return m_rendererType;
}

void te::edit::EditionLayer::setRendererType(const std::string& t)
{
  m_rendererType = t;
}
