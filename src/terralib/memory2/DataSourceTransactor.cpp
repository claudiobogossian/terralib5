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
  \file terralib/memory/DataSourceTransactor.cpp

  \brief An implementation of the DataSourceTransactor class for the Memory Data Access driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess2/dataset/DataSetType.h"
#include "../geometry/Envelope.h"
#include "DataSet.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"
#include "Exception.h"


te::mem::DataSourceTransactor::DataSourceTransactor(DataSource* ds)
  : m_ds(ds)
{
}

te::mem::DataSourceTransactor::~DataSourceTransactor()
{
}

te::da::DataSource* te::mem::DataSourceTransactor::getDataSource() const
{
  return m_ds;
}

void te::mem::DataSourceTransactor::begin()
{
// Not supported by the Memory Driver!
}

void te::mem::DataSourceTransactor::commit()
{
// Not supported by the Memory Driver!
}

void te::mem::DataSourceTransactor::rollBack()
{
// Not supported by the Memory Driver!
}

bool te::mem::DataSourceTransactor::isInTransaction() const
{
  return false;
}

std::auto_ptr<te::da::DataSet> te::mem::DataSourceTransactor::getDataSet(const std::string& name,
                                                                         te::common::TraverseType travType,
                                                                         bool /*connected*/) 
{
  return m_ds->getDataSet(name, travType);
}

std::auto_ptr<te::da::DataSet> te::mem::DataSourceTransactor::getDataSet(const std::string& name,
                                                                         const std::string& propertyName,
                                                                         const te::gm::Envelope* e,
                                                                         te::gm::SpatialRelation r,
                                                                         te::common::TraverseType travType,
                                                                         bool /*connected*/)
{
  return std::auto_ptr<te::da::DataSet>(0);
}

std::auto_ptr<te::da::DataSet> te::mem::DataSourceTransactor::getDataSet(const std::string& name,
                                                                         const std::string& propertyName,
                                                                         const te::gm::Geometry* g,
                                                                         te::gm::SpatialRelation r,
                                                                         te::common::TraverseType travType,
                                                                         bool /*connected*/)
{
  return std::auto_ptr<te::da::DataSet>(0);
}

std::auto_ptr<te::da::DataSet> te::mem::DataSourceTransactor::query(const te::da::Select& q, 
                                                                    te::common::TraverseType travType, 
                                                                    bool /*connected*/)
{
  return std::auto_ptr<te::da::DataSet>(0);
}

std::auto_ptr<te::da::DataSet> te::mem::DataSourceTransactor::query(const std::string& q,
                                                                    te::common::TraverseType travType, 
                                                                    bool /*connected*/)
{
  return std::auto_ptr<te::da::DataSet>(0);
}

void te::mem::DataSourceTransactor::execute(const te::da::Query& /*command*/)
{
}

void te::mem::DataSourceTransactor::execute(const std::string& /*command*/)
{
}

std::auto_ptr<te::da::PreparedQuery> te::mem::DataSourceTransactor::getPrepared(const std::string& qName)
{
  return std::auto_ptr<te::da::PreparedQuery>(0);
}

std::auto_ptr<te::da::BatchExecutor> te::mem::DataSourceTransactor::getBatchExecutor()
{
  return std::auto_ptr<te::da::BatchExecutor>(0);
}

void te::mem::DataSourceTransactor::cancel()
{
}

boost::int64_t te::mem::DataSourceTransactor::getLastGeneratedId()
{
  return 0;
}

std::string te::mem::DataSourceTransactor::escape(const std::string& value)
{
  return std::string(value);
}

bool te::mem::DataSourceTransactor::isDataSetNameValid(const std::string& datasetName)
{
  return true;
}

bool te::mem::DataSourceTransactor::isPropertyNameValid(const std::string& propertyName)
{
  return true;
}

std::vector<std::string> te::mem::DataSourceTransactor::getDataSetNames()
{
  return m_ds->getDataSetNames();
}

std::size_t te::mem::DataSourceTransactor::getNumberOfDataSets()
{
  return m_ds->getNumberOfDataSets();
}

te::da::DataSetTypePtr te::mem::DataSourceTransactor::getDataSetType(const std::string& name)
{
  return m_ds->getDataSetType(name);
}

boost::ptr_vector<te::dt::Property> te::mem::DataSourceTransactor::getProperties(const std::string& datasetName)
{
  return m_ds->getProperties(datasetName);
}

std::auto_ptr<te::dt::Property> te::mem::DataSourceTransactor::getProperty(const std::string& datasetName, const std::string& name)
{
  return m_ds->getProperty(datasetName, name);
}

std::auto_ptr<te::dt::Property> te::mem::DataSourceTransactor::getProperty(const std::string& datasetName, std::size_t propertyPos)
{
  return m_ds->getProperty(datasetName, propertyPos);
}

std::vector<std::string> te::mem::DataSourceTransactor::getPropertyNames(const std::string& datasetName)
{
  return m_ds->getPropertyNames(datasetName);
}

std::size_t te::mem::DataSourceTransactor::getNumberOfProperties(const std::string& datasetName)
{
  return m_ds->getNumberOfProperties(datasetName);
}

bool te::mem::DataSourceTransactor::propertyExists(const std::string& datasetName, const std::string& name)
{
  return m_ds->propertyExists(datasetName, name);
}

void te::mem::DataSourceTransactor::addProperty(const std::string& datasetName, te::dt::Property* p)
{
  m_ds->addProperty(datasetName, p);
}

void te::mem::DataSourceTransactor::dropProperty(const std::string& datasetName, const std::string& name)
{
  m_ds->dropProperty(datasetName, name);
}

void te::mem::DataSourceTransactor::renameProperty(const std::string& datasetName,
                                                   const std::string& propertyName,
                                                   const std::string& newPropertyName)
{
  m_ds->renameProperty(datasetName, propertyName, newPropertyName);
}

std::auto_ptr<te::da::PrimaryKey> te::mem::DataSourceTransactor::getPrimaryKey(const std::string& datasetName)
{
  return std::auto_ptr<te::da::PrimaryKey>(0);
}

bool te::mem::DataSourceTransactor::primaryKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return false;
}

void te::mem::DataSourceTransactor::addPrimaryKey(const std::string& /*datasetName*/, te::da::PrimaryKey* /*pk*/)
{
}

void te::mem::DataSourceTransactor::dropPrimaryKey(const std::string& /*datasetName*/)
{
}

std::auto_ptr<te::da::ForeignKey> te::mem::DataSourceTransactor::getForeignKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return std::auto_ptr<te::da::ForeignKey>(0);
}

std::vector<std::string> te::mem::DataSourceTransactor::getForeignKeyNames(const std::string& /*datasetName*/)
{
  return std::vector<std::string>();
}

bool te::mem::DataSourceTransactor::foreignKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return false;
}

void te::mem::DataSourceTransactor::addForeignKey(const std::string& /*datasetName*/, te::da::ForeignKey* /*fk*/)
{
}

void te::mem::DataSourceTransactor::dropForeignKey(const std::string& /*datasetName*/, const std::string& /*fkName*/)
{
}

std::auto_ptr<te::da::UniqueKey> te::mem::DataSourceTransactor::getUniqueKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return std::auto_ptr<te::da::UniqueKey>(0);
}

std::vector<std::string> te::mem::DataSourceTransactor::getUniqueKeyNames(const std::string& /*datasetName*/)
{
  return std::vector<std::string>();
}

bool te::mem::DataSourceTransactor::uniqueKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return false;
}

void te::mem::DataSourceTransactor::addUniqueKey(const std::string& /*datasetName*/, te::da::UniqueKey* /*uk*/)
{
}

void te::mem::DataSourceTransactor::dropUniqueKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
}

std::auto_ptr<te::da::CheckConstraint> te::mem::DataSourceTransactor::getCheckConstraint(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return std::auto_ptr<te::da::CheckConstraint>(0);
}

std::vector<std::string> te::mem::DataSourceTransactor::getCheckConstraintNames(const std::string& /*datasetName*/)
{
  return std::vector<std::string>();
}

bool te::mem::DataSourceTransactor::checkConstraintExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return false;
}

void te::mem::DataSourceTransactor::addCheckConstraint(const std::string& /*datasetName*/, te::da::CheckConstraint* /*cc*/)
{
}

void te::mem::DataSourceTransactor::dropCheckConstraint(const std::string& /*datasetName*/, const std::string& /*name*/)
{
}

std::auto_ptr<te::da::Index> te::mem::DataSourceTransactor::getIndex(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return std::auto_ptr<te::da::Index>();
}

std::vector<std::string> te::mem::DataSourceTransactor::getIndexNames(const std::string& /*datasetName*/)
{
  return std::vector<std::string>();
}

bool te::mem::DataSourceTransactor::indexExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return false;
}

void te::mem::DataSourceTransactor::addIndex(const std::string& /*datasetName*/, te::da::Index* /*idx*/,
                                             const std::map<std::string, std::string>& /*options*/) 
{
}

void te::mem::DataSourceTransactor::dropIndex(const std::string& /*datasetName*/, const std::string& /*idxName*/)
{
}

std::auto_ptr<te::da::Sequence> te::mem::DataSourceTransactor::getSequence(const std::string& /*name*/)
{
  return std::auto_ptr<te::da::Sequence>(0);
}

std::vector<std::string> te::mem::DataSourceTransactor::getSequenceNames()
{
  return std::vector<std::string>();
}

bool te::mem::DataSourceTransactor::sequenceExists(const std::string& /*name*/)
{
  return false;
}

void te::mem::DataSourceTransactor::addSequence(te::da::Sequence* /*sequence*/)
{
}

void te::mem::DataSourceTransactor::dropSequence(const std::string& /*name*/)
{
}

std::auto_ptr<te::gm::Envelope> te::mem::DataSourceTransactor::getExtent(const std::string& /*datasetName*/,
                                                                         const std::string& /*propertyName*/)
{
  return std::auto_ptr<te::gm::Envelope>(0);
}

std::auto_ptr<te::gm::Envelope> te::mem::DataSourceTransactor::getExtent(const std::string& /*datasetName*/,
                                                                         std::size_t /*propertyPos*/)
{
  return std::auto_ptr<te::gm::Envelope>(0);
}

std::size_t te::mem::DataSourceTransactor::getNumberOfItems(const std::string& datasetName)
{
  return m_ds->getNumberOfItems(datasetName);
}

bool te::mem::DataSourceTransactor::hasDataSets()
{
  return m_ds->hasDataSets();
}

bool te::mem::DataSourceTransactor::dataSetExists(const std::string& name)
{
  return m_ds->dataSetExists(name);
}

void te::mem::DataSourceTransactor::createDataSet(te::da::DataSetType* dt, const std::map<std::string, std::string>& options)
{
  m_ds->createDataSet(dt, options);
}

void te::mem::DataSourceTransactor::cloneDataSet(const std::string& name,
                                                 const std::string& cloneName,
                                                 const std::map<std::string, std::string>& options)
{
  m_ds->cloneDataSet(name, cloneName, options);
}

void te::mem::DataSourceTransactor::dropDataSet(const std::string& name)
{
  m_ds->dropDataSet(name);
}

void te::mem::DataSourceTransactor::renameDataSet(const std::string& name, const std::string& newName)
{
  m_ds->renameDataSet(name, newName);
}

void te::mem::DataSourceTransactor::add(const std::string& datasetName,
                                        te::da::DataSet* d,
                                        const std::map<std::string, std::string>& options,
                                        std::size_t limit)
{
  m_ds->add(datasetName, d, options, limit);
}

void te::mem::DataSourceTransactor::remove(const std::string& datasetName, const te::da::ObjectIdSet* oids)
{
  m_ds->remove(datasetName, oids);
}

void te::mem::DataSourceTransactor::update(const std::string& datasetName,
                                           te::da::DataSet* dataset,
                                           const std::vector<std::size_t>& properties,
                                           const te::da::ObjectIdSet* oids,
                                           const std::map<std::string, std::string>& options,
                                           std::size_t limit)
{
  m_ds->update(datasetName, dataset, properties, oids, options, limit);
}

void te::mem::DataSourceTransactor::optimize(const std::map<std::string, std::string>& /*opInfo*/)
{
}
