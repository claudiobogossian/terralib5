/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/postgis/Transactor.cpp

  \brief Transactor class for PostGIS driver.  
*/

// TerraLib
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../dataaccess/query/Query.h"
#include "../dataaccess/query/Select.h"
#include "../dataaccess/query/SQLDialect.h"
#include "../dataaccess/query/SQLVisitor.h"
#include "CatalogLoader.h"
#include "Connection.h"
#include "ConnectionPool.h"
#include "DataSet.h"
#include "DataSetPersistence.h"
#include "DataSetTypePersistence.h"
#include "DataSource.h"
#include "DataTypes.h"
#include "Exception.h"
#include "PreparedQuery.h"
#include "SQLVisitor.h"
#include "Transactor.h"
#include "Utils.h"

// STL
#include <memory>

// libpq
#include <libpq-fe.h>

te::pgis::Transactor::Transactor(DataSource* ds, Connection* conn)
  : m_ds(ds),
    m_conn(conn),
    m_isInTransaction(false)
{
}

te::pgis::Transactor::~Transactor()
{
  m_ds->getConnPool()->release(m_conn);
}

void te::pgis::Transactor::begin()
{
  execute("BEGIN");
  m_isInTransaction = true;
}

void te::pgis::Transactor::commit()
{
  m_isInTransaction = false;
  execute("COMMIT");
}

void te::pgis::Transactor::rollBack()
{
  m_isInTransaction = false;
  execute("ROLLBACK");
}

bool te::pgis::Transactor::isInTransaction() const
{
  return m_isInTransaction;
}

te::da::DataSet* te::pgis::Transactor::getDataSet(const std::string& name, 
                                                  te::common::TraverseType /*travType*/,
                                                  te::common::AccessPolicy /*rwRole*/)
{
  std::string sql("SELECT * FROM ");
              sql += name;

  PGresult* result = m_conn->query(sql);

  return new DataSet(result, this, new std::string(sql), new std::string(name));
}

te::da::DataSet* te::pgis::Transactor::getDataSet(const std::string& name,
                                                  const te::dt::Property* p,
                                                  const te::gm::Envelope* e,
                                                  te::gm::SpatialRelation r,
                                                  te::common::TraverseType /*travType*/,
                                                  te::common::AccessPolicy /*rwRole*/)
{
  if(e == 0)
    throw Exception(TR_PGIS("The envelope is missing!"));

  const te::gm::GeometryProperty* gp = static_cast<const te::gm::GeometryProperty*>(p);

  std::string rel = GetBoxSpatialRelation(r);

  std::string sql("SELECT * FROM ");
              sql += name;
              sql += " WHERE ";
              sql += p->getName();
              sql += rel;

  Convert2PostGIS(e, gp->getSRID(), sql);

  PGresult* result = m_conn->query(sql);

  return new DataSet(result, this, new std::string(sql), new std::string(name));
}

te::da::DataSet* te::pgis::Transactor::getDataSet(const std::string& name,
                                                  const te::dt::Property* p,
                                                  const te::gm::Geometry* g,
                                                  te::gm::SpatialRelation r,
                                                  te::common::TraverseType /*travType*/, 
                                                  te::common::AccessPolicy /*rwRole*/)
{
  if(g == 0)
    throw Exception(TR_PGIS("The geometry is missing!"));

  std::string rel = GetSpatialRelation(r);

  std::string sql("SELECT * FROM ");
              sql += name;
              sql += " WHERE ";
              sql += rel;
              sql += "(";

  Convert2PostGIS(m_conn->m_pgconn, g, sql);

  sql += ",";
  sql += p->getName();
  sql += ")";

  PGresult* result = m_conn->query(sql);

  return new DataSet(result, this, new std::string(sql), new std::string(name));
}

te::da::DataSet* te::pgis::Transactor::query(const te::da::Select& q, 
                                             te::common::TraverseType travType,
                                             te::common::AccessPolicy rwRole)
{
  std::string sql;

  SQLVisitor visitor(*(m_ds->getDialect()), sql, m_conn->getConn());
  q.accept(visitor);
 
  return query(sql, travType, rwRole);
}

te::da::DataSet* te::pgis::Transactor::query(const std::string& query, 
                                             te::common::TraverseType /*travType*/,
                                             te::common::AccessPolicy /*rwRole*/)
{
  PGresult* result = m_conn->query(query);

  return new DataSet(result, this, new std::string(query));
}

void te::pgis::Transactor::execute(const te::da::Query& command)
{
  std::string sql;

  SQLVisitor visitor(*(m_ds->getDialect()), sql, m_conn->getConn());
  command.accept(visitor);
 
  execute(sql);
}

void te::pgis::Transactor::execute(const std::string& command)
{
  m_conn->execute(command);
}

te::da::PreparedQuery* te::pgis::Transactor::getPrepared(const std::string& qName)
{
  return new PreparedQuery(this, qName);
}

te::da::BatchExecutor* te::pgis::Transactor::getBatchExecutor()
{
  return 0;
}

te::da::DataSourceCatalogLoader* te::pgis::Transactor::getCatalogLoader()
{
  return new CatalogLoader(this);
}

te::da::DataSetTypePersistence* te::pgis::Transactor::getDataSetTypePersistence()
{
  return new DataSetTypePersistence(this);
}

te::da::DataSetPersistence* te::pgis::Transactor::getDataSetPersistence()
{
  return new DataSetPersistence(this);
}

void te::pgis::Transactor::cancel()
{
  return;
}

te::da::DataSource* te::pgis::Transactor::getDataSource() const
{
  return m_ds;
}

te::pgis::CatalogLoader* te::pgis::Transactor::getPGCatalogLoader()
{
  return new CatalogLoader(this);
}

te::pgis::PreparedQuery* te::pgis::Transactor::getPGPrepared(const std::string& qName)
{
  return new PreparedQuery(this, qName);
}

unsigned int te::pgis::Transactor::getGeomTypeId()
{
  std::string sql("SELECT oid FROM pg_type WHERE typname = 'geometry'");

  std::auto_ptr<te::da::DataSet> result(query(sql));

  if(result->moveNext() == false)
    return 0;

  unsigned int id = result->getInt32(0);

  return id;
}

void te::pgis::Transactor::getDatabaseInfo(std::string& currentSchema)
{
  std::string sql("SELECT current_schema()");

  std::auto_ptr<te::da::DataSet> result(query(sql));

  if(result->moveNext() == false)
    Exception(TR_PGIS("Could not get information about PostgreSQL database backend!"));

  currentSchema = result->getString(0);
}

