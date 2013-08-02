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
#include "DataSourceFactory.h"


te::da::DataSource::DataSource()
{
}

te::da::DataSource::~DataSource()
{
}

te::da::Connection* te::da::DataSource::getConnection()
{
  return 0;
}

void te::da::DataSource::closeConnection(te::da::Connection* /*conn*/)
{
}

const te::da::SQLDialect* te::da::DataSource::getDialect() const
{
  return 0;
}

void te::da::DataSource::begin()
{
}

void te::da::DataSource::commit()
{
}

void te::da::DataSource::rollBack()
{
}

bool te::da::DataSource::isInTransaction() const
{
  return false;
}

std::auto_ptr<te::da::DataSet> te::da::DataSource::getDataSet(const std::string& /*name*/,
                                                              const std::string& /*propertyName*/,
                                                              const te::gm::Envelope* /*e*/,
                                                              te::gm::SpatialRelation /*r*/,
                                                              te::common::TraverseType /*travType*/)
{
  return std::auto_ptr<te::da::DataSet>(0);
}

std::auto_ptr<te::da::DataSet> te::da::DataSource::getDataSet(const std::string& /*name*/,
                                                              const std::string& /*propertyName*/,
                                                              const te::gm::Geometry* /*g*/,
                                                              te::gm::SpatialRelation /*r*/,
                                                              te::common::TraverseType /*travType*/)
{
  return std::auto_ptr<te::da::DataSet>(0);
}

std::auto_ptr<te::da::DataSet> te::da::DataSource::getDataSet(const std::string& name,
                                                              const te::da::ObjectIdSet* oids,
                                                              te::common::TraverseType travType)
{
  assert(!name.empty());
  assert(oids);
  assert(oids->size() > 0);

  // ObjectIds restriction
  Expression* exp = oids->getExpression();
  assert(exp);

  // Where clause
  Where* filter = new Where(exp);
  
  // All fields (?)
  te::da::Fields* all = new te::da::Fields;
  all->push_back(new te::da::Field("*"));
  
  // From the data set
  FromItem* fromItem = new DataSetName(name);
  From* from = new From;
  from->push_back(fromItem);

  // The final Select
  std::auto_ptr<Select> select(new Select(all, from, filter));

  std::auto_ptr<te::da::DataSet> result = query(select.get(), travType);

  return result;
}

std::auto_ptr<te::da::DataSet> te::da::DataSource::query(const Select& /*q*/, te::common::TraverseType /*travType*/)
{
  return std::auto_ptr<te::da::DataSet>(0);
}

std::auto_ptr<te::da::DataSet> te::da::DataSource::query(const std::string& /*query*/, te::common::TraverseType /*travType*/)
{
  return std::auto_ptr<te::da::DataSet>(0);
}

void te::da::DataSource::execute(const Query& /*command*/)
{
}

void te::da::DataSource::execute(const std::string& /*command*/)
{
}

void te::da::DataSource::cancel()
{
}

boost::int64_t te::da::DataSource::getLastGeneratedId()
{
  return 0;
}

std::string te::da::DataSource::escape(const std::string& value)
{
  return value;
}

bool te::da::DataSource::isDataSetNameValid(const std::string& /*datasetName*/)
{
  return true;
}

bool te::da::DataSource::isPropertyNameValid(const std::string& /*propertyName*/)
{
  return true;
}

void te::da::DataSource::addProperty(const std::string& /*datasetName*/, te::dt::Property* /*p*/)
{
}

void te::da::DataSource::dropProperty(const std::string& /*datasetName*/, const std::string& /*name*/)
{
}

void te::da::DataSource::renameProperty(const std::string& datasetName,
                                        const std::string& propertyName,
                                        const std::string& newPropertyName)
{
}

te::da::PrimaryKey* te::da::DataSource::getPrimaryKey(const std::string& /*datasetName*/)
{
  return 0;
}

bool te::da::DataSource::primaryKeyExists(const std::string& datasetName, const std::string& /*name*/)
{
  return false;
}

void te::da::DataSource::addPrimaryKey(const std::string& /*datasetName*/, PrimaryKey* /*pk*/)
{
}

void te::da::DataSource::dropPrimaryKey(const std::string& /*datasetName*/)
{
}

std::vector<std::string> te::da::DataSource::getForeignKeyNames(const std::string& /*datasetName*/)
{
  return std::vector<std::string>();
}

bool te::da::DataSource::foreignKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return false;
}

te::da::ForeignKey* te::da::DataSource::getForeignKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return 0;
}

void te::da::DataSource::addForeignKey(const std::string& /*datasetName*/, ForeignKey* /*fk*/)
{
}

void te::da::DataSource::dropForeignKey(const std::string& /*datasetName*/, const std::string& /*fkName*/)
{
}

std::vector<std::string> te::da::DataSource::getUniqueKeyNames(const std::string& /*datasetName*/)
{
  return std::vector<std::string>();
}

bool te::da::DataSource::uniqueKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return false;
}

te::da::UniqueKey* te::da::DataSource::getUniqueKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return 0;
}

void te::da::DataSource::addUniqueKey(const std::string& /*datasetName*/, UniqueKey* /*uk*/)
{
}

void te::da::DataSource::dropUniqueKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
}

std::vector<std::string> te::da::DataSource::getCheckConstraintNames(const std::string& /*datasetName*/)
{
  return std::vector<std::string>();
}

bool te::da::DataSource::checkConstraintExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return false;
}

te::da::CheckConstraint* te::da::DataSource::getCheckConstraint(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return 0;
}

void te::da::DataSource::addCheckConstraint(const std::string& /*datasetName*/, CheckConstraint* /*cc*/)
{
}

void te::da::DataSource::dropCheckConstraint(const std::string& /*datasetName*/, const std::string& /*name*/)
{
}

std::vector<std::string> te::da::DataSource::getIndexNames(const std::string& /*datasetName*/)
{
  return std::vector<std::string>();
}

bool te::da::DataSource::indexExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return false;
}

te::da::Index* te::da::DataSource::getIndex(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return 0;
}

void te::da::DataSource::addIndex(const std::string& /*datasetName*/, Index* /*idx*/,
                                  const std::map<std::string, std::string>& /*options*/)
{
}

void te::da::DataSource::dropIndex(const std::string& /*datasetName*/, const std::string& /*idxName*/)
{
}

std::vector<std::string> te::da::DataSource::getSequenceNames()
{
  return std::vector<std::string>();
}

bool te::da::DataSource::sequenceExists(const std::string& /*name*/)
{
  return false;
}

te::da::Sequence* te::da::DataSource::getSequence(const std::string& /*name*/)
{
  return 0;
}

void te::da::DataSource::addSequence(Sequence* /*sequence*/)
{
}

void te::da::DataSource::dropSequence(const std::string& /*name*/)
{
}

std::auto_ptr<te::gm::Envelope> te::da::DataSource::getExtent(const std::string& /*datasetName*/,
                                                              const std::string& /*propertyName*/)
{
  return std::auto_ptr<te::gm::Envelope>(0);
}

std::auto_ptr<te::gm::Envelope> te::da::DataSource::getExtent(const std::string& /*datasetName*/,
                                                              std::size_t /*propertyPos*/)
{
  return std::auto_ptr<te::gm::Envelope>(0);
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

std::vector<std::string> te::da::DataSource::getEncodings(const std::string& dsType, const std::map<std::string, std::string>& dsInfo)
{
  std::auto_ptr<DataSource> ds(DataSourceFactory::make(dsType));

  if(ds.get() == 0)
    throw Exception(TR_DATAACCESS("Could not find the appropriate factory to create a data source instance!"));

  return ds->getEncodings(dsInfo);
}

void te::da::DataSource::create(const std::map<std::string, std::string>& /*dsInfo*/)
{
}

void te::da::DataSource::drop(const std::map<std::string, std::string>& /*dsInfo*/)
{
}

bool te::da::DataSource::exists(const std::map<std::string, std::string>& /*dsInfo*/)
{
  return false;
}

std::vector<std::string> te::da::DataSource::getDataSourceNames(const std::map<std::string, std::string>& /*dsInfo*/)
{
  return std::vector<std::string>();
}

std::vector<std::string> te::da::DataSource::getEncodings(const std::map<std::string, std::string>& /*dsInfo*/)
{
  return std::vector<std::string>();
}
