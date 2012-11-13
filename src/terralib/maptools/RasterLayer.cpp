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
  \file RasterLayer.cpp

  \brief A RasterLayer is a reference to a virtual dataset with raster geometry.
*/

// TerraLib
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../geometry/Envelope.h"
#include "../raster/Raster.h"
#include "../raster/Grid.h"
#include "../raster/RasterProperty.h"
#include "../se/FeatureTypeStyle.h"
#include "../se/RasterSymbolizer.h"
#include "../se/Rule.h"
#include "../se/Style.h"
#include "RasterLayer.h"
#include "RasterLayerRenderer.h"


const std::string te::map::RasterLayer::sm_type("RASTERLAYER");

te::map::RasterLayer::RasterLayer(const std::string& id,
                      const std::string& title,
                      AbstractLayer* parent)
  : te::map::AbstractLayer(id, title, parent),
    m_srid(-1),
    m_ds(0),
    m_mbr(0),
    m_style(0),
    m_renderer(0),
    m_raster(0),
    m_rasterProperty(0),
    m_symbolizer(0)
{
}

te::map::RasterLayer::~RasterLayer()
{
  delete m_rasterProperty;
}

const std::string& te::map::RasterLayer::getType() const
{
  return sm_type;
}

bool te::map::RasterLayer::isValid() const
{
  return m_ds->isValid();
}

bool te::map::RasterLayer::isQueryable() const
{
  return false;
}

void te::map::RasterLayer::draw(Canvas* canvas, const te::gm::Envelope& bbox, int srid)
{
  assert(m_renderer.get());
  m_renderer->draw(this, canvas, bbox, srid);
}

te::se::Style* te::map::RasterLayer::getStyle() const
{
  return m_style.get();
}

void te::map::RasterLayer::setStyle(te::se::Style* style)
{
  m_style.reset(style);

  if(m_style->getNRules() != 0)
  {
    const te::se::Rule* r = m_style->getRule(0);

    if(r && r->getSymbolizers().empty() == false)
    {
      m_symbolizer = static_cast<const te::se::RasterSymbolizer*>(r->getSymbolizer(0));
    }
  }
}

const te::gm::Envelope* te::map::RasterLayer::getExtent() const
{
  return m_mbr.get();
}

void te::map::RasterLayer::setExtent(te::gm::Envelope* mbr)
{
  m_mbr.reset(mbr);
}

int te::map::RasterLayer::getSRID() const
{
  return m_srid;
}

void te::map::RasterLayer::setSRID(int srid)
{
  m_srid = srid;

  m_raster->getGrid()->setSRID(m_srid);
}

te::da::DataSource* te::map::RasterLayer::getDataSource() const
{
  return m_ds;
}

void te::map::RasterLayer::setDataSource(te::da::DataSource* ds)
{
  m_ds = ds;

  loadRasterObjects();
}

const std::string& te::map::RasterLayer::getDataSetName() const
{
  return m_datasetName;
}

void te::map::RasterLayer::setDataSetName(const std::string& datasetName)
{
  m_datasetName = datasetName;

  loadRasterObjects();
}

te::map::RasterLayerRenderer* te::map::RasterLayer::getRenderer() const
{
  return m_renderer.get();
}

void te::map::RasterLayer::setRenderer(RasterLayerRenderer* renderer)
{
  m_renderer.reset(renderer);
}

te::rst::Raster* te::map::RasterLayer::getRaster()
{
  return m_raster;
}

const te::rst::RasterProperty* te::map::RasterLayer::getRasterProperty()
{
  return m_rasterProperty;
}

const te::se::RasterSymbolizer* te::map::RasterLayer::getRasterSymbolizer()
{
  if(m_symbolizer == 0)
  {
    if(m_renderer.get() != 0)
    {
      m_renderer->createVisualDefault(this);
    }
  }

  return m_symbolizer;
}

void te::map::RasterLayer::loadRasterObjects()
{
  m_raster = 0;
  m_rasterProperty = 0;

  if(m_ds && !m_datasetName.empty())
  {
    te::da::DataSourceTransactor* tr = m_ds->getTransactor();
    te::da::DataSetType* dt = tr->getCatalogLoader()->getDataSetType(m_datasetName);
    
    m_rasterProperty = static_cast<te::rst::RasterProperty*>(dt->getProperties()[0]->clone());

    te::da::DataSet* dataSet = tr->getDataSet(m_datasetName);
    m_raster = dataSet->getRaster();

    m_srid = m_raster->getSRID();

    te::gm::Envelope* e = new te::gm::Envelope(*m_raster->getExtent());

    setExtent(e);

    delete dt;
    delete tr;
  }
}
