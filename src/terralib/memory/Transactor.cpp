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
  \file terralib/memory/Transactor.cpp

  \brief An implementation of the Transactor class for the Memory Data Access driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../geometry/Envelope.h"
#include "DataSet.h"
#include "DataSource.h"
#include "Transactor.h"
#include "Exception.h"


te::mem::Transactor::Transactor(DataSource* ds)
  : m_ds(ds)
{
}

te::mem::Transactor::~Transactor()
{
}

te::da::DataSource* te::mem::Transactor::getDataSource() const
{
  return m_ds;
}

void te::mem::Transactor::begin()
{
// Not supported by the Memory Driver!
}

void te::mem::Transactor::commit()
{
// Not supported by the Memory Driver!
}

void te::mem::Transactor::rollBack()
{
// Not supported by the Memory Driver!
}

bool te::mem::Transactor::isInTransaction() const
{
  return false;
}

std::auto_ptr<te::da::DataSet> te::mem::Transactor::getDataSet(const std::string& name,
                                                                         te::common::TraverseType travType,
                                                                         bool /*connected*/) 
{
  return m_ds->getDataSet(name, travType);
}

std::auto_ptr<te::da::DataSet> te::mem::Transactor::getDataSet(const std::string& name,
                                                                         const std::string& propertyName,
                                                                         const te::gm::Envelope* e,
                                                                         te::gm::SpatialRelation r,
                                                                         te::common::TraverseType travType,
                                                                         bool /*connected*/)
{
  return std::auto_ptr<te::da::DataSet>(0);
}

std::auto_ptr<te::da::DataSet> te::mem::Transactor::getDataSet(const std::string& name,
                                                                         const std::string& propertyName,
                                                                         const te::gm::Geometry* g,
                                                                         te::gm::SpatialRelation r,
                                                                         te::common::TraverseType travType,
                                                                         bool /*connected*/)
{
  return std::auto_ptr<te::da::DataSet>(0);
}

std::auto_ptr<te::da::DataSet> te::mem::Transactor::query(const te::da::Select& q, 
                                                                    te::common::TraverseType travType, 
                                                                    bool /*connected*/)
{
  return std::auto_ptr<te::da::DataSet>(0);
}

std::auto_ptr<te::da::DataSet> te::mem::Transactor::query(const std::string& q,
                                                                    te::common::TraverseType travType, 
                                                                    bool /*connected*/)
{
  return std::auto_ptr<te::da::DataSet>(0);
}

void te::mem::Transactor::execute(const te::da::Query& /*command*/)
{
}

void te::mem::Transactor::execute(const std::string& /*command*/)
{
}

std::auto_ptr<te::da::PreparedQuery> te::mem::Transactor::getPrepared(const std::string& qName)
{
  return std::auto_ptr<te::da::PreparedQuery>(0);
}

std::auto_ptr<te::da::BatchExecutor> te::mem::Transactor::getBatchExecutor()
{
  return std::auto_ptr<te::da::BatchExecutor>(0);
}

void te::mem::Transactor::cancel()
{
}

boost::int64_t te::mem::Transactor::getLastGeneratedId()
{
  return 0;
}

std::string te::mem::Transactor::escape(const std::string& value)
{
  return std::string(value);
}

bool te::mem::Transactor::isDataSetNameValid(const std::string& datasetName)
{
  return true;
}

bool te::mem::Transactor::isPropertyNameValid(const std::string& propertyName)
{
  return true;
}

std::vector<std::string> te::mem::Transactor::getDataSetNames()
{
  return m_ds->getDataSetNames();
}

std::size_t te::mem::Transactor::getNumberOfDataSets()
{
  return m_ds->getNumberOfDataSets();
}

std::auto_ptr<te::da::DataSetType> te::mem::Transactor::getDataSetType(const std::string& name)
{
  return m_ds->getDataSetType(name);
}

boost::ptr_vector<te::dt::Property> te::mem::Transactor::getProperties(const std::string& datasetName)
{
  return m_ds->getProperties(datasetName);
}

std::auto_ptr<te::dt::Property> te::mem::Transactor::getProperty(const std::string& datasetName, const std::string& name)
{
  return m_ds->getProperty(datasetName, name);
}

std::auto_ptr<te::dt::Property> te::mem::Transactor::getProperty(const std::string& datasetName, std::size_t propertyPos)
{
  return m_ds->getProperty(datasetName, propertyPos);
}

std::vector<std::string> te::mem::Transactor::getPropertyNames(const std::string& datasetName)
{
  return m_ds->getPropertyNames(datasetName);
}

std::size_t te::mem::Transactor::getNumberOfProperties(const std::string& datasetName)
{
  return m_ds->getNumberOfProperties(datasetName);
}

bool te::mem::Transactor::propertyExists(const std::string& datasetName, const std::string& name)
{
  return m_ds->propertyExists(datasetName, name);
}

void te::mem::Transactor::addProperty(const std::string& datasetName, te::dt::Property* p)
{
  m_ds->addProperty(datasetName, p);
}

void te::mem::Transactor::dropProperty(const std::string& datasetName, const std::string& name)
{
  m_ds->dropProperty(datasetName, name);
}

void te::mem::Transactor::renameProperty(const std::string& datasetName,
                                                   const std::string& propertyName,
                                                   const std::string& newPropertyName)
{
  m_ds->renameProperty(datasetName, propertyName, newPropertyName);
}

std::auto_ptr<te::da::PrimaryKey> te::mem::Transactor::getPrimaryKey(const std::string& datasetName)
{
  return std::auto_ptr<te::da::PrimaryKey>(0);
}

bool te::mem::Transactor::primaryKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return false;
}

void te::mem::Transactor::addPrimaryKey(const std::string& /*datasetName*/, te::da::PrimaryKey* /*pk*/)
{
}

void te::mem::Transactor::dropPrimaryKey(const std::string& /*datasetName*/)
{
}

std::auto_ptr<te::da::ForeignKey> te::mem::Transactor::getForeignKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return std::auto_ptr<te::da::ForeignKey>(0);
}

std::vector<std::string> te::mem::Transactor::getForeignKeyNames(const std::string& /*datasetName*/)
{
  return std::vector<std::string>();
}

bool te::mem::Transactor::foreignKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return false;
}

void te::mem::Transactor::addForeignKey(const std::string& /*datasetName*/, te::da::ForeignKey* /*fk*/)
{
}

void te::mem::Transactor::dropForeignKey(const std::string& /*datasetName*/, const std::string& /*fkName*/)
{
}

std::auto_ptr<te::da::UniqueKey> te::mem::Transactor::getUniqueKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return std::auto_ptr<te::da::UniqueKey>(0);
}

std::vector<std::string> te::mem::Transactor::getUniqueKeyNames(const std::string& /*datasetName*/)
{
  return std::vector<std::string>();
}

bool te::mem::Transactor::uniqueKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return false;
}

void te::mem::Transactor::addUniqueKey(const std::string& /*datasetName*/, te::da::UniqueKey* /*uk*/)
{
}

void te::mem::Transactor::dropUniqueKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
}

std::auto_ptr<te::da::CheckConstraint> te::mem::Transactor::getCheckConstraint(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return std::auto_ptr<te::da::CheckConstraint>(0);
}

std::vector<std::string> te::mem::Transactor::getCheckConstraintNames(const std::string& /*datasetName*/)
{
  return std::vector<std::string>();
}

bool te::mem::Transactor::checkConstraintExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return false;
}

void te::mem::Transactor::addCheckConstraint(const std::string& /*datasetName*/, te::da::CheckConstraint* /*cc*/)
{
}

void te::mem::Transactor::dropCheckConstraint(const std::string& /*datasetName*/, const std::string& /*name*/)
{
}

std::auto_ptr<te::da::Index> te::mem::Transactor::getIndex(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return std::auto_ptr<te::da::Index>();
}

std::vector<std::string> te::mem::Transactor::getIndexNames(const std::string& /*datasetName*/)
{
  return std::vector<std::string>();
}

bool te::mem::Transactor::indexExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return false;
}

void te::mem::Transactor::addIndex(const std::string& /*datasetName*/, te::da::Index* /*idx*/,
                                             const std::map<std::string, std::string>& /*options*/) 
{
}

void te::mem::Transactor::dropIndex(const std::string& /*datasetName*/, const std::string& /*idxName*/)
{
}

std::auto_ptr<te::da::Sequence> te::mem::Transactor::getSequence(const std::string& /*name*/)
{
  return std::auto_ptr<te::da::Sequence>(0);
}

std::vector<std::string> te::mem::Transactor::getSequenceNames()
{
  return std::vector<std::string>();
}

bool te::mem::Transactor::sequenceExists(const std::string& /*name*/)
{
  return false;
}

void te::mem::Transactor::addSequence(te::da::Sequence* /*sequence*/)
{
}

void te::mem::Transactor::dropSequence(const std::string& /*name*/)
{
}

std::auto_ptr<te::gm::Envelope> te::mem::Transactor::getExtent(const std::string& /*datasetName*/,
                                                                         const std::string& /*propertyName*/)
{
  return std::auto_ptr<te::gm::Envelope>(0);
}

std::auto_ptr<te::gm::Envelope> te::mem::Transactor::getExtent(const std::string& /*datasetName*/,
                                                                         std::size_t /*propertyPos*/)
{
  return std::auto_ptr<te::gm::Envelope>(0);
}

std::size_t te::mem::Transactor::getNumberOfItems(const std::string& datasetName)
{
  return m_ds->getNumberOfItems(datasetName);
}

bool te::mem::Transactor::hasDataSets()
{
  return m_ds->hasDataSets();
}

bool te::mem::Transactor::dataSetExists(const std::string& name)
{
  return m_ds->dataSetExists(name);
}

void te::mem::Transactor::createDataSet(te::da::DataSetType* dt, const std::map<std::string, std::string>& options)
{
  m_ds->createDataSet(dt, options);
}

void te::mem::Transactor::cloneDataSet(const std::string& name,
                                                 const std::string& cloneName,
                                                 const std::map<std::string, std::string>& options)
{
  m_ds->cloneDataSet(name, cloneName, options);
}

void te::mem::Transactor::dropDataSet(const std::string& name)
{
  m_ds->dropDataSet(name);
}

void te::mem::Transactor::renameDataSet(const std::string& name, const std::string& newName)
{
  m_ds->renameDataSet(name, newName);
}

void te::mem::Transactor::add(const std::string& datasetName,
                                        te::da::DataSet* d,
                                        const std::map<std::string, std::string>& options,
                                        std::size_t limit)
{
  m_ds->add(datasetName, d, options, limit);
}

void te::mem::Transactor::remove(const std::string& datasetName, const te::da::ObjectIdSet* oids)
{
  m_ds->remove(datasetName, oids);
}

void te::mem::Transactor::update(const std::string& datasetName,
                                           te::da::DataSet* dataset,
                                           const std::vector<std::size_t>& properties,
                                           const te::da::ObjectIdSet* oids,
                                           const std::map<std::string, std::string>& options,
                                           std::size_t limit)
{
  m_ds->update(datasetName, dataset, properties, oids, options, limit);
}

void te::mem::Transactor::optimize(const std::map<std::string, std::string>& /*opInfo*/)
{
}

te::common::CharEncoding te::mem::Transactor::getEncoding()
{
  return te::common::UTF8;
}
