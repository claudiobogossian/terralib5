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
  \file DataSet.cpp

  \brief Implementation of the DataSet class to ADO.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "../memory/DataSetItem.h"
#include "../geometry/WKBWriter.h"
#include "../geometry/WKBReader.h"
#include "../geometry/Geometry.h"
#include "../datatype/ByteArray.h"
#include "../datatype/SimpleProperty.h"
#include "../datatype/ByteArrayProperty.h"
#include "DataSourceTransactor.h"
#include "DataSet.h"
#include "Exception.h"
#include "Utils.h"

te::ado::DataSet::DataSet(te::da::DataSetType* dt, _RecordsetPtr result, te::da::DataSourceTransactor* transactor) :
  m_i(-1),
  m_result(result),
  m_dt(dt),
  m_t(transactor),
  m_name(0),
  m_geomFilter(0),
  m_mbrFilter(0),
  m_relation(te::gm::UNKNOWN_SPATIAL_RELATION)

{
  m_size = m_result->GetRecordCount();
  m_ncols = m_result->GetFields()->GetCount();
}

te::ado::DataSet::~DataSet()
{
  delete m_dt;
  delete m_geomFilter;
}

te::common::TraverseType te::ado::DataSet::getTraverseType() const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::common::AccessPolicy te::ado::DataSet::getAccessPolicy() const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::DataSetType* te::ado::DataSet::getType()
{
  return m_dt;
}

const te::da::DataSetType* te::ado::DataSet::getType() const
{
  return m_dt;
}

te::da::DataSourceTransactor* te::ado::DataSet::getTransactor() const
{
  return m_t;
}

void te::ado::DataSet::loadTypeInfo()
{
  getType();
}

te::da::DataSet* te::ado::DataSet::getParent() const
{
  return 0;
}

te::gm::Envelope* te::ado::DataSet::getExtent(const te::dt::Property* p)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setFilter(te::dt::Property* p,
                                      const te::gm::Geometry* g,
                                      te::gm::SpatialRelation r)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}
 
void te::ado::DataSet::setFilter(te::dt::Property* p,
                                      const te::gm::Envelope* e,
                                      te::gm::SpatialRelation r)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::DataSetItem* te::ado::DataSet::getItem() const
{
  return new te::mem::DataSetItem(this);
}

void te::ado::DataSet::add(te::da::DataSetItem* item)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSet::isEmpty() const
{
  return (m_size == 0);
}

std::size_t te::ado::DataSet::size() const
{
  return m_size;
}

bool te::ado::DataSet::moveNext()
{
  if(m_i != -1)
    m_result->MoveNext();
  
  ++m_i;
  return (m_i < m_size);
}

bool te::ado::DataSet::movePrevious()
{
  if(m_i != m_size)
    m_result->MovePrevious();

  --m_i;
  return (m_i > -1);
}

bool te::ado::DataSet::moveFirst()
{
  m_result->MoveFirst();
  m_i = 0;
  return m_size != 0;
}

bool te::ado::DataSet::moveBeforeFirst()
{
  m_result->MoveFirst();
  m_i = -1;
  return m_size != 0;
}

bool te::ado::DataSet::moveLast()
{
  m_result->MoveLast();
  m_i = m_size - 1;
  return (m_i < m_size);
}

bool te::ado::DataSet::moveAfterLast()
{
  m_result->MoveLast();
  m_i = m_size;
  return m_size != 0;
}

bool te::ado::DataSet::move(std::size_t i)
{
  m_result->Move((long)i);
  m_i = static_cast<int>(i);
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

char te::ado::DataSet::getChar(int i) const
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

char te::ado::DataSet::getChar(const std::string& name) const
{
  char ival;

  try
  {
    ival = (char)m_result->GetFields()->GetItem(name.c_str())->GetValue();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  return ival;
}

void te::ado::DataSet::setChar(int i, char value)
{
  try
  {
    m_result->GetFields()->GetItem(i)->PutValue((_variant_t)value);
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::DataSet::setChar(const std::string& name, char value)
{
  try
  {
    m_result->GetFields()->GetItem(name.c_str())->PutValue((_variant_t)value);
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

unsigned char te::ado::DataSet::getUChar(int i) const
{
  return (unsigned char)getChar(i);
}

unsigned char te::ado::DataSet::getUChar(const std::string& name) const
{
  return (unsigned char)getChar(name);
}

void te::ado::DataSet::setUChar(int i, unsigned char value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setUChar(const std::string& name, unsigned char value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

boost::int16_t te::ado::DataSet::getInt16(int i) const
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

boost::int16_t te::ado::DataSet::getInt16(const std::string& name) const
{
  int16_t ival; 
  
  try
  {
    ival = (int16_t)m_result->GetFields()->GetItem(name.c_str())->GetValue();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  return ival;
}

void te::ado::DataSet::setInt16(int i, boost::int16_t value)
{
  try
  {
    m_result->GetFields()->GetItem(i)->PutValue((_variant_t)value);
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::DataSet::setInt16(const std::string& name, boost::int16_t value)
{
  try
  {
    m_result->GetFields()->GetItem(name.c_str())->PutValue((_variant_t)value);
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

boost::int32_t te::ado::DataSet::getInt32(int i) const
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

boost::int32_t te::ado::DataSet::getInt32(const std::string& name) const
{
  int32_t ival;
  
  try
  {
    ival = (int32_t)m_result->GetFields()->GetItem(name.c_str())->GetValue();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  return ival;
}

void te::ado::DataSet::setInt32(int i, boost::int32_t value)
{
  try
  {
    m_result->GetFields()->GetItem(i)->PutValue((_variant_t)value);
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::DataSet::setInt32(const std::string& name, boost::int32_t value)
{
  try
  {
    m_result->GetFields()->GetItem(name.c_str())->PutValue((_variant_t)value);
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

boost::int64_t te::ado::DataSet::getInt64(int i) const
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

boost::int64_t te::ado::DataSet::getInt64(const std::string& name) const
{
  int64_t ival;
  
  try
  {
    ival = (int64_t)m_result->GetFields()->GetItem(name.c_str())->GetValue();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  return ival;
}

void te::ado::DataSet::setInt64(int i, boost::int64_t value)
{
  try
  {
    m_result->GetFields()->GetItem(i)->PutValue((_variant_t)value);
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::DataSet::setInt64(const std::string& name, boost::int64_t value)
{
  try
  {
    m_result->GetFields()->GetItem(name.c_str())->PutValue((_variant_t)value);
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

bool te::ado::DataSet::getBool(int i) const
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

bool te::ado::DataSet::getBool(const std::string& name) const
{
  bool ival;
  
  try
  {
    ival = (bool)m_result->GetFields()->GetItem(name.c_str())->GetValue();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  return ival;
}

void te::ado::DataSet::setBool(int i, bool value)
{
  try
  {
    m_result->GetFields()->GetItem(i)->PutValue((_variant_t)value);
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::DataSet::setBool(const std::string& name, bool value)
{
  try
  {
    m_result->GetFields()->GetItem(name.c_str())->PutValue((_variant_t)value);
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

float te::ado::DataSet::getFloat(int i) const
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

float te::ado::DataSet::getFloat(const std::string& name) const
{
  float ival;
  
  try
  {
    ival = (float)m_result->GetFields()->GetItem(name.c_str())->GetValue();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  return ival;
}

void te::ado::DataSet::setFloat(int i, float value)
{
  _variant_t vtIndex;
  vtIndex.vt = VT_I4;
  vtIndex.lVal = i;

  try
  {
    m_result->GetFields()->GetItem(vtIndex)->PutValue((_variant_t)value);
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::DataSet::setFloat(const std::string& name, float value)
{
  try
  {
    m_result->GetFields()->GetItem(name.c_str())->PutValue((_variant_t)value);
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

double te::ado::DataSet::getDouble(int i) const
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

//double te::ado::DataSet::getDouble(int i) const
//{
//  double ival;
//  
//  try
//  {
//    ival = (double)m_result->GetFields()->GetItem(i)->GetValue();
//  }
//  catch(_com_error& e)
//  {
//    throw Exception(TR_ADO(e.Description()));
//  }
//
//  return ival;
//}

double te::ado::DataSet::getDouble(const std::string& name) const
{
  double ival;
  
  try
  {
    ival = (double)m_result->GetFields()->GetItem(name.c_str())->GetValue();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  return ival;
}

void te::ado::DataSet::setDouble(int i, double value)
{
  _variant_t vtIndex;
  vtIndex.vt = VT_I4;
  vtIndex.lVal = i;

  try
  {
    m_result->GetFields()->GetItem(vtIndex)->PutValue((_variant_t)value);
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::DataSet::setDouble(const std::string& name, double value)
{
  try
  {
    m_result->GetFields()->GetItem(name.c_str())->PutValue((_variant_t)value);
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

std::string te::ado::DataSet::getNumeric(int i) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::string te::ado::DataSet::getNumeric(const std::string& name) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setNumeric(int i, const std::string& value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setNumeric(const std::string& name, const std::string& value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

std::string te::ado::DataSet::getString(int i) const
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

std::string te::ado::DataSet::getString(const std::string& name) const
{
  std::string ival;
  
  try
  {
    ival = (LPCSTR)(_bstr_t)m_result->GetFields()->GetItem(name.c_str())->GetValue();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  return ival;
}

void te::ado::DataSet::setString(int i, const std::string& value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setString(const std::string& name, const std::string& value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::dt::ByteArray* te::ado::DataSet::getByteArray(int i) const
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
    ::Field15Ptr field = m_result->GetFields()->GetItem(vtIndex);

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

  return new te::dt::ByteArray(data, size);
}

te::dt::ByteArray* te::ado::DataSet::getByteArray(const std::string& name) const
{
  _variant_t varBLOB;
  char *cdata = 0;
  long size;
  char* data;

  try
  {
    size = m_result->GetFields()->GetItem(name.c_str())->ActualSize;
    if(size > 0)
    {
      VariantInit(&varBLOB);

      varBLOB = m_result->GetFields()->GetItem(name.c_str())->GetChunk(size);
      te::ado::Variant2Blob(varBLOB, size, data);
    }
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  return new te::dt::ByteArray(data, size);
}

void te::ado::DataSet::setByteArray(int i, const te::dt::ByteArray& value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setByteArray(const std::string& name, const te::dt::ByteArray& value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::gm::Geometry* te::ado::DataSet::getGeometry(int i) const
{
  te::dt::ByteArray* ba = getByteArray(i);

  te::gm::Geometry* geom = te::gm::WKBReader::read(ba->getData());

  std::size_t wkb_size = geom->getWkbSize();

  if(ba->bytesUsed() - wkb_size >= 4)
    geom->setSRID(*((int*)(ba->getData() + wkb_size)));

  return geom;
}

te::gm::Geometry* te::ado::DataSet::getGeometry(const std::string& name) const
{
  te::dt::ByteArray* ba = getByteArray(name);
  
  te::gm::Geometry* geom = te::gm::WKBReader::read(ba->getData());

  std::size_t wkb_size = geom->getWkbSize();

  if(ba->bytesUsed() - wkb_size >= 4)
    geom->setSRID(*((int*)(ba->getData() + wkb_size)));

  return geom;
}

void te::ado::DataSet::setGeometry(int i, const te::gm::Geometry& value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setGeometry(const std::string& name, const te::gm::Geometry& value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::rst::Raster* te::ado::DataSet::getRaster(int i) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::rst::Raster* te::ado::DataSet::getRaster(const std::string& name) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setRaster(int i, const te::rst::Raster& value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setRaster(const std::string& name, const te::rst::Raster& value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::dt::DateTime* te::ado::DataSet::getDateTime(int i) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::dt::DateTime* te::ado::DataSet::getDateTime(const std::string& name) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setDateTime(int i, const te::dt::DateTime& value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setDateTime(const std::string& name, const te::dt::DateTime& value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::getArray(int i, std::vector<boost::int16_t>& a) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::getArray(const std::string& name, std::vector<boost::int16_t>& a) const
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

const unsigned char* te::ado::DataSet::getWKB(int i) const
{
  te::dt::ByteArray* ba = getByteArray(i);

  return (unsigned char*)ba->getData();
}

const unsigned char* te::ado::DataSet::getWKB(const std::string& name) const
{
  te::dt::ByteArray* ba = getByteArray(name);

  return (unsigned char*)ba->getData();
}

te::da::DataSet* te::ado::DataSet::getDataSet(int i)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setDataSet(int i, const te::da::DataSet& value)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSet::setValue(int i, te::dt::AbstractData* ad)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSet::isNull(int i) const
{
  _variant_t value;

  std::string propertyName = m_dt->getProperty(i)->getName();

  try
  {
    value = m_result->GetFields()->GetItem(propertyName.c_str())->GetValue();
  }

  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  if(value.vt == VT_NULL)
    return true;

  return false;
}

//bool te::ado::DataSet::isNull(int i) const
//{
//  _variant_t vtIndex;
//  vtIndex.lVal = i;
//
//  _variant_t value;
//
//  try
//  {
//    //value = m_result->GetFields()->GetItem(vtIndex)->GetValue();
//    value = m_result->GetFields()->GetItem(vtIndex)->Value;
//  }
//
//  catch(_com_error& e)
//  {
//    throw Exception(TR_ADO(e.Description()));
//  }
//
//  if(value.vt == VT_NULL)
//    return true;
//
//  return false;
//}

