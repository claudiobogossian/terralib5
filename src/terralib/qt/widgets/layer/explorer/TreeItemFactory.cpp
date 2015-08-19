/*  Copyright (C) 2001-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/layer/explorer/AbstractTreeItemFactory.cpp

  \brief A class for building layer items.
*/

#include "TreeItemFactory.h"

// TerraLib
#include "../../../../maptools/AbstractLayer.h"

#include "LayerItem.h"
#include "FolderItem.h"

// Boost
#include <boost/functional/factory.hpp>
#include <boost/bind.hpp>

std::map<std::string, te::qt::widgets::TreeItemFactory::FactoryFnctType> te::qt::widgets::TreeItemFactory::sm_factories;

void te::qt::widgets::TreeItemFactory::reg(const std::string& key, const FactoryFnctType& f)
{
  sm_factories[key] = f;
}

void te::qt::widgets::TreeItemFactory::initialize()
{
  if(!sm_factories.empty())
    return;
}

void te::qt::widgets::TreeItemFactory::finalize()
{
  sm_factories.clear();
}

te::qt::widgets::TreeItem* te::qt::widgets::TreeItemFactory::make(const te::map::AbstractLayerPtr& layer)
{
  if(layer->getType() == "DATASETLAYER" || layer->getType() == "DATASETADAPTERLAYER" || layer->getType() == "QUERYLAYER" || layer->getType() == "RASTERLAYER")
    return new LayerItem(layer);

  if(layer->getType() == "FOLDERLAYER")
    return new FolderItem(layer->getTitle());

  if(sm_factories.find(layer->getType()) == sm_factories.end())
    return new LayerItem(layer);

  return sm_factories[layer->getType()](layer);
}

