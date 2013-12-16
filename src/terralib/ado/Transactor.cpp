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
  \file terralib/ado/Transactor.cpp

  \brief DataSourceTransactor class implementation for Microsoft Access driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/CheckConstraint.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/ForeignKey.h"
#include "../dataaccess/dataset/Index.h"
#include "../dataaccess/dataset/ObjectIdSet.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/Sequence.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../dataaccess/datasource/ScopedTransaction.h"
#include "../dataaccess/query/Select.h"
#include "../dataaccess/query/SQLDialect.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/Array.h"
#include "../datatype/Date.h"
#include "../datatype/DateTimeProperty.h"
#include "../datatype/Property.h"
#include "../datatype/SimpleData.h"
#include "../datatype/StringProperty.h"
#include "../geometry/Envelope.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Utils.h"
#include "../geometry/Geometry.h"
#include "../memory/DataSet.h"
#include "Connection.h"
#include "DataSet.h"
#include "DataSource.h"
#include "Exception.h"
#include "Globals.h"
#include "SQLVisitor.h"
#include "Transactor.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <iostream>

// Boost
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// ADO
#import "msado15.dll" \
  no_namespace rename("EOF", "EndOfFile")
#import "msadox.dll"

inline void TESTHR(HRESULT hr)
{
  if(FAILED(hr))
    _com_issue_error(hr);
}

te::ado::Transactor::Transactor(DataSource* ds)
  : m_ds(ds),
    m_conn(0),
    m_isInTransaction(false)
{
  std::string connInfoAux = MakeConnectionStr(ds->getConnectionInfo());

  m_conn = new te::ado::Connection(connInfoAux);
}

te::ado::Transactor::~Transactor()
{
  delete m_conn;
}

te::da::DataSource* te::ado::Transactor::getDataSource() const
{
  return m_ds;
}

void te::ado::Transactor::begin()
{
  try
  {
    m_conn->getConn()->BeginTrans();
    m_isInTransaction = true;
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::Transactor::commit()
{
  try
  {
    m_conn->getConn()->CommitTrans();
    m_isInTransaction = false;
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::Transactor::rollBack()
{
  try
  {
    m_conn->getConn()->RollbackTrans();
    m_isInTransaction = false;
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

bool te::ado::Transactor::isInTransaction() const
{
  return m_isInTransaction;
}

std::auto_ptr<te::da::DataSet> te::ado::Transactor::getDataSet(const std::string& name,
                                                                te::common::TraverseType travType,
                                                                bool connected,
                                                                const te::common::AccessPolicy accessPolicy)
{
  std::string sql("SELECT * FROM ");
  sql += name;

  return query(sql, travType, connected, accessPolicy);
}

std::auto_ptr<te::da::DataSet> te::ado::Transactor::getDataSet(const std::string& name,
                                                               const std::string& propertyName,
                                                               const te::gm::Envelope* e,
                                                               te::gm::SpatialRelation r,
                                                               te::common::TraverseType travType,
                                                               bool connected,
                                                               const te::common::AccessPolicy accessPolicy)
{
  if(e == 0)
    throw Exception(TR_ADO("The envelope is missing!"));

  std::string lowerX = "lower_x";
  std::string upperX = "upper_x";
  std::string lowerY = "lower_y";
  std::string upperY = "upper_y";

  std::string q("SELECT * FROM " + name + " WHERE ");

  q += "NOT("+ lowerX +" > " + boost::lexical_cast<std::string>(e->m_urx) + " OR ";
  q += upperX +" < " + boost::lexical_cast<std::string>(e->m_llx) + " OR ";
  q += lowerY +" > " + boost::lexical_cast<std::string>(e->m_ury) + " OR ";
  q += upperY +" < " + boost::lexical_cast<std::string>(e->m_lly) + ")";

  return query(q, travType, connected, accessPolicy);
}

std::auto_ptr<te::da::DataSet> te::ado::Transactor::getDataSet(const std::string& /*name*/,
                                                               const std::string& /*propertyName*/,
                                                               const te::gm::Geometry* /*g*/,
                                                               te::gm::SpatialRelation /*r*/,
                                                               te::common::TraverseType /*travType*/,
                                                               bool /*connected*/,
                                                               const te::common::AccessPolicy /*accessPolicy*/)
{
  throw Exception(TR_ADO("Method getDataSet by geometry filter: not implemented yet!"));
}

std::auto_ptr<te::da::DataSet> te::ado::Transactor::getDataSet(const std::string& /*name*/,
                                                               const ObjectIdSet* /*oids*/, 
                                                               te::common::TraverseType /*travType*/, 
                                                               bool /*connected*/,
                                                               const te::common::AccessPolicy /*accessPolicy*/)
{
  throw Exception(TR_ADO("Method getDataSet by oids: not implemented yet!"));
}

std::auto_ptr<te::da::DataSet> te::ado::Transactor::query(const te::da::Select& q,
                                                          te::common::TraverseType travType,
                                                          bool connected,
                                                          const te::common::AccessPolicy accessPolicy)
{
  std::string sql;

  SQLVisitor visitor(*(m_ds->getDialect()), sql, m_conn->getConn());

  q.accept(visitor);

  return query(sql, travType);
}

std::auto_ptr<te::da::DataSet> te::ado::Transactor::query(const std::string& query,
                                                          te::common::TraverseType travType,
                                                          bool connected,
                                                          const te::common::AccessPolicy accessPolicy)
{
  _RecordsetPtr result = m_conn->query(query, connected);

  long i = result->GetRecordCount();

  std::auto_ptr<te::da::DataSet> dset(new DataSet(this, result));

  if(connected)
  {
    return dset;
  }
  else
  {
    std::auto_ptr<te::da::DataSet> mdset(new te::mem::DataSet(*dset));

    return mdset;
  }
}

void te::ado::Transactor::execute(const te::da::Query& command)
{
  std::string sql;

  SQLVisitor visitor(*(m_ds->getDialect()), sql, m_conn->getConn());

  command.accept(visitor);

  execute(sql);
}

void te::ado::Transactor::execute(const std::string& command)
{
  m_conn->execute(command);
}

std::auto_ptr<te::da::PreparedQuery> te::ado::Transactor::getPrepared(const std::string& qName)
{
  throw Exception(TR_ADO("Method getPrepared: not implemented yet!"));
}

std::auto_ptr<te::da::BatchExecutor> te::ado::Transactor::getBatchExecutor()
{
  throw Exception(TR_ADO("Method getBatchExecutor: not implemented yet!"));
}

void te::ado::Transactor::cancel()
{
}

boost::int64_t te::ado::Transactor::getLastGeneratedId()
{
  throw Exception(TR_ADO("Method getLastGeneratedId: not implemented yet!"));
}

std::string te::ado::Transactor::escape(const std::string& value)
{
  return value;
}

bool te::ado::Transactor::isDataSetNameValid(const std::string& /*datasetName*/)
{
  return true;
}

bool te::ado::Transactor::isPropertyNameValid(const std::string& /*propertyName*/)
{
  return true;
}

std::vector<std::string> te::ado::Transactor::getDataSetNames()
{
  std::vector<std::string> datasets;

  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_conn->getConn()));

    ADOX::TablesPtr tables = pCatalog->GetTables();
    
    for(long i = 0; i < tables->GetCount(); ++i)
    {
      ADOX::_TablePtr table = tables->GetItem(i);
      std::string tableName = table->GetName();

      std::string tabletype = table->GetType();

      if(table->GetType() == _bstr_t("ACCESS TABLE") || 
         table->GetType() == _bstr_t("LINK") || 
         table->GetType() == _bstr_t("PASS-THROUGH") ||
         table->GetType() == _bstr_t("SYSTEM TABLE") ||
         table->GetType() == _bstr_t("VIEW") ||
         tableName == "geometry_columns")
         continue;

      datasets.push_back(std::string(table->GetName()));
    }
  }
  catch(_com_error &e)
  {
    throw Exception(TR_ADO(e.ErrorMessage()));
  }

  return datasets;
}

std::size_t te::ado::Transactor::getNumberOfDataSets()
{
  return getDataSetNames().size();
}

std::auto_ptr<te::da::DataSetType> te::ado::Transactor::getDataSetType(const std::string& name)
{
  std::auto_ptr<te::da::DataSetType> dt(new te::da::DataSetType(name));

  dt->setTitle(name);

  getProperties(dt.get());

  getPrimaryKey(dt.get());

  getUniqueKeys(dt.get());

  getIndexes(dt.get());

  getCheckConstraints(dt.get());

  return dt;
}

boost::ptr_vector<te::dt::Property> te::ado::Transactor::getProperties(const std::string& datasetName)
{
  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  std::vector<te::dt::Property*> dtProperties = dt->getProperties();

  boost::ptr_vector<te::dt::Property> properties;

  for(std::size_t i = 0; i < dtProperties.size(); ++i)
    properties.push_back(dtProperties[i]->clone());

  return properties;
}

std::auto_ptr<te::dt::Property> te::ado::Transactor::getProperty(const std::string& datasetName, const std::string& name)
{
  if(!propertyExists(datasetName, name))
    throw Exception((boost::format(TR_ADO("The dataset \"%1%\" has no property with this name \"%2%\"!")) % datasetName % name).str());

  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  return std::auto_ptr<te::dt::Property>(dt->getProperty(name)->clone());
}

std::auto_ptr<te::dt::Property> te::ado::Transactor::getProperty(const std::string& datasetName, std::size_t propertyPos)
{
  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  assert(propertyPos < dt->size());

  return std::auto_ptr<te::dt::Property>(dt->getProperty(propertyPos)->clone());
}

std::vector<std::string> te::ado::Transactor::getPropertyNames(const std::string& datasetName)
{
  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  std::vector<std::string> pNames;

  std::size_t numProperties = dt->size();

  for(std::size_t i = 0; i < numProperties; ++i)
    pNames.push_back(dt->getProperty(i)->getName());

  return pNames;
}

std::size_t te::ado::Transactor::getNumberOfProperties(const std::string& datasetName)
{
  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  return dt->size();
}

bool te::ado::Transactor::propertyExists(const std::string& datasetName, const std::string& name)
{
  std::vector<std::string> datasets;

  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_conn->getConn()));

    ADOX::TablesPtr tables = pCatalog->GetTables();

    ADOX::_TablePtr table;

    for(long i = 0; i < tables->GetCount(); ++i)
    {
      table = 0;
      table = tables->GetItem(i);
      std::string tableName = table->GetName();

      if(tableName == datasetName)
      {
        break;
      }
    }

    if(table)
    {
      ADOX::ColumnsPtr cols = table->GetColumns();

      for(int i = 0; i < cols->Count; ++i)
      {
        ADOX::_ColumnPtr col = cols->GetItem((long)i);

        if(((LPCSTR)(_bstr_t)col->GetName()) == name)
          return true;
      }
    }

    return false;

  }
  catch(_com_error &e)
  {
    throw Exception(TR_ADO(e.ErrorMessage()));
  }
}

void te::ado::Transactor::addProperty(const std::string& datasetName, te::dt::Property* p)
{
  const std::string& propertyName = p->getName();

  if(propertyExists(datasetName, propertyName))
    throw Exception((boost::format(TR_ADO("The dataset already \"%1%\" has a property with this name \"%2%\"!")) % datasetName % propertyName).str());

  ADOX::_CatalogPtr pCatalog = 0;

  ADOX::_TablePtr pTable = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_conn->getConn()));

    pTable = pCatalog->Tables->GetItem(datasetName.c_str());

    ADOX::_ColumnPtr newColumn = 0;

    TESTHR(newColumn.CreateInstance(__uuidof(ADOX::Column)));

    newColumn->PutName(propertyName.c_str());

    newColumn->PutType(te::ado::Convert2Ado(p->getType()));

    ADOX::DataTypeEnum ado_type = te::ado::Convert2Ado(p->getType());

    switch(p->getType())
    {
      case te::dt::CHAR_TYPE:
      case te::dt::UCHAR_TYPE:
      case te::dt::INT16_TYPE:
      case te::dt::INT32_TYPE:
      case te::dt::INT64_TYPE:
      case te::dt::FLOAT_TYPE:
      case te::dt::DOUBLE_TYPE:
      case te::dt::BOOLEAN_TYPE:
      case te::dt::BYTE_ARRAY_TYPE:
      case te::dt::ARRAY_TYPE:
      case te::dt::DATETIME_TYPE:
      case te::dt::NUMERIC_TYPE:
      {
        const te::dt::SimpleProperty* simple = static_cast<const te::dt::SimpleProperty*>(p);

        if(!simple->isRequired())
          newColumn->PutAttributes(ADOX::adColNullable);

        pTable->Columns->Append(_variant_t ((IDispatch*)newColumn), ado_type, 0);

        break;
      }

      case te::dt::STRING_TYPE:
      {
        const te::dt::StringProperty* sp = static_cast<const te::dt::StringProperty*>(p);

        long ssize = 0;

        if(sp->size() != 0)
          ssize = sp->size();

        newColumn->DefinedSize = ssize;

        if(!sp->isRequired())
          newColumn->PutAttributes(ADOX::adColNullable);

        pTable->Columns->Append(_variant_t ((IDispatch*)newColumn), ado_type, ssize);

        break;
      }

      case te::dt::GEOMETRY_TYPE:
      {
        const te::dt::SimpleProperty* simple = static_cast<const te::dt::SimpleProperty*>(p);

        if(!simple->isRequired())
          newColumn->PutAttributes(ADOX::adColNullable);

        pTable->Columns->Append(_variant_t ((IDispatch*)newColumn), ado_type, 0);

// update geometry columns and metadata cache
        std::auto_ptr<te::dt::SimpleProperty> lowerX(new te::dt::SimpleProperty("lower_x", te::dt::DOUBLE_TYPE));
        std::auto_ptr<te::dt::SimpleProperty> lowerY(new te::dt::SimpleProperty("lower_y", te::dt::DOUBLE_TYPE));
        std::auto_ptr<te::dt::SimpleProperty> upperX(new te::dt::SimpleProperty("upper_x", te::dt::DOUBLE_TYPE));
        std::auto_ptr<te::dt::SimpleProperty> upperY(new te::dt::SimpleProperty("upper_y", te::dt::DOUBLE_TYPE));

        addProperty(datasetName, lowerX.get());
        addProperty(datasetName, lowerY.get());
        addProperty(datasetName, upperX.get());
        addProperty(datasetName, upperY.get());

        insertIntoGeometryColumns(datasetName, static_cast<te::gm::GeometryProperty*>(p));

        break;
      }

      default:
        throw te::ado::Exception(TR_ADO("The informed type could not be mapped to ADO type system!"));
      break;
    }
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::Transactor::dropProperty(const std::string& datasetName, const std::string& name)
{
  if(!propertyExists(datasetName, name))
    throw Exception((boost::format(TR_ADO("The dataset \"%1%\" has no property with this name \"%2%\"!")) % datasetName % name).str());

  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  te::dt::Property* p = dt->getProperty(name);

  ADOX::_CatalogPtr pCatalog = 0;

  ADOX::_TablePtr pTable = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_conn->getConn()));

    pTable = pCatalog->Tables->GetItem(p->getParent()->getName().c_str());

    TESTHR(pTable->GetColumns()->Delete(p->getName().c_str()));

    if(p->getType() == te::dt::GEOMETRY_TYPE)
    {
      std::string sql  = "delete from geometry_columns where f_geometry_column = '";
                  sql += p->getName();
                  sql += "' AND f_table_name = '";
                  sql += datasetName;
                  sql +="'";

      m_conn->execute(sql);
    }
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::Transactor::renameProperty(const std::string& datasetName,
                                         const std::string& propertyName,
                                         const std::string& newPropertyName)
{
  ADOX::_CatalogPtr pCatalog = 0;

  ADOX::_TablePtr pTable = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_conn->getConn()));

    pTable = pCatalog->Tables->GetItem(datasetName.c_str());

    ADOX::_ColumnPtr col = pTable->GetColumns()->GetItem(propertyName.c_str());
  
    col->PutName(newPropertyName.c_str());

    pCatalog->GetTables()->Refresh();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

std::auto_ptr<te::da::PrimaryKey> te::ado::Transactor::getPrimaryKey(const std::string& datasetName)
{
  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  return std::auto_ptr<te::da::PrimaryKey>(static_cast<te::da::PrimaryKey*>(dt->getPrimaryKey()->clone()));
}

bool te::ado::Transactor::primaryKeyExists(const std::string& datasetName, const std::string& name)
{
  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  if(dt->getPrimaryKey()->getName() == name)
    return true;

  return false;
}

void te::ado::Transactor::addPrimaryKey(const std::string& datasetName, te::da::PrimaryKey* pk)
{
  _variant_t vOptional;
  vOptional.vt = VT_ERROR;
  vOptional.scode = DISP_E_PARAMNOTFOUND;

  ADOX::_KeyPtr pKey = 0;
  ADOX::_TablePtr pTable = 0;
  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));
  TESTHR(pKey.CreateInstance(__uuidof(ADOX::Key)));
  
  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_conn->getConn()));

    pTable = pCatalog->Tables->GetItem(datasetName.c_str());

    pKey->Name = pTable->Name + "_pk";
    pKey->Type = ADOX::adKeyPrimary;

    for(size_t i = 0; i < pk->getProperties().size(); i++)
    {
      te::dt::Property* p = pk->getProperties()[i];
      TESTHR(pKey->Columns->Append(p->getName().c_str(), te::ado::Convert2Ado(p->getType()), 256));

    }

    TESTHR(pTable->Keys->Append(_variant_t((IDispatch *)pKey),ADOX::adKeyPrimary,vOptional,L"",L""));
    pCatalog->Tables->Refresh();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::Transactor::dropPrimaryKey(const std::string& datasetName)
{
  ADOX::_KeyPtr pKey = 0;
  ADOX::_TablePtr pTable = 0;
  ADOX::_CatalogPtr pCatalog = 0;

  std::auto_ptr<te::da::PrimaryKey> pk(getPrimaryKey(datasetName));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_conn->getConn()));

    pTable = pCatalog->Tables->GetItem(datasetName.c_str());

    pKey = pTable->Keys->GetItem(pk->getName().c_str());

    TESTHR(pTable->GetKeys()->Delete(_variant_t((IDispatch *)pKey)));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

std::auto_ptr<te::da::ForeignKey> te::ado::Transactor::getForeignKey(const std::string& datasetName, const std::string& name)
{
  if(!foreignKeyExists(datasetName, name))
    throw Exception((boost::format(TR_ADO("The dataset \"%1%\" has no foreign key with this name \"%2%\"!")) % datasetName % name).str());

  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  return std::auto_ptr<te::da::ForeignKey>(static_cast<te::da::ForeignKey*>(dt->getForeignKey(name)->clone()));
}

std::vector<std::string> te::ado::Transactor::getForeignKeyNames(const std::string& datasetName)
{
  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  std::vector<std::string> fkNames;

  std::size_t numFK = dt->getNumberOfForeignKeys();

  for(std::size_t i = 0; i < numFK; ++i)
    fkNames.push_back(dt->getForeignKey(i)->getName());

  return fkNames;
}

bool te::ado::Transactor::foreignKeyExists(const std::string& datasetName, const std::string& name)
{
  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  std::vector<std::string> fkNames = getForeignKeyNames(datasetName);

  if(std::find(fkNames.begin(), fkNames.end(), name) != fkNames.end())
    return true;

  return false;
}

void te::ado::Transactor::addForeignKey(const std::string& datasetName, te::da::ForeignKey* fk)
{
  _variant_t vOptional;
  vOptional.vt = VT_ERROR;
  vOptional.scode = DISP_E_PARAMNOTFOUND;

  ADOX::_KeyPtr pKey = 0;
  ADOX::_TablePtr pTable = 0;
  ADOX::_CatalogPtr pCatalog = 0;

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_conn->getConn()));

    pTable = pCatalog->Tables->GetItem(datasetName.c_str());

    TESTHR(pKey.CreateInstance(__uuidof(ADOX::Key)));

    pKey->Name = pTable->Name + "_fk";
    pKey->Type = ADOX::adKeyForeign;
    pKey->RelatedTable = fk->getReferencedDataSetType()->getName().c_str();  

    for(size_t i = 0; i < fk->getProperties().size(); i++)
    {

      te::dt::Property* p = fk->getProperties()[i];

      TESTHR(pKey->Columns->Append(p->getName().c_str(), te::ado::Convert2Ado(p->getType()), 256));
      pKey->Columns->GetItem(p->getName().c_str())->RelatedColumn = fk->getReferencedProperties()[i]->getName().c_str();

    }

    TESTHR(pTable->Keys->Append(_variant_t((IDispatch *)pKey),ADOX::adKeyPrimary,vOptional,L"",L""));
  
    pCatalog->Tables->Refresh();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::Transactor::dropForeignKey(const std::string& datasetName, const std::string& fkName)
{
  ADOX::_KeyPtr pKey = 0;
  ADOX::_TablePtr pTable = 0;
  ADOX::_CatalogPtr pCatalog = 0;

  try
  {

    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_conn->getConn()));

    pTable = pCatalog->Tables->GetItem(datasetName.c_str());

    pKey = pTable->Keys->GetItem(fkName.c_str());

    TESTHR(pTable->GetKeys()->Delete(_variant_t((IDispatch *)pKey)));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

std::auto_ptr<te::da::UniqueKey> te::ado::Transactor::getUniqueKey(const std::string& datasetName, const std::string& name)
{
  if(!uniqueKeyExists(datasetName, name))
    throw Exception((boost::format(TR_ADO("The dataset \"%1%\" has no unique key with this name \"%2%\"!")) % datasetName % name).str());

  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  return std::auto_ptr<te::da::UniqueKey>(static_cast<te::da::UniqueKey*>(dt->getUniqueKey(name)->clone()));
}

std::vector<std::string> te::ado::Transactor::getUniqueKeyNames(const std::string& datasetName)
{
  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  std::vector<std::string> ukNames;

  std::size_t numUKs = dt->getNumberOfUniqueKeys();

  for(std::size_t i = 0; i < numUKs; ++i)
    ukNames.push_back(dt->getUniqueKey(i)->getName());

  return ukNames;
}

bool te::ado::Transactor::uniqueKeyExists(const std::string& datasetName, const std::string& name)
{
  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  std::vector<std::string> ukNames = getUniqueKeyNames(datasetName);

  if(std::find(ukNames.begin(), ukNames.end(), name) != ukNames.end())
    return true;

  return false;
}

void te::ado::Transactor::addUniqueKey(const std::string& datasetName, te::da::UniqueKey* uk)
{
  _variant_t vOptional;
  vOptional.vt = VT_ERROR;
  vOptional.scode = DISP_E_PARAMNOTFOUND;

  ADOX::_KeyPtr pKey = 0;
  ADOX::_TablePtr pTable = 0;
  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));
  TESTHR(pKey.CreateInstance(__uuidof(ADOX::Key)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_conn->getConn()));

    pTable = pCatalog->Tables->GetItem(datasetName.c_str());

    pKey->Name = pTable->Name + "_uk";
    pKey->Type = ADOX::adKeyUnique;

    for(size_t i = 0; i < uk->getProperties().size(); i++)
    {
      te::dt::Property* p = uk->getProperties()[i];

      TESTHR(pKey->Columns->Append(p->getName().c_str(), te::ado::Convert2Ado(p->getType()), 256));
    }

    TESTHR(pTable->Keys->Append(_variant_t((IDispatch *)pKey),ADOX::adKeyUnique,vOptional,L"",L""));

    pCatalog->Tables->Refresh();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::Transactor::dropUniqueKey(const std::string& datasetName, const std::string& name)
{
  ADOX::_KeyPtr pKey = 0;
  ADOX::_TablePtr pTable = 0;
  ADOX::_CatalogPtr pCatalog = 0;

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_conn->getConn()));

    pTable = pCatalog->Tables->GetItem(datasetName.c_str());

    pKey = pTable->Keys->GetItem(name.c_str());

    TESTHR(pTable->GetKeys()->Delete(_variant_t((IDispatch *)pKey)));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

std::auto_ptr<te::da::CheckConstraint> te::ado::Transactor::getCheckConstraint(const std::string& datasetName, const std::string& name)
{
  if(!checkConstraintExists(datasetName, name))
    throw Exception((boost::format(TR_ADO("The dataset \"%1%\" has no check constraint with this name \"%2%\"!")) % datasetName % name).str());

  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  return std::auto_ptr<te::da::CheckConstraint>(static_cast<te::da::CheckConstraint*>(dt->getCheckConstraint(name)->clone()));
}

std::vector<std::string> te::ado::Transactor::getCheckConstraintNames(const std::string& datasetName)
{
  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  std::vector<std::string> ccNames;

  std::size_t numCCs = dt->getNumberOfCheckConstraints();

  for(std::size_t i = 0; i < numCCs; ++i)
    ccNames.push_back(dt->getCheckConstraint(i)->getName());

  return ccNames;
}

bool te::ado::Transactor::checkConstraintExists(const std::string& datasetName, const std::string& name)
{
  std::vector<std::string> ccNames = getCheckConstraintNames(datasetName);

  if(std::find(ccNames.begin(), ccNames.end(), name) != ccNames.end())
    return true;

  return false;
}

void te::ado::Transactor::addCheckConstraint(const std::string& /*datasetName*/, te::da::CheckConstraint* /*cc*/)
{
  throw Exception(TR_ADO("Method addCheckConstraint: not implemented yet!"));
}

void te::ado::Transactor::dropCheckConstraint(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TR_ADO("Method dropCheckConstraint: not implemented yet!"));
}

std::auto_ptr<te::da::Index> te::ado::Transactor::getIndex(const std::string& datasetName, const std::string& name)
{
  if(!indexExists(datasetName, name))
    throw Exception((boost::format(TR_ADO("The dataset \"%1%\" has no index with this name \"%2%\"!")) % datasetName % name).str());

  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  return std::auto_ptr<te::da::Index>(dt->getIndex(name)->clone());
}

std::vector<std::string> te::ado::Transactor::getIndexNames(const std::string& datasetName)
{
  std::vector<std::string> idxNames;

  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  std::size_t numIdxs = dt->getNumberOfIndexes();

  for(std::size_t i = 0; i < numIdxs; ++i)
    idxNames.push_back(dt->getIndex(i)->getName());

  return idxNames;
}

bool te::ado::Transactor::indexExists(const std::string& datasetName, const std::string& name)
{
  std::vector<std::string> idxNames = getIndexNames(datasetName);

  if(std::find(idxNames.begin(), idxNames.end(), name) != idxNames.end())
    return true;

  return false;
}

void te::ado::Transactor::addIndex(const std::string& datasetName, te::da::Index* idx,
                                   const std::map<std::string, std::string>& options)
{
  ADOX::_IndexPtr pIndex = 0;
  ADOX::_TablePtr pTable = 0;
  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));
  TESTHR(pIndex.CreateInstance(__uuidof(ADOX::Index)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_conn->getConn()));

    pTable = pCatalog->Tables->GetItem(datasetName.c_str());

    pIndex->Name = idx->getName().c_str();

    std::vector<te::dt::Property*> idxProps = idx->getProperties();
    for(size_t i = 0; i < idxProps.size(); i++)
    {
      long size = 0;
      if(idxProps[i]->getType() == te::dt::STRING_TYPE)
        size = (long)((te::dt::StringProperty*)idxProps[i])->size();

      TESTHR(pIndex->Columns->Append(idxProps[i]->getName().c_str(), te::ado::Convert2Ado(idxProps[i]->getType()), size));
    }

    TESTHR(pTable->Indexes->Append(_variant_t((IDispatch *)pIndex)));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::Transactor::dropIndex(const std::string& datasetName, const std::string& idxName)
{
  if(!indexExists(datasetName, idxName))
    throw Exception((boost::format(TR_ADO("The dataset \"%1%\" has no index with this name: \"%2%\"!")) % datasetName % idxName).str());

  std::string sql("DROP INDEX ");
  sql += idxName;

  execute(sql);

  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  te::da::Index* idx = dt->getIndex(idxName);

  dt->remove(idx);
}

std::auto_ptr<te::da::Sequence> te::ado::Transactor::getSequence(const std::string& name)
{
  throw Exception(TR_ADO("Method getSequence: not implemented yet!"));
}

std::vector<std::string> te::ado::Transactor::getSequenceNames()
{
  throw Exception(TR_ADO("Method getSequenceNames: not implemented yet!"));
}

bool te::ado::Transactor::sequenceExists(const std::string& /*name*/)
{
  throw Exception(TR_ADO("Method sequenceExists: not implemented yet!"));
}

void te::ado::Transactor::addSequence(te::da::Sequence* /*sequence*/)
{
  throw Exception(TR_ADO("Method addSequence: not implemented yet!"));
}

void te::ado::Transactor::dropSequence(const std::string& /*name*/)
{
  throw Exception(TR_ADO("Method dropSequence: not implemented yet!"));
}

std::auto_ptr<te::gm::Envelope> te::ado::Transactor::getExtent(const std::string& datasetName,
                                                               const std::string& /*propertyName*/)
{
  if(!dataSetExists(datasetName))
    throw Exception(TR_ADO("The Data Set Type does not exist!"));

  std::string sql = "SELECT MIN(lower_x), MIN(lower_y), MAX(upper_x), MAX(upper_y) from " + datasetName;

  std::auto_ptr<te::da::DataSet> resultBox(query(sql));

  std::auto_ptr<te::gm::Envelope> env(new te::gm::Envelope());

  if(resultBox.get())
  {
    resultBox->moveFirst();
    env->m_llx = resultBox->getDouble(0);
    env->m_lly = resultBox->getDouble(1);
    env->m_urx = resultBox->getDouble(2);
    env->m_ury = resultBox->getDouble(3);
  }
  else
  {
    throw Exception(TR_ADO("Error when calculating the envelope!"));
  }

  return env;
}

std::auto_ptr<te::gm::Envelope> te::ado::Transactor::getExtent(const std::string& datasetName,
                                                               std::size_t /*propertyPos*/)
{
  return getExtent(datasetName, 0);
}

std::size_t te::ado::Transactor::getNumberOfItems(const std::string& datasetName)
{
  std::auto_ptr<te::da::DataSet> result(getDataSet(datasetName));

  return result->size();
}

bool te::ado::Transactor::hasDataSets()
{
  std::vector<std::string> datasetNames = getDataSetNames();

  if(datasetNames.empty())
    return false;

  return true;
}

bool te::ado::Transactor::dataSetExists(const std::string& name)
{
  std::vector<std::string> datasetNames = getDataSetNames();

  if(std::find(datasetNames.begin(), datasetNames.end(), name) != datasetNames.end())
    return true;

  return false;
}

void te::ado::Transactor::createDataSet(te::da::DataSetType* dt, const std::map<std::string, std::string>& options)
{
  ADOX::_TablePtr pTable = 0;
  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pTable.CreateInstance(__uuidof (ADOX::Table)));
  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_conn->getConn()));

    pTable->Name = dt->getName().c_str();

    TESTHR(pCatalog->Tables->Append(_variant_t((IDispatch*)pTable)));

  }
  catch(_com_error &e)
  {
    throw Exception(TR_ADO(e.ErrorMessage()));
  }

  std::size_t ncols = dt->size();

  for(std::size_t i = 0; i < ncols; ++i)
    addProperty(dt->getName(), dt->getProperty(i));

  if(dt->getPrimaryKey())
    addPrimaryKey(dt->getName(), dt->getPrimaryKey());
}

void te::ado::Transactor::cloneDataSet(const std::string& /*name*/,
                                       const std::string& /*cloneName*/,
                                       const std::map<std::string, std::string>& /*options*/)
{
  throw Exception(TR_ADO("Method cloneDataSet: not implemented yet!"));
}

void te::ado::Transactor::dropDataSet(const std::string& name)
{
  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_conn->getConn()));
    
    TESTHR(pCatalog->Tables->Delete(name.c_str()));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::Transactor::renameDataSet(const std::string& name, const std::string& newName)
{
  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->Tables->GetItem(name.c_str())->PutName(newName.c_str());
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::Transactor::add(const std::string& datasetName,
                              te::da::DataSet* d,
                              const std::map<std::string, std::string>& options,
                              std::size_t limit)
{
  _RecordsetPtr recset;
  TESTHR(recset.CreateInstance(__uuidof(Recordset)));

  try
  {
    TESTHR(recset->Open(_bstr_t(datasetName.c_str()),
      _variant_t((IDispatch*)m_conn->getConn(),true), adOpenKeyset, adLockOptimistic, adCmdTable));
    
    while(d->moveNext())
    {
      TESTHR(recset->AddNew());

      for(std::size_t i = 0; i < d->getNumProperties(); ++i)
      {

        std::string pname = d->getPropertyName(i);
        int pType = d->getPropertyDataType(i);

        if(d->isNull(i))
          continue;
        //{
        //  _variant_t var;
        //  var.ChangeType(VT_NULL, NULL);
        //  recset->GetFields()->GetItem(pname.c_str())->PutValue(var);
        //}
        

        switch(pType)
        {
          case te::dt::CHAR_TYPE:
            recset->GetFields()->GetItem(pname.c_str())->Value = (_bstr_t)d->getChar(pname.c_str());
            break;

          case te::dt::UCHAR_TYPE:
            recset->GetFields()->GetItem(pname.c_str())->Value = (_bstr_t)d->getUChar(pname.c_str());
            break;

          case te::dt::INT16_TYPE:
            recset->GetFields()->GetItem(pname.c_str())->Value = (_variant_t)d->getInt16(pname.c_str());
            break;

          case te::dt::INT32_TYPE:
            recset->GetFields()->GetItem(pname.c_str())->Value = (_variant_t)d->getInt32(pname.c_str());
            break;

          case te::dt::INT64_TYPE:
            recset->GetFields()->GetItem(pname.c_str())->Value = (_variant_t)d->getInt64(pname.c_str());
            break;

          case te::dt::NUMERIC_TYPE:
            {
              std::string sval = d->getNumeric(pname);
              if(sval.empty())
                sval = "0.0";

              double dval = boost::lexical_cast<double>(sval);
              recset->GetFields()->GetItem(pname.c_str())->Value = (_variant_t)(dval);
            }
            break;

          case te::dt::DATETIME_TYPE:
          { 
            std::auto_ptr<te::dt::DateTime> dtm(d->getDateTime(i));

            std::string dtt = te::ado::GetFormattedDateTime(dtm.get());

            recset->GetFields()->GetItem(pname.c_str())->Value = (_bstr_t)dtt.c_str();

            break;
          }

          case te::dt::FLOAT_TYPE:
            recset->GetFields()->GetItem(pname.c_str())->Value = (_variant_t)d->getFloat(pname.c_str());
            break;

          case te::dt::DOUBLE_TYPE:
            recset->GetFields()->GetItem(pname.c_str())->Value = (_variant_t)d->getDouble(pname.c_str());
            break;

          case te::dt::STRING_TYPE:
            recset->GetFields()->GetItem(pname.c_str())->Value = (_bstr_t)d->getString(pname.c_str()).c_str();
            break;
          
          case te::dt::BOOLEAN_TYPE:
            recset->GetFields()->GetItem(pname.c_str())->Value = (_variant_t)d->getBool(pname.c_str());
            break;

          case te::dt::BYTE_ARRAY_TYPE:
          {
            /*char * data = ((te::dt::ByteArray*)props[i])->getData();

            _variant_t var;
            te::ado::Blob2Variant(data, ((te::dt::ByteArray*)props[i])->bytesUsed(), var);

            recset->Fields->GetItem(props[i]->getName().c_str())->AppendChunk (var);

            break;*/
          }

          //case te::dt::ARRAY_TYPE:
          case te::dt::GEOMETRY_TYPE:
          {
            std::auto_ptr<te::gm::Geometry> geometry(d->getGeometry(pname));
            const te::gm::Envelope* env = geometry->getMBR();
            
            recset->GetFields()->GetItem("lower_x")->Value = (_variant_t)env->m_llx;
            recset->GetFields()->GetItem("lower_y")->Value = (_variant_t)env->m_lly;
            recset->GetFields()->GetItem("upper_x")->Value = (_variant_t)env->m_urx;
            recset->GetFields()->GetItem("upper_y")->Value = (_variant_t)env->m_ury;

            _variant_t var;
            Convert2Ado(geometry.get(), var);

            recset->Fields->GetItem(pname.c_str())->AppendChunk (var);
            
            break;
          }

          default:
            throw te::ado::Exception(TR_ADO("The informed type could not be mapped to ADO type system!"));
            break;
          }
      }

      TESTHR(recset->Update());

    }
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::Transactor::remove(const std::string& datasetName, const te::da::ObjectIdSet* oids)
{
  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_conn->getConn()));
    
    TESTHR(pCatalog->Tables->Delete(datasetName.c_str()));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::Transactor::update(const std::string& datasetName,
                                 te::da::DataSet* dataset,
                                 const std::vector<std::size_t>& properties,
                                 const te::da::ObjectIdSet* oids,
                                 const std::map<std::string, std::string>& options,
                                 std::size_t limit)
{
  //TODO
}

void te::ado::Transactor::optimize(const std::map<std::string, std::string>& opInfo)
{

}

void te::ado::Transactor::getPrimaryKey(te::da::DataSetType* dt)
{
  _ConnectionPtr adoConn = m_conn->getConn();

  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
  ADOX::TablesPtr tables = pCatalog->GetTables();

  ADOX::_TablePtr t = tables->GetItem(dt->getName().c_str());

  ADOX::KeysPtr keys = t->GetKeys();

  ADOX::_KeyPtr pk = 0;

  for(long i = 0; i < keys->Count; i++)
  {
    if(keys->GetItem(i)->GetType() == ADOX::adKeyPrimary)
      pk = keys->GetItem(i);
  }

  if(pk == 0)
  {
    dt->setPrimaryKey(0);
    return;
  }

  ADOX::ColumnsPtr cols = pk->GetColumns();

  te::da::PrimaryKey* tlPk = new te::da::PrimaryKey(std::string(pk->GetName()), dt);

  for(long i = 0; i < cols->GetCount(); i++)
    tlPk->add(dt->getProperty(std::string(cols->GetItem(i)->GetName())));
}

void te::ado::Transactor::getProperties(te::da::DataSetType* dt)
{
  std::string dsName = dt->getName();
  int numCols = 0;

  ADOX::DataTypeEnum colType;
  std::map<int, std::string> colNamesMap;
  std::map<int, ADOX::DataTypeEnum> colTypesMap;
  std::map<int, int> charLengthMap;
  std::map<int, bool> isRequiredMap;
  std::map<int, bool> hasDefaultMap;
  std::map<int, std::string> defaultValueMap;

  _ConnectionPtr conn = 0;

  try
  {
    HRESULT hr = S_OK;

    conn = m_conn->getConn();

    _RecordsetPtr rs = NULL;

    TESTHR(rs.CreateInstance(__uuidof(Recordset)));

    // Create a safearray which takes three elements,and pass it as 
    // the second parameter in the OpenSchema method.
    SAFEARRAY FAR* psa = NULL;
    SAFEARRAYBOUND rgsabound;
    _variant_t var[3];

    _variant_t  Array;
    rgsabound.lLbound = 0;
    rgsabound.cElements = 3;
    psa = SafeArrayCreate(VT_VARIANT, 1, &rgsabound);

    var[0].vt = VT_EMPTY;
    var[1].vt = VT_EMPTY;
    var[2] = dsName.c_str();

    // Fill the safe array.
    for(LONG i = 0; i < 3; ++i)
      hr = SafeArrayPutElement(psa, &i, &var[i]);

    Array.vt = VT_ARRAY | VT_VARIANT;
    Array.parray = psa;  

    rs = conn->OpenSchema(adSchemaColumns, &Array, vtMissing);

    int pos;
    while (!(rs->EndOfFile))
    {
      // Get the column name
      _bstr_t columnName = rs->Fields->GetItem("COLUMN_NAME")->Value;
      pos = rs->Fields->GetItem("ORDINAL_POSITION")->Value;
      pos = pos - 1;
      colNamesMap[pos] = (LPCSTR)columnName;

      // Get the data type of the column
      colType = ADOX::DataTypeEnum(int(rs->Fields->GetItem("DATA_TYPE")->Value));
      colTypesMap[pos] = colType;

      // Get the length of the column
      _variant_t length = rs->Fields->GetItem("CHARACTER_MAXIMUM_LENGTH")->Value;
      int charLength = 0;
      if(length.vt != VT_NULL)
        charLength = (int)length.dblVal;
      charLengthMap[pos] = charLength;

      // Get the columns that accept null values
      bool nullVal = rs->Fields->GetItem("IS_NULLABLE")->Value;
      isRequiredMap[pos] = !nullVal;

      // Get the columns that has default values
      bool hasDefault = rs->Fields->GetItem("COLUMN_HASDEFAULT")->Value;
      hasDefaultMap[pos] = !hasDefault;

      // Get the default value
      std::string defaultStr;
      if(hasDefault)
      {
        _bstr_t defaultValue = rs->Fields->GetItem("COLUMN_DEFAULT")->Value;
         defaultStr = (LPSTR)defaultValue;
      }
        
      defaultValueMap[pos] = defaultStr;

      rs->MoveNext();
      ++numCols;
    }
  }
  catch (_com_error& e)
  {
    std::cout << "Error = " << (char*) e.ErrorMessage() << std::endl;
  }

  // Create the dataset properties
  for(int i = 0; i < numCols; ++i)
  {
    te::dt::Property* p = 0;
    ADOX::DataTypeEnum colType = colTypesMap[i];
    std::string colName = colNamesMap[i];

    switch(colType)
    {
      case ::adBoolean:
      {
        p = new te::dt::SimpleProperty(colName, Convert2Terralib(colType));
        te::dt::SimpleProperty* sp = (te::dt::SimpleProperty*)p;

        sp->setRequired(isRequiredMap[i]);
        break;
      }

      case ::adVarWChar:
      case ::adWChar:
      case ::adVarChar:
      case ::adLongVarChar:
      case ::adLongVarWChar:
      case ::adBSTR:
      case ::adChar:
      {
        p = new te::dt::StringProperty(colName, (te::dt::StringType)Convert2Terralib(colType), charLengthMap[i]);
        te::dt::StringProperty* sp = (te::dt::StringProperty*)p;
        
        sp->setRequired(isRequiredMap[i]);
        sp->setSize(charLengthMap[i]);

        break;
      }

      case ADOX::adTinyInt:
      case ADOX::adSmallInt:
      case ADOX::adInteger:
      case ADOX::adBigInt:
      case ADOX::adSingle:
      case ADOX::adDouble:
      case ADOX::adDecimal:
      case ::adUnsignedBigInt:
      case ::adUnsignedInt:
      case ::adUnsignedSmallInt:
      case ::adUnsignedTinyInt:
      {
        p = new te::dt::SimpleProperty(colName, Convert2Terralib(colType));
        te::dt::SimpleProperty* sp = (te::dt::SimpleProperty*)p;
        
        sp->setRequired(isRequiredMap[i]);
        break;
      }

      case ADOX::adBinary:
      case ADOX::adLongVarBinary:
      {
        
        std::map<std::string, std::string> geomColumns = m_ds->getGeomColumns();
        std::map<std::string, std::string>::iterator it = geomColumns.find(dsName);

        if(it != geomColumns.end())
        {
          if(it->second == colName)
          {
            p = new te::gm::GeometryProperty(colName, te::ado::GetSRID(conn, dsName, colName), te::ado::GetType(conn, dsName, colName));
          }
        }
        else
        {
          p = new te::dt::SimpleProperty(colName, Convert2Terralib(colType));
        }

        break;
      }

      case ADOX::adDate:
      case ADOX::adDBDate:
      case ADOX::adDBTime:
      case ADOX::adDBTimeStamp:
        p = new te::dt::DateTimeProperty(colName, te::dt::TIME_INSTANT);
        break;
          
      default:
        p = new te::dt::SimpleProperty(colName, te::dt::UNKNOWN_TYPE);
        break;
    }

    dt->add(p);
  }
}

void te::ado::Transactor::getUniqueKeys(te::da::DataSetType* dt)
{
  _ConnectionPtr adoConn = m_conn->getConn();

  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  ADOX::TablesPtr tables = pCatalog->GetTables();

  ADOX::_TablePtr t = tables->GetItem(dt->getName().c_str());

  ADOX::KeysPtr keys = t->GetKeys();  

  for(long i = 0; i < keys->Count; i++)
  {
    if(keys->GetItem(i)->GetType() == ADOX::adKeyUnique)
    {
      ADOX::_KeyPtr uk = keys->GetItem(i);
      
      te::da::UniqueKey* tlUk = new te::da::UniqueKey(std::string(uk->GetName()), dt);

      ADOX::ColumnsPtr cols = uk->GetColumns();

      for(long j = 0; j < cols->Count; j++)
        tlUk->add(dt->getProperty(std::string(cols->GetItem(i)->GetName())));
    }
  } 
}

void te::ado::Transactor::getIndexes(te::da::DataSetType* dt)
{
  _ConnectionPtr adoConn = m_conn->getConn();

  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR((pCatalog.CreateInstance(__uuidof(ADOX::Catalog))));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  ADOX::TablesPtr tables = pCatalog->GetTables();

  ADOX::_TablePtr t = tables->GetItem(dt->getName().c_str());

  ADOX::IndexesPtr idxs = t->GetIndexes();

  for(long i = 0; i < idxs->GetCount(); i++)
  {
    ADOX::_IndexPtr idx = idxs->GetItem(i);

    te::da::Index* tlIdx = new te::da::Index();
    tlIdx->setName(std::string(idx->GetName()));

    std::vector<te::dt::Property*> props;

    ADOX::ColumnsPtr cols = idx->GetColumns();
    for(long i = 0; i < cols->GetCount(); i++)
      props.push_back(dt->getProperty(std::string(cols->GetItem(i)->GetName())));
    
    tlIdx->setProperties(props);

    dt->add(tlIdx);
  } 
}

void te::ado::Transactor::getCheckConstraints(te::da::DataSetType* dt)
{
  _RecordsetPtr rs = NULL;

  std::string dtName = dt->getName();

  std::string str = "[" + dtName + "]";

  try
  {
    HRESULT hr = S_OK;

    _ConnectionPtr adoConn = m_conn->getConn();

    TESTHR(rs.CreateInstance(__uuidof(Recordset)));

    rs = adoConn->OpenSchema(adSchemaCheckConstraints);

    while (!(rs->EndOfFile))
    {
      std::string constraintName = (LPCSTR)(bstr_t)(rs->Fields->GetItem("CONSTRAINT_NAME")->GetValue());
      
      if(constraintName.find(str) != std::string::npos)
      {
        te::da::CheckConstraint* cc = new te::da::CheckConstraint(constraintName, dt);
        std::string checkClause = (LPCSTR)(bstr_t)(rs->Fields->GetItem("CHECK_CLAUSE")->GetValue());
        cc->setExpression(checkClause);
      }

      rs->MoveNext();         
    }
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  // Clean up objects before exit.
  if(rs && rs->State == adStateOpen)
   rs->Close();
}

void te::ado::Transactor::insertIntoGeometryColumns(const std::string& datasetName,
                                                    te::gm::GeometryProperty* geomProp)
{
  _ConnectionPtr adoConn = m_conn->getConn();

  int coord_dimension = 2;

  if(te::ado::IsZProperty(geomProp->getGeometryType()))
    coord_dimension = 3;

  _RecordsetPtr recset;
  TESTHR(recset.CreateInstance(__uuidof(Recordset)));

  try
  {
    TESTHR(recset->Open(_bstr_t("geometry_columns"),
           _variant_t((IDispatch*)adoConn,true), adOpenKeyset, adLockOptimistic, adCmdTable));

    TESTHR(recset->AddNew());

    recset->GetFields()->GetItem("f_table_catalog")->Value = (_bstr_t)std::string("''").c_str();
    recset->GetFields()->GetItem("f_table_schema")->Value = (_bstr_t)std::string("public").c_str();
    recset->GetFields()->GetItem("f_table_name")->Value = (_bstr_t)datasetName.c_str();
    recset->GetFields()->GetItem("f_geometry_column")->Value = (_bstr_t)geomProp->getName().c_str();
    recset->GetFields()->GetItem("coord_dimension")->Value = (_variant_t)coord_dimension;
    recset->GetFields()->GetItem("srid")->Value = (_variant_t)geomProp->getSRID();
    recset->GetFields()->GetItem("type")->Value = (_bstr_t)te::ado::GetGeometryName(geomProp->getGeometryType()).c_str();

    recset->Update();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  m_ds->registerGeometryColumn(datasetName, geomProp->getName());
}

