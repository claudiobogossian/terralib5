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
  \file LayerItemFactory.cpp

  \brief This is the concrete factory for LayerItem objects.  
 */

// TerraLib
#include "../../../maptools/Layer.h"
#include "LayerItem.h"
#include "LayerItemFactory.h"

te::qt::widgets::LayerItemFactory* te::qt::widgets::LayerItemFactory::sm_factory(0);

te::qt::widgets::LayerItemFactory::~LayerItemFactory()
{}

void te::qt::widgets::LayerItemFactory::initialize()
{
  finalize();
  sm_factory = new LayerItemFactory;
}

void te::qt::widgets::LayerItemFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

te::qt::widgets::AbstractLayerItem* te::qt::widgets::LayerItemFactory::build(std::pair<te::map::AbstractLayer*, te::qt::widgets::AbstractLayerItem*> p)
{
  return new LayerItem(static_cast<te::map::Layer*>(p.first), p.second);
}

te::qt::widgets::LayerItemFactory::LayerItemFactory()
  : te::qt::widgets::AbstractLayerItemFactory(te::map::Layer::sm_type)
{}
