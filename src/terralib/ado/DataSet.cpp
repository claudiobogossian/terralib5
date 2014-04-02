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
  \file terralib/ado/DataSet.cpp

  \brief DataSet class implementation for Microsoft Access driver.
*/

// TerraLib
#include "../common/ByteSwapUtils.h"
#include "../common/Globals.h"
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../datatype/Array.h"
#include "../datatype/ByteArray.h"
#include "../datatype/DateTimeProperty.h"
#include "../datatype/SimpleData.h"
#include "../datatype/TimeInstant.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../geometry/WKBReader.h"
#include "Connection.h"
#include "DataSet.h"
#include "DataSource.h"
#include "Exception.h"
#include "Transactor.h"
#include "Utils.h"

// STL
#include <memory>

// Boost
#include <boost/dynamic_bitset.hpp>
#include <boost/lexical_cast.hpp>

inline void TESTHR( HRESULT hr )
{
  if( FAILED(hr) ) _com_issue_error( hr );
}

te::ado::DataSet::DataSet(Transactor* t, _RecordsetPtr result)
  : m_result(result),
    m_t(t),
    m_i(-1),
    m_size(-1),
    m_ncols(-1)
{
  m_size = m_result->GetRecordCount();

  m_ncols = m_result->GetFields()->GetCount();

// convert data types from ADO to TerraLib
  FieldsPtr fields = m_result->GetFields();

  for(int i = 0; i != m_ncols; ++i)
  {
    FieldPtr field = fields->GetItem((long)i);

    std::string columnName = field->GetName();

    m_colNames.push_back(columnName);

    int terralib_data_type = te::ado::Convert2Terralib(field->GetType());

// if it is a byte array it may be a geometric column!
    if(terralib_data_type == te::dt::BYTE_ARRAY_TYPE)
    {
      std::string tableName = (LPCSTR)(_bstr_t)field->GetProperties()->GetItem("BASETABLENAME")->GetValue();

      bool is_geom = m_t->getAdoDataSource()->isGeometryColumn(tableName, columnName);

      if(is_geom)
        m_datatypes.push_back(te::dt::GEOMETRY_TYPE);
      else
        m_datatypes.push_back(terralib_data_type);
    }
    else
    {
      m_datatypes.push_back(terralib_data_type);
    }
  }
}

te::ado::DataSet::~DataSet()
{
  m_result->Close();
}

te::common::TraverseType te::ado::DataSet::getTraverseType() const
{
  return te::common::RANDOM;
}

te::common::AccessPolicy te::ado::DataSet::getAccessPolicy() const
{
  return te::common::RAccess;
}

std::auto_ptr<te::gm::Envelope> te::ado::DataSet::getExtent(std::size_t /*i*/)
{
  throw Exception(TR_ADO("Method getExtent: not implemented yet!"));
}

std::size_t te::ado::DataSet::getNumProperties() const
{
  return m_ncols;
}

int te::ado::DataSet::getPropertyDataType(std::size_t i) const
{
  return m_datatypes[i];
}

std::string te::ado::DataSet::getPropertyName(std::size_t i) const
{
  return m_colNames[i];
}

te::common::CharEncoding te::ado::DataSet::getPropertyCharEncoding(std::size_t i) const
{
  return te::common::UNKNOWN_CHAR_ENCODING; // TODO
}

std::string te::ado::DataSet::getDatasetNameOfProperty(std::size_t /*i*/) const
{
  throw Exception(TR_ADO("Method getDatasetNameOfProperty: not implemented yet!"));
}

bool te::ado::DataSet::isEmpty() const
{
  return (m_size == 0);
}

bool te::ado::DataSet::isConnected() const
{
  return true;
}

std::size_t te::ado::DataSet::size() const
{
  return m_size;
}

bool te::ado::DataSet::moveNext()
{
  if(m_i != -1)
    TESTHR(m_result->MoveNext());

  ++m_i;
  bool teste = (m_i < m_size);
  return (m_i < m_size);
}

bool te::ado::DataSet::movePrevious()
{
  if(m_i != m_size)
    m_result->MovePrevious();

  --m_i;
  return (m_i > -1);
}

bool te::ado::DataSet::moveBeforeFirst()
{
  if(m_result->BOF != 0)
    return true;
  
  TESTHR(m_result->MoveFirst());
  m_i = -1;

  return m_size != 0;
}

bool te::ado::DataSet::moveFirst()
{
  m_result->MoveFirst();
  m_i = 0;
  return m_size != 0;
}

bool te::ado::DataSet::moveLast()
{
  TESTHR(m_result->MoveLast());
  m_i = m_size - 1;
  return (m_i < m_size);
}

bool te::ado::DataSet::move(std::size_t i)
{
  if(i == 0)
    return moveFirst();

  if(m_i == i)
    return true;

  long p = static_cast<long>(i-m_i);

  TESTHR(m_result->Move(p));

  m_i += p;
  return (m_i < m_size);
}

bool te::ado::DataSet::isAtBegin() const
{
  return m_i == 0;
}

bool te::ado::DataSet::isBeforeBegin() const
{
  return m_i < 0;
}

bool te::ado::DataSet::isAtEnd() const
{
  return m_i == (m_size - 1);
}

bool te::ado::DataSet::isAfterEnd() const
{
  return m_i >= m_size;
}

char te::ado::DataSet::getChar(std::size_t i) const
{
  _variant_t vtIndex;
  vtIndex.vt = VT_I4;
  vtIndex.lVal = i;

  char ival;

  try
  {
    ival = (char)m_result->GetFields()->GetItem(vtIndex)->GetValue();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  return ival;
}

unsigned char te::ado::DataSet::getUChar(std::size_t i) const
{
  return (unsigned char)getChar(i);
}

boost::int16_t te::ado::DataSet::getInt16(std::size_t i) const
{
  _variant_t vtIndex;
  vtIndex.vt = VT_I4;
  vtIndex.lVal = i;

  int16_t ival;

  try
  {
    ival = (int16_t)m_result->GetFields()->GetItem(vtIndex)->GetValue();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  return ival;
}

boost::int32_t te::ado::DataSet::getInt32(std::size_t i) const
{
  _variant_t vtIndex;
  vtIndex.vt = VT_I4;
  vtIndex.lVal = i;

  int32_t ival;

  try
  {
    ival = (int32_t)m_result->GetFields()->GetItem(vtIndex)->GetValue();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  return ival;
}

boost::int64_t te::ado::DataSet::getInt64(std::size_t i) const
{
  _variant_t vtIndex;
  vtIndex.vt = VT_I4;
  vtIndex.lVal = i;

  int64_t ival;

  try
  {
    ival = (int64_t)m_result->GetFields()->GetItem(vtIndex)->GetValue();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  return ival;
}

bool te::ado::DataSet::getBool(std::size_t i) const
{
  _variant_t vtIndex;
  vtIndex.vt = VT_I4;
  vtIndex.lVal = i;

  bool ival;

  try
  {
    ival = (bool)m_result->GetFields()->GetItem(vtIndex)->GetValue();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  return ival;
}

float te::ado::DataSet::getFloat(std::size_t i) const
{
  float value;

  _variant_t vtIndex;
  vtIndex.vt = VT_I4;
  vtIndex.lVal = i;

  try
  {
    value = (float)m_result->GetFields()->GetItem(vtIndex)->GetValue();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  return value;
}

double te::ado::DataSet::getDouble(std::size_t i) const
{
  double value;

  _variant_t vtIndex;
  vtIndex.vt = VT_I4;
  vtIndex.lVal = i;

  try
  {
    value = (double)m_result->GetFields()->GetItem(vtIndex)->GetValue();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  return value;
}

std::string te::ado::DataSet::getNumeric(std::size_t /*i*/) const
{
  throw Exception(TR_ADO("Method getNumeric: not implemented yet!"));
}

std::string te::ado::DataSet::getString(std::size_t i) const
{
  _variant_t vtIndex;
  vtIndex.vt = VT_I4;
  vtIndex.lVal = i;

  std::string ival;

  try
  {
    ival = (LPCSTR)(_bstr_t)m_result->GetFields()->GetItem(vtIndex)->GetValue();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  return ival;
}

std::auto_ptr<te::dt::ByteArray> te::ado::DataSet::getByteArray(std::size_t i) const
{
  _variant_t vtIndex;
  vtIndex.vt = VT_I4;
  vtIndex.lVal = i;

  _variant_t varBLOB;
  char *cdata = 0;
  long size;
  char* data;

  try
  {
    ::Field15Ptr field;

    if(m_result->MoveNext() == S_OK)
      m_result->MovePrevious();
    else if(m_result->MovePrevious() == S_OK)
      m_result->MoveNext();
    else
    {
      _RecordsetPtr copy = m_result->Clone(adLockReadOnly);

      field = copy->GetFields()->GetItem(vtIndex);
    }

    field = m_result->GetFields()->GetItem(vtIndex);

    size = field->ActualSize;
    if(size > 0)
    {
      VariantInit(&varBLOB);

      varBLOB = field->GetChunk(size);

      if(varBLOB.vt == (VT_ARRAY | VT_UI1))
      {
        SafeArrayAccessData(varBLOB.parray,(void **)&cdata);
        data = new char[size];
        memcpy(data, cdata, size);
        SafeArrayUnaccessData(varBLOB.parray);
      }
    }
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  return std::auto_ptr<te::dt::ByteArray>(new te::dt::ByteArray(data, size));
}

std::auto_ptr<te::gm::Geometry> te::ado::DataSet::getGeometry(std::size_t i) const
{
  std::auto_ptr<te::dt::ByteArray> ba(getByteArray(i));

  std::auto_ptr<te::gm::Geometry> geom(te::gm::WKBReader::read(ba->getData()));

  std::size_t wkb_size = geom->getWkbSize();

  if(ba->bytesUsed() - wkb_size >= 4)
    geom->setSRID(*((int*)(ba->getData() + wkb_size)));

  return geom;
}

std::auto_ptr<te::rst::Raster> te::ado::DataSet::getRaster(std::size_t /*i*/) const
{
  throw Exception(TR_ADO("Method getRaster: not implemented yet!"));
}

std::auto_ptr<te::dt::DateTime> te::ado::DataSet::getDateTime(std::size_t i) const
{
  _variant_t vtIndex;
  _variant_t value;

  vtIndex.vt = VT_I4;
  vtIndex.lVal = i;

  te::dt::DateTime* dateTime = 0;

  std::string strDate;
  try
  {
    value = m_result->GetFields()->GetItem(vtIndex)->Value;
  }
  catch(_com_error &e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  if(value.vt == VT_NULL)
    return std::auto_ptr<te::dt::DateTime>(0);

  strDate = (LPCSTR)(_bstr_t)value;

  // Getting system format
  std::string indAM;
  std::string indPM;
  std::string sepD;
  std::string sepT;

  std::string mask = te::ado::GetSystemDateTimeFormat(indAM, indPM, sepD, sepT);

  std::auto_ptr<te::dt::DateTime> result = te::ado::GetDateTime(strDate, mask, sepD, sepT);

  return result;
}

std::auto_ptr<te::dt::Array> te::ado::DataSet::getArray(std::size_t /*i*/) const
{
  throw Exception(TR_ADO("Method getArray: not implemented yet!"));
}

bool te::ado::DataSet::isNull(std::size_t i) const
{
  _variant_t value;

  std::string propertyName = getPropertyName(i);

  try
  {
    value = m_result->GetFields()->GetItem(static_cast<long>(i))->GetValue();
  }

  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  if(value.vt == VT_NULL)
    return true;

  return false;
}
