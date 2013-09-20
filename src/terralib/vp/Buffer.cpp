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
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
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
#include "../qt/widgets/layer/utils/DataSet2Layer.h"
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
  //o retorno da função, é um dataSetType "preparado" com as propriedades que serão utilizadas para o resultado da operação
  std::auto_ptr<te::da::DataSetType> outputDataSetType(te::vp::GetDataSetType(inputLayer, outputLayerName, bufferBoundariesRule, copyInputColumns));
  std::auto_ptr<te::mem::DataSet> outputDataSet(new te::mem::DataSet(outputDataSetType.get()));

  //Faço um dynamic cast para um dataSetLayer para ter acesso ao seu dataSource
  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(inputLayer.get());
  te::da::DataSourcePtr dataSource = te::da::GetDataSource(dsLayer->getDataSourceId(), true);

  //Por meio do dataSource tenho acesso aos seus capabilities
  const te::da::DataSourceCapabilities dsCapabilities = dataSource->getCapabilities();
  
  //Lógica para execução da operação, em memória ou via query.
  if(dsCapabilities.supportsPreparedQueryAPI() && dsCapabilities.supportsSpatialOperators())
  {
    //te::vp::SetBufferQuery(outputDataSet.get(), distance, bufferPolygonRule, levels);
  }
  else
  {
    te::vp::SetBuffer(outputDataSet.get(), distance, bufferPolygonRule, levels);
  }

  //Entra nessa condição caso o usuário queira dissolver as bordas que se tocam entre as geometrias resultantes.
  if(bufferBoundariesRule == te::vp::DISSOLVE)
  {
   te::mem::DataSet* dataSetDissolved = te::vp::SetDissolvedBoundaries(outputDataSetType.get(), outputDataSet.get(), levels);
   outputDataSet.reset(dataSetDissolved);
  }

  //Função de armazenamento do resultado da operação
  Persistence(outputDataSetType.get(), outputDataSet.get(), dsInfo);

  te::da::DataSourcePtr dataSourceManager = te::da::DataSourceManager::getInstance().get(dsInfo->getId(), dsInfo->getType(), dsInfo->getConnInfo());
  
  //Conversão de um DataSet para um Layer
  te::qt::widgets::DataSet2Layer converter(dataSourceManager->getId());
  te::da::DataSetTypePtr dt(dataSourceManager->getDataSetType(outputDataSetType->getName()).release());
  te::map::DataSetLayerPtr newLayer = converter(dt);

  return newLayer;
}

te::da::DataSetType* te::vp::GetDataSetType(const te::map::AbstractLayerPtr& inputLayer,
                                            const std::string& outputLayerName,
                                            const int& bufferBoundariesRule,
                                            const bool& copyInputColumns)
{
  te::da::DataSetType* dsType = new te::da::DataSetType(outputLayerName);

  //te::dt::SimpleProperty* pkProperty = new te::dt::SimpleProperty("id", te::dt::INT32_TYPE);
  //dsType->add(pkProperty);

  //std::vector<te::dt::Property*> propPK;
  //propPK.push_back(pkProperty);

  //te::da::PrimaryKey* pk = new te::da::PrimaryKey("id_pk", dsType);
  //pk->setProperties(propPK);
  //dsType->setPrimaryKey(pk);

  te::dt::SimpleProperty* levelProperty = new te::dt::SimpleProperty("level", te::dt::INT32_TYPE);
  dsType->add(levelProperty);

  if(bufferBoundariesRule == te::vp::NOT_DISSOLVE)
  {
    te::dt::SimpleProperty* distanceProperty = new te::dt::SimpleProperty("distance", te::dt::DOUBLE_TYPE);
    dsType->add(distanceProperty);
  }

  if(copyInputColumns)
  {
    std::auto_ptr<const te::map::LayerSchema> schema(inputLayer->getSchema());
    const std::vector<te::dt::Property*> props = schema->getProperties();

    for(std::size_t i=0; i < props.size(); ++i)
    {
      if(props[i]->getType() != te::dt::GEOMETRY_TYPE)
        dsType->add(props[i]->clone());
    }
  }

  te::gm::GeometryProperty* geometry = new te::gm::GeometryProperty("geom");
  geometry->setGeometryType(te::gm::GeometryType);
  dsType->add(geometry);

  return dsType;
}

void te::vp::SetBuffer(te::mem::DataSet* dataSet,
                      const std::map<te::gm::Geometry*, double>& distance,
                      const int& bufferPolygonRule,
                      const int& levels)
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
          for(std::size_t level=1; level <= levels; ++level)
          {
            te::mem::DataSetItem* item = new te::mem::DataSetItem(dataSet);

            std::auto_ptr<te::gm::Geometry> outGeom(it->first->buffer(it->second * level, 16, te::gm::CapButtType));
            std::auto_ptr<te::gm::Geometry> inGeom(it->first->buffer(-it->second * level, 16, te::gm::CapButtType));
            te::gm::Geometry* diffGeom = outGeom->difference(inGeom.get());

            item->setInt32(0, level);
            item->setGeometry(pos, *diffGeom);
            dataSet->add(item);
          }
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
          for(std::size_t level=1; level <= levels; ++level)
          {
            te::mem::DataSetItem* item = new te::mem::DataSetItem(dataSet);
            std::auto_ptr<te::gm::Geometry> newGeom(it->first->buffer(it->second, 16, te::gm::CapButtType));
            te::gm::Geometry* diffGeom = newGeom->difference(it->first);

            item->setInt32(0, level);
            item->setGeometry(pos, *diffGeom);
            dataSet->add(item);
          }
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
          for(std::size_t level=1; level <= levels; ++level)
          {
            te::mem::DataSetItem* item = new te::mem::DataSetItem(dataSet);
            std::auto_ptr<te::gm::Geometry> newGeom(it->first->buffer(-it->second, 16, te::gm::CapButtType));
            te::gm::Geometry* diffGeom = it->first->difference(newGeom.get());

            item->setInt32(0, level);
            item->setGeometry(pos, *diffGeom);
            dataSet->add(item);
          }
        }
        dataSet->moveNext();

        ++it;
      }
    break;
  }
}

te::mem::DataSet* te::vp::SetDissolvedBoundaries(te::da::DataSetType* dataSetType, 
                                                te::mem::DataSet* dataset,
                                                const int& levels)
{
  te::mem::DataSet* outputDataSet = new te::mem::DataSet(dataSetType);
  outputDataSet->moveFirst();

  int posPK = 0;
  int posLevel = 0;

  dataset->moveFirst();
  std::size_t pos = te::da::GetFirstSpatialPropertyPos(dataset);

  if(dataset->size() > 1)
  {
    std::auto_ptr<te::gm::Geometry> seedGeometry = dataset->getGeometry(pos);

    te::gm::Geometry* currentGeometry;

    for(std::size_t level = 1; level <= levels; ++level)
    {
      dataset->moveBeforeFirst();

      while(dataset->moveNext())
      {
        if(dataset->getGeometry(pos)->isValid() && dataset->getInt32(posLevel) == level)
        {
          currentGeometry = seedGeometry->Union(dataset->getGeometry(pos).get());
          seedGeometry.reset(currentGeometry);
        }
      }

      te::gm::GeometryCollection* geomCollection = (te::gm::GeometryCollection*)seedGeometry.release();

      for(std::size_t i = 0; i < geomCollection->getNumGeometries(); ++i)
      {
        te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(outputDataSet);
        dataSetItem->setInt32(posPK, i);
        dataSetItem->setInt32(posLevel, level);
        dataSetItem->setGeometry(pos, *geomCollection->getGeometryN(i));
        outputDataSet->add(dataSetItem);
      }
    }

    delete currentGeometry;
  }
  return outputDataSet;
}