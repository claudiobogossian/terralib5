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
  \file Buffer.h

  \brief Buffer Vector Processing functions.
*/

//Terralib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "../dataaccess/datasource/DataSourceInfo.h"
#include "../dataaccess/datasource/DataSourceManager.h"
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "../dataaccess/query_h.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/Property.h"
#include "../datatype/SimpleProperty.h"
#include "../datatype/StringProperty.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryProperty.h"
#include "../maptools/AbstractLayer.h"
#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"
#include "Buffer.h"
#include "BufferDialog.h"
#include "Config.h"
#include "Exception.h"
#include "Utils.h"

// STL
#include <map>
#include <math.h>
#include <string>
#include <vector>

// BOOST
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

te::map::AbstractLayerPtr te::vp::Buffer(const te::map::AbstractLayerPtr& inputLayer,
                                  const std::map<te::gm::Geometry*, double>& distance,
                                  const int& bufferPolygonRule,
                                  const int& bufferBoundariesRule,
                                  const bool& copyInputColumns,
                                  const int& levels,
                                  const std::string& outputLayerName,
                                  const te::da::DataSourceInfoPtr& dsInfo)
{
  te::map::AbstractLayerPtr outputLayer;

  std::auto_ptr<te::da::DataSetType> dataSetType(new te::da::DataSetType(outputLayerName));
  std::auto_ptr<te::mem::DataSet> dataSet;

  if(copyInputColumns)
  {

  }
  else
  {
    te::gm::GeometryProperty* geometry = new te::gm::GeometryProperty("geom");
    geometry->setGeometryType(te::gm::GeometryType);
    dataSetType->add(geometry);
    dataSet.reset(new te::mem::DataSet(dataSetType.get()));
  }

  std::map<te::gm::Geometry*, double>::const_iterator it = distance.begin();
  dataSet->moveFirst();
  
  while(it != distance.end())
  {
    te::mem::DataSetItem* item = new te::mem::DataSetItem(dataSet.get());
    te::gm::Geometry* newGeom = it->first->buffer(it->second, 32, te::gm::CapButtType);

    item->setGeometry(0, *newGeom);
    dataSet->add(item);
    dataSet->moveNext();

    ++it;
  }

  Persistence(dataSetType.get(), dataSet.get(), dsInfo);

  return outputLayer;
}

te::map::AbstractLayerPtr te::vp::Buffer(const te::map::AbstractLayerPtr& inputLayer,
                                  const std::map<te::gm::Geometry*, double>& distance,
                                  const int& bufferPolygonRule,
                                  const int& bufferBoundariesRule,
                                  const bool& copyInputColumns,
                                  const int& levels,
                                  const std::string& outputLayerName,
                                  const std::string& outputArchive)
{
  te::map::AbstractLayerPtr outputLayer;

  return outputLayer;
}