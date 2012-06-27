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
  \file terralib/sqlite/DataSourceTransactor.cpp

  \brief An implementation of DataSourceTransactor class for the TerraLib SQLite Data Access Driver.
*/

// TerraLib
#include "../common/Globals.h"
#include "../common/Translator.h"
#include "../dataaccess/query/Select.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryProperty.h"
#include "BatchExecutor.h"
#include "DataSetPersistence.h"
#include "DataSetTypePersistence.h"
#include "DataSource.h"
#include "DataSourceCatalogLoader.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "EWKBSize.h"
#include "EWKBWriter.h"
#include "FwDataSet.h"
#include "PreparedQuery.h"
#include "SQLVisitor.h"
#include "Utils.h"

// Boost
#include <boost/format.hpp>

// SQLite
#include <sqlite3.h>

te::sqlite::DataSourceTransactor::DataSourceTransactor(DataSource* ds)
  : m_ds(ds),
    m_isInTransaction(false)
{
}

te::sqlite::DataSourceTransactor::~DataSourceTransactor()
{
}

void te::sqlite::DataSourceTransactor::begin()
{
  execute("BEGIN TRANSACTION");
  m_isInTransaction = true;
}

void te::sqlite::DataSourceTransactor::commit()
{
  m_isInTransaction = false;
  execute("COMMIT TRANSACTION");
}

void te::sqlite::DataSourceTransactor::rollBack()
{
  m_isInTransaction = false;  

  char* errmsg = 0;

  sqlite3_exec(m_ds->getDB(), "ROLLBACK TRANSACTION", 0, 0, &errmsg); // according to SQLite manual it is safe to ignore errors from rollback statements!
}

bool te::sqlite::DataSourceTransactor::isInTransaction() const
{
  return m_isInTransaction;
}

te::da::DataSet* te::sqlite::DataSourceTransactor::getDataSet(const std::string& name, 
                                                              te::common::TraverseType travType, 
                                                              te::common::AccessPolicy rwRole)
{
  std::string sql("SELECT * FROM ");
              sql += name;

  return query(sql, travType, rwRole);
}

te::da::DataSet* te::sqlite::DataSourceTransactor::getDataSet(const std::string& name,
                                                              const te::dt::Property* p,
                                                              const te::gm::Envelope* e,
                                                              te::gm::SpatialRelation r,
                                                              te::common::TraverseType travType, 
                                                              te::common::AccessPolicy rwRole)
{
  if(e == 0)
    throw Exception(TR_SQLITE("The envelope is missing!"));

  if(p == 0)
    throw Exception(TR_SQLITE("The property is missing!"));

// note: we need to check if table has an spatial index before using this code!
  std::string sql ("SELECT * FROM ");
              sql += name;
              sql += " WHERE ROWID IN (SELECT pkid FROM ";
              sql += "idx_" + name + "_" + p->getName();
              sql += " WHERE pkid MATCH ";
              sql += GetRtreeFilter(e, r);
              sql += ")";

  return query(sql, travType, rwRole);
}

te::da::DataSet* te::sqlite::DataSourceTransactor::getDataSet(const std::string& name,
                                                              const te::dt::Property* p,
                                                              const te::gm::Geometry* g,
                                                              te::gm::SpatialRelation r,
                                                              te::common::TraverseType /*travType*/, 
                                                              te::common::AccessPolicy /*rwRole*/)
{
  if(g == 0)
    throw Exception(TR_SQLITE("The geometry is missing!"));

  if(p == 0)
    throw Exception(TR_SQLITE("The property is missing!"));

#ifdef TE_ENABLE_SPATIALITE

// note: we need to check if table has an spatial index before using this code!
  std::string sql ("SELECT * FROM ");
              sql += name;
              sql += " WHERE ";
              sql += GetBindableSpatialRelation(p->getName(), r);
              sql += " AND ROWID IN (SELECT pkid FROM ";
              sql += "idx_" + name + "_" + p->getName();
              sql += " WHERE pkid MATCH ";
              sql += GetRtreeFilter(g->getMBR(), r);
              sql += ")";

  sqlite3_stmt* stmt = queryLite(sql);

  std::size_t ewkbSize = EWKBSize::getEWKBSize(g);

  unsigned char* ewkb = new unsigned char[ewkbSize];

  EWKBWriter::write(g, ewkb, te::common::Globals::sm_machineByteOrder);

  int retval = sqlite3_bind_blob(stmt, 1, ewkb, ewkbSize, SQLITE_TRANSIENT);

  delete [] ewkb;

  if(retval != SQLITE_OK)
  {
    sqlite3_finalize(stmt);

    throw Exception(TR_SQLITE("Could not bind geometry in the spatial filter!"));
  }

  return new FwDataSet(stmt, this);
#else
  return 0;
#endif // TE_ENABLE_SPATIALITE

}

te::da::DataSet* te::sqlite::DataSourceTransactor::query(const te::da::Select& q, 
                                                         te::common::TraverseType travType, 
                                                         te::common::AccessPolicy rwRole)
{
  std::string sql;

  SQLVisitor visitor(*(m_ds->getDialect()), sql);
  q.accept(visitor);
 
  return query(sql, travType, rwRole);
}

te::da::DataSet* te::sqlite::DataSourceTransactor::query(const std::string& query, 
                                                         te::common::TraverseType /*travType*/, 
                                                         te::common::AccessPolicy /*rwRole*/)
{
  sqlite3_stmt* result = queryLite(query);

  return new FwDataSet(result, this);
}

void te::sqlite::DataSourceTransactor::execute(const te::da::Query& /*command*/)
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
  //std::string sql;

  //SQLVisitor visitor(*(m_ds->getDialect()), sql, m_conn->getConn());
  //command.accept(visitor);
 
  //execute(sql);
}

void te::sqlite::DataSourceTransactor::execute(const std::string& command)
{
  char* errmsg = 0;

  int rc = sqlite3_exec(m_ds->getDB(), command.c_str(), 0, 0, &errmsg);

  if(rc != SQLITE_OK)
  {
    boost::format msg(TR_SQLITE("Could not execute the SQL command due to the following error: %1%."));
    
    msg = msg % errmsg;

    sqlite3_free(errmsg);

    throw Exception(msg.str());
  }
}

te::da::PreparedQuery* te::sqlite::DataSourceTransactor::getPrepared(const std::string& /*1Name*/)
{
  return new PreparedQuery(this);
}

te::da::BatchExecutor* te::sqlite::DataSourceTransactor::getBatchExecutor()
{
  return new BatchExecutor(this);
}

te::da::DataSourceCatalogLoader* te::sqlite::DataSourceTransactor::getCatalogLoader()
{
  return new DataSourceCatalogLoader(this);
}

te::da::DataSetTypePersistence* te::sqlite::DataSourceTransactor::getDataSetTypePersistence()
{
  return new DataSetTypePersistence(this);
}

te::da::DataSetPersistence* te::sqlite::DataSourceTransactor::getDataSetPersistence()
{
  return new DataSetPersistence(this);
}

void te::sqlite::DataSourceTransactor::cancel()
{
  sqlite3_interrupt(m_ds->getDB());
}

boost::int64_t te::sqlite::DataSourceTransactor::getLastInsertId()
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

te::da::DataSource* te::sqlite::DataSourceTransactor::getDataSource() const
{
  return m_ds;
}

te::sqlite::PreparedQuery* te::sqlite::DataSourceTransactor::getLitePrepared()
{
  return new PreparedQuery(this);
}

te::sqlite::BatchExecutor* te::sqlite::DataSourceTransactor::getLiteBatchExecutor()
{
  return new BatchExecutor(this);
}

sqlite3_stmt* te::sqlite::DataSourceTransactor::queryLite(const std::string& query)
{
  sqlite3_stmt* stmt = 0;

  int ret = sqlite3_prepare_v2(m_ds->getDB(), query.c_str(), -1, &stmt, 0);

  if(ret != SQLITE_OK)
  {
    if(stmt)
      sqlite3_finalize(stmt);

    throw Exception((boost::format(TR_SQLITE("Could not excute the given query due to the following error: %1%.")) % sqlite3_errmsg(m_ds->getDB())).str());
  }

  return stmt;
}

te::sqlite::DataSourceCatalogLoader* te::sqlite::DataSourceTransactor::getSQLiteCatalogLoader()
{
  return new DataSourceCatalogLoader(this);
}

