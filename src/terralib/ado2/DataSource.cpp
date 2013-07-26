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
  \file terralib/ado2/DataSource.cpp

  \brief ????
*/

// TerraLib
//#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess2/dataset/CheckConstraint.h"
#include "../dataaccess2/dataset/DataSet.h"
#include "../dataaccess2/dataset/ForeignKey.h"
#include "../dataaccess2/dataset/Index.h"
#include "../dataaccess2/dataset/PrimaryKey.h"
#include "../dataaccess2/dataset/Sequence.h"
#include "../dataaccess2/dataset/UniqueKey.h"
#include "../dataaccess2/datasource/DataSourceCatalog.h"
#include "../dataaccess2/query/Select.h"
#include "../dataaccess2/query/SQLDialect.h"
#include "../dataaccess2/utils/Utils.h"
//#include "../datatype/Array.h"
//#include "../datatype/Property.h"
//#include "../datatype/SimpleData.h"
//#include "../geometry/GeometryProperty.h"
//#include "../raster/Grid.h"
//#include "../raster/BandProperty.h"
//#include "../raster/RasterProperty.h"
//#include "../geometry/Geometry.h"
//#include "Connection.h"
#include "ConnectionPool.h"
#include "DataSource.h"
#include "DataSet.h"
//#include "DataTypes.h"
#include "Exception.h"
#include "Globals.h"
//#include "SQLVisitor.h"
#include "Utils.h"

// STL
#include <cassert>
//#include <cstring>
#include <memory>

// Boost
//#include <boost/algorithm/string/case_conv.hpp>
#include <boost/format.hpp>
//#include <boost/lexical_cast.hpp>
//#include <boost/thread.hpp>

class te::ado::DataSource::Impl
{
public:

  Impl(te::ado::DataSource* ds)
    : m_ds(ds),
    m_catalog(0),
    m_pool(0),
    m_conn(0),
    m_currentSchema(0),
    m_isInTransaction(false)
  {
    m_catalog = new te::da::DataSourceCatalog;
    m_catalog->setDataSource(m_ds);
    m_pool = new ConnectionPool(m_ds);
  }

  ~Impl()
  {
    delete m_catalog;
    delete m_pool;
    delete m_currentSchema;
  }

  te::ado::DataSource* m_ds;                       //!< The data source which this class implements.
  te::da::DataSourceCatalog* m_catalog;             //!< The main system catalog.
  std::map<std::string, std::string> m_connInfo;    //!< Connection information.
  ConnectionPool* m_pool;                           //!< The connection pool.
  Connection* m_conn;                               //!< The connection to the ADO data source.
  std::string* m_currentSchema;                     //!< The default schema used when no one is provided.
  bool m_timeIsInteger;                             //!< It indicates if the ADO stores, internally, time and timestamp as an integer. 
  bool m_isInTransaction;                           //!< It indicates if there is a transaction in progress.
  std::vector<std::string> m_datasetNames;          //!< The list of dataset names of the data source.

  static te::da::DataSourceCapabilities sm_capabilities;  //!< ADO capabilities.
  static te::da::SQLDialect* sm_dialect;                  //!< ADO SQL dialect.
};


te::da::DataSourceCapabilities te::ado::DataSource::Impl::sm_capabilities;
te::da::SQLDialect* te::ado::DataSource::Impl::sm_dialect(0);

te::ado::DataSource::DataSource()
  : m_pImpl(0)
{
  m_pImpl = new Impl(this);
}

te::ado::DataSource::~DataSource()
{
  delete m_pImpl;
}

std::string te::ado::DataSource::getType() const
{
  return ADO_DRIVER_IDENTIFIER;
}

const std::map<std::string, std::string>& te::ado::DataSource::getConnectionInfo() const
{
  return m_pImpl->m_connInfo;
}

void te::ado::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_pImpl->m_connInfo = connInfo;
}

bool te::ado::DataSource::isTimeAnInteger()
{
  return m_pImpl->m_timeIsInteger;
}

void te::ado::DataSource::setTimeAsInteger(bool timeIsInteger)
{
  m_pImpl->m_timeIsInteger = timeIsInteger;
}

void te::ado::DataSource::open()
{
  // Assure we are in a closed state
  close();

  m_pImpl->m_pool->initialize();
  m_pImpl->m_conn = m_pImpl->m_pool->getConnection();

  if(m_pImpl->m_currentSchema == 0)
    m_pImpl->m_currentSchema = new std::string;

  // Find ADO current schema of the connection
  getDatabaseInfo(*m_pImpl->m_currentSchema);

  // Get the dataset names of the data source
  getDataSetNames();
}

void te::ado::DataSource::close()
{
  if(m_pImpl->m_conn)
    m_pImpl->m_pool->release(m_pImpl->m_conn);

  m_pImpl->m_pool->finalize();

  delete m_pImpl->m_currentSchema;
  m_pImpl->m_currentSchema = 0;
}

bool te::ado::DataSource::isOpened() const
{
  return m_pImpl->m_pool->isInitialized();
}

bool te::ado::DataSource::isValid() const
{
  return m_pImpl->m_pool->isValid();
}

const te::da::DataSourceCapabilities& te::ado::DataSource::getCapabilities() const
{
  return Impl::sm_capabilities;
}

const te::da::SQLDialect* te::ado::DataSource::getDialect() const
{
  return Impl::sm_dialect;
}

void te::ado::DataSource::begin()
{
  try
  {
    m_conn->BeginTrans();
    m_pImpl->m_isInTransaction = true;
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::DataSource::commit()
{
  try
  {
    m_conn->CommitTrans();
    m_pImpl->m_isInTransaction = false;
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::DataSource::rollBack()
{
  try
  {
    m_conn->RollbackTrans();
    m_pImpl->m_isInTransaction = false;
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

bool te::ado::DataSource::isInTransaction() const
{
  return m_pImpl->m_isInTransaction;
}

std::auto_ptr<te::da::DataSet> te::ado::DataSource::getDataSet(const std::string& name, 
                                                               te::common::TraverseType travType)
{
  _RecordsetPtr recset;
  TESTHR(recset.CreateInstance(__uuidof(Recordset)));

  try
  {
    TESTHR(recset->Open(_bstr_t(name.c_str()),
      variant_t((IDispatch*)m_conn, true), adOpenKeyset, adLockOptimistic, adCmdTable));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  return new DataSet(dt, recset, this);
}

std::auto_ptr<te::da::DataSet> te::ado::DataSource::getDataSet(const std::string& name,
                                                               const std::string& propertyName,
                                                               const te::gm::Envelope* e,
                                                               te::gm::SpatialRelation r,
                                                               te::common::TraverseType travType)
{
  std::string lowerX = "lower_x";
  std::string upperX = "upper_x";
  std::string lowerY = "lower_y";
  std::string upperY = "upper_y";

  std::string query = "SELECT * FROM " + dt->getName() + " WHERE ";
  query += "NOT("+ lowerX +" > " + boost::lexical_cast<std::string>(e->m_urx) + " OR ";
  query += upperX +" < " + boost::lexical_cast<std::string>(e->m_llx) + " OR ";
  query += lowerY +" > " + boost::lexical_cast<std::string>(e->m_ury) + " OR ";
  query += upperY +" < " + boost::lexical_cast<std::string>(e->m_lly) + ")";

  _RecordsetPtr recset;
  TESTHR(recset.CreateInstance(__uuidof(Recordset)));

  try
  {
    recset->Open(query.c_str(), _variant_t((IDispatch *)m_conn), adOpenStatic, adLockReadOnly, adCmdText);
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  return new DataSet(dt, recset, this);
}

std::auto_ptr<te::da::DataSet> te::ado::DataSource::getDataSet(const std::string& name,
                                                               const std::string& propertyName,
                                                               const te::gm::Geometry* g,
                                                               te::gm::SpatialRelation r,
                                                               te::common::TraverseType travType)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::auto_ptr<te::da::DataSet> te::ado::DataSource::query(const te::da::Select& q,
                                                          te::common::TraverseType travType)
{
  std::string sql;

  SQLVisitor visitor(*(getDialect()), sql, m_pImpl->m_conn->getConn());
  q.accept(visitor);

  return query(sql, travType);
}

std::auto_ptr<te::da::DataSet> te::ado::DataSource::query(const std::string& query, 
                                                          te::common::TraverseType travType)
{
  _RecordsetPtr* result = m_pImpl->m_conn->query(query);

  return std::auto_ptr<te::da::DataSet>(new DataSet(result, m_pImpl->m_conn, new std::string(query)));
}

void te::ado::DataSource::execute(const te::da::Query& command)
{
  std::string sql;

  SQLVisitor visitor(*(getDialect()), sql, m_pImpl->m_conn->getConn());
  command.accept(visitor);

  execute(sql);
}

void te::ado::DataSource::execute(const std::string& command)
{
  m_pImpl->m_conn->execute(command);
}

void te::ado::DataSource::cancel()
{
}

boost::int64_t te::ado::DataSource::getLastGeneratedId()
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::string te::ado::DataSource::escape(const std::string& value)
{
  return value;
}

bool te::ado::DataSource::isDataSetNameValid(const std::string& /*datasetName*/)
{
  return true;
}

bool te::ado::DataSource::isPropertyNameValid(const std::string& /*propertyName*/)
{
  return true;
}

std::vector<std::string> te::ado::DataSource::getDataSetNames()
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

const te::da::DataSetTypePtr& te::ado::DataSource::getDataSetType(const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::vector<std::string> te::ado::DataSource::getPropertyNames(const std::string& datasetName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::size_t te::ado::DataSource::getNumberOfProperties(const std::string& datasetName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSource::propertyExists(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

boost::ptr_vector<te::dt::Property> te::ado::DataSource::getProperties(const std::string& datasetName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::dt::Property* te::ado::DataSource::getProperty(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::dt::Property* te::ado::DataSource::getProperty(const std::string& datasetName, std::size_t propertyPos)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::addProperty(const std::string& datasetName, te::dt::Property* p)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::dropProperty(const std::string& datasetName, const std::string& propertyName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::renameProperty(const std::string& datasetName, const std::string& propertyName, const std::string& newPropertyName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::PrimaryKey* te::ado::DataSource::getPrimaryKey(const std::string& datasetName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSource::primaryKeyExists(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::addPrimaryKey(const std::string& datasetName, te::da::PrimaryKey* pk)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::dropPrimaryKey(const std::string& datasetName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::vector<std::string> te::ado::DataSource::getForeignKeyNames(const std::string& datasetName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSource::foreignKeyExists(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::ForeignKey* te::ado::DataSource::getForeignKey(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::addForeignKey(const std::string& datasetName, te::da::ForeignKey* fk)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::dropForeignKey(const std::string& datasetName, const std::string& fkName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::vector<std::string> te::ado::DataSource::getUniqueKeyNames(const std::string& datasetName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSource::uniqueKeyExists(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::UniqueKey* te::ado::DataSource::getUniqueKey(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::addUniqueKey(const std::string& datasetName, te::da::UniqueKey* uk)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::dropUniqueKey(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::vector<std::string> te::ado::DataSource::getCheckConstraintNames(const std::string& datasetName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSource::checkConstraintExists(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::CheckConstraint* te::ado::DataSource::getCheckConstraint(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::addCheckConstraint(const std::string& datasetName, te::da::CheckConstraint* cc)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::dropCheckConstraint(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::vector<std::string> te::ado::DataSource::getIndexNames(const std::string& datasetName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSource::indexExists(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::Index* te::ado::DataSource::getIndex(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::addIndex(const std::string& datasetName, te::da::Index* idx, const std::map<std::string, std::string>& options)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::dropIndex(const std::string& datasetName, const std::string& idxName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::vector<std::string> te::ado::DataSource::getSequenceNames()
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSource::sequenceExists(const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::Sequence* te::ado::DataSource::getSequence(const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::addSequence(te::da::Sequence* sequence)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::dropSequence(const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::auto_ptr<te::gm::Envelope> te::ado::DataSource::getExtent(const std::string& datasetName, const std::string& propertyName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::auto_ptr<te::gm::Envelope> te::ado::DataSource::getExtent(const std::string& datasetName, std::size_t propertyPos)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::size_t te::ado::DataSource::getNumberOfItems(const std::string& datasetName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSource::hasDataSets()
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSource::dataSetExists(const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::createDataSet(te::da::DataSetType* dt, const std::map<std::string, std::string>& options)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::cloneDataSet(const std::string& name, const std::string& cloneName,
                                       const std::map<std::string, std::string>& options)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::dropDataSet(const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::renameDataSet(const std::string& name, const std::string& newName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::add(const std::string& datasetName, te::da::DataSet* d,
                              const std::map<std::string, std::string>& options, std::size_t limit)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::remove(const std::string& datasetName, const te::da::ObjectIdSet* oids)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::update(const std::string& datasetName,
                                 te::da::DataSet* dataset,
                                 const std::vector<std::size_t>& properties,
                                 const te::da::ObjectIdSet* oids,
                                 const std::map<std::string, std::string>& options,
                                 std::size_t limit)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::optimize(const std::map<std::string, std::string>& opInfo)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::vector<std::string> te::ado::DataSource::getDataSourceNames(const std::map<std::string, std::string>& info)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::vector<std::string> te::ado::DataSource::getEncodings(const std::map<std::string, std::string>& info)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::create(const std::map<std::string, std::string>& dsInfo)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::drop(const std::map<std::string, std::string>& dsInfo)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSource::exists(const std::map<std::string, std::string>& dsInfo)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}