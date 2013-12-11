
/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/vp/core/IntersectionDialog.cpp

  \brief Group of functions to intersection operation
*/

// Terralib
#include "../common/Exception.h"
#include "../common/progress/TaskProgress.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/datasource/DataSourceInfo.h"
#include "../dataaccess/datasource/DataSourceManager.h"
#include "../dataaccess/dataset/DataSetTypeConverter.h"

#include "../dataaccess/query_h.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/Property.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryCollection.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/MultiLineString.h"
#include "../geometry/MultiPoint.h"
#include "../geometry/MultiPolygon.h"
#include "../maptools/AbstractLayer.h"
#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"
#include "../qt/widgets/layer/utils/DataSet2Layer.h"
#include "Config.h"
#include "Exception.h"
#include "Intersection.h"
#include "Utils.h"

// Boost
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <iostream>
#include <memory>


// -- auxiliary function
struct IntersectionMember
{
  te::da::DataSetType* dt;
  te::da::DataSet* ds;
  std::vector<te::dt::Property*> props;
};

typedef te::sam::rtree::Index<size_t, 8>* DataSetRTree;

bool IntersectionQuery(const std::string& inFirstDataSetName,
                      const std::string& inSecondDataSetName,
                      te::da::DataSource* inFirstDataSource,
                      const std::string& outDataSetName,
                      te::da::DataSetType*& outDataSetType,
                      te::da::DataSet*& outDataSet,
                      const bool& copyInputColumns,
                      size_t outputSRID);

bool IntersectionMemory(const std::string& inFirstDataSetName,
                        te::da::DataSource* inFirstDataSource,
                        const std::string& inSecondDataSetName,
                        te::da::DataSource* inSecondDataSource,
                        const std::string& outDataSetName,
                        te::da::DataSetType*& outDataSetType,
                        te::da::DataSet*& outDataSet,
                        const bool& copyInputColumns,
                        size_t outputSRID);

te::da::DataSetType* CreateDataSetType(std::string newName, 
                                        te::da::DataSetType* firstDt,
                                        std::vector<te::dt::Property*> firstProps, 
                                        te::da::DataSetType* secondDt,
                                        std::vector<te::dt::Property*> secondProps);

DataSetRTree CreateRTree(te::da::DataSetType* dt, te::da::DataSet* ds);

std::pair<te::da::DataSetType*, te::da::DataSet*> PairwiseIntersection(std::string newName, 
                                                                        IntersectionMember firstMember, 
                                                                        IntersectionMember secondMember,
                                                                        std::size_t outputSRID = 0);

std::vector<te::dt::Property*> GetTabularProperties(te::da::DataSetType* dsType);

te::da::DataSet* UpdateGeometryType(te::da::DataSetType* dsType, te::da::DataSet* ds);
// ---

bool te::vp::Intersection(const std::string& inFirstDataSetName,
                          te::da::DataSource* inFirstDataSource,
                          const std::string& inSecondDataSetName,
                          te::da::DataSource* inSecondDataSource,
                          const bool& copyInputColumns,
                          const std::string& outDataSetName,
                          te::da::DataSource* outDataSource)
{
  assert(inFirstDataSource);
  assert(inSecondDataSource);
  assert(outDataSource);

  std::pair<te::da::DataSetType*, te::da::DataSet*> resultPair;
  std::size_t outputSRID = 0;

  te::da::DataSetType* outDataSetType = 0;
  te::da::DataSet* outDataSet = 0;

  const te::da::DataSourceCapabilities firstDSCapabilities = inFirstDataSource->getCapabilities();
  const te::da::DataSourceCapabilities secondDSCapabilities = inSecondDataSource->getCapabilities();

  bool res;
  if( firstDSCapabilities.getQueryCapabilities().supportsSpatialSQLDialect() && 
      secondDSCapabilities.getQueryCapabilities().supportsSpatialSQLDialect() && 
      (inFirstDataSource->getId() == inSecondDataSource->getId()))
  {
    res = IntersectionQuery(inFirstDataSetName,
                            inSecondDataSetName,
                            inFirstDataSource,
                            outDataSetName,
                            outDataSetType,
                            outDataSet,
                            copyInputColumns,
                            outputSRID);
  }
  else
  {
    res = IntersectionMemory(inFirstDataSetName,
                            inFirstDataSource,
                            inSecondDataSetName,
                            inSecondDataSource, 
                            outDataSetName,
                            outDataSetType,
                            outDataSet,
                            copyInputColumns,
                            outputSRID);
  }

  if(!res)
    return false;

  // do any adaptation necessary to persist the output dataset
  te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(outDataSetType, outDataSource->getCapabilities());
  te::da::DataSetType* dsTypeResult = converter->getResult();
  std::auto_ptr<te::da::DataSetAdapter> dsAdapter(te::da::CreateAdapter(outDataSet, converter));
  
  std::map<std::string, std::string> options;
  // create the dataset
  outDataSource->createDataSet(dsTypeResult, options);
  
  // copy from memory to output datasource
  outDataSet->moveBeforeFirst();
  outDataSource->add(dsTypeResult->getName(),outDataSet, options);

  return true;
}

bool IntersectionQuery(const std::string& inFirstDataSetName,
                      const std::string& inSecondDataSetName,
                      te::da::DataSource* inFirstDataSource,
                      const std::string& outDataSetName,
                      te::da::DataSetType*& outDataSetType,
                      te::da::DataSet*& outDataSet,
                      const bool& copyInputColumns,
                      size_t outputSRID)
{
  if(outputSRID == 0)
  {
    std::auto_ptr<te::da::DataSetType> inDataSetType = inFirstDataSource->getDataSetType(inFirstDataSetName);
    te::gm::GeometryProperty* geom = te::da::GetFirstGeomProperty(inDataSetType.get());

    outputSRID = geom->getSRID();
  }

  te::da::Fields* fields = new te::da::Fields;
  te::da::Select* select = new te::da::Select;

  std::auto_ptr<te::da::DataSetType> firstDSType(inFirstDataSource->getDataSetType(inFirstDataSetName));
  std::auto_ptr<te::da::DataSetType> secondDSType(inFirstDataSource->getDataSetType(inSecondDataSetName));

  std::vector<te::dt::Property*> firstProps = GetTabularProperties(firstDSType.get());
  std::vector<te::dt::Property*> secondProps;
  if(copyInputColumns)
    secondProps = GetTabularProperties(secondDSType.get());

  te::gm::GeometryProperty* firstGeom;
  te::gm::GeometryProperty* secondGeom;

  firstGeom = te::da::GetFirstGeomProperty(firstDSType.get());
  secondGeom = te::da::GetFirstGeomProperty(secondDSType.get());
      
  std::string firstTableName = te::vp::GetSimpleTableName(firstDSType->getTitle());
  std::string secondTableName = te::vp::GetSimpleTableName(secondDSType->getTitle());

  for(std::size_t i = 0; i < firstProps.size(); ++i)
  {
    te::da::Field* f_field = new te::da::Field(firstTableName + "." + firstProps[i]->getName() + " ", firstTableName + "_" + firstProps[i]->getName());
    fields->push_back(f_field);
  }

  for(std::size_t i = 0; i < secondProps.size(); ++i)
  {
    te::da::Field* f_field = new te::da::Field(secondTableName + "." + secondProps[i]->getName() + " ", secondTableName + "_" + secondProps[i]->getName());
    fields->push_back(f_field);
  }

  te::da::Expression* e_intersection = new te::da::ST_Intersection( new te::da::PropertyName(firstDSType->getName() + "." + firstGeom->getName()),
                                                                    new te::da::PropertyName(secondDSType->getName() + "." + secondGeom->getName()));
  te::da::Field* f_intersection = new te::da::Field(*e_intersection, "geom");
  fields->push_back(f_intersection);

  te::da::FromItem* firstFromItem = new te::da::DataSetName(firstDSType->getName());
  te::da::FromItem* secondFromItem = new te::da::DataSetName(secondDSType->getName());
  te::da::Expression* e_intersects = new te::da::ST_Intersects( new te::da::PropertyName(firstDSType->getName() + "." + firstGeom->getName()),
                                                                new te::da::PropertyName(secondDSType->getName() + "." + secondGeom->getName()));
  te::da::JoinConditionOn* on = new te::da::JoinConditionOn(e_intersects);
  te::da::Join* join = new te::da::Join(*firstFromItem, *secondFromItem, te::da::INNER_JOIN, *on);

  te::da::From* from = new te::da::From;
  from->push_back(join);

  select->setFields(fields);
  select->setFrom(from);

  std::auto_ptr<te::da::DataSet> dsQuery = inFirstDataSource->query(select);
  dsQuery->moveFirst();

  outDataSetType = CreateDataSetType(outDataSetName, firstDSType.get(), firstProps, secondDSType.get(), secondProps);
  outDataSet = UpdateGeometryType(outDataSetType, dsQuery.release());

  outDataSet->moveBeforeFirst();

  return true;
}

bool IntersectionMemory(const std::string& inFirstDataSetName,
                        te::da::DataSource* inFirstDataSource,
                        const std::string& inSecondDataSetName,
                        te::da::DataSource* inSecondDataSource,
                        const std::string& outDataSetName,
                        te::da::DataSetType*& outDataSetType,
                        te::da::DataSet*& outDataSet,
                        const bool& copyInputColumns,
                        size_t outputSRID)
{
  if(outputSRID == 0)
  {
    std::auto_ptr<te::da::DataSetType> inDataSetType = inFirstDataSource->getDataSetType(inFirstDataSetName);
    te::gm::GeometryProperty* geom = te::da::GetFirstGeomProperty(inDataSetType.get());

    outputSRID = geom->getSRID();
  }

  std::auto_ptr<te::da::DataSetType> firstDSType(inFirstDataSource->getDataSetType(inFirstDataSetName));
  std::auto_ptr<te::da::DataSetType> secondDSType(inSecondDataSource->getDataSetType(inSecondDataSetName));

  std::vector<te::dt::Property*> firstProps = GetTabularProperties(firstDSType.get());

  IntersectionMember firstMember;
  firstMember.dt = firstDSType.release();
  firstMember.ds =  inFirstDataSource->getDataSet(inFirstDataSetName).release();
  firstMember.props = firstProps;

  std::vector<te::dt::Property*> secondProps = GetTabularProperties(secondDSType.get());

  IntersectionMember secondMember;
  secondMember.dt = secondDSType.release();
  secondMember.ds = inSecondDataSource->getDataSet(inSecondDataSetName).release();
  if(copyInputColumns)
    secondMember.props = secondProps;
  
  std::pair<te::da::DataSetType*, te::da::DataSet*> resultPair;
  resultPair = PairwiseIntersection(outDataSetName, firstMember, secondMember, outputSRID);

  if(resultPair.second->size() < 1)
    throw te::common::Exception(TR_VP("The Layers do not intersect!"));

  outDataSetType = resultPair.first;
  outDataSet = resultPair.second;

  return true;
}

te::da::DataSetType* CreateDataSetType(std::string newName, 
                                      te::da::DataSetType* firstDt,
                                      std::vector<te::dt::Property*> firstProps, 
                                      te::da::DataSetType* secondDt,
                                      std::vector<te::dt::Property*> secondProps)
{
  te::da::DataSetType* outputDt = new te::da::DataSetType(newName);
  
  

  te::dt::SimpleProperty* pkProperty = new te::dt::SimpleProperty(newName + "_id", te::dt::INT32_TYPE);
  pkProperty->setAutoNumber(true);
  outputDt->add(pkProperty);
  
  te::da::PrimaryKey* pk = new te::da::PrimaryKey(newName + "_pk", outputDt);
  pk->add(pkProperty);
  outputDt->setPrimaryKey(pk);

  for(size_t i = 0; i < firstProps.size(); ++i)
  {
    te::dt::Property* prop = firstProps[i]->clone();
    if(!firstDt->getTitle().empty())
      prop->setName(te::vp::GetSimpleTableName(firstDt->getTitle()) + "_" + prop->getName());
    outputDt->add(prop);
  }

  for(size_t i = 0; i < secondProps.size(); ++i)
  {
    te::dt::Property* prop = secondProps[i]->clone();
    prop->setName(te::vp::GetSimpleTableName(secondDt->getTitle()) + "_" + prop->getName());
    outputDt->add(prop);
  }

  te::gm::GeomType newType = te::vp::GeomOpResultType(te::da::GetFirstGeomProperty(firstDt)->getGeometryType(), te::da::GetFirstGeomProperty(secondDt)->getGeometryType());

  te::gm::GeometryProperty* newGeomProp = new te::gm::GeometryProperty("geom");
  newGeomProp->setGeometryType(newType);
  newGeomProp->setSRID(te::da::GetFirstGeomProperty(firstDt)->getSRID());
  
  outputDt->add(newGeomProp);

  return outputDt;
}

DataSetRTree CreateRTree(te::da::DataSetType* dt, te::da::DataSet* ds)
{
  DataSetRTree rtree(new te::sam::rtree::Index<size_t, 8>);

  size_t secGeomPropPos = dt->getPropertyPosition(dt->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE));

  size_t secondDsCount = 0;
  while(ds->moveNext())
  {
    std::auto_ptr<te::gm::Geometry> g = ds->getGeometry(secGeomPropPos);

    rtree->insert(*g->getMBR(), secondDsCount);

    ++secondDsCount;
  }

  return rtree;
}

std::pair<te::da::DataSetType*, te::da::DataSet*> PairwiseIntersection(std::string newName, 
                                                                      IntersectionMember firstMember, 
                                                                      IntersectionMember secondMember,
                                                                      std::size_t outputSRID)
{

  //Creating the RTree with the secound layer geometries
  te::sam::rtree::Index<size_t, 8>* rtree = CreateRTree(secondMember.dt, secondMember.ds);

  firstMember.ds->moveBeforeFirst();

  std::auto_ptr<te::gm::GeometryProperty> fiGeomProp (te::da::GetFirstGeomProperty(firstMember.dt));
  size_t fiGeomPropPos = firstMember.dt->getPropertyPosition(fiGeomProp.get());

  size_t secGeomPropPos = secondMember.dt->getPropertyPosition(te::da::GetFirstGeomProperty(secondMember.dt));

  // Create the DataSetType and DataSet
  te::da::DataSetType* outputDt = CreateDataSetType(newName, firstMember.dt, firstMember.props, secondMember.dt, secondMember.props);
  te::mem::DataSet* outputDs = new te::mem::DataSet(outputDt);

  std::pair<te::da::DataSetType*, te::da::DataSet*> resultPair;

  te::common::TaskProgress task("Processing intersection...");
  task.setTotalSteps(firstMember.ds->size());
  task.useTimer(true);

  std::size_t pk = 0;

  while(firstMember.ds->moveNext())
  {
    std::auto_ptr<te::gm::Geometry> currGeom = firstMember.ds->getGeometry(fiGeomPropPos);

    if(currGeom->getSRID() != outputSRID && outputSRID != 0)
      currGeom->transform(outputSRID);

    std::vector<size_t> report;
    rtree->search(*currGeom->getMBR(), report);

    for(size_t i = 0; i < report.size(); ++i)
    {
      secondMember.ds->move(report[i]);
      std::auto_ptr<te::gm::Geometry> secGeom = secondMember.ds->getGeometry(secGeomPropPos);

      if(secGeom->getSRID() != outputSRID && outputSRID != 0)
        secGeom->transform(outputSRID);

      if(!currGeom->intersects(secGeom.get()))
        continue;

      te::mem::DataSetItem* item = new te::mem::DataSetItem(outputDs);
      std::auto_ptr<te::gm::Geometry> resultGeom; 

      if(currGeom->isValid() && secGeom->isValid())
        resultGeom.reset(currGeom->intersection(secGeom.get()));
      
      if(resultGeom.get()!=0 && resultGeom->isValid())
      {
        te::gm::GeometryProperty* fiGeomProp = (te::gm::GeometryProperty*)outputDt->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE);

        if(fiGeomProp->getGeometryType() == te::gm::MultiPolygonType)
        {
          if(resultGeom->getGeomTypeId() == te::gm::MultiPolygonType)
          {
            item->setGeometry("geom", resultGeom.release());
          }
          else if(resultGeom->getGeomTypeId() == te::gm::PolygonType)
          {
            te::gm::MultiPolygon* newGeom = new te::gm::MultiPolygon(0, te::gm::MultiPolygonType, resultGeom->getSRID());
            newGeom->add(resultGeom.release());
            item->setGeometry("geom", newGeom);
          }
        }
        else if(fiGeomProp->getGeometryType() == te::gm::MultiLineStringType)
        {
          if(resultGeom->getGeomTypeId() == te::gm::MultiLineStringType)
          {
            item->setGeometry("geom", resultGeom.release());
          }
          else if(resultGeom->getGeomTypeId() == te::gm::LineStringType)
          {
            te::gm::MultiLineString* newGeom = new te::gm::MultiLineString(0, te::gm::MultiLineStringType, resultGeom->getSRID());
            newGeom->add(resultGeom.release());
            item->setGeometry("geom", newGeom);
          }
        }
        else if(fiGeomProp->getGeometryType() == te::gm::MultiPointType)
        {
          if(resultGeom->getGeomTypeId() == te::gm::MultiPointType)
          {
            item->setGeometry("geom", resultGeom.release());
          }
          else if(resultGeom->getGeomTypeId() == te::gm::PointType)
          {
            te::gm::MultiPoint* newGeom = new te::gm::MultiPoint(0, te::gm::MultiPointType, resultGeom->getSRID());
            newGeom->add(resultGeom.release());
            item->setGeometry("geom", newGeom);
          }
        }
      }
      else
      {
        continue;
      }

      for(size_t j = 0; j < firstMember.props.size(); ++j)
      {
        std::string name = "";

        name = firstMember.props[j]->getName();

        if(!firstMember.dt->getTitle().empty())
          name = firstMember.dt->getTitle() + "_" + firstMember.props[j]->getName();

          te::dt::AbstractData* ad = firstMember.ds->getValue(firstMember.props[j]->getName()).release();

          item->setValue(name, ad);
      }

      for(size_t j = 0; j < secondMember.props.size(); ++j)
      {
        std::string name = secondMember.dt->getTitle() + "_" + secondMember.props[j]->getName();

        te::dt::AbstractData* ad = secondMember.ds->getValue(secondMember.props[j]->getName()).release();

        item->setValue(name, ad);
      }

      item->setInt32(newName + "_id", pk);
      ++pk;

      outputDs->moveNext();

      int aux = te::da::GetFirstSpatialPropertyPos(outputDs);

      if(!item->isNull(aux))
        outputDs->add(item);
    }

    if(task.isActive() == false)
    {
      delete outputDt;
      delete outputDs;

      throw te::common::Exception(TR_VP("Operation canceled!"));
    }

    task.pulse();
  }

  outputDs->moveBeforeFirst();

  resultPair.first = outputDt;
  resultPair.second = outputDs;
  return resultPair;
}

std::vector<te::dt::Property*> GetTabularProperties(te::da::DataSetType* dsType)
{
  std::vector<te::dt::Property*> props;
  te::dt::Property* prop;

  for(std::size_t i = 0; i < dsType->getProperties().size(); ++i)
  {
    prop = dsType->getProperty(i);

    if(prop->getType() != te::dt::GEOMETRY_TYPE && prop->getType() != te::dt::NUMERIC_TYPE) //Remover o numeric da condição quando ajustar o driver do PostGis
    {
      props.push_back(prop);
    }
  }

  return props;
}

te::da::DataSet* UpdateGeometryType(te::da::DataSetType* dsType, te::da::DataSet* ds)
{
  te::mem::DataSet* dsMem = new te::mem::DataSet(dsType);
  te::mem::DataSetItem* dsItem = new te::mem::DataSetItem(dsMem);
  std::size_t pk = 0;
  std::size_t type = 0;
  std::vector<te::dt::Property*> props = dsType->getProperties();

  while(ds->moveNext())
  {
    std::string propName;
    te::mem::DataSetItem* dsItem = new te::mem::DataSetItem(dsMem);

    for(int i=0; i<props.size(); ++i)
    {
      type = props[i]->getType();
      propName = props[i]->getName();

      if(type != te::dt::GEOMETRY_TYPE)
      {
        if(propName == dsType->getName() + "_id")
        {
          dsItem->setInt32(i, pk);
        }
        else
        {
          std::auto_ptr<te::dt::AbstractData> value = ds->getValue(propName);
          dsItem->setValue(i, value.release());
        }
      }
      else
      {
        std::auto_ptr<te::gm::Geometry> geom = ds->getGeometry(propName);
        std::auto_ptr<te::gm::GeometryProperty> geomProp((te::gm::GeometryProperty*)props[i]->clone());

        if(geomProp->getGeometryType() == te::gm::MultiPolygonType)
        {
          if(geom->getGeomTypeId() == te::gm::MultiPolygonType)
          {
            dsItem->setGeometry(i, geom.release());
          }
          else if(geom->getGeomTypeId() == te::gm::PolygonType)
          {
            std::auto_ptr<te::gm::GeometryCollection> newGeom(new te::gm::GeometryCollection(0, te::gm::MultiPolygonType, geom->getSRID()));
            newGeom->add(geom.release());
            dsItem->setGeometry(i, newGeom.release());
          }
        }
        else if(geomProp->getGeometryType() == te::gm::MultiLineStringType)
        {
          if(geom->getGeomTypeId() == te::gm::MultiLineStringType)
          {
            dsItem->setGeometry(i, geom.release());
          }
          else if(geom->getGeomTypeId() == te::gm::LineStringType)
          {
            std::auto_ptr<te::gm::GeometryCollection> newGeom(new te::gm::GeometryCollection(0, te::gm::MultiLineStringType, geom->getSRID()));
            newGeom->add(geom.release());
            dsItem->setGeometry(i, newGeom.release());
          }
        }
        else if(geomProp->getGeometryType() == te::gm::MultiPointType)
        {
          if(geom->getGeomTypeId() == te::gm::MultiPointType)
          {
            dsItem->setGeometry(i, geom.release());
          }
          else if(geom->getGeomTypeId() == te::gm::PointType)
          {
            std::auto_ptr<te::gm::GeometryCollection> newGeom(new te::gm::GeometryCollection(0, te::gm::MultiPointType, geom->getSRID()));
            newGeom->add(geom.release());
            dsItem->setGeometry(i, newGeom.release());
          }
        }
      }
    }
    ++pk;
    dsMem->add(dsItem);
  }
  return dsMem;
}
