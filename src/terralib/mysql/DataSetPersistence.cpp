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
  \file terralib/mysql/DataSetPersistence.cpp

  \brief DataSetPersistence implementation for MySQL data access driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../dataaccess/datasource/ScopedTransaction.h"
#include "../datatype/Property.h"
#include "../datatype/Utils.h"
#include "Connection.h"
#include "DataSetPersistence.h"
#include "DataSetTypePersistence.h"
#include "DataSourceCatalogLoader.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "PreparedQuery.h"
#include "ScopedLock.h"
#include "Utils.h"

// STL
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>

// Boost
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// MySQL
#include <driver/mysql_public_iface.h>

te::mysql::DataSetPersistence::DataSetPersistence(DataSourceTransactor* parent)
  : m_t(parent)
{
}

te::mysql::DataSetPersistence::~DataSetPersistence()
{
}

void te::mysql::DataSetPersistence::create(te::da::DataSetType* dt, te::da::DataSet* d, const std::map<std::string, std::string>& options, std::size_t limit)
{
  std::auto_ptr<te::da::DataSetTypePersistence> dtp(m_t->getDataSetTypePersistence());

  dtp->create(dt, options);
  
  add(dt->getName(), d, options, limit);
}

void te::mysql::DataSetPersistence::remove(const std::string& datasetName)
{
  std::string sql("DELETE * FROM ");
              sql += datasetName;

  m_t->execute(sql);
}

void te::mysql::DataSetPersistence::remove(const std::string& /*datasetName*/, const te::da::ObjectIdSet* /*oids*/)
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

void te::mysql::DataSetPersistence::add(const std::string& /*datasetName*/, te::da::DataSet* /*d*/, const std::map<std::string, std::string>& /*options*/, std::size_t /*limit*/)
{
  throw Exception(TR_MYSQL("Not implemented yet!"));

  /*std::map<std::string, std::string>::const_iterator it = options.find("ADD_TYPE");

  if(it == options.end())
  {
    add8(dt, d);
    return;
  }

  if(it->second == "T1")
    add1(dt, d);
  else if(it->second == "T2")
    add2(dt, d);
  else if(it->second == "T3")
    add3(dt, d);
  else if(it->second == "T4")
    add4(dt, d);
  else if(it->second == "T5")
    add5(dt, d);
  else if(it->second == "T6")
    add6(dt, d);
  else if(it->second == "T7")
    add7(dt, d);
  else if(it->second == "T8")
    add8(dt, d);
  else if(it->second == "T9")
    add9(dt, d);
  else// if(it->second == "T10")
    add10(dt, d);
}

void te::mysql::DataSetPersistence::add(const te::da::DataSetType* dt, te::da::DataSetItem* item)
{
// create a prepared statement
  std::string sql  = "INSERT INTO ";
              sql += dt->getName();
              sql += GetSQLValueNames(dt);
              sql += " VALUES";
              sql += GetSQLBindValues(dt);

  std::auto_ptr<PreparedQuery> pq(static_cast<PreparedQuery*>(m_t->getPrepared()));

  pq->prepare(sql, dt->getProperties());
  pq->bind(dt, item);
  pq->execute();*/
}

void te::mysql::DataSetPersistence::update(const std::string& /*datasetName*/,
                                          te::da::DataSet* /*dataset*/,
                                          const std::vector<std::size_t>& /*properties*/,
                                          const te::da::ObjectIdSet* /*oids*/,
                                          const std::map<std::string, std::string>& /*options*/,
                                          std::size_t /*limit*/)
{
  throw Exception(TR_MYSQL("Not implemented yet!"));

  //std::map<std::string, std::string>::const_iterator it = options.find("ADD_TYPE");

  //if(it == options.end())
  //{
  //  update2(dt, dataset, properties);
  //  return;
  //}

  //if(it->second == "T4")
  //  update1(dt, dataset, properties);
  //else //if(it->second == "T8")
  //  update2(dt, dataset, properties);
}

te::da::DataSourceTransactor* te::mysql::DataSetPersistence::getTransactor() const
{
  return m_t;
}

// insere 1-linha nao preparada por item
void te::mysql::DataSetPersistence::add1(const te::da::DataSetType* dt, te::da::DataSet* d)
{
  sql::mysql::MySQL_Connection* conn = static_cast<sql::mysql::MySQL_Connection*>(m_t->getConnection()->getConn());

  try
  {
    do
    {
      std::string sql  = "INSERT INTO ";
                  sql += dt->getName();
                  sql += GetSQLValueNames(dt);
                  sql += " VALUES";
                  sql += GetSQLValues(dt, d, conn);

      m_t->execute(sql);

    }while(d->moveNext());
  }
  catch(const sql::SQLException &e)
  {
    throw Exception((boost::format(TR_MYSQL("Could not add dataset to the table due to the following error: %1%")) % e.what()).str());
  }
  catch(...)
  {
    throw Exception(TR_MYSQL("Unknown exception when adding a dataset to a table!"));
  }
}

 //usa transacao + insere 1-linha nao preparada por item
void te::mysql::DataSetPersistence::add2(const te::da::DataSetType* dt, te::da::DataSet* d)
{
// find out the database engine used by the table in order to lock or start a transaction
  std::auto_ptr<DataSourceCatalogLoader> cloader(m_t->getMyCatalogLoader());

  std::string tengine = cloader->getTableEngine(dt->getName());

  std::auto_ptr<te::da::ScopedTransaction> st;
  std::auto_ptr<ScopedLock> sl;

  if(tengine == "MyISAM")
    sl.reset(new ScopedLock(*m_t, dt->getName()));
  else if(tengine == "InnoDB")
    st.reset(new te::da::ScopedTransaction(*m_t));

  sql::mysql::MySQL_Connection* conn = static_cast<sql::mysql::MySQL_Connection*>(m_t->getConnection()->getConn());

  try
  {
    do
    {
      std::string sql  = "INSERT INTO ";
                  sql += dt->getName();
                  sql += GetSQLValueNames(dt);
                  sql += " VALUES";
                  sql += GetSQLValues(dt, d, conn);

      m_t->execute(sql);

    }while(d->moveNext());

    if(st.get())
      st->commit();
  }
  catch(const sql::SQLException &e)
  {
    throw Exception((boost::format(TR_MYSQL("Could not add dataset to the table due to the following error: %1%")) % e.what()).str());
  }
  catch(...)
  {
    throw Exception(TR_MYSQL("Unknown exception when adding a dataset to a table!"));
  }
}

// insere 1-linha preparada por item
void te::mysql::DataSetPersistence::add3(const te::da::DataSetType* dt, te::da::DataSet* d)
{
// create a prepared statement
  std::string sql  = "INSERT INTO ";
              sql += dt->getName();
              sql += GetSQLValueNames(dt);
              sql += " VALUES";
              sql += GetSQLBindValues(dt);

  std::auto_ptr<PreparedQuery> pq(static_cast<PreparedQuery*>(m_t->getPrepared()));
  pq->prepare(sql, dt->getProperties());

  try
  {
    do
    {
      pq->bind(dt, d);
      pq->execute();

    }while(d->moveNext());
  }
  catch(const sql::SQLException &e)
  {
    throw Exception((boost::format(TR_MYSQL("Could not add dataset to the table due to the following error: %1%")) % e.what()).str());
  }
  catch(...)
  {
    throw Exception(TR_MYSQL("Unknown exception when adding a dataset to a table!"));
  }
}

// usa transacao + insere 1-linha preparada por item
void te::mysql::DataSetPersistence::add4(const te::da::DataSetType* dt, te::da::DataSet* d)
{
// find out the database engine used by the table in order to lock or start a transaction
  std::auto_ptr<DataSourceCatalogLoader> cloader(m_t->getMyCatalogLoader());

  std::string tengine = cloader->getTableEngine(dt->getName());

  std::auto_ptr<te::da::ScopedTransaction> st;
  std::auto_ptr<ScopedLock> sl;

  if(tengine == "MyISAM")
    sl.reset(new ScopedLock(*m_t, dt->getName()));
  else if(tengine == "InnoDB")
    st.reset(new te::da::ScopedTransaction(*m_t));

// create a prepared statement
  std::string sql  = "INSERT INTO ";
              sql += dt->getName();
              sql += GetSQLValueNames(dt);
              sql += " VALUES";
              sql += GetSQLBindValues(dt);

  std::auto_ptr<PreparedQuery> pq(static_cast<PreparedQuery*>(m_t->getPrepared()));
  pq->prepare(sql, dt->getProperties());

  try
  {
    do
    {
      pq->bind(dt, d);
      pq->execute();

    }while(d->moveNext());

    if(st.get())
      st->commit();
      
  }
  catch(const sql::SQLException &e)
  {
    throw Exception((boost::format(TR_MYSQL("Could not add dataset to the table due to the following error: %1%")) % e.what()).str());
  }
  catch(...)
  {
    throw Exception(TR_MYSQL("Unknown exception when adding a dataset to a table!"));
  }
}

// insere usando multiplas linhas
void te::mysql::DataSetPersistence::add5(const te::da::DataSetType* dt, te::da::DataSet* d)
{
  int ncommands = 100;

  int i = 0;
  
  std::auto_ptr<sql::Statement> stmt(m_t->getConnection()->getConn()->createStatement());

  sql::mysql::MySQL_Connection* conn = static_cast<sql::mysql::MySQL_Connection*>(m_t->getConnection()->getConn());

  try
  {
    std::string sql;

    do
    {
      sql += "INSERT INTO ";
      sql += dt->getName();
      sql += GetSQLValueNames(dt);
      sql += " VALUES";
      sql += GetSQLValues(dt, d, conn);
      sql += ";";

      ++i;

      if(i % ncommands == 0)
      {
        stmt->execute(sql);

        while(stmt->getMoreResults())
          std::auto_ptr<sql::ResultSet> res(stmt->getResultSet());

        i = 0;
        sql = "";
      }
    }while(d->moveNext());

    if(i != 0)
    {
      stmt->execute(sql);

      while(stmt->getMoreResults())
        std::auto_ptr<sql::ResultSet> res(stmt->getResultSet());
    }
  }
  catch(const sql::SQLException &e)
  {
    throw Exception((boost::format(TR_MYSQL("Could not add dataset to the table due to the following error: %1%")) % e.what()).str());
  }
  catch(...)
  {
    throw Exception(TR_MYSQL("Unknown exception when adding a dataset to a table!"));
  }
}

// usa transacao + insere usando multiplas linhas
void te::mysql::DataSetPersistence::add6(const te::da::DataSetType* dt, te::da::DataSet* d)
{
// find out the database engine used by the table in order to lock or start a transaction
  std::auto_ptr<DataSourceCatalogLoader> cloader(m_t->getMyCatalogLoader());

  std::string tengine = cloader->getTableEngine(dt->getName());

  std::auto_ptr<te::da::ScopedTransaction> st;
  std::auto_ptr<ScopedLock> sl;

  if(tengine == "MyISAM")
    sl.reset(new ScopedLock(*m_t, dt->getName()));
  else if(tengine == "InnoDB")
    st.reset(new te::da::ScopedTransaction(*m_t));

// start inserts
  int ncommands = 100;

  int i = 0;
  
  std::auto_ptr<sql::Statement> stmt(m_t->getConnection()->getConn()->createStatement());

  sql::mysql::MySQL_Connection* conn = static_cast<sql::mysql::MySQL_Connection*>(m_t->getConnection()->getConn());

  try
  {
    std::string sql;

    do
    {
      sql += "INSERT INTO ";
      sql += dt->getName();
      sql += GetSQLValueNames(dt);
      sql += " VALUES";
      sql += GetSQLValues(dt, d, conn);
      sql += ";";

      ++i;

      if(i % ncommands == 0)
      {
        stmt->execute(sql);

        while(stmt->getMoreResults())
          std::auto_ptr<sql::ResultSet> res(stmt->getResultSet());

        i = 0;
        sql = "";
      }
    }while(d->moveNext());

    if(i != 0)
    {
      stmt->execute(sql);

      while(stmt->getMoreResults())
        std::auto_ptr<sql::ResultSet> res(stmt->getResultSet());
    }

    if(st.get())
      st->commit();
  }
  catch(const sql::SQLException &e)
  {
    throw Exception((boost::format(TR_MYSQL("Could not add dataset to the table due to the following error: %1%")) % e.what()).str());
  }
  catch(...)
  {
    throw Exception(TR_MYSQL("Unknown exception when adding a dataset to a table!"));
  }
}

// insercao multipla em uma linha
void te::mysql::DataSetPersistence::add7(const te::da::DataSetType* dt, te::da::DataSet* d)
{
  int ncommands = 10;
  int ngroups = 100;

  int i = 0;

  std::auto_ptr<sql::Statement> stmt(m_t->getConnection()->getConn()->createStatement());

  sql::mysql::MySQL_Connection* conn = static_cast<sql::mysql::MySQL_Connection*>(m_t->getConnection()->getConn());

  try
  {
    std::string sql;

    do
    {
      sql += "INSERT INTO ";
      sql += dt->getName();
      sql += GetSQLValueNames(dt);
      sql += " VALUES";

      int j = 0;

      do
      {
        if(j != 0)
          sql += ",";

        sql += GetSQLValues(dt, d, conn);
        
        ++j;

        if(j % ngroups == 0)
          break;
      }while(d->moveNext());

      sql += ";";

      ++i;

      if(i % ncommands == 0)
      {
        stmt->execute(sql);

        while(stmt->getMoreResults())
          std::auto_ptr<sql::ResultSet> res(stmt->getResultSet());

        i = 0;
        sql = "";
      }
    }while(d->moveNext());

    if(i != 0)
    {
      stmt->execute(sql);

      while(stmt->getMoreResults())
        std::auto_ptr<sql::ResultSet> res(stmt->getResultSet());
    }
  }
  catch(const sql::SQLException &e)
  {
    throw Exception((boost::format(TR_MYSQL("Could not add dataset to the table due to the following error: %1%")) % e.what()).str());
  }
  catch(...)
  {
    throw Exception(TR_MYSQL("Unknown exception when adding a dataset to a table!"));
  }
}

// usa transacao + insercao multipla em uma linha
void te::mysql::DataSetPersistence::add8(const te::da::DataSetType* dt, te::da::DataSet* d)
{
// find out the database engine used by the table in order to lock or start a transaction
  std::auto_ptr<DataSourceCatalogLoader> cloader(m_t->getMyCatalogLoader());

  std::string tengine = cloader->getTableEngine(dt->getName());

  std::auto_ptr<te::da::ScopedTransaction> st;
  std::auto_ptr<ScopedLock> sl;

  if(tengine == "MyISAM")
    sl.reset(new ScopedLock(*m_t, dt->getName()));
  else if(tengine == "InnoDB")
    st.reset(new te::da::ScopedTransaction(*m_t));

// start inserts
  int ncommands = 10;
  int ngroups = 100;
  //int ncommands = 10;
  //int ngroups = 2;

  int i = 0;

  std::auto_ptr<sql::Statement> stmt(m_t->getConnection()->getConn()->createStatement());

  sql::mysql::MySQL_Connection* conn = static_cast<sql::mysql::MySQL_Connection*>(m_t->getConnection()->getConn());

  try
  {
    std::string sql;

    bool nonstop = true;

    do
    {
      sql += "INSERT INTO ";
      sql += dt->getName();
      sql += GetSQLValueNames(dt);
      sql += " VALUES";

      int j = 0;

      do
      {
        if(j != 0)
          sql += ",";

        sql += GetSQLValues(dt, d, conn);
        
        ++j;

        if(j % ngroups == 0)
        {
          nonstop = d->moveNext();
          break;
        }

        nonstop = d->moveNext();

      }while(nonstop);

      sql += ";";

      ++i;

      if(i % ncommands == 0)
      {
        stmt->execute(sql);

        while(stmt->getMoreResults())
          std::auto_ptr<sql::ResultSet> res(stmt->getResultSet());

        i = 0;
        sql = "";
      }
    }while(nonstop);

    if(i != 0)
    {
      stmt->execute(sql);

      while(stmt->getMoreResults())
        std::auto_ptr<sql::ResultSet> res(stmt->getResultSet());
    }

    if(st.get())
      st->commit();
  }
  catch(const sql::SQLException &e)
  {
    throw Exception((boost::format(TR_MYSQL("Could not add dataset to the table due to the following error: %1%")) % e.what()).str());
  }
  catch(...)
  {
    throw Exception(TR_MYSQL("Unknown exception when adding a dataset to a table!"));
  }
}

// gera um arquivo e usa LOAD remoto
void te::mysql::DataSetPersistence::add9(const te::da::DataSetType* dt, te::da::DataSet* d)
{
  std::ofstream fout("c:/temp/mydump.sql", std::ios_base::trunc);

  sql::mysql::MySQL_Connection* conn = static_cast<sql::mysql::MySQL_Connection*>(m_t->getConnection()->getConn());

  try
  {
    std::string sql;

    do
    {
      sql = GetLoadDataRow(dt, d, conn);

      fout << sql;

    }while(d->moveNext());

    fout.close();

    sql  = ("LOAD DATA LOCAL INFILE 'c:/temp/mydump.sql' INTO TABLE ");
    sql += dt->getName();
    sql += " FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '\\'' LINES TERMINATED BY '\\n' ";
    sql += GetLoadDataFieldList(dt);

    m_t->execute(sql);
  }
  catch(const sql::SQLException &e)
  {
    fout.close();
    throw Exception((boost::format(TR_MYSQL("Could not add dataset to the table due to the following error: %1%")) % e.what()).str());
  }
  catch(...)
  {
    fout.close();
    throw Exception(TR_MYSQL("Unknown exception when adding a dataset to a table!"));
  }
}

// gera um arquivo e usa LOAD local
void te::mysql::DataSetPersistence::add10(const te::da::DataSetType* dt, te::da::DataSet* d)
{
// find out the database engine used by the table in order to lock or start a transaction
  std::auto_ptr<DataSourceCatalogLoader> cloader(m_t->getMyCatalogLoader());

  std::string tengine = cloader->getTableEngine(dt->getName());

  std::auto_ptr<te::da::ScopedTransaction> st;
  std::auto_ptr<ScopedLock> sl;

  if(tengine == "MyISAM")
    sl.reset(new ScopedLock(*m_t, dt->getName()));
  else if(tengine == "InnoDB")
    st.reset(new te::da::ScopedTransaction(*m_t));

  std::ofstream fout("c:/temp/mydump.sql", std::ios_base::trunc);

  sql::mysql::MySQL_Connection* conn = static_cast<sql::mysql::MySQL_Connection*>(m_t->getConnection()->getConn());

  try
  {
    std::string sql;

    do
    {
      sql = GetLoadDataRow(dt, d, conn);

      fout << sql;

    }while(d->moveNext());

    fout.close();

    sql  = ("LOAD DATA INFILE 'c:/temp/mydump.sql' INTO TABLE ");
    sql += dt->getName();
    sql += " FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '\\'' LINES TERMINATED BY '\\n' ";
    sql += GetLoadDataFieldList(dt);

    //m_t->execute(sql);

    if(st.get())
      st->commit();
  }
  catch(const sql::SQLException &e)
  {
    fout.close();
    throw Exception((boost::format(TR_MYSQL("Could not add dataset to the table due to the following error: %1%")) % e.what()).str());
  }
  catch(...)
  {
    fout.close();
    throw Exception(TR_MYSQL("Unknown exception when adding a dataset to a table!"));
  }
}

void te::mysql::DataSetPersistence::update1(const te::da::DataSetType* dt,
                                            te::da::DataSet* dataset,
                                            const std::vector<te::dt::Property*>& properties)
{
  const std::vector<te::dt::Property*>* keyProperties = 0;
 
  if(dt->getPrimaryKey())
  {
    keyProperties = &(dt->getPrimaryKey()->getProperties());
  }
  else if(dt->getNumberOfUniqueKeys() > 0)
  {
    keyProperties = &(dt->getUniqueKey(0)->getProperties());
  }
  else
  {
    throw Exception(TR_MYSQL("Can not update dataset item(s) because dataset doesn't have a primary key or unique key!")); 
  }

// find out the database engine used by the table in order to make a lock or start a transaction
  std::auto_ptr<DataSourceCatalogLoader> cloader(m_t->getMyCatalogLoader());

  std::string tengine = cloader->getTableEngine(dt->getName());

// create a prepared statement
  std::string sql  = "UPDATE ";
              sql += dt->getName();
              sql += " SET ";
              sql += GetBindableUpdateSQL(properties);
              sql += " WHERE ";
              sql += GetBindableWhereSQL(*keyProperties);

  std::vector<std::size_t> propertiesPos;

  te::dt::GetPropertiesPosition(properties, dt, propertiesPos);

  te::dt::GetPropertiesPosition(*keyProperties, dt, propertiesPos);

  std::auto_ptr<PreparedQuery> pq(static_cast<PreparedQuery*>(m_t->getPrepared()));

  std::vector<te::dt::Property*> allprops(properties);

  std::copy(keyProperties->begin(), keyProperties->end(), std::back_inserter(allprops));

  std::auto_ptr<te::da::ScopedTransaction> st;
  std::auto_ptr<ScopedLock> sl;

  if(tengine == "MyISAM")
    sl.reset(new ScopedLock(*m_t, dt->getName()));
  else if(tengine == "InnoDB")
    st.reset(new te::da::ScopedTransaction(*m_t));

  pq->prepare(sql, allprops);

  do
  {
    pq->bind(propertiesPos, dt, dataset);
    pq->execute();

  }while(dataset->moveNext());

  if(tengine == "InnoDB")
    st->commit();
}

void te::mysql::DataSetPersistence::update2(const te::da::DataSetType* dt,
                                            te::da::DataSet* dataset,
                                            const std::vector<te::dt::Property*>& properties)
{
// find out keys and properties position
  std::vector<std::size_t> propertiesPos;

  te::dt::GetPropertiesPosition(properties, dt, propertiesPos);

  const std::vector<te::dt::Property*>* keyProperties = 0;
 
  if(dt->getPrimaryKey())
  {
    keyProperties = &(dt->getPrimaryKey()->getProperties());
  }
  else if(dt->getNumberOfUniqueKeys() > 0)
  {
    keyProperties = &(dt->getUniqueKey(0)->getProperties());
  }
  else
  {
    throw Exception(TR_MYSQL("Can not update dataset item(s) because dataset doesn't have a primary key or unique key!")); 
  }

  std::vector<std::size_t> keyPos;

  te::dt::GetPropertiesPosition(*keyProperties, dt, keyPos);

// find out the database engine used by the table in order to lock or start a transaction
  std::auto_ptr<DataSourceCatalogLoader> cloader(m_t->getMyCatalogLoader());

  std::string tengine = cloader->getTableEngine(dt->getName());

  std::auto_ptr<te::da::ScopedTransaction> st;
  std::auto_ptr<ScopedLock> sl;

  if(tengine == "MyISAM")
    sl.reset(new ScopedLock(*m_t, dt->getName()));
  else if(tengine == "InnoDB")
    st.reset(new te::da::ScopedTransaction(*m_t));

// start inserts
  int ncommands = 100;

  int i = 0;

  std::auto_ptr<sql::Statement> stmt(m_t->getConnection()->getConn()->createStatement());

  sql::mysql::MySQL_Connection* conn = static_cast<sql::mysql::MySQL_Connection*>(m_t->getConnection()->getConn());

  try
  {
    std::string sql;

    do
    {
      sql += "UPDATE ";
      sql += dt->getName();
      sql += " SET ";
      sql += GetSQLUpdateValues(dt, dataset, propertiesPos, conn);
      sql += " WHERE ";
      sql += GetSQLWhereValues(dt, dataset, keyPos, conn);
      sql += ";";

      ++i;

      if(i % ncommands == 0)
      {
        stmt->execute(sql);

        while(stmt->getMoreResults())
          std::auto_ptr<sql::ResultSet> res(stmt->getResultSet());

        i = 0;
        sql = "";
      }
    }while(dataset->moveNext());

    if(i != 0)
    {
      stmt->execute(sql);

      while(stmt->getMoreResults())
        std::auto_ptr<sql::ResultSet> res(stmt->getResultSet());
    }

    if(st.get())
      st->commit();
  }
  catch(const sql::SQLException &e)
  {
    throw Exception((boost::format(TR_MYSQL("Could not update dataset due to the following error: %1%")) % e.what()).str());
  }
  catch(...)
  {
    throw Exception(TR_MYSQL("Unknown exception when updating a dataset!"));
  }
}
