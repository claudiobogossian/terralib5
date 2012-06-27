/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/mysql/DataSourceTransactor.cpp

  \brief An implementation of DataSourceTransactor class for MySQL data access driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/query/Select.h"
#include "../datatype/Property.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"
#include "BatchExecutor.h"
#include "Connection.h"
#include "ConnectionPool.h"
#include "DataSet.h"
#include "DataSetPersistence.h"
#include "DataSetTypePersistence.h"
#include "DataSource.h"
#include "DataSourceCatalogLoader.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "PreparedQuery.h"
#include "Raster.h"
#include "SQLVisitor.h"
#include "Utils.h"

// STL
#include <memory>

// Boost
#include <boost/format.hpp>

// MySQL
#include <driver/mysql_public_iface.h>

te::mysql::DataSourceTransactor::DataSourceTransactor(DataSource* parent, Connection* conn)
  : m_ds(parent),
    m_conn(conn),
    m_isInTransaction(false)
{
}

te::mysql::DataSourceTransactor::~DataSourceTransactor()
{
  m_ds->getPool()->release(m_conn);
}

void te::mysql::DataSourceTransactor::begin()
{
  execute("START TRANSACTION");
  m_isInTransaction = true;
}

void te::mysql::DataSourceTransactor::commit()
{
  m_isInTransaction = false;
  execute("COMMIT");
}

void te::mysql::DataSourceTransactor::rollBack()
{
  m_isInTransaction = false;
  execute("ROLLBACK");
}

bool te::mysql::DataSourceTransactor::isInTransaction() const
{
  return m_isInTransaction;
}

te::da::DataSet* te::mysql::DataSourceTransactor::getDataSet(const std::string& name, 
                                                             te::common::TraverseType travType, 
                                                             te::common::AccessPolicy rwRole)
{
  if(m_ds->isRaster(name))
    return getRasterDataSet(name);

  std::string sql("SELECT * FROM ");
              sql += name;

  return query(sql, travType, rwRole);
}

te::da::DataSet* te::mysql::DataSourceTransactor::getDataSet(const std::string& name,
                                                             const te::dt::Property* p,
                                                             const te::gm::Envelope* e,
                                                             te::gm::SpatialRelation r,
                                                             te::common::TraverseType travType, 
                                                             te::common::AccessPolicy rwRole)
{
  if(e == 0)
    throw Exception(TR_MYSQL("The parameter envelope is missing!"));

  if(p == 0)
    throw Exception(TR_MYSQL("The parameter property is missing!"));

  if(m_ds->isRaster(name))
    return getRasterDataSet(name);

  std::string sql ("SELECT * FROM ");
              sql += name;
              sql += " WHERE ";

  GetFilter(p->getName(), e, r, sql);

  return query(sql, travType, rwRole);
}

te::da::DataSet* te::mysql::DataSourceTransactor::getDataSet(const std::string& name,
                                                             const te::dt::Property* p,
                                                             const te::gm::Geometry* g,
                                                             te::gm::SpatialRelation r,
                                                             te::common::TraverseType travType, 
                                                             te::common::AccessPolicy rwRole)
{
  if(g == 0)
    throw Exception(TR_MYSQL("The parameter geometry is missing!"));

  if(p == 0)
    throw Exception(TR_MYSQL("The parameter property is missing!"));

  if(m_ds->isRaster(name))
    return getRasterDataSet(name);

// note: we need to check if table has an spatial index before using this code!
  std::string sql ("SELECT * FROM ");
              sql += name;
              sql += " WHERE ";

  GetFilter(p->getName(), g, r, sql);

  return query(sql, travType, rwRole);
}

te::da::DataSet* te::mysql::DataSourceTransactor::query(const te::da::Select& q, 
                                                        te::common::TraverseType travType, 
                                                        te::common::AccessPolicy rwRole)
{
  std::string sql;

  SQLVisitor visitor(*(m_ds->getDialect()), sql);
  q.accept(visitor);
 
  return query(sql, travType, rwRole);
}

te::da::DataSet* te::mysql::DataSourceTransactor::query(const std::string& query, 
                                                        te::common::TraverseType /*travType*/, 
                                                        te::common::AccessPolicy /*rwRole*/)
{
  try
  {
    std::auto_ptr<sql::Statement> stmt(m_conn->getConn()->createStatement());

    sql::ResultSet* result = stmt->executeQuery(query);

    return new DataSet(this, result, new std::string(query));
  }
  catch(const sql::SQLException& e)
  {
    throw Exception((boost::format(TR_MYSQL("Could not execute query due to the following error: %1%")) % e.what()).str());
  }
}

void te::mysql::DataSourceTransactor::execute(const te::da::Query& command)
{
  std::string sql;

  SQLVisitor visitor(*(m_ds->getDialect()), sql);
  command.accept(visitor);
 
  execute(sql);
}

void te::mysql::DataSourceTransactor::execute(const std::string& command)
{
  try
  {
    std::auto_ptr<sql::Statement> stmt(m_conn->getConn()->createStatement());

    stmt->execute(command);
  }
  catch(const sql::SQLException &e)
  {
    throw Exception((boost::format(TR_MYSQL("Could not execute the SQL command due to the following error: %1%")) % e.what()).str());
  }
}

te::da::PreparedQuery* te::mysql::DataSourceTransactor::getPrepared(const std::string& /*qName*/)
{
  return new PreparedQuery(this);
}

te::da::BatchExecutor* te::mysql::DataSourceTransactor::getBatchExecutor()
{
  return new BatchExecutor(this);
}

te::da::DataSourceCatalogLoader* te::mysql::DataSourceTransactor::getCatalogLoader()
{
  return new DataSourceCatalogLoader(this);
}

te::da::DataSetTypePersistence* te::mysql::DataSourceTransactor::getDataSetTypePersistence()
{
  return new DataSetTypePersistence(this);
}

te::da::DataSetPersistence* te::mysql::DataSourceTransactor::getDataSetPersistence()
{
  return new DataSetPersistence(this);
}

void te::mysql::DataSourceTransactor::cancel()
{
  execute("KILL QUERY");
}

boost::int64_t te::mysql::DataSourceTransactor::getLastInsertId()
{
  std::auto_ptr<te::da::DataSet> lastid(query("SELECT LAST_INSERT_ID()"));

  if(!lastid->moveNext())
    throw Exception(TR_MYSQL("Could not retrieve last insert generated id!"));

  return lastid->getInt64(0);
}

te::da::DataSource* te::mysql::DataSourceTransactor::getDataSource() const
{
  return m_ds;
}

te::mysql::Connection* te::mysql::DataSourceTransactor::getConnection() const
{
  return m_conn;
}

te::mysql::DataSourceCatalogLoader* te::mysql::DataSourceTransactor::getMyCatalogLoader()
{
  return new DataSourceCatalogLoader(this);
}

sql::ResultSet* te::mysql::DataSourceTransactor::myQuery(const std::string& query)
{
  try
  {
    std::auto_ptr<sql::Statement> stmt(m_conn->getConn()->createStatement());

    sql::ResultSet* result = stmt->executeQuery(query);

    return result;
  }
  catch(const sql::SQLException &e)
  {
    throw Exception((boost::format(TR_MYSQL("Could not execute the SQL command due to the following error: %1%")) % e.what()).str());
  }
}

te::da::DataSet* te::mysql::DataSourceTransactor::getRasterDataSet(const std::string& name)
{
  std::auto_ptr<DataSourceCatalogLoader> cloader(getMyCatalogLoader());
  std::auto_ptr<te::da::DataSetType> dt(cloader->getRasterDataSet(name));
  std::auto_ptr<te::mem::DataSet> dataset(new te::mem::DataSet(dt.release()));
  std::auto_ptr<te::mem::DataSetItem> item(new te::mem::DataSetItem(dataset.get()));
  std::auto_ptr<Raster> raster(new Raster(this, dt.get()));
  item->setValue(0, raster.release());
  return dataset.release();
}

te::mysql::DataSource* te::mysql::DataSourceTransactor::getMyDataSource() const
{
  return m_ds;
}