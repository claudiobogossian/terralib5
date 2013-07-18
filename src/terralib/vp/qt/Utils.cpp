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
  \file terralib/vp/qt/Utils.cpp
   
  \brief Utility functions for Vector Processing.
*/

// TerraLib
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../geometry/Geometry.h"
#include "../../geometry/GeometryCollection.h"
#include "../../geometry/GeometryProperty.h"
#include "Utils.h"

// Qt
#include <QtGui/QTreeView>

//STL
#include <vector>

// Boost
#include <boost/algorithm/string.hpp>

std::list<te::map::AbstractLayerPtr> te::vp::GetFilteredLayers(std::string text, std::list<te::map::AbstractLayerPtr> layers)
{
  std::list<te::map::AbstractLayerPtr> filteredList;

  for(std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin(); it != layers.end(); ++it)
  {
    std::string layName = (*it)->getTitle().substr(0, text.size());
    
    if(boost::to_upper_copy(layName) == boost::to_upper_copy(text))
      filteredList.push_back(*it);
  }

  return filteredList;
}

te::gm::Geometry* te::vp::GetUnionGeometry(const std::vector<te::mem::DataSetItem*>& items, size_t geomIdx)
{
  te::gm::Geometry* resultGeometry = 0; 

  te::gm::Geometry* seedGeometry = items[0]->getGeometry(geomIdx);
  
  if(items.size() < 2)
  {
      resultGeometry = seedGeometry;
  }
  if(items.size() == 2)
  {
    te::gm::Geometry* teGeom = items[1]->getGeometry(geomIdx);

    if(items[1]->getGeometry(geomIdx)->isValid())
      resultGeometry = seedGeometry->Union(teGeom);
    else
      resultGeometry = seedGeometry;
  }
  if(items.size() > 2)
  {
    te::gm::GeometryCollection* teGeomColl = new te::gm::GeometryCollection(0, te::gm::GeometryCollectionType, seedGeometry->getSRID());

    for(std::size_t i = 1; i < items.size(); ++i)
    {
      if(items[i]->getGeometry(geomIdx)->isValid())
        teGeomColl->add(items[i]->getGeometry(geomIdx));
    }

    resultGeometry = seedGeometry->Union(teGeomColl);
  }

  return resultGeometry;
}