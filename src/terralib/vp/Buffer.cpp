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
  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(inputLayer.get());

  std::auto_ptr<te::da::DataSetType> outputDataSetType(te::vp::GetDataSetType(inputLayer, outputLayerName, bufferBoundariesRule, copyInputColumns));
  std::auto_ptr<te::mem::DataSet> outputDataSet;

  te::da::DataSourcePtr dataSource = te::da::GetDataSource(dsLayer->getDataSourceId(), true);

  const te::da::DataSourceCapabilities dsCapabilities = dataSource->getCapabilities();

  if(dsCapabilities.supportsPreparedQueryAPI() && dsCapabilities.supportsSpatialOperators())
  {
    outputDataSet.reset(new te::mem::DataSet(*te::vp::BufferQuery(inputLayer, distance, bufferPolygonRule, bufferBoundariesRule, levels), true));
    outputDataSet.reset(new te::mem::DataSet(*te::vp::PrepareDataSet(outputDataSetType.get(), outputDataSet.get()), true));
  }
  else
  {
    outputDataSet.reset(new te::mem::DataSet(outputDataSetType.get()));
    te::vp::BufferMemory(outputDataSet.get(), distance, bufferPolygonRule, levels);
  }

  Persistence(outputDataSetType.get(), outputDataSet.get(), dsInfo);

  te::da::DataSourcePtr dataSourceManager = te::da::DataSourceManager::getInstance().get(dsInfo->getId(), dsInfo->getType(), dsInfo->getConnInfo());

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

  //te::dt::SimpleProperty* levelProperty = new te::dt::SimpleProperty("level", te::dt::INT32_TYPE);
  //dsType->add(levelProperty);

  //if(bufferBoundariesRule == te::vp::NOT_DISSOLVE)
  //{
  //  te::dt::SimpleProperty* distanceProperty = new te::dt::SimpleProperty("distance", te::dt::DOUBLE_TYPE);
  //  dsType->add(distanceProperty);
  //}
  
  std::auto_ptr<const te::map::LayerSchema> schema(inputLayer->getSchema());

  if(copyInputColumns)
  {
    const std::vector<te::dt::Property*> props = schema->getProperties();

    for(std::size_t i=0; i < props.size(); ++i)
    {
      if(props[i]->getType() != te::dt::GEOMETRY_TYPE)
        dsType->add(props[i]->clone());
    }
  }

  te::gm::GeometryProperty* p = static_cast<te::gm::GeometryProperty*>(schema->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE));

  te::gm::GeometryProperty* geometry = new te::gm::GeometryProperty("geom");
  geometry->setGeometryType(te::gm::GeometryType);
  geometry->setSRID(inputLayer->getSRID());
  dsType->add(geometry);

  return dsType;
}

void te::vp::BufferMemory(te::mem::DataSet* dataSet,
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

te::da::DataSet* te::vp::BufferQuery(const te::map::AbstractLayerPtr& inputLayer,
                                    const std::map<te::gm::Geometry*, double>& distance,
                                    const int& bufferPolygonRule,
                                    const int& bufferBoundariesRule,
                                    const int& levels)
{
  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(inputLayer.get());

  te::da::DataSourcePtr dataSource = te::da::GetDataSource(dsLayer->getDataSourceId(), true);

  std::auto_ptr<te::da::DataSetType> dsType = dsLayer->getSchema();

  te::da::Fields* fields = new te::da::Fields;

  te::gm::GeometryProperty* geom = te::da::GetFirstGeomProperty(dsType.get());

  double dist= 1;

  te::da::Expression* e_level = 0;
  te::da::Expression* e_buffer = 0;
  te::da::Expression* e_aux = 0;
  te::da::Field* f_level = 0;
  te::da::Field* f_buffer = 0;
  

  for(int i=1; i <= levels; ++i)
  {
    std::stringstream ss;
    ss << i;
    std::string index = ss.str();

    //buffer
    if(bufferPolygonRule == te::vp::INSIDE_OUTSIDE)
    {
      te::da::Expression* e_buffer1 = new te::da::ST_Buffer(te::da::PropertyName(geom->getName()), dist*i);
      te::da::Expression* e_buffer2 = new te::da::ST_Buffer(te::da::PropertyName(geom->getName()), -dist*i);
      e_buffer = new te::da::ST_Difference(e_buffer1, e_buffer2);
      f_buffer = new te::da::Field(*e_buffer, "geom"+index);
    }
    else
    {
      e_buffer = new te::da::ST_Buffer(te::da::PropertyName(geom->getName()), dist*i);
      f_buffer = new te::da::Field(*e_buffer, "geom"+index);
    }

    if(bufferBoundariesRule == te::vp::DISSOLVE)
    {
      delete f_buffer;
      te::da::Expression* e_union = new te::da::ST_Union(e_buffer);

      if(!e_aux)
      {
        f_buffer = new te::da::Field(*e_union, "geom"+index);
        fields->push_back(f_buffer);
        e_aux = e_union;
      }
      else
      {
        te::da::Expression* e_diff = new te::da::ST_Difference(e_union, e_aux);
        f_buffer = new te::da::Field(*e_diff, "geom"+index);
        fields->push_back(f_buffer);
        e_aux = e_union;
      }
    }
    else
    {
      if(!e_aux)
      {
        fields->push_back(f_buffer);
        e_aux = e_buffer;
      }
      else
      {
        te::da::Expression* e_diff = new te::da::ST_Difference(e_buffer, e_aux);
        f_buffer = new te::da::Field(*e_diff, "geom"+index);
        fields->push_back(f_buffer);
        e_aux = e_buffer;
      }
    }
  }

  te::da::FromItem* fromItem = new te::da::DataSetName(dsType->getName());
  te::da::From* from = new te::da::From;
  from->push_back(fromItem);

  te::da::Select select(fields, from);
  std::auto_ptr<te::da::DataSet> dsQuery = dataSource->query(select);

  return dsQuery.release();
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

te::mem::DataSet* te::vp::PrepareDataSet(te::da::DataSetType* dataSetType, 
                                        te::mem::DataSet* dataSet)
{
  std::auto_ptr<te::mem::DataSet> outputDataSet(new te::mem::DataSet(dataSetType));

  std::size_t numProps = dataSet->getNumProperties();
  std::size_t firstGeomPos = te::da::GetFirstSpatialPropertyPos(dataSet);
  int numItems = numProps - firstGeomPos;

  dataSet->moveBeforeFirst();
  
  unsigned int type;

  while(dataSet->moveNext())
  {
    int numCurrentItem = 0;

    for(int i = 0; i < numItems; ++i)
    {
      te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(outputDataSet.get());

      for(int j = 0; j < numProps; ++j)
      {
        type = dataSet->getPropertyDataType(j);

        if(type != te::dt::GEOMETRY_TYPE)
        {

        }
        else
        {
          dataSetItem->setGeometry(j, *dataSet->getGeometry(j+numCurrentItem));
          outputDataSet->add(dataSetItem);

          ++numCurrentItem;
          j = numProps;
        }
      }
    }
  }

  return outputDataSet.release();
}