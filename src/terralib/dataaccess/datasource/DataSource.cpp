/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/datasource/DataSource.cpp

  \brief An abstract class for data providers like a DBMS, Web Services or a regular file.
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../geometry/Envelope.h"
#include "../dataset/ObjectIdSet.h"
#include "../query/DataSetName.h"
#include "../query/Field.h"
#include "../query/Fields.h"
#include "../query/From.h"
#include "../query/FromItem.h"
#include "../query/Select.h"
#include "../query/Where.h"
#include "DataSource.h"
#include "DataSourceCatalog.h"
#include "DataSourceFactory.h"
#include "DataSourceTransactor.h"

te::da::DataSource::DataSource()
{
}

te::da::DataSource::~DataSource()
{
}

const std::string& te::da::DataSource::getId() const
{
  return m_id;
}

void te::da::DataSource::setId(const std::string& id)
{
  m_id = id;
}

std::auto_ptr<te::da::DataSet> te::da::DataSource::getDataSet(const std::string& name, 
                                                              te::common::TraverseType travType,
                                                              const te::common::AccessPolicy accessPolicy)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getDataSet(name, travType, false, accessPolicy);
}

std::auto_ptr<te::da::DataSet> te::da::DataSource::getDataSet(const std::string& name,
                                                              const std::string& propertyName,
                                                              const te::gm::Envelope* e,
                                                              te::gm::SpatialRelation r,
                                                              te::common::TraverseType travType,
                                                              const te::common::AccessPolicy accessPolicy)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getDataSet(name, propertyName, e, r, travType, false, accessPolicy);
}

std::auto_ptr<te::da::DataSet> te::da::DataSource::getDataSet(const std::string& name,
                                                              const std::string& propertyName,
                                                              const te::gm::Geometry* g,
                                                              te::gm::SpatialRelation r,
                                                              te::common::TraverseType travType,
                                                              const te::common::AccessPolicy accessPolicy)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getDataSet(name, propertyName, g, r, travType, false, accessPolicy);
}

std::auto_ptr<te::da::DataSet> te::da::DataSource::getDataSet(const std::string& name,
                                                              const te::da::ObjectIdSet* oids,
                                                              te::common::TraverseType travType,
                                                              const te::common::AccessPolicy accessPolicy)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getDataSet(name, oids, travType, false, accessPolicy);
}

std::auto_ptr<te::da::DataSet> te::da::DataSource::query(const Select& q, te::common::TraverseType travType,
                                                         const te::common::AccessPolicy accessPolicy)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->query(q, travType, false,accessPolicy);
}

std::auto_ptr<te::da::DataSet> te::da::DataSource::query(const std::string& query, te::common::TraverseType travType,
                                                         const te::common::AccessPolicy accessPolicy)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->query(query, travType, false,accessPolicy);
}

void te::da::DataSource::execute(const Query& command)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->execute(command);
}

void te::da::DataSource::execute(const std::string& command)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->execute(command);
}

std::string te::da::DataSource::escape(const std::string& value)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->escape(value);
}

bool te::da::DataSource::isDataSetNameValid(const std::string& datasetName)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->isDataSetNameValid(datasetName);
}

bool te::da::DataSource::isPropertyNameValid(const std::string& propertyName)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->isPropertyNameValid(propertyName);
}

std::vector<std::string> te::da::DataSource::getDataSetNames()
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getDataSetNames();
}

std::size_t te::da::DataSource::getNumberOfDataSets()
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getNumberOfDataSets();
}

std::auto_ptr<te::da::DataSetType> te::da::DataSource::getDataSetType(const std::string& name)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getDataSetType(name);
}

std::auto_ptr<te::da::DataSetTypeCapabilities> te::da::DataSource::getCapabilities(const std::string &name)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getCapabilities(name);
}

boost::ptr_vector<te::dt::Property> te::da::DataSource::getProperties(const std::string& datasetName)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getProperties(datasetName);
}

std::vector<std::string> te::da::DataSource::getPropertyNames(const std::string& datasetName)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getPropertyNames(datasetName);
}

std::size_t te::da::DataSource::getNumberOfProperties(const std::string& datasetName)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getNumberOfProperties(datasetName);
}

bool te::da::DataSource::propertyExists(const std::string& datasetName, const std::string& name)
{  
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->propertyExists(datasetName, name);
}

std::auto_ptr<te::dt::Property> te::da::DataSource::getProperty(const std::string& datasetName, const std::string& name)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getProperty(datasetName, name);  
}

std::auto_ptr<te::dt::Property> te::da::DataSource::getProperty(const std::string& datasetName, std::size_t propertyPos)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getProperty(datasetName, propertyPos);  
}

void te::da::DataSource::addProperty(const std::string& datasetName, te::dt::Property* p)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->addProperty(datasetName, p);  
}

void te::da::DataSource::dropProperty(const std::string& datasetName, const std::string& name)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->dropProperty(datasetName, name);  
}

void te::da::DataSource::renameProperty(const std::string& datasetName,
                                        const std::string& propertyName,
                                        const std::string& newPropertyName)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->renameProperty(datasetName, propertyName, newPropertyName);  
}

void te::da::DataSource::changePropertyDefinition(const std::string& datasetName, const std::string& propName, te::dt::Property* newProp)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->changePropertyDefinition(datasetName, propName, newProp);  
}

void te::da::DataSource::changePropertiesDefinitions(const std::string& datasetName, const std::vector<std::string>& propsNames, const std::vector<te::dt::Property*> newProps)
{
  assert(propsNames.size() == newProps.size());

  for(std::size_t i = 0; i < propsNames.size(); ++i)
    changePropertyDefinition(datasetName, propsNames[i], newProps[i]);
}


std::auto_ptr<te::da::PrimaryKey> te::da::DataSource::getPrimaryKey(const std::string& datasetName)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getPrimaryKey(datasetName);
}

bool te::da::DataSource::primaryKeyExists(const std::string& datasetName, const std::string& name)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->primaryKeyExists(datasetName, name);
}

void te::da::DataSource::addPrimaryKey(const std::string& datasetName, PrimaryKey* pk)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->addPrimaryKey(datasetName, pk);
}

void te::da::DataSource::dropPrimaryKey(const std::string& datasetName)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->dropPrimaryKey(datasetName);
}

std::auto_ptr<te::da::ForeignKey> te::da::DataSource::getForeignKey(const std::string& datasetName, const std::string& name)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getForeignKey(datasetName, name);
}

std::vector<std::string> te::da::DataSource::getForeignKeyNames(const std::string& datasetName)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getForeignKeyNames(datasetName);
}

bool te::da::DataSource::foreignKeyExists(const std::string& datasetName, const std::string& name)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->foreignKeyExists(datasetName, name);
}

void te::da::DataSource::addForeignKey(const std::string& datasetName, ForeignKey* fk)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->addForeignKey(datasetName, fk);
}

void te::da::DataSource::dropForeignKey(const std::string& datasetName, const std::string& fkName)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->dropForeignKey(datasetName, fkName);
}

std::auto_ptr<te::da::UniqueKey> te::da::DataSource::getUniqueKey(const std::string& datasetName, 
                                                                  const std::string& name)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getUniqueKey(datasetName, name);
}

std::vector<std::string> te::da::DataSource::getUniqueKeyNames(const std::string& datasetName)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getUniqueKeyNames(datasetName);
}

bool te::da::DataSource::uniqueKeyExists(const std::string& datasetName, const std::string& name)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->uniqueKeyExists(datasetName, name);
}

void te::da::DataSource::addUniqueKey(const std::string& datasetName, UniqueKey* uk)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->addUniqueKey(datasetName, uk);
}

void te::da::DataSource::dropUniqueKey(const std::string& datasetName, const std::string& name)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->dropUniqueKey(datasetName, name);
}

std::auto_ptr<te::da::CheckConstraint> te::da::DataSource::getCheckConstraint(const std::string& datasetName, 
                                                                              const std::string& name)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getCheckConstraint(datasetName, name);
}

std::vector<std::string> te::da::DataSource::getCheckConstraintNames(const std::string& datasetName)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getCheckConstraintNames(datasetName);
}

bool te::da::DataSource::checkConstraintExists(const std::string& datasetName, const std::string& name)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->checkConstraintExists(datasetName, name);
}

void te::da::DataSource::addCheckConstraint(const std::string& datasetName, CheckConstraint* cc)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->addCheckConstraint(datasetName, cc);
}

void te::da::DataSource::dropCheckConstraint(const std::string& datasetName, const std::string& name)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->dropCheckConstraint(datasetName, name);
}

std::auto_ptr<te::da::Index> te::da::DataSource::getIndex(const std::string& datasetName, const std::string& name)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getIndex(datasetName, name);
}

std::vector<std::string> te::da::DataSource::getIndexNames(const std::string& datasetName)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getIndexNames(datasetName);
}

bool te::da::DataSource::indexExists(const std::string& datasetName, const std::string& name)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->indexExists(datasetName, name);
}

void te::da::DataSource::addIndex(const std::string& datasetName, Index* idx,
                                  const std::map<std::string, std::string>& options)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->addIndex(datasetName, idx, options);
}

void te::da::DataSource::dropIndex(const std::string& datasetName, const std::string& idxName)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->dropIndex(datasetName, idxName);
}

std::auto_ptr<te::da::Sequence> te::da::DataSource::getSequence(const std::string& name)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getSequence(name);
}

std::vector<std::string> te::da::DataSource::getSequenceNames()
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getSequenceNames();
}

bool te::da::DataSource::sequenceExists(const std::string& name)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->sequenceExists(name);
}

void te::da::DataSource::addSequence(Sequence* sequence)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->addSequence(sequence);
}

void te::da::DataSource::dropSequence(const std::string& name)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->dropSequence(name);
}

std::auto_ptr<te::gm::Envelope> te::da::DataSource::getExtent(const std::string& datasetName, const std::string& propertyName)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getExtent(datasetName, propertyName);
}

std::auto_ptr<te::gm::Envelope> te::da::DataSource::getExtent(const std::string& datasetName, std::size_t propertyPos)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getExtent(datasetName, propertyPos);
}

std::size_t te::da::DataSource::getNumberOfItems(const std::string& datasetName)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->getNumberOfItems(datasetName);
}

bool te::da::DataSource::hasDataSets()
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->hasDataSets();
}

bool te::da::DataSource::dataSetExists(const std::string& name)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->dataSetExists(name);
}

void te::da::DataSource::createDataSet(DataSetType* dt, const std::map<std::string, std::string>& options)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->createDataSet(dt, options);
}

void te::da::DataSource::cloneDataSet(const std::string& name,
                                      const std::string& cloneName,
                                      const std::map<std::string, std::string>& options)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->cloneDataSet(name, cloneName, options);
}

void te::da::DataSource::dropDataSet(const std::string& name)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->dropDataSet(name);
}

void te::da::DataSource::renameDataSet(const std::string& name, const std::string& newName)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->renameDataSet(name, newName);
}

void te::da::DataSource::add(const std::string& datasetName,
                             DataSet* d,
                             const std::map<std::string, std::string>& options,
                             std::size_t limit)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->add(datasetName, d, options, limit);
}

void te::da::DataSource::remove(const std::string& datasetName, const ObjectIdSet* oids)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->remove(datasetName);
}

void te::da::DataSource::update(const std::string& datasetName,
                                DataSet* dataset,
                                const std::vector<std::size_t>& properties,
                                const ObjectIdSet* oids,
                                const std::map<std::string, std::string>& options,
                                std::size_t limit)
{
  std::auto_ptr<DataSourceTransactor> t = getTransactor();
  return t->update(datasetName, dataset, properties, oids, options, limit);
}

std::auto_ptr<te::da::DataSource> te::da::DataSource::create(const std::string& dsType, const std::map<std::string, std::string>& dsInfo)
{
  std::auto_ptr<DataSource> ds(DataSourceFactory::make(dsType));

  if(ds.get() == 0)
    throw Exception(TR_DATAACCESS("Could not find the appropriate factory to create a data source instance!"));

  ds->create(dsInfo);

  return ds;
}

void te::da::DataSource::drop(const std::string& dsType, const std::map<std::string, std::string>& dsInfo)
{
  std::auto_ptr<DataSource> ds(DataSourceFactory::make(dsType));

  if(ds.get() == 0)
    throw Exception(TR_DATAACCESS("Could not find the appropriate factory to create a data source instance!"));

  ds->drop(dsInfo);
}

bool te::da::DataSource::exists(const std::string& dsType, const std::map<std::string, std::string>& dsInfo)
{
  std::auto_ptr<DataSource> ds(DataSourceFactory::make(dsType));

  if(ds.get() == 0)
    throw Exception(TR_DATAACCESS("Could not find the appropriate factory in order to create a data source instance!"));

  return ds->exists(dsInfo);
}

std::vector<std::string> te::da::DataSource::getDataSourceNames(const std::string& dsType, const std::map<std::string, std::string>& dsInfo)
{
  std::auto_ptr<DataSource> ds(DataSourceFactory::make(dsType));

  if(ds.get() == 0)
    throw Exception(TR_DATAACCESS("Could not find the appropriate factory to create a data source instance!"));

  return ds->getDataSourceNames(dsInfo);
}

std::vector<te::common::CharEncoding> te::da::DataSource::getEncodings(const std::string& dsType, const std::map<std::string, std::string>& dsInfo)
{
  std::auto_ptr<DataSource> ds(DataSourceFactory::make(dsType));

  if(ds.get() == 0)
    throw Exception(TR_DATAACCESS("Could not find the appropriate factory to create a data source instance!"));

  return ds->getEncodings(dsInfo);
}
