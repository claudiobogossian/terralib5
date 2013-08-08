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
  throw Exception(TR_ADO("Not implemented yet!"));
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
  throw Exception(TR_ADO("Not implemented yet!"));
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
  throw Exception(TR_ADO("Not implemented yet!"));
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
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::dropProperty(const std::string& datasetName, const std::string& propertyName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::renameProperty(const std::string& datasetName, const std::string& propertyName, const std::string& newPropertyName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
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
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::dropPrimaryKey(const std::string& datasetName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
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
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::dropForeignKey(const std::string& datasetName, const std::string& fkName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::vector<std::string> te::ado::DataSource::getUniqueKeyNames(const std::string& datasetName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSource::uniqueKeyExists(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::UniqueKey* te::ado::DataSource::getUniqueKey(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::addUniqueKey(const std::string& datasetName, te::da::UniqueKey* uk)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::dropUniqueKey(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::vector<std::string> te::ado::DataSource::getCheckConstraintNames(const std::string& datasetName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSource::checkConstraintExists(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::CheckConstraint* te::ado::DataSource::getCheckConstraint(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::addCheckConstraint(const std::string& datasetName, te::da::CheckConstraint* cc)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::dropCheckConstraint(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::vector<std::string> te::ado::DataSource::getIndexNames(const std::string& datasetName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSource::indexExists(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::Index* te::ado::DataSource::getIndex(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::addIndex(const std::string& datasetName, te::da::Index* idx, const std::map<std::string, std::string>& options)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::dropIndex(const std::string& datasetName, const std::string& idxName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::vector<std::string> te::ado::DataSource::getSequenceNames()
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSource::sequenceExists(const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::Sequence* te::ado::DataSource::getSequence(const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::addSequence(te::da::Sequence* sequence)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::dropSequence(const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::auto_ptr<te::gm::Envelope> te::ado::DataSource::getExtent(const std::string& datasetName, const std::string& propertyName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::auto_ptr<te::gm::Envelope> te::ado::DataSource::getExtent(const std::string& datasetName, std::size_t propertyPos)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::size_t te::ado::DataSource::getNumberOfItems(const std::string& datasetName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSource::hasDataSets()
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSource::dataSetExists(const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::createDataSet(te::da::DataSetType* dt, const std::map<std::string, std::string>& options)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::cloneDataSet(const std::string& name, const std::string& cloneName,
                                       const std::map<std::string, std::string>& options)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::dropDataSet(const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::renameDataSet(const std::string& name, const std::string& newName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::add(const std::string& datasetName, te::da::DataSet* d,
                              const std::map<std::string, std::string>& options, std::size_t limit)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::remove(const std::string& datasetName, const te::da::ObjectIdSet* oids)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::update(const std::string& datasetName,
                                 te::da::DataSet* dataset,
                                 const std::vector<std::size_t>& properties,
                                 const te::da::ObjectIdSet* oids,
                                 const std::map<std::string, std::string>& options,
                                 std::size_t limit)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::optimize(const std::map<std::string, std::string>& opInfo)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::vector<std::string> te::ado::DataSource::getDataSourceNames(const std::map<std::string, std::string>& info)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::vector<std::string> te::ado::DataSource::getEncodings(const std::map<std::string, std::string>& info)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::create(const std::map<std::string, std::string>& dsInfo)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::drop(const std::map<std::string, std::string>& dsInfo)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSource::exists(const std::map<std::string, std::string>& dsInfo)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}