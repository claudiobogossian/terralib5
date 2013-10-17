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
                  const int& levels,
                  te::mem::DataSet* outputDataSet,
                  te::da::DataSetType* outputDataSetType,
                  te::gm::GeomType outGeoType);

bool BufferMemory(const std::string& inDatasetName,
                  te::da::DataSource* inDataSource,
                  const std::string& distance,
                  const int& bufferPolygonRule,
                  const int& bufferBoundariesRule,
                  const int& levels,
                  te::mem::DataSet* outputDataSet,
                  te::da::DataSetType* outputDataSetType,
                  te::gm::GeomType outGeoType);

bool BufferQuery(const std::string& inDatasetName,
                te::da::DataSource* inDataSource,
                const double& fixedDistance,
                const int& bufferPolygonRule,
                const int& bufferBoundariesRule,
                const int& levels,
                te::mem::DataSet* outputDataSet,
                te::da::DataSetType* outputDataSetType,
                te::gm::GeomType outGeoType);

bool BufferQuery(const std::string& inDatasetName,
                te::da::DataSource* inDataSource,
                const std::string& fromAttDistance,
                const int& bufferPolygonRule,
                const int& bufferBoundariesRule,
                const int& levels,
                te::mem::DataSet* outputDataSet,
                te::da::DataSetType* outputDataSetType,
                te::gm::GeomType outGeoType);

te::mem::DataSet* SetDissolvedBoundaries(te::da::DataSetType* dataSetType, 
                                        te::mem::DataSet* dataset, 
                                        const int& levels);

void PrepareDataSet(te::da::DataSetType* dataSetType, 
                    te::da::DataSet* dataSetQuery,
                    te::mem::DataSet* outputDataSet);
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
  if(dsCapabilities.supportsPreparedQueryAPI() && dsCapabilities.supportsSpatialOperators())
  {
    if(fromAttDistance == "")
      res = BufferQuery(inDatasetName, inDataSource, fixedDistance, bufferPolygonRule, bufferBoundariesRule, levels, outputDataSet.get(), outputDataSetType.get(), te::gm::MultiPolygonType);
    else
      res = BufferQuery(inDatasetName, inDataSource, fromAttDistance, bufferPolygonRule, bufferBoundariesRule, levels, outputDataSet.get(), outputDataSetType.get(), te::gm::MultiPolygonType);
  }
  else
  {
    res = BufferMemory(inDatasetName, inDataSource, fixedDistance, bufferPolygonRule, bufferBoundariesRule, levels, outputDataSet.get(), outputDataSetType.get(), te::gm::MultiPolygonType);
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
  outputDataSet->moveBeforeFirst();
  outDataSource->add(dsTypeResult->getName(),outputDataSet.get(), options);
  
  // create the primary key if it is possible
  //if (outDataSource->getCapabilities().getDataSetTypeCapabilities().supportsPrimaryKey())
  //{
  //  std::string pk_name = dsTypeResult->getName() + "_pk";
  //  te::da::PrimaryKey* pk = new te::da::PrimaryKey(pk_name, dsTypeResult);
  //  pk->add(dsTypeResult->getProperty(0));
  //  outDataSource->addPrimaryKey(outDataSetName,pk);
  //}
  return true;
}

te::da::DataSetType* GetDataSetType(const std::string& inDatasetName,
                                    te::da::DataSource* inDataSource,
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
  
  if(copyInputColumns)
  {
    boost::ptr_vector<te::dt::Property> props = inDataSource->getProperties(inDatasetName);
    boost::ptr_vector<te::dt::Property>::iterator it = props.begin();

    while(it != props.end())
    {
      if(it->getType() != te::dt::GEOMETRY_TYPE)
        dsType->add(it->clone());
    }
  }
  
  std::auto_ptr<te::da::DataSetType> inputDataSetType = inDataSource->getDataSetType(inDatasetName);
  te::gm::GeometryProperty* p = static_cast<te::gm::GeometryProperty*>(te::da::GetFirstGeomProperty(inputDataSetType.get()));

  te::gm::GeometryProperty* geometry = new te::gm::GeometryProperty("geom");
  geometry->setGeometryType(te::gm::GeometryType);
  geometry->setSRID(p->getSRID());
  dsType->add(geometry);

  return dsType;
}

bool BufferMemory(const std::string& inDatasetName,
                  te::da::DataSource* inDataSource,
                  const double& distance,
                  const int& bufferPolygonRule,
                  const int& bufferBoundariesRule,
                  const int& levels,
                  te::mem::DataSet* outputDataSet,
                  te::da::DataSetType* outputDataSetType,
                  te::gm::GeomType outGeoType)
{
  /*std::map<te::gm::Geometry*, double>::const_iterator it = distance.begin();
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
  }*/

  return false;
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
  /*std::map<te::gm::Geometry*, double>::const_iterator it = distance.begin();
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
  }*/

  return false;
}

bool BufferQuery(const std::string& inDatasetName,
                te::da::DataSource* inDataSource,
                const double& distance,
                const int& bufferPolygonRule,
                const int& bufferBoundariesRule,
                const int& levels,
                te::mem::DataSet* outputDataSet,
                te::da::DataSetType* outputDataSetType,
                te::gm::GeomType outGeoType)
{
  std::auto_ptr<te::da::DataSetType> dsType = inDataSource->getDataSetType(inDatasetName);

  te::da::Fields* fields = new te::da::Fields;

  te::gm::GeometryProperty* geom = te::da::GetFirstGeomProperty(dsType.get());

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
      te::da::Expression* e_buffer1 = new te::da::ST_Buffer(te::da::PropertyName(geom->getName()), distance*i);
      te::da::Expression* e_buffer2 = new te::da::ST_Buffer(te::da::PropertyName(geom->getName()), -distance*i);
      e_buffer = new te::da::ST_Difference(e_buffer1, e_buffer2);
      f_buffer = new te::da::Field(*e_buffer, "geom"+index);
    }
    else if(bufferPolygonRule == te::vp::ONLY_OUTSIDE)
    {
      te::da::Expression* e_buffer1 = new te::da::ST_Buffer(te::da::PropertyName(geom->getName()), distance*i);
      te::da::Expression* e_buffer2 = new te::da::ST_Buffer(te::da::PropertyName(geom->getName()), 0);
      e_buffer = new te::da::ST_Difference(e_buffer1, e_buffer2);
      f_buffer = new te::da::Field(*e_buffer, "geom"+index);
    }
    else
    {
      te::da::Expression* e_buffer1 = new te::da::ST_Buffer(te::da::PropertyName(geom->getName()), -distance*i);
      te::da::Expression* e_buffer2 = new te::da::ST_Buffer(te::da::PropertyName(geom->getName()), 0);
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

  PrepareDataSet(outputDataSetType, dsQuery.get(), outputDataSet);

  return true;
}

bool BufferQuery(const std::string& inDatasetName,
                te::da::DataSource* inDataSource,
                const std::string& distance,
                const int& bufferPolygonRule,
                const int& bufferBoundariesRule,
                const int& levels,
                te::mem::DataSet* outputDataSet,
                te::da::DataSetType* outputDataSetType,
                te::gm::GeomType outGeoType)
{
  std::auto_ptr<te::da::DataSetType> dsType = inDataSource->getDataSetType(inDatasetName);

  te::da::Fields* fields = new te::da::Fields;

  te::gm::GeometryProperty* geom = te::da::GetFirstGeomProperty(dsType.get());

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

  PrepareDataSet(outputDataSetType, dsQuery.get(), outputDataSet);

  return true;
}

te::mem::DataSet* SetDissolvedBoundaries(te::da::DataSetType* dataSetType, 
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

void PrepareDataSet(te::da::DataSetType* dataSetType, 
                    te::da::DataSet* dataSetQuery,
                    te::mem::DataSet* outputDataSet)
{
  std::size_t numProps = dataSetQuery->getNumProperties();
  std::size_t firstGeomPos = te::da::GetFirstSpatialPropertyPos(dataSetQuery);
  int numItems = numProps - firstGeomPos;

  dataSetQuery->moveBeforeFirst();
  
  unsigned int type;

  while(dataSetQuery->moveNext())
  {
    int numCurrentItem = 0;

    for(int i = 0; i < numItems; ++i)
    {
      te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(outputDataSet);

      for(int j = 0; j < numProps; ++j)
      {
        type = dataSetQuery->getPropertyDataType(j);

        if(type != te::dt::GEOMETRY_TYPE)
        {

        }
        else
        {
          dataSetItem->setGeometry(j, *dataSetQuery->getGeometry(j+numCurrentItem));
          outputDataSet->add(dataSetItem);

          ++numCurrentItem;
          j = numProps;
        }
      }
    }
  }
}