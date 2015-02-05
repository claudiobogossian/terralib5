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
  \file terralib/maptools/AbstractLayerRendererFactory.cpp

  \brief This is the concrete factory for renderers of AbstractLayers. i.e. builds a generic renderer.
*/

// TerraLib
#include "AbstractLayerRenderer.h"
#include "AbstractLayerRendererFactory.h"

te::map::AbstractLayerRendererFactory* te::map::AbstractLayerRendererFactory::sm_factory(0);

void te::map::AbstractLayerRendererFactory::initialize()
{
  finalize();
  sm_factory = new AbstractLayerRendererFactory;
}

void te::map::AbstractLayerRendererFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

te::map::AbstractLayerRendererFactory::~AbstractLayerRendererFactory()
{
}

te::map::AbstractRenderer* te::map::AbstractLayerRendererFactory::build()
{
  return new AbstractLayerRenderer;
}

te::map::AbstractLayerRendererFactory::AbstractLayerRendererFactory()
  : RendererFactory("ABSTRACT_LAYER_RENDERER")
{
}
