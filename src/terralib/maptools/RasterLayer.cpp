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
  \file terralib/maptools/RasterLayer.cpp

  \brief A layer with reference to a raster.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../raster/Band.h"
#include "../raster/BandProperty.h"
#include "../raster/Grid.h"
#include "../raster/Raster.h"
#include "../raster/RasterFactory.h"
#include "Exception.h"
#include "RasterLayer.h"
#include "RendererFactory.h"

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

const std::string te::map::RasterLayer::sm_type("RASTERLAYER");

te::map::RasterLayer::RasterLayer(AbstractLayer* parent)
  : AbstractLayer(parent)
{
}

te::map::RasterLayer::RasterLayer(const std::string& id, AbstractLayer* parent)
  : AbstractLayer(id, parent)
{
}

te::map::RasterLayer::RasterLayer(const std::string& id,
                                  const std::string& title,
                                  AbstractLayer* parent)
  : AbstractLayer(id, title, parent)
{
}

te::map::RasterLayer::~RasterLayer()
{
  m_rinfo.clear();
}

std::auto_ptr<te::map::LayerSchema> te::map::RasterLayer::getSchema() const
{
  LayerSchema* ls = 0;

  te::rst::Raster* r = te::rst::RasterFactory::open(m_rinfo);

  if(r)
  {
    te::rst::Grid* grid = new te::rst::Grid(*r->getGrid());

    std::vector<te::rst::BandProperty*> bprops;

    for(size_t t = 0; t < r->getNumberOfBands(); ++t)
    {
      te::rst::BandProperty* bp = new te::rst::BandProperty(*r->getBand(t)->getProperty());

      bprops.push_back(bp);
    }

    te::rst::RasterProperty* rp = new te::rst::RasterProperty(grid, bprops, m_rinfo);

    std::string path = "";

    std::map<std::string, std::string>::const_iterator it = m_rinfo.find("URI");

    if(it != m_rinfo.end())
    {
      path = it->second;
    }

    boost::filesystem::path rpath (path);

    ls = new te::da::DataSetType(rpath.filename().string());

    ls->add(rp);
  }

  delete r;

  return std::auto_ptr<LayerSchema>(ls);
}

std::auto_ptr<te::da::DataSet> te::map::RasterLayer::getData( te::common::TraverseType travType,
                                                              const te::common::AccessPolicy) const
{ 
  return std::auto_ptr<te::da::DataSet>(0); 
}

std::auto_ptr<te::da::DataSet> te::map::RasterLayer::getData(const std::string& /*propertyName*/,
                                                             const te::gm::Envelope* /*e*/,
                                                             te::gm::SpatialRelation /*r*/,
                                                             te::common::TraverseType /*travType*/,
                                                             const te::common::AccessPolicy) const
{ 
  return std::auto_ptr<te::da::DataSet>(0); 
}

std::auto_ptr<te::da::DataSet> te::map::RasterLayer::getData(const std::string& /*propertyName*/,
                                                             const te::gm::Geometry* /*g*/,
                                                             te::gm::SpatialRelation /*r*/,
                                                             te::common::TraverseType /*travType*/,
                                                             const te::common::AccessPolicy) const
{ 
  return std::auto_ptr<te::da::DataSet>(0); 
}

std::auto_ptr<te::da::DataSet> te::map::RasterLayer::getData(te::da::Expression* /*restriction*/,
                                                             te::common::TraverseType /*travType*/,
                                                             const te::common::AccessPolicy) const
{ 
  return std::auto_ptr<te::da::DataSet>(0); 
}

std::auto_ptr<te::da::DataSet> te::map::RasterLayer::getData(const te::da::ObjectIdSet* /*oids*/,
                                                             te::common::TraverseType /*travType*/,
                                                             const te::common::AccessPolicy) const
{ 
  return std::auto_ptr<te::da::DataSet>(0); 
}


bool te::map::RasterLayer::isValid() const
{
  return true;
}

void te::map::RasterLayer::draw(Canvas* canvas, const te::gm::Envelope& bbox, int srid)
{
  if(m_rendererType.empty())
    throw Exception((boost::format(TE_TR("Could not draw the data set layer %1%. The renderer type is empty!")) % getTitle()).str());

  // Try get the defined renderer
  std::auto_ptr<AbstractRenderer> renderer(RendererFactory::make(m_rendererType));
  if(renderer.get() == 0)
    throw Exception((boost::format(TE_TR("Could not draw the data set layer %1%. The renderer %2% could not be created!")) % getTitle() % m_rendererType).str());

  renderer->draw(this, canvas, bbox, srid);
}

const std::string& te::map::RasterLayer::getType() const
{
  return sm_type;
}

const std::string& te::map::RasterLayer::getRendererType() const
{
  return m_rendererType;
}

void te::map::RasterLayer::setRendererType(const std::string& t)
{
  m_rendererType = t;
}

te::rst::Raster* te::map::RasterLayer::getRaster() const
{
  te::rst::Raster* r = te::rst::RasterFactory::open(m_rinfo);

  return r;
}

void  te::map::RasterLayer::setRasterInfo(const std::map<std::string, std::string>& rinfo)
{
  m_rinfo = rinfo;

  te::rst::Raster* r = te::rst::RasterFactory::open(rinfo);

  if(r)
  {
    setSRID(r->getSRID());
    setExtent(*r->getExtent());
  }

  delete r;
}

std::map<std::string, std::string> te::map::RasterLayer::getRasterInfo() const
{
  return m_rinfo;
}
