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
  \file QueryLayerRenderer.cpp

  \brief It renders the objects associated to a QueryLayer.
 */

// TerraLib
#include "../geometry/Envelope.h"
#include "../se/CoverageStyle.h"
#include "../se/FeatureTypeStyle.h"
#include "../se/Style.h"
#include "AbstractLayer.h"
#include "Canvas.h"
#include "QueryLayer.h"
#include "QueryLayerRenderer.h"

te::map::QueryLayerRenderer::QueryLayerRenderer()
{}

te::map::QueryLayerRenderer::~QueryLayerRenderer()
{}

void te::map::QueryLayerRenderer::draw(AbstractLayer* /*layer*/, Canvas* /*canvas*/,
                                       const te::gm::Envelope& /*bbox*/, int /*srid*/)
{}

void te::map::QueryLayerRenderer::draw(QueryLayer* /*layer*/, te::se::FeatureTypeStyle* /*style*/, Canvas* /*canvas*/, 
                                       const te::gm::Envelope& /*bbox*/, int /*srid*/)
{}

void te::map::QueryLayerRenderer::draw(QueryLayer* /*layer*/, te::se::CoverageStyle* /*style*/, Canvas* /*canvas*/,
                                       const te::gm::Envelope& /*bbox*/, int /*srid*/)
{}

