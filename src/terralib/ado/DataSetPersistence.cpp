/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file DataSetPersistence.cpp

  \brief DataSetPersistence implementation for ADO.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/DataSetItem.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../geometry/WKBReader.h"
#include "../geometry/WKBWriter.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryProperty.h"
#include "../datatype/ByteArray.h"
#include "../datatype/AbstractData.h"
#include "../memory/DataSetItem.h"
#include "../dataaccess/dataset/DataSetTypePersistence.h"
#include "../datatype/Property.h"
#include "DataSetPersistence.h"
#include "DataSourceTransactor.h"
#include "Exception.h"

#include "Utils.h"

// ADO
#import "msado15.dll" \
    no_namespace rename("EOF", "EndOfFile")
#import "msadox.dll"

#include <vector>

inline void TESTHR(HRESULT x)
{
  if FAILED(x)
    _com_issue_error(x);
};

te::ado::DataSetPersistence::DataSetPersistence(DataSourceTransactor* parent)
  : m_t(parent)
{
}

te::ado::DataSetPersistence::~DataSetPersistence()
{
}

void te::ado::DataSetPersistence::create(te::da::DataSetType* dt, te::da::DataSet* d, const std::map<std::string, std::string>& options, std::size_t limit)
{
  te::da::DataSetTypePersistence* dtp = m_t->getDataSetTypePersistence();

  dtp->create(dt);

  add(dt, d);
}

void te::ado::DataSetPersistence::remove(const te::da::DataSetType* dt)
{

  _ConnectionPtr adoConn = m_t->getADOConnection();

  _RecordsetPtr recset;
  TESTHR(recset.CreateInstance(__uuidof(Recordset)));

  recset->CursorLocation = adUseClient;
  recset->CursorType = adOpenStatic;
  recset->LockType = adLockBatchOptimistic;

  try
  {
    TESTHR(recset->Open(_bstr_t(dt->getName().c_str()),
    _variant_t((IDispatch*)adoConn,true), adOpenKeyset, adLockOptimistic, adCmdTable));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  TESTHR(recset->Delete(adAffectCurrent));

  te::da::DataSet* ds = m_t->getDataSet(dt->getName());

}

void te::ado::DataSetPersistence::remove(const te::da::DataSetType* dt, te::da::DataSetItem* item)
{
  std::vector<te::dt::Property*> keyProperties;
 
  if(dt->getPrimaryKey())
  {
    keyProperties = dt->getPrimaryKey()->getProperties();
  }
  else if(dt->getNumberOfUniqueKeys() > 0)
  {
    keyProperties = dt->getUniqueKey(0)->getProperties();
  }
  else
  {
    throw Exception(TR_ADO("Can not remove dataset items because dataset doesn't have a primary key or unique key!")); 
  }

  _ConnectionPtr adoConn = m_t->getADOConnection();

  std::string sql = "SELECT * FROM " + dt->getName() + " WHERE ";

  for(size_t i = 0; i < keyProperties.size(); i++)
  {
    if(keyProperties[i]->getType() == te::dt::STRING_TYPE)
      sql += keyProperties[i]->getName() + " = '" + item->getValue(keyProperties[i]->getName())->toString() + "'";
    else
      sql += keyProperties[i]->getName() + " = " + item->getValue(keyProperties[i]->getName())->toString();

    if(i+1 != keyProperties.size())
      sql += " AND ";
  }

  sql += ";";
  
  _RecordsetPtr adoItem = 0;

  try
  {
    TESTHR(adoItem.CreateInstance(__uuidof(Recordset)));
    adoItem->Open(sql.c_str(), 
           _variant_t((IDispatch *)adoConn), 
           adOpenKeyset, adLockOptimistic, adCmdText);

    adoItem->Delete(adAffectCurrent);
    adoItem->Update();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::DataSetPersistence::remove(const te::da::DataSetType* dt, te::da::DataSet* d, std::size_t /*limit*/)
{
  try
  {
    do
    {
      m_t->begin();
      remove(dt, d->getItem());
      m_t->commit();
    }
    while(d->moveNext());
  }
  catch(...)
  {
    m_t->rollBack();
  }

}

void te::ado::DataSetPersistence::add(const te::da::DataSetType* dt, te::da::DataSet* d, std::size_t /*limit*/)
{
  do
  {
    add(dt, d->getItem());
  }
  while(d->moveNext());
}

void te::ado::DataSetPersistence::add(const te::da::DataSetType* dt, te::da::DataSetItem* item)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  std::vector<te::dt::Property*> props = dt->getProperties();

  _RecordsetPtr recset;
  TESTHR(recset.CreateInstance(__uuidof(Recordset)));
  
  try
  {
    TESTHR(recset->Open(_bstr_t(dt->getName().c_str()),
    _variant_t((IDispatch*)adoConn,true), adOpenKeyset, adLockOptimistic, adCmdTable));

    TESTHR(recset->AddNew());

    te::gm::GeometryProperty* geomProp = 0;
    geomProp = dt->getDefaultGeomProperty();

    te::ado::addItem(dt, recset, props, item);

    /*for(size_t i = 0; i < props.size(); i++)
    {
      te::ado::addItem()
      te::ado::updateAdoColumn(dt, recset, props[i], item);
    }*/
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

}

void te::ado::DataSetPersistence::update(const te::da::DataSetType* dt,
                      te::da::DataSet* dataset,
                      const std::vector<te::dt::Property*>& properties,
                      std::size_t /*limit*/)
{
  try
  {
    do
    {
      m_t->begin();
      update(dt, dataset->getItem(), properties);
      m_t->commit();
    }
    while(dataset->moveNext());
  }
  catch(...)
  {
    m_t->rollBack();
  }
  
}

void te::ado::DataSetPersistence::update(const te::da::DataSetType* /*dt*/,
                    te::da::DataSet* /*oldD*/,
                    te::da::DataSet* /*newD*/,
                    const std::vector<te::dt::Property*>& /*properties*/,
                    std::size_t /*limit*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSetPersistence::update(const te::da::DataSetType* dt,
                    te::da::DataSetItem* item,
                    const std::vector<te::dt::Property*>& properties)
{
  std::vector<te::dt::Property*> keyProperties;
 
  if(dt->getPrimaryKey())
  {
    keyProperties = dt->getPrimaryKey()->getProperties();
  }
  else if(dt->getNumberOfUniqueKeys() > 0)
  {
    keyProperties = dt->getUniqueKey(0)->getProperties();
  }
  else
  {
    throw Exception(TR_ADO("Can not remove dataset items because dataset doesn't have a primary key or unique key!")); 
  }

  _ConnectionPtr adoConn = m_t->getADOConnection();

  std::string sql = "SELECT * FROM " + dt->getName() + " WHERE ";

  for(size_t i = 0; i < keyProperties.size(); i++)
  {
    if(keyProperties[i]->getType() == te::dt::STRING_TYPE)
      sql += keyProperties[i]->getName() + " = '" + item->getValue(keyProperties[i]->getName())->toString() + "'";
    else
      sql += keyProperties[i]->getName() + " = " + item->getValue(keyProperties[i]->getName())->toString();

    if(i+1 != keyProperties.size())
      sql += " AND ";
  }

  sql += ";";
  
  _RecordsetPtr adoItem = 0;

  try
  {
    TESTHR(adoItem.CreateInstance(__uuidof(Recordset)));
    adoItem->Open(sql.c_str(), 
           _variant_t((IDispatch *)adoConn), 
           adOpenKeyset, adLockOptimistic, adCmdText);
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  for(size_t i = 0; i < properties.size(); i++)
    te::ado::updateAdoColumn(dt, adoItem, properties[i], item);
}

void te::ado::DataSetPersistence::update(const te::da::DataSetType* /*dt*/,
                    te::da::DataSetItem* /*oldItem*/,
                    te::da::DataSetItem* /*newItem*/,
                    const std::vector<te::dt::Property*>& /*properties*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::DataSourceTransactor* te::ado::DataSetPersistence::getTransactor() const
{
  return m_t;
}
                 
