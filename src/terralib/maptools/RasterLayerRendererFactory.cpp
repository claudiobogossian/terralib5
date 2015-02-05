/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/maptools/RasterLayerRendererFactory.cpp

  \brief This is the concrete factory for renderers of a RasterLayer.
*/

// TerraLib
#include "RasterLayerRenderer.h"
#include "RasterLayerRendererFactory.h"

te::map::RasterLayerRendererFactory* te::map::RasterLayerRendererFactory::sm_factory(0);

void te::map::RasterLayerRendererFactory::initialize()
{
  finalize();
  sm_factory = new RasterLayerRendererFactory;
}

void te::map::RasterLayerRendererFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}


te::map::RasterLayerRendererFactory::~RasterLayerRendererFactory()
{
}

te::map::AbstractRenderer* te::map::RasterLayerRendererFactory::build()
{
  return new RasterLayerRenderer;
}

te::map::RasterLayerRendererFactory::RasterLayerRendererFactory()
  : RendererFactory("RASTER_LAYER_RENDERER")
{
}
