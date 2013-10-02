
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

te::da::DataSetType* te::vp::CreateDataSetType(std::string newName, 
                                               te::da::DataSetType* firstDt,
                                               std::vector<te::dt::Property*> firstProps, 
                                               te::da::DataSetType* secondDt,
                                               std::vector<te::dt::Property*> secondProps)
{
  te::da::DataSetType* outputDt = new te::da::DataSetType(newName);

//  size_t fiGeomPropPos = firstDt->getPropertyPosition(firstDt->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE));

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

  te::gm::GeometryProperty* fiGeomProp = te::vp::SetOutputGeometryType(te::da::GetFirstGeomProperty(firstDt), te::da::GetFirstGeomProperty(secondDt));

  te::gm::GeometryProperty* newGeomProp = (te::gm::GeometryProperty*)fiGeomProp->clone();
  outputDt->add(newGeomProp);

  return outputDt;
}

te::vp::DataSetRTree te::vp::CreateRTree(te::da::DataSetType* dt, te::da::DataSet* ds)
{
  te::vp::DataSetRTree rtree(new te::sam::rtree::Index<size_t, 8>);

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

std::vector<te::dt::Property*> GetPropertiesByPos(te::da::DataSetType* dt, std::vector<size_t> propsPos)
{
  std::vector<te::dt::Property*> props;

  for(size_t i = 0; i < propsPos.size(); ++i)
  {
    te::dt::Property* prop = dt->getProperty(propsPos[i]);
    props.push_back(prop);
  }

  return props;
}

te::map::AbstractLayerPtr te::vp::Intersection( const te::map::AbstractLayerPtr& firstLayer,
                                                const te::map::AbstractLayerPtr& secondLayer,
                                                const bool& copyInputColumns,
                                                const std::string& newLayerName,
                                                const te::da::DataSourceInfoPtr& dsinfo)
{
  std::pair<te::da::DataSetType*, te::da::DataSet*> resultPair;
  std::size_t outputSRID = 0;

  te::map::DataSetLayer* firstDSLayer = dynamic_cast<te::map::DataSetLayer*>(firstLayer.get());
  te::da::DataSourcePtr firstDataSource = te::da::GetDataSource(firstDSLayer->getDataSourceId(), true);
  const te::da::DataSourceCapabilities firstDSCapabilities = firstDataSource->getCapabilities();

  te::map::DataSetLayer* secondDSLayer = dynamic_cast<te::map::DataSetLayer*>(secondLayer.get());
  te::da::DataSourcePtr secondDataSource = te::da::GetDataSource(secondDSLayer->getDataSourceId(), true);
  const te::da::DataSourceCapabilities secondDSCapabilities = secondDataSource->getCapabilities();

  if(firstDSCapabilities.supportsSpatialOperators() && secondDSCapabilities.supportsSpatialOperators() && (firstDataSource->getId() == secondDataSource->getId()))
  {
    resultPair = IntersectionQuery(newLayerName, firstLayer, secondLayer, copyInputColumns, outputSRID);
  }
  else
  {
    resultPair = IntersectionMemory(newLayerName, firstLayer, secondLayer, copyInputColumns, outputSRID);
  }

  te::da::DataSourcePtr dataSource = te::da::DataSourceManager::getInstance().get(dsinfo->getId(), dsinfo->getType(), dsinfo->getConnInfo());

  te::da::Create(dataSource.get(), resultPair.first, resultPair.second);

  te::qt::widgets::DataSet2Layer converter(dataSource->getId());

  te::da::DataSetTypePtr dt(dataSource->getDataSetType(resultPair.first->getName()).release());

  te::map::DataSetLayerPtr newLayer = converter(dt);

  delete resultPair.first;
  delete resultPair.second;

  return newLayer;
}

std::pair<te::da::DataSetType*, te::da::DataSet*> te::vp::IntersectionMemory(const std::string& newLayerName,
                                                                            const te::map::AbstractLayerPtr& firstLayer,
                                                                            const te::map::AbstractLayerPtr& secondLayer,
                                                                            const bool& copyInputColumns,
                                                                            size_t outputSRID)
{
  if(outputSRID == 0)
    outputSRID = firstLayer->getSRID();

  std::pair<te::da::DataSetType*, te::da::DataSet*> resultPair;

  te::map::DataSetLayer* firstDSLayer = dynamic_cast<te::map::DataSetLayer*>(firstLayer.get());
  te::map::DataSetLayer* secondDSLayer = dynamic_cast<te::map::DataSetLayer*>(secondLayer.get());

  std::auto_ptr<te::da::DataSetType> firstDSType(firstDSLayer->getSchema().release());
  std::auto_ptr<te::da::DataSetType> secondDSType(secondDSLayer->getSchema().release());

  std::vector<te::dt::Property*> firstProps = GetPropertiesWithoutGeom(firstDSType.get());

  IntersectionMember firstMember;
  firstMember.dt = firstLayer->getSchema().release();
  firstMember.ds = firstLayer->getData().release();
  firstMember.props = firstProps;

  std::vector<te::dt::Property*> secondProps = GetPropertiesWithoutGeom(secondDSType.get());

  IntersectionMember secondMember;
  secondMember.dt = secondLayer->getSchema().release();
  secondMember.ds = secondLayer->getData().release();
  if(copyInputColumns)
    secondMember.props = secondProps;

  resultPair = PairwiseIntersection(newLayerName, firstMember, secondMember, outputSRID);

  if(resultPair.second->size() < 1)
    throw te::common::Exception(TR_VP("The Layers do not intersect!"));

  return resultPair;
}

std::pair<te::da::DataSetType*, te::da::DataSet*> te::vp::IntersectionQuery(const std::string& newLayerName,
                                                                            const te::map::AbstractLayerPtr& firstLayer,
                                                                            const te::map::AbstractLayerPtr& secondLayer,
                                                                            const bool& copyInputColumns,
                                                                            size_t outputSRID)
{
  if(outputSRID == 0)
    outputSRID = firstLayer->getSRID();

  std::pair<te::da::DataSetType*, te::da::DataSet*> resultPair;

  te::map::DataSetLayer* firstDSLayer;
  te::map::DataSetLayer* secondDSLayer;

  te::da::Fields* fields = new te::da::Fields;
  te::da::Select* select = new te::da::Select;

  firstDSLayer = dynamic_cast<te::map::DataSetLayer*>(firstLayer.get());
  secondDSLayer = dynamic_cast<te::map::DataSetLayer*>(secondLayer.get());

  te::da::DataSourcePtr dataSource = te::da::GetDataSource(firstDSLayer->getDataSourceId(), true);

  std::auto_ptr<te::da::DataSetType> firstDSType(firstDSLayer->getSchema().release());
  std::auto_ptr<te::da::DataSetType> secondDSType(secondDSLayer->getSchema().release());

  std::vector<te::dt::Property*> firstProps = GetPropertiesWithoutGeom(firstDSType.get());
  std::vector<te::dt::Property*> secondProps;
  if(copyInputColumns)
    secondProps = GetPropertiesWithoutGeom(secondDSType.get());

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

  std::auto_ptr<te::da::DataSet> dsQuery = dataSource->query(select);
  dsQuery->moveFirst();

  te::da::DataSetType* dsType = CreateDataSetType(newLayerName, firstDSType.get(), firstProps, secondDSType.get(), secondProps);

  resultPair.first = dsType;

  resultPair.second = UpdateGeometryType(dsType, dsQuery.release());

  resultPair.second->moveBeforeFirst();

  return resultPair;
}

std::pair<te::da::DataSetType*, te::da::DataSet*> te::vp::PairwiseIntersection(std::string newName, 
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
      te::gm::Geometry* resultGeom = 0;

      if(currGeom->isValid() && secGeom->isValid())
        resultGeom = currGeom->intersection(secGeom.get());
      
      if(resultGeom && resultGeom->isValid())
      {
        te::gm::GeometryProperty* fiGeomProp = (te::gm::GeometryProperty*)outputDt->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE);

        if(fiGeomProp->getGeometryType() == te::gm::MultiPolygonType)
        {
          if(resultGeom->getGeomTypeId() == te::gm::MultiPolygonType)
          {
            item->setGeometry("geom", *resultGeom);
          }
          else if(resultGeom->getGeomTypeId() == te::gm::PolygonType)
          {
            te::gm::MultiPolygon* newGeom = new te::gm::MultiPolygon(0, te::gm::MultiPolygonType, resultGeom->getSRID());
            newGeom->add(resultGeom);
            item->setGeometry("geom", *newGeom);
          }
        }
        else if(fiGeomProp->getGeometryType() == te::gm::MultiLineStringType)
        {
          if(resultGeom->getGeomTypeId() == te::gm::MultiLineStringType)
          {
            item->setGeometry("geom", *resultGeom);
          }
          else if(resultGeom->getGeomTypeId() == te::gm::LineStringType)
          {
            te::gm::MultiLineString* newGeom = new te::gm::MultiLineString(0, te::gm::MultiLineStringType, resultGeom->getSRID());
            newGeom->add(resultGeom);
            item->setGeometry("geom", *newGeom);
          }
        }
        else if(fiGeomProp->getGeometryType() == te::gm::MultiPointType)
        {
          if(resultGeom->getGeomTypeId() == te::gm::MultiPointType)
          {
            item->setGeometry("geom", *resultGeom);
          }
          else if(resultGeom->getGeomTypeId() == te::gm::PointType)
          {
            te::gm::MultiPoint* newGeom = new te::gm::MultiPoint(0, te::gm::MultiPointType, resultGeom->getSRID());
            newGeom->add(resultGeom);
            item->setGeometry("geom", *newGeom);
          }
        }
      }
      else
      {
        //Emitir uma msg para o usuário dizendo que há geometrias invalidas, se ele deseja continuar mesmo assim!
        std::cout << "GEOMETRIA INVALIDA!\n";
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

      outputDs->moveNext();

      int aux = te::da::GetFirstSpatialPropertyPos(outputDs);

      if(!item->isNull(aux))
        outputDs->add(item);
    }
    
    if(task.isActive() == false)
    {
      delete outputDt;
      delete outputDs;

      throw Exception(TR_VP("Operation canceled!"));
    }

    task.pulse();
  }

  outputDs->moveBeforeFirst();

  resultPair.first = outputDt;
  resultPair.second = outputDs;
  return resultPair;
}

std::vector<te::dt::Property*> te::vp::GetPropertiesWithoutGeom(te::da::DataSetType* dsType)
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

te::da::DataSet* te::vp::UpdateGeometryType(te::da::DataSetType* dsType, te::da::DataSet* ds)
{
  te::gm::Geometry* geom = 0;
  te::mem::DataSet* dsMem = new te::mem::DataSet(*ds, true);

  std::size_t i = te::da::GetFirstSpatialPropertyPos(ds);

  te::gm::GeometryProperty* geomProp = (te::gm::GeometryProperty*)dsType->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE);
  dsMem->moveBeforeFirst();

  if(geomProp->getGeometryType() == te::gm::MultiPolygonType)
  {
    while(dsMem->moveNext())
    {
      if(dsMem->getGeometry(i)->getGeomTypeId() == te::gm::MultiPolygonType)
      {
        continue;
      }
      else if(dsMem->getGeometry(i)->getGeomTypeId() == te::gm::PolygonType)
      {
        te::gm::MultiPolygon* newGeom = new te::gm::MultiPolygon(0, te::gm::MultiPolygonType, dsMem->getGeometry(i)->getSRID());
        newGeom->add(dsMem->getGeometry(i).release());
        dsMem->setGeometry(i, *newGeom);
      }
    }
  }
  else if(geomProp->getGeometryType() == te::gm::MultiLineStringType)
  {
    while(dsMem->moveNext())
    {
      if(dsMem->getGeometry(i)->getGeomTypeId() == te::gm::MultiLineStringType)
      {
        continue;
      }
      else if(dsMem->getGeometry(i)->getGeomTypeId() == te::gm::LineStringType)
      {
        te::gm::MultiLineString* newGeom = new te::gm::MultiLineString(0, te::gm::MultiLineStringType, dsMem->getGeometry(i)->getSRID());
        newGeom->add(dsMem->getGeometry(i).release());
        dsMem->setGeometry(i, *newGeom);
      }
    }
  }
  else if(geomProp->getGeometryType() == te::gm::MultiPointType)
  {
    while(dsMem->moveNext())
    {
      if(dsMem->getGeometry(i)->getGeomTypeId() == te::gm::MultiPointType)
      {
        continue;
      }
      else if(dsMem->getGeometry(i)->getGeomTypeId() == te::gm::PointType)
      {
        te::gm::MultiPoint* newGeom = new te::gm::MultiPoint(0, te::gm::MultiPointType, dsMem->getGeometry(i)->getSRID());
        newGeom->add(dsMem->getGeometry(i).release());
        dsMem->setGeometry(i, *newGeom);
      }
    }
  }

  return dsMem;
}