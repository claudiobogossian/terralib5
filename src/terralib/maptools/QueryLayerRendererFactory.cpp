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
  \file terralib/maptools/QueryLayerRendererFactory.cpp

  \brief This is the concrete factory for renderers of a QueryLayer.
*/

// TerraLib
#include "QueryLayerRenderer.h"
#include "QueryLayerRendererFactory.h"

te::map::QueryLayerRendererFactory* te::map::QueryLayerRendererFactory::sm_factory(0);

void te::map::QueryLayerRendererFactory::initialize()
{
  finalize();
  sm_factory = new QueryLayerRendererFactory;
}

void te::map::QueryLayerRendererFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

te::map::QueryLayerRendererFactory::~QueryLayerRendererFactory()
{
}

te::map::AbstractRenderer* te::map::QueryLayerRendererFactory::build()
{
  return new QueryLayerRenderer;
}

te::map::QueryLayerRendererFactory::QueryLayerRendererFactory()
  : RendererFactory("QUERY_LAYER_RENDERER")
{
}
