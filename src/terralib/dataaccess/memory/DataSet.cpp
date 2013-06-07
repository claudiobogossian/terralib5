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
  \file terralib/dataaccess/memory/DataSet.cpp

  \brief Implementation of a random-access dataset class for the TerraLib In-Memory Data Access driver.
*/

// TerraLib
#include "../../common/STLUtils.h"
#include "../../common/Translator.h"
#include "../../datatype/ByteArray.h"
#include "../../datatype/DataType.h"
#include "../../datatype/Property.h"
#include "../../datatype/SimpleData.h"
#include "../../datatype/Utils.h"
#include "../../geometry/Envelope.h"
#include "../../geometry/Geometry.h"
#include "../../geometry/Utils.h"
#include "../core/dataset/DataSetType.h"
#include "../core/utils/Utils.h"
#include "DataSet.h"
#include "DataSetItem.h"
#include "Exception.h"

// STL
#include <limits>

te::da::mem::DataSet::DataSet(const te::da::core::DataSetType* const dt)
  : m_items(new boost::ptr_vector<DataSetItem>),
    m_i(-1)
{
  te::da::core::GetPropertyInfo(dt, m_pnames, m_ptypes);
}

te::da::mem::DataSet::DataSet(te::da::core::DataSet& rhs)
  : m_items(new boost::ptr_vector<DataSetItem>),
    m_i(-1)
{
  te::da::core::GetPropertyInfo(&rhs, m_pnames, m_ptypes);

  copy(rhs, 0);
}

te::da::mem::DataSet::DataSet(const DataSet& rhs, const bool deepCopy)
  : m_items(),
    m_i(-1)
{
  te::da::core::GetPropertyInfo(&rhs, m_pnames, m_ptypes);

  if(deepCopy)
    m_items.reset(new boost::ptr_vector<DataSetItem>(*(rhs.m_items)));
  else
    m_items = rhs.m_items;
}

te::da::mem::DataSet::DataSet(te::da::core::DataSet& rhs, const std::vector<std::size_t>& properties, std::size_t limit)
  : m_items(),
    m_i(-1)
{
  for(std::size_t i = 0; i != properties.size(); ++i)
  {
    m_pnames.push_back(rhs.getPropertyName(properties[i]));
    m_ptypes.push_back(rhs.getPropertyDataType(properties[i]));
  }

  copy(rhs, properties, limit);
}

te::da::mem::DataSet::~DataSet()
{
}

void te::da::mem::DataSet::clear()
{
  m_items->clear();
}

void te::da::mem::DataSet::copy(te::da::core::DataSet& src, std::size_t limit)
{
  std::vector<std::size_t> properties;

  const std::size_t np = src.getNumProperties();

  for(std::size_t i = 0; i != np; ++i)
    properties.push_back(i);

  copy(src, properties, limit);
}

void te::da::mem::DataSet::copy(te::da::core::DataSet& src, const std::vector<std::size_t>& properties, std::size_t limit)
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

  do
  {
    std::auto_ptr<DataSetItem> item(new DataSetItem(this));

    for(std::size_t c = 0; c < nproperties; ++c)
    {
      if(!src.isNull(properties[c]))
        item->setValue(c, src.getValue(properties[c]));
      else
        item->setValue(c, 0);
    }

    m_items->push_back(item.release());

    ++i;

  }while(src.moveNext() && (i < limit));

  if(!unlimited & (i < limit))
    throw Exception(TR_MEMORY("The source dataset has few items than requested copy limit!"));
}

void te::da::mem::DataSet::add(DataSetItem* item)
{
  m_items->push_back(item);
}

void te::da::mem::DataSet::remove()
{
  m_items->erase(m_items->begin() + m_i);
}

void te::da::mem::DataSet::remove(DataSetItem* item)
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

  throw Exception(TR_DATAACCESS("Item not found in the dataset!"));
}

void te::da::mem::DataSet::add(const std::string& propertyName, std::size_t propertyType, const te::dt::AbstractData* defaultValue)
{
  m_pnames.push_back(propertyName);
  m_ptypes.push_back(propertyType);

  const std::size_t nitems = m_items->size();

  for(std::size_t i = 0; i < nitems; ++i)
    m_items->operator[](i).m_data.push_back(defaultValue ? defaultValue->clone() : 0);
}

void te::da::mem::DataSet::drop(std::size_t pos)
{
  const std::size_t nitems = m_items->size();

  for(std::size_t i = 0; i < nitems; ++i)
    m_items->operator[](i).m_data.erase( m_items->operator[](i).m_data.begin() + pos);

  m_pnames.erase(m_pnames.begin() + pos);
  m_ptypes.erase(m_ptypes.begin() + pos);
}

void te::da::mem::DataSet::update(te::dt::Property* /*prop*/)
{
  //std::size_t propPos = m_dt->getPropertyPosition(prop);

  //const std::size_t nitems = m_items->size();

  //for(std::size_t i = 0; i < nitems; ++i)
  //  m_items->operator[](i).m_data.replace(m_items->operator[](i).m_data.begin() + propPos, 0);
  throw Exception("No implemented yet!");
}

te::common::TraverseType te::da::mem::DataSet::getTraverseType() const 
{ 
  return te::common::RANDOM;
}

te::common::AccessPolicy te::da::mem::DataSet::getAccessPolicy() const 
{
  return te::common::RWAccess;
}

te::gm::Envelope* te::da::mem::DataSet::getExtent(std::size_t i)
{
  std::auto_ptr<te::gm::Envelope> mbr(new te::gm::Envelope);

  const std::size_t nitems = m_items->size();
  
  for(std::size_t ii = 0; ii < nitems; ++ii)
  {
    std::auto_ptr<te::gm::Geometry> geom((*m_items)[ii].getGeometry(i));

    mbr->Union(*(geom->getMBR()));
  }

  return mbr.release();
}

std::size_t te::da::mem::DataSet::getNumProperties() const
{
  return m_pnames.size();
}

int te::da::mem::DataSet::getPropertyDataType(std::size_t pos) const
{
  return m_ptypes[pos];
}

void te::da::mem::DataSet::setPropertyDataType(int dt, std::size_t pos)
{
  m_ptypes[pos] = dt;
}

std::string te::da::mem::DataSet::getPropertyName(std::size_t pos) const
{
  return m_pnames[pos];
}

void te::da::mem::DataSet::setPropertyName(const std::string& name, std::size_t pos)
{
  m_pnames[pos] = name;
}

std::string te::da::mem::DataSet::getDatasetNameOfProperty(std::size_t /*pos*/) const
{
  throw Exception("No implemented yet!");
}

te::da::mem::DataSetItem* te::da::mem::DataSet::getItem() const
{
  return (*m_items)[m_i].clone();
}

bool te::da::mem::DataSet::isEmpty() const
{
  return m_items->empty();
}

std::size_t te::da::mem::DataSet::size() const
{
  return m_items->size();
}

bool te::da::mem::DataSet::moveNext()
{
  ++m_i;
  return m_i < static_cast<int>(m_items->size());
}

bool te::da::mem::DataSet::movePrevious()
{
  --m_i;
  return m_i > 0;
}

bool te::da::mem::DataSet::moveBeforeFirst()
{
  m_i = -1;
  return true;
}

bool te::da::mem::DataSet::moveFirst()
{
  m_i = 0;
  return true;
}

bool te::da::mem::DataSet::moveLast()
{
  m_i = m_items->size() - 1;
  return m_i < static_cast<int>(m_items->size());
}

bool te::da::mem::DataSet::move(std::size_t i)
{
  m_i = i;
  return m_i < static_cast<int>(m_items->size());
}

bool te::da::mem::DataSet::isAtBegin() const
{
  return m_i == 0;
}

bool te::da::mem::DataSet::isBeforeBegin() const
{
  return m_i < 0;
}

bool te::da::mem::DataSet::isAtEnd() const
{
  return m_i == (static_cast<int>(m_items->size()) - 1);
}

bool te::da::mem::DataSet::isAfterEnd() const
{
  return m_i > static_cast<int>(m_items->size());
}

char te::da::mem::DataSet::getChar(std::size_t i) const
{
  return (*m_items)[m_i].getChar(i);
}

void te::da::mem::DataSet::setChar(std::size_t i, char value)
{
  (*m_items)[m_i].setChar(i, value);
}

void te::da::mem::DataSet::setChar(const std::string& name, char value) 
{
  (*m_items)[m_i].setChar(name, value);
}

unsigned char te::da::mem::DataSet::getUChar(std::size_t i) const
{
  return (*m_items)[m_i].getUChar(i);
}

void te::da::mem::DataSet::setUChar(std::size_t i, unsigned char value)
{
  (*m_items)[m_i].setUChar(i, value);
}

void te::da::mem::DataSet::setUChar(const std::string& name, unsigned char value)
{
  (*m_items)[m_i].setUChar(name, value);
}

boost::int16_t te::da::mem::DataSet::getInt16(std::size_t i) const
{
  return (*m_items)[m_i].getInt16(i);
}

void te::da::mem::DataSet::setInt16(std::size_t i, boost::int16_t value)
{
  (*m_items)[m_i].setInt16(i, value);
}

void te::da::mem::DataSet::setInt16(const std::string& name, boost::int16_t value)
{
  (*m_items)[m_i].setInt16(name, value);
}

boost::int32_t te::da::mem::DataSet::getInt32(std::size_t i) const
{
  return (*m_items)[m_i].getInt32(i);
}

void te::da::mem::DataSet::setInt32(std::size_t i, boost::int32_t value)
{
  (*m_items)[m_i].setInt32(i, value);
}

void te::da::mem::DataSet::setInt32(const std::string& name, boost::int32_t value)
{
  (*m_items)[m_i].setInt32(name, value);
}

boost::int64_t te::da::mem::DataSet::getInt64(std::size_t i) const
{
  return (*m_items)[m_i].getInt64(i);
}

void te::da::mem::DataSet::setInt64(std::size_t i, boost::int64_t value)
{
  (*m_items)[m_i].setInt64(i, value);
}

void te::da::mem::DataSet::setInt64(const std::string& name, boost::int64_t value) 
{
  (*m_items)[m_i].setInt64(name, value);
}

bool te::da::mem::DataSet::getBool(std::size_t i) const
{
  return (*m_items)[m_i].getBool(i);
}

void te::da::mem::DataSet::setBool(std::size_t i, bool value)
{
  (*m_items)[m_i].setBool(i, value);
}

void te::da::mem::DataSet::setBool(const std::string& name, bool value)
{
  (*m_items)[m_i].setBool(name, value);
}

float te::da::mem::DataSet::getFloat(std::size_t i) const
{
  return (*m_items)[m_i].getFloat(i);
}

void te::da::mem::DataSet::setFloat(std::size_t i, float value)
{
  (*m_items)[m_i].setFloat(i, value);
}

void te::da::mem::DataSet::setFloat(const std::string& name, float value)
{
  (*m_items)[m_i].setFloat(name, value);
}

double te::da::mem::DataSet::getDouble(std::size_t i) const
{
  return (*m_items)[m_i].getDouble(i);
}

void te::da::mem::DataSet::setDouble(std::size_t i, double value)
{
  (*m_items)[m_i].setDouble(i, value);
}

void te::da::mem::DataSet::setDouble(const std::string& name, double value)
{
  (*m_items)[m_i].setDouble(name, value);
}

std::string te::da::mem::DataSet::getNumeric(std::size_t i) const
{
  return (*m_items)[m_i].getNumeric(i);
}

void te::da::mem::DataSet::setNumeric(std::size_t i, const std::string& value)
{
  (*m_items)[m_i].setNumeric(i, value);
}

void te::da::mem::DataSet::setNumeric(const std::string& name, const std::string& value) 
{
  (*m_items)[m_i].setNumeric(name, value);
}

std::string te::da::mem::DataSet::getString(std::size_t i) const
{
  return (*m_items)[m_i].getString(i);
}

void te::da::mem::DataSet::setString(std::size_t i, const std::string& value) 
{
  (*m_items)[m_i].setString(i, value);
}

void te::da::mem::DataSet::setString(const std::string& name, const std::string& value) 
{
  (*m_items)[m_i].setString(name, value);
}

te::dt::ByteArray* te::da::mem::DataSet::getByteArray(std::size_t i) const
{
  return (*m_items)[m_i].getByteArray(i);
}

void te::da::mem::DataSet::setByteArray(std::size_t i, const te::dt::ByteArray& value)
{
  (*m_items)[m_i].setByteArray(i, value);
}

void te::da::mem::DataSet::setByteArray(const std::string& name, const te::dt::ByteArray& value) 
{
  (*m_items)[m_i].setByteArray(name, value);
}

te::gm::Geometry* te::da::mem::DataSet::getGeometry(std::size_t i) const
{
  return (*m_items)[m_i].getGeometry(i);
}

void te::da::mem::DataSet::setGeometry(std::size_t i, const te::gm::Geometry& value)
{
  (*m_items)[m_i].setGeometry(i, value);
}

void te::da::mem::DataSet::setGeometry(const std::string& name, const te::gm::Geometry& value) 
{
  (*m_items)[m_i].setGeometry(name, value);
}

te::rst::Raster* te::da::mem::DataSet::getRaster(std::size_t i) const
{
  return (*m_items)[m_i].getRaster(i);
}

void te::da::mem::DataSet::setRaster(std::size_t i, const te::rst::Raster& value)
{
  (*m_items)[m_i].setRaster(i, value);
}

void te::da::mem::DataSet::setRaster(const std::string& name, const te::rst::Raster& value)
{
  (*m_items)[m_i].setRaster(name, value);
}

te::dt::DateTime* te::da::mem::DataSet::getDateTime(std::size_t i) const
{
  return (*m_items)[m_i].getDateTime(i);
}

void te::da::mem::DataSet::setDateTime(std::size_t i, const te::dt::DateTime& value) 
{
  (*m_items)[m_i].setDateTime(i, value);
}

void te::da::mem::DataSet::setDateTime(const std::string& name, const te::dt::DateTime& value) 
{
  (*m_items)[m_i].setDateTime(name, value);
}

te::dt::Array* te::da::mem::DataSet::getArray(std::size_t i) const
{
  return (*m_items)[m_i].getArray(i);
}

te::dt::AbstractData* te::da::mem::DataSet::getValue(std::size_t i) const
{
  return (*m_items)[m_i].getValue(i);
}

void te::da::mem::DataSet::setValue(std::size_t i, te::dt::AbstractData* value)
{
  (*m_items)[m_i].setValue(i, value);
}

void te::da::mem::DataSet::setValue(const std::string& name, te::dt::AbstractData* ad)
{
  (*m_items)[m_i].setValue(name, ad);
}

bool te::da::mem::DataSet::isNull(std::size_t i) const
{
  return (*m_items)[m_i].isNull(i);
}
