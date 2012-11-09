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
  \file terralib/mem/DataSet.cpp

  \brief Implementation of a random-access dataset class for the TerraLib In-Memory Data Access driver.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../datatype/ByteArray.h"
#include "../datatype/DataType.h"
#include "../datatype/Property.h"
#include "../datatype/SimpleData.h"
#include "../datatype/Utils.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../geometry/Utils.h"
#include "DataSet.h"
#include "DataSetItem.h"
#include "DataSourceTransactor.h"
#include "Exception.h"

// STL
#include <limits>

te::mem::DataSet::DataSet(te::da::DataSetType* dt, DataSourceTransactor* t)
  : m_items(new boost::ptr_vector<te::mem::DataSetItem>),
    m_dt(dt),
    m_parent(0),
    m_t(t),
    m_i(-1)
{
}

te::mem::DataSet::DataSet(te::da::DataSet& rhs)
  : m_items(new boost::ptr_vector<te::mem::DataSetItem>),
    m_dt(0),
    m_parent(0),
    m_t(0),
    m_i(-1)
{
  if(rhs.getType())
    m_dt.reset(static_cast<te::da::DataSetType*>(rhs.getType()->clone()));

  copy(rhs, 0);
}

te::mem::DataSet::DataSet(const DataSet& rhs, const bool deepCopy)
  : m_items(),
    m_dt(0),
    m_parent(0),
    m_t(rhs.m_t),
    m_i(-1)
{
  if(rhs.getType())
    m_dt.reset(static_cast<te::da::DataSetType*>(rhs.getType()->clone()));

  if(deepCopy)
    m_items.reset(new boost::ptr_vector<te::mem::DataSetItem>(*(rhs.m_items)));
  else
    m_items = rhs.m_items;
}

te::mem::DataSet::DataSet(te::da::DataSet& rhs, const std::vector<te::dt::Property*>& properties, std::size_t limit)
  : m_items(),
    m_dt(0),
    m_parent(0),
    m_t(0),
    m_i(-1)
{
  m_dt.reset(new te::da::DataSetType("", 0));

  te::common::Clone(properties, m_dt->getProperties());

  copy(rhs, properties, limit);  
}

te::mem::DataSet::~DataSet()
{
}

void te::mem::DataSet::copy(te::da::DataSet& src, std::size_t limit)
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

  const std::size_t nproperties = src.getType()->size();

  do
  {
    std::auto_ptr<te::mem::DataSetItem> item(new te::mem::DataSetItem(this, m_dt.get(), false));

    for(std::size_t c = 0; c < nproperties; ++c)
    {
      if(!src.isNull(c))
        item->setValue(c, src.getValue(c));
      else
        item->setValue(c, 0);
    }

    m_items->push_back(item.release());

    ++i;

  }while(src.moveNext() && (i < limit));

  if(!unlimited & (i < limit))
    throw Exception(TR_MEMORY("The source dataset has few items than requested copy limit!"));
}

void te::mem::DataSet::copy(te::da::DataSet& src, const std::vector<te::dt::Property*>& properties, std::size_t limit)
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

  std::vector<std::size_t> poslist;

  te::dt::GetPropertiesPosition(properties, src.getType(), poslist);

  const std::size_t nproperties = properties.size();

  do
  {
    std::auto_ptr<te::mem::DataSetItem> item(new te::mem::DataSetItem(this, m_dt.get(), false));

    for(std::size_t c = 0; c < nproperties; ++c)
    {
      if(!src.isNull(poslist[c]))
        item->setValue(c, src.getValue(poslist[c]));
      else
        item->setValue(c, 0);
    }

    m_items->push_back(item.release());

    ++i;

  }while(src.moveNext() && (i < limit));

  if(!unlimited & (i < limit))
    throw Exception(TR_MEMORY("The source dataset has few items than requested copy limit!"));
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

  throw Exception(TR_DATAACCESS("Item not found in the dataset!"));
}

void te::mem::DataSet::add(te::dt::Property* prop, const te::dt::AbstractData* defaultValue)
{
  m_dt->add(prop);

  const std::size_t nitems = m_items->size();

  for(std::size_t i = 0; i < nitems; ++i)
    m_items->operator[](i).m_data.push_back(defaultValue ? defaultValue->clone() : 0);
}

void te::mem::DataSet::drop(te::dt::Property* prop)
{
  std::size_t propPos = m_dt->getPropertyPosition(prop);

  const std::size_t nitems = m_items->size();

  for(std::size_t i = 0; i < nitems; ++i)
    m_items->operator[](i).m_data.erase( m_items->operator[](i).m_data.begin() + propPos);

  m_dt->remove(prop);
}

void te::mem::DataSet::update(te::dt::Property* prop)
{
  std::size_t propPos = m_dt->getPropertyPosition(prop);

  const std::size_t nitems = m_items->size();

  for(std::size_t i = 0; i < nitems; ++i)
    m_items->operator[](i).m_data.replace(m_items->operator[](i).m_data.begin() + propPos, 0);
}

void te::mem::DataSet::setTransactor(DataSourceTransactor* t)
{
  m_t = t;
}

te::common::TraverseType te::mem::DataSet::getTraverseType() const 
{ 
  return te::common::RANDOM;
}

te::common::AccessPolicy te::mem::DataSet::getAccessPolicy() const 
{
  return te::common::RWAccess;
}

te::da::DataSetType* te::mem::DataSet::getType()
{
  return m_dt.get();
}

const te::da::DataSetType* te::mem::DataSet::getType() const
{
  return m_dt.get();
}

te::da::DataSourceTransactor* te::mem::DataSet::getTransactor() const
{
  return m_t;
}

void te::mem::DataSet::loadTypeInfo()
{
// don't need to implement
}

te::da::DataSet* te::mem::DataSet::getParent() const
{
  return m_parent;
}

te::gm::Envelope* te::mem::DataSet::getExtent(const te::dt::Property* p)
{
  std::size_t idx = m_dt->getPropertyPosition(p);
  
  std::auto_ptr<te::gm::Envelope> mbr(new te::gm::Envelope);

  const std::size_t nitems = m_items->size();
  
  for(std::size_t i = 0; i < nitems; ++i)
  {
    std::auto_ptr<te::gm::Geometry> geom((*m_items)[i].getGeometry(idx));

    mbr->Union(*(geom->getMBR()));
  }

  return mbr.release();
}

void te::mem::DataSet::setFilter(te::dt::Property* /*p*/, const te::gm::Geometry* /*g*/, te::gm::SpatialRelation /*r*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSet::setFilter(te::dt::Property* /*p*/, const te::gm::Envelope* /*e*/, te::gm::SpatialRelation /*r*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::da::DataSetItem* te::mem::DataSet::getItem() const
{
  return static_cast<te::da::DataSetItem*>((*m_items)[m_i].clone());
}

void te::mem::DataSet::add(te::da::DataSetItem* item)
{
  std::auto_ptr<te::da::DataSetItem> pitem(item);

  m_items->push_back(new te::mem::DataSetItem(*item, this, m_dt.get(), false));
}

bool te::mem::DataSet::isEmpty() const
{
  return m_items->empty();
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

bool te::mem::DataSet::moveFirst()
{
  m_i = 0;
  return true;
}

bool te::mem::DataSet::moveBeforeFirst()
{
  m_i = -1;
  return true;
}

bool te::mem::DataSet::moveLast()
{
  m_i = m_items->size() - 1;
  return m_i < static_cast<int>(m_items->size());
}

bool te::mem::DataSet::moveAfterLast()
{
  m_i = m_items->size();
  return true;
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

char te::mem::DataSet::getChar(int i) const
{
  return (*m_items)[m_i].getChar(i);
}

char te::mem::DataSet::getChar(const std::string& name) const
{
  return (*m_items)[m_i].getChar(name);
}

void te::mem::DataSet::setChar(int i, char value)
{
  (*m_items)[m_i].setChar(i, value);
}

void te::mem::DataSet::setChar(const std::string& name, char value) 
{
  (*m_items)[m_i].setChar(name, value);
}

unsigned char te::mem::DataSet::getUChar(int i) const
{
  return (*m_items)[m_i].getUChar(i);
}

unsigned char te::mem::DataSet::getUChar(const std::string& name) const
{
  return (*m_items)[m_i].getUChar(name);
}

void te::mem::DataSet::setUChar(int i, unsigned char value)
{
  (*m_items)[m_i].setUChar(i, value);
}

void te::mem::DataSet::setUChar(const std::string& name, unsigned char value)
{
  (*m_items)[m_i].setUChar(name, value);
}

boost::int16_t te::mem::DataSet::getInt16(int i) const
{
  return (*m_items)[m_i].getInt16(i);
}

boost::int16_t te::mem::DataSet::getInt16(const std::string& name) const
{
  return (*m_items)[m_i].getInt16(name);
}

void te::mem::DataSet::setInt16(int i, boost::int16_t value)
{
  (*m_items)[m_i].setInt16(i, value);
}

void te::mem::DataSet::setInt16(const std::string& name, boost::int16_t value)
{
  (*m_items)[m_i].setInt16(name, value);
}

boost::int32_t te::mem::DataSet::getInt32(int i) const
{
  return (*m_items)[m_i].getInt32(i);
}

boost::int32_t te::mem::DataSet::getInt32(const std::string& name) const
{
  return (*m_items)[m_i].getInt32(name);
}

void te::mem::DataSet::setInt32(int i, boost::int32_t value)
{
  (*m_items)[m_i].setInt32(i, value);
}

void te::mem::DataSet::setInt32(const std::string& name, boost::int32_t value)
{
  (*m_items)[m_i].setInt32(name, value);
}

boost::int64_t te::mem::DataSet::getInt64(int i) const
{
  return (*m_items)[m_i].getInt64(i);
}

boost::int64_t te::mem::DataSet::getInt64(const std::string& name) const
{
  return (*m_items)[m_i].getInt64(name);
}

void te::mem::DataSet::setInt64(int i, boost::int64_t value)
{
  (*m_items)[m_i].setInt64(i, value);
}

void te::mem::DataSet::setInt64(const std::string& name, boost::int64_t value) 
{
  (*m_items)[m_i].setInt64(name, value);
}

bool te::mem::DataSet::getBool(int i) const
{
  return (*m_items)[m_i].getBool(i);
}

bool te::mem::DataSet::getBool(const std::string& name) const
{
  return (*m_items)[m_i].getBool(name);
}

void te::mem::DataSet::setBool(int i, bool value)
{
  (*m_items)[m_i].setBool(i, value);
}

void te::mem::DataSet::setBool(const std::string& name, bool value)
{
  (*m_items)[m_i].setBool(name, value);
}

float te::mem::DataSet::getFloat(int i) const
{
  return (*m_items)[m_i].getFloat(i);
}

float te::mem::DataSet::getFloat(const std::string& name) const
{
  return (*m_items)[m_i].getFloat(name);
}

void te::mem::DataSet::setFloat(int i, float value)
{
  (*m_items)[m_i].setFloat(i, value);
}

void te::mem::DataSet::setFloat(const std::string& name, float value)
{
  (*m_items)[m_i].setFloat(name, value);
}

double te::mem::DataSet::getDouble(int i) const
{
  return (*m_items)[m_i].getDouble(i);
}

double te::mem::DataSet::getDouble(const std::string& name) const
{
  return (*m_items)[m_i].getDouble(name);
}

void te::mem::DataSet::setDouble(int i, double value)
{
  (*m_items)[m_i].setDouble(i, value);
}

void te::mem::DataSet::setDouble(const std::string& name, double value)
{
  (*m_items)[m_i].setDouble(name, value);
}

std::string te::mem::DataSet::getNumeric(int i) const
{
  return (*m_items)[m_i].getNumeric(i);
}

std::string te::mem::DataSet::getNumeric(const std::string& name) const
{
  return (*m_items)[m_i].getNumeric(name);
}

void te::mem::DataSet::setNumeric(int i, const std::string& value)
{
  (*m_items)[m_i].setNumeric(i, value);
}

void te::mem::DataSet::setNumeric(const std::string& name, const std::string& value) 
{
  (*m_items)[m_i].setNumeric(name, value);
}

std::string te::mem::DataSet::getString(int i) const
{
  return (*m_items)[m_i].getString(i);
}

std::string te::mem::DataSet::getString(const std::string& name) const
{
  return (*m_items)[m_i].getString(name);
}

void te::mem::DataSet::setString(int i, const std::string& value) 
{
  (*m_items)[m_i].setString(i, value);
}

void te::mem::DataSet::setString(const std::string& name, const std::string& value) 
{
  (*m_items)[m_i].setString(name, value);
}

te::dt::ByteArray* te::mem::DataSet::getByteArray(int i) const
{
  return (*m_items)[m_i].getByteArray(i);
}

te::dt::ByteArray* te::mem::DataSet::getByteArray(const std::string& name) const
{
  return (*m_items)[m_i].getByteArray(name);
}

void te::mem::DataSet::setByteArray(int i, const te::dt::ByteArray& value)
{
  (*m_items)[m_i].setByteArray(i, value);
}

void te::mem::DataSet::setByteArray(const std::string& name, const te::dt::ByteArray& value) 
{
  (*m_items)[m_i].setByteArray(name, value);
}

te::gm::Geometry* te::mem::DataSet::getGeometry(int i) const
{
  return (*m_items)[m_i].getGeometry(i);
}

te::gm::Geometry* te::mem::DataSet::getGeometry(const std::string& name) const
{
  return (*m_items)[m_i].getGeometry(name);
}

void te::mem::DataSet::setGeometry(int i, const te::gm::Geometry& value)
{
  (*m_items)[m_i].setGeometry(i, value);
}

void te::mem::DataSet::setGeometry(const std::string& name, const te::gm::Geometry& value) 
{
  (*m_items)[m_i].setGeometry(name, value);
}

te::rst::Raster* te::mem::DataSet::getRaster(int i) const
{
  return (*m_items)[m_i].getRaster(i);
}

te::rst::Raster* te::mem::DataSet::getRaster(const std::string& name) const
{
  return (*m_items)[m_i].getRaster(name);
}

void te::mem::DataSet::setRaster(int i, const te::rst::Raster& value)
{
  (*m_items)[m_i].setRaster(i, value);
}

void te::mem::DataSet::setRaster(const std::string& name, const te::rst::Raster& value)
{
  (*m_items)[m_i].setRaster(name, value);
}

te::dt::DateTime* te::mem::DataSet::getDateTime(int i) const
{
  return (*m_items)[m_i].getDateTime(i);
}

te::dt::DateTime* te::mem::DataSet::getDateTime(const std::string& name) const
{
  return (*m_items)[m_i].getDateTime(name);
}

void te::mem::DataSet::setDateTime(int i, const te::dt::DateTime& value) 
{
  (*m_items)[m_i].setDateTime(i, value);
}

void te::mem::DataSet::setDateTime(const std::string& name, const te::dt::DateTime& value) 
{
  (*m_items)[m_i].setDateTime(name, value);
}

void te::mem::DataSet::getArray(int i, std::vector<boost::int16_t>& values) const
{
  return (*m_items)[m_i].getArray(i, values);
}

void te::mem::DataSet::getArray(const std::string& name, std::vector<boost::int16_t>& values) const
{
  return (*m_items)[m_i].getArray(name, values);
}

const unsigned char* te::mem::DataSet::getWKB(int i) const
{
  return (*m_items)[m_i].getWKB(i);
}

const unsigned char* te::mem::DataSet::getWKB(const std::string& name) const
{
  return (*m_items)[m_i].getWKB(name);
}

te::da::DataSet* te::mem::DataSet::getDataSet(int i)
{
  return (*m_items)[m_i].getDataSet(i);
}

te::da::DataSet* te::mem::DataSet::getDataSet(const std::string& name)
{
  return (*m_items)[m_i].getDataSet(name);
}

void te::mem::DataSet::setDataSet(int i, const te::da::DataSet& value)
{
  (*m_items)[m_i].setDataSet(i, value);
}

void te::mem::DataSet::setDataSet(const std::string& name, const te::da::DataSet& value)
{
  (*m_items)[m_i].setDataSet(name, value);
}

te::dt::AbstractData* te::mem::DataSet::getValue(int i) const
{
  return (*m_items)[m_i].getValue(i);
}

te::dt::AbstractData* te::mem::DataSet::getValue(const std::string& name) const
{
  return (*m_items)[m_i].getValue(name);
}

void te::mem::DataSet::setValue(int i, te::dt::AbstractData* value)
{
  (*m_items)[m_i].setValue(i, value);
}

void te::mem::DataSet::setValue(const std::string& name, te::dt::AbstractData* ad)
{
  (*m_items)[m_i].setValue(name, ad);
}

bool te::mem::DataSet::isNull(int i) const
{
  return (*m_items)[m_i].isNull(i);
}

bool te::mem::DataSet::isNull(const std::string& name) const
{
  return (*m_items)[m_i].isNull(name);
}

te::dt::AbstractData* te::mem::DataSet::clone() const
{
  return new DataSet(*this);
}
