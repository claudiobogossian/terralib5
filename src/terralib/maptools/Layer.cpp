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
#include "../dataaccess/datasource/DataSource.h"
#include "../geometry/Envelope.h"
#include "../se/FeatureTypeStyle.h"
#include "../se/Style.h"
#include "Layer.h"
#include "LayerRenderer.h"

const std::string te::map::Layer::sm_type("LAYER");

te::map::Layer::Layer(const std::string& id,
                      const std::string& title,
                      AbstractLayer* parent)
  : te::map::AbstractLayer(id, title, parent),
    m_srid(-1),
    m_ds(0),
    m_mbr(0),
    m_style(0),
    m_renderer(0)
{
}

te::map::Layer::~Layer()
{  
}

const std::string& te::map::Layer::getType() const
{
  return sm_type;
}

bool te::map::Layer::isValid() const
{
  return m_ds->isValid();
}

bool te::map::Layer::isQueryable() const
{
  return false;
}

void te::map::Layer::draw(Canvas* canvas, const te::gm::Envelope& bbox, int srid)
{
  assert(m_renderer.get());
  m_renderer->draw(this, canvas, bbox, srid);
}

te::se::Style* te::map::Layer::getStyle() const
{
  return m_style.get();
}

void te::map::Layer::setStyle(te::se::Style* style)
{
  m_style.reset(style);
}

const te::gm::Envelope* te::map::Layer::getExtent() const
{
  return m_mbr.get();
}

void te::map::Layer::setExtent(te::gm::Envelope* mbr)
{
  m_mbr.reset(mbr);
}

int te::map::Layer::getSRID() const
{
  return m_srid;
}

void te::map::Layer::setSRID(int srid)
{
  m_srid = srid;
}

te::da::DataSource* te::map::Layer::getDataSource() const
{
  return m_ds;
}

void te::map::Layer::setDataSource(te::da::DataSource* ds)
{
  m_ds = ds;
}

const std::string& te::map::Layer::getDataSetName() const
{
  return m_datasetName;
}

void te::map::Layer::setDataSetName(const std::string& datasetName)
{
  m_datasetName = datasetName;
}

te::map::LayerRenderer* te::map::Layer::getRenderer() const
{
  return m_renderer.get();
}

void te::map::Layer::setRenderer(LayerRenderer* renderer)
{
  m_renderer.reset(renderer);
}
