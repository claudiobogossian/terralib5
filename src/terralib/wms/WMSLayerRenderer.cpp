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
  \file terralib/wms/WMSLayerRenderer.cpp

  \brief It renders the data associated to a WMS layer.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/utils/Utils.h"
#include "../geometry/Envelope.h"
#include "../maptools/Canvas.h"
#include "../maptools/Utils.h"
#include "../raster/RasterProperty.h"
#include "../se/CoverageStyle.h"
#include "../se/Utils.h"
#include "../srs/Config.h"
#include "Exception.h"
#include "WMSLayer.h"
#include "WMSLayerRenderer.h"

// Boost
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// STL
#include <cassert>
#include <memory>

te::wms::WMSLayerRenderer::WMSLayerRenderer()
{
}

te::wms::WMSLayerRenderer::~WMSLayerRenderer()
{
}

void te::wms::WMSLayerRenderer::draw(te::map::AbstractLayer* layer,
                                     te::map::Canvas* canvas,
                                     const te::gm::Envelope& bbox,
                                     int srid)
{
  if(!bbox.isValid())
    throw Exception(TR_WMS("The requested box is invalid!"));

  assert(layer);
  assert(canvas);

  // Should I render this layer?
  WMSLayer* wmsLayer = dynamic_cast<WMSLayer*>(layer);

  if(wmsLayer == 0)
    throw Exception(TR_WMS("Wrong type render type for this layer!"));

  draw(wmsLayer, canvas, bbox, srid);
}

void te::wms::WMSLayerRenderer::draw(WMSLayer* layer, te::map::Canvas* canvas, const te::gm::Envelope& bbox, int srid)
{
  // Check if layer extent intersects the drawing area and so compute bounding box intersection
  te::gm::Envelope reprojectedBBOX(bbox);

  if((layer->getSRID() != TE_UNKNOWN_SRS) && (srid != TE_UNKNOWN_SRS))
  {
    reprojectedBBOX.transform(srid, layer->getSRID());
  }
  else if(layer->getSRID() != srid)
  {
    throw Exception(TR_WMS("The layer or map don't have a valid SRID!"));
  }

  if(!reprojectedBBOX.intersects(layer->getExtent()))
    return;

  te::gm::Envelope ibbox = reprojectedBBOX.intersection(layer->getExtent());
  assert(ibbox.isValid());

   // Gets the layer schema
  std::auto_ptr<te::map::LayerSchema> schema(layer->getSchema());
  assert(schema.get());

  // Get the raster property
  te::rst::RasterProperty* rasterProperty = te::da::GetFirstRasterProperty(schema.get());

  // Get the layer style
  te::se::Style* style = layer->getStyle();
  if(style == 0)
  {
    // Try create an appropriate style. i.e. a CoverageStyle
    style = te::se::CreateCoverageStyle(rasterProperty->getBandProperties());

    if(style == 0)
      throw Exception((boost::format(TR_WMS("Could not create a default coverage style to the layer %1%.")) % layer->getTitle()).str());

    layer->setStyle(style);
  }

  // Should I render this style?
  te::se::CoverageStyle* cs = dynamic_cast<te::se::CoverageStyle*>(style);
  if(cs == 0)
    throw Exception(TR_WMS("The layer style is not a Coverage Style!"));

  // Setup the WMS Layer to request data
  layer->setWidth(canvas->getWidth());
  layer->setHeight(canvas->getHeight());

  // Request the WMS data
  std::auto_ptr<te::da::DataSet> dataset = layer->getData(rasterProperty->getName(), &ibbox, te::gm::INTERSECTS);

  if(dataset.get() == 0)
    throw Exception((boost::format(TR_WMS("Could not retrieve the WMS data from the layer %1%.")) % layer->getTitle()).str());

  std::size_t rpos = te::da::GetFirstPropertyPos(dataset.get(), te::dt::RASTER_TYPE);

  // Retrieve the raster
  std::auto_ptr<te::rst::Raster> raster(dataset->getRaster(rpos));
  if(dataset.get() == 0)
    throw Exception((boost::format(TR_WMS("Could not retrieve the WMS data from the layer %1%.")) % layer->getTitle()).str());

  // Let's draw!
  te::map::DrawRaster(raster.get(), canvas, ibbox, layer->getSRID(), bbox, srid, cs);
}
