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
#include "../common/progress/TaskProgress.h"
#include "../dataaccess.h"
#include "../fe/Filter.h"
#include "../geometry.h"
#include "../raster.h"
#include "../raster/RasterSummary.h"
#include "../raster/RasterSummaryManager.h"
#include "../se.h"
#include "../srs/Converter.h"
#include "AbstractLayer.h"
#include "Canvas.h"
#include "CanvasConfigurer.h"
#include "RasterLayer.h"
#include "RasterLayerRenderer.h"
#include "RasterTransform.h"
#include "RasterTransformConfigurer.h"
#include "Utils.h"


te::map::RasterLayerRenderer::RasterLayerRenderer(): m_gridCanvas(0)
{
}

te::map::RasterLayerRenderer::~RasterLayerRenderer()
{
  delete m_gridCanvas;
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

  //create raster canvas with width and height from canvas 
  buildRasterCanvas(canvas, bbox, srid);

  //apply the raster symbolizer
  applyStyle(l, canvas, srid);
}

bool te::map::RasterLayerRenderer::hasIntersection(const te::gm::Envelope& bbox, te::rst::Raster* raster, Canvas* canvas,  int srid)
{
  te::gm::Envelope e(bbox);

  e.transform(srid, raster->getSRID());

  return e.intersects(*raster->getExtent());
}

void te::map::RasterLayerRenderer::buildRasterCanvas(Canvas* canvas, const te::gm::Envelope& bbox, int srid)
{
  delete m_gridCanvas;

  unsigned int h = canvas->getHeight();
  unsigned int w = canvas->getWidth();

  //build raster canvas
  te::gm::Envelope* env = new te::gm::Envelope(bbox);
  m_gridCanvas = new te::rst::Grid(w, h, env, srid);
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

  te::se::Style* s = new te::se::CoverageStyle();
  s->push_back(r);

  layer->setStyle(s);
}

void te::map::RasterLayerRenderer::applyStyle(RasterLayer* layer, Canvas* canvas, int srid)
{
  te::se::RasterSymbolizer* rs = (te::se::RasterSymbolizer*)layer->getRasterSymbolizer()->clone();
    
  te::rst::Raster* raster = layer->getRaster();

  //create the raster transform
  RasterTransform rt(raster, 0);

  double rMin, rMax;

  getMinMaxValues(rMin, rMax, layer);

  rt.setLinearTransfParameters(0, 255, rMin, rMax);

  //configure the raster transformation using the raster symbolizer
  RasterTransformConfigurer rtc(rs, &rt);

  rtc.configure();

  bool reproject = false;
  if(srid != layer->getSRID())
  {
    reproject = true;
  }

  te::common::TaskProgress t;
  t.setMessage(TR_MAP("Drawing Raster..."));
  t.setTotalSteps(m_gridCanvas->getNumberOfRows());

  te::color::RGBAColor** rgba = new te::color::RGBAColor*[m_gridCanvas->getNumberOfRows()];

  std::auto_ptr<te::srs::Converter> converter(new te::srs::Converter());
  converter->setSourceSRID(srid);
  converter->setTargetSRID(layer->getSRID());

  for (unsigned r = 0; r < m_gridCanvas->getNumberOfRows(); r++)
  {
    if(t.isActive() == false)
    {
      break;
    }

    te::color::RGBAColor* s = new te::color::RGBAColor[m_gridCanvas->getNumberOfColumns()];

    for (unsigned c = 0; c < m_gridCanvas->getNumberOfColumns(); c++)
    {
      te::gm::Coord2D inputGeo = m_gridCanvas->gridToGeo(c, r);

      if(reproject)
      {
        converter->convert(inputGeo.x, inputGeo.y, inputGeo.x, inputGeo.y);
      }

      te::gm::Coord2D outputGrid = raster->getGrid()->geoToGrid(inputGeo.x, inputGeo.y);

      int x = te::rst::Round(outputGrid.x);
      int y = te::rst::Round(outputGrid.y);

      te::color::RGBAColor color(255, 255, 255, 0);

      if((x >=0 && x < (int)raster->getNumberOfColumns()) && (y >=0 && y < (int)raster->getNumberOfRows()))
      {
         color = rt.apply(x, y);
      }

      s[c] = color;
    }

    rgba[r] = s;

    t.pulse();
  }

  canvas->drawImage(0, 0, rgba, canvas->getWidth(), canvas->getHeight());

  te::common::Free(rgba, m_gridCanvas->getNumberOfRows());

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
      
      if(srid != layer->getSRID())
      {
        g->transform(srid);
      }

      canvas->draw(g);

      delete g;
    }
  }

  delete rs;
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
