/*  Copyright (C) 2009-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/sqlite/DataSourceTransactor.cpp

  \brief An implementation of DataSourceTransactor class for the TerraLib SQLite Data Access Driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "DataSourceTransactor.h"

te::sqlite::DataSourceTransactor::DataSourceTransactor(DataSource* parent, sqlite3* db)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

te::sqlite::DataSourceTransactor::~DataSourceTransactor()
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

te::da::DataSource* te::sqlite::DataSourceTransactor::getDataSource() const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::begin()
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::commit()
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::rollBack()
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::DataSourceTransactor::isInTransaction() const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::da::DataSet>
te::sqlite::DataSourceTransactor::getDataSet(const std::string& name, 
                                             te::common::TraverseType travType, 
                                             bool connected,
                                             const te::common::AccessPolicy accessPolicy)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::da::DataSet>
te::sqlite::DataSourceTransactor::getDataSet(const std::string& name,
                                            const std::string& propertyName,
                                            const te::gm::Envelope* e,
                                            te::gm::SpatialRelation r,
                                            te::common::TraverseType travType,
                                            bool connected,
                                            const te::common::AccessPolicy accessPolicy)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::da::DataSet>
te::sqlite::DataSourceTransactor::getDataSet(const std::string& name,
                                             const std::string& propertyName,
                                             const te::gm::Geometry* g,
                                             te::gm::SpatialRelation r,
                                             te::common::TraverseType travType,
                                             bool connected,
                                             const te::common::AccessPolicy accessPolicy)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::da::DataSet>
te::sqlite::DataSourceTransactor::query(const te::da::Select& q,
                                        te::common::TraverseType travType,
                                        bool connected,
                                        const te::common::AccessPolicy accessPolicy)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::da::DataSet>
te::sqlite::DataSourceTransactor::query(const std::string& query,
                                        te::common::TraverseType travType,
                                        bool connected,
                                        const te::common::AccessPolicy accessPolicy)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::execute(const te::da::Query& command)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::execute(const std::string& command)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::da::PreparedQuery>
te::sqlite::DataSourceTransactor::getPrepared(const std::string& qName)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::da::BatchExecutor> te::sqlite::DataSourceTransactor::getBatchExecutor()
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::cancel()
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

boost::int64_t te::sqlite::DataSourceTransactor::getLastGeneratedId()
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::string te::sqlite::DataSourceTransactor::escape(const std::string& value)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::DataSourceTransactor::isDataSetNameValid(const std::string& datasetName)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::DataSourceTransactor::isPropertyNameValid(const std::string& propertyName)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::vector<std::string>
te::sqlite::DataSourceTransactor::getDataSetNames()
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::size_t te::sqlite::DataSourceTransactor::getNumberOfDataSets()
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::da::DataSetType>
te::sqlite::DataSourceTransactor::getDataSetType(const std::string& name)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

boost::ptr_vector<te::dt::Property>
te::sqlite::DataSourceTransactor::getProperties(const std::string& datasetName)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::dt::Property>
te::sqlite::DataSourceTransactor::getProperty(const std::string& datasetName,
                                              const std::string& name)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::dt::Property> te::sqlite::DataSourceTransactor::getProperty(const std::string& datasetName, std::size_t propertyPos)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::vector<std::string> te::sqlite::DataSourceTransactor::getPropertyNames(const std::string& datasetName)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::size_t te::sqlite::DataSourceTransactor::getNumberOfProperties(const std::string& datasetName)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::DataSourceTransactor::propertyExists(const std::string& datasetName, const std::string& name)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::addProperty(const std::string& datasetName, te::dt::Property* p)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::dropProperty(const std::string& datasetName, const std::string& name)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::renameProperty(const std::string& datasetName,
                                                      const std::string& propertyName,
                                                      const std::string& newPropertyName)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::da::PrimaryKey> te::sqlite::DataSourceTransactor::getPrimaryKey(const std::string& datasetName)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::DataSourceTransactor::primaryKeyExists(const std::string& datasetName, const std::string& name)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::addPrimaryKey(const std::string& datasetName, te::da::PrimaryKey* pk)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::dropPrimaryKey(const std::string& datasetName)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::da::ForeignKey> te::sqlite::DataSourceTransactor::getForeignKey(const std::string& datasetName, const std::string& name)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::vector<std::string> te::sqlite::DataSourceTransactor::getForeignKeyNames(const std::string& datasetName)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::DataSourceTransactor::foreignKeyExists(const std::string& datasetName, const std::string& name)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::addForeignKey(const std::string& datasetName, te::da::ForeignKey* fk)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::dropForeignKey(const std::string& datasetName, const std::string& fkName)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::da::UniqueKey> te::sqlite::DataSourceTransactor::getUniqueKey(const std::string& datasetName, const std::string& name)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::vector<std::string> te::sqlite::DataSourceTransactor::getUniqueKeyNames(const std::string& datasetName)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::DataSourceTransactor::uniqueKeyExists(const std::string& datasetName, const std::string& name)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::addUniqueKey(const std::string& datasetName, te::da::UniqueKey* uk)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::dropUniqueKey(const std::string& datasetName, const std::string& name)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::da::CheckConstraint> te::sqlite::DataSourceTransactor::getCheckConstraint(const std::string& datasetName, const std::string& name)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::vector<std::string> te::sqlite::DataSourceTransactor::getCheckConstraintNames(const std::string& datasetName)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::DataSourceTransactor::checkConstraintExists(const std::string& datasetName, const std::string& name)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::addCheckConstraint(const std::string& datasetName, te::da::CheckConstraint* cc)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::dropCheckConstraint(const std::string& datasetName, const std::string& name)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::da::Index> te::sqlite::DataSourceTransactor::getIndex(const std::string& datasetName, const std::string& name)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::vector<std::string> te::sqlite::DataSourceTransactor::getIndexNames(const std::string& datasetName)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::DataSourceTransactor::indexExists(const std::string& datasetName, const std::string& name)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::addIndex(const std::string& datasetName,
              te::da::Index* idx,
              const std::map<std::string, std::string>& options)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::dropIndex(const std::string& datasetName, const std::string& idxName)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::da::Sequence> te::sqlite::DataSourceTransactor::getSequence(const std::string& name)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::vector<std::string> te::sqlite::DataSourceTransactor::getSequenceNames()
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::DataSourceTransactor::sequenceExists(const std::string& name)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::addSequence(te::da::Sequence* sequence)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::dropSequence(const std::string& name)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::gm::Envelope> te::sqlite::DataSourceTransactor::getExtent(const std::string& datasetName,
                                                                            const std::string& propertyName)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::gm::Envelope> te::sqlite::DataSourceTransactor::getExtent(const std::string& datasetName,
                                                                            std::size_t propertyPos)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::size_t te::sqlite::DataSourceTransactor::getNumberOfItems(const std::string& datasetName)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::DataSourceTransactor::hasDataSets()
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::DataSourceTransactor::dataSetExists(const std::string& name)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::createDataSet(te::da::DataSetType* dt,
                                                     const std::map<std::string, std::string>& options)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::cloneDataSet(const std::string& name,
                                                    const std::string& cloneName,
                                                    const std::map<std::string, std::string>& options)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::dropDataSet(const std::string& name)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::renameDataSet(const std::string& name, const std::string& newName)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::add(const std::string& datasetName,
                                           te::da::DataSet* d,
                                           const std::map<std::string, std::string>& options,
                                           std::size_t limit)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::remove(const std::string& datasetName, const te::da::ObjectIdSet* oids)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::update(const std::string& datasetName,
                                              te::da::DataSet* dataset,
                                              const std::vector<std::size_t>& properties,
                                              const te::da::ObjectIdSet* oids,
                                              const std::map<std::string, std::string>& options,
                                              std::size_t limit)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

void te::sqlite::DataSourceTransactor::optimize(const std::map<std::string, std::string>& opInfo)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}
sqlite3* te::sqlite::DataSourceTransactor::getDB() const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}
