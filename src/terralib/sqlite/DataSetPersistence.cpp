/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/sqlite/DataSetPersistence.cpp

  \brief DataSetPersistence implementation for the TerraLib SQLite Data Access Driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetItem.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/DataSetTypePersistence.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../dataaccess/datasource/ScopedTransaction.h"
#include "../datatype/Property.h"
#include "../datatype/Utils.h"
#include "DataSetPersistence.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "PreparedQuery.h"
#include "Utils.h"

// STL
#include <memory>

// Boost
#include <boost/format.hpp>

// SQLite
#include <sqlite3.h>

te::sqlite::DataSetPersistence::DataSetPersistence(DataSourceTransactor* parent)
  : m_t(parent)
{
}

te::sqlite::DataSetPersistence::~DataSetPersistence()
{
}

void te::sqlite::DataSetPersistence::create(te::da::DataSetType* dt, te::da::DataSet* d, const std::map<std::string, std::string>& options, std::size_t limit)
{
  std::auto_ptr<te::da::DataSetTypePersistence> dtp(m_t->getDataSetTypePersistence());

  dtp->create(dt, options);
  
  add(dt, d, options, limit);
}

void te::sqlite::DataSetPersistence::remove(const te::da::DataSetType* dt)
{
  std::string sql("DELETE FROM ");
              sql += dt->getName();

  m_t->execute(sql);  
}

void te::sqlite::DataSetPersistence::remove(const std::string& /*datasetName*/)
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

void te::sqlite::DataSetPersistence::remove(const te::da::DataSetType* dt, te::da::DataSet* d, std::size_t /*limit*/)
{
  const std::vector<te::dt::Property*>* keyproperties; 
 
  if(dt->getPrimaryKey())
  {
    keyproperties = &(dt->getPrimaryKey()->getProperties()); 
  }
  else if(dt->getNumberOfUniqueKeys() > 0)
  {
    keyproperties = &(dt->getUniqueKey(0)->getProperties());
  }
  else
  {
    throw Exception(TR_SQLITE("Can not remove dataset item(s) because dataset doesn't have a primary key or unique key!")); 
  }

// create a prepared statement
  std::string sql  = "DELETE FROM ";
              sql += dt->getName();
              sql += " WHERE ";
              sql += GetBindableWhereSQL(keyproperties->begin(), keyproperties->end());

  std::vector<std::size_t> propertiesPos;
  
  te::dt::GetPropertiesPosition(*keyproperties, dt, propertiesPos);

  std::auto_ptr<PreparedQuery> pq(m_t->getLitePrepared());

  pq->prepare(sql);

  do
  {
    pq->bind(propertiesPos, dt, d);
    pq->execute();
  }while(d->moveNext());
}

void te::sqlite::DataSetPersistence::remove(const te::da::DataSetType* dt, te::da::DataSetItem* item)
{
  const std::vector<te::dt::Property*>* keyproperties; 
 
  if(dt->getPrimaryKey())
  {
    keyproperties = &(dt->getPrimaryKey()->getProperties()); 
  }
  else if(dt->getNumberOfUniqueKeys() > 0)
  {
    keyproperties = &(dt->getUniqueKey(0)->getProperties());
  }
  else
  {
    throw Exception(TR_SQLITE("Can not remove dataset item because dataset doesn't have a primary key or unique key!")); 
  }

// create a prepared statement
  std::string sql  = "DELETE FROM ";
              sql += dt->getName();
              sql += " WHERE ";
              sql += GetBindableWhereSQL(keyproperties->begin(), keyproperties->end());

  std::vector<std::size_t> propertiesPos;
  
  te::dt::GetPropertiesPosition(*keyproperties, dt, propertiesPos);

  std::auto_ptr<PreparedQuery> pq(m_t->getLitePrepared());

  pq->prepare(sql);
  pq->bind(propertiesPos, dt, item);
  pq->execute();
}

void te::sqlite::DataSetPersistence::add(const te::da::DataSetType* dt, te::da::DataSet* d, const std::map<std::string, std::string>& /*options*/, std::size_t /*limit*/)
{
// create a prepared statement
  std::string sql  = "INSERT INTO ";
              sql += dt->getName();
              sql += GetSQLValueNames(dt);
              sql += " VALUES";
              sql += GetSQLBindValues(dt);

  te::da::ScopedTransaction st(*m_t);

  std::auto_ptr<PreparedQuery> pq(m_t->getLitePrepared());

  pq->prepare(sql);

  do
  {
    pq->bind(dt, d);
    pq->execute();
  }while(d->moveNext());

  st.commit();
}

void te::sqlite::DataSetPersistence::add(const te::da::DataSetType* dt, te::da::DataSetItem* item)
{
// create a prepared statement
  std::string sql  = "INSERT INTO ";
              sql += dt->getName();
              sql += GetSQLValueNames(dt);
              sql += " VALUES";
              sql += GetSQLBindValues(dt);

  std::auto_ptr<PreparedQuery> pq(m_t->getLitePrepared());

  pq->prepare(sql);
  pq->bind(dt, item);
  pq->execute();
}

void te::sqlite::DataSetPersistence::update(const te::da::DataSetType* dt,
                                            te::da::DataSet* dataset,
                                            const std::vector<te::dt::Property*>& properties,
                                            const std::map<std::string, std::string>& /*options*/,
                                            std::size_t /*limit*/)
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
    throw Exception(TR_SQLITE("Can not update dataset item(s) because dataset doesn't have a primary key or unique key!")); 
  }

// create a prepared statement
  std::string sql  = "UPDATE ";
              sql += dt->getName();
              sql += " SET ";
              sql += GetBindableUpdateSQL(properties);
              sql += " WHERE ";
              sql += GetBindableWhereSQL(keyProperties->begin(), keyProperties->end());

  std::vector<std::size_t> propertiesPos;

  te::dt::GetPropertiesPosition(properties, dt, propertiesPos);

  te::dt::GetPropertiesPosition(*keyProperties, dt, propertiesPos);

  std::auto_ptr<PreparedQuery> pq(m_t->getLitePrepared());

  te::da::ScopedTransaction st(*m_t);

  pq->prepare(sql);

  do
  {
    pq->bind(propertiesPos, dt, dataset);
    pq->execute();

  }while(dataset->moveNext());

  st.commit();
}

void te::sqlite::DataSetPersistence::update(const te::da::DataSetType* dt,
                                            te::da::DataSet* oldD,
                                            te::da::DataSet* newD,
                                            const std::vector<te::dt::Property*>& properties,
                                            std::size_t /*limit*/)
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
    throw Exception(TR_SQLITE("Can not update dataset item(s) because dataset doesn't have a primary key or unique key!")); 
  }

// create a prepared statement
  std::string sql  = "UPDATE ";
              sql += dt->getName();
              sql += " SET ";
              sql += GetBindableUpdateSQL(properties);
              sql += " WHERE ";
              sql += GetBindableWhereSQL(keyProperties->begin(), keyProperties->end());

  std::vector<std::size_t> propertiesPos;

  te::dt::GetPropertiesPosition(properties, dt, propertiesPos);

  std::vector<std::size_t> keysPos;
  
  te::dt::GetPropertiesPosition(*keyProperties, dt, keysPos);

  std::auto_ptr<PreparedQuery> pq(m_t->getLitePrepared());

  te::da::ScopedTransaction st(*m_t);

  pq->prepare(sql);

  do
  {
    pq->bind(propertiesPos, dt, newD);
    pq->bind(keysPos, propertiesPos.size(), dt, oldD);
    pq->execute();
  }while(oldD->moveNext() && newD->moveNext());

  st.commit();
}

void te::sqlite::DataSetPersistence::update(const te::da::DataSetType* dt,
                                            te::da::DataSetItem* item,
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
    throw Exception(TR_SQLITE("Can not update dataset item(s) because dataset doesn't have a primary key or unique key!")); 
  }

// create a prepared statement
  std::string sql  = "UPDATE ";
              sql += dt->getName();
              sql += " SET ";
              sql += GetBindableUpdateSQL(properties);
              sql += " WHERE ";
              sql += GetBindableWhereSQL(keyProperties->begin(), keyProperties->end());

  std::vector<std::size_t> propertiesPos;

  te::dt::GetPropertiesPosition(properties, dt, propertiesPos);

  te::dt::GetPropertiesPosition(*keyProperties, dt, propertiesPos);

  std::auto_ptr<PreparedQuery> pq(m_t->getLitePrepared());

  pq->prepare(sql);

  pq->bind(propertiesPos, dt, item);
  pq->execute();
}

void te::sqlite::DataSetPersistence::update(const te::da::DataSetType* dt,
                                            te::da::DataSetItem* oldItem,
                                            te::da::DataSetItem* newItem,
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
    throw Exception(TR_SQLITE("Can not update dataset item(s) because dataset doesn't have a primary key or unique key!")); 
  }

// create a prepared statement
  std::string sql  = "UPDATE ";
              sql += dt->getName();
              sql += " SET ";
              sql += GetBindableUpdateSQL(properties);
              sql += " WHERE ";
              sql += GetBindableWhereSQL(keyProperties->begin(), keyProperties->end());

  std::vector<std::size_t> propertiesPos;

  te::dt::GetPropertiesPosition(properties, dt, propertiesPos);

  std::vector<std::size_t> keysPos;
  
  te::dt::GetPropertiesPosition(*keyProperties, dt, keysPos);

  std::auto_ptr<PreparedQuery> pq(m_t->getLitePrepared());

  pq->prepare(sql);

  pq->bind(propertiesPos, dt, newItem);
  pq->bind(keysPos, propertiesPos.size(), dt, oldItem);
  pq->execute();
}

te::da::DataSourceTransactor* te::sqlite::DataSetPersistence::getTransactor() const
{
  return m_t;
}

