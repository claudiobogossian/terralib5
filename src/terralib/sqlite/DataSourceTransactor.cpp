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
#include "../common/Globals.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/query/Select.h"
#include "../geometry/Envelope.h"
#include "../geometry/GeometryProperty.h"
#include "../memory/DataSet.h"
#include "BatchExecutor.h"
#include "DataSource.h"
#include "DataSourceCatalogLoader.h"
#include "DataSourceTransactor.h"
#include "EWKBSize.h"
#include "EWKBWriter.h"
#include "FwDataSet.h"
#include "PreparedQuery.h"
#include "SQLVisitor.h"
#include "Utils.h"

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// SQLite
#include <sqlite3.h>

class te::sqlite::DataSourceTransactor::Impl
{
  public:

    Impl(DataSource* parent, sqlite3* db);

    sqlite3_stmt* queryLite(const std::string& query);

    DataSource* m_parent;
    sqlite3* m_db;
    bool m_isInTransaction;
};

te::sqlite::DataSourceTransactor::Impl::Impl(DataSource* parent, sqlite3* db)
  : m_parent(parent),
    m_db(db),
    m_isInTransaction(false)
{
}

sqlite3_stmt* te::sqlite::DataSourceTransactor::Impl::queryLite(const std::string& query)
{
  sqlite3_stmt* stmt = 0;

  int ret = sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, 0);

  if(ret != SQLITE_OK)
  {
    if(stmt)
      sqlite3_finalize(stmt);

    throw te::common::Exception((boost::format(TR_COMMON("Could not excute the given query due to the following error: %1%.")) % sqlite3_errmsg(m_db)).str());
  }

  return stmt;
}

te::sqlite::DataSourceTransactor::DataSourceTransactor(DataSource* parent, sqlite3* db)
  : m_pImpl(0)
{
  m_pImpl = new Impl(parent, db);
}

te::sqlite::DataSourceTransactor::~DataSourceTransactor()
{
  delete m_pImpl;
}

te::da::DataSource* te::sqlite::DataSourceTransactor::getDataSource() const
{
  return m_pImpl->m_parent;
}

void te::sqlite::DataSourceTransactor::begin()
{
  execute("BEGIN TRANSACTION");
  m_pImpl->m_isInTransaction = true;
}

void te::sqlite::DataSourceTransactor::commit()
{
  m_pImpl->m_isInTransaction = false;
  execute("COMMIT TRANSACTION");
}

void te::sqlite::DataSourceTransactor::rollBack()
{
  m_pImpl->m_isInTransaction = false;

  char* errmsg = 0;

  sqlite3_exec(m_pImpl->m_db, "ROLLBACK TRANSACTION", 0, 0, &errmsg);
}

bool te::sqlite::DataSourceTransactor::isInTransaction() const
{
  return m_pImpl->m_isInTransaction;
}

std::auto_ptr<te::da::DataSet>
te::sqlite::DataSourceTransactor::getDataSet(const std::string& name,
                                             te::common::TraverseType travType,
                                             bool connected,
                                             const te::common::AccessPolicy accessPolicy)
{
  std::string sql("SELECT * FROM ");
              sql += name;

  return query(sql, travType, connected, accessPolicy);
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
  if(e == 0)
    throw te::common::Exception(TR_COMMON("The envelope is missing!"));

  if(propertyName.empty())
    throw te::common::Exception(TR_COMMON("The property is missing!"));

// TODO: we need to check if table has an spatial index before using this code!
  std::string sql ("SELECT * FROM ");
              sql += name;
              sql += " WHERE ROWID IN (SELECT rowid FROM SpatialIndex WHERE f_table_name = '";
              sql += boost::to_lower_copy(name) + "' AND f_geometry_column = '";
              sql += boost::to_lower_copy(propertyName) + "' AND ";
              sql += "search_frame = BuildMbr(" + boost::lexical_cast<std::string>(e->m_llx);
              sql += ", " + boost::lexical_cast<std::string>(e->m_lly);
              sql += ", " + boost::lexical_cast<std::string>(e->m_urx);
              sql += ", " + boost::lexical_cast<std::string>(e->m_ury);
              sql += "))";

  return query(sql, travType, connected, accessPolicy);
}

std::auto_ptr<te::da::DataSet>
te::sqlite::DataSourceTransactor::getDataSet(const std::string& name,
                                             const std::string& propertyName,
                                             const te::gm::Geometry* g,
                                             te::gm::SpatialRelation r,
                                             te::common::TraverseType travType,
                                             bool /*connected*/,
                                             const te::common::AccessPolicy /*accessPolicy*/)
{
  if(g == 0)
    throw te::common::Exception(TR_COMMON("The geometry is missing!"));

  if(propertyName.empty())
    throw te::common::Exception(TR_COMMON("The property is missing!"));

  const te::gm::Envelope* e = g->getMBR();

// TODO: we need to check if table has an spatial index before using this code!
  std::string sql ("SELECT * FROM ");
              sql += name;
              sql += " WHERE ";
              sql += GetBindableSpatialRelation(propertyName, r);
              sql += " AND ROWID IN (SELECT rowid FROM SpatialIndex WHERE f_table_name = '";
              sql += boost::to_lower_copy(name) + "' AND f_geometry_column = '";
              sql += boost::to_lower_copy(propertyName) + "' AND ";
              sql += "search_frame = BuildMbr(" + boost::lexical_cast<std::string>(e->m_llx);
              sql += ", " + boost::lexical_cast<std::string>(e->m_lly);
              sql += ", " + boost::lexical_cast<std::string>(e->m_urx);
              sql += ", " + boost::lexical_cast<std::string>(e->m_ury);
              sql += "))";

  sqlite3_stmt* stmt = m_pImpl->queryLite(sql);

  std::size_t ewkbSize = EWKBSize::getEWKBSize(g);

  unsigned char* ewkb = new unsigned char[ewkbSize];

  EWKBWriter::write(g, ewkb, te::common::Globals::sm_machineByteOrder);

  int retval = sqlite3_bind_blob(stmt, 1, ewkb, ewkbSize, SQLITE_TRANSIENT);

  delete [] ewkb;

  if(retval != SQLITE_OK)
  {
    sqlite3_finalize(stmt);

    throw te::common::Exception(TR_COMMON("Could not bind geometry in the spatial filter!"));
  }

  std::auto_ptr<te::da::DataSet> litedataset(new FwDataSet(stmt, this));

  if(travType == te::common::FORWARDONLY)
    return litedataset;

  std::auto_ptr<te::da::DataSet> fatdataset(new te::mem::DataSet(*litedataset, true));

  return fatdataset;
}

std::auto_ptr<te::da::DataSet>
te::sqlite::DataSourceTransactor::query(const te::da::Select& q,
                                        te::common::TraverseType travType,
                                        bool connected,
                                        const te::common::AccessPolicy /*accessPolicy*/)
{
  std::string sql;

  SQLVisitor visitor(*(m_pImpl->m_parent->getDialect()), sql);
  q.accept(visitor);
 
  return query(sql, travType, connected);
}

std::auto_ptr<te::da::DataSet>
te::sqlite::DataSourceTransactor::query(const std::string& query,
                                        te::common::TraverseType travType,
                                        bool connected,
                                        const te::common::AccessPolicy /*accessPolicy*/)
{
  sqlite3_stmt* result = m_pImpl->queryLite(query);

  std::auto_ptr<te::da::DataSet> litedataset(new FwDataSet(result, this));

  if(travType == te::common::FORWARDONLY)
    return litedataset;

  std::auto_ptr<te::da::DataSet> fatdataset(new te::mem::DataSet(*litedataset, true));

  return fatdataset;
}

void te::sqlite::DataSourceTransactor::execute(const te::da::Query& command)
{
  std::string sql;

  SQLVisitor visitor(*(m_pImpl->m_parent->getDialect()), sql);
  command.accept(visitor);

  execute(sql);
}

void te::sqlite::DataSourceTransactor::execute(const std::string& command)
{
  char* errmsg = 0;

  int rc = sqlite3_exec(m_pImpl->m_db, command.c_str(), 0, 0, &errmsg);

  if(rc != SQLITE_OK)
  {
    boost::format msg(TR_COMMON("Could not execute the SQL command due to the following error: %1%."));
    
    msg = msg % errmsg;

    sqlite3_free(errmsg);

    throw te::common::Exception(msg.str());
  }
}

std::auto_ptr<te::da::PreparedQuery>
te::sqlite::DataSourceTransactor::getPrepared(const std::string& qName)
{
  return std::auto_ptr<te::da::PreparedQuery>(new PreparedQuery(this, m_pImpl->m_db));
}

std::auto_ptr<te::da::BatchExecutor> te::sqlite::DataSourceTransactor::getBatchExecutor()
{
  return std::auto_ptr<te::da::BatchExecutor>(new BatchExecutor(this));
}

void te::sqlite::DataSourceTransactor::cancel()
{
  sqlite3_interrupt(m_pImpl->m_db);
}

boost::int64_t te::sqlite::DataSourceTransactor::getLastGeneratedId()
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

std::string te::sqlite::DataSourceTransactor::escape(const std::string& value)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

bool te::sqlite::DataSourceTransactor::isDataSetNameValid(const std::string& datasetName)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

bool te::sqlite::DataSourceTransactor::isPropertyNameValid(const std::string& propertyName)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

std::vector<std::string>
te::sqlite::DataSourceTransactor::getDataSetNames()
{
  DataSourceCatalogLoader cloader(this);

  return cloader.getDataSetNames();
}

std::size_t te::sqlite::DataSourceTransactor::getNumberOfDataSets()
{
  return getDataSetNames().size();
}

std::auto_ptr<te::da::DataSetType>
te::sqlite::DataSourceTransactor::getDataSetType(const std::string& name)
{
  DataSourceCatalogLoader cloader(this);

  return std::auto_ptr<te::da::DataSetType>(cloader.getDataSetType(name));
}

boost::ptr_vector<te::dt::Property>
te::sqlite::DataSourceTransactor::getProperties(const std::string& datasetName)
{
  DataSourceCatalogLoader cloader(this);

  return cloader.getProperties(datasetName);
}

std::auto_ptr<te::dt::Property>
te::sqlite::DataSourceTransactor::getProperty(const std::string& datasetName,
                                              const std::string& name)
{
  DataSourceCatalogLoader cloader(this);

  std::auto_ptr<te::da::DataSetType> dt(cloader.getDataSetType(datasetName));

  te::dt::Property* p = dt->getProperty(name);

  return std::auto_ptr<te::dt::Property>(p->clone());
}

std::auto_ptr<te::dt::Property> te::sqlite::DataSourceTransactor::getProperty(const std::string& datasetName, std::size_t propertyPos)
{
  DataSourceCatalogLoader cloader(this);

  std::auto_ptr<te::da::DataSetType> dt(cloader.getDataSetType(datasetName));

  te::dt::Property* p = dt->getProperty(propertyPos);

  return std::auto_ptr<te::dt::Property>(p->clone());
}

std::vector<std::string> te::sqlite::DataSourceTransactor::getPropertyNames(const std::string& datasetName)
{
  DataSourceCatalogLoader cloader(this);

  std::auto_ptr<te::da::DataSetType> dt(cloader.getDataSetType(datasetName));

  std::vector<std::string> pnames;

  for(std::size_t i = 0; i != dt->size(); ++i)
    pnames.push_back(dt->getProperty(i)->getName());

  return pnames;
}

std::size_t te::sqlite::DataSourceTransactor::getNumberOfProperties(const std::string& datasetName)
{
  DataSourceCatalogLoader cloader(this);

  std::auto_ptr<te::da::DataSetType> dt(cloader.getDataSetType(datasetName));

  return dt->size();
}

bool te::sqlite::DataSourceTransactor::propertyExists(const std::string& datasetName, const std::string& name)
{
  DataSourceCatalogLoader cloader(this);

  std::auto_ptr<te::da::DataSetType> dt(cloader.getDataSetType(datasetName));

  return dt->getProperty(name) != 0;
}

void te::sqlite::DataSourceTransactor::addProperty(const std::string& datasetName, te::dt::Property* p)
{
  std::string sql;

  if(p->getType() == te::dt::GEOMETRY_TYPE)
  {
    te::gm::GeometryProperty* gp = static_cast<te::gm::GeometryProperty*>(p);

    std::string gtype;
    std::string cdim;

    Convert2SpatiaLiteGeom(gp->getGeometryType(), gtype, cdim);

    sql  = "SELECT AddGeometryColumn('";

    sql += boost::to_lower_copy(datasetName);
    sql += "', '";
    sql += boost::to_lower_copy(p->getName());
    sql += "', ";
    sql += boost::lexical_cast<std::string>(gp->getSRID());
    sql += ", '";
    sql += gtype;
    sql += "', '";
    sql += cdim;

    if(gp->isRequired())
      sql += "', 1)";
    else
      sql += "')";
  }
  else
  {
    sql = "ALTER TABLE ";
    sql += boost::to_lower_copy(datasetName);
    sql += " ADD COLUMN ";
    sql += boost::to_lower_copy(p->getName());
    sql += " ";
    sql += GetSQLType(p);
  }

  execute(sql);
}

void te::sqlite::DataSourceTransactor::dropProperty(const std::string& datasetName, const std::string& name)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

void te::sqlite::DataSourceTransactor::renameProperty(const std::string& datasetName,
                                                      const std::string& propertyName,
                                                      const std::string& newPropertyName)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

std::auto_ptr<te::da::PrimaryKey> te::sqlite::DataSourceTransactor::getPrimaryKey(const std::string& datasetName)
{
  DataSourceCatalogLoader cloader(this);

  std::auto_ptr<te::da::DataSetType> dt(cloader.getDataSetType(datasetName));

  return std::auto_ptr<te::da::PrimaryKey>(static_cast<te::da::PrimaryKey*>(dt->getPrimaryKey()->clone()));
}

bool te::sqlite::DataSourceTransactor::primaryKeyExists(const std::string& datasetName, const std::string& name)
{
  std::string sql  =  "PRAGMA table_info(";
              sql += datasetName;
              sql += ")";

  std::auto_ptr<te::da::DataSet> attributes(query(sql));

  while(attributes->moveNext())
  {
    bool isPK = attributes->getInt32(5) != 0;

    if(isPK)
      return true;
  }

  return false;
}

void te::sqlite::DataSourceTransactor::addPrimaryKey(const std::string& datasetName, te::da::PrimaryKey* pk)
{
  // TO BE CONFIRMED: SQLite doesn't support to alter a table to add a primary key. This must be done during table creation!
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

void te::sqlite::DataSourceTransactor::dropPrimaryKey(const std::string& datasetName)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

std::auto_ptr<te::da::ForeignKey> te::sqlite::DataSourceTransactor::getForeignKey(const std::string& datasetName, const std::string& name)
{
  std::string sql("PRAGMA foreign_key_list(");
              sql += datasetName;
              sql += ")";

  std::auto_ptr<te::da::DataSet> result(query(sql));

  int id = boost::lexical_cast<int>(name);

  std::auto_ptr<te::da::ForeignKey> fk(new te::da::ForeignKey(name, id));

  while(result->moveNext())
  {
    if(id != result->getInt32(0))
      continue;

    std::string onUpdate = result->getString(5);
    std::string onDeletion = result->getString(6);

    fk->setOnUpdateAction(GetAction(onUpdate));
    fk->setOnDeleteAction(GetAction(onDeletion));

    //std::string from = result->getString(3);
    //std::string to = result->getString(4);

    //te::dt::Property* fromP = dt->getProperty(from);
    //te::dt::Property* toP = dt->getProperty(to);

    //fk->add(fromP);
    //fk->addRefProperty(toP);
  }

  return fk;
}

std::vector<std::string> te::sqlite::DataSourceTransactor::getForeignKeyNames(const std::string& datasetName)
{
  std::string sql("PRAGMA foreign_key_list(");
              sql += datasetName;
              sql += ")";

  std::auto_ptr<te::da::DataSet> result(query(sql));

  int id = -1;

  std::vector<std::string> fkNames;

  while(result->moveNext())
  {
    int fkid = result->getInt32(0);

    if(fkid != id)
    {
      std::string fkname = boost::lexical_cast<std::string>(result->getInt32(0));

      fkNames.push_back(fkname);

      id = fkid;
    }
  }

  return fkNames;
}

bool te::sqlite::DataSourceTransactor::foreignKeyExists(const std::string& datasetName, const std::string& name)
{
  std::string sql("PRAGMA foreign_key_list(");
              sql += datasetName;
              sql += ")";

  std::auto_ptr<te::da::DataSet> result(query(sql));

  while(result->moveNext())
  {
    if(name == boost::lexical_cast<std::string>(result->getInt32(0)))
      return true;
  }

  return false;
}

void te::sqlite::DataSourceTransactor::addForeignKey(const std::string& datasetName, te::da::ForeignKey* fk)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

void te::sqlite::DataSourceTransactor::dropForeignKey(const std::string& datasetName, const std::string& fkName)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

std::auto_ptr<te::da::UniqueKey> te::sqlite::DataSourceTransactor::getUniqueKey(const std::string& datasetName, const std::string& name)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

std::vector<std::string> te::sqlite::DataSourceTransactor::getUniqueKeyNames(const std::string& datasetName)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

bool te::sqlite::DataSourceTransactor::uniqueKeyExists(const std::string& datasetName, const std::string& name)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

void te::sqlite::DataSourceTransactor::addUniqueKey(const std::string& datasetName, te::da::UniqueKey* uk)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

void te::sqlite::DataSourceTransactor::dropUniqueKey(const std::string& datasetName, const std::string& name)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

std::auto_ptr<te::da::CheckConstraint> te::sqlite::DataSourceTransactor::getCheckConstraint(const std::string& datasetName, const std::string& name)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

std::vector<std::string> te::sqlite::DataSourceTransactor::getCheckConstraintNames(const std::string& datasetName)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

bool te::sqlite::DataSourceTransactor::checkConstraintExists(const std::string& datasetName, const std::string& name)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

void te::sqlite::DataSourceTransactor::addCheckConstraint(const std::string& datasetName, te::da::CheckConstraint* cc)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

void te::sqlite::DataSourceTransactor::dropCheckConstraint(const std::string& datasetName, const std::string& name)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

std::auto_ptr<te::da::Index> te::sqlite::DataSourceTransactor::getIndex(const std::string& datasetName, const std::string& name)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

std::vector<std::string> te::sqlite::DataSourceTransactor::getIndexNames(const std::string& datasetName)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

bool te::sqlite::DataSourceTransactor::indexExists(const std::string& datasetName, const std::string& name)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

void te::sqlite::DataSourceTransactor::addIndex(const std::string& datasetName,
              te::da::Index* idx,
              const std::map<std::string, std::string>& options)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

void te::sqlite::DataSourceTransactor::dropIndex(const std::string& datasetName, const std::string& idxName)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

std::auto_ptr<te::da::Sequence> te::sqlite::DataSourceTransactor::getSequence(const std::string& name)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

std::vector<std::string> te::sqlite::DataSourceTransactor::getSequenceNames()
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

bool te::sqlite::DataSourceTransactor::sequenceExists(const std::string& name)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

void te::sqlite::DataSourceTransactor::addSequence(te::da::Sequence* sequence)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

void te::sqlite::DataSourceTransactor::dropSequence(const std::string& name)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

std::auto_ptr<te::gm::Envelope> te::sqlite::DataSourceTransactor::getExtent(const std::string& datasetName,
                                                                            const std::string& propertyName)
{
  DataSourceCatalogLoader cloader(this);

  return std::auto_ptr<te::gm::Envelope>(cloader.getExtent(datasetName, propertyName));
}

std::auto_ptr<te::gm::Envelope> te::sqlite::DataSourceTransactor::getExtent(const std::string& datasetName,
                                                                            std::size_t propertyPos)
{
  DataSourceCatalogLoader cloader(this);

  std::auto_ptr<te::da::DataSetType> dt(cloader.getDataSetType(datasetName));

  return std::auto_ptr<te::gm::Envelope>(cloader.getExtent(datasetName, dt->getProperty(propertyPos)->getName()));
}

std::size_t te::sqlite::DataSourceTransactor::getNumberOfItems(const std::string& datasetName)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

bool te::sqlite::DataSourceTransactor::hasDataSets()
{
  DataSourceCatalogLoader cloader(this);

  return cloader.hasDataSets();
}

bool te::sqlite::DataSourceTransactor::dataSetExists(const std::string& name)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

void te::sqlite::DataSourceTransactor::createDataSet(te::da::DataSetType* dt,
                                                     const std::map<std::string, std::string>& options)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

void te::sqlite::DataSourceTransactor::cloneDataSet(const std::string& name,
                                                    const std::string& cloneName,
                                                    const std::map<std::string, std::string>& options)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

void te::sqlite::DataSourceTransactor::dropDataSet(const std::string& name)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

void te::sqlite::DataSourceTransactor::renameDataSet(const std::string& name, const std::string& newName)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

void te::sqlite::DataSourceTransactor::add(const std::string& datasetName,
                                           te::da::DataSet* d,
                                           const std::map<std::string, std::string>& options,
                                           std::size_t limit)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

void te::sqlite::DataSourceTransactor::remove(const std::string& datasetName, const te::da::ObjectIdSet* oids)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

void te::sqlite::DataSourceTransactor::update(const std::string& datasetName,
                                              te::da::DataSet* dataset,
                                              const std::vector<std::size_t>& properties,
                                              const te::da::ObjectIdSet* oids,
                                              const std::map<std::string, std::string>& options,
                                              std::size_t limit)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}

void te::sqlite::DataSourceTransactor::optimize(const std::map<std::string, std::string>& opInfo)
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}
sqlite3* te::sqlite::DataSourceTransactor::getDB() const
{
  throw te::common::Exception(TR_COMMON("SQLITE: Not implemented yet!"));
}
