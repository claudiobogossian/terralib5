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
 \file Difference.cpp
 */

#include "../common/Translator.h"

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

#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"

#include "../dataaccess/query/Coalesce.h"
#include "../dataaccess/query/DataSetName.h"
#include "../dataaccess/query/Expression.h"
#include "../dataaccess/query/Field.h"
#include "../dataaccess/query/Fields.h"
#include "../dataaccess/query/From.h"
#include "../dataaccess/query/Insert.h"
#include "../dataaccess/query/PropertyName.h"
#include "../dataaccess/query/Not.h"
#include "../dataaccess/query/Select.h"
#include "../dataaccess/query/SelectExpression.h"
#include "../dataaccess/query/SubSelect.h"
#include "../dataaccess/query/ST_Difference.h"
#include "../dataaccess/query/ST_Intersects.h"
#include "../dataaccess/query/ST_IsEmpty.h"
#include "../dataaccess/query/ST_Multi.h"
#include "../dataaccess/query/ST_Union.h"
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

#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"

#include "AlgorithmParams.h"
#include "Difference.h"
#include "Utils.h"

// BOOST
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::vp::Difference::Difference()
{
}

bool te::vp::Difference::getSpecificParams()
{
  return m_copyInputColumns;
}

void te::vp::Difference::setSpecificParams(bool copyInputColumns)
{
  m_copyInputColumns = copyInputColumns;
}

bool te::vp::Difference::executeMemory(te::vp::AlgorithmParams* mainParams)
{
// Validating parameters
  std::vector<te::vp::InputParams> inputParams = mainParams->getInputParams();
  
  if (inputParams.size() < 2)
    throw te::common::Exception(TE_TR("It is necessary more than one item for performing the operation."));

// Get DataSetType and Geometry Property of InputLayer Layer.
  if (!inputParams[0].m_inputDataSetType)
    throw te::common::Exception(TE_TR("It is necessary to set the DataSetType from Input Layer."));

  std::auto_ptr<te::da::DataSetType> dsType_input(inputParams[0].m_inputDataSetType);

  te::gm::GeometryProperty* inputGeomProp = te::da::GetFirstGeomProperty(dsType_input.get());


// Get DataSetType and Geometry Property of Difference Layer.
  if (!inputParams[1].m_inputDataSetType)
    throw te::common::Exception(TE_TR("It is necessary to set the DataSetType or DataSetName from Difference Layer."));

  std::auto_ptr<te::da::DataSetType> dsType_difference(inputParams[1].m_inputDataSetType);

  te::gm::GeometryProperty* differenceGeomProp = te::da::GetFirstGeomProperty(dsType_difference.get());

// Verify if the operation has DataSet.
  if (!inputParams[0].m_inputDataSet)
    throw te::common::Exception(TE_TR("It is necessary to set the Input DataSet."));

  std::auto_ptr<te::da::DataSet>inputDataSet(inputParams[0].m_inputDataSet);

  if (!inputParams[1].m_inputDataSet)
    throw te::common::Exception(TE_TR("It is necessary to set the Difference DataSet."));

  std::auto_ptr<te::da::DataSet> differenceDataSet(inputParams[1].m_inputDataSet);

// Get Output DataSource.
  if (!mainParams->getOutputDataSource())
    throw te::common::Exception(TE_TR("It is necessary to set the Output DataSource."));

  te::da::DataSourcePtr outputDataSource = mainParams->getOutputDataSource();

// Build output dataset type
  std::auto_ptr<te::da::DataSetType> outputDataSetType(getOutputDataSetType(mainParams));

// Get the first geometry property from output datasettype.
  te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(outputDataSetType.get());


// Insert difference dataset in rtree.
  DataSetRTree rtree(new te::sam::rtree::Index<size_t, 8>);

// Geometry type for the output data.
  te::gm::GeomType outputGeomType = setGeomResultType(inputGeomProp->getGeometryType());

// Create output dataset in memory.
  std::auto_ptr<te::mem::DataSet> outputDataSet(new te::mem::DataSet(outputDataSetType.get()));
  
  size_t p = 0;
  int inputSRID = inputGeomProp->getSRID();
  int differenceSRID = differenceGeomProp->getSRID();

  std::string intupGeomName = inputGeomProp->getName();
  std::string differenceGeomName = differenceGeomProp->getName();

  differenceDataSet->moveBeforeFirst();
  while (differenceDataSet->moveNext())
  {
    std::auto_ptr<te::gm::Geometry> g = differenceDataSet->getGeometry(differenceGeomName);

    rtree->insert(*g->getMBR(), p);

    ++p;
  }


// Get specific params.
  const std::map<std::string, te::dt::AbstractData*>& specificParams = mainParams->getSpecificParams();
  std::vector<std::string> propNames = getPropNames(specificParams);

  inputDataSet->moveBeforeFirst();
  while (inputDataSet->moveNext())
  {
// Create DataSetItem based on output DataSet;
    std::auto_ptr<te::mem::DataSetItem> item(new te::mem::DataSetItem(outputDataSet.get()));

// Populate item with tabular data.
    for (std::size_t p = 0; p < propNames.size(); ++p)
      item->setValue(propNames[p], inputDataSet->getValue(propNames[p]).release());

// Current geometry from Input Layer.
    std::auto_ptr<te::gm::Geometry> geom = inputDataSet->getGeometry(intupGeomName);

// If is different srid, transform current geom to the "difference" projection, to search in rtree the candidates.
    if (inputSRID != differenceSRID)
      geom->transform(differenceSRID);

    std::vector<size_t> report;
    rtree->search(*geom->getMBR(), report);

// If the geom SRID was changed, return to original SRID.
    if (geom->getSRID() != inputSRID)
      geom->transform(inputSRID);

// If the search is empty, insert this item into Output dataSet;
    if (report.empty())
    {
// Insert the current geometry into item;
      if (geom->getGeomTypeId() != outputGeomType)
        geom.reset(setGeometryType(geom.release()));

      item->setGeometry(geomProp->getName(), geom.release()); 

// Insert this item into Output dataSet;
      outputDataSet->add(item.release());

      continue;
    }

// Difference Operation.
    differenceDataSet->move(report[0]);
    std::auto_ptr<te::gm::Geometry> unionGeom = differenceDataSet->getGeometry(differenceGeomName);

    for (size_t i = 1; i < report.size(); ++i)
    {
      differenceDataSet->move(report[i]);
      std::auto_ptr<te::gm::Geometry> diffGeom = differenceDataSet->getGeometry(differenceGeomName);
      unionGeom.reset(unionGeom->Union(diffGeom.get()));
    }

    if (inputSRID != differenceSRID)
      unionGeom->transform(inputSRID);

    geom.reset(geom->difference(unionGeom.get()));

    if (!geom->isEmpty())
    {
// Insert the current geometry into item;
      if (geom->getGeomTypeId() != outputGeomType)
        geom.reset(setGeometryType(geom.release()));
      
      item->setGeometry(geomProp->getName(), geom.release());

// Insert this item into Output dataSet;
      outputDataSet->add(item.release());
    }
  }


  std::vector<std::string> pkPropNames = getPKPropNames(outputDataSetType.get());

  if (!pkPropNames.empty())
  {
    std::auto_ptr<te::da::DataSet> dataSet = te::da::HideColumns(outputDataSet.get(), outputDataSetType.get(), pkPropNames);
    Save(outputDataSource.get(), dataSet.get(), outputDataSetType.get());
  }
  else
  {
    Save(outputDataSource.get(), outputDataSet.get(), outputDataSetType.get());
  }

  return true;
}

bool te::vp::Difference::executeQuery(te::vp::AlgorithmParams* mainParams)
{
// Validating parameters
  std::vector<te::vp::InputParams> inputParams = mainParams->getInputParams();

  if (inputParams.size() < 2)
    throw te::common::Exception(TE_TR("It is necessary more than one item for performing the operation."));


// Get DataSetType and Geometry Property of InputLayer Layer.
  if (!inputParams[0].m_inputDataSetType)
    throw te::common::Exception(TE_TR("It is necessary to set the DataSetType from Input Layer."));
    
  std::auto_ptr<te::da::DataSetType> dsType_input(inputParams[0].m_inputDataSetType);

  te::gm::GeometryProperty* geom_input = te::da::GetFirstGeomProperty(dsType_input.get());
  
  std::string aliasInput = dsType_input->getName();

// Get DataSetType and Geometry Property of Difference Layer.
  if (!inputParams[1].m_inputDataSetType)
    throw te::common::Exception(TE_TR("It is necessary to set the DataSetType or DataSetName from Difference Layer."));
    
  std::auto_ptr<te::da::DataSetType> dsType_difference(inputParams[1].m_inputDataSetType);
  
  te::gm::GeometryProperty* geom_difference = te::da::GetFirstGeomProperty(dsType_difference.get());
  
  std::string aliasDifference = dsType_difference->getName();


// Verify if the operation has Query.
  if (!inputParams[0].m_inputQuery)
    throw te::common::Exception(TE_TR("It is necessary to set the Input Query."));

  te::da::Select* selectInput = inputParams[0].m_inputQuery;
  te::da::SubSelect* subSelectInput = new te::da::SubSelect(selectInput, "inputLayer");
  aliasInput = subSelectInput->getAlias();

  if (!inputParams[1].m_inputQuery)
    throw te::common::Exception(TE_TR("It is necessary to set the Difference Query."));

  te::da::Select* selectDifference = inputParams[1].m_inputQuery;
  te::da::SubSelect* subSelectDifference = new te::da::SubSelect(selectDifference, "differenceLayer");
  aliasDifference = subSelectDifference->getAlias();



// Building Difference Query.

// Union Expression to set into Difference expression.
  te::da::Fields* fieldsUnion = new te::da::Fields;

  te::da::Expression* e_union = new te::da::ST_Union(
    new te::da::PropertyName(aliasDifference + "." + geom_difference->getName()));
  te::da::Field* f_union = new te::da::Field(*e_union, "c_union");
  fieldsUnion->push_back(f_union);


// FROM clause - This from clause is for difference layer.
  if (!subSelectDifference)
    throw te::common::Exception(TE_TR("A problem was found. SubSelect Difference with problem."));

  te::da::From* fromUnion = new te::da::From;
  fromUnion->push_back(subSelectDifference);

  te::da::Expression* e_intersects = new te::da::ST_Intersects(
    new te::da::PropertyName(aliasInput + "." + geom_input->getName()),
    new te::da::PropertyName(aliasDifference + "." + geom_difference->getName()));

  te::da::Where* whereUnion = new te::da::Where(e_intersects);

  te::da::Select* selectUnion = new te::da::Select(fieldsUnion, fromUnion, whereUnion);
  te::da::SelectExpression* subSelectExpression = new te::da::SelectExpression(selectUnion);


// Expressions to execute the Difference.
  te::da::Expression* e_difference = new te::da::ST_Difference(
    new te::da::PropertyName(aliasInput + "." + geom_input->getName()), subSelectExpression);


  te::da::Fields* fieldsExpressions = new te::da::Fields;

// Tabular attributes.
  const std::map<std::string, te::dt::AbstractData*> attributesMap = mainParams->getSpecificParams();
  std::map<std::string, te::dt::AbstractData*>::const_iterator it = attributesMap.begin();

  while (it != attributesMap.end())
  {
    te::dt::SimpleData<std::string, te::dt::STRING_TYPE>* sd =
      dynamic_cast<te::dt::SimpleData<std::string, te::dt::STRING_TYPE>*>(it->second);

    std::string columnName = sd->getValue();

    te::da::Field* f_att = new te::da::Field(aliasInput + "." + columnName);
    fieldsExpressions->push_back(f_att);

    ++it;
  }

// Expression to set geometries as multigeometries.
  te::da::Expression* e_coalesce = new te::da::Coalesce(e_difference,
    new te::da::PropertyName(aliasInput + "." + geom_input->getName()));

  te::da::Expression* e_multi = new te::da::ST_Multi(*e_coalesce);

  te::da::Field* f_multi = new te::da::Field(*e_multi, "geom");
  fieldsExpressions->push_back(f_multi);

// FROM clause - This from clause is for input layer.
  if (!subSelectInput)
    throw te::common::Exception(TE_TR("A problem was found. SubSelect Input with problem."));

  te::da::From* fromDifference = new te::da::From;
  fromDifference->push_back(subSelectInput);

  te::da::Select* selectIn = new te::da::Select(fieldsExpressions, fromDifference);
  te::da::SubSelect* subSelectIn = new te::da::SubSelect(selectIn, "result");


// Outer Select 
  te::da::Fields* outerFields = new te::da::Fields;
  te::da::Field* f_outer = new te::da::Field("*");
  outerFields->push_back(f_outer);

  te::da::From* outerFrom = new te::da::From;
  outerFrom->push_back(subSelectIn);

  te::da::Expression* e_isempty = new te::da::ST_IsEmpty(subSelectIn->getAlias() + ".geom");
  te::da::Expression* e_not = new te::da::Not(e_isempty);
  te::da::Where* outerWhere = new te::da::Where(e_not);

  te::da::Select* outerSelect = new te::da::Select(outerFields, outerFrom, outerWhere);


/*Check if the input and output dataSource are the same, if so, 
  persists the result of select query into database with insert command.*/
  te::da::DataSourcePtr outputDataSource = mainParams->getOutputDataSource();

  te::da::DataSourceInfoPtr inDataSourceInfoPtr = te::da::DataSourceInfoManager::getInstance().get(inputParams[0].m_inputDataSource->getId());
  te::da::DataSourceInfoPtr outDataSourceInfoPtr = te::da::DataSourceInfoManager::getInstance().get(outputDataSource->getId());

// Create output dataset
  std::auto_ptr<te::da::DataSourceTransactor> t = outputDataSource->getTransactor();
  std::map<std::string, std::string> options;

  std::auto_ptr<te::da::DataSetType> outDataSetType(this->getOutputDataSetType(mainParams));

  if (outputDataSource->getType() == "OGR")
  {
    outputDataSource->createDataSet(outDataSetType.get(), options);
  }
  else
  {
    t->begin();
    t->createDataSet(outDataSetType.get(), options);
    t->commit();

    if (!inDataSourceInfoPtr)
    {
      t->rollBack();
      throw te::common::Exception(TE_TR("Input DataSource ID not found."));
    }

    if (!outDataSourceInfoPtr)
    {
      t->rollBack();
      throw te::common::Exception(TE_TR("Output DataSource ID not found."));
    }
  }

  std::string inputConnection = inDataSourceInfoPtr->getConnInfoAsString();
  std::string outputConnection = outDataSourceInfoPtr->getConnInfoAsString();

// Execute Query
  if (inputConnection == outputConnection)
  {
    te::da::Fields* insertFields = new te::da::Fields;

    const std::map<std::string, te::dt::AbstractData*> attributesMap = mainParams->getSpecificParams();
    std::map<std::string, te::dt::AbstractData*>::const_iterator it = attributesMap.begin();

    while (it != attributesMap.end())
    {
      te::dt::SimpleData<std::string, te::dt::STRING_TYPE>* sd =
        dynamic_cast<te::dt::SimpleData<std::string, te::dt::STRING_TYPE>*>(it->second);

      std::string columnName = sd->getValue();

      te::da::Field* f_att = new te::da::Field(columnName);
      insertFields->push_back(f_att);

      ++it;
    }

    te::da::Field* f_insert = new te::da::Field("geom");
    insertFields->push_back(f_insert);

    te::da::Insert* insert = new te::da::Insert(new te::da::DataSetName(mainParams->getOutputDataSetName()), insertFields, outerSelect);
    inputParams[0].m_inputDataSource->execute(*insert);
  }
  else
  {
    std::auto_ptr<te::da::DataSet> dsQuery = inputParams[0].m_inputDataSource->query(outerSelect);
    dsQuery->moveBeforeFirst();

    if (dsQuery->size() == 0)
      throw te::common::Exception("The Layers do not intersect to set difference!");

    std::string outputDsName = mainParams->getOutputDataSetName();

    std::auto_ptr<te::da::DataSet> outDset(updateGeomType(outDataSetType.get(), dsQuery.release()));

    outDset->moveBeforeFirst();

    if (outputDataSource->getType() == "OGR")
    {
      outputDataSource->add(outputDsName, outDset.get(), options);
    }
    else
    {
      t->add(outputDsName, outDset.get(), options);
      t->commit();
    }
  }

  return true;
}

bool te::vp::Difference::persistsDataSet()
{
  return true;
}

bool te::vp::Difference::validMemoryParams(te::vp::AlgorithmParams* mainParams)
{
  //TODO - VALID ALL MEMORY PARAMETERS.
  return true;
}

bool te::vp::Difference::validQueryParams(te::vp::AlgorithmParams* mainParams)
{
  //TODO - VALID ALL QUERY PARAMETERS.
  return true;
}

std::vector<std::string> te::vp::Difference::getPropNames(const std::map<std::string, te::dt::AbstractData*>& specificParams)
{
  std::vector<std::string> propNames;
  std::map<std::string, te::dt::AbstractData*>::const_iterator it = specificParams.begin();

  while (it != specificParams.end())
  {
    te::dt::SimpleData<std::string, te::dt::STRING_TYPE>* sd =
      dynamic_cast<te::dt::SimpleData<std::string, te::dt::STRING_TYPE>*>(it->second);

    std::string columnName = sd->getValue();
    
    if (!columnName.empty())
      propNames.push_back(columnName);

    ++it;
  }

  return propNames;
}

std::vector<std::string> te::vp::Difference::getPKPropNames(const te::da::DataSetType* dataSetType)
{
  std::vector<std::string> pkPropNames;

  te::da::PrimaryKey* pk = dataSetType->getPrimaryKey();

  std::vector<te::dt::Property*> props;

  if (pk)
    props = pk->getProperties();

  for (std::size_t i = 0; i < props.size(); ++i)
  {
    te::dt::SimpleProperty* sp = dynamic_cast<te::dt::SimpleProperty*>(props[i]);

    if (sp && sp->isAutoNumber())
      pkPropNames.push_back(sp->getName());
  }

  return pkPropNames;
}

te::da::DataSet* te::vp::Difference::updateGeomType(te::da::DataSetType* dsType, te::da::DataSet* ds)
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

te::da::DataSetType* te::vp::Difference::getOutputDataSetType(te::vp::AlgorithmParams* mainParams)
{
// Get the input parameters
  std::vector<te::vp::InputParams> inputParams = mainParams->getInputParams();


// Get the output dataset name.
  std::string outputDataSetName = mainParams->getOutputDataSetName();
  te::da::DataSetType* outputDataSetType = new te::da::DataSetType(outputDataSetName);


// Set to output datasettype the primary key property.
  if (mainParams->getOutputDataSource()->getType() == "OGR")
  {
    te::dt::SimpleProperty* fidProperty = new te::dt::SimpleProperty("FID", te::dt::INT32_TYPE);
    outputDataSetType->add(fidProperty);
  }
  else
  {
    te::dt::SimpleProperty* pkProperty = new te::dt::SimpleProperty(outputDataSetName + "_id", te::dt::INT32_TYPE);
    pkProperty->setAutoNumber(true);
    outputDataSetType->add(pkProperty);

    te::da::PrimaryKey* pk = new te::da::PrimaryKey(outputDataSetName + "_pk", outputDataSetType);
    pk->add(pkProperty);
    outputDataSetType->setPrimaryKey(pk);
  }


// Get tabular attributes.
  te::da::DataSetType* dsType = 0;
  if (inputParams[0].m_inputDataSetType)
  {
    dsType = inputParams[0].m_inputDataSetType;
  }
  else
  {
    dsType = te::da::GetDataSetType(inputParams[0].m_inputDataSetName, inputParams[0].m_inputDataSource->getId());
  }

  const std::map<std::string, te::dt::AbstractData*> specificParams = mainParams->getSpecificParams();
  std::map<std::string, te::dt::AbstractData*>::const_iterator it = specificParams.begin();

  while (it != specificParams.end())
  {
    te::dt::SimpleData<std::string, te::dt::STRING_TYPE>* sd =
      dynamic_cast<te::dt::SimpleData<std::string, te::dt::STRING_TYPE>*>(it->second);

    std::string columnName = sd->getValue();

    te::dt::Property* prop = dsType->getProperty(columnName);

    if (!prop)
      continue;

    te::dt::SimpleProperty* currentProperty = new te::dt::SimpleProperty(columnName, prop->getType());
    outputDataSetType->add(currentProperty);

    ++it;
  }


// Creating the geometry property
  te::gm::GeometryProperty* newGeomProp = new te::gm::GeometryProperty("geom");

  te::gm::GeometryProperty* intputGeomProp = te::da::GetFirstGeomProperty(dsType);
  te::gm::GeomType newType = setGeomResultType(te::da::GetFirstGeomProperty(dsType)->getGeometryType());
  newGeomProp->setGeometryType(newType);
  newGeomProp->setSRID(intputGeomProp->getSRID());
  outputDataSetType->add(newGeomProp);

  return outputDataSetType;
}

te::gm::GeomType te::vp::Difference::setGeomResultType(te::gm::GeomType firstGeom)
{
  if ((firstGeom == te::gm::PointType) ||
    (firstGeom == te::gm::PointZType) ||
    (firstGeom == te::gm::PointMType) ||
    (firstGeom == te::gm::PointZMType) ||
    (firstGeom == te::gm::PointKdType) ||

    (firstGeom == te::gm::MultiPointType) ||
    (firstGeom == te::gm::MultiPointZType) ||
    (firstGeom == te::gm::MultiPointMType) ||
    (firstGeom == te::gm::MultiPointZMType))

    return te::gm::MultiPointType;

  else if ((firstGeom == te::gm::LineStringType) ||
    (firstGeom == te::gm::LineStringZType) ||
    (firstGeom == te::gm::LineStringMType) ||
    (firstGeom == te::gm::LineStringZMType) ||

    (firstGeom == te::gm::MultiLineStringType) ||
    (firstGeom == te::gm::MultiLineStringZType) ||
    (firstGeom == te::gm::MultiLineStringMType) ||
    (firstGeom == te::gm::MultiLineStringZMType))

    return te::gm::MultiLineStringType;

  else
    return te::gm::MultiPolygonType;
}

te::gm::Geometry* te::vp::Difference::setGeometryType(te::gm::Geometry* geom)
{

  switch (geom->getGeomTypeId())
  {
    case te::gm::PointType:
    {
                            te::gm::MultiPoint* geomColl = new te::gm::MultiPoint(0, te::gm::MultiPointType, geom->getSRID());
                            geomColl->add(geom);

                            return geomColl;
    }
    case te::gm::LineStringType:
    {
                                 te::gm::MultiLineString* geomColl = new te::gm::MultiLineString(0, te::gm::MultiLineStringType, geom->getSRID());
                                 geomColl->add(geom);

                                 return geomColl;
    }
    case te::gm::PolygonType:
    {
                              te::gm::MultiPolygon* geomColl = new te::gm::MultiPolygon(0, te::gm::MultiPolygonType, geom->getSRID());
                              geomColl->add(geom);

                              return geomColl;
    }
  }

  return geom;
}

