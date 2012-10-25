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
#include "../dataaccess.h"
#include "../fe/Filter.h"
#include "../geometry.h"
#include "../raster.h"
#include "../raster/RasterSummary.h"
#include "../raster/RasterSummaryManager.h"
#include "../se.h"
#include "AbstractLayer.h"
#include "Canvas.h"
#include "CanvasConfigurer.h"
#include "RasterLayer.h"
#include "RasterLayerRenderer.h"
#include "RasterTransform.h"
#include "RasterTransformConfigurer.h"
#include "Utils.h"


te::map::RasterLayerRenderer::RasterLayerRenderer(): m_rasterCanvas(0), m_rasterCanvasStyled(0), m_resampledRaster(0)
{
}

te::map::RasterLayerRenderer::~RasterLayerRenderer()
{
  delete m_rasterCanvas;
  delete m_resampledRaster;
  delete m_rasterCanvasStyled;
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
    createVisualDefault(l);
  }

  //reproject and resample input raster to raster canvas 
  buildRasterCanvas(r, canvas, bbox, srid);

  //apply the raster symbolizer
  applyStyle(l, canvas);
}

bool te::map::RasterLayerRenderer::hasIntersection(const te::gm::Envelope& bbox, te::rst::Raster* raster, Canvas* canvas,  int srid)
{
  te::gm::Envelope e(bbox);

  e.transform(srid, raster->getSRID());

  return e.intersects(*raster->getExtent());
}

void te::map::RasterLayerRenderer::buildRasterCanvas(te::rst::Raster* raster, Canvas* canvas, const te::gm::Envelope& bbox, int srid)
{
  delete m_rasterCanvas;

  std::map<std::string, std::string> rinfo;
  rinfo["FORCE_MEM_DRIVER"] = "TRUE";
  //rinfo["SOURCE"] = "d:/rasterCanvas.tif";

  if(raster->getSRID() != srid)
  {
    te::gm::Envelope* env = new te::gm::Envelope(bbox);
    env->transform(srid, raster->getSRID());

    double resx = bbox.getWidth()/canvas->getWidth();
    double resy = bbox.getHeight()/canvas->getHeight();
    
    m_rasterCanvas = raster->transform(srid, env->getLowerLeftX(), env->getLowerLeftY(), env->getUpperRightX(), env->getUpperRightY(), resx, resy, rinfo);

    delete env;
  }
  else
  {
    te::gm::Coord2D ul = raster->getGrid()->geoToGrid(bbox.getLowerLeftX(), bbox.getUpperRightY());
    te::gm::Coord2D lr = raster->getGrid()->geoToGrid(bbox.getUpperRightX(), bbox.getLowerLeftY());
    
    int r = te::rst::Round(ul.y);
    if(r < 0)
      r = 0;

    int c = te::rst::Round(ul.x);
    if(c < 0)
      c = 0;

    int ih = te::rst::Round(lr.y) - te::rst::Round(ul.y);
    if(ih + r >= raster->getNumberOfRows())
    {
      int aux = (ih + r) - raster->getNumberOfRows();
      ih = ih - aux -1;
    }
    
    int iw = te::rst::Round(lr.x) - te::rst::Round(ul.x);
    if(iw + c >= raster->getNumberOfColumns())
    {
      int aux = (iw + c) - raster->getNumberOfColumns();
      iw = iw - aux - 1;
    }

    unsigned int h = canvas->getHeight();
    unsigned int w = canvas->getWidth();

    //build raster canvas
    te::gm::Envelope* env = new te::gm::Envelope(bbox);
    te::rst::Grid* grid = new te::rst::Grid(raster->getResolutionX(), raster->getResolutionY(), env, srid);

    //copy the band definitions
    std::vector<te::rst::BandProperty*> bands;
    for (unsigned int b = 0; b < raster->getNumberOfBands(); ++b)
    {
      te::rst::BandProperty* bb = new te::rst::BandProperty(b, raster->getBand(b)->getProperty()->getType());

      bands.push_back(bb);
    }

    m_rasterCanvas = te::rst::RasterFactory::make("MEM", grid, bands, rinfo);

    applyBackgroundColor();

    te::rst::Copy(r, c, ih, iw, *raster, *m_rasterCanvas);
  }
}

void te::map::RasterLayerRenderer::createVisualDefault(RasterLayer* layer)
{
  te::rst::Raster* raster = layer->getRaster();
  assert(raster);

  //create default raster symbolizer
  te::se::RasterSymbolizer* rs = new te::se::RasterSymbolizer();

  //general paramaters
  rs->setOpacity(new te::se::ParameterValue("1.0"));
  rs->setGain(new te::se::ParameterValue("1.0"));
  rs->setOffset(new te::se::ParameterValue("0.0"));

  //set channel selection
  if(raster->getNumberOfBands() == 1)
  {
    te::se::ChannelSelection* cs = new te::se::ChannelSelection();

    cs->setColorCompositionType(te::se::GRAY_COMPOSITION);

    te::se::SelectedChannel* sc = new te::se::SelectedChannel();
    sc->setSourceChannelName("0");
    cs->setGrayChannel(sc);

    rs->setChannelSelection(cs);
  }
  else if(raster->getNumberOfBands() >= 3)
  {
    te::se::ChannelSelection* cs = new te::se::ChannelSelection();

    cs->setColorCompositionType(te::se::RGB_COMPOSITION);

    //channel R
    te::se::SelectedChannel* scR = new te::se::SelectedChannel();
    scR->setSourceChannelName("0");
    cs->setRedChannel(scR);

    //channel G
    te::se::SelectedChannel* scG = new te::se::SelectedChannel();
    scG->setSourceChannelName("1");
    cs->setGreenChannel(scG);

    //channel B
    te::se::SelectedChannel* scB = new te::se::SelectedChannel();
    scB->setSourceChannelName("2");
    cs->setBlueChannel(scB);

    rs->setChannelSelection(cs);
  }

  //add symbolizer to a layer style
  te::se::Rule* r = new te::se::Rule();
  r->push_back(rs);

  te::se::Style* s = new te::se::Style();
  s->push_back(r);

  layer->setStyle(s);
}

void te::map::RasterLayerRenderer::applyStyle(RasterLayer* layer, Canvas* canvas)
{
  delete m_rasterCanvasStyled;

  assert(m_rasterCanvas);

  //create new grid
  te::gm::Envelope* env = new te::gm::Envelope(*m_rasterCanvas->getExtent());
  te::rst::Grid* grid = new te::rst::Grid(*m_rasterCanvas->getGrid());

  //copy the band definitions
  std::vector<te::rst::BandProperty*> bands;
  for (unsigned int b = 0; b < 3; ++b)
  {
    te::rst::BandProperty* bb = new te::rst::BandProperty(b, te::dt::INT32_TYPE);

    bands.push_back(bb);
  }

  //create the styled raster
  std::map<std::string, std::string> rinfo;
  rinfo["FORCE_MEM_DRIVER"] = "TRUE";
  
  m_rasterCanvasStyled = te::rst::RasterFactory::make("MEM", grid, bands, rinfo);

  te::se::RasterSymbolizer* rs = (te::se::RasterSymbolizer*)layer->getRasterSymbolizer()->clone();

  //create the raster transform
  RasterTransform rt(m_rasterCanvas, m_rasterCanvasStyled);

  double rMin, rMax;

  getMinMaxValues(rMin, rMax, layer);

  rt.setLinearTransfParameters(0, 255, rMin, rMax);

  //configure the raster transformation using the raster symbolizer
  RasterTransformConfigurer rtc(rs, &rt);

  rtc.configure();

  //fill output raster
  for(unsigned int r = 0; r < m_rasterCanvas->getNumberOfRows(); ++r)
  {
    for(unsigned int c = 0; c < m_rasterCanvas->getNumberOfColumns(); ++c)
    {
      rt.apply(c, r, c, r);
    }
  }

  //get opacity
  std::string opacityStr = te::map::GetString(rs->getOpacity());
  double opacity = (double)TE_OPAQUE * atof(opacityStr.c_str());

  //paint raster
  canvas->drawImage(0, 0, canvas->getWidth(), canvas->getHeight(), m_rasterCanvasStyled, 
    0, 0, m_rasterCanvasStyled->getNumberOfColumns(), m_rasterCanvasStyled->getNumberOfRows(), (int)opacity);

  //verify for image outline
  if(rs->getImageOutline())
  {
    te::se::Symbolizer* s = rs->getImageOutline()->getSymbolizer();

    if(s)
    {
      // The canvas configurer
      te::map::CanvasConfigurer cc(canvas);
      cc.config(s);

      te::gm::Geometry* g = te::gm::GetGeomFromEnvelope(layer->getRaster()->getExtent(), layer->getSRID());

      canvas->draw(g);

      delete g;
    }
  }

  delete rs;
}

void te::map::RasterLayerRenderer::applyBackgroundColor()
{
  assert(m_rasterCanvas);
    
  for( unsigned int b = 0; b < m_rasterCanvas->getNumberOfBands(); ++b)
  {
    for( unsigned int r = 0; r < m_rasterCanvas->getNumberOfRows(); ++r)
    {
      for( unsigned int c = 0; c < m_rasterCanvas->getNumberOfColumns(); ++c)
      {
        m_rasterCanvas->setValue(c , r, 255., b);
      }
    }
  }
}

void te::map::RasterLayerRenderer::getMinMaxValues(double& rMin, double& rMax, RasterLayer* layer)
{
  if(layer->getRaster())
  {
    const te::rst::RasterSummary* rsMin = te::rst::RasterSummaryManager::getInstance().get(layer->getRaster(), te::rst::SUMMARY_MIN);
    const te::rst::RasterSummary* rsMax = te::rst::RasterSummaryManager::getInstance().get(layer->getRaster(), te::rst::SUMMARY_MAX);
    const std::complex<double>* cmin = rsMin->at(0).m_minVal;
    const std::complex<double>* cmax = rsMax->at(0).m_maxVal;
    rMin = cmin->real();
    rMax = cmax->real();
  }
}
