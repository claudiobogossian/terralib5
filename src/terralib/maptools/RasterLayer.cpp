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
#include "../raster/Raster.h"
#include "../raster/RasterFactory.h"
#include "../se/CoverageStyle.h"
#include "Exception.h"
#include "RasterLayer.h"
#include "RendererFactory.h"

// Boost
#include <boost/format.hpp>

const std::string te::map::RasterLayer::sm_type("RASTERLAYER");

te::map::RasterLayer::RasterLayer(AbstractLayer* parent)
  : AbstractLayer(parent),
    m_style(0)
{
}

te::map::RasterLayer::RasterLayer(const std::string& id, AbstractLayer* parent)
  : AbstractLayer(id, parent),
    m_style(0)
{
}

te::map::RasterLayer::RasterLayer(const std::string& id,
                                  const std::string& title,
                                  AbstractLayer* parent)
  : AbstractLayer(id, title, parent),
    m_style(0)
{
}

te::map::RasterLayer::~RasterLayer()
{
  delete m_style;

  m_rinfo.clear();
}

te::da::DataSet* te::map::RasterLayer::getData(te::common::TraverseType travType, 
                                                te::common::AccessPolicy rwRole) const
{
  return 0;
}

bool te::map::RasterLayer::isValid() const
{
  return true;
}

void te::map::RasterLayer::draw(Canvas* canvas, const te::gm::Envelope& bbox, int srid)
{
  if(m_rendererType.empty())
    throw Exception((boost::format(TR_MAP("Could not draw the data set layer %1%. The renderer type is empty!")) % getTitle()).str());

  // Try get the defined renderer
  std::auto_ptr<AbstractRenderer> renderer(RendererFactory::make(m_rendererType));
  if(renderer.get() == 0)
    throw Exception((boost::format(TR_MAP("Could not draw the data set layer %1%. The renderer %2% could not be created!")) % getTitle() % m_rendererType).str());

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

te::se::CoverageStyle* te::map::RasterLayer::getStyle() const
{
  return m_style;
}

void te::map::RasterLayer::setStyle(te::se::CoverageStyle* style)
{
  delete m_style;

  m_style = style;
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