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
#include "../dataaccess/query/ST_Dump.h"
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
#include "ComplexData.h"
#include "Difference.h"
#include "Utils.h"

// BOOST
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::vp::Difference::Difference()
{
}

bool te::vp::Difference::executeMemory(te::vp::AlgorithmParams* mainParams)
{
  te::vp::ValidateAlgorithmParams(mainParams, te::vp::MEMORY);

// Validating parameters
  std::vector<te::vp::InputParams> inputParams = mainParams->getInputParams();
  
  if (inputParams.size() < 2)
    throw te::common::Exception(TE_TR("It is necessary more than one item for performing the operation."));

  te::da::DataSetType* dsType_input = inputParams[0].m_inputDataSetType;

  te::gm::GeometryProperty* inputGeomProp = te::da::GetFirstGeomProperty(dsType_input);

  te::da::DataSetType* dsType_difference = inputParams[1].m_inputDataSetType;

  te::gm::GeometryProperty* differenceGeomProp = te::da::GetFirstGeomProperty(dsType_difference);
  
  te::da::DataSet* inputDataSet = inputParams[0].m_inputDataSet;
  
  te::da::DataSet* differenceDataSet = inputParams[1].m_inputDataSet;
  
  te::da::DataSourcePtr outputDataSource = mainParams->getOutputDataSource();

// Build output dataset type
  std::auto_ptr<te::da::DataSetType> outputDataSetType(getOutputDataSetType(mainParams));

// Get the first geometry property from output datasettype.
  te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(outputDataSetType.get());

// Insert difference dataset in rtree.
  DataSetRTree rtree(new te::sam::rtree::Index<size_t, 8>);

// Get specific params.
  const std::map<std::string, te::dt::AbstractData*>& specificParams = mainParams->getSpecificParams();

  bool isCollection = this->isCollection(specificParams);

// Geometry type for the output data.
  te::gm::GeomType outputGeomType = setGeomResultType(inputGeomProp->getGeometryType(), isCollection);

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


  std::vector<std::string> propNames = this->getPropNames(specificParams);

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
      std::vector<te::gm::Geometry*> geomVec;
      
      if (isCollection)
        geomVec.push_back(geom.release());
      else
        te::gm::Multi2Single(geom.release(), geomVec);

      for (std::size_t g = 0; g < geomVec.size(); ++g)
      {
        if (!te::vp::IsMultiType(geomVec[g]->getGeomTypeId()) && isCollection)
          geomVec[g] = setGeomAsMulti(geomVec[g]);

        std::auto_ptr<te::mem::DataSetItem> currentItem = item->clone();
        currentItem->setGeometry(geomProp->getName(), geomVec[g]);

// Insert this item into Output dataSet;
        outputDataSet->add(currentItem.release());
      }

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
      std::vector<te::gm::Geometry*> geomVec;

      if (isCollection)
        geomVec.push_back(geom.release());
      else
        te::gm::Multi2Single(geom.release(), geomVec);

      for (std::size_t g = 0; g < geomVec.size(); ++g)
      {
        if (!te::vp::IsMultiType(geomVec[g]->getGeomTypeId()) && isCollection)
          geomVec[g] = setGeomAsMulti(geomVec[g]);

        std::auto_ptr<te::mem::DataSetItem> currentItem = item->clone();
        currentItem->setGeometry(geomProp->getName(), geomVec[g]);

// Insert this item into Output dataSet;
        outputDataSet->add(currentItem.release());
      }
    }
  }

  std::auto_ptr<te::da::DataSet> dataSetPrepared = PrepareAdd(outputDataSet.release(), outputDataSetType.get());

  if (!dataSetPrepared.get())
    throw te::common::Exception(TE_TR("Output DataSet was not prepared to save."));
  
  if (dataSetPrepared->size() == 0)
    throw te::common::Exception("The resultant layer is empty!");

  Save(outputDataSource.get(), dataSetPrepared.get(), outputDataSetType.get());

  return true;
}

bool te::vp::Difference::executeQuery(te::vp::AlgorithmParams* mainParams)
{
  te::vp::ValidateAlgorithmParams(mainParams, te::vp::QUERY);

// Validating parameters
  std::vector<te::vp::InputParams> inputParams = mainParams->getInputParams();

  if (inputParams.size() < 2)
    throw te::common::Exception(TE_TR("It is necessary more than one item for performing the operation."));

  te::da::DataSetType* dsType_input = inputParams[0].m_inputDataSetType;

  te::gm::GeometryProperty* geom_input = te::da::GetFirstGeomProperty(dsType_input);

  std::string aliasInput = dsType_input->getName();

  te::da::DataSetType* dsType_difference = inputParams[1].m_inputDataSetType;

  te::gm::GeometryProperty* geom_difference = te::da::GetFirstGeomProperty(dsType_difference);
  
  std::string aliasDifference = dsType_difference->getName();

  te::da::Select* selectInput = inputParams[0].m_inputQuery;
  te::da::SubSelect* subSelectInput = new te::da::SubSelect(selectInput, "inputLayer");
  aliasInput = subSelectInput->getAlias();

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
  const std::map<std::string, te::dt::AbstractData*> specificParams = mainParams->getSpecificParams();
  std::vector<std::string> propNames = this->getPropNames(specificParams);

  for (std::size_t i = 0; i < propNames.size(); ++i)
  {
    te::da::Field* f_att = new te::da::Field(aliasInput + "." + propNames[i]);
    fieldsExpressions->push_back(f_att);
  }

// Expression to set geometries as multigeometries.
  te::da::Expression* e_coalesce = new te::da::Coalesce(e_difference,
    new te::da::PropertyName(aliasInput + "." + geom_input->getName()));

  bool isCollection = this->isCollection(specificParams);

  if (isCollection)
  {
    te::da::Expression* e_multi = new te::da::ST_Multi(*e_coalesce);

    te::da::Field* f_multi = new te::da::Field(*e_multi, "geom");
    fieldsExpressions->push_back(f_multi);
  }
  else
  {
    te::da::Expression* e_dump = new te::da::ST_Dump(*e_coalesce);

    te::da::Field* f_dump = new te::da::Field(*e_dump, "geom");
    fieldsExpressions->push_back(f_dump);
  }

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

    for (std::size_t p = 0; p < propNames.size(); ++p)
    {
      te::da::Field* f_att = new te::da::Field(propNames[p]);
      insertFields->push_back(f_att);
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
      throw te::common::Exception("The resultant layer is empty!");

    std::string outputDsName = mainParams->getOutputDataSetName();

    dsQuery->moveBeforeFirst();

    if (outputDataSource->getType() == "OGR")
    {
      outputDataSource->add(outputDsName, dsQuery.get(), options);
    }
    else
    {
      t->add(outputDsName, dsQuery.get(), options);
      t->commit();
    }
  }

  return true;
}

std::vector<std::string> te::vp::Difference::getPropNames(const std::map<std::string, te::dt::AbstractData*>& specificParams)
{
  std::vector<std::string> propNames;
  std::map<std::string, te::dt::AbstractData*>::const_iterator it = specificParams.begin();

  while (it != specificParams.end())
  {
    if (it->first != "ATTRIBUTES")
    {
      ++it;
      continue;
    }

    te::vp::ComplexData<std::vector<std::string> >* cd =
      dynamic_cast<te::vp::ComplexData<std::vector<std::string> >* >(it->second);

    if (cd)
      propNames = cd->getValue();

    ++it;
  }

  return propNames;
}

bool te::vp::Difference::isCollection(const std::map<std::string, te::dt::AbstractData*>& specificParams)
{
  bool isCollection = false;

  std::map<std::string, te::dt::AbstractData*>::const_iterator it = specificParams.begin();
  while (it != specificParams.end())
  {
    if (it->first != "IS_COLLECTION")
    {
      ++it;
      continue;
    }

    te::dt::SimpleData<bool, te::dt::BOOLEAN_TYPE>* sd =
      dynamic_cast<te::dt::SimpleData<bool, te::dt::BOOLEAN_TYPE >* >(it->second);

    if (sd)
      isCollection = sd->getValue();

    ++it;
  }

  return isCollection;
}

te::da::DataSetType* te::vp::Difference::getOutputDataSetType(te::vp::AlgorithmParams* mainParams)
{
// Get the input parameters
  std::vector<te::vp::InputParams> inputParams = mainParams->getInputParams();


// Get the output dataset name.
  std::string outputDataSetName = mainParams->getOutputDataSetName();
  te::da::DataSetType* outputDataSetType = new te::da::DataSetType(outputDataSetName);


// Set to output datasettype the primary key property.
  if (mainParams->getOutputDataSource()->getType() != "OGR")
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
  std::vector<std::string> propNames = getPropNames(specificParams);
  
  for (std::size_t i = 0; i < propNames.size(); ++i)
  {
    te::dt::Property* prop = dsType->getProperty(propNames[i]);

    if (!prop)
      continue;

    te::dt::SimpleProperty* currentProperty = new te::dt::SimpleProperty(propNames[i], prop->getType());
    outputDataSetType->add(currentProperty);
  }


// Creating the geometry property
  te::gm::GeometryProperty* newGeomProp = new te::gm::GeometryProperty("geom");

  te::gm::GeometryProperty* intputGeomProp = te::da::GetFirstGeomProperty(dsType);

  bool isCollection = this->isCollection(specificParams);

  te::gm::GeomType type = setGeomResultType(te::da::GetFirstGeomProperty(dsType)->getGeometryType(), isCollection);
  newGeomProp->setGeometryType(type);

  newGeomProp->setSRID(intputGeomProp->getSRID());

  outputDataSetType->add(newGeomProp);

  return outputDataSetType;
}

te::gm::GeomType te::vp::Difference::setGeomResultType(const te::gm::GeomType& geomType, const bool& isCollection)
{
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

te::gm::Geometry* te::vp::Difference::setGeomAsMulti(te::gm::Geometry* geom)
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

