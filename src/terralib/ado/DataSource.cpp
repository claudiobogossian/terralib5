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
#include "../dataaccess/dataset/CheckConstraint.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/ForeignKey.h"
#include "../dataaccess/dataset/Index.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/Sequence.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "../dataaccess/query/Select.h"
#include "../dataaccess/query/SQLDialect.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/DateTimeProperty.h"
#include "../datatype/StringProperty.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "Connection.h"
#include "DataSource.h"
#include "DataSet.h"
#include "Exception.h"
#include "Globals.h"
#include "Transactor.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <iostream>

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

inline void TESTHR(HRESULT hr)
{
  if(FAILED(hr))
    _com_issue_error(hr);
}

te::ado::DataSource::DataSource()
  : m_catalog(0),
    m_conn(0),
    m_isOpened(0)
{
  ::CoInitialize(0);
  m_catalog = new te::da::DataSourceCatalog;
  m_catalog->setDataSource(this);
}

te::ado::DataSource::~DataSource()
{
  delete m_catalog;
  ::CoUninitialize();
}

std::string te::ado::DataSource::getType() const
{
  return ADO_DRIVER_IDENTIFIER;
}

const std::map<std::string, std::string>& te::ado::DataSource::getConnectionInfo() const
{
  return m_connInfo;
}

void te::ado::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_connInfo = connInfo;
}

std::auto_ptr<te::da::DataSourceTransactor> te::ado::DataSource::getTransactor()
{
  return std::auto_ptr<te::da::DataSourceTransactor>(new te::ado::Transactor(this, m_connInfo));
}

void te::ado::DataSource::open()
{
  // Assure we are in a closed state
  close();

  std::string connInfo = MakeConnectionStr(m_connInfo);

  m_conn = new te::ado::Connection(connInfo);

  te::ado::GetGeometryColumnsInfo(m_conn->getConn(), m_geomColumns);

  m_isOpened = true;
}

std::map<std::string, std::string> te::ado::DataSource::getGeomColumns()
{
  te::ado::GetGeometryColumnsInfo(m_conn->getConn(), m_geomColumns);

  return m_geomColumns;
}

void te::ado::DataSource::close()
{
  if(m_conn)
    delete m_conn;

  m_isOpened = false;
}

bool te::ado::DataSource::isOpened() const
{
  return m_isOpened;
}

bool te::ado::DataSource::isValid() const
{
  return m_conn->isValid();
}

const te::da::DataSourceCapabilities& te::ado::DataSource::getCapabilities() const
{
  return *te::ado::Globals::sm_capabilities;
}

const te::da::SQLDialect* te::ado::DataSource::getDialect() const
{
  return te::ado::Globals::sm_queryDialect;
}

static std::vector<std::string> getDataSourceNames(const std::string& dsType, const std::map<std::string, std::string>& info)
{
  return std::vector<std::string>();
}

static std::vector<std::string> getEncodings(const std::string& dsType, const std::map<std::string, std::string>& info)
{
  return std::vector<std::string>(); //TODO how?
}

void te::ado::DataSource::create(const std::map<std::string, std::string>& dsInfo)
{
  m_connInfo = dsInfo;
  std::string connInfo = te::ado::MakeConnectionStr(dsInfo);

  // Create the new database
  ADOX::_CatalogPtr pCatalog = 0;
  pCatalog.CreateInstance(__uuidof(ADOX::Catalog));

  try
  {
    pCatalog->Create(connInfo.c_str());
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
  
  std::map<std::string, std::string>::const_iterator it = dsInfo.find("CREATE_OGC_METADATA_TABLES");
  std::map<std::string, std::string>::const_iterator it_end = dsInfo.end();

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

  close();
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

std::vector<std::string> te::ado::DataSource::getDataSourceNames(const std::map<std::string, std::string>& dsInfo)
{
  return std::vector<std::string>(0); // The DataSource is a File.
}

std::vector<std::string> te::ado::DataSource::getEncodings(const std::map<std::string, std::string>& dsInfo)
{
  return std::vector<std::string>(0); //TODO how?
}

void te::ado::DataSource::getIndexes(te::da::DataSetTypePtr& dt)
{
  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR((pCatalog.CreateInstance(__uuidof(ADOX::Catalog))));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)m_conn));
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