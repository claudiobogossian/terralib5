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
#include "../geometry/GeometryCollection.h"
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
  te::mem::DataSet* dataSet;

  if(copyInputColumns)
  {
    std::auto_ptr<te::da::DataSet> inputDataSet(inputLayer->getData());
    inputDataSet->moveNext();
    dataSet = new te::mem::DataSet(*inputDataSet);
  }
  else
  {
    te::gm::GeometryProperty* geometry = new te::gm::GeometryProperty("geom");
    geometry->setGeometryType(te::gm::GeometryType);
    dataSetType->add(geometry);
    dataSet = new te::mem::DataSet(dataSetType.get());
  }

  te::vp::SetBuffer(dataSet, distance, bufferPolygonRule);

  if(bufferBoundariesRule == te::vp::DISSOLVE)
  {
    te::vp::SetDissolvedBoundaries(dataSet);
  }

  Persistence(dataSetType.get(), dataSet, dsInfo);

  delete dataSet;

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

void te::vp::SetBuffer(te::mem::DataSet* dataSet,
                      const std::map<te::gm::Geometry*, double>& distance,
                      const int& bufferPolygonRule)
{
  std::map<te::gm::Geometry*, double>::const_iterator it = distance.begin();
  dataSet->moveFirst();
  std::size_t pos = te::da::GetFirstSpatialPropertyPos(dataSet);

  switch(bufferPolygonRule)
  {
    case te::vp::INSIDE_OUTSIDE:
      while(it != distance.end())
      {
        if(it->first->isValid())
        {
          //te::mem::DataSetItem* item = new te::mem::DataSetItem(dataSet);

          std::auto_ptr<te::gm::Geometry> outGeom(it->first->buffer(it->second, 16, te::gm::CapButtType));
          std::auto_ptr<te::gm::Geometry> inGeom(it->first->buffer(it->second, 16, te::gm::CapButtType));

          std::auto_ptr<te::gm::Geometry> diffOutGeom(outGeom->difference(it->first));
          std::auto_ptr<te::gm::Geometry> diffInGeom(it->first->difference(inGeom.get()));

          te::gm::Geometry* bufferGeom = diffOutGeom->Union(diffInGeom.get());

          /*item->setGeometry(pos, *bufferGeom);
          dataSet->add(item);
*/
          dataSet->setValue(pos, bufferGeom);
        }
        dataSet->moveNext();

        ++it;
      }
    break;

    case te::vp::ONLY_OUTSIDE:
      while(it != distance.end())
      {
        if(it->first->isValid())
        {
          te::mem::DataSetItem* item = new te::mem::DataSetItem(dataSet);
          std::auto_ptr<te::gm::Geometry> newGeom(it->first->buffer(it->second, 16, te::gm::CapButtType));
          te::gm::Geometry* diffGeom = newGeom->difference(it->first);

          item->setGeometry(pos, *diffGeom);
          dataSet->add(item);
        }
        dataSet->moveNext();

        ++it;
      }
    break;

    case te::vp::ONLY_INSIDE:
      while(it != distance.end())
      {
        if(it->first->isValid())
        {
          te::mem::DataSetItem* item = new te::mem::DataSetItem(dataSet);
          std::auto_ptr<te::gm::Geometry> newGeom(it->first->buffer(-it->second, 16, te::gm::CapButtType));
          te::gm::Geometry* diffGeom = it->first->difference(newGeom.get());

          item->setGeometry(pos, *diffGeom);
          dataSet->add(item);
        }
        dataSet->moveNext();

        ++it;
      }
    break;
  }
}

void te::vp::SetDissolvedBoundaries(te::mem::DataSet* dataset)
{
  dataset->moveFirst();

  std::size_t pos = te::da::GetFirstSpatialPropertyPos(dataset);

  if(dataset->size() > 1)
  {
    te::gm::Geometry* seedGeometry;
    seedGeometry = dataset->getGeometry(pos);

    te::gm::Geometry* currentGeometry;

    while(dataset->moveNext())
    {
      if(dataset->getGeometry(pos)->isValid())
      {
        currentGeometry = seedGeometry->Union(dataset->getGeometry(pos));
        seedGeometry = currentGeometry;
      }
    }

    dataset->clear();
    dataset->moveFirst();
    te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(dataset);
    dataSetItem->setGeometry(pos, *seedGeometry);
    dataset->add(dataSetItem);

    delete currentGeometry;
  }
}