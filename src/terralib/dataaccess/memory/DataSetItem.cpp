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
  \file terralib/dataaccess/memory/DataSetItem.cpp

  \brief An implementation of the DatasetItem class for the TerraLib In-Memory Data Access driver.
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../datatype/AbstractData.h"
#include "../../datatype/ByteArray.h"
#include "../../datatype/DateTime.h"
#include "../../datatype/Property.h"
#include "../../datatype/SimpleData.h"
#include "../../geometry/Geometry.h"
#include "../../raster/Grid.h"
#include "../../raster/RasterProperty.h"
#include "../core/dataset/DataSetType.h"
#include "../core/utils/Utils.h"
#include "DataSet.h"
#include "DataSetItem.h"
#include "Exception.h"
#include "Raster.h"

// STL
#include <cassert>

te::da::mem::DataSetItem::DataSetItem(const te::da::core::DataSet* parent)
  : m_parent(parent)
{
  if(parent == 0)
    return;

  std::size_t nproperties = parent->getNumProperties();

  m_data.resize(nproperties, 0);
}

te::da::mem::DataSetItem::DataSetItem(const DataSetItem& rhs)
  : m_parent(rhs.m_parent)
{
  m_data.reserve(rhs.getNumProperties());

  for(std::size_t i = 0; i < rhs.getNumProperties(); ++i)
  {
    if(rhs.isNull(i) == false)
      m_data.push_back(rhs.getValue(i));
    else
      m_data.push_back(0);
  }
}

te::da::mem::DataSetItem::~DataSetItem()
{
}

te::da::mem::DataSetItem& te::da::mem::DataSetItem::operator=(const DataSetItem& rhs)
{
  assert(getNumProperties() == rhs.getNumProperties());

  if(this != &rhs)
  {
    m_parent = rhs.m_parent;
    m_data = rhs.m_data;
  }

  return *this;
}

te::da::mem::DataSetItem* te::da::mem::DataSetItem::clone() const
{
  return new DataSetItem(*this);
}

te::da::core::DataSet* te::da::mem::DataSetItem::getParent() const
{
  return const_cast<te::da::core::DataSet*>(m_parent);
}

std::size_t te::da::mem::DataSetItem::getNumProperties() const
{
  return m_parent->getNumProperties();
}

int te::da::mem::DataSetItem::getPropertyDataType(std::size_t pos) const
{
  return m_parent->getPropertyDataType(pos);
}

std::string te::da::mem::DataSetItem::getPropertyName(std::size_t pos) const
{
  return m_parent->getPropertyName(pos);
}

char te::da::mem::DataSetItem::getChar(std::size_t i) const
{
  return static_cast<const te::dt::Char*>(&m_data[i])->getValue();
}

void te::da::mem::DataSetItem::setChar(std::size_t i, char value) 
{
  m_data.replace(i, new te::dt::Char(value));
}

void te::da::mem::DataSetItem::setChar(const std::string& name, char value)
{
  std::size_t pos = te::da::core::GetPropertyPos(m_parent, name);
  setChar(pos, value);
}

unsigned char te::da::mem::DataSetItem::getUChar(std::size_t i) const
{
  return static_cast<const te::dt::UChar*>(&m_data[i])->getValue();
}

void te::da::mem::DataSetItem::setUChar(std::size_t i, unsigned char value)
{
  m_data.replace(i, new te::dt::UChar(value));
}

void te::da::mem::DataSetItem::setUChar(const std::string& name, unsigned char value)
{
  std::size_t pos = te::da::core::GetPropertyPos(m_parent, name);
  setUChar(pos, value);
}

boost::int16_t te::da::mem::DataSetItem::getInt16(std::size_t i) const
{
  return static_cast<const te::dt::Int16*>(&m_data[i])->getValue();
}

void te::da::mem::DataSetItem::setInt16(std::size_t i, boost::int16_t value)
{
  m_data.replace(i, new te::dt::Int16(value));
}

void te::da::mem::DataSetItem::setInt16(const std::string& name, boost::int16_t value)
{
  std::size_t pos = te::da::core::GetPropertyPos(m_parent, name);
  setInt16(pos, value);
}

boost::int32_t te::da::mem::DataSetItem::getInt32(std::size_t i) const
{
  return static_cast<const te::dt::Int32*>(&m_data[i])->getValue();
}

void te::da::mem::DataSetItem::setInt32(std::size_t i, boost::int32_t value)
{
  m_data.replace(i, new te::dt::Int32(value));
}

void te::da::mem::DataSetItem::setInt32(const std::string& name, boost::int32_t value)
{
  std::size_t pos = te::da::core::GetPropertyPos(m_parent, name);
  setInt32(pos, value);
}

boost::int64_t te::da::mem::DataSetItem::getInt64(std::size_t i) const
{
  return static_cast<const te::dt::Int64*>(&m_data[i])->getValue();
}

void te::da::mem::DataSetItem::setInt64(std::size_t i, boost::int64_t value)
{
  m_data.replace(i, new te::dt::Int64(value));
}

void te::da::mem::DataSetItem::setInt64(const std::string& name, boost::int64_t value)
{
  std::size_t pos = te::da::core::GetPropertyPos(m_parent, name);
  setInt64(pos, value);
}

bool te::da::mem::DataSetItem::getBool(std::size_t i) const
{
  return static_cast<const te::dt::Boolean*>(&m_data[i])->getValue();  
}

void te::da::mem::DataSetItem::setBool(std::size_t i, bool value) 
{
  m_data.replace(i, new te::dt::Boolean(value));
}

void te::da::mem::DataSetItem::setBool(const std::string& name, bool value) 
{
  std::size_t pos = te::da::core::GetPropertyPos(m_parent, name);
  setBool(pos, value);
}

float te::da::mem::DataSetItem::getFloat(std::size_t i) const
{
  return static_cast<const te::dt::Float*>(&m_data[i])->getValue();  
}

void te::da::mem::DataSetItem::setFloat(std::size_t i, float value) 
{
  m_data.replace(i, new te::dt::Float(value));
}

void te::da::mem::DataSetItem::setFloat(const std::string& name, float value) 
{
  std::size_t pos = te::da::core::GetPropertyPos(m_parent, name);
  setFloat(pos, value);
}

double te::da::mem::DataSetItem::getDouble(std::size_t i) const
{
  return static_cast<const te::dt::Double*>(&m_data[i])->getValue();  
}

void te::da::mem::DataSetItem::setDouble(std::size_t i, double value) 
{
  m_data.replace(i, new te::dt::Double(value));
}

void te::da::mem::DataSetItem::setDouble(const std::string& name, double value) 
{
  std::size_t pos = te::da::core::GetPropertyPos(m_parent, name);
  setDouble(pos, value);
}

std::string te::da::mem::DataSetItem::getNumeric(std::size_t i) const
{
  return static_cast<const te::dt::Numeric*>(&m_data[i])->getValue();  
}

void te::da::mem::DataSetItem::setNumeric(std::size_t i, const std::string& value) 
{
  m_data.replace(i, new te::dt::Numeric(value));
}

void te::da::mem::DataSetItem::setNumeric(const std::string& name, const std::string& value) 
{
  std::size_t pos = te::da::core::GetPropertyPos(m_parent, name);
  setNumeric(pos, value);
}

std::string te::da::mem::DataSetItem::getString(std::size_t i) const
{
  return static_cast<const te::dt::String*>(&m_data[i])->getValue();  
}

void te::da::mem::DataSetItem::setString(std::size_t i, const std::string& value) 
{
  m_data.replace(i, new te::dt::String(value));
}

void te::da::mem::DataSetItem::setString(const std::string& name, const std::string& value) 
{
  std::size_t pos = te::da::core::GetPropertyPos(m_parent, name);
  setString(pos, value);
}

te::dt::ByteArray* te::da::mem::DataSetItem::getByteArray(std::size_t i) const
{
  te::dt::ByteArray* b = new te::dt::ByteArray(*static_cast<const te::dt::ByteArray*>(&m_data[i]));
  return b;
}

void te::da::mem::DataSetItem::setByteArray(std::size_t i, const te::dt::ByteArray& value) 
{
  m_data.replace(i, new te::dt::ByteArray(value));
}

void te::da::mem::DataSetItem::setByteArray(const std::string& name, const te::dt::ByteArray& value) 
{
  std::size_t pos = te::da::core::GetPropertyPos(m_parent, name);
  setByteArray(pos, value);
}

te::gm::Geometry* te::da::mem::DataSetItem::getGeometry(std::size_t i) const
{
  return static_cast<te::gm::Geometry*>(m_data[i].clone());
}

void te::da::mem::DataSetItem::setGeometry(std::size_t i, const te::gm::Geometry& value)
{
  m_data.replace(i, value.clone());
}

void te::da::mem::DataSetItem::setGeometry(const std::string& name, const te::gm::Geometry& value)
{
  std::size_t pos = te::da::core::GetPropertyPos(m_parent, name);
  setGeometry(pos, value);
}

te::rst::Raster* te::da::mem::DataSetItem::getRaster(std::size_t i) const
{
  return static_cast<te::rst::Raster*>(m_data[i].clone());
}

void te::da::mem::DataSetItem::setRaster(std::size_t i, const te::rst::Raster& value)
{
  m_data.replace(i, value.clone());
}

void te::da::mem::DataSetItem::setRaster(const std::string& name, const  te::rst::Raster& value)
{
  std::size_t pos = te::da::core::GetPropertyPos(m_parent, name);
  setRaster(pos, value);
}

te::dt::DateTime* te::da::mem::DataSetItem::getDateTime(std::size_t i) const
{
  return static_cast<te::dt::DateTime*>(m_data[i].clone());
}

void te::da::mem::DataSetItem::setDateTime(std::size_t i, const te::dt::DateTime& value) 
{
  m_data.replace(i, value.clone());
}

void te::da::mem::DataSetItem::setDateTime(const std::string& name, const te::dt::DateTime& value)
{
  std::size_t pos = te::da::core::GetPropertyPos(m_parent, name);
  setDateTime(pos, value);
}

te::dt::Array* te::da::mem::DataSetItem::getArray(std::size_t /*i*/) const
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::dt::AbstractData* te::da::mem::DataSetItem::getValue(std::size_t i) const
{
  switch(getPropertyDataType(i))
  {
    case te::dt::CHAR_TYPE:
      return new te::dt::Char(getChar(i));

    case te::dt::UCHAR_TYPE:
      return new te::dt::UChar(getUChar(i));

    case te::dt::INT16_TYPE:
      return new te::dt::Int16(getInt16(i));

    case te::dt::UINT16_TYPE:
      return new te::dt::UInt16(getInt16(i));

    case te::dt::INT32_TYPE:
      return new te::dt::Int32(getInt32(i));

    case te::dt::UINT32_TYPE:
      return new te::dt::UInt32(getInt32(i));

    case te::dt::INT64_TYPE:
      return new te::dt::Int64(getInt64(i));

    case te::dt::UINT64_TYPE:
      return new te::dt::UInt64(getInt64(i));

    case te::dt::BOOLEAN_TYPE:
      return new te::dt::Boolean(getBool(i));

    case te::dt::FLOAT_TYPE:
      return new te::dt::Float(getFloat(i));

    case te::dt::DOUBLE_TYPE:
      return new te::dt::Double(getDouble(i));

    case te::dt::NUMERIC_TYPE:
      return new te::dt::Numeric(getNumeric(i));

    case te::dt::STRING_TYPE:
      return new te::dt::String(getString(i));

    case te::dt::BYTE_ARRAY_TYPE:
       return getByteArray(i);

    case te::dt::GEOMETRY_TYPE:
      return getGeometry(i);

    case te::dt::DATETIME_TYPE:
      return getDateTime(i);

    case te::dt::RASTER_TYPE:
      return getRaster(i);

    default:
      return 0;
  }
}

void te::da::mem::DataSetItem::setValue(std::size_t i, te::dt::AbstractData* value)
{
  m_data.replace(i, value);
}

void te::da::mem::DataSetItem::setValue(const std::string& name, te::dt::AbstractData* value)
{
  std::size_t pos = te::da::core::GetPropertyPos(m_parent, name);
  setValue(pos, value);
} 

bool te::da::mem::DataSetItem::isNull(std::size_t i) const
{
  return boost::is_null(m_data.begin() + i);
}
