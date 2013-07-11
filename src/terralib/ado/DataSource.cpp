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
  \file terralib/ado/DataSource.cpp

  \brief It implements the DataSource class for ADO.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../dataaccess/query/SQLDialect.h"
#include "../datatype/StringProperty.h"
#include "DataSet.h"
#include "DataSource.h"
#include "DataSourceCatalogLoader.h"
#include "DataSourceTransactor.h"
#include "DataSetTypePersistence.h"
#include "Exception.h"
#include "Globals.h"
#include "Utils.h"

#include <boost/filesystem.hpp>

inline void TESTHR(HRESULT hr)
{
  if(FAILED(hr))
    _com_issue_error(hr);
}

te::ado::DataSource::DataSource()
  : m_conn(0),
    m_connInUse(false),
    m_catalog(0)
{
  try
  {
    ::CoInitialize(0);
    m_catalog = new te::da::DataSourceCatalog;
    m_catalog->setDataSource(this);
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

te::ado::DataSource::~DataSource()
{
  try
  {
    delete m_catalog;
    close();
    ::CoUninitialize();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

const std::string& te::ado::DataSource::getType() const
{
  return Globals::sm_driverIdentifier;
}

const std::map<std::string, std::string>& te::ado::DataSource::getConnectionInfo() const
{
  return m_connectionInfo;
}

void te::ado::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_connectionInfo = connInfo;
}

std::string te::ado::DataSource::getConnectionStr()
{
  return m_connStr;
}

const te::da::DataSourceCapabilities& te::ado::DataSource::getCapabilities() const
{
  return *Globals::sm_capabilities;
}

const te::da::SQLDialect* te::ado::DataSource::getDialect() const
{
  return Globals::sm_queryDialect;
}

void te::ado::DataSource::open()
{
  close();

  m_conn = 0;
  
  m_connStr = "Provider=" + m_connectionInfo["PROVIDER"] + ";";
  m_connStr += "Data Source=" + m_connectionInfo["DB_NAME"] + ";";
  m_connStr += "User Id=" + m_connectionInfo["USER_NAME"] + ";";
  // Microsoft Access password
  m_connStr += "Jet OLEDB:Database Password=" + m_connectionInfo["PASSWORD"] + ";";

  _bstr_t connStr = m_connStr.c_str();

  try
  {
    m_conn.CreateInstance(__uuidof(Connection));
    TESTHR(m_conn->Open(connStr, "", "", -1));
    m_connInUse = false;
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::DataSource::close()
{
  if(m_conn != 0 && m_conn->State == adStateOpen)
    m_conn->Close();

  m_conn = 0;
}

bool te::ado::DataSource::isOpened() const
{
  if(m_conn != 0 && m_conn->State == adStateOpen)
    return true;

  return false;
}

bool te::ado::DataSource::isValid() const
{
  return true;
}

bool te::ado::DataSource::isConnectionInUse() const
{
  return m_connInUse;
}

void te::ado::DataSource::setConnectionAsUsed(bool connInUse)
{
  m_connInUse = connInUse;
}


te::da::DataSourceCatalog* te::ado::DataSource::getCatalog() const
{
  return m_catalog;
}

te::da::DataSourceTransactor* te::ado::DataSource::getTransactor()
{
  _ConnectionPtr conn = 0;

  if(m_connInUse == false)
  {
    conn = m_conn;
    m_connInUse = true;
  }
  else
  {
    conn.CreateInstance(__uuidof(Connection));

    try
    {
      TESTHR(conn->Open(m_connStr.c_str(), "", "", -1));
    }
    catch(_com_error& e)
    {
      throw Exception(TR_ADO(e.Description()));
    }
  }

  te::da::DataSourceTransactor* t(new DataSourceTransactor(this, conn));

  return t;
}

void te::ado::DataSource::optimize(const std::map<std::string, std::string>& /*opInfo*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
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

    te::da::DataSourceTransactor* t = ds->getTransactor();

    t->getDataSetTypePersistence()->create(geomColsDt);

    delete t;
  }

  m_connectionInfo = dsInfo;
  m_connStr = connStr;
}

//void te::ado::DataSource::create(const std::map<std::string, std::string>& dsInfo)
//{
//  m_connectionInfo = dsInfo;
//
//  std::string info = "provider=" + m_connectionInfo["provider"] +
//  ";Data Source=" + m_connectionInfo["dbname"]+
//  ";User Id=;Password=";
//
//  m_strCnn = info.c_str();
//
//  // Let's have a connection for an auxiliary database
//  std::auto_ptr<DataSource> ds(new DataSource());
//
//  ds->setConnectionInfo(dsInfo);
//
//  ADOX::_CatalogPtr pCatalog = 0;
//
//  pCatalog.CreateInstance(__uuidof(ADOX::Catalog));
//
//  try
//  {
//    pCatalog->Create(m_strCnn);
//
//    ds->open();
//  }
//  catch(_com_error& e)
//  {
//    throw Exception(TR_ADO(e.Description()));
//  }
//
//  if(!getTransactor()->getCatalogLoader()->datasetExists("geometry_columns"))
//  {
//    te::da::DataSetType* geomColsDt = new te::da::DataSetType("geometry_columns");
//
//    geomColsDt->add(new te::dt::StringProperty("f_table_catalog", te::dt::VAR_STRING, 256));
//    geomColsDt->add(new te::dt::StringProperty("f_table_schema", te::dt::VAR_STRING, 256));
//    geomColsDt->add(new te::dt::StringProperty("f_table_name", te::dt::VAR_STRING, 256));
//    geomColsDt->add(new te::dt::StringProperty("f_geometry_column", te::dt::VAR_STRING, 256));
//    geomColsDt->add(new te::dt::SimpleProperty("coord_dimension", te::dt::INT32_TYPE));
//    geomColsDt->add(new te::dt::SimpleProperty("srid", te::dt::INT32_TYPE));
//    geomColsDt->add(new te::dt::StringProperty("type", te::dt::VAR_STRING, 30));
//
//    getTransactor()->getDataSetTypePersistence()->create(geomColsDt);
//  }
//}

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

std::vector<std::string> te::ado::DataSource::getDataSources(const std::map<std::string, std::string>& info)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::vector<std::string>te::ado::DataSource::getEncodings(const std::map<std::string, std::string>& info)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}
