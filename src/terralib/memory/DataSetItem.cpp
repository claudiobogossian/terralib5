/*  Copyright (C) 2010-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file DataSetItem.cpp

  \brief An implementation of the DatasetItem class for the TerraLib In-Memory Data Access driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../datatype/AbstractData.h"
#include "../datatype/ByteArray.h"
#include "../datatype/DateTime.h"
#include "../datatype/Property.h"
#include "../datatype/SimpleData.h"
#include "../geometry/Geometry.h"
#include "../raster/Grid.h"
#include "../raster/RasterProperty.h"
#include "DataSet.h"
#include "DataSetItem.h"
#include "Exception.h"
#include "Raster.h"

// STL
#include <cassert>

te::mem::DataSetItem::DataSetItem(const te::da::DataSet* parent)
  : m_parent(parent),
    m_dt(0, false)
{
  if(parent != 0)
  {
    const te::da::DataSetType* dt = parent->getType();
    m_dt.m_handle = const_cast<te::da::DataSetType*>(dt);
    m_data.resize(m_dt.m_handle->size(), 0);
  }
}

te::mem::DataSetItem::DataSetItem(te::da::DataSetType* dt)
  : m_parent(0),
    m_dt(dt, true)
{
  if(dt != 0)
    m_data.resize(dt->size(), 0);
}

te::mem::DataSetItem::DataSetItem(const te::da::DataSet* parent, te::da::DataSetType* dt, const bool dtOwnership)
  : m_parent(parent),
    m_dt(dt, dtOwnership)
{
  if(dt != 0)
    m_data.resize(dt->size(), 0);
}

te::mem::DataSetItem::DataSetItem(const te::da::DataSetItem& rhs, te::da::DataSetType* dt)
  : m_parent(0),
    m_dt(dt, true)
{
  const std::size_t numOfValues = dt->size();

  m_data.reserve(numOfValues);

  for(std::size_t i = 0; i < numOfValues; ++i)
  {
    if(rhs.isNull(i) == false)
      m_data.push_back(rhs.getValue(i));
    else
      m_data.push_back(0);
  }
}

te::mem::DataSetItem::DataSetItem(const te::da::DataSetItem& rhs, DataSet* parent, te::da::DataSetType* dt, const bool dtOwnership)
  : m_parent(parent),
    m_dt(dt, dtOwnership)
{
  const std::size_t numOfValues = dt->size();

  m_data.reserve(numOfValues);

  for(std::size_t i = 0; i < numOfValues; ++i)
  {
    if(rhs.isNull(i) == false)
      m_data.push_back(rhs.getValue(i));
    else
      m_data.push_back(0);
  }
}

te::mem::DataSetItem::DataSetItem(const DataSetItem& rhs)
  : m_parent(rhs.m_parent),
    m_data(rhs.m_data),
    m_dt(0, false)
{
  if(!rhs.m_dt.m_isOwner)
  {
    m_dt.m_handle = rhs.m_dt.m_handle;
    m_dt.m_isOwner = false;
  }
  else
  {
    m_dt.m_handle = static_cast<te::da::DataSetType*>(rhs.m_dt.m_handle->clone());
    m_dt.m_isOwner = true;
  }
}

te::mem::DataSetItem::~DataSetItem()
{
}

te::mem::DataSetItem& te::mem::DataSetItem::operator=(const te::da::DataSetItem& rhs)
{
  assert(getType()->size() == rhs.getType()->size());

  if(this != &rhs)
  {
    const std::size_t numOfValues = rhs.getType()->size();

    for(std::size_t i = 0; i < numOfValues; ++i)
    { 
      if(rhs.isNull(i) == false)
        m_data.replace(i, rhs.getValue(i));
      else
        m_data.replace(i, 0);
    }

    if(rhs.getParent() != 0)
    {
      m_dt.m_handle = rhs.getParent()->getType();
      m_dt.m_isOwner = false;
    }
    else
    {
      m_dt.m_handle = static_cast<te::da::DataSetType*>(rhs.getType()->clone());
      m_dt.m_isOwner = true;
    }
  }

  return *this;
}

te::mem::DataSetItem& te::mem::DataSetItem::operator=(const DataSetItem& rhs)
{
  assert(getType()->size() == rhs.getType()->size());

  if(this != &rhs)
  {
    m_parent = rhs.m_parent;

    m_data = rhs.m_data;

    if(!rhs.m_dt.m_isOwner)
    {
      m_dt.m_handle = rhs.m_dt.m_handle;
      m_dt.m_isOwner = false;
    }
    else
    {
      m_dt.m_handle = static_cast<te::da::DataSetType*>(rhs.m_dt.m_handle->clone());
      m_dt.m_isOwner = true;
    }
  }

  return *this;
}

te::mem::DataSetItem* te::mem::DataSetItem::clone() const
{
  return new DataSetItem(*this);
}

te::da::DataSetType* te::mem::DataSetItem::getType()
{
  return m_dt.m_handle;
} 

const te::da::DataSetType* te::mem::DataSetItem::getType() const
{
  return m_dt.m_handle;
} 

te::da::DataSet* te::mem::DataSetItem::getParent() const
{
  return const_cast<te::da::DataSet*>(m_parent);
}       

char te::mem::DataSetItem::getChar(int i) const
{
  return static_cast<const te::dt::Char*>(&m_data[i])->getValue();
}

char te::mem::DataSetItem::getChar(const std::string& name) const
{
  std::size_t i = getType()->getPropertyPosition(name);
  return static_cast<const te::dt::Char*>(&m_data[i])->getValue();
}
        
void te::mem::DataSetItem::setChar(int i, char value) 
{
  m_data.replace(i, new te::dt::Char(value));
}

void te::mem::DataSetItem::setChar(const std::string& name, char value) 
{
  std::size_t i = getType()->getPropertyPosition(name);
  m_data.replace(i, new te::dt::Char(value));
}

unsigned char te::mem::DataSetItem::getUChar(int i) const
{
  return static_cast<const te::dt::UChar*>(&m_data[i])->getValue();
}

unsigned char te::mem::DataSetItem::getUChar(const std::string& name) const
{
  std::size_t i = getType()->getPropertyPosition(name);
  return static_cast<const te::dt::UChar*>(&m_data[i])->getValue();
}

void te::mem::DataSetItem::setUChar(int i, unsigned char value) 
{
  m_data.replace(i, new te::dt::UChar(value));
}

void te::mem::DataSetItem::setUChar(const std::string& name, unsigned char value) 
{
  std::size_t i = getType()->getPropertyPosition(name);
  m_data.replace(i, new te::dt::UChar(value));
}

boost::int16_t te::mem::DataSetItem::getInt16(int i) const
{
  return static_cast<const te::dt::Int16*>(&m_data[i])->getValue();  
}

boost::int16_t te::mem::DataSetItem::getInt16(const std::string& name) const
{
  std::size_t i = getType()->getPropertyPosition(name);
  return static_cast<const te::dt::Int16*>(&m_data[i])->getValue();
}

void te::mem::DataSetItem::setInt16(int i, boost::int16_t value) 
{
  m_data.replace(i, new te::dt::Int16(value));
}

void te::mem::DataSetItem::setInt16(const std::string& name, boost::int16_t value) 
{
  std::size_t i = getType()->getPropertyPosition(name);
  m_data.replace(i, new te::dt::Int16(value));
}

boost::int32_t te::mem::DataSetItem::getInt32(int i) const
{
  return static_cast<const te::dt::Int32*>(&m_data[i])->getValue(); 
}

boost::int32_t te::mem::DataSetItem::getInt32(const std::string& name) const
{
  std::size_t i = getType()->getPropertyPosition(name);
  return static_cast<const te::dt::Int32*>(&m_data[i])->getValue();
}

void te::mem::DataSetItem::setInt32(int i, boost::int32_t value) 
{
  m_data.replace(i, new te::dt::Int32(value));
}

void te::mem::DataSetItem::setInt32(const std::string& name, boost::int32_t value) 
{
  std::size_t i = getType()->getPropertyPosition(name);
  m_data.replace(i, new te::dt::Int32(value));
}

boost::int64_t te::mem::DataSetItem::getInt64(int i) const
{
  return static_cast<const te::dt::Int64*>(&m_data[i])->getValue();  
}

boost::int64_t te::mem::DataSetItem::getInt64(const std::string& name) const
{
  std::size_t i = getType()->getPropertyPosition(name);
  return static_cast<const te::dt::Int64*>(&m_data[i])->getValue();  
}

void te::mem::DataSetItem::setInt64(int i, boost::int64_t value) 
{
  m_data.replace(i, new te::dt::Int64(value));
}

void te::mem::DataSetItem::setInt64(const std::string& name, boost::int64_t value) 
{
  std::size_t i = getType()->getPropertyPosition(name);
  m_data.replace(i, new te::dt::Int64(value));
}

bool te::mem::DataSetItem::getBool(int i) const
{
  return static_cast<const te::dt::Boolean*>(&m_data[i])->getValue();  
}

bool te::mem::DataSetItem::getBool(const std::string& name) const
{
  std::size_t i = getType()->getPropertyPosition(name);
  return static_cast<const te::dt::Boolean*>(&m_data[i])->getValue();  
}

void te::mem::DataSetItem::setBool(int i, bool value) 
{
  m_data.replace(i, new te::dt::Boolean(value));
}

void te::mem::DataSetItem::setBool(const std::string& name, bool value) 
{
  std::size_t i = getType()->getPropertyPosition(name);
  m_data.replace(i, new te::dt::Boolean(value));
}

float te::mem::DataSetItem::getFloat(int i) const
{
  return static_cast<const te::dt::Float*>(&m_data[i])->getValue();  
}

float te::mem::DataSetItem::getFloat(const std::string& name) const
{
  std::size_t i = getType()->getPropertyPosition(name);
  return static_cast<const te::dt::Float*>(&m_data[i])->getValue();  
}

void te::mem::DataSetItem::setFloat(int i, float value) 
{
  m_data.replace(i, new te::dt::Float(value));
}

void te::mem::DataSetItem::setFloat(const std::string& name, float value) 
{
  std::size_t i = getType()->getPropertyPosition(name);
  m_data.replace(i, new te::dt::Float(value));
}

double te::mem::DataSetItem::getDouble(int i) const
{
  return static_cast<const te::dt::Double*>(&m_data[i])->getValue();  
}

double te::mem::DataSetItem::getDouble(const std::string& name) const
{
  std::size_t i = getType()->getPropertyPosition(name);
  return static_cast<const te::dt::Double*>(&m_data[i])->getValue();  
}

void te::mem::DataSetItem::setDouble(int i, double value) 
{
  m_data.replace(i, new te::dt::Double(value));
}

void te::mem::DataSetItem::setDouble(const std::string& name, double value) 
{
  std::size_t i = getType()->getPropertyPosition(name);
  m_data.replace(i, new te::dt::Double(value));
}

std::string te::mem::DataSetItem::getNumeric(int i) const
{
  return static_cast<const te::dt::Numeric*>(&m_data[i])->getValue();  
}

std::string te::mem::DataSetItem::getNumeric(const std::string& name) const
{
  std::size_t i = getType()->getPropertyPosition(name);
  return static_cast<const te::dt::Numeric*>(&m_data[i])->getValue();  
}

void te::mem::DataSetItem::setNumeric(int i, const std::string& value) 
{
  m_data.replace(i, new te::dt::Numeric(value));
}

void te::mem::DataSetItem::setNumeric(const std::string& name, const std::string& value) 
{
  std::size_t i = getType()->getPropertyPosition(name);
  m_data.replace(i, new te::dt::Numeric(value));
}

std::string te::mem::DataSetItem::getString(int i) const
{
  return static_cast<const te::dt::String*>(&m_data[i])->getValue();  
}

std::string te::mem::DataSetItem::getString(const std::string& name) const
{
  std::size_t i = getType()->getPropertyPosition(name);
  return static_cast<const te::dt::String*>(&m_data[i])->getValue();  
}

void te::mem::DataSetItem::setString(int i, const std::string& value) 
{
  m_data.replace(i, new te::dt::String(value));
}

void te::mem::DataSetItem::setString(const std::string& name, const std::string& value) 
{
  std::size_t i = getType()->getPropertyPosition(name);
  m_data.replace(i, new te::dt::String(value));
}

te::dt::ByteArray* te::mem::DataSetItem::getByteArray(int i) const
{
  te::dt::ByteArray* b = new te::dt::ByteArray(*static_cast<const te::dt::ByteArray*>(&m_data[i]));
  return b;
}

te::dt::ByteArray* te::mem::DataSetItem::getByteArray(const std::string& name) const
{
  std::size_t i = getType()->getPropertyPosition(name);
  return getByteArray(i);
}

void te::mem::DataSetItem::setByteArray(int i, const te::dt::ByteArray& value) 
{
  m_data.replace(i, new te::dt::ByteArray(value));
}

void te::mem::DataSetItem::setByteArray(const std::string& name, const te::dt::ByteArray& value) 
{
  std::size_t i = getType()->getPropertyPosition(name);
  m_data.replace(i, new te::dt::ByteArray(value));
}

te::gm::Geometry* te::mem::DataSetItem::getGeometry(int i) const
{
  return static_cast<te::gm::Geometry*>(m_data[i].clone());
}

te::gm::Geometry* te::mem::DataSetItem::getGeometry(const std::string& name) const
{
  std::size_t i = getType()->getPropertyPosition(name);
  return static_cast<te::gm::Geometry*>(m_data[i].clone());
}

void te::mem::DataSetItem::setGeometry(int i, const te::gm::Geometry& value)
{
  m_data.replace(i, value.clone());
}

void te::mem::DataSetItem::setGeometry(const std::string& name, const te::gm::Geometry& value)
{
  std::size_t i = getType()->getPropertyPosition(name);
  m_data.replace(i, value.clone());
}

te::rst::Raster* te::mem::DataSetItem::getRaster(int i) const
{
  return static_cast<te::rst::Raster*>(m_data[i].clone());
}

te::rst::Raster* te::mem::DataSetItem::getRaster(const std::string& name) const
{
  int idx = static_cast<int>(m_dt->getPropertyPosition(name));

  return getRaster(idx);
}

void te::mem::DataSetItem::setRaster(int i, const te::rst::Raster& value)
{
  m_data.replace(i, value.clone());
}

void te::mem::DataSetItem::setRaster(const std::string& name, const  te::rst::Raster& value)
{
  std::size_t i = getType()->getPropertyPosition(name);
  m_data.replace(i, value.clone());
}

te::dt::DateTime* te::mem::DataSetItem::getDateTime(int i) const
{
  return static_cast<te::dt::DateTime*>(m_data[i].clone());
}

te::dt::DateTime* te::mem::DataSetItem::getDateTime(const std::string& name) const
{
  std::size_t i = getType()->getPropertyPosition(name);
  return static_cast<te::dt::DateTime*>(m_data[i].clone());
}

void te::mem::DataSetItem::setDateTime(int i, const te::dt::DateTime& value) 
{
  m_data.replace(i, value.clone());
}

void te::mem::DataSetItem::setDateTime(const std::string& name, const te::dt::DateTime& value) 
{
  std::size_t i = getType()->getPropertyPosition(name);
  m_data.replace(i, value.clone());
}

void te::mem::DataSetItem::getArray(int /*i*/, std::vector<boost::int16_t>& /*values*/) const
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSetItem::getArray(const std::string& /*name*/, std::vector<boost::int16_t>& /*values*/) const
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

const unsigned char* te::mem::DataSetItem::getWKB(int /*i*/) const
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

const unsigned char* te::mem::DataSetItem::getWKB(const std::string& /*name*/) const
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::da::DataSet* te::mem::DataSetItem::getDataSet(int /*i*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::da::DataSet* te::mem::DataSetItem::getDataSet(const std::string& /*name*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSetItem::setDataSet(int /*i*/, const te::da::DataSet& /*value*/) 
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSetItem::setDataSet(const std::string& /*name*/, const te::da::DataSet& /*value*/) 
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSetItem::setValue(int i, te::dt::AbstractData* value)
{
  m_data.replace(i, value);
}

void te::mem::DataSetItem::setValue(const std::string& name, te::dt::AbstractData* value)
{
  std::size_t i = getType()->getPropertyPosition(name);
  m_data.replace(i, value);
} 

bool te::mem::DataSetItem::isNull(int i) const
{
  return boost::is_null(m_data.begin() + i);
}

bool te::mem::DataSetItem::isNull(const std::string& name) const
{
  std::size_t i = getType()->getPropertyPosition(name);
  return boost::is_null(m_data.begin() + i);
}

