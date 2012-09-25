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
  \file LayerRenderer.cpp

  \brief It renders the objects associated to a Layer.
 */

// TerraLib
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "../fe/Filter.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../raster.h"
#include "../se/CoverageStyle.h"
#include "../se/FeatureTypeStyle.h"
#include "../se/Style.h"
#include "../se/Rule.h"
#include "../se/RasterSymbolizer.h"
#include "AbstractLayer.h"
#include "Canvas.h"
#include "RasterLayer.h"
#include "RasterLayerRenderer.h"


te::map::RasterLayerRenderer::RasterLayerRenderer(): _rasterCanvas(0), _resampledRaster(0)
{
}

te::map::RasterLayerRenderer::~RasterLayerRenderer()
{
  delete _rasterCanvas;
  delete _resampledRaster;
}

void te::map::RasterLayerRenderer::draw(AbstractLayer* layer, Canvas* canvas,
                                  const te::gm::Envelope& bbox, int srid)
{
  // Is our business?
  te::map::RasterLayer* l = dynamic_cast<RasterLayer*>(layer);
  assert(l);

  // Get Raster
  te::rst::Raster* r = l->getRaster();
  assert(r);

  // Check Intersection
  if(!hasIntersection(bbox, r, canvas, srid))
  {
    return;
  }

  // Get Symbolizer
  if(l->getRasterSymbolizer() == 0)
  {
    //createVisualDefault(l);
  }

  //get resampled input raster
 // getResampledRaster(r, canvas, bbox, srid);

  //reproject and resample input raster to raster canvas 
  buildRasterCanvas(r, canvas, bbox, srid);

  //apply the raster symbolizer


  //paint raster
  canvas->drawImage(0, 0, canvas->getWidth(), canvas->getHeight(), _rasterCanvas, 
    0, 0, _rasterCanvas->getNumberOfColumns(), _rasterCanvas->getNumberOfRows());
}

bool te::map::RasterLayerRenderer::hasIntersection(const te::gm::Envelope& bbox, te::rst::Raster* raster, Canvas* canvas,  int srid)
{
  te::gm::Envelope e(bbox);

  e.transform(srid, raster->getSRID());

  return e.intersects(*raster->getExtent());
}

void te::map::RasterLayerRenderer::getResampledRaster(te::rst::Raster* raster, Canvas* canvas, const te::gm::Envelope& bbox, int srid)
{
  delete _resampledRaster;

  te::gm::Envelope e(bbox);

  e.transform(srid, raster->getSRID());

  double resx = e.getWidth()/canvas->getWidth();
  double resy = e.getHeight()/canvas->getHeight();

  double x = resx/raster->getResolutionX();
  double y = resy/raster->getResolutionY();

  int scale = (int) std::min(x,y);
  
  std::map<std::string, std::string> rinfo;

  std::vector<te::rst::BandProperty*> bp;

  for(size_t i = 0; i < raster->getNumberOfBands(); ++i)
  {
    bp.push_back(new te::rst::BandProperty(*raster->getBand(i)->getProperty()));
  }

  te::rst::Grid* g = raster->getResampledGrid(scale);

  _resampledRaster = te::rst::RasterFactory::make("MEM", g, bp, rinfo);
}

void te::map::RasterLayerRenderer::buildRasterCanvas(te::rst::Raster* raster, Canvas* canvas, const te::gm::Envelope& bbox, int srid)
{
  delete _rasterCanvas;

  std::map<std::string, std::string> rinfo;
  rinfo["MEM_IS_DATA_BUFFER"] = "TRUE";

  if(raster->getSRID() != srid)
  {
    double resx = bbox.getWidth()/canvas->getWidth();
    double resy = bbox.getHeight()/canvas->getHeight();

    _rasterCanvas = raster->transform(srid, bbox.getLowerLeftX(), bbox.getLowerLeftY(), bbox.getUpperRightX(), bbox.getUpperRightY(), resx, resy, rinfo);
  }
  else
  {
    double resx = bbox.getWidth()/canvas->getWidth();
    double resy = bbox.getHeight()/canvas->getHeight();

    double x = resx/raster->getResolutionX();
    double y = resy/raster->getResolutionY();

    int scale = (int) std::min(x,y);

    _rasterCanvas = raster->resample(1, scale, rinfo);
  }
}

void te::map::RasterLayerRenderer::createVisualDefault(RasterLayer* layer)
{
  te::rst::Raster* raster = layer->getRaster();
  assert(raster);

  //create default raster symbolizer
  te::se::RasterSymbolizer* rs = 0;



  //add symbolizer to a layer style
  te::se::Rule* r = new te::se::Rule();
  r->push_back(rs);

  te::se::Style* s = new te::se::Style();
  s->push_back(r);

  layer->setStyle(s);
}
