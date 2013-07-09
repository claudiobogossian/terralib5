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
  \file terralib/postgis/DataSource.cpp

  \brief Implementation of the data source for the PostGIS driver.
*/

// TerraLib
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess2/dataset/CheckConstraint.h"
#include "../dataaccess2/dataset/ForeignKey.h"
#include "../dataaccess2/dataset/DataSet.h"
#include "../dataaccess2/datasource/DataSourceCatalog.h"
#include "../dataaccess2/query/Select.h"
#include "../dataaccess2/query/SQLDialect.h"
#include "../dataaccess2/utils/Utils.h"
#include "../geometry/Geometry.h"
#include "Connection.h"
#include "ConnectionPool.h"
#include "DataSource.h"
#include "DataSet.h"
//#include "DataTypes.h"
#include "Exception.h"
#include "Globals.h"
#include "SQLVisitor.h"
#include "Utils.h"

// STL
#include <cassert>
//#include <cstring>
//#include <memory>

// Boost
//#include <boost/algorithm/string/case_conv.hpp>
#include <boost/format.hpp>
//#include <boost/lexical_cast.hpp>
//#include <boost/thread.hpp>

class te::pgis::DataSource::Impl
{
  public:

    Impl(te::pgis::DataSource* ds)
      : m_ds(ds),
        m_pool(0),
        m_geomTypeOid(0),
        m_rasterTypeOid(0),
        m_currentSchema(0),
        m_timeIsInteger(true),
        m_isInTransaction(false)
    {
      m_pool = new ConnectionPool(m_ds);
      m_conn = m_pool->getConnection();
    }

    ~Impl()
    {
      delete m_pool;
      delete m_currentSchema;
    }

    te::pgis::DataSource* m_ds;                       //!< The data source which this class implements.
    std::map<std::string, std::string> m_connInfo;    //!< Connection information.
    ConnectionPool* m_pool;                           //!< The connection pool.
    Connection* m_conn;                               //!< The connection to the PostGIS data source.
    unsigned int m_geomTypeOid;                       //!< PostGIS Geometry type OID.
    unsigned int m_rasterTypeOid;                     //!< PostGIS Raster type OID.
    std::string* m_currentSchema;                     //!< The default schema used when no one is provided.
    bool m_timeIsInteger;                             //!< It indicates if the postgis stores, internally, time and timestamp as an integer. 
    bool m_isInTransaction;                           //!< It indicates if there is a transaction in progress.

    static te::da::DataSourceCapabilities sm_capabilities;  //!< PostGIS capabilities.
    static te::da::SQLDialect* sm_dialect;                  //!< PostGIS SQL dialect.
};


te::da::DataSourceCapabilities te::pgis::DataSource::Impl::sm_capabilities;
te::da::SQLDialect* te::pgis::DataSource::Impl::sm_dialect(0);


te::pgis::DataSource::DataSource()
  : m_pImpl(0)
{
  m_pImpl = new Impl(this);
}

te::pgis::DataSource::~DataSource()
{
  delete m_pImpl;
}

std::string te::pgis::DataSource::getType() const throw()
{
  return PGIS_DRIVER_IDENTIFIER;
}

const std::map<std::string, std::string>& te::pgis::DataSource::getConnectionInfo() const throw()
{
  return m_pImpl->m_connInfo;
}

void te::pgis::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo) throw()
{
  m_pImpl->m_connInfo = connInfo;
}

bool te::pgis::DataSource::isTimeAnInteger() throw()
{
  return m_pImpl->m_timeIsInteger;
}

void te::pgis::DataSource::setTimeAsInteger(bool timeIsInteger) throw()
{
  m_pImpl->m_timeIsInteger = timeIsInteger;
}

void te::pgis::DataSource::open() throw(te::da::Exception)
{
  // Assure we are in a closed state
  close();

  m_pImpl->m_pool->initialize();

  // Find PostGIS types
  m_pImpl->m_geomTypeOid = getGeomTypeId();
  m_pImpl->m_rasterTypeOid = getRasterTypeId();

  if(m_pImpl->m_currentSchema == 0)
    m_pImpl->m_currentSchema = new std::string;

  // Find PostgreSQL current schema of the connection
  getDatabaseInfo(*m_pImpl->m_currentSchema);
}

void te::pgis::DataSource::close() throw(te::da::Exception)
{
  m_pImpl->m_pool->finalize();

  delete m_pImpl->m_currentSchema;
  m_pImpl->m_currentSchema = 0;
}

bool te::pgis::DataSource::isOpened() const throw()
{
  return m_pImpl->m_pool->isInitialized();
}

bool te::pgis::DataSource::isValid() const throw()
{
  return m_pImpl->m_pool->isValid();
}

const te::da::DataSourceCapabilities& te::pgis::DataSource::getCapabilities() const throw()
{
  return Impl::sm_capabilities;
}

const te::da::SQLDialect* te::pgis::DataSource::getDialect() const throw()
{
  return Impl::sm_dialect;
}

void te::pgis::DataSource::begin() throw(te::da::Exception)
{
  execute("BEGIN");
  m_pImpl->m_isInTransaction = true;
}

void te::pgis::DataSource::commit() throw(te::da::Exception)
{
  m_pImpl->m_isInTransaction = false;
  execute("COMMIT");
}

void te::pgis::DataSource::rollBack() throw(te::da::Exception)
{
  m_pImpl->m_isInTransaction = false;
  execute("ROLLBACK");
}

bool te::pgis::DataSource::isInTransaction() const throw()
{
  return m_pImpl->m_isInTransaction;
}

std::auto_ptr<te::da::DataSet> te::pgis::DataSource::getDataSet(const std::string& name,
                                                                te::common::TraverseType /*travType*/) throw(te::da::Exception)
{
   std::auto_ptr<std::string> sql(new std::string("SELECT * FROM "));
                             *sql += name;

  PGresult* result = m_pImpl->m_conn->query(*sql);

  return std::auto_ptr<te::da::DataSet>(new DataSet(result, m_pImpl->m_conn, sql.release()));
}

std::auto_ptr<te::da::DataSet> te::pgis::DataSource::getDataSet(const std::string& name,
                                                                const std::string& propertyName,
                                                                const te::gm::Envelope* e,
                                                                te::gm::SpatialRelation r,
                                                                te::common::TraverseType travType) throw(te::da::Exception)
{
  if(e == 0)
    throw Exception(TR_PGIS("The envelope is missing!"));

  std::auto_ptr<te::dt::Property> p = getProperty(name, propertyName);

  //const te::gm::GeometryProperty* gp = static_cast<const te::gm::GeometryProperty*>(p);
  const te::gm::GeometryProperty* gp = static_cast<const te::gm::GeometryProperty*>(p.release());

  std::string rel = GetBoxSpatialRelation(r);

  std::string sql("SELECT * FROM ");
              sql += name;
              sql += " WHERE ";
              sql += propertyName;
              sql += rel;

  Convert2PostGIS(e, gp->getSRID(), sql);

  PGresult* result = m_pImpl->m_conn->query(sql);

  return std::auto_ptr<te::da::DataSet>(new DataSet(result, m_pImpl->m_conn, new std::string(sql)));
}

std::auto_ptr<te::da::DataSet> te::pgis::DataSource::getDataSet(const std::string& name,
                                                                const std::string& propertyName,
                                                                const te::gm::Geometry* g,
                                                                te::gm::SpatialRelation r,
                                                                te::common::TraverseType travType) throw(te::da::Exception)
{
 if(g == 0)
    throw Exception(TR_PGIS("The geometry is missing!"));

  std::string rel = GetSpatialRelation(r);

  std::string sql("SELECT * FROM ");
              sql += name;
              sql += " WHERE ";
              sql += rel;
              sql += "(";

  Convert2PostGIS( m_pImpl->m_conn->m_pgconn, g, sql);

  sql += ",";
  sql += propertyName;
  sql += ")";

  PGresult* result = m_pImpl->m_conn->query(sql);

  return std::auto_ptr<te::da::DataSet>(new DataSet(result, m_pImpl->m_conn, new std::string(sql)));
}

std::auto_ptr<te::da::DataSet> te::pgis::DataSource::query(const te::da::Select& q,
                                                           te::common::TraverseType travType) throw(te::da::Exception)
{
  std::string sql;

  SQLVisitor visitor(*(getDialect()), sql, m_pImpl->m_conn->getConn());
  q.accept(visitor);
 
  return query(sql, travType);
}

std::auto_ptr<te::da::DataSet> te::pgis::DataSource::query(const std::string& query, 
                                                           te::common::TraverseType travType) throw(te::da::Exception)
{
  PGresult* result = m_pImpl->m_conn->query(query);

  return std::auto_ptr<te::da::DataSet>(new DataSet(result, m_pImpl->m_conn, new std::string(query)));
}

void te::pgis::DataSource::execute(const te::da::Query& command) throw(te::da::Exception)
{
  std::string sql;

  SQLVisitor visitor(*(getDialect()), sql, m_pImpl->m_conn->getConn());
  command.accept(visitor);
 
  execute(sql);
}

void te::pgis::DataSource::execute(const std::string& command) throw(te::da::Exception)
{
  m_pImpl->m_conn->execute(command);
}

void te::pgis::DataSource::cancel() throw(te::da::Exception)
{
}

boost::int64_t te::pgis::DataSource::getLastGeneratedId() throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

unsigned int te::pgis::DataSource::getGeomTypeId()
{
  std::string sql("SELECT oid FROM pg_type WHERE typname = 'geometry'");

  std::auto_ptr<te::da::DataSet> result(query(sql));

  if(result->moveNext() == false)
    return 0;

  unsigned int id = result->getInt32(0);

  return id;
}

unsigned int te::pgis::DataSource::getRasterTypeId()
{
  std::string sql("SELECT oid FROM pg_type WHERE typname = 'raster'");

  std::auto_ptr<te::da::DataSet> result(query(sql));

  if(result->moveNext() == false)
    return 0;

  unsigned int id = result->getInt32(0);

  return id;
}

const std::string* te::pgis::DataSource::getCurrentSchema()
{
  return m_pImpl->m_currentSchema;
}

void te::pgis::DataSource::getDatabaseInfo(std::string& currentSchema)
{
 std::string sql("SELECT current_schema()");

  std::auto_ptr<te::da::DataSet> result(query(sql));

  if(result->moveNext() == false)
    Exception(TR_PGIS("Could not get information about PostgreSQL database backend!"));

  currentSchema = result->getString(0);
}

std::string te::pgis::DataSource::escape(const std::string& value) throw(te::da::Exception)
{
  return value;
}

bool te::pgis::DataSource::isDataSetNameValid(const std::string& /*datasetName*/) throw(te::da::Exception)
{
  return true;
}

bool te::pgis::DataSource::isPropertyNameValid(const std::string& /*propertyName*/) throw(te::da::Exception)
{
  return true;
}

std::vector<std::string> te::pgis::DataSource::getDataSetNames() throw(te::da::Exception)
{
  // Get an auxiliary connection
  std::auto_ptr<DataSource> ds(new DataSource());

  ds->setConnectionInfo(m_pImpl->m_connInfo);

  ds->open();

  std::string sql("SELECT datname FROM pg_database");

  std::auto_ptr<te::da::DataSet> dataset(query(sql));

  std::vector<std::string> datasetNames;

  while(dataset->moveNext())
    datasetNames.push_back(dataset->getString(0));

  return datasetNames;
}

std::auto_ptr<te::da::DataSetType> te::pgis::DataSource::getDataSetType(const std::string& name) throw(te::da::Exception)
{
  // Find the table oid
  unsigned int dtid = getTableId(name);
  
  // Create the dataset type and get their properties and their associated constraints
  std::auto_ptr<te::da::DataSetType> dt(new te::da::DataSetType(name, dtid));
  dt->setTitle(name);

  getProperties(name);

  return dt;
}

std::size_t te::pgis::DataSource::getNumberOfProperties(const std::string& datasetName) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

boost::ptr_vector<te::dt::Property> te::pgis::DataSource::getProperties(const std::string& datasetName) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::auto_ptr<te::dt::Property>te::pgis::DataSource:: getProperty(const std::string& datasetName,
                                                                     const std::string& propertyName) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::auto_ptr<te::dt::Property> te::pgis::DataSource::getProperty(const std::string& datasetName,
                                                                     std::size_t propertyPos) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::auto_ptr<te::da::PrimaryKey> te::pgis::DataSource::getPrimaryKey(const std::string& /*datasetName*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::vector<std::string> te::pgis::DataSource::getUniqueKeyNames(const std::string& /*datasetName*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

boost::ptr_vector<te::da::UniqueKey> te::pgis::DataSource::getUniqueKeys(const std::string& /*datasetName*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::auto_ptr<te::da::UniqueKey> te::pgis::DataSource::getUniqueKey(const std::string& /*datasetName*/,
                                                                             const std::string& /*name*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::vector<std::string> te::pgis::DataSource::getForeignKeyNames(const std::string& /*datasetName*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::auto_ptr<te::da::ForeignKey> te::pgis::DataSource::getForeignKey(const std::string& /*datasetName*/,
                                                                               const std::string& /*name*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::vector<std::string> te::pgis::DataSource::getIndexNames(const std::string& /*datasetName*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::auto_ptr<te::da::Index> te::pgis::DataSource::getIndex(const std::string& /*datasetName*/,
                                                                     const std::string& /*name*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::vector<std::string> te::pgis::DataSource::getCheckConstraintNames(const std::string& datasetName) throw(te::da::Exception)
{
  std::vector<std::string> ccNames;

  std::auto_ptr<te::da::DataSetType> dt = getDataSetType(datasetName);

  std::auto_ptr<te::da::DataSet> ccInfo = getConstraints(dt->getId(), 'c');

  while(ccInfo->moveNext())
  {
    std::string ccName  = ccInfo->getString(2);
    ccNames.push_back(ccName);
  }

  return ccNames;
}

std::auto_ptr<te::da::CheckConstraint> te::pgis::DataSource::getCheckConstraint(const std::string& datasetName,
                                                                                const std::string& name) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::vector<std::string> te::pgis::DataSource::getSequenceNames() throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::auto_ptr<te::da::Sequence> te::pgis::DataSource::getSequence(const std::string& /*name*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::auto_ptr<te::gm::Envelope> te::pgis::DataSource::getExtent(const std::string& /*datasetName*/,
                                                                   const std::string& /*propertyName*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::auto_ptr<te::gm::Envelope> te::pgis::DataSource::getExtent(const std::string& /*datasetName*/,
                                                                   std::size_t /*propertyPos*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::size_t te::pgis::DataSource::getNumberOfItems(const std::string& /*datasetName*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

bool te::pgis::DataSource::hasDataSets() throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

bool te::pgis::DataSource::datasetExists(const std::string& /*name*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

bool te::pgis::DataSource::primarykeyExists(const std::string& /*datasetName*/,
                                               const std::string& /*name*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

bool te::pgis::DataSource::uniquekeyExists(const std::string& /*datasetName*/,
                                              const std::string& /*name*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

bool te::pgis::DataSource::foreignkeyExists(const std::string& /*datasetName*/,
                                               const std::string& /*name*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

bool te::pgis::DataSource::checkConstraintExists(const std::string& /*datasetName*/,
                                                    const std::string& /*name*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

bool te::pgis::DataSource::indexExists(const std::string& /*datasetName*/,
                                          const std::string& /*name*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

bool te::pgis::DataSource::sequenceExists(const std::string& /*name*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::createDataSet(te::da::DataSetType* dt,
                                            const std::map<std::string, std::string>& /*options*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::cloneDataSet(const std::string& /*name*/,
                                           const std::string& /*cloneName*/,
                                           const std::map<std::string, std::string>& /*options*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::dropDataSet(const std::string& /*name*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(datasetName))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% doesn't exist!")) % datasetName).str());

  //ds->remove(datasetName);

  //if(ds->getCatalog() && ds->getCatalog()->datasetExists(datasetName))
  //{
  //  te::da::DataSetTypePtr dt = ds->getCatalog()->getDataSetType(datasetName);

  //  ds->getCatalog()->remove(dt.get());
  //}
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::renameDataSet(const std::string& /*name*/,
                                            const std::string& /*newName*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% not exists!")) % dt->getName()).str());

  //std::string oldName = dt->getName();

  //if(dt->getCatalog())
  //  ds->getCatalog()->rename(dt, newName);

  //ds->rename(oldName, newName);
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::addProperty(const std::string& /*datasetName*/,
                                          const te::dt::Property* /*p*/) throw(te::da::Exception)
{
//  DataSource* ds = m_t->getMemDataSource();
//
//  DataSource::LockWrite l(ds);
//
//  if(dt->getProperty(p->getName()))
//    throw Exception((boost::format(TR_PGIS("A property with the same name (%1%) already exists!")) % p->getName()).str());
//
//  if(!ds->datasetExists(dt->getName()))
//    throw Exception((boost::format(TR_PGIS("The dataset %1% not exists!")) % dt->getName()).str()); 
//
//  dt->add(p);
//
//// adjust the dataset properties
//  DataSet* dataset = ds->getDataSet(dt->getName());
//
//  dataset->add(p->getName(), p->getType());
//
//// adjust dataset schema
//  te::da::DataSetType* dschema = ds->getDataSetType(dt->getName());
//
//  dschema->add(p->clone());
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::dropProperty(const std::string& /*datasetName*/,
                                           const std::string& /*propertyName*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //te::da::DataSetType* dt = dynamic_cast<te::da::DataSetType*>(p->getParent());

  //if(dt == 0)
  //  throw Exception((boost::format(TR_PGIS("Dataset for property %1% is unknown!")) % p->getName()).str());

  //DataSource::LockWrite l(ds);

  //DataSet* dataset = ds->getDataSet(dt->getName());

  //te::da::DataSetType* dschema = ds->getDataSetType(dt->getName());

  //std::size_t pos = te::da::GetPropertyPos(dataset, p->getName());

  //dataset->drop(pos);

  //te::dt::Property* pp = dschema->getProperty(p->getName());

  //dschema->remove(pp);

  //dt->remove(p);

  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::renameProperty(const std::string& /*datasetName*/,
                                             const std::string& /*propertyName*/,
                                             const std::string& /*newPropertyName*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //te::da::DataSetType* dt = dynamic_cast<te::da::DataSetType*>(p->getParent());

  //if(dt == 0)
  //  throw Exception((boost::format(TR_PGIS("Dataset for property %1% is unknown!")) % p->getName()).str());

  //DataSource::LockWrite l(ds);

  //te::da::DataSetType* dataset = ds->getDataSetType(p->getName());

  //dataset->getProperty(p->getName())->setName(newName);

  //p->setName(newName);

  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::addPrimaryKey(const std::string& /*datasetName*/,
                                            const te::da::PrimaryKey* /*pk*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));

  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% not exists!")) % dt->getName()).str());

  //if(ds->getDataSet(dt->getName())->getType()->getPrimaryKey())
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% already has a Primary Key!")) % dt->getName()).str());

  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //te::da::PrimaryKey* newPk = new te::da::PrimaryKey(pk->getName(), dsDt);

  //for(std::size_t i = 0; i < pk->getProperties().size(); ++i)
  //{
  //   const te::dt::Property* p = pk->getProperties()[i];

  //   te::dt::Property* dsP = dsDt->getProperty(p->getName());

  //   newPk->add(dsP);
  //}

  //dt->setPrimaryKey(pk);
}

void te::pgis::DataSource::dropPrimaryKey(const std::string& /*datasetName*/,
                                             const std::string& /*primaryKeyName*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //te::da::DataSetType* dt = pk->getDataSetType();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% not exists!")) % dt->getName()).str());

  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //if(!dsDt->getPrimaryKey())
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% has not a Primary Key!")) % dsDt->getName()).str());

  //if(dsDt->getPrimaryKey()->getName() != pk->getName())
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% has not a Primary Key named %2%!")) % dsDt->getName() % pk->getName()).str());
  //  
  //if(dsDt != dt)
  //  dsDt->setPrimaryKey(0);

  //if(ds->getCatalog()->getDataSetType(dsDt->getName()))
  //  ds->getCatalog()->getDataSetType(dsDt->getName())->setPrimaryKey(0);
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::addUniqueKey(const std::string& /*datasetName*/,
                                           const te::da::UniqueKey* /*uk*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% not exists!")) % dt->getName()).str());

  //if(ds->getDataSet(dt->getName())->getType()->getUniqueKey(uk->getName()))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% already has a Unique Key with this name (%2%)!")) % dt->getName() % uk->getName()).str());

  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //te::da::UniqueKey* newUk = new te::da::UniqueKey(uk->getName(), dsDt);

  //for(std::size_t i = 0; i < uk->getProperties().size(); ++i)
  //{
  //   const te::dt::Property* p = uk->getProperties()[i];

  //   te::dt::Property* dsP = dsDt->getProperty(p->getName());

  //   newUk->add(dsP);
  //}

  //dt->add(uk);
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::dropUniqueKey(const std::string& /*datasetName*/,
                                            const std::string& /*uniqueKeyName*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //te::da::DataSetType* dt = uk->getDataSetType();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% not exists!")) % dt->getName()).str());
  //
  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //if(!dsDt->getUniqueKey(uk->getName()))
  //  throw Exception((boost::format(TR_PGIS("The Unique Key %1% not exists!")) % uk->getName()).str());
  //  
  //if(dsDt != dt)
  //  dsDt->remove(dsDt->getUniqueKey(uk->getName()));

  //if(ds->getCatalog()->getDataSetType(dsDt->getName()))
  //  ds->getCatalog()->getDataSetType(dsDt->getName())->remove(dsDt->getUniqueKey(uk->getName()));
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::addIndex(const std::string& /*datasetName*/,
                                       const te::da::Index* /*idx*/,
                                       const std::map<std::string, std::string>& /*options*/) throw(te::da::Exception) 
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% not exists!")) % dt->getName()).str());

  //if(ds->getDataSet(dt->getName())->getType()->getIndex(index->getName()))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% already has a Index with this name (%2%)!")) % dt->getName() % index->getName()).str());

  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //te::da::Index* newIndex = new te::da::Index(index->getName(), index->getIndexType(), dsDt);

  //for(std::size_t i = 0; i < index->getProperties().size(); ++i)
  //{
  //   const te::dt::Property* p = index->getProperties()[i];

  //   te::dt::Property* dsP = dsDt->getProperty(p->getName());

  //   newIndex->add(dsP);
  //}

  //dt->add(index);
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::dropIndex(const std::string& /*datasetName*/,
                                        const std::string& /*idxName*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //te::da::DataSetType* dt = index->getDataSetType();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% not exists!")) % dt->getName()).str());
  //
  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //if(!dsDt->getIndex(index->getName()))
  //  throw Exception((boost::format(TR_PGIS("The Index %1% not exists!")) % index->getName()).str());

  //dsDt->remove(dsDt->getIndex(index->getName()));

  //if(dsDt != dt)
  //  dt->remove(index);

  //if(ds->getCatalog()->getDataSetType(dsDt->getName()))
  //  ds->getCatalog()->getDataSetType(dsDt->getName())->remove(index);
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::addForeignKey(const std::string& /*datasetName*/,
                                            const te::da::ForeignKey* /*fk*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% not exists!")) % dt->getName()).str());

  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //te::da::ForeignKey* newFk = new te::da::ForeignKey(fk->getName()); 

  //for(std::size_t i = 0; i < fk->getProperties().size(); ++i)
  //{
  //   const te::dt::Property* p = fk->getProperties()[i];

  //   te::dt::Property* dsP = dsDt->getProperty(p->getName());

  //   newFk->add(dsP);
  //}

  //for(std::size_t i = 0; i < fk->getReferencedProperties().size(); ++i)
  //{
  //   const te::dt::Property* p = fk->getProperties()[i];

  //   te::da::DataSetType* currDt = static_cast<te::da::DataSetType*>(p->getParent());

  //   if(!ds->datasetExists(currDt->getName()))
  //     throw Exception((boost::format(TR_PGIS("The dataset %1% referenced not exists!")) % currDt->getName()).str());

  //   te::da::DataSetType* currRefDt = ds->getDataSet(currDt->getName())->getType();

  //   te::dt::Property* refP = currRefDt->getProperty(fk->getReferencedProperties()[i]->getName());

  //   newFk->addRefProperty(refP);
  //}

  //dsDt->add(newFk);
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::dropForeignKey(const std::string& /*datasetName*/,
                                             const std::string& /*fkName*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //te::da::DataSetType* dt = fk->getDataSetType();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% not exists!")) % dt->getName()).str());
  //
  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //te::da::ForeignKey* dsFk = dsDt->getForeignKey(fk->getName());
  //
  //if(!dsFk)
  //  throw Exception((boost::format(TR_PGIS("The Foreign Key %1% not exists!")) % fk->getName()).str());
  //  
  //if(dsDt != dt)
  //  dsDt->remove(dsFk);

  //if(ds->getCatalog()->getDataSetType(dsDt->getName()))
  //  ds->getCatalog()->getDataSetType(dsDt->getName())->remove(dsFk);
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::addCheckConstraint(const std::string& datasetName,
                                              const te::da::CheckConstraint* cc) throw(te::da::Exception)
{
  if(!datasetExists(datasetName))
    throw Exception((boost::format(TR_PGIS("The dataset %1% doesn't exist!")) % datasetName).str());

  if(getCheckConstraint(datasetName, cc->getName()).get() != 0)
    throw Exception((boost::format(TR_PGIS("The dataset %1% already has a Check Constraint with this name (%2%)!")) % datasetName % cc->getName()).str());

  te::da::DataSetType* datasetType = getDataSetType(datasetName).get();

  te::da::CheckConstraint* newCC = new te::da::CheckConstraint(cc->getName(), datasetType);
  newCC->setExpression(cc->getExpression());
}

void te::pgis::DataSource::dropCheckConstraint(const std::string& /*datasetName*/,
                                                  const std::string& /*name*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //te::da::DataSetType* dt = cc->getDataSetType();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% not exists!")) % dt->getName()).str());
  //
  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //if(!dsDt->getCheckConstraint(cc->getName()))
  //  throw Exception((boost::format(TR_PGIS("The Check Constraint %1% not exists!")) % cc->getName()).str());  

  //if(dsDt != dt)
  //  dsDt->remove(dsDt->getCheckConstraint(cc->getName()));

  //if(ds->getCatalog()->getDataSetType(dsDt->getName()))
  //  ds->getCatalog()->getDataSetType(dsDt->getName())->remove(dsDt->getCheckConstraint(cc->getName()));
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::createSequence(const te::da::Sequence* /*sequence*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(ds->getCatalog()->getSequence(sequence->getName()))
  //  throw Exception((boost::format(TR_PGIS("The datasource already has a Sequence with this name (%1%)!")) % sequence->getName()).str());
  //   
  //te::da::Sequence* newSeq = new te::da::Sequence(*sequence);
  //newSeq->setCatalog(ds->getCatalog());

  ////getting the datasettype by the name of the sequence datasettype property involved
  //const te::da::DataSetTypePtr& dsDt = ds->getCatalog()->getDataSetType(static_cast<te::da::DataSetType*>(sequence->getOwner()->getParent())->getName());

  //newSeq->setOwner(dsDt->getProperty(sequence->getOwner()->getName()));
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::dropSequence(const std::string& /*name*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(ds->getCatalog()->getSequence(sequence->getName()))
  //  throw Exception((boost::format(TR_PGIS("The datasource has no Sequence with this name (%1%)!")) % sequence->getName()).str());

  //ds->getCatalog()->remove(ds->getCatalog()->getSequence(sequence->getName()));
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::add(const std::string& /*datasetName*/,
                                  te::da::DataSet* /*d*/,
                                  const std::map<std::string, std::string>& /*options*/,
                                  std::size_t /*limit*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //te::pgis::DataSet* idataset = ds->getDataSetRef(datasetName);

  //if(idataset == 0)
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% doesn't exist!")) % datasetName).str());

  //idataset->copy(*d, limit);
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::remove(const std::string& /*datasetName*/,
                                     const te::da::ObjectIdSet* /*oids*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //DataSet* dataset = ds->getDataSetRef(datasetName);

  //if(dataset == 0)
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% doesn't exist!")) % datasetName).str());

  //dataset->clear();
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::update(const std::string& /*datasetName*/,
                                     te::da::DataSet* /*dataset*/,
                                     const std::vector<std::size_t>& /*properties*/,
                                     const te::da::ObjectIdSet* /*oids*/,
                                     const std::map<std::string, std::string>& /*options*/,
                                     std::size_t /*limit*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

unsigned int te::pgis::DataSource::getTableId(const std::string& tableName) throw(te::da::Exception)
{
 std::string tname, sname;
  
  SplitTableName(tableName, m_pImpl->m_currentSchema, sname, tname);

  std::string sql("SELECT pg_class.oid "
                  "FROM pg_class, pg_namespace "
                  "WHERE pg_class.relnamespace = pg_namespace.oid " 
                  "AND lower(pg_class.relname) = '");

  sql += te::common::Convert2LCase(tname);
  sql += "' AND lower(pg_namespace.nspname) = '";
  sql += te::common::Convert2LCase(sname);
  sql += "'";

  std::auto_ptr<te::da::DataSet> result(query(sql));

  if(result->moveNext() == false)
    throw Exception(TR_PGIS("Could not find the table oid!"));

  unsigned int tableid = result->getInt32(0);

  return tableid;
}

std::string te::pgis::DataSource::getTableName(unsigned int id)
{
  std::string sql("SELECT pg_namespace.nspname, pg_class.relname "
                  "FROM pg_class, pg_namespace "
                  "WHERE pg_class.relnamespace = pg_namespace.oid " 
                  "AND pg_class.oid = ");

  sql += te::common::Convert2String(id);

  std::auto_ptr<te::da::DataSet> result(query(sql));

  if(result->moveNext() == false)
    throw Exception(TR_PGIS("Could not find the table name!"));

  std::string tname  = result->getString(0);
              tname += ".";
              tname += result->getString(1);

  return tname;
}

std::auto_ptr<te::da::DataSet> te::pgis::DataSource::getConstraints(unsigned int dtid, char conType)
{
  std::string sql("SELECT c.oid, n.nspname, c.conname, c.contype, c.confrelid, c.confupdtype, c.confdeltype, c.confmatchtype, c.conkey, c.confkey, pg_get_constraintdef(c.oid) "
                  "FROM pg_constraint c, pg_namespace n "
                  "WHERE c.connamespace = n.oid "
                  "AND c.conrelid = ");
              sql += te::common::Convert2String(dtid);

  if(conType != '\0')
  {
    sql += " AND c.contype = '";
    sql += conType;
    sql += "'";
  }

  return query(sql);
}

void te::pgis::DataSource::create(const std::map<std::string, std::string>& dsInfo) throw(te::da::Exception)
{
  m_pImpl->m_connInfo = dsInfo;

  open();

  close();
}

void te::pgis::DataSource::drop(const std::map<std::string, std::string>& /*dsInfo*/) throw(te::da::Exception)
{
}

bool te::pgis::DataSource::exists(const std::map<std::string, std::string>& /*dsInfo*/) throw(te::da::Exception)
{
  return false;
}

std::vector<std::string> te::pgis::DataSource::getDataSourceNames(const std::map<std::string, std::string>& /*info*/) throw(te::da::Exception)
{
  return std::vector<std::string>();
}
