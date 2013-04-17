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
  \file terralib/postgis/DataSetPersistence.cpp

  \brief This is the PostgreSQL implementation for the DataSetPersistence API.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/DataSetTypePersistence.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../dataaccess/datasource/ScopedTransaction.h"
#include "../datatype/Utils.h"
#include "Connection.h"
#include "DataSetPersistence.h"
#include "DataSource.h"
#include "Exception.h"
#include "PreparedQuery.h"
#include "Transactor.h"
#include "Utils.h"

// STL
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>

// Boost
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// libpq
#include <libpq-fe.h>

te::pgis::DataSetPersistence::DataSetPersistence(Transactor* t)
  : m_t(t)
{
  assert(m_t);
}

te::pgis::DataSetPersistence::~DataSetPersistence()
{
}

void te::pgis::DataSetPersistence::remove(const std::string& datasetName, const te::da::ObjectIdSet* oids)
{
  //std::string sql("DELETE FROM ");
  //          sql += datasetName);

  //m_t->execute(sql);

//  const std::vector<te::dt::Property*>* keyProperties = 0;
// 
//  if(dt->getPrimaryKey())
//  {
//    keyProperties = &(dt->getPrimaryKey()->getProperties());
//  }
//  else if(dt->getNumberOfUniqueKeys() > 0)
//  {
//    keyProperties = &(dt->getUniqueKey(0)->getProperties());
//  }
//  else
//  {
//    throw Exception(TR_PGIS("Can not remove dataset items because dataset doesn't have a primary key or unique key!")); 
//  }
//
//// create a prepared statement
//  std::string sql  = "DELETE FROM ";
//              sql += dt->getName();
//              sql += " WHERE ";
//              sql += GetBindableWhereSQL(*keyProperties);
//
//  std::vector<std::size_t> propertiesPos;
//  
//  te::dt::GetPropertiesPosition(*keyProperties, dt, propertiesPos);
//
//  std::auto_ptr<PreparedQuery> pq(m_t->getPGPrepared("a" + boost::lexical_cast<std::string>((boost::int64_t)(this))));
//
//  te::da::ScopedTransaction st(*m_t);
//
//  pq->prepare(sql, *keyProperties);
//
//  do
//  {
//    pq->bind(propertiesPos, dt, d);
//    pq->execute();
//
//  }while(d->moveNext());
//
//  st.commit();

//  const std::vector<te::dt::Property*>* keyProperties = 0;
// 
//  if(dt->getPrimaryKey())
//  {
//    keyProperties = &(dt->getPrimaryKey()->getProperties());
//  }
//  else if(dt->getNumberOfUniqueKeys() > 0)
//  {
//    keyProperties = &(dt->getUniqueKey(0)->getProperties());
//  }
//  else
//  {
//    throw Exception(TR_PGIS("Can not remove dataset item because dataset type doesn't have a primary key or unique key!")); 
//  }
//
//// create a prepared statement
//  std::string sql  = "DELETE FROM ";
//              sql += dt->getName();
//              sql += " WHERE ";
//              sql += GetBindableWhereSQL(*keyProperties);
//
//  std::vector<std::size_t> propertiesPos;
//  
//  te::dt::GetPropertiesPosition(*keyProperties, dt, propertiesPos);
//
//  std::auto_ptr<PreparedQuery> pq(m_t->getPGPrepared("a" + boost::lexical_cast<std::string>((boost::int64_t)(this))));
//
//  pq->prepare(sql, *keyProperties);
//  pq->bind(propertiesPos, dt, item);
//  pq->execute();
}

void te::pgis::DataSetPersistence::add(const std::string& datasetName, te::da::DataSet* d, const std::map<std::string, std::string>& /*options*/, std::size_t /*limit*/)
{
//// create a prepared statement
//  std::string sql  = "INSERT INTO ";
//              sql += datasetName;
//              sql += GetSQLValueNames(d);
//              sql += " VALUES";
//              sql += GetSQLBindValues(d->getNumProperties());
//
//  std::auto_ptr<PreparedQuery> pq(m_t->getPGPrepared("a" + boost::lexical_cast<std::string>((boost::int64_t)(this))));
//
//  te::da::ScopedTransaction st(*m_t);
//
//  pq->prepare(sql, dt->getProperties());
//
//  do
//  {
//    pq->bind(dt, d);
//    pq->execute();
//
//  }while(d->moveNext());
//
//  st.commit();

//// create a prepared statement
//  std::string sql  = "INSERT INTO ";
//              sql += dt->getName();
//              sql += GetSQLValueNames(dt);
//              sql += " VALUES";
//              sql += GetSQLBindValues(dt);
//
//  std::auto_ptr<PreparedQuery> pq(m_t->getPGPrepared("a" + boost::lexical_cast<std::string>((boost::int64_t)(this))));
//
//  pq->prepare(sql, dt->getProperties());
//  pq->bind(dt, item);
//  pq->execute();

}

void te::pgis::DataSetPersistence::update(const std::string& datasetName,
                                          te::da::DataSet* dataset,
                                          const std::vector<std::size_t>& properties,
                                          const te::da::ObjectIdSet* oids,
                                          const std::map<std::string, std::string>& options,
                                          std::size_t limit)
{
//  const std::vector<te::dt::Property*>* keyProperties = 0;
// 
//  if(dt->getPrimaryKey())
//  {
//    keyProperties = &(dt->getPrimaryKey()->getProperties());
//  }
//  else if(dt->getNumberOfUniqueKeys() > 0)
//  {
//    keyProperties = &(dt->getUniqueKey(0)->getProperties());
//  }
//  else
//  {
//    throw Exception(TR_PGIS("Can not update dataset item(s) because dataset doesn't have a primary key or unique key!")); 
//  }
//
//// create a prepared statement
//  std::string sql  = "UPDATE ";
//              sql += dt->getName();
//              sql += " SET ";
//              sql += GetBindableUpdateSQL(properties);
//              sql += " WHERE ";
//              sql += GetBindableWhereSQL(*keyProperties, properties.size());
//
//  std::vector<std::size_t> propertiesPos;
//
//  te::dt::GetPropertiesPosition(properties, dt, propertiesPos);
//
//  te::dt::GetPropertiesPosition(*keyProperties, dt, propertiesPos);
//
//  std::auto_ptr<PreparedQuery> pq(m_t->getPGPrepared("a" + boost::lexical_cast<std::string>((boost::int64_t)(this))));
//
//  std::vector<te::dt::Property*> allprops(properties);
//
//  std::copy(keyProperties->begin(), keyProperties->end(), allprops.end());
//
//  te::da::ScopedTransaction st(*m_t);
//
//  pq->prepare(sql, allprops);
//  
//  do
//  {
//    pq->bind(propertiesPos, dt, dataset);
//    pq->execute();
//
//  }while(dataset->moveNext());
//
//  st.commit();
}

//void te::pgis::DataSetPersistence::update(const te::da::DataSetType* dt,
//                                          te::da::DataSet* oldD,
//                                          te::da::DataSet* newD,
//                                          const std::vector<te::dt::Property*>& properties,
//                                          std::size_t /*limit*/)
//{
//  const std::vector<te::dt::Property*>* keyProperties = 0;
// 
//  if(dt->getPrimaryKey())
//  {
//    keyProperties = &(dt->getPrimaryKey()->getProperties());
//  }
//  else if(dt->getNumberOfUniqueKeys() > 0)
//  {
//    keyProperties = &(dt->getUniqueKey(0)->getProperties());
//  }
//  else
//  {
//    throw Exception(TR_PGIS("Can not update dataset item(s) because dataset doesn't have a primary key or unique key!")); 
//  }
//
//// create a prepared statement
//  std::string sql  = "UPDATE ";
//              sql += dt->getName();
//              sql += " SET ";
//              sql += GetBindableUpdateSQL(properties);
//              sql += " WHERE ";
//              sql += GetBindableWhereSQL(*keyProperties, properties.size());
//
//  std::vector<std::size_t> propertiesPos;
//
//  te::dt::GetPropertiesPosition(properties, dt, propertiesPos);
//
//  std::vector<std::size_t> keysPos;
//  
//  te::dt::GetPropertiesPosition(*keyProperties, dt, keysPos);
//
//  std::vector<te::dt::Property*> allprops(properties);
//
//  std::copy(keyProperties->begin(), keyProperties->end(), allprops.end());
//
//  std::auto_ptr<PreparedQuery> pq(m_t->getPGPrepared("a" + boost::lexical_cast<std::string>((boost::int64_t)(this))));
//
//  te::da::ScopedTransaction st(*m_t);
//
//  pq->prepare(sql, allprops);
//
//  do
//  {
//    pq->bind(propertiesPos, dt, newD);
//    pq->bind(keysPos, propertiesPos.size(), dt, oldD);
//    pq->execute();
//
//  }while(oldD->moveNext() && newD->moveNext());
//
//  st.commit();
//}
//            
//void te::pgis::DataSetPersistence::update(const te::da::DataSetType* dt,
//                                          te::da::DataSetItem* item,
//                                          const std::vector<te::dt::Property*>& properties)
//{
//  const std::vector<te::dt::Property*>* keyProperties = 0;
// 
//  if(dt->getPrimaryKey())
//  {
//    keyProperties = &(dt->getPrimaryKey()->getProperties());
//  }
//  else if(dt->getNumberOfUniqueKeys() > 0)
//  {
//    keyProperties = &(dt->getUniqueKey(0)->getProperties());
//  }
//  else
//  {
//    throw Exception(TR_PGIS("Can not update dataset item(s) because dataset doesn't have a primary key or unique key!")); 
//  }
//
//// create a prepared statement
//  std::string sql  = "UPDATE ";
//              sql += dt->getName();
//              sql += " SET ";
//              sql += GetBindableUpdateSQL(properties);
//              sql += " WHERE ";
//              sql += GetBindableWhereSQL(*keyProperties, properties.size());
//
//  std::vector<std::size_t> propertiesPos;
//
//  te::dt::GetPropertiesPosition(properties, dt, propertiesPos);
//
//  te::dt::GetPropertiesPosition(*keyProperties, dt, propertiesPos);
//
//  std::vector<te::dt::Property*> allprops(properties);
//
//  std::copy(keyProperties->begin(), keyProperties->end(), allprops.end());
//
//  std::auto_ptr<PreparedQuery> pq(m_t->getPGPrepared("a" + boost::lexical_cast<std::string>((boost::int64_t)(this))));
//
//  pq->prepare(sql, allprops);
//
//  pq->bind(propertiesPos, dt, item);
//  pq->execute();
//}
//            
//void te::pgis::DataSetPersistence::update(const te::da::DataSetType* dt,
//                                          te::da::DataSetItem* oldItem,
//                                          te::da::DataSetItem* newItem,
//                                          const std::vector<te::dt::Property*>& properties)
//{
//  const std::vector<te::dt::Property*>* keyProperties = 0;
// 
//  if(dt->getPrimaryKey())
//  {
//    keyProperties = &(dt->getPrimaryKey()->getProperties());
//  }
//  else if(dt->getNumberOfUniqueKeys() > 0)
//  {
//    keyProperties = &(dt->getUniqueKey(0)->getProperties());
//  }
//  else
//  {
//    throw Exception(TR_PGIS("Can not update dataset item(s) because dataset doesn't have a primary key or unique key!")); 
//  }
//
//// create a prepared statement
//  std::string sql  = "UPDATE ";
//              sql += dt->getName();
//              sql += " SET ";
//              sql += GetBindableUpdateSQL(properties);
//              sql += " WHERE ";
//              sql += GetBindableWhereSQL(*keyProperties, properties.size());
//
//  std::vector<std::size_t> propertiesPos;
//
//  te::dt::GetPropertiesPosition(properties, dt, propertiesPos);
//
//  std::vector<std::size_t> keysPos;
//  
//  te::dt::GetPropertiesPosition(*keyProperties, dt, keysPos);
//
//  std::vector<te::dt::Property*> allprops(properties);
//
//  std::copy(keyProperties->begin(), keyProperties->end(), allprops.end());
//
//  std::auto_ptr<PreparedQuery> pq(m_t->getPGPrepared("a" + boost::lexical_cast<std::string>((boost::int64_t)(this))));
//
//  pq->prepare(sql, allprops);
//
//  pq->bind(propertiesPos, dt, newItem);
//  pq->bind(keysPos, propertiesPos.size(), dt, oldItem);
//  pq->execute();
//}

