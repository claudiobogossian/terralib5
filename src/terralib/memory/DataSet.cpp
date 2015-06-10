/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/memory/DataSet.cpp

  \brief Implementation of a random-access dataset class for the TerraLib In-Memory Data Access driver.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/ByteArray.h"
#include "../datatype/DataType.h"
#include "../datatype/Property.h"
#include "../datatype/SimpleData.h"
#include "../datatype/StringProperty.h"
#include "../datatype/Utils.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../geometry/Utils.h"
#include "DataSet.h"
#include "DataSetItem.h"
#include "Exception.h"

// STL
#include <limits>

te::mem::DataSet::DataSet(const te::da::DataSetType* const dt)
  : m_items(new boost::ptr_vector<DataSetItem>),
    m_i(-1)
{
  te::da::GetPropertyInfo(dt, m_pnames, m_ptypes);

  // Fill char-encodings
  for(std::size_t i = 0; i < dt->size(); ++i)
  {
    te::dt::StringProperty* p = dynamic_cast<te::dt::StringProperty*>(dt->getProperty(i));
    if(p != 0)
      m_encodings[i] = p->getCharEncoding();
  }
}

te::mem::DataSet::DataSet(te::da::DataSet& rhs)
  : m_items(new boost::ptr_vector<DataSetItem>),
    m_i(-1)
{
  te::da::GetPropertyInfo(&rhs, m_pnames, m_ptypes);

  // Fill char-encodings
  for(std::size_t i = 0; i < m_ptypes.size(); ++i)
    if(m_ptypes[i] == te::dt::STRING_TYPE)
      m_encodings[i] = rhs.getPropertyCharEncoding(i);

  copy(rhs, 0);
}

te::mem::DataSet::DataSet(const DataSet& rhs, const bool deepCopy)
  : m_items(new boost::ptr_vector<DataSetItem>),
    m_i(-1)
{
  te::da::GetPropertyInfo(&rhs, m_pnames, m_ptypes);

  // Fill char-encodings
  for(std::size_t i = 0; i < m_ptypes.size(); ++i)
    if(m_ptypes[i] == te::dt::STRING_TYPE)
      m_encodings[i] = rhs.getPropertyCharEncoding(i);

  if(deepCopy)
    m_items.reset(new boost::ptr_vector<DataSetItem>(*(rhs.m_items)));
  else
    m_items = rhs.m_items;
}

te::mem::DataSet::DataSet(te::da::DataSet& rhs, const std::vector<std::size_t>& properties, std::size_t limit)
  : m_items(new boost::ptr_vector<DataSetItem>),
    m_i(-1)
{
  for(std::size_t i = 0; i != properties.size(); ++i)
  {
    int ptype = rhs.getPropertyDataType(properties[i]);

    m_pnames.push_back(rhs.getPropertyName(properties[i]));
    m_ptypes.push_back(ptype);
    if(ptype == te::dt::STRING_TYPE)
      m_encodings[properties[i]] = rhs.getPropertyCharEncoding(properties[i]);
  }

  copy(rhs, properties, limit);
}

te::mem::DataSet::~DataSet()
{
}

void te::mem::DataSet::clear()
{
  m_items->clear();
}

void te::mem::DataSet::copy(te::da::DataSet& src, std::size_t limit)
{
  std::vector<std::size_t> properties;

  const std::size_t np = src.getNumProperties();

  for(std::size_t i = 0; i != np; ++i)
    properties.push_back(i);

  copy(src, properties, limit);
}

void te::mem::DataSet::copy(te::da::DataSet& src, const std::vector<std::size_t>& properties, std::size_t limit)
{
  bool unlimited = true;

  if(limit == 0)
  {
    limit = std::numeric_limits<std::size_t>::max();
  }
  else
  {
    m_items->reserve(m_items->size() + limit);
    unlimited = false;
  }

  std::size_t i = 0;

  const std::size_t nproperties = properties.size();

  while(src.moveNext() && (i < limit))
  {
    std::auto_ptr<DataSetItem> item(new DataSetItem(this));

    for(std::size_t c = 0; c < nproperties; ++c)
    {
      if(!src.isNull(properties[c]))
        item->setValue(c, src.getValue(properties[c]).release());
      else
        item->setValue(c, 0);
    }

    m_items->push_back(item.release());

    ++i;
  }

  //src.moveBeforeFirst();

  if(!unlimited & (i < limit))
    throw Exception(TE_TR("The source dataset has few items than requested copy limit!"));
}

void te::mem::DataSet::add(DataSetItem* item)
{
  m_items->push_back(item);
}

void te::mem::DataSet::remove()
{
  m_items->erase(m_items->begin() + m_i);
}

void te::mem::DataSet::remove(DataSetItem* item)
{
  const std::size_t nitems = m_items->size();

  for(std::size_t i = 0; i < nitems; ++i)
  {
    if(&(m_items->operator[](i)) == item) 
    {
      m_items->erase(m_items->begin() + i);
      break;
    }
  }

  throw Exception(TE_TR("Item not found in the dataset!"));
}

void te::mem::DataSet::add(const std::string& propertyName, std::size_t propertyType, const te::dt::AbstractData* defaultValue)
{
  m_pnames.push_back(propertyName);
  m_ptypes.push_back(propertyType);

  const std::size_t nitems = m_items->size();

  for(std::size_t i = 0; i < nitems; ++i)
    m_items->operator[](i).m_data.push_back(defaultValue ? defaultValue->clone() : 0);
}

void te::mem::DataSet::drop(std::size_t pos)
{
  const std::size_t nitems = m_items->size();

  for(std::size_t i = 0; i < nitems; ++i)
    m_items->operator[](i).m_data.erase( m_items->operator[](i).m_data.begin() + pos);

  m_pnames.erase(m_pnames.begin() + pos);
  m_ptypes.erase(m_ptypes.begin() + pos);
}

void te::mem::DataSet::update(te::dt::Property* /*prop*/)
{
  //std::size_t propPos = m_dt->getPropertyPosition(prop);

  //const std::size_t nitems = m_items->size();

  //for(std::size_t i = 0; i < nitems; ++i)
  //  m_items->operator[](i).m_data.replace(m_items->operator[](i).m_data.begin() + propPos, 0);
  throw Exception("No implemented yet!");
}

te::common::TraverseType te::mem::DataSet::getTraverseType() const 
{ 
  return te::common::RANDOM;
}

te::common::AccessPolicy te::mem::DataSet::getAccessPolicy() const 
{
  return te::common::RWAccess;
}

std::auto_ptr<te::gm::Envelope> te::mem::DataSet::getExtent(std::size_t i)
{
  std::auto_ptr<te::gm::Envelope> mbr(new te::gm::Envelope);

  const std::size_t nitems = m_items->size();
  
  for(std::size_t ii = 0; ii < nitems; ++ii)
  {
    std::auto_ptr<te::gm::Geometry> geom((*m_items)[ii].getGeometry(i));

    mbr->Union(*(geom->getMBR()));
  }

  return mbr;
}

std::size_t te::mem::DataSet::getNumProperties() const
{
  return m_pnames.size();
}

int te::mem::DataSet::getPropertyDataType(std::size_t pos) const
{
  return m_ptypes[pos];
}

void te::mem::DataSet::setPropertyDataType(int dt, std::size_t pos)
{
  m_ptypes[pos] = dt;
}

std::string te::mem::DataSet::getPropertyName(std::size_t pos) const
{
  return m_pnames[pos];
}

void te::mem::DataSet::setPropertyName(const std::string& name, std::size_t pos)
{
  m_pnames[pos] = name;
}

te::common::CharEncoding te::mem::DataSet::getPropertyCharEncoding(std::size_t i) const
{
  if (m_encodings.size()== 0) return te::common::UNKNOWN_CHAR_ENCODING;
  std::map<int, te::common::CharEncoding>::const_iterator it = m_encodings.find(i);
  assert(it != m_encodings.end());

  return it->second;
}

std::string te::mem::DataSet::getDatasetNameOfProperty(std::size_t /*pos*/) const
{
  throw Exception("No implemented yet!");
}

te::mem::DataSetItem* te::mem::DataSet::getItem() const
{
  return (*m_items)[m_i].clone().get();
}

bool te::mem::DataSet::isEmpty() const
{
  return m_items->empty();
}

bool te::mem::DataSet::isConnected() const
{
  return false;
}

std::size_t te::mem::DataSet::size() const
{
  return m_items->size();
}

bool te::mem::DataSet::moveNext()
{
  ++m_i;
  return m_i < static_cast<int>(m_items->size());
}

bool te::mem::DataSet::movePrevious()
{
  --m_i;
  return m_i > 0;
}

bool te::mem::DataSet::moveBeforeFirst()
{
  m_i = -1;
  return true;
}

bool te::mem::DataSet::moveFirst()
{
  m_i = 0;
  return true;
}

bool te::mem::DataSet::moveLast()
{
  m_i = m_items->size() - 1;
  return m_i < static_cast<int>(m_items->size());
}

bool te::mem::DataSet::move(std::size_t i)
{
  m_i = i;
  return m_i < static_cast<int>(m_items->size());
}

bool te::mem::DataSet::isAtBegin() const
{
  return m_i == 0;
}

bool te::mem::DataSet::isBeforeBegin() const
{
  return m_i < 0;
}

bool te::mem::DataSet::isAtEnd() const
{
  return m_i == (static_cast<int>(m_items->size()) - 1);
}

bool te::mem::DataSet::isAfterEnd() const
{
  return m_i > static_cast<int>(m_items->size());
}

char te::mem::DataSet::getChar(std::size_t i) const
{
  return (*m_items)[m_i].getChar(i);
}

void te::mem::DataSet::setChar(std::size_t i, char value)
{
  (*m_items)[m_i].setChar(i, value);
}

void te::mem::DataSet::setChar(const std::string& name, char value) 
{
  (*m_items)[m_i].setChar(name, value);
}

unsigned char te::mem::DataSet::getUChar(std::size_t i) const
{
  return (*m_items)[m_i].getUChar(i);
}

void te::mem::DataSet::setUChar(std::size_t i, unsigned char value)
{
  (*m_items)[m_i].setUChar(i, value);
}

void te::mem::DataSet::setUChar(const std::string& name, unsigned char value)
{
  (*m_items)[m_i].setUChar(name, value);
}

boost::int16_t te::mem::DataSet::getInt16(std::size_t i) const
{
  return (*m_items)[m_i].getInt16(i);
}

void te::mem::DataSet::setInt16(std::size_t i, boost::int16_t value)
{
  (*m_items)[m_i].setInt16(i, value);
}

void te::mem::DataSet::setInt16(const std::string& name, boost::int16_t value)
{
  (*m_items)[m_i].setInt16(name, value);
}

boost::int32_t te::mem::DataSet::getInt32(std::size_t i) const
{
  return (*m_items)[m_i].getInt32(i);
}

void te::mem::DataSet::setInt32(std::size_t i, boost::int32_t value)
{
  (*m_items)[m_i].setInt32(i, value);
}

void te::mem::DataSet::setInt32(const std::string& name, boost::int32_t value)
{
  (*m_items)[m_i].setInt32(name, value);
}

boost::int64_t te::mem::DataSet::getInt64(std::size_t i) const
{
  return (*m_items)[m_i].getInt64(i);
}

void te::mem::DataSet::setInt64(std::size_t i, boost::int64_t value)
{
  (*m_items)[m_i].setInt64(i, value);
}

void te::mem::DataSet::setInt64(const std::string& name, boost::int64_t value) 
{
  (*m_items)[m_i].setInt64(name, value);
}

bool te::mem::DataSet::getBool(std::size_t i) const
{
  return (*m_items)[m_i].getBool(i);
}

void te::mem::DataSet::setBool(std::size_t i, bool value)
{
  (*m_items)[m_i].setBool(i, value);
}

void te::mem::DataSet::setBool(const std::string& name, bool value)
{
  (*m_items)[m_i].setBool(name, value);
}

float te::mem::DataSet::getFloat(std::size_t i) const
{
  return (*m_items)[m_i].getFloat(i);
}

void te::mem::DataSet::setFloat(std::size_t i, float value)
{
  (*m_items)[m_i].setFloat(i, value);
}

void te::mem::DataSet::setFloat(const std::string& name, float value)
{
  (*m_items)[m_i].setFloat(name, value);
}

double te::mem::DataSet::getDouble(std::size_t i) const
{
  return (*m_items)[m_i].getDouble(i);
}

void te::mem::DataSet::setDouble(std::size_t i, double value)
{
  (*m_items)[m_i].setDouble(i, value);
}

void te::mem::DataSet::setDouble(const std::string& name, double value)
{
  (*m_items)[m_i].setDouble(name, value);
}

std::string te::mem::DataSet::getNumeric(std::size_t i) const
{
  return (*m_items)[m_i].getNumeric(i);
}

void te::mem::DataSet::setNumeric(std::size_t i, const std::string& value)
{
  (*m_items)[m_i].setNumeric(i, value);
}

void te::mem::DataSet::setNumeric(const std::string& name, const std::string& value) 
{
  (*m_items)[m_i].setNumeric(name, value);
}

std::string te::mem::DataSet::getString(std::size_t i) const
{
  return (*m_items)[m_i].getString(i);
}

void te::mem::DataSet::setString(std::size_t i, const std::string& value) 
{
  (*m_items)[m_i].setString(i, value);
}

void te::mem::DataSet::setString(const std::string& name, const std::string& value) 
{
  (*m_items)[m_i].setString(name, value);
}

std::auto_ptr<te::dt::ByteArray> te::mem::DataSet::getByteArray(std::size_t i) const
{
  return (*m_items)[m_i].getByteArray(i);
}

void te::mem::DataSet::setByteArray(std::size_t i, te::dt::ByteArray* value)
{
  (*m_items)[m_i].setByteArray(i, value);
}

void te::mem::DataSet::setByteArray(const std::string& name, te::dt::ByteArray* value) 
{
  (*m_items)[m_i].setByteArray(name, value);
}

std::auto_ptr<te::gm::Geometry> te::mem::DataSet::getGeometry(std::size_t i) const
{
  return (*m_items)[m_i].getGeometry(i);
}

void te::mem::DataSet::setGeometry(std::size_t i, te::gm::Geometry* value)
{
  (*m_items)[m_i].setGeometry(i, value);
}

void te::mem::DataSet::setGeometry(const std::string& name, te::gm::Geometry* value) 
{
  (*m_items)[m_i].setGeometry(name, value);
}

std::auto_ptr<te::rst::Raster> te::mem::DataSet::getRaster(std::size_t i) const
{
  return (*m_items)[m_i].getRaster(i);
}

void te::mem::DataSet::setRaster(std::size_t i, te::rst::Raster* value)
{
  (*m_items)[m_i].setRaster(i, value);
}

void te::mem::DataSet::setRaster(const std::string& name, te::rst::Raster* value)
{
  (*m_items)[m_i].setRaster(name, value);
}

std::auto_ptr<te::dt::DateTime> te::mem::DataSet::getDateTime(std::size_t i) const
{
  return std::auto_ptr<te::dt::DateTime>((*m_items)[m_i].getDateTime(i));
}

void te::mem::DataSet::setDateTime(std::size_t i, te::dt::DateTime* value) 
{
  (*m_items)[m_i].setDateTime(i, value);
}

void te::mem::DataSet::setDateTime(const std::string& name, te::dt::DateTime* value) 
{
  (*m_items)[m_i].setDateTime(name, value);
}

std::auto_ptr<te::dt::Array> te::mem::DataSet::getArray(std::size_t i) const
{
  return std::auto_ptr<te::dt::Array>(0);
}

std::auto_ptr<te::dt::AbstractData> te::mem::DataSet::getValue(std::size_t i) const
{
  return (*m_items)[m_i].getValue(i);
}

void te::mem::DataSet::setValue(std::size_t i, te::dt::AbstractData* value)
{
  (*m_items)[m_i].setValue(i, value);
}

void te::mem::DataSet::setValue(const std::string& name, te::dt::AbstractData* ad)
{
  (*m_items)[m_i].setValue(name, ad);
}

bool te::mem::DataSet::isNull(std::size_t i) const
{
  return (*m_items)[m_i].isNull(i);
}
