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
#include "../../common/Translator.h"
#include "../../common/Exception.h"
#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetPersistence.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/dataset/DataSetTypePersistence.h"
#include "../../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/Property.h"
#include "../../memory/DataSet.h"
#include "../../memory/DataSetItem.h"
#include "../../geometry/Geometry.h"
#include "../../geometry/GeometryProperty.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "Config.h"
#include "Exception.h"
#include "Intersection.h"

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

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
      prop->setName(firstDt->getTitle() + "_" + prop->getName());
    outputDt->add(prop);
  }

  for(size_t i = 0; i < secondProps.size(); ++i)
  {
    te::dt::Property* prop = secondProps[i]->clone();
    prop->setName(secondDt->getTitle() + "_" + prop->getName());
    outputDt->add(prop);
  }

  te::gm::GeometryProperty* fiGeomProp = te::da::GetFirstGeomProperty(firstDt);

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
    te::gm::Geometry* g = ds->getGeometry(secGeomPropPos);

    rtree->insert(*g->getMBR(), secondDsCount);

    delete g;

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

te::map::AbstractLayerPtr te::vp::Intersection(const std::string& newLayerName,
                                               const std::vector<LayerInputData>& idata,
                                               const te::da::DataSourceInfoPtr& dsinfo,
                                               size_t outputSRID,
                                               const std::map<std::string, std::string>& options)
{
  std::pair<te::da::DataSetType*, te::da::DataSet*> resultPair;
  resultPair = Intersection(newLayerName, idata, outputSRID);

  te::da::DataSourcePtr dataSource = te::da::DataSourceManager::getInstance().get(dsinfo->getId(), dsinfo->getType(), dsinfo->getConnInfo());

  te::da::DataSourceTransactor* t = dataSource->getTransactor();

  te::da::Create(t, resultPair.first, resultPair.second, options);

  te::qt::widgets::DataSet2Layer converter(dataSource->getId());

  te::da::DataSourceCatalogLoader* loader = t->getCatalogLoader();
  te::da::DataSetTypePtr dstPtr(loader->getDataSetType(resultPair.first->getName()));

  delete t;
  delete loader;

  te::map::DataSetLayerPtr newLayer = converter(dstPtr);

  return newLayer;
}

te::map::AbstractLayerPtr te::vp::Intersection(const std::string& newLayerName,
                                               const std::vector<LayerInputData>& idata,
                                               std::string outputArchive,
                                               size_t outputSRID,
                                               const std::map<std::string, std::string>& options)
{
  std::pair<te::da::DataSetType*, te::da::DataSet*> resultPair;
  resultPair = Intersection(newLayerName, idata, outputSRID);

  static boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();
  std::string id = boost::uuids::to_string(u);

  std::map<std::string, std::string> conn;
  conn["connection_string"] = outputArchive;

  te::da::DataSource* dataSource = te::da::DataSource::create("OGR", conn);

  te::da::DataSourceTransactor* t = dataSource->getTransactor();

  te::da::Create(t, resultPair.first, resultPair.second, options);

  te::qt::widgets::DataSet2Layer converter(dataSource->getId());

  te::da::DataSourceCatalogLoader* loader = t->getCatalogLoader();
  te::da::DataSetTypePtr dstPtr(loader->getDataSetType(resultPair.first->getName()));

  delete t;
  delete loader;

  te::map::DataSetLayerPtr newLayer = converter(dstPtr);

  return newLayer;
}

std::pair<te::da::DataSetType*, te::da::DataSet*> te::vp::Intersection(const std::string& newLayerName,
  const std::vector<LayerInputData>& idata,
  size_t outputSRID)
{
  if(idata.size() <= 1)
    throw te::common::Exception(TR_VP("At least two layers are necessary for an intersection!"));

  if(outputSRID == 0)
    outputSRID = idata.begin()->first->getSRID();

  size_t countAux = 0;
  LayerInputData aux;
  std::pair<te::da::DataSetType*, te::da::DataSet*> resultPair;

  for(size_t i = 0; i < idata.size(); ++i)
  {
    if(i == 0)
    {
      aux = idata[i];
      ++countAux;
      continue;
    }

    te::map::AbstractLayerPtr secondLayer = idata[i].first;
    std::vector<size_t> secondPropsPos = idata[i].second;

    IntersectionMember secondMember;
    secondMember.dt = (te::da::DataSetType*)secondLayer->getSchema();
    secondMember.ds = secondLayer->getData();
    secondMember.props = GetPropertiesByPos(secondMember.dt, secondPropsPos);

    if(countAux == 1)
    {
      std::vector<size_t> firstPropsPos = aux.second;

      IntersectionMember firstMember;
      firstMember.dt = (te::da::DataSetType*)aux.first->getSchema();
      firstMember.ds = aux.first->getData();
      firstMember.props = GetPropertiesByPos(firstMember.dt, firstPropsPos);

      resultPair = PairwiseIntersection(newLayerName, firstMember, secondMember, outputSRID);
    }
    else if(countAux > 1)
    {
      std::vector<te::dt::Property*> resultProps = resultPair.first->getProperties();
      std::vector<te::dt::Property*> auxProps;

      for(size_t i = 0; i < resultProps.size(); ++i)
      {
        if(resultProps[i] != resultPair.first->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE))
          auxProps.push_back(resultProps[i]);
      }

      IntersectionMember auxMember;
      auxMember.dt = resultPair.first;
      auxMember.ds = resultPair.second;
      auxMember.props = auxProps;

      resultPair = PairwiseIntersection(newLayerName, auxMember, secondMember, outputSRID);
    }

    ++countAux;
  }

  if(resultPair.second->size() < 1)
    throw te::common::Exception(TR_VP("The Layers do not intersect!"));

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

  te::gm::GeometryProperty* fiGeomProp = te::da::GetFirstGeomProperty(firstMember.dt);
  size_t fiGeomPropPos = firstMember.dt->getPropertyPosition(fiGeomProp);

  size_t secGeomPropPos = secondMember.dt->getPropertyPosition(te::da::GetFirstGeomProperty(secondMember.dt));

  // Create the DataSetType and DataSet
  te::da::DataSetType* outputDt = CreateDataSetType(newName, firstMember.dt, firstMember.props, secondMember.dt, secondMember.props);
  te::mem::DataSet* outputDs = new te::mem::DataSet(outputDt);

  while(firstMember.ds->moveNext())
  {
    te::gm::Geometry* currGeom = firstMember.ds->getGeometry(fiGeomPropPos);

    if(currGeom->getSRID() != outputSRID)
      currGeom->transform(outputSRID);

    std::vector<size_t> report;
    rtree->search(*currGeom->getMBR(), report);

    for(size_t i = 0; i < report.size(); ++i)
    {
      secondMember.ds->move(report[i]);
      te::gm::Geometry* secGeom = secondMember.ds->getGeometry(secGeomPropPos);

      if(secGeom->getSRID() != outputSRID)
        secGeom->transform(outputSRID);

      if(!currGeom->intersects(secGeom))
        continue;

      te::mem::DataSetItem* item = new te::mem::DataSetItem(outputDs);
      te::gm::Geometry* newGeom = currGeom->intersection(secGeom);

      item->setGeometry(fiGeomProp->getName(), *newGeom);

      for(size_t j = 0; j < firstMember.props.size(); ++j)
      {
        std::string name = "";

        name = firstMember.props[j]->getName();

        if(!firstMember.dt->getTitle().empty())
          name = firstMember.dt->getTitle() + "_" + firstMember.props[j]->getName();

        if(!firstMember.ds->isNull(firstMember.props[j]->getName()))
        {
          te::dt::AbstractData* ad = firstMember.ds->getValue(firstMember.props[j]->getName());

          item->setValue(name, ad);
        }
      }

      for(size_t j = 0; j < secondMember.props.size(); ++j)
      {
        std::string name = secondMember.dt->getTitle() + "_" + secondMember.props[j]->getName();

        if(!secondMember.ds->isNull(secondMember.props[j]->getName()))
        {
          te::dt::AbstractData* ad = secondMember.ds->getValue(secondMember.props[j]->getName());

          item->setValue(name, ad);
        }
      }

      outputDs->moveNext();
      outputDs->add(item);

    }
  }

  std::pair<te::da::DataSetType*, te::da::DataSet*> resultPair;
  resultPair.first = outputDt;
  resultPair.second = outputDs;
  return resultPair;
}