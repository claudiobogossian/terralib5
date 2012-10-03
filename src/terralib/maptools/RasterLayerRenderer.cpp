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
#include "../se.h"
#include "AbstractLayer.h"
#include "Canvas.h"
#include "RasterLayer.h"
#include "RasterLayerRenderer.h"
#include "Utils.h"


te::map::RasterLayerRenderer::RasterLayerRenderer(): _rasterCanvas(0), _rasterCanvasStyled(0), _resampledRaster(0)
{
}

te::map::RasterLayerRenderer::~RasterLayerRenderer()
{
  delete _rasterCanvas;
  delete _resampledRaster;
  delete _rasterCanvasStyled;
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

  //get resampled input raster
 // getResampledRaster(r, canvas, bbox, srid);

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
  rinfo["MEM_SRC_RASTER_DRIVER_TYPE"] = "GDAL";
  rinfo["SOURCE"] = "d:/rasterCanvas.tif";

  if(raster->getSRID() != srid)
  {
    te::gm::Envelope* env = new te::gm::Envelope(bbox);
    env->transform(srid, raster->getSRID());

    double resx = bbox.getWidth()/canvas->getWidth();
    double resy = bbox.getHeight()/canvas->getHeight();
    
    _rasterCanvas = raster->transform(srid, env->getLowerLeftX(), env->getLowerLeftY(), env->getUpperRightX(), env->getUpperRightY(), resx, resy, rinfo);

    delete env;
  }
  else
  {
    te::gm::Coord2D ll = raster->getGrid()->geoToGrid(bbox.getLowerLeftX(), bbox.getLowerLeftY());
    te::gm::Coord2D ur = raster->getGrid()->geoToGrid(bbox.getUpperRightX(), bbox.getUpperRightY());
    
    int r = ll.x;
    int c = ur.y;
    int ih = ll.y - ur.y;
    int iw = ur.x - ll.x;

    _rasterCanvas = raster->resample(1, r, c, ih, iw, canvas->getHeight(), canvas->getWidth(), rinfo);
  }
}

void te::map::RasterLayerRenderer::createVisualDefault(RasterLayer* layer)
{
  te::rst::Raster* raster = layer->getRaster();
  assert(raster);

  //create default raster symbolizer
  te::se::RasterSymbolizer* rs = new te::se::RasterSymbolizer();

  //set transparency
  rs->setOpacity(new te::se::ParameterValue("1.0"));

  //set channel selection
  if(raster->getNumberOfBands() == 1)
  {
    te::se::ChannelSelection* cs = new te::se::ChannelSelection();

    te::se::SelectedChannel* sc = new te::se::SelectedChannel();
    sc->setSourceChannelName("0");
    cs->setGrayChannel(sc);

    rs->setChannelSelection(cs);
  }
  else if(raster->getNumberOfBands() >= 3)
  {
    te::se::ChannelSelection* cs = new te::se::ChannelSelection();

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


/*
 “Normalize” means to stretch the contrast so that the dimmest color is stretched to black 
 and the brightest color is stretched to white, with all colors in between stretched out linearly. 
 
 “Histogram” means to stretch the contrast based on a histogram of how many colors are at each 
 brightness level on input, with the goal of producing equal number of pixels in the image at each 
 brightness level on output. This has the effect of revealing many subtle ground features. 
 
 A “GammaValue” tells how much to brighten (values greater than 1.0) or dim (values less than 1.0) 
 an image. The default GammaValue is 1.0 (no change). If none of Normalize, Histogram, or GammaValue 
 are selected in a ContrastEnhancement, then no enhancement is performed.
*/

void te::map::RasterLayerRenderer::applyStyle(RasterLayer* layer, Canvas* canvas)
{
  delete _rasterCanvasStyled;

  assert(_rasterCanvas);

  te::se::RasterSymbolizer* rs = (te::se::RasterSymbolizer*)layer->getRasterSymbolizer()->clone();

  //create new grid
  te::gm::Envelope* env = new te::gm::Envelope(*_rasterCanvas->getExtent());
  te::rst::Grid* grid = new te::rst::Grid(_rasterCanvas->getResolutionX(), _rasterCanvas->getResolutionY(), env, _rasterCanvas->getSRID());

  //copy the band definitions
  std::vector<te::rst::BandProperty*> bands;
  for (unsigned int b = 0; b < 3; ++b)
  {
    te::rst::BandProperty* bb = new te::rst::BandProperty(b, te::dt::INT32_TYPE);

    bands.push_back(bb);
  }

  //create the styled raster
  _rasterCanvasStyled = te::rst::RasterFactory::make("MEM", grid, bands, std::map<std::string, std::string>());


  assert(rs->getChannelSelection());

  bool isMono = false;
  int mBand, rBand, gBand, bBand;
  double mGamma, rGamma, gGamma, bGamma;
  if(rs->getChannelSelection()->getGrayChannel())
  {
    isMono = true;
    mBand = atoi(rs->getChannelSelection()->getGrayChannel()->getSourceChannelName().c_str()); //MUST VERIFY THIS

    if(rs->getChannelSelection()->getGrayChannel()->getContrastEnhancement())
    {
      mGamma = rs->getChannelSelection()->getGrayChannel()->getContrastEnhancement()->getGammaValue();
    }
    else
    {
      mGamma = TE_SE_DEFAULT_GAMMA_VALUE;
    }
  }
  else
  {
    if(rs->getChannelSelection()->getRedChannel())
    {
      rBand = atoi(rs->getChannelSelection()->getRedChannel()->getSourceChannelName().c_str()); //MUST VERIFY THIS

      if(rs->getChannelSelection()->getRedChannel()->getContrastEnhancement())
      {
        rGamma = rs->getChannelSelection()->getRedChannel()->getContrastEnhancement()->getGammaValue();
      }
      else
      {
        rGamma = TE_SE_DEFAULT_GAMMA_VALUE;
      }
    }

    if(rs->getChannelSelection()->getGreenChannel())
    {
      gBand = atoi(rs->getChannelSelection()->getGreenChannel()->getSourceChannelName().c_str()); //MUST VERIFY THIS

      if(rs->getChannelSelection()->getGreenChannel()->getContrastEnhancement())
      {
        gGamma = rs->getChannelSelection()->getGreenChannel()->getContrastEnhancement()->getGammaValue();
      }
      else
      {
        gGamma = TE_SE_DEFAULT_GAMMA_VALUE;
      }
    }

    if(rs->getChannelSelection()->getBlueChannel())
    {
      bBand = atoi(rs->getChannelSelection()->getBlueChannel()->getSourceChannelName().c_str()); //MUST VERIFY THIS

      if(rs->getChannelSelection()->getBlueChannel()->getContrastEnhancement())
      {
        bGamma = rs->getChannelSelection()->getBlueChannel()->getContrastEnhancement()->getGammaValue();
      }
      else
      {
        bGamma = TE_SE_DEFAULT_GAMMA_VALUE;
      }
    }
  }


  for(unsigned int r = 0; r < _rasterCanvas->getNumberOfRows(); ++r)
  {
    for(unsigned int c = 0; c < _rasterCanvas->getNumberOfColumns(); ++c)
    {
      if(isMono)
      {
        double val = 0.;
        _rasterCanvas->getValue(c, r, val, mBand);

        _rasterCanvasStyled->setValue(c, r, val * mGamma, 0);
        _rasterCanvasStyled->setValue(c, r, val * mGamma, 1);
        _rasterCanvasStyled->setValue(c, r, val * mGamma, 2);
      }
      else
      {
        if(rs->getChannelSelection()->getRedChannel())
        {
          double val = 0.;
          _rasterCanvas->getValue(c, r, val, rBand);
          _rasterCanvasStyled->setValue(c, r, val * rGamma, 0);
        }

        if(rs->getChannelSelection()->getGreenChannel())
        {
          double val = 0.;
          _rasterCanvas->getValue(c, r, val, gBand);
          _rasterCanvasStyled->setValue(c, r, val * gGamma, 1);
        }

        if(rs->getChannelSelection()->getBlueChannel())
        {
          double val = 0.;
          _rasterCanvas->getValue(c, r, val, bBand);
          _rasterCanvasStyled->setValue(c, r, val * bGamma, 2);
        }
      }
    }
  }

  //get opacity
  std::string opacityStr = te::map::GetString(rs->getOpacity());
  double opacity = TE_OPAQUE * atof(opacityStr.c_str());

  //paint raster
  canvas->drawImage(0, 0, canvas->getWidth(), canvas->getHeight(), _rasterCanvasStyled, 
    0, 0, _rasterCanvasStyled->getNumberOfColumns(), _rasterCanvasStyled->getNumberOfRows(), (int)opacity);

  delete rs;
}
