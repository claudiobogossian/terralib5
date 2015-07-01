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
  \file terralib/qt/widgets/layer/explorer/AbstractTreeItemFactory.cpp

  \brief A class for building layer items.
*/

// TerraLib
#include "../../../../maptools/AbstractLayer.h"
#include "../../../../maptools/DataSetLayer.h"
#include "../../../../maptools/DataSetAdapterLayer.h"
#include "../../../../maptools/FolderLayer.h"
#include "../../../../maptools/QueryLayer.h"
#include "../../../../maptools/RasterLayer.h"
#include "AbstractTreeItemFactory.h"
#include "DataSetLayerItem.h"
#include "DataSetAdapterLayerItem.h"
#include "FolderLayerItem.h"
#include "QueryLayerItem.h"
#include "RasterLayerItem.h"

// Boost
#include <boost/functional/factory.hpp>
#include <boost/bind.hpp>

std::map<std::string, te::qt::widgets::AbstractTreeItemFactory::FactoryFnctType> te::qt::widgets::AbstractTreeItemFactory::sm_factories;

void te::qt::widgets::AbstractTreeItemFactory::reg(const std::string& key, const FactoryFnctType& f)
{
  sm_factories[key] = f;
}

void te::qt::widgets::AbstractTreeItemFactory::initialize()
{
  if(!sm_factories.empty())
    return;

  sm_factories["DATASETLAYER"] =  boost::bind(boost::factory<DataSetLayerItem*>(),_1, _2);
  sm_factories["DATASETADAPTERLAYER"] =  boost::bind(boost::factory<DataSetAdapterLayerItem*>(),_1, _2);
  sm_factories["QUERYLAYER"] =  boost::bind(boost::factory<QueryLayerItem*>(),_1, _2);
  sm_factories["FOLDERLAYER"] =  boost::bind(boost::factory<FolderLayerItem*>(),_1, _2);
  sm_factories["RASTERLAYER"] =  boost::bind(boost::factory<RasterLayerItem*>(),_1, _2);
}

void te::qt::widgets::AbstractTreeItemFactory::finalize()
{
  sm_factories.clear();
}

