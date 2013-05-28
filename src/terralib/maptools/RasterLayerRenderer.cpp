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
  \file terralib/maptools/RasterLayerRenderer.cpp

  \brief It renders the objects associated to a dataset layer.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "../common/progress/TaskProgress.h"
#include "../geometry/Envelope.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Geometry.h"
#include "../geometry/Utils.h"
#include "../raster/Grid.h"
#include "../raster/Raster.h"
#include "../raster/RasterProperty.h"
#include "../raster/RasterSummary.h"
#include "../raster/RasterSummaryManager.h"
#include "../raster/Utils.h"
#include "../se/CoverageStyle.h"
#include "../se/FeatureTypeStyle.h"
#include "../se/ImageOutline.h"
#include "../se/RasterSymbolizer.h"
#include "../se/Rule.h"
#include "../se/Utils.h"
#include "../srs/Config.h"
#include "../srs/Converter.h"
#include "Canvas.h"
#include "CanvasConfigurer.h"
#include "Exception.h"
#include "RasterLayer.h"
#include "RasterLayerRenderer.h"
#include "RasterTransform.h"
#include "RasterTransformConfigurer.h"

// Boost
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// STL
#include <cassert>
#include <memory>

te::map::RasterLayerRenderer::RasterLayerRenderer()
{
}

te::map::RasterLayerRenderer::~RasterLayerRenderer()
{
}

void te::map::RasterLayerRenderer::draw(AbstractLayer* layer,
                                         Canvas* canvas,
                                         const te::gm::Envelope& bbox,
                                         int srid)
{
// should I render this layer?
  RasterLayer* dlayer = dynamic_cast<RasterLayer*>(layer);

  if(dlayer == 0)
    throw Exception(TR_MAP("Wrong type render type for this layer!"));

// check if layer extent intersects the drawing area and so compute bounding box intersection
  te::gm::Envelope reprojectedBBOX(bbox);

  if((dlayer->getSRID() != TE_UNKNOWN_SRS) && (srid != TE_UNKNOWN_SRS))
  {
    reprojectedBBOX.transform(srid, dlayer->getSRID());
  }
  else if(dlayer->getSRID() != srid)
  {
    throw Exception(TR_MAP("The layer or map don't have a valid SRID!"));
  }

  if(!reprojectedBBOX.intersects(dlayer->getExtent()))
    return;

  te::gm::Envelope ibbox = reprojectedBBOX.intersection(dlayer->getExtent());

  drawRaster(dlayer, canvas, reprojectedBBOX, bbox, srid);
}

void te::map::RasterLayerRenderer::drawRaster(RasterLayer* layer,
                                              Canvas* canvas,
                                              const te::gm::Envelope& bbox,
                                              const te::gm::Envelope& visibleArea,
                                              int srid)
{
  te::rst::Raster* raster = layer->getRaster();

// get the associated layer style
  te::se::RasterSymbolizer* style = layer->getStyle();
  if(style == 0)
  {
// try create an appropriate style
    style = (te::se::RasterSymbolizer*)te::se::CreateCoverageStyle(raster->getNumberOfBands());

    if(style == 0)
      throw Exception((boost::format(TR_MAP("Could not create a default coverage style to the layer %1%.")) % layer->getTitle()).str());

    layer->setStyle(style);
  }

// build the grid canvas. i.e. a grid with canvas dimensions and requested mbr
  te::gm::Envelope* gmbr = new te::gm::Envelope(visibleArea);
  std::auto_ptr<te::rst::Grid> gridCanvas(new te::rst::Grid(static_cast<unsigned int>(canvas->getWidth()), static_cast<unsigned int>(canvas->getHeight()), gmbr, srid));

// create a raster transform
  RasterTransform rasterTransform(raster, 0);

// configure the raster transformation based on the raster symbolizer
  RasterTransformConfigurer rtc(style, &rasterTransform);
  rtc.configure();

//check band data type
  if(raster->getBandDataType(0) != te::dt::UCHAR_TYPE)
  {
    // raster min / max values
    const te::rst::RasterSummary* rsMin = te::rst::RasterSummaryManager::getInstance().get(raster, te::rst::SUMMARY_MIN);
    const te::rst::RasterSummary* rsMax = te::rst::RasterSummaryManager::getInstance().get(raster, te::rst::SUMMARY_MAX);
    const std::complex<double>* cmin = rsMin->at(0).m_minVal;
    const std::complex<double>* cmax = rsMax->at(0).m_maxVal;
    double min = cmin->real();
    double max = cmax->real();

    // *** aqui temos a questão da variável global que diz se é para normalizar ou não os valores do raster ***
    rasterTransform.setLinearTransfParameters(min, max, 0, 255);
  }

// verify if is necessary convert the raster to the given srid
  bool needRemap = false;
  if((layer->getSRID() != TE_UNKNOWN_SRS) && (srid != TE_UNKNOWN_SRS) && (layer->getSRID() != srid))
    needRemap = true;

// build task message; e.g. ("Drawing the layer cbers_sao_jose_dos_campos.")
  std::string message = TR_MAP("Drawing the layer");
  message += " " + layer->getTitle() + ". ";

// create the draw task
  te::common::TaskProgress task(message, te::common::TaskProgress::DRAW, gridCanvas->getNumberOfRows());

// create a RGBA array that will be drawn in the canvas. i.e. This array represents a row of the render process.
  te::color::RGBAColor** row = new te::color::RGBAColor*[1];
  te::color::RGBAColor* columns = new te::color::RGBAColor[gridCanvas->getNumberOfColumns()];
  row[0] = columns;

// create a SRS converter
  std::auto_ptr<te::srs::Converter> converter(new te::srs::Converter());
    
  if(needRemap)
  {
    converter->setSourceSRID(srid);
    converter->setTargetSRID(layer->getSRID());
  }

// fill the result RGBA array
  for(unsigned int r = 0; r < gridCanvas->getNumberOfRows(); ++r)
  {
    for(unsigned int c = 0; c < gridCanvas->getNumberOfColumns(); ++c)
    {
      te::gm::Coord2D inputGeo = gridCanvas->gridToGeo(c, r);

      if(needRemap)
        converter->convert(inputGeo.x, inputGeo.y, inputGeo.x, inputGeo.y);

      te::gm::Coord2D outputGrid = raster->getGrid()->geoToGrid(inputGeo.x, inputGeo.y);

      // TODO: round or truncate?
      int x = te::rst::Round(outputGrid.x);
      int y = te::rst::Round(outputGrid.y);

      te::color::RGBAColor color(255, 255, 255, 0);

      if((x >= 0 && x < (int)(raster->getNumberOfColumns())) &&
         (y >= 0 && y < (int)(raster->getNumberOfRows())))
        color = rasterTransform.apply(x, y);

      columns[c] = color;
    }

    if(!task.isActive())
    {
// draw the part of result
      canvas->drawImage(0, r, row, canvas->getWidth(), 1);

// free memory
      te::common::Free(row, 1);

      return;
    }

    canvas->drawImage(0, r, row, canvas->getWidth(), 1);

    task.pulse();
  }

// free memory
  te::common::Free(row, 1);

// image outline
  if(style->getImageOutline() == 0)
    return;

// get the symbolizer that will be used to draw the image outline
  te::se::Symbolizer* symbolizer = style->getImageOutline()->getSymbolizer();
  if(symbolizer == 0)
    return;

// create a canvas configurer
  te::map::CanvasConfigurer cc(canvas);
  cc.config(symbolizer);

// creates the image outline
  std::auto_ptr<te::gm::Geometry> geom(te::gm::GetGeomFromEnvelope(raster->getExtent(), layer->getSRID()));
  if(needRemap)
  {
    geom->setSRID(layer->getSRID());
    geom->transform(srid);
  }

  canvas->draw(geom.get());

  delete raster;
}
