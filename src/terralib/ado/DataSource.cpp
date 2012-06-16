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
  \file DataSource.cpp

  \brief Implements the DataSource class for ADO.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../datatype/StringProperty.h"
#include "DataSource.h"
#include "DataSet.h"
#include "DataSourceTransactor.h"
#include "DataSetTypePersistence.h"
#include "Exception.h"

inline void TESTHR( HRESULT hr )
{
	if( FAILED(hr) ) _com_issue_error( hr );
}

te::ado::DataSource::DataSource()
  : m_conn(0)
{
  ::CoInitialize(0);
}

te::ado::DataSource::~DataSource()
{
  ::CoUninitialize();
}

const std::string& te::ado::DataSource::getType() const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

const std::map<std::string, std::string>& te::ado::DataSource::getConnectionInfo() const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_connectionInfo = connInfo;
}

void te::ado::DataSource::getCapabilities(std::map<std::string, std::string>& /*capabilities*/) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

const te::da::SQLDialect* te::ado::DataSource::getDialect() const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::open()
{
  close();

  m_conn = 0;
  
  std::string info = "provider="+m_connectionInfo["provider"]+
    ";Data Source="+m_connectionInfo["dbname"]+
    ";User Id=;Password=";

  m_strCnn = info.c_str();

  try
  {
    m_conn.CreateInstance(__uuidof(Connection));
    HRESULT hr  = m_conn->Open (m_strCnn,"","",-1);
		TESTHR( hr );
  }
  catch(_com_error &e)
	{
    throw Exception(TR_ADO(e.ErrorMessage()));
	}
  catch(...)
  {
    throw Exception(TR_ADO("Unknown Error..."));
  }
}

void te::ado::DataSource::close()
{
  if(m_conn != 0)
    m_conn->Close();
  m_conn = 0;
}

bool te::ado::DataSource::isOpened() const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSource::isValid() const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::DataSourceCatalog* te::ado::DataSource::getCatalog() const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::DataSourceTransactor* te::ado::DataSource::getTransactor()
{
  _ConnectionPtr newConn = 0;

  try
  {
    newConn.CreateInstance(__uuidof(Connection));
    TESTHR(newConn->Open (m_strCnn,"","",-1));
  }
  catch(_com_error &e)
  {
    std::string description(e.Description()); 
    throw Exception(TR_ADO("ADO Driver Error: " + description));
  }
  catch(...)
  {
    
  }

  std::auto_ptr<te::da::DataSourceTransactor> transactor(new DataSourceTransactor(this, newConn));

  return transactor.release();
}

void te::ado::DataSource::optimize(const std::map<std::string, std::string>& /*opInfo*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSource::create(const std::map<std::string, std::string>& dsInfo)
{
  try
  {
    m_connectionInfo = dsInfo;

    std::string info = "provider="+m_connectionInfo["provider"]+
    ";Data Source="+m_connectionInfo["dbname"]+
    ";User Id=;Password=";

    m_strCnn = info.c_str();

    // let's have a connection to the auxiliary database
    std::auto_ptr<DataSource> ds(new DataSource());

    ds->setConnectionInfo(dsInfo);

    ADOX::_CatalogPtr pCatalog = 0;

    pCatalog.CreateInstance(__uuidof(ADOX::Catalog));

    pCatalog->Create(m_strCnn);

    ds->open();
  }
  catch(_com_error &e)
  {
    std::string description(e.Description()); 
    throw Exception(TR_ADO("ADO Driver Error: " + description));
  }
  catch(...)
  {
    
  }

}

void te::ado::DataSource::drop(const std::map<std::string, std::string>& /*dsInfo*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSource::exists(const std::map<std::string, std::string>& /*dsInfo*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}
