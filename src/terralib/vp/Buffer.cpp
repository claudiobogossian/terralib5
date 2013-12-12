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
#include "../common/progress/TaskProgress.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/datasource/DataSourceInfo.h"
#include "../dataaccess/datasource/DataSourceManager.h"
#include "../dataaccess/datasource/DataSourceFactory.h"
#include "../dataaccess/dataset/DataSetTypeConverter.h"

#include "../dataaccess/query_h.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/Property.h"
#include "../datatype/SimpleProperty.h"
#include "../datatype/StringProperty.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryCollection.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/MultiPolygon.h"
#include "../maptools/AbstractLayer.h"
#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"
#include "../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../sam/rtree.h"
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


// -- auxiliary functions
te::da::DataSetType* GetDataSetType(const std::string& inDatasetName,
                                    te::da::DataSource* inDataSource,
                                    const std::string& outputLayerName,
                                    const int& bufferBoundariesRule,
                                    const bool& copyInputColumns);

bool BufferMemory(const std::string& inDatasetName,
                  te::da::DataSource* inDataSource,
                  const double& distance,
                  const int& bufferPolygonRule,
                  const int& bufferBoundariesRule,
                  const bool& copyInputColumns,
                  const int& levels,
                  te::mem::DataSet* outputDataSet,
                  te::da::DataSetType* outputDataSetType,
                  te::gm::GeomType outGeoType);

bool BufferMemory(const std::string& inDatasetName,
                  te::da::DataSource* inDataSource,
                  const std::string& distance,
                  const int& bufferPolygonRule,
                  const int& bufferBoundariesRule,
                  const bool& copyInputColumns,
                  const int& levels,
                  te::mem::DataSet* outputDataSet,
                  te::da::DataSetType* outputDataSetType,
                  te::gm::GeomType outGeoType);

bool BufferQuery(const std::string& inDatasetName,
                te::da::DataSource* inDataSource,
                const double& fixedDistance,
                const int& bufferPolygonRule,
                const int& bufferBoundariesRule,
                const bool& copyInputColumns,
                const int& levels,
                te::mem::DataSet* outputDataSet,
                te::da::DataSetType* outputDataSetType,
                te::gm::GeomType outGeoType);

bool BufferQuery(const std::string& inDatasetName,
                te::da::DataSource* inDataSource,
                const std::string& fromAttDistance,
                const int& bufferPolygonRule,
                const int& bufferBoundariesRule,
                const bool& copyInputColumns,
                const int& levels,
                te::mem::DataSet* outputDataSet,
                te::da::DataSetType* outputDataSetType,
                te::gm::GeomType outGeoType);

te::gm::Geometry* SetBuffer(te::gm::Geometry* geom, 
                            const int& bufferPolygonRule, 
                            const double& distance, 
                            const int& level,
                            te::gm::Geometry*& auxGeom);

void PrepareDataSet(te::da::DataSetType* dataSetType, 
                    te::da::DataSet* dataSetQuery,
                    te::mem::DataSet* outputDataSet,
                    const double& distance);
//  ---



bool te::vp::Buffer(const std::string& inDatasetName,
                    te::da::DataSource* inDataSource,
                    const int& bufferPolygonRule,
                    const int& bufferBoundariesRule,
                    const bool& copyInputColumns,
                    const int& levels,
                    const std::string& outDataSetName,
                    te::da::DataSource* outDataSource,
                    const double& fixedDistance,
                    const std::string& fromAttDistance)
{
  assert(inDataSource);
  assert(outDataSource);

  // define the schema of the output dataset based on the aggregation parameters for the non-spatial attributes
  std::auto_ptr<te::da::DataSetType> outputDataSetType(GetDataSetType(inDatasetName, inDataSource, outDataSetName, bufferBoundariesRule, copyInputColumns));
  std::auto_ptr<te::mem::DataSet> outputDataSet(new te::mem::DataSet(outputDataSetType.get()));

  // select a strategy based on the capabilities of the input datasource
  const te::da::DataSourceCapabilities dsCapabilities = inDataSource->getCapabilities();
  
  // execute the strategy
  bool res;
  if(dsCapabilities.supportsPreparedQueryAPI() && dsCapabilities.getQueryCapabilities().supportsSpatialSQLDialect())
  {
    if(fromAttDistance == "")
      res = BufferQuery(inDatasetName, inDataSource, fixedDistance, bufferPolygonRule, bufferBoundariesRule, copyInputColumns, levels, outputDataSet.get(), outputDataSetType.get(), te::gm::MultiPolygonType);
    else
      res = BufferQuery(inDatasetName, inDataSource, fromAttDistance, bufferPolygonRule, bufferBoundariesRule, copyInputColumns,levels, outputDataSet.get(), outputDataSetType.get(), te::gm::MultiPolygonType);
  }
  else
  {
    res = BufferMemory(inDatasetName, inDataSource, fixedDistance, bufferPolygonRule, bufferBoundariesRule, copyInputColumns, levels, outputDataSet.get(), outputDataSetType.get(), te::gm::MultiPolygonType);
  }

  if (!res)
    return false;

  // do any adaptation necessary to persist the output dataset
  te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(outputDataSetType.get(), outDataSource->getCapabilities());
  te::da::DataSetType* dsTypeResult = converter->getResult();
  std::auto_ptr<te::da::DataSetAdapter> dsAdapter(te::da::CreateAdapter(outputDataSet.get(), converter));
  
  std::map<std::string, std::string> options;
  // create the dataset
  outDataSource->createDataSet(dsTypeResult, options);
  
  // copy from memory to output datasource
  te::common::TaskProgress task("Saving buffer...");
  outputDataSet->moveBeforeFirst();
  outDataSource->add(dsTypeResult->getName(),outputDataSet.get(), options);

  return true;
}

te::da::DataSetType* GetDataSetType(const std::string& inDatasetName,
                                    te::da::DataSource* inDataSource,
                                    const std::string& outputLayerName,
                                    const int& bufferBoundariesRule,
                                    const bool& copyInputColumns)
{
  te::da::DataSetType* dsType = new te::da::DataSetType(outputLayerName);

  //Primary key
  te::dt::SimpleProperty* pkProperty = new te::dt::SimpleProperty(outputLayerName + "_id", te::dt::INT32_TYPE);
  pkProperty->setAutoNumber(true);
  dsType->add(pkProperty);

  te::da::PrimaryKey* pk = new te::da::PrimaryKey(outputLayerName + "_pk", dsType);
  pk->add(pkProperty);
  dsType->setPrimaryKey(pk);

  te::dt::SimpleProperty* levelProperty = new te::dt::SimpleProperty("level", te::dt::INT32_TYPE);
  dsType->add(levelProperty);

  te::dt::SimpleProperty* distanceProperty = new te::dt::SimpleProperty("distance", te::dt::DOUBLE_TYPE);
  dsType->add(distanceProperty);

  
  if(copyInputColumns)
  {
    boost::ptr_vector<te::dt::Property> props = inDataSource->getProperties(inDatasetName);
    boost::ptr_vector<te::dt::Property>::iterator it = props.begin();

    while(it != props.end())
    {
      if(it->getType() != te::dt::GEOMETRY_TYPE && it->getName() != "FID")
        dsType->add(it->clone());

      ++it;
    }
  }
  
  std::auto_ptr<te::da::DataSetType> inputDataSetType = inDataSource->getDataSetType(inDatasetName);
  te::gm::GeometryProperty* p = static_cast<te::gm::GeometryProperty*>(te::da::GetFirstGeomProperty(inputDataSetType.get()));

  te::gm::GeometryProperty* geometry = new te::gm::GeometryProperty("geom");
  geometry->setGeometryType(te::gm::MultiPolygonType);
  geometry->setSRID(p->getSRID());
  dsType->add(geometry);

  return dsType;
}

bool BufferMemory(const std::string& inDataSetName,
                  te::da::DataSource* inDataSource,
                  const double& distance,
                  const int& bufferPolygonRule,
                  const int& bufferBoundariesRule,
                  const bool& copyInputColumns,
                  const int& levels,
                  te::mem::DataSet* outputDataSet,
                  te::da::DataSetType* outputDataSetType,
                  te::gm::GeomType outGeoType)
{
  std::auto_ptr<te::da::DataSet> inputDataSet = inDataSource->getDataSet(inDataSetName);

  int type;
  int pk = 0;

  
  te::common::TaskProgress task("Processing buffer...");
  task.setTotalSteps(inputDataSet->size());
  task.useTimer(true);

  inputDataSet->moveBeforeFirst();
  while(inputDataSet->moveNext())
  {
    te::gm::Geometry* auxGeom = 0;

    for(int i = 1; i <= levels; ++i)
    {
      te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(outputDataSet);

      for(std::size_t j = 0; j < inputDataSet->getNumProperties(); ++j)
      {
        type = inputDataSet->getPropertyDataType(j);
        if(copyInputColumns)
        {
          switch (type)
          {
            case te::dt::INT32_TYPE:
              if(inputDataSet->getPropertyName(j) != "FID")
                dataSetItem->setInt32(j+2, inputDataSet->getInt32(j));
              break;
            case te::dt::INT64_TYPE:
              dataSetItem->setInt64(j+2, inputDataSet->getInt64(j));
              break;
            case te::dt::DOUBLE_TYPE:
              dataSetItem->setDouble(j+2, inputDataSet->getDouble(j));
              break;
            case te::dt::STRING_TYPE:
              dataSetItem->setString(j+2, inputDataSet->getString(j));
              break;
            case te::dt::GEOMETRY_TYPE:
              dataSetItem->setInt32(0, pk); //pk
              dataSetItem->setInt32(1, i); //level
              dataSetItem->setDouble(2, distance*(i)); //distance

              std::auto_ptr<te::gm::Geometry> currentGeom = inputDataSet->getGeometry(j);
              std::auto_ptr<te::gm::Geometry> outGeom;

              if(currentGeom->isValid())
                outGeom.reset(SetBuffer(currentGeom.get(), bufferPolygonRule, distance, i, auxGeom));
              
              if(outGeom.get() && outGeom->isValid())
              {
                if(outGeom->getGeomTypeId() == te::gm::MultiPolygonType)
                {
                  dataSetItem->setGeometry(j+2, outGeom.release());
                }
                else
                {
                  std::auto_ptr<te::gm::GeometryCollection> mPolygon(new te::gm::GeometryCollection(0, te::gm::MultiPolygonType, outGeom->getSRID()));
                  mPolygon->add(outGeom.release());
                  dataSetItem->setGeometry(j+2, mPolygon.release());
                }

                outputDataSet->add(dataSetItem);
                ++pk;
              }
          }
        }
        else
        {
          if(type == te::dt::GEOMETRY_TYPE)
          {
            dataSetItem->setInt32(0, pk); //pk
            dataSetItem->setInt32(1, i); //level
            dataSetItem->setDouble(2, distance*(i)); //distance

            std::auto_ptr<te::gm::Geometry> currentGeom = inputDataSet->getGeometry(j);
            std::auto_ptr<te::gm::Geometry> outGeom;

            if(currentGeom->isValid())
              outGeom.reset(SetBuffer(currentGeom.get(), bufferPolygonRule, distance, i, auxGeom));

            if(outGeom.get() && outGeom->isValid())
            {
              if(outGeom->getGeomTypeId() == te::gm::MultiPolygonType)
              {
                dataSetItem->setGeometry(3, outGeom.release());
              }
              else
              {
                std::auto_ptr<te::gm::GeometryCollection> mPolygon(new te::gm::GeometryCollection(0, te::gm::MultiPolygonType, outGeom->getSRID()));
                mPolygon->add(outGeom.release());
                dataSetItem->setGeometry(3, mPolygon.release());
              }

              outputDataSet->add(dataSetItem);
              ++pk;
            }
          }
        }
      }
    }
    delete auxGeom;

    task.pulse();
  }
    
  if(bufferBoundariesRule == te::vp::DISSOLVE)
  {
    std::map<int, std::vector<te::gm::Geometry*> > mapGeom;
    
    int levelPos = te::da::GetPropertyPos(outputDataSet, "level");
    int geomPos  = te::da::GetPropertyPos(outputDataSet, "geom");
    int level;

    task.setMessage("Dissolving boundaries...");
    task.setTotalSteps(levels*outputDataSet->size());
    task.setCurrentStep(1);
    for(int i = 1; i <= levels; ++i)
    {
      te::sam::rtree::Index<te::gm::Geometry*, 4> rtree;
      
      outputDataSet->moveBeforeFirst();
      while(outputDataSet->moveNext())
      {
        level = outputDataSet->getInt32(levelPos);
        if(level == i)
        {
          te::gm::Geometry* geom = outputDataSet->getGeometry(geomPos).release();
          
          std::vector<te::gm::Geometry*> vec;
          
          rtree.search(*(geom->getMBR()), vec);
          
          if(!vec.empty())
          {
            for(std::size_t t = 0; t < vec.size(); ++t)
            {
              if(geom->intersects(vec[t]))
              {
                geom = geom->Union(vec[t]);
                rtree.remove(*(vec[t]->getMBR()), vec[t]);
              }
            }
          }
          rtree.insert(*(geom->getMBR()), geom);
        }
        task.pulse();
      }
      
      std::vector<te::gm::Geometry*> geomVec;
      std::auto_ptr<te::gm::Envelope> e = outputDataSet->getExtent(geomPos);
      rtree.search(*(e.get()), geomVec);
      mapGeom.insert(std::pair<int, std::vector<te::gm::Geometry*> >(i, geomVec));
      rtree.clear();
    }
    
    outputDataSet->clear();
    outputDataSet->moveBeforeFirst();
    
    task.setTotalSteps(mapGeom.size());
    task.setCurrentStep(1);
    int pk = 0;
    std::map<int, std::vector<te::gm::Geometry*> >::iterator it = mapGeom.begin();
    while(it != mapGeom.end())
    {
      for(std::size_t i = 0; i < it->second.size(); ++i)
      {
        te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(outputDataSet);
        dataSetItem->setInt32(0, pk); //pk
        dataSetItem->setInt32(1, it->first); //level
        dataSetItem->setDouble(2, 0/*distance*(i)*/); //distance
        
        if(it->second[i]->getGeomTypeId() == te::gm::MultiPolygonType)
        {
          dataSetItem->setGeometry(3, it->second[i]);
        }
        else
        {
          std::auto_ptr<te::gm::GeometryCollection> mPolygon(new te::gm::GeometryCollection(0, te::gm::MultiPolygonType, it->second[i]->getSRID()));
          mPolygon->add(it->second[i]);
          dataSetItem->setGeometry(3, mPolygon.release());
        }
        
        outputDataSet->add(dataSetItem);
        ++pk;
      }
      ++it;
      task.pulse();
    }
  }

  return true;
}

bool BufferMemory(const std::string& inDatasetName,
                  te::da::DataSource* inDataSource,
                  const std::string& distance,
                  const int& bufferPolygonRule,
                  const int& bufferBoundariesRule,
                  const int& levels,
                  te::mem::DataSet* outputDataSet,
                  te::da::DataSetType* outputDataSetType,
                  te::gm::GeomType outGeoType)
{
  return false;
}

bool BufferQuery(const std::string& inDatasetName,
                te::da::DataSource* inDataSource,
                const double& distance,
                const int& bufferPolygonRule,
                const int& bufferBoundariesRule,
                const bool& copyInputColumns,
                const int& levels,
                te::mem::DataSet* outputDataSet,
                te::da::DataSetType* outputDataSetType,
                te::gm::GeomType outGeoType)
{
  std::auto_ptr<te::da::DataSetType> dsType = inDataSource->getDataSetType(inDatasetName);

  te::da::Fields* fields = new te::da::Fields;

  if(copyInputColumns)
  {
    std::vector<te::dt::Property*> props = dsType->getProperties();
    for(std::size_t i=0; i < props.size(); ++i)
    {
      if(props[i]->getType() != te::dt::GEOMETRY_TYPE)
      {
        te::da::Field* f_props = new te::da::Field(te::da::PropertyName(props[i]->getName()));
        fields->push_back(f_props);
      }
    }
  }

  te::gm::GeometryProperty* geom = te::da::GetFirstGeomProperty(dsType.get());

  te::da::Expression* e_buffer = 0;
  te::da::Expression* e_aux = 0;
  te::da::Field* f_buffer = 0;
  

  for(int i=1; i <= levels; ++i)
  {
    std::stringstream ss;
    ss << i;
    std::string index = ss.str();

    //buffer
    if(bufferPolygonRule == te::vp::INSIDE_OUTSIDE)
    {
      te::da::Expression* e_buffer1 = new te::da::ST_Buffer(te::da::PropertyName(geom->getName()), distance*i);
      te::da::Expression* e_buffer2 = new te::da::ST_Buffer(te::da::PropertyName(geom->getName()), -distance*i);
      e_buffer = new te::da::ST_Difference(e_buffer1, e_buffer2);
      f_buffer = new te::da::Field(*e_buffer, "geom"+index);
    }
    else if(bufferPolygonRule == te::vp::ONLY_OUTSIDE)
    {
      te::da::Expression* e_buffer1 = new te::da::ST_Buffer(te::da::PropertyName(geom->getName()), distance*i);
      te::da::Expression* e_buffer2 = new te::da::PropertyName(geom->getName());
      e_buffer = new te::da::ST_Difference(e_buffer1, e_buffer2);
      f_buffer = new te::da::Field(*e_buffer, "geom"+index);
    }
    else
    {
      te::da::Expression* e_buffer1 = new te::da::ST_Buffer(te::da::PropertyName(geom->getName()), -distance*i);
      te::da::Expression* e_buffer2 = new te::da::PropertyName(geom->getName());
      e_buffer = new te::da::ST_Difference(e_buffer2, e_buffer1);
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
  std::auto_ptr<te::da::DataSet> dsQuery = inDataSource->query(select);

  PrepareDataSet(outputDataSetType, dsQuery.get(), outputDataSet, distance);

  return true;
}

bool BufferQuery(const std::string& inDatasetName,
                te::da::DataSource* inDataSource,
                const std::string& distance,
                const int& bufferPolygonRule,
                const int& bufferBoundariesRule,
                const bool& copyInputColumns,
                const int& levels,
                te::mem::DataSet* outputDataSet,
                te::da::DataSetType* outputDataSetType,
                te::gm::GeomType outGeoType)
{
  std::auto_ptr<te::da::DataSetType> dsType = inDataSource->getDataSetType(inDatasetName);

  te::da::Fields* fields = new te::da::Fields;

  std::vector<te::dt::Property*> props = dsType->getProperties();
  for(std::size_t i=0; i < props.size(); ++i)
  {
    te::da::Field* f_props = new te::da::Field(te::da::PropertyName(props[i]->getName()));
    fields->push_back(f_props);
  }

  te::gm::GeometryProperty* geom = te::da::GetFirstGeomProperty(dsType.get());
  te::da::Expression* e_buffer = 0;
  te::da::Expression* e_aux = 0;
  te::da::Field* f_buffer = 0;

  for(int i=1; i <= levels; ++i)
  {
    std::stringstream ss;
    ss << i;
    std::string index = ss.str();

    //buffer
    if(bufferPolygonRule == te::vp::INSIDE_OUTSIDE)
    {
      te::da::Expression* e_buffer1 = new te::da::ST_Buffer(te::da::PropertyName(geom->getName()), te::da::PropertyName(distance));
      te::da::Expression* e_buffer2 = new te::da::ST_Buffer(te::da::PropertyName(geom->getName()), te::da::PropertyName(distance));
      e_buffer = new te::da::ST_Difference(e_buffer1, e_buffer2);
      f_buffer = new te::da::Field(*e_buffer, "geom"+index);
    }
    else
    {
      e_buffer = new te::da::ST_Buffer(te::da::PropertyName(geom->getName()), te::da::PropertyName(distance));
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
  std::auto_ptr<te::da::DataSet> dsQuery = inDataSource->query(select);

  //PrepareDataSet(outputDataSetType, dsQuery.get(), outputDataSet);

  return false;
}

te::gm::Geometry* SetBuffer(te::gm::Geometry* geom, 
                            const int& bufferPolygonRule, 
                            const double& distance, 
                            const int& level,
                            te::gm::Geometry*& auxGeom)
{
  te::gm::Geometry* geomResult = 0;
  te::gm::Geometry* geomTemp = 0;
  std::auto_ptr<te::gm::Geometry> outGeom;
  std::auto_ptr<te::gm::Geometry> inGeom;
  switch(bufferPolygonRule)
  {
    case (te::vp::INSIDE_OUTSIDE):
      outGeom.reset(geom->buffer(distance * level, 16, te::gm::CapButtType));
      inGeom.reset(geom->buffer(-distance * level, 16, te::gm::CapButtType));
      geomResult = outGeom->difference(inGeom.get());
      
      geomTemp = (te::gm::Geometry*)geomResult->clone();
      if(auxGeom && auxGeom->isValid())
        geomResult = geomResult->difference(auxGeom);

      delete auxGeom;
      auxGeom = geomTemp;

      break;

    case (te::vp::ONLY_OUTSIDE):
      outGeom.reset(geom->buffer(distance * level, 16, te::gm::CapButtType));
      geomResult = outGeom->difference(geom);

      geomTemp = (te::gm::Geometry*)geomResult->clone();
      if(auxGeom && auxGeom->isValid())
        geomResult = geomResult->difference(auxGeom);

      delete auxGeom;
      auxGeom = geomTemp;

      break;

    case (te::vp::ONLY_INSIDE):
      inGeom.reset(geom->buffer(-distance * level, 16, te::gm::CapButtType));
      geomResult = geom->difference(inGeom.get());

      geomTemp = (te::gm::Geometry*)geomResult->clone();
      if(auxGeom && auxGeom->isValid())
        geomResult = geomResult->difference(auxGeom);

      delete auxGeom;
      auxGeom = geomTemp;

      break;
  }
  return geomResult;
}

void PrepareDataSet(te::da::DataSetType* dataSetType, 
                    te::da::DataSet* dataSetQuery,
                    te::mem::DataSet* outputDataSet,
                    const double& distance)
{
  std::size_t numProps = dataSetQuery->getNumProperties();
  std::size_t firstGeomPos = te::da::GetFirstSpatialPropertyPos(dataSetQuery);
  int numItems = numProps - firstGeomPos;
  int pk = 0;
  dataSetQuery->moveBeforeFirst();
  
  unsigned int type;

  while(dataSetQuery->moveNext())
  {
    int numCurrentItem = 0;

    for(int i = 0; i < numItems; ++i)
    {
      te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(outputDataSet);

      for(std::size_t j = 0; j < numProps; ++j)
      {
        type = dataSetQuery->getPropertyDataType(j);

        switch (type){
          case te::dt::INT32_TYPE:
            dataSetItem->setInt32(j+3, dataSetQuery->getInt32(j));
            break;
          case te::dt::INT64_TYPE:
            dataSetItem->setInt64(j+3, dataSetQuery->getInt64(j));
            break;
          case te::dt::DOUBLE_TYPE:
            dataSetItem->setDouble(j+3, dataSetQuery->getDouble(j));
            break;
          case te::dt::STRING_TYPE:
            dataSetItem->setString(j+3, dataSetQuery->getString(j));
            break;
          case te::dt::GEOMETRY_TYPE:
            dataSetItem->setInt32(0, pk); //pk
            dataSetItem->setInt32(1, i+1); //level
            dataSetItem->setDouble(2, distance*(i+1)); //distance
            
            std::auto_ptr<te::gm::Geometry> geom = dataSetQuery->getGeometry(j+numCurrentItem);
            if(geom->getGeomTypeId() == te::gm::MultiPolygonType)
            {
              dataSetItem->setGeometry(j+3, geom.release());
            }
            else
            {
              std::auto_ptr<te::gm::GeometryCollection> mPolygon(new te::gm::GeometryCollection(0, te::gm::MultiPolygonType, geom->getSRID()));
              mPolygon->add(geom.release());
              dataSetItem->setGeometry(j+3, mPolygon.release());
            }

            outputDataSet->add(dataSetItem);

            ++numCurrentItem;
            ++pk;
            j = numProps;
            break;
        }
      }
    }
  }
}
