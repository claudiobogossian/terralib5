/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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

  \brief It renders the objects associated to a raster layer.
*/

// TerraLib
#include "../common/Translator.h"
#include "../geometry/Envelope.h"
#include "../raster/Raster.h"
#include "../se/CoverageStyle.h"
#include "../se/Utils.h"
#include "../srs/Config.h"
#include "Exception.h"
#include "RasterLayer.h"
#include "RasterLayerRenderer.h"
#include "Utils.h"

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
  RasterLayer* rlayer = dynamic_cast<RasterLayer*>(layer);

  if(rlayer == 0)
    throw Exception(TE_TR("Wrong type render type for this layer!"));

// check if layer extent intersects the drawing area and so compute bounding box intersection
  te::gm::Envelope reprojectedBBOX(bbox);

  if((rlayer->getSRID() != TE_UNKNOWN_SRS) && (srid != TE_UNKNOWN_SRS))
  {
    reprojectedBBOX.transform(srid, rlayer->getSRID());
  }
  else if(rlayer->getSRID() != srid)
  {
    throw Exception(TE_TR("The layer or map don't have a valid SRID!"));
  }

  if(!reprojectedBBOX.intersects(rlayer->getExtent()))
    return;

  te::gm::Envelope ibbox = reprojectedBBOX.intersection(rlayer->getExtent());

  std::auto_ptr<te::rst::Raster> raster(rlayer->getRaster());

// get the associated layer style
  te::se::Style* style = rlayer->getStyle();
  if(style == 0)
  {
// try create an appropriate style
    style = te::se::CreateCoverageStyle(raster->getNumberOfBands());

    if(style == 0)
      throw Exception((boost::format(TE_TR("Could not create a default coverage style to the layer %1%.")) % layer->getTitle()).str());

    rlayer->setStyle(style);
  }

// should I render this style?
  te::se::CoverageStyle* cs = dynamic_cast<te::se::CoverageStyle*>(style);
  if(cs == 0)
    throw Exception(TE_TR("The layer style is not a Coverage Style!"));

  DrawRaster(raster.get(), canvas, ibbox, rlayer->getSRID(), bbox, srid, dynamic_cast<te::se::CoverageStyle*>(style));
}
