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
  \file terralib/ado/DataSourceTransactor.cpp

  \brief An implementation of DataSourceTransactor class for the TerraLib ADO Data Access driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"
#include "DataSource.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "DataSet.h"
#include "DataSourceCatalogLoader.h"
#include "DataSourceTransactor.h"
#include "DataSetTypePersistence.h"
#include "DataSetPersistence.h"
#include "Exception.h"

// ADO
#import "msado15.dll" \
    no_namespace rename("EOF", "EndOfFile")

#include <oledb.h>
#include <stdio.h>
#include <conio.h>
#include "icrsint.h"

inline void TESTHR(HRESULT x)
{
  if FAILED(x)
    _com_issue_error(x);
};

te::ado::DataSourceTransactor::DataSourceTransactor(DataSource* parent, _ConnectionPtr conn)
  : m_ds(parent),
  m_conn(conn),
  m_isInTransaction(false)
{
}

te::ado::DataSourceTransactor::~DataSourceTransactor()
{
}

void te::ado::DataSourceTransactor::begin()
{
  m_conn->BeginTrans();
  m_isInTransaction = true;
}

void te::ado::DataSourceTransactor::commit()
{
  m_conn->CommitTrans();
  m_isInTransaction = false;
}

void te::ado::DataSourceTransactor::rollBack()
{
  m_conn->RollbackTrans();
  m_isInTransaction = false;
}

bool te::ado::DataSourceTransactor::isInTransaction() const
{
  return m_isInTransaction;
}

te::da::DataSet* te::ado::DataSourceTransactor::getDataSet(const std::string& name, 
                                                                te::common::TraverseType travType, 
                                                                te::common::AccessPolicy rwRole)
{
  te::da::DataSourceCatalogLoader* loader = getCatalogLoader();
  te::da::DataSetType* dt = 0;
  dt = loader->getDataSetType(name);

  if(dt == 0)
    throw Exception(TR_ADO("Data Set not found!"));

  _RecordsetPtr recset;
  TESTHR(recset.CreateInstance(__uuidof(Recordset)));

  TESTHR(recset->Open(_bstr_t(name.c_str()),
    _variant_t((IDispatch*)m_conn,true), adOpenKeyset, adLockOptimistic, adCmdTable));

  return new DataSet(dt, recset, this);
}

te::da::DataSet* te::ado::DataSourceTransactor::getDataSet(const std::string& name,
                                                                const te::dt::Property* p,
                                                                const te::gm::Envelope* e,
                                                                te::gm::SpatialRelation r,
                                                                te::common::TraverseType travType, 
                                                                te::common::AccessPolicy rwRole)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::DataSet* te::ado::DataSourceTransactor::getDataSet(const std::string& name,
                                                                const te::dt::Property* p,
                                                                const te::gm::Geometry* g,
                                                                te::gm::SpatialRelation r,
                                                                te::common::TraverseType travType, 
                                                                te::common::AccessPolicy rwRole)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::DataSet* te::ado::DataSourceTransactor::query(const te::da::Select& q, 
                                                           te::common::TraverseType travType, 
                                                           te::common::AccessPolicy rwRole)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::DataSet* te::ado::DataSourceTransactor::query(const std::string& query, 
                                                           te::common::TraverseType travType, 
                                                           te::common::AccessPolicy rwRole)
{
  _RecordsetPtr rs;
  
  TESTHR(rs.CreateInstance(__uuidof(Recordset)));
  
  rs->Open(query.c_str(), _variant_t((IDispatch *)m_conn), adOpenStatic, adLockReadOnly, adCmdText);

  return new DataSet(0, rs, this);
}

void te::ado::DataSourceTransactor::execute(const te::da::Query& command)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSourceTransactor::execute(const std::string& command)
{
  try
  {
    m_conn->Execute(_bstr_t(command.c_str()),0, adCmdText);
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

te::da::PreparedQuery* te::ado::DataSourceTransactor::getPrepared(const std::string& qName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::BatchExecutor* te::ado::DataSourceTransactor::getBatchExecutor()
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::DataSourceCatalogLoader* te::ado::DataSourceTransactor::getCatalogLoader()
{
  return new DataSourceCatalogLoader(this);
}

te::da::DataSetTypePersistence* te::ado::DataSourceTransactor::getDataSetTypePersistence()
{
  return new DataSetTypePersistence(this);
}

te::da::DataSetPersistence* te::ado::DataSourceTransactor::getDataSetPersistence()
{
  return new DataSetPersistence(this);
}

void te::ado::DataSourceTransactor::cancel()
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::DataSource* te::ado::DataSourceTransactor::getDataSource() const
{
  return m_ds;
}

_ConnectionPtr te::ado::DataSourceTransactor::getADOConnection()
{
  return m_conn;
}

