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
  \file terralib/qt/widgets/layer/explorer/AbstractLayerTreeItemFactory.cpp

  \brief A class for building layer items.
*/

// TerraLib
#include "../../maptools/AbstractLayer.h"
#include "../../maptools/DataSetLayer.h"
#include "../../maptools/FolderLayer.h"
#include "../../maptools/QueryLayer.h"
#include "AbstractLayerTreeItemFactory.h"
#include "DataSetLayerItem.h"
#include "FolderLayerItem.h"
#include "QueryLayerItem.h"

// Boost
#include <boost/functional/factory.hpp>
#include <boost/bind.hpp>

std::map<std::string, te::vp::AbstractLayerTreeItemFactory::FactoryFnctType> te::vp::AbstractLayerTreeItemFactory::sm_factories;


void te::vp::AbstractLayerTreeItemFactory::initialize()
{
  if(!sm_factories.empty())
    return;

  sm_factories["DATASETLAYER"] =  boost::bind(boost::factory<DataSetLayerItem*>(),_1, _2);
  sm_factories["QUERYLAYER"] =  boost::bind(boost::factory<QueryLayerItem*>(),_1, _2);
  sm_factories["FOLDERLAYER"] =  boost::bind(boost::factory<FolderLayerItem*>(),_1, _2);
}

void te::vp::AbstractLayerTreeItemFactory::finalize()
{
  sm_factories.clear();
}

