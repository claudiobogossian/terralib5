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
#include "../dataaccess/query/Coalesce.h"
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
#include "../dataaccess/query/Not.h"
#include "../dataaccess/query/Select.h"
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

#include "../sam.h"

#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"

#include "../statistics/core/Utils.h"

#include "AlgorithmParams.h"
#include "Difference.h"
#include "Utils.h"

// BOOST
#include <boost/lexical_cast.hpp>

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

bool te::vp::Difference::executeMemory(te::vp::AlgorithmParams* mainParams, te::da::DataSet* teste)
{
  return false;
}

bool te::vp::Difference::executeQuery(te::vp::AlgorithmParams* mainParams)
{
  std::vector<te::vp::InputParams> inputParams = mainParams->getInputParams();

  if (inputParams.size() < 2)
  {
    //Return a message: It is necessary more than one item for performing the operation
    return false;
  }

// Build Difference Query.

  std::auto_ptr<te::da::DataSetType> dsType_first;
  std::auto_ptr<te::da::DataSetType> dsType_second;

  te::gm::GeometryProperty* geom_first;
  te::gm::GeometryProperty* geom_second;

  dsType_first = inputParams[0].m_inputDataSource->getDataSetType(inputParams[0].m_inputDataSetName);
  geom_first = te::da::GetFirstGeomProperty(dsType_first.get());

  dsType_second = inputParams[1].m_inputDataSource->getDataSetType(inputParams[1].m_inputDataSetName);
  geom_second = te::da::GetFirstGeomProperty(dsType_second.get());


// Subselect to join the layer that will be used to diff.
  te::da::Fields* fields = new te::da::Fields;
  te::da::Expression* e_union = new te::da::ST_Union(
    new te::da::PropertyName(dsType_second->getName() + "." + geom_second->getName()));
  te::da::Field* f_union = new te::da::Field(*e_union, "c_union");
  fields->push_back(f_union);

  te::da::FromItem * internFromItem = new te::da::DataSetName(dsType_second->getName());
  te::da::From* internFrom = new te::da::From;
  internFrom->push_back(internFromItem);

  te::da::Select* internSelect = new te::da::Select(fields, internFrom);
  te::da::SubSelect* subInternSelect = new te::da::SubSelect(internSelect, "internSelect");

// Expressions to execute the Difference.
  te::da::Expression* e_difference = new te::da::ST_Difference(
    new te::da::PropertyName(dsType_first->getName() + "." + geom_first->getName()),
    new te::da::PropertyName(subInternSelect->getAlias() + "." + "c_union"));

  te::da::Expression* e_coalesce = new te::da::Coalesce(e_difference,
    new te::da::PropertyName(dsType_first->getName() + "." + geom_first->getName()));

  te::da::Expression* e_multi = new te::da::ST_Multi(*e_coalesce);
  
  te::da::Fields* fieldsExpressions = new te::da::Fields;
  te::da::Field* f_multi = new te::da::Field(*e_multi, "geom");
  fieldsExpressions->push_back(f_multi);

// FROM clause with LEFT JOIN.
  te::da::FromItem* firstFromItem = new te::da::DataSetName(dsType_first->getName());
  
  te::da::Expression* e_intersects = new te::da::ST_Intersects(
    new te::da::PropertyName(dsType_first->getName() + "." + geom_first->getName()),
    new te::da::PropertyName(subInternSelect->getAlias() + ".c_union"));
  
  te::da::JoinConditionOn* on = new te::da::JoinConditionOn(e_intersects);
  te::da::Join* join = new te::da::Join(*firstFromItem, *subInternSelect, te::da::LEFT_JOIN, *on);
  te::da::From* from = new te::da::From;
  from->push_back(join);

  te::da::Select* mainSelect = new te::da::Select(fieldsExpressions, from);
  te::da::SubSelect* subMainSelect = new te::da::SubSelect(mainSelect, "result");

// Outer Select 
  te::da::Fields* outerFields = new te::da::Fields;
  te::da::Field* f_outer = new te::da::Field("*");
  outerFields->push_back(f_outer);

  te::da::From* outerFrom = new te::da::From;
  outerFrom->push_back(subMainSelect);

  te::da::Expression* e_isempty = new te::da::ST_IsEmpty(subMainSelect->getAlias() + ".geom");
  te::da::Expression* e_not = new te::da::Not(e_isempty);
  te::da::Where* outerWhere = new te::da::Where(e_not);

  te::da::Select* outerSelect = new te::da::Select(outerFields, outerFrom, outerWhere);

/*Check if the input and output dataSource are the same, if so, 
  persists the result of select query into database with insert command.*/
  te::da::DataSource* outputDataSource = mainParams->getOutputDataSource();

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

    te::da::Insert* insert = new te::da::Insert(new te::da::DataSetName(mainParams->getOutputDataSetName()), insertFields, outerSelect);
    inputParams[0].m_inputDataSource->execute(*insert);
  }
  else
  {
    std::auto_ptr<te::da::DataSet> dsQuery = inputParams[0].m_inputDataSource->query(mainSelect);
    dsQuery->moveBeforeFirst();

    if (dsQuery->size() == 0)
      throw te::common::Exception("The Layers do not intersect to set difference!");

    std::string outputDsName = mainParams->getOutputDataSetName();

    std::auto_ptr<te::da::DataSetType> outDataSetType(new te::da::DataSetType(outputDsName));

    te::dt::SimpleProperty* pkProperty = new te::dt::SimpleProperty(outputDsName + "_id", te::dt::INT32_TYPE);
    pkProperty->setAutoNumber(true);
    outDataSetType->add(pkProperty);

    te::da::PrimaryKey* pk = new te::da::PrimaryKey(outputDsName + "_pk", outDataSetType.get());
    pk->add(pkProperty);
    outDataSetType->setPrimaryKey(pk);

    te::gm::GeomType newType = setGeomResultType(te::da::GetFirstGeomProperty(dsType_first.get())->getGeometryType(),
                                                te::da::GetFirstGeomProperty(dsType_second.get())->getGeometryType());

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


bool te::vp::Difference::isSupportQuery(te::vp::AlgorithmParams* mainParams)
{
  return true;
}

te::da::DataSetType* te::vp::Difference::getOutputDataSetType(te::vp::AlgorithmParams* mainParams)
{
// Get the output dataset name.
  std::string outputDataSetName = mainParams->getOutputDataSetName();
  te::da::DataSetType* outputDataSetType = new te::da::DataSetType(outputDataSetName);

// Set to output datasettype the primary key property.
  te::dt::SimpleProperty* pkProperty = new te::dt::SimpleProperty(outputDataSetName + "_id", te::dt::INT32_TYPE);
  pkProperty->setAutoNumber(true);
  outputDataSetType->add(pkProperty);

  te::da::PrimaryKey* pk = new te::da::PrimaryKey(outputDataSetName + "_pk", outputDataSetType);
  pk->add(pkProperty);
  outputDataSetType->setPrimaryKey(pk);

// Get the first layer SRID to set in output dataSetType.
  std::vector<te::vp::InputParams> inputParams = mainParams->getInputParams();

  te::da::DataSourcePtr dataSource = inputParams[0].m_inputDataSource;
  std::string dataSetName = inputParams[0].m_inputDataSetName;

  std::auto_ptr<te::da::DataSetType> dataSetType;
  if (!dataSetName.empty())
    dataSetType = dataSource->getDataSetType(dataSetName);

// Creating the geometry property
  te::gm::GeometryProperty* newGeomProp = new te::gm::GeometryProperty("geom");
  te::gm::GeometryProperty* intputGeomProp = te::da::GetFirstGeomProperty(dataSetType.get());
  newGeomProp->setGeometryType(intputGeomProp->getGeometryType());
  newGeomProp->setSRID(intputGeomProp->getSRID());
  outputDataSetType->add(newGeomProp);

  return outputDataSetType;
}

te::gm::GeomType te::vp::Difference::setGeomResultType(te::gm::GeomType firstGeom, te::gm::GeomType secondGeom)
{
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

    return te::gm::MultiPointType;

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

    return te::gm::MultiLineStringType;

  else
    return te::gm::MultiPolygonType;
}

