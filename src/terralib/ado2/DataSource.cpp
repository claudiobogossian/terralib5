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
  \file terralib/ado2/DataSource.cpp

  \brief ????
*/

// TerraLib
//#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess2/dataset/CheckConstraint.h"
#include "../dataaccess2/dataset/DataSet.h"
#include "../dataaccess2/dataset/ForeignKey.h"
#include "../dataaccess2/dataset/Index.h"
#include "../dataaccess2/dataset/PrimaryKey.h"
#include "../dataaccess2/dataset/Sequence.h"
#include "../dataaccess2/dataset/UniqueKey.h"
#include "../dataaccess2/datasource/DataSourceCatalog.h"
#include "../dataaccess2/query/Select.h"
#include "../dataaccess2/query/SQLDialect.h"
#include "../dataaccess2/utils/Utils.h"
//#include "../datatype/Array.h"
//#include "../datatype/Property.h"
//#include "../datatype/SimpleData.h"
#include "../datatype/DateTimeProperty.h"
#include "../datatype/StringProperty.h"
#include "../geometry/GeometryProperty.h"
//#include "../raster/Grid.h"
//#include "../raster/BandProperty.h"
//#include "../raster/RasterProperty.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "Connection.h"
#include "DataSource.h"
#include "DataSet.h"
//#include "DataTypes.h"
#include "Exception.h"
#include "Globals.h"
//#include "SQLVisitor.h"
#include "Utils.h"

// STL
#include <cassert>
//#include <cstring>
#include <memory>

// Boost
//#include <boost/algorithm/string/case_conv.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
//#include <boost/thread.hpp>

inline void TESTHR(HRESULT hr)
{
  if(FAILED(hr))
    _com_issue_error(hr);
}

class te::ado::DataSource::Impl
{
public:

  Impl(te::ado::DataSource* ds)
    : m_ds(ds),
    m_catalog(0),
    m_conn(0),
    m_isInTransaction(false)
  {
    m_catalog = new te::da::DataSourceCatalog;
    m_catalog->setDataSource(m_ds);
  }

  ~Impl()
  {
    delete m_catalog;
  }

  te::ado::DataSource* m_ds;                       //!< The data source which this class implements.
  te::da::DataSourceCatalog* m_catalog;             //!< The main system catalog.
  std::map<std::string, std::string> m_connInfo;    //!< Connection information.
  Connection* m_conn;                               //!< The connection to the ADO data source.
  bool m_timeIsInteger;                             //!< It indicates if the ADO stores, internally, time and timestamp as an integer. 
  bool m_isInTransaction;                           //!< It indicates if there is a transaction in progress.
  std::vector<std::string> m_datasetNames;          //!< The list of dataset names of the data source.

  static te::da::DataSourceCapabilities sm_capabilities;  //!< ADO capabilities.
  static te::da::SQLDialect* sm_dialect;                  //!< ADO SQL dialect.
};


te::da::DataSourceCapabilities te::ado::DataSource::Impl::sm_capabilities;
te::da::SQLDialect* te::ado::DataSource::Impl::sm_dialect(0);

te::ado::DataSource::DataSource()
  : m_pImpl(0)
{
  m_pImpl = new Impl(this);
}

te::ado::DataSource::~DataSource()
{
  delete m_pImpl;
}

std::string te::ado::DataSource::getType() const
{
  return ADO_DRIVER_IDENTIFIER;
}

const std::map<std::string, std::string>& te::ado::DataSource::getConnectionInfo() const
{
  return m_pImpl->m_connInfo;
}

void te::ado::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_pImpl->m_connInfo = connInfo;
}

bool te::ado::DataSource::isTimeAnInteger()
{
  return m_pImpl->m_timeIsInteger;
}

void te::ado::DataSource::setTimeAsInteger(bool timeIsInteger)
{
  m_pImpl->m_timeIsInteger = timeIsInteger;
}

void te::ado::DataSource::open()
{
  // Assure we are in a closed state
  close();

  std::string connInfo = MakeConnectionStr(m_pImpl->m_connInfo);

  m_pImpl->m_conn = new te::ado::Connection(connInfo);

  // Get the dataset names of the data source
  getDataSetNames();
}

void te::ado::DataSource::close()
{
  if(m_pImpl->m_conn)
    delete m_pImpl->m_conn;
}

bool te::ado::DataSource::isOpened() const
{
  return m_pImpl->m_conn->isValid();
}

bool te::ado::DataSource::isValid() const
{
  return m_pImpl->m_conn->isValid();
}

const te::da::DataSourceCapabilities& te::ado::DataSource::getCapabilities() const
{
  return Impl::sm_capabilities;
}

const te::da::SQLDialect* te::ado::DataSource::getDialect() const
{
  return Impl::sm_dialect;
}

void te::ado::DataSource::begin()
{
  try
  {
    m_pImpl->m_conn->getConn()->BeginTrans();
    m_pImpl->m_isInTransaction = true;
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::DataSource::commit()
{
  try
  {
    m_pImpl->m_conn->getConn()->CommitTrans();
    m_pImpl->m_isInTransaction = false;
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::DataSource::rollBack()
{
  try
  {
    m_pImpl->m_conn->getConn()->RollbackTrans();
    m_pImpl->m_isInTransaction = false;
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

bool te::ado::DataSource::isInTransaction() const
{
  return m_pImpl->m_isInTransaction;
}

std::auto_ptr<te::da::DataSet> te::ado::DataSource::getDataSet(const std::string& name, 
                                                               te::common::TraverseType travType)
{
  std::auto_ptr<std::string> sql(new std::string("SELECT * FROM "));
  *sql += name;

  _RecordsetPtr result = m_pImpl->m_conn->query(*sql);

  return std::auto_ptr<te::da::DataSet>(new DataSet(result, m_pImpl->m_conn, sql.release()));
}

std::auto_ptr<te::da::DataSet> te::ado::DataSource::getDataSet(const std::string& name,
                                                               const std::string& propertyName,
                                                               const te::gm::Envelope* e,
                                                               te::gm::SpatialRelation r,
                                                               te::common::TraverseType travType)
{
  if(e == 0)
    throw Exception(TR_ADO("The envelope is missing!"));

  std::string lowerX = "lower_x";
  std::string upperX = "upper_x";
  std::string lowerY = "lower_y";
  std::string upperY = "upper_y";

  std::auto_ptr<std::string> query(new std::string("SELECT * FROM " + name + " WHERE "));

  *query += "NOT("+ lowerX +" > " + boost::lexical_cast<std::string>(e->m_urx) + " OR ";
  *query += upperX +" < " + boost::lexical_cast<std::string>(e->m_llx) + " OR ";
  *query += lowerY +" > " + boost::lexical_cast<std::string>(e->m_ury) + " OR ";
  *query += upperY +" < " + boost::lexical_cast<std::string>(e->m_lly) + ")";

  _RecordsetPtr result = m_pImpl->m_conn->query(*query);

  return std::auto_ptr<te::da::DataSet>(new DataSet(result, m_pImpl->m_conn, query.release()));
}

std::auto_ptr<te::da::DataSet> te::ado::DataSource::getDataSet(const std::string& name,
                                                               const std::string& propertyName,
                                                               const te::gm::Geometry* g,
                                                               te::gm::SpatialRelation r,
                                                               te::common::TraverseType travType)
{
  return std::auto_ptr<te::da::DataSet>(0);
}

std::auto_ptr<te::da::DataSet> te::ado::DataSource::query(const te::da::Select& q,
                                                          te::common::TraverseType travType)
{
  std::string sql;

  //SQLVisitor visitor(*(getDialect()), sql, m_pImpl->m_conn->getConn());
  //q.accept(visitor);

  return query(sql, travType);
}

std::auto_ptr<te::da::DataSet> te::ado::DataSource::query(const std::string& query, 
                                                          te::common::TraverseType travType)
{
  _RecordsetPtr result = m_pImpl->m_conn->query(query);

  return std::auto_ptr<te::da::DataSet>(new DataSet(result, m_pImpl->m_conn, new std::string(query)));
}

void te::ado::DataSource::execute(const te::da::Query& command)
{
  std::string sql;

  //SQLVisitor visitor(*(getDialect()), sql, m_pImpl->m_conn->getConn());
  //command.accept(visitor);

  execute(sql);
}

void te::ado::DataSource::execute(const std::string& command)
{
  m_pImpl->m_conn->execute(command);
}

void te::ado::DataSource::cancel()
{
}

boost::int64_t te::ado::DataSource::getLastGeneratedId()
{
  return 0;
}

std::string te::ado::DataSource::escape(const std::string& value)
{
  return value;
}

bool te::ado::DataSource::isDataSetNameValid(const std::string& /*datasetName*/)
{
  return true;
}

bool te::ado::DataSource::isPropertyNameValid(const std::string& /*propertyName*/)
{
  return true;
}

std::vector<std::string> te::ado::DataSource::getDataSetNames()
{
  std::vector<std::string> datasets;

  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_pImpl->m_conn));

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

const te::da::DataSetTypePtr& te::ado::DataSource::getDataSetType(const std::string& name)
{
  if(!dataSetExists(name))
    throw Exception((boost::format(TR_ADO("The dataset \"%1%\" doesn't exist!")) % name).str());

  // If the DataSetType exists in the catalog, get it from there.
  if(m_pImpl->m_catalog->datasetExists(name))
    return m_pImpl->m_catalog->getDataSetType(name);

  // Create the dataset type
  te::da::DataSetTypePtr dt(new te::da::DataSetType(name));
  dt->setTitle(name);

  // Get the properties of the dataset and add them to its schema
  getProperties(dt);

  // Get the indexes of the dataset and add them to its schema
  getIndexes(dt);

  // Add the dataset schema to the catalog
  m_pImpl->m_catalog->add(dt);

  // Try to link the primary key to an index
  std::vector<std::string> indexNames = getIndexNames(name);

  te::da::PrimaryKey* pk = dt->getPrimaryKey();
  if(pk)
  {
    for(std::size_t i = 0; i < indexNames.size(); ++i)
    {
      if(pk->getName() == indexNames[i])
      {
        pk->setAssociatedIndex(dt->getIndex(indexNames[i]));
        break;
      }
    }
  }

  // Try to link the unique keys to an index
  std::size_t numUKs = dt->getNumberOfUniqueKeys();

  for(std::size_t i = 0; i < numUKs; ++i)
  {
    te::da::UniqueKey* uk = dt->getUniqueKey(i);

    for(std::size_t j = 0; j < indexNames.size(); ++j)
    {
      if(uk->getName() == indexNames[j])
      {
        uk->setAssociatedIndex(dt->getIndex(indexNames[j]));
        break;
      }
    }
  }

  return m_pImpl->m_catalog->getDataSetType(name);
}

std::vector<std::string> te::ado::DataSource::getPropertyNames(const std::string& datasetName)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  std::vector<std::string> pNames;

  std::size_t numProperties = dt->size();

  for(std::size_t i = 0; i < numProperties; ++i)
    pNames.push_back(dt->getProperty(i)->getName());

  return pNames;
}

std::size_t te::ado::DataSource::getNumberOfProperties(const std::string& datasetName)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  return dt->size();
}

bool te::ado::DataSource::propertyExists(const std::string& datasetName, const std::string& name)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  std::vector<std::string> pNames = getPropertyNames(datasetName);

  if(std::find(pNames.begin(), pNames.end(), name) != pNames.end())
    return true;

  return false;
}

boost::ptr_vector<te::dt::Property> te::ado::DataSource::getProperties(const std::string& datasetName)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  std::vector<te::dt::Property*>& dtProperties = dt->getProperties();

  boost::ptr_vector<te::dt::Property> properties;

  for(std::size_t i = 0; i < dtProperties.size(); ++i)
    properties.push_back(dtProperties[i]);

  return properties;
}

te::dt::Property* te::ado::DataSource::getProperty(const std::string& datasetName, const std::string& name)
{
  if(!propertyExists(datasetName, name))
    throw Exception((boost::format(TR_ADO("The dataset \"%1%\" has no property with this name \"%2%\"!")) % datasetName % name).str());

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  return dt->getProperty(name)->clone();
}

te::dt::Property* te::ado::DataSource::getProperty(const std::string& datasetName, std::size_t propertyPos)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  assert(propertyPos < dt->size());

  return dt->getProperty(propertyPos)->clone();
}

void te::ado::DataSource::addProperty(const std::string& datasetName, te::dt::Property* p)
{
  std::string name = p->getName();
  if(propertyExists(datasetName, name))
    throw Exception((boost::format(TR_ADO("The dataset already \"%1%\" has a property with this name \"%2%\"!")) % datasetName % name).str());

  ADOX::_CatalogPtr pCatalog = 0;
  ADOX::_TablePtr pTable = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_pImpl->m_conn));

    pTable = pCatalog->Tables->GetItem(datasetName.c_str());
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  te::ado::AddAdoPropertyFromTerralib(pTable, p);
}

void te::ado::DataSource::dropProperty(const std::string& datasetName, const std::string& propertyName)
{
  if(!propertyExists(datasetName, propertyName))
    throw Exception((boost::format(TR_ADO("The dataset \"%1%\" has no property with this name \"%2%\"!")) % datasetName % propertyName).str());

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  te::dt::Property* p = dt->getProperty(propertyName);

  ADOX::_CatalogPtr pCatalog = 0;
  ADOX::_TablePtr pTable = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_pImpl->m_conn));

    pTable = pCatalog->Tables->GetItem(p->getParent()->getName().c_str());
    TESTHR(pTable->GetColumns()->Delete(p->getName().c_str()));

    if(p->getType() == te::dt::GEOMETRY_TYPE)
    {
      std::string sql = "drop from geometry_columns where f_geometry_column = '"+ p->getName() +"'";
      m_pImpl->m_conn->execute(sql);
    }
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::DataSource::renameProperty(const std::string& datasetName, const std::string& propertyName, const std::string& newPropertyName)
{
  ADOX::_CatalogPtr pCatalog = 0;
  ADOX::_TablePtr pTable = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_pImpl->m_conn));

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

te::da::PrimaryKey* te::ado::DataSource::getPrimaryKey(const std::string& datasetName)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  return static_cast<te::da::PrimaryKey*>(dt->getPrimaryKey()->clone());
}

bool te::ado::DataSource::primaryKeyExists(const std::string& datasetName, const std::string& name)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  if(dt->getPrimaryKey()->getName() == name)
    return true;

  return false;
}

void te::ado::DataSource::addPrimaryKey(const std::string& datasetName, te::da::PrimaryKey* pk)
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
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_pImpl->m_conn));

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

void te::ado::DataSource::dropPrimaryKey(const std::string& datasetName)
{
  ADOX::_KeyPtr pKey = 0;
  ADOX::_TablePtr pTable = 0;
  ADOX::_CatalogPtr pCatalog = 0;

  te::da::PrimaryKey* pk = getPrimaryKey(datasetName);

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_pImpl->m_conn));

    pTable = pCatalog->Tables->GetItem(datasetName.c_str());

    pKey = pTable->Keys->GetItem(pk->getName().c_str());

    TESTHR(pTable->GetKeys()->Delete(_variant_t((IDispatch *)pKey)));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

std::vector<std::string> te::ado::DataSource::getForeignKeyNames(const std::string& datasetName)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  std::vector<std::string> fkNames;

  std::size_t numFK = dt->getNumberOfForeignKeys();

  for(std::size_t i = 0; i < numFK; ++i)
    fkNames.push_back(dt->getForeignKey(i)->getName());

  return fkNames;
}

bool te::ado::DataSource::foreignKeyExists(const std::string& datasetName, const std::string& name)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  std::vector<std::string> fkNames = getForeignKeyNames(datasetName);

  if(std::find(fkNames.begin(), fkNames.end(), name) != fkNames.end())
    return true;

  return false;
}

te::da::ForeignKey* te::ado::DataSource::getForeignKey(const std::string& datasetName, const std::string& name)
{
  if(!foreignKeyExists(datasetName, name))
    throw Exception((boost::format(TR_ADO("The dataset \"%1%\" has no foreign key with this name \"%2%\"!")) % datasetName % name).str());

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  return static_cast<te::da::ForeignKey*>(dt->getForeignKey(name)->clone());
}

void te::ado::DataSource::addForeignKey(const std::string& datasetName, te::da::ForeignKey* fk)
{
 _variant_t vOptional;
  vOptional.vt = VT_ERROR;
  vOptional.scode = DISP_E_PARAMNOTFOUND;

  ADOX::_KeyPtr pKey = 0;
  ADOX::_TablePtr pTable = 0;
  ADOX::_CatalogPtr pCatalog = 0;

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_pImpl->m_conn));

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

void te::ado::DataSource::dropForeignKey(const std::string& datasetName, const std::string& fkName)
{
  ADOX::_KeyPtr pKey = 0;
  ADOX::_TablePtr pTable = 0;
  ADOX::_CatalogPtr pCatalog = 0;

  try
  {

    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_pImpl->m_conn));

    pTable = pCatalog->Tables->GetItem(datasetName.c_str());

    pKey = pTable->Keys->GetItem(fkName.c_str());

    TESTHR(pTable->GetKeys()->Delete(_variant_t((IDispatch *)pKey)));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

std::vector<std::string> te::ado::DataSource::getUniqueKeyNames(const std::string& datasetName)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  std::vector<std::string> ukNames;

  std::size_t numUKs = dt->getNumberOfUniqueKeys();

  for(std::size_t i = 0; i < numUKs; ++i)
    ukNames.push_back(dt->getUniqueKey(i)->getName());

  return ukNames;
}

bool te::ado::DataSource::uniqueKeyExists(const std::string& datasetName, const std::string& name)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  std::vector<std::string> ukNames = getUniqueKeyNames(datasetName);

  if(std::find(ukNames.begin(), ukNames.end(), name) != ukNames.end())
    return true;

  return false;
}

te::da::UniqueKey* te::ado::DataSource::getUniqueKey(const std::string& datasetName, const std::string& name)
{
  if(!uniqueKeyExists(datasetName, name))
    throw Exception((boost::format(TR_ADO("The dataset \"%1%\" has no unique key with this name \"%2%\"!")) % datasetName % name).str());

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  return static_cast<te::da::UniqueKey*>(dt->getUniqueKey(name)->clone());
}

void te::ado::DataSource::addUniqueKey(const std::string& datasetName, te::da::UniqueKey* uk)
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
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_pImpl->m_conn));

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

void te::ado::DataSource::dropUniqueKey(const std::string& datasetName, const std::string& name)
{
  ADOX::_KeyPtr pKey = 0;
  ADOX::_TablePtr pTable = 0;
  ADOX::_CatalogPtr pCatalog = 0;

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_pImpl->m_conn));

    pTable = pCatalog->Tables->GetItem(datasetName.c_str());

    pKey = pTable->Keys->GetItem(name.c_str());

    TESTHR(pTable->GetKeys()->Delete(_variant_t((IDispatch *)pKey)));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

//std::vector<std::string> te::ado::DataSource::getCheckConstraintNames(const std::string& datasetName)
//{
//  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);
//
//  std::vector<std::string> ccNames;
//
//  std::size_t numCCs = dt->getNumberOfCheckConstraints();
//
//  for(std::size_t i = 0; i < numCCs; ++i)
//    ccNames.push_back(dt->getCheckConstraint(i)->getName());
//
//  return ccNames;
//}
//
//bool te::ado::DataSource::checkConstraintExists(const std::string& datasetName, const std::string& name)
//{
//  std::vector<std::string> ccNames = getCheckConstraintNames(datasetName);
//
//  if(std::find(ccNames.begin(), ccNames.end(), name) != ccNames.end())
//    return true;
//
//  return false;
//}
//
//te::da::CheckConstraint* te::ado::DataSource::getCheckConstraint(const std::string& datasetName, const std::string& name)
//{
//  if(!checkConstraintExists(datasetName, name))
//    throw Exception((boost::format(TR_ADO("The dataset \"%1%\" has no check constraint with this name \"%2%\"!")) % datasetName % name).str());
//
//  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);
//
//  return static_cast<te::da::CheckConstraint*>(dt->getCheckConstraint(name)->clone());
//}
//
//void te::ado::DataSource::addCheckConstraint(const std::string& datasetName, te::da::CheckConstraint* cc)
//{
//  //TODO: Parece que não existe CheckConstraint no ADO.
//}
//
//void te::ado::DataSource::dropCheckConstraint(const std::string& datasetName, const std::string& name)
//{
//  //TODO: Parece que não existe CheckConstraint no ADO.
//}

std::vector<std::string> te::ado::DataSource::getIndexNames(const std::string& datasetName)
{
  std::vector<std::string> idxNames;

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  std::size_t numIdxs = dt->getNumberOfIndexes();

  for(std::size_t i = 0; i < numIdxs; ++i)
    idxNames.push_back(dt->getIndex(i)->getName());

  return idxNames;
}

bool te::ado::DataSource::indexExists(const std::string& datasetName, const std::string& name)
{
  std::vector<std::string> idxNames = getIndexNames(datasetName);

  if(std::find(idxNames.begin(), idxNames.end(), name) != idxNames.end())
    return true;

  return false;
}

te::da::Index* te::ado::DataSource::getIndex(const std::string& datasetName, const std::string& name)
{
  if(!indexExists(datasetName, name))
    throw Exception((boost::format(TR_ADO("The dataset \"%1%\" has no index with this name \"%2%\"!")) % datasetName % name).str());

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  return dt->getIndex(name)->clone();
}

void te::ado::DataSource::addIndex(const std::string& datasetName, te::da::Index* idx, const std::map<std::string, std::string>& options)
{
  ADOX::_IndexPtr pIndex = 0;
  ADOX::_TablePtr pTable = 0;
  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));
  TESTHR(pIndex.CreateInstance(__uuidof(ADOX::Index)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_pImpl->m_conn));

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

void te::ado::DataSource::dropIndex(const std::string& datasetName, const std::string& idxName)
{
  if(!indexExists(datasetName, idxName))
    throw Exception((boost::format(TR_ADO("The dataset \"%1%\" has no index with this name: \"%2%\"!")) % datasetName % idxName).str());

  std::string sql("DROP INDEX ");
  sql += idxName;

  execute(sql);

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  te::da::Index* idx = dt->getIndex(idxName);

  dt->remove(idx);
}

//std::vector<std::string> te::ado::DataSource::getSequenceNames()
//{
//  return std::vector<std::string>();
//}
//
//bool te::ado::DataSource::sequenceExists(const std::string& name)
//{
//  return false;
//}
//
//te::da::Sequence* te::ado::DataSource::getSequence(const std::string& name)
//{
//  return 0;
//}
//
//void te::ado::DataSource::addSequence(te::da::Sequence* sequence)
//{
//  
//}
//
//void te::ado::DataSource::dropSequence(const std::string& name)
//{
//  
//}

std::auto_ptr<te::gm::Envelope> te::ado::DataSource::getExtent(const std::string& datasetName, const std::string& propertyName)
{
  if(!dataSetExists(datasetName))
    throw Exception(TR_ADO("The Data Set Type does not exist!"));

  std::string sql = "SELECT MIN(lower_x), MIN(lower_y), MAX(upper_x), MAX(upper_y) from " + propertyName;

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

std::auto_ptr<te::gm::Envelope> te::ado::DataSource::getExtent(const std::string& datasetName, std::size_t propertyPos)
{
  if(!dataSetExists(datasetName))
    throw Exception(TR_ADO("The Data Set Type does not exist!"));

  std::auto_ptr<te::dt::Property> p(getProperty(datasetName, propertyPos));

  std::string sql = "SELECT MIN(lower_x), MIN(lower_y), MAX(upper_x), MAX(upper_y) from " + p->getName();

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

std::size_t te::ado::DataSource::getNumberOfItems(const std::string& datasetName)
{
  std::auto_ptr<te::da::DataSet> result = getDataSet(datasetName);
  return result->size();
}

bool te::ado::DataSource::hasDataSets()
{
  std::vector<std::string> datasetNames = getDataSetNames();

  if(datasetNames.empty())
    return false;

  return true;
}

bool te::ado::DataSource::dataSetExists(const std::string& name)
{
  std::vector<std::string> datasetNames = getDataSetNames();

  if(std::find(datasetNames.begin(), datasetNames.end(), name) != datasetNames.end())
    return true;

  return false;
}

void te::ado::DataSource::createDataSet(te::da::DataSetType* dt, const std::map<std::string, std::string>& options)
{
  ADOX::_TablePtr pTable = 0;
  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pTable.CreateInstance(__uuidof (ADOX::Table)));
  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_pImpl->m_conn));

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
  
  te::gm::GeometryProperty* geomProp = 0;

  if(dt->hasGeom())
  {
    te::dt::SimpleProperty* lowerX = new te::dt::SimpleProperty("lower_x", te::dt::DOUBLE_TYPE);
    te::dt::SimpleProperty* lowerY = new te::dt::SimpleProperty("lower_y", te::dt::DOUBLE_TYPE);
    te::dt::SimpleProperty* upperX = new te::dt::SimpleProperty("upper_x", te::dt::DOUBLE_TYPE);
    te::dt::SimpleProperty* upperY = new te::dt::SimpleProperty("upper_y", te::dt::DOUBLE_TYPE);

    addProperty(dt->getName(), lowerX);
    addProperty(dt->getName(), lowerY);
    addProperty(dt->getName(), upperX);
    addProperty(dt->getName(), upperY);

    geomProp = te::da::GetFirstGeomProperty(dt);
  }

  //TODO: Verificar o motivo do erro ao descomentar a ultima linha
  //if(geomProp)
    //te::ado::InsertInGeometryColumns(m_pImpl->m_conn, dt);
}

void te::ado::DataSource::cloneDataSet(const std::string& name, const std::string& cloneName,
                                       const std::map<std::string, std::string>& options)
{
  //TODO
}

void te::ado::DataSource::dropDataSet(const std::string& name)
{
  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_pImpl->m_conn));
    
    TESTHR(pCatalog->Tables->Delete(name.c_str()));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::DataSource::renameDataSet(const std::string& name, const std::string& newName)
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

void te::ado::DataSource::add(const std::string& datasetName, te::da::DataSet* d,
                              const std::map<std::string, std::string>& options, std::size_t limit)
{
  _RecordsetPtr recset;
  TESTHR(recset.CreateInstance(__uuidof(Recordset)));

  try
  {
    TESTHR(recset->Open(_bstr_t(datasetName.c_str()),
      _variant_t((IDispatch*)m_pImpl->m_conn,true), adOpenKeyset, adLockOptimistic, adCmdTable));
    
    do
    {
      TESTHR(recset->AddNew());

      for(std::size_t i = 0; i < d->getNumProperties(); ++i)
      {
      
        std::string pname = d->getPropertyName(i);
        int pType = d->getPropertyDataType(i);

        switch(pType)
        {
          case te::dt::CHAR_TYPE:
            recset->GetFields()->GetItem(pname.c_str())->Value = (_bstr_t)d->getChar(pname.c_str());
            break;

          //case te::dt::UCHAR_TYPE:

          case te::dt::INT16_TYPE:
            recset->GetFields()->GetItem(pname.c_str())->Value = (_variant_t)d->getInt16(pname.c_str());
            break;

          case te::dt::INT32_TYPE:
            recset->GetFields()->GetItem(pname.c_str())->Value = (_variant_t)d->getInt32(pname.c_str());
            break;

          case te::dt::INT64_TYPE:
            recset->GetFields()->GetItem(pname.c_str())->Value = (_variant_t)d->getInt64(pname.c_str());
            break;

          //case te::dt::NUMERIC_TYPE:
          //case te::dt::DATETIME_TYPE:
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
            te::gm::Geometry* geometry = d->getGeometry(pname);
            const te::gm::Envelope* env = geometry->getMBR();
            
            recset->GetFields()->GetItem("lower_x")->Value = (_variant_t)env->m_llx;
            recset->GetFields()->GetItem("lower_y")->Value = (_variant_t)env->m_lly;
            recset->GetFields()->GetItem("upper_x")->Value = (_variant_t)env->m_urx;
            recset->GetFields()->GetItem("upper_y")->Value = (_variant_t)env->m_ury;

            _variant_t var;
            Convert2Ado(d->getGeometry(pname), var);

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
    while(d->moveNext());
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::DataSource::remove(const std::string& datasetName, const te::da::ObjectIdSet* /*oids*/)
{
  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_pImpl->m_conn));
    
    TESTHR(pCatalog->Tables->Delete(datasetName.c_str()));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::DataSource::update(const std::string& datasetName,
                                 te::da::DataSet* dataset,
                                 const std::vector<std::size_t>& properties,
                                 const te::da::ObjectIdSet* oids,
                                 const std::map<std::string, std::string>& options,
                                 std::size_t limit)
{
  //TODO
}

std::vector<std::string> te::ado::DataSource::getDataSourceNames(const std::map<std::string, std::string>& info)
{
  return std::vector<std::string>();
}

std::vector<std::string> te::ado::DataSource::getEncodings(const std::map<std::string, std::string>& info)
{
  return std::vector<std::string>(); //TODO how?
}

void te::ado::DataSource::create(const std::map<std::string, std::string>& dsInfo)
{
  // Create a connection to an auxiliary data source
  std::auto_ptr<DataSource> ds(new DataSource());

  ds->setConnectionInfo(dsInfo);

  // Mount the connection string
  std::string connStr;

  std::map<std::string, std::string>::const_iterator it = dsInfo.find("PROVIDER");
  std::map<std::string, std::string>::const_iterator it_end = dsInfo.end();

  if(it != it_end)
    connStr += "Provider=" + it->second + ";";
  else
    throw Exception(TR_ADO("The database couldn't be created due the missing parameter: PROVIDER!"));

  it = dsInfo.find("DB_NAME");
  if(it != it_end)
    connStr += "Data Source=" + it->second + ";";
  else
    throw Exception(TR_ADO("The database couldn't be created due the missing parameter: DB_NAME!"));

  it = dsInfo.find("USER_NAME");
  if(it != it_end)
    connStr += "User Id=" + it->second + ";";

  it = dsInfo.find("PASSWORD");
  if(it != it_end)
    connStr += "Jet OLEDB:Database Password=" + it->second + ";";

  // Create the new database
  ADOX::_CatalogPtr pCatalog = 0;
  pCatalog.CreateInstance(__uuidof(ADOX::Catalog));

  try
  {
    pCatalog->Create(connStr.c_str());
    ds->open();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  it = dsInfo.find("CREATE_OGC_METADATA_TABLES");
  if(it != it_end && it->second == "TRUE")
  {
    // Create the geometry_columns dataset
    te::da::DataSetType* geomColsDt = new te::da::DataSetType("geometry_columns");

    geomColsDt->add(new te::dt::StringProperty("f_table_catalog", te::dt::VAR_STRING, 256));
    geomColsDt->add(new te::dt::StringProperty("f_table_schema", te::dt::VAR_STRING, 256));
    geomColsDt->add(new te::dt::StringProperty("f_table_name", te::dt::VAR_STRING, 256));
    geomColsDt->add(new te::dt::StringProperty("f_geometry_column", te::dt::VAR_STRING, 256));
    geomColsDt->add(new te::dt::SimpleProperty("coord_dimension", te::dt::INT32_TYPE));
    geomColsDt->add(new te::dt::SimpleProperty("srid", te::dt::INT32_TYPE));
    geomColsDt->add(new te::dt::StringProperty("type", te::dt::VAR_STRING, 30));

    std::map<std::string, std::string> op;
    createDataSet(geomColsDt, op);
  }
}

void te::ado::DataSource::drop(const std::map<std::string, std::string>& dsInfo)
{
  if(!exists(dsInfo))
    throw Exception(TR_ADO("The data source doesn't exist!"));

  std::map<std::string, std::string> info = dsInfo;

  boost::filesystem::path path(info["DB_NAME"]);
  if(boost::filesystem::remove(path) == false)
    throw Exception(TR_ADO("The data source could not be dropped!"));
}

bool te::ado::DataSource::exists(const std::map<std::string, std::string>& dsInfo)
{
  std::map<std::string, std::string> info = dsInfo;

  boost::filesystem::path path(info["DB_NAME"]);
  return boost::filesystem::exists(path);
}

void te::ado::DataSource::getIndexes(te::da::DataSetTypePtr& dt)
{
  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR((pCatalog.CreateInstance(__uuidof(ADOX::Catalog))));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_pImpl->m_conn));
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

void te::ado::DataSource::getProperties(te::da::DataSetTypePtr& dt)
{
  std::string dsName = dt->getName();
  int numCols = 0;
  te::dt::Property* p = 0;

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
      isRequiredMap[pos] = !hasDefault;

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
    ADOX::DataTypeEnum colType = colTypesMap[i];
    std::string colName = colNamesMap[i];

    switch(colType)
    {
      case ::adBoolean:
        p = new te::dt::SimpleProperty(colName, Convert2Terralib(colType));
        break;

      case ::adVarWChar:
      case ::adWChar:
      case ::adVarChar:
      case ::adLongVarChar:
      case ::adLongVarWChar:
      case ::adBSTR:
      case ::adChar:
        p = new te::dt::StringProperty(colName, (te::dt::StringType)Convert2Terralib(colType), charLengthMap[i]);
        break;

      case ADOX::adTinyInt:
      case ADOX::adSmallInt:
      case ADOX::adInteger:
      case ADOX::adBigInt:
      case ADOX::adSingle:
      case ADOX::adDouble:
      case ::adUnsignedBigInt:
      case ::adUnsignedInt:
      case ::adUnsignedSmallInt:
      case ::adUnsignedTinyInt:
      case ADOX::adLongVarBinary:
      case ADOX::adBinary:
      {
        if(te::ado::IsGeomProperty(conn, dsName, colName))
        {
          p = new te::gm::GeometryProperty(colName, te::ado::GetSRID(conn, dsName, colName), te::ado::GetType(conn, dsName, colName));
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