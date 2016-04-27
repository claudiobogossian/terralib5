/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
 \file Intersection.cpp
 */

#include "../common/Translator.h"
#include "../common/Logger.h"

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetAdapter.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/DataSetTypeConverter.h"
#include "../dataaccess/dataset/ObjectIdSet.h"

#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceInfo.h"
#include "../dataaccess/datasource/DataSourceInfoManager.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/datasource/DataSourceTransactor.h"

#include "../dataaccess/query/And.h"
#include "../dataaccess/query/DataSetName.h"
#include "../dataaccess/query/Expression.h"
#include "../dataaccess/query/Field.h"
#include "../dataaccess/query/Fields.h"
#include "../dataaccess/query/From.h"
#include "../dataaccess/query/FromItem.h"
#include "../dataaccess/query/Insert.h"
#include "../dataaccess/query/Join.h"
#include "../dataaccess/query/JoinConditionOn.h"
#include "../dataaccess/query/PropertyName.h"
#include "../dataaccess/query/Select.h"
#include "../dataaccess/query/SubSelect.h"
#include "../dataaccess/query/ST_Intersection.h"
#include "../dataaccess/query/ST_Intersects.h"
#include "../dataaccess/query/ST_Multi.h"
#include "../dataaccess/query/Where.h"

#include "../dataaccess/utils/Utils.h"

#include "../datatype/Property.h"
#include "../datatype/SimpleData.h"
#include "../datatype/StringProperty.h"

#include "../geometry/Geometry.h"
#include "../geometry/GeometryCollection.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/MultiLineString.h"
#include "../geometry/MultiPoint.h"
#include "../geometry/MultiPolygon.h"
#include "../geometry/Utils.h"

#include "../sam.h"

#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"

#include "../statistics/core/Utils.h"

#include "AlgorithmParams.h"
#include "ComplexData.h"
#include "Intersection.h"
#include "Utils.h"

// BOOST
#include <boost/lexical_cast.hpp>

te::vp::Intersection::Intersection()
{
}

bool te::vp::Intersection::executeMemory(te::vp::AlgorithmParams* mainParams)
{
  te::vp::ValidateAlgorithmParams(mainParams, te::vp::MEMORY);

  te::vp::InputParams firstInputParams = mainParams->getInputParams()[0];
  te::vp::InputParams secondInputParams = mainParams->getInputParams()[1];

  te::da::DataSourcePtr outputDataSource = mainParams->getOutputDataSource();

  const std::map<std::string, te::dt::AbstractData*> specificParams = mainParams->getSpecificParams();

  bool isCollection = this->isCollection(specificParams);

  te::da::DataSourcePtr     firstDataSource = firstInputParams.m_inputDataSource;
  te::da::DataSetType*      firstDataSetType = firstInputParams.m_inputDataSetType;
  te::da::DataSet*          firstDataSet = firstInputParams.m_inputDataSet;
  te::gm::GeometryProperty* firstGeometryProperty = te::da::GetFirstGeomProperty(firstDataSetType);

  te::da::DataSourcePtr     secondDataSource = secondInputParams.m_inputDataSource;
  te::da::DataSetType*      secondDataSetType = secondInputParams.m_inputDataSetType;
  te::da::DataSet*          secondDataSet = secondInputParams.m_inputDataSet;
  te::gm::GeometryProperty* secondGeometryProperty = te::da::GetFirstGeomProperty(secondDataSetType);

  std::auto_ptr<te::da::DataSetType> outputDataSetType(getOutputDataSetType(mainParams));
  std::auto_ptr<te::mem::DataSet>    outputDataSet(new te::mem::DataSet(outputDataSetType.get()));
  te::gm::GeometryProperty*          outputGeometryProperty = te::da::GetFirstGeomProperty(outputDataSetType.get());

  te::sam::rtree::Index<size_t, 8>* rtree = te::vp::GetRtree(secondDataSet);

  std::vector<te::dt::Property*> firstSelectedProperties  = getFirstSelectedProperties(firstDataSetType, specificParams);
  std::vector<te::dt::Property*> secondSelectedProperties = getSecondSelectedProperties(secondDataSetType, specificParams);

  firstDataSet->moveBeforeFirst();
  while (firstDataSet->moveNext())
  {
    std::auto_ptr<te::mem::DataSetItem> item(new te::mem::DataSetItem(outputDataSet.get()));

    std::auto_ptr<te::gm::Geometry> currentGeometry = firstDataSet->getGeometry(firstGeometryProperty->getName());

    std::vector<std::size_t> rtreeReport;
    rtree->search(*currentGeometry->getMBR(), rtreeReport);

    for (std::size_t i = 0; i < rtreeReport.size(); ++i)
    {
      secondDataSet->move(rtreeReport[i]);

      std::auto_ptr<te::gm::Geometry> candidateGeometry = secondDataSet->getGeometry(secondGeometryProperty->getName());

      if (!currentGeometry->intersects(candidateGeometry.get()))
        continue;

      te::gm::Geometry* resultingGeometry = currentGeometry->intersection(candidateGeometry.get());

      if (!resultingGeometry || !resultingGeometry->isValid())
      {
#ifdef TERRALIB_LOGGER_ENABLED
        te::common::Logger::logDebug("vp", "Intersection - Invalid geometry found");
#endif //TERRALIB_LOGGER_ENABLED
        continue;
      }

      for (std::map<std::string, std::string>::iterator it = m_firstAttrNameMap.begin(); it != m_firstAttrNameMap.end(); ++it)
      {
        if(!firstDataSet->isNull(it->second))
          item->setValue(it->first, firstDataSet->getValue(it->second).release());
      }

      for (std::map<std::string, std::string>::iterator it = m_secondAttrNameMap.begin(); it != m_secondAttrNameMap.end(); ++it)
      {
        if (!secondDataSet->isNull(it->second))
          item->setValue(it->first, secondDataSet->getValue(it->second).release());
      }

      if (!resultingGeometry->isEmpty())
      {
        std::vector<te::gm::Geometry*> geomVec;

        if (isCollection)
          geomVec.push_back(resultingGeometry);
        else
          te::gm::Multi2Single(resultingGeometry, geomVec);

        for (std::size_t g = 0; g < geomVec.size(); ++g)
        {
          if (!te::vp::IsMultiType(geomVec[g]->getGeomTypeId()) && isCollection)
            geomVec[g] = te::vp::SetGeomAsMulti(geomVec[g]);

          std::auto_ptr<te::mem::DataSetItem> currentItem = item->clone();
          currentItem->setGeometry(outputGeometryProperty->getName(), geomVec[g]);

          outputDataSet->add(currentItem.release());
        }
      }

      outputDataSet->moveNext();

      std::size_t outputDsGeomePropPosition = te::da::GetFirstSpatialPropertyPos(outputDataSet.get());

      if (!item->isNull(outputDsGeomePropPosition))
        outputDataSet->add(item.release());
    }
  }

  outputDataSet->moveBeforeFirst();

  std::auto_ptr<te::da::DataSet> dataSetPrepared = PrepareAdd(outputDataSet.release(), outputDataSetType.get());

  if (!dataSetPrepared.get())
    throw te::common::Exception(TE_TR("Output DataSet was not prepared to save."));

  if (dataSetPrepared->size() == 0)
    throw te::common::Exception("The resultant layer is empty!");

  te::vp::Save(outputDataSource.get(), dataSetPrepared.get(), outputDataSetType.get());

  return true;
}

std::vector<te::dt::Property*> te::vp::Intersection::getFirstSelectedProperties(const te::da::DataSetType* dataSetType, const std::map<std::string, te::dt::AbstractData*>& specificParams)
{
  std::vector<te::dt::Property*> result;

  std::vector<std::pair<std::string, std::string> > selectedProperties = getSelectedProperties(specificParams);

  for (std::size_t i = 0; i < selectedProperties.size(); ++i)
  {
    if (selectedProperties[i].first == "FIRST")
    {
      result.push_back(dataSetType->getProperty(selectedProperties[i].second));
    }
  }

  return result;
}

std::vector<te::dt::Property*> te::vp::Intersection::getSecondSelectedProperties(const te::da::DataSetType* dataSetType, const std::map<std::string, te::dt::AbstractData*>& specificParams)
{
  std::vector<te::dt::Property*> result;

  std::vector<std::pair<std::string, std::string> > selectedProperties = getSelectedProperties(specificParams);

  for (std::size_t i = 0; i < selectedProperties.size(); ++i)
  {
    if (selectedProperties[i].first == "SECOND")
    {
      result.push_back(dataSetType->getProperty(selectedProperties[i].second));
    }
  }

  return result;
}

std::vector<std::pair<std::string, std::string> > te::vp::Intersection::getSelectedProperties(const std::map<std::string, te::dt::AbstractData*>& specificParams)
{
  std::vector<std::pair<std::string, std::string> > result;

  for (std::map<std::string, te::dt::AbstractData*>::const_iterator it = specificParams.begin(); it != specificParams.end(); ++it)
  {
    if (it->first == "ATTRIBUTES")
    {
      te::vp::ComplexData<std::vector<std::pair<std::string, std::string> > >* cd =
        dynamic_cast<te::vp::ComplexData<std::vector<std::pair<std::string, std::string> > >* >(it->second);

      if (cd)
        result = cd->getValue();
    }
  }

  return result;
}

bool te::vp::Intersection::isCollection(const std::map<std::string, te::dt::AbstractData*>& specificParams)
{
  bool isCollection = false;

  for (std::map<std::string, te::dt::AbstractData*>::const_iterator it = specificParams.begin(); it != specificParams.end(); ++it)
  {
    if (it->first != "IS_COLLECTION")
    {
      continue;
    }

    te::dt::SimpleData<bool, te::dt::BOOLEAN_TYPE>* sd =
      dynamic_cast<te::dt::SimpleData<bool, te::dt::BOOLEAN_TYPE >* >(it->second);

    if (sd)
      isCollection = sd->getValue();
  }

  return isCollection;
}

bool te::vp::Intersection::executeQuery(te::vp::AlgorithmParams* mainParams)
{
  
  bool isCollection;

  //*TESTE SPECIFC PARAM*//
  //std::map<std::string, te::dt::AbstractData*> specificParams = mainParams->getSpecificParams();
  //std::map<std::string, te::dt::AbstractData*>::iterator it = specificParams.begin();
  //while (it != specificParams.end())
  //{
  //  std::string key = it->first;
  //  int type = it->second->getTypeCode();

  //  te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>* sd = dynamic_cast< te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>* >(it->second);
  //  double value = sd->getValue();
  //  ++it;
  //}
  //*TESTE SPECIFC PARAM*//

  std::vector<te::vp::InputParams> inputParams = mainParams->getInputParams();

  if (inputParams.size() < 2)
  {
    //Return a message: It is necessary more than one item for performing the operation
    return false;
  }

// Verify if input parameters has two entry, if so, verify if both are the same and set as true the self intersection check.
  bool selfIntersection = false;

  if (inputParams.size() == 2)
  {
    std::string table1 = inputParams[0].m_inputDataSetName;
    std::string table2 = inputParams[1].m_inputDataSetName;

    if (table1 == table2)
      selfIntersection = true;
  }


// Build Query to Intersect data.
  te::da::Fields* fields = new te::da::Fields;
  
  std::auto_ptr<te::da::DataSetType> dsType_first;
  std::auto_ptr<te::da::DataSetType> dsType_second;

  te::gm::GeometryProperty* geom_first;
  te::gm::GeometryProperty* geom_second;

  dsType_first = inputParams[0].m_inputDataSource->getDataSetType(inputParams[0].m_inputDataSetName);
  geom_first = te::da::GetFirstGeomProperty(dsType_first.get());

  dsType_second = inputParams[1].m_inputDataSource->getDataSetType(inputParams[1].m_inputDataSetName);
  geom_second = te::da::GetFirstGeomProperty(dsType_second.get());

  te::da::Expression* e_intersection = new te::da::ST_Intersection(
    new te::da::PropertyName(dsType_first->getName() + "." + geom_first->getName()),
    new te::da::PropertyName(dsType_second->getName() + "." + geom_second->getName()));

  te::da::Expression* e_multi = new te::da::ST_Multi(*e_intersection);

  te::da::Field* f_intersection = new te::da::Field(*e_multi, "geom");

  fields->push_back(f_intersection);

  te::da::FromItem* firstFromItem = new te::da::DataSetName(dsType_first->getName());
  te::da::FromItem* secondFromItem = new te::da::DataSetName(dsType_second->getName());

  te::da::Expression* e_intersects = new te::da::ST_Intersects(
    new te::da::PropertyName(dsType_first->getName() + "." + geom_first->getName()),
    new te::da::PropertyName(dsType_second->getName() + "." + geom_second->getName()));

  te::da::JoinConditionOn* on = new te::da::JoinConditionOn(e_intersects);
  te::da::Join* join = new te::da::Join(*firstFromItem, *secondFromItem, te::da::INNER_JOIN, *on);

  te::da::From* from = new te::da::From;
  from->push_back(join);

  te::da::Select* mainSelect = new te::da::Select(fields, from);

/*If the input parameters size is greater than two, add the previous 
  select query into a subselect and builds a new external select to query.*/
  for (std::size_t i = 2; i < inputParams.size(); ++i)
  {
    te::da::SubSelect* subSelect = new te::da::SubSelect(mainSelect, "Intersection" + boost::lexical_cast<std::string>(i));

    te::da::Fields* external_fields = new te::da::Fields;
    std::auto_ptr<te::da::DataSetType> external_dsType = inputParams[i].m_inputDataSource->getDataSetType(inputParams[i].m_inputDataSetName);
    te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(external_dsType.get());

    te::da::Expression* e_externalIntersection = new te::da::ST_Intersection(
      new te::da::PropertyName(external_dsType->getName() + "." + geomProp->getName()),
      new te::da::PropertyName(subSelect->getAlias() + ".geom"));

    te::da::Expression* e_externalMulti = new te::da::ST_Multi(*e_externalIntersection);

    te::da::Field* f_externalIntersection = new te::da::Field(*e_externalMulti, "geom");
    external_fields->push_back(f_externalIntersection);

    te::da::FromItem* externalFromItem = new te::da::DataSetName(external_dsType->getName());
    te::da::FromItem* internalFromItem = new te::da::SubSelect(*subSelect);

    te::da::Expression* e_externalIntersects = new te::da::ST_Intersects(
      new te::da::PropertyName(external_dsType->getName() + "." + geomProp->getName()),
      new te::da::PropertyName(subSelect->getAlias() + ".geom"));

    te::da::JoinConditionOn* externalOn = new te::da::JoinConditionOn(e_externalIntersects);
    te::da::Join* externalJoin = new te::da::Join(*externalFromItem, *internalFromItem, te::da::INNER_JOIN, *externalOn);

    te::da::From* externalFrom = new te::da::From;
    externalFrom->push_back(externalJoin);

    mainSelect = new te::da::Select(external_fields, externalFrom);
  }

/*Check if the input and output dataSource are the same, if so, 
  persists the result of select query into database with insert command.*/
  te::da::DataSourcePtr outputDataSource = mainParams->getOutputDataSource();

  te::da::DataSourceInfoPtr inDataSourceInfoPtr = te::da::DataSourceInfoManager::getInstance().get(inputParams[0].m_inputDataSource->getId());
  te::da::DataSourceInfoPtr outDataSourceInfoPtr = te::da::DataSourceInfoManager::getInstance().get(outputDataSource->getId());

// Create output dataset
  std::auto_ptr<te::da::DataSourceTransactor> t = outputDataSource->getTransactor();
  std::map<std::string, std::string> options;
  t->begin();
  t->createDataSet(getOutputDataSetType(mainParams), options);
  t->commit();

  if (!inDataSourceInfoPtr)
  {
    t->rollBack();
    return false;
  }

  if (!outDataSourceInfoPtr)
  {
    t->rollBack();
    return false;
  }

  std::string inputConnection = inDataSourceInfoPtr->getConnInfoAsString();
  std::string outputConnection = outDataSourceInfoPtr->getConnInfoAsString();

// Execute Query
  if (inputConnection == outputConnection)
  {
    te::da::Fields* insertFields = new te::da::Fields;
    te::da::Field* f_insert = new te::da::Field("geom");
    insertFields->push_back(f_insert);

    te::da::Insert* insert = new te::da::Insert(new te::da::DataSetName(mainParams->getOutputDataSetName()), insertFields, mainSelect);
    inputParams[0].m_inputDataSource->execute(*insert);
  }
  else
  {
    std::auto_ptr<te::da::DataSet> dsQuery = inputParams[0].m_inputDataSource->query(mainSelect);
    dsQuery->moveBeforeFirst();

    if (dsQuery->size() == 0)
      throw te::common::Exception("The Layers do not intersect!");

    std::string outputDsName = mainParams->getOutputDataSetName();

    std::auto_ptr<te::da::DataSetType> outDataSetType(new te::da::DataSetType(outputDsName));

    te::dt::SimpleProperty* pkProperty = new te::dt::SimpleProperty(outputDsName + "_id", te::dt::INT32_TYPE);
    pkProperty->setAutoNumber(true);
    outDataSetType->add(pkProperty);

    te::da::PrimaryKey* pk = new te::da::PrimaryKey(outputDsName + "_pk", outDataSetType.get());
    pk->add(pkProperty);
    outDataSetType->setPrimaryKey(pk);

    te::gm::GeomType newType = getGeomResultType(te::da::GetFirstGeomProperty(dsType_first.get())->getGeometryType(),
                                                te::da::GetFirstGeomProperty(dsType_second.get())->getGeometryType(), isCollection);

    te::gm::GeometryProperty* newGeomProp = new te::gm::GeometryProperty("geom");
    newGeomProp->setGeometryType(newType);

    outDataSetType->add(newGeomProp);

    std::auto_ptr<te::da::DataSet> outDset(updateGeomType(outDataSetType.get(), dsQuery.release()));

    outDset->moveBeforeFirst();

    t->add(outputDsName, outDset.get(), options);
    t->commit();
  }

  
  return true;
}

te::da::DataSet* te::vp::Intersection::updateGeomType(te::da::DataSetType* dsType, te::da::DataSet* ds)
{
  te::mem::DataSet* dsMem = new te::mem::DataSet(dsType);

  std::size_t type = 0;
  std::vector<te::dt::Property*> props = dsType->getProperties();

  int pk = 0;
  while (ds->moveNext())
  {
    std::string propName;
    te::mem::DataSetItem* dsItem = new te::mem::DataSetItem(dsMem);

    dsItem->setInt32(0, pk);

    for (std::size_t i = 1; i < props.size(); ++i)
    {
      type = props[i]->getType();
      propName = props[i]->getName();

      if (type != te::dt::GEOMETRY_TYPE)
      {
        if (!ds->isNull(propName))
        {
          std::auto_ptr<te::dt::AbstractData> value = ds->getValue(propName);
          dsItem->setValue(i, value.release());
        }
      }
      else
      {
        std::auto_ptr<te::gm::Geometry> geom = ds->getGeometry(propName);
        std::auto_ptr<te::gm::GeometryProperty> geomProp((te::gm::GeometryProperty*)props[i]->clone());

        if (geomProp->getGeometryType() == te::gm::MultiPolygonType)
        {
          if (geom->getGeomTypeId() == te::gm::MultiPolygonType)
          {
            dsItem->setGeometry(i, geom.release());
          }
          else if (geom->getGeomTypeId() == te::gm::PolygonType)
          {
            std::auto_ptr<te::gm::GeometryCollection> newGeom(new te::gm::GeometryCollection(0, te::gm::MultiPolygonType, geom->getSRID()));
            newGeom->add(geom.release());
            dsItem->setGeometry(i, newGeom.release());
          }
        }
        else if (geomProp->getGeometryType() == te::gm::MultiLineStringType)
        {
          if (geom->getGeomTypeId() == te::gm::MultiLineStringType)
          {
            dsItem->setGeometry(i, geom.release());
          }
          else if (geom->getGeomTypeId() == te::gm::LineStringType)
          {
            std::auto_ptr<te::gm::GeometryCollection> newGeom(new te::gm::GeometryCollection(0, te::gm::MultiLineStringType, geom->getSRID()));
            newGeom->add(geom.release());
            dsItem->setGeometry(i, newGeom.release());
          }
        }
        else if (geomProp->getGeometryType() == te::gm::MultiPointType)
        {
          if (geom->getGeomTypeId() == te::gm::MultiPointType)
          {
            dsItem->setGeometry(i, geom.release());
          }
          else if (geom->getGeomTypeId() == te::gm::PointType)
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

te::da::DataSetType* te::vp::Intersection::getOutputDataSetType(te::vp::AlgorithmParams* mainParams)
{
  te::vp::InputParams firstInputParams = mainParams->getInputParams()[0];
  te::vp::InputParams secondInputParams = mainParams->getInputParams()[1];

  std::string outputDataSetName = mainParams->getOutputDataSetName();

  const std::map<std::string, te::dt::AbstractData*> specificParams = mainParams->getSpecificParams();

  bool isCollection = this->isCollection(specificParams);

  te::da::DataSetType*      firstDataSetType = firstInputParams.m_inputDataSetType;
  te::gm::GeometryProperty* firstGeometryProperty = te::da::GetFirstGeomProperty(firstDataSetType);
  
  te::da::DataSetType*      secondDataSetType = secondInputParams.m_inputDataSetType;
  te::gm::GeometryProperty* secondGeometryProperty = te::da::GetFirstGeomProperty(secondDataSetType);

  std::vector<te::dt::Property*> firstSelectedProperties = getFirstSelectedProperties(firstDataSetType, specificParams);
  std::vector<te::dt::Property*> secondSelectedProperties = getSecondSelectedProperties(secondDataSetType, specificParams);

  te::da::DataSetType* outputDataSetType = new te::da::DataSetType(outputDataSetName);

  std::vector<std::string> attrNamesAux;

  if (mainParams->getOutputDataSource()->getType() != "OGR")
  {
    te::dt::SimpleProperty* pkProperty = new te::dt::SimpleProperty(outputDataSetName + "_id", te::dt::INT32_TYPE);
    pkProperty->setAutoNumber(true);
    outputDataSetType->add(pkProperty);

    te::da::PrimaryKey* pk = new te::da::PrimaryKey(outputDataSetName + "_pk", outputDataSetType);
    pk->add(pkProperty);
    outputDataSetType->setPrimaryKey(pk);
  }

  for (std::size_t i = 0; i < firstSelectedProperties.size(); ++i)
  {
    te::dt::Property* clonedProperty = firstSelectedProperties[i]->clone();

    std::string pName = clonedProperty->getName();

    if (mainParams->getOutputDataSource()->getType() == "OGR")
      pName = GetDistinctName(pName, attrNamesAux, 10);
    else
      pName = GetDistinctName(pName, attrNamesAux);

    m_firstAttrNameMap[pName] = clonedProperty->getName();

    clonedProperty->setName(pName);

    outputDataSetType->add(clonedProperty);

    attrNamesAux.push_back(clonedProperty->getName());
  }

  for (std::size_t i = 0; i < secondSelectedProperties.size(); ++i)
  {
    te::dt::Property* clonedProperty = secondSelectedProperties[i]->clone();

    std::string pName = clonedProperty->getName();

    if (mainParams->getOutputDataSource()->getType() == "OGR")
      pName = GetDistinctName(pName, attrNamesAux, 10);
    else
      pName = GetDistinctName(pName, attrNamesAux);

    m_secondAttrNameMap[pName] = clonedProperty->getName();

    clonedProperty->setName(pName);

    outputDataSetType->add(clonedProperty);

    attrNamesAux.push_back(clonedProperty->getName());
  }

  te::gm::GeometryProperty* newGeometryProperty = new te::gm::GeometryProperty("geom");

  te::gm::GeomType type = getGeomResultType(firstGeometryProperty->getGeometryType(), secondGeometryProperty->getGeometryType(), isCollection);
  newGeometryProperty->setGeometryType(type);

  newGeometryProperty->setSRID(firstGeometryProperty->getSRID());

  outputDataSetType->add(newGeometryProperty);

  return outputDataSetType;
}

te::gm::GeomType te::vp::Intersection::getGeomResultType(const te::gm::GeomType& firstGeom, const te::gm::GeomType& secondGeom, const bool& isCollection)
{
  te::gm::GeomType geomType;

  if ((firstGeom == te::gm::PointType) ||
    (firstGeom == te::gm::PointZType) ||
    (firstGeom == te::gm::PointMType) ||
    (firstGeom == te::gm::PointZMType) ||
    (firstGeom == te::gm::PointKdType) ||

    (secondGeom == te::gm::PointType) ||
    (secondGeom == te::gm::PointZType) ||
    (secondGeom == te::gm::PointMType) ||
    (secondGeom == te::gm::PointZMType) ||
    (secondGeom == te::gm::PointKdType) ||

    (firstGeom == te::gm::MultiPointType) ||
    (firstGeom == te::gm::MultiPointZType) ||
    (firstGeom == te::gm::MultiPointMType) ||
    (firstGeom == te::gm::MultiPointZMType) ||

    (secondGeom == te::gm::MultiPointType) ||
    (secondGeom == te::gm::MultiPointZType) ||
    (secondGeom == te::gm::MultiPointMType) ||
    (secondGeom == te::gm::MultiPointZMType))
  {
    geomType = te::gm::MultiPointType;
  }
  else if ((firstGeom == te::gm::LineStringType) ||
    (firstGeom == te::gm::LineStringZType) ||
    (firstGeom == te::gm::LineStringMType) ||
    (firstGeom == te::gm::LineStringZMType) ||

    (secondGeom == te::gm::LineStringType) ||
    (secondGeom == te::gm::LineStringZType) ||
    (secondGeom == te::gm::LineStringMType) ||
    (secondGeom == te::gm::LineStringZMType) ||

    (firstGeom == te::gm::MultiLineStringType) ||
    (firstGeom == te::gm::MultiLineStringZType) ||
    (firstGeom == te::gm::MultiLineStringMType) ||
    (firstGeom == te::gm::MultiLineStringZMType) ||

    (secondGeom == te::gm::MultiLineStringType) ||
    (secondGeom == te::gm::MultiLineStringZType) ||
    (secondGeom == te::gm::MultiLineStringMType) ||
    (secondGeom == te::gm::MultiLineStringZMType))
  {
    geomType = te::gm::MultiLineStringType;
  }
  else
    geomType = te::gm::MultiPolygonType;

  if (isCollection)
  {
    if (te::vp::IsMultiType(geomType))
      return geomType;
    else
      return te::vp::GetMultiType(geomType);
  }
  else
  {
    if (te::vp::IsMultiType(geomType))
      return te::vp::GetSimpleType(geomType);
    else
      return geomType;
  }
}