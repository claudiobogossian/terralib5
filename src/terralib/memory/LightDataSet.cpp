/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file LightDataSet.cpp

  \brief A thin and lean implementation of Dataset class.
*/

// TerraLib
#include "../common/Translator.h"
#include "../common/STLUtils.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "Exception.h"
#include "LightDataSet.h"
#include "LightItem.h"

// STL
#include <cassert>

te::mem::LightDataSet::LightDataSet(const te::da::DataSetTypePtr& dt, std::size_t n)
  : //m_items(n, 0),
    m_dt(dt),
    m_i(static_cast<std::size_t>(-1))
{
  m_items.reserve(n);
}

te::mem::LightDataSet::~LightDataSet()
{
  te::common::FreeContents(m_items);
}

void te::mem::LightDataSet::copy(te::da::DataSet* sourceDataSet, std::size_t n)
{
  const std::size_t nproperties = m_dt->size();

  std::size_t i = 0;

  do
  {
    std::auto_ptr<LightItem> item(new LightItem(nproperties));

    for(std::size_t c = 0; c < nproperties; ++c)
    {
      if(!sourceDataSet->isNull(c))
        item->setValue(c, sourceDataSet->getValue(c));
      else
        item->setValue(c, 0);
    }

    m_items.push_back(item.release());

    ++i;

  }while(sourceDataSet->moveNext() && (i < n));
}

void te::mem::LightDataSet::resizeItems(std::size_t n)
{
  const std::size_t size = m_items.size();

  for(std::size_t i = 0; i != size; ++i)
    m_items[i]->resize(n);
}

void te::mem::LightDataSet::add(LightItem* item)
{
  m_items.push_back(item);
}

void te::mem::LightDataSet::clear()
{
  te::common::FreeContents(m_items);
  m_items.clear();
  m_i = static_cast<std::size_t>(-1);
}

void te::mem::LightDataSet::reserve(std::size_t n)
{
  m_items.reserve(n);
}

te::da::DataSetItem* te::mem::LightDataSet::getItem() const
{
  return m_items[m_i];
}

bool te::mem::LightDataSet::moveNext()
{
  return ++m_i != m_items.size();
}

bool te::mem::LightDataSet::movePrevious()
{
  return --m_i < m_items.size();
}
        
bool te::mem::LightDataSet::moveFirst()
{
  m_i = 0;
  return m_i != m_items.size();
}

bool te::mem::LightDataSet::moveBeforeFirst()
{
  m_i = static_cast<std::size_t>(-1);
  return m_items.size() != 0;
}

bool te::mem::LightDataSet::moveLast()
{
  m_i = m_items.size() - 1;
  return m_items.size() != 0;
}

bool te::mem::LightDataSet::moveAfterLast()
{
  m_i = m_items.size();
  return m_items.size() != 0;
}

bool te::mem::LightDataSet::move(std::size_t i)
{
  m_i = i;
  return m_i < m_items.size();
}

bool te::mem::LightDataSet::isAtBegin() const
{
  return m_i == 0;
}

bool te::mem::LightDataSet::isBeforeBegin() const
{
  return m_i == static_cast<std::size_t>(-1);
}

bool te::mem::LightDataSet::isAtEnd() const
{
  return m_i == (m_items.size() - 1);
}

bool te::mem::LightDataSet::isAfterEnd() const
{
  return m_i == m_items.size();
}

char te::mem::LightDataSet::getChar(int i) const
{
  return m_items[m_i]->getChar(i);
}

void te::mem::LightDataSet::setChar(int i, char value)
{
  m_items[m_i]->setChar(i, value);
}

unsigned char te::mem::LightDataSet::getUChar(int i) const
{
  return m_items[m_i]->getUChar(i);
}

void te::mem::LightDataSet::setUChar(int i, unsigned char value)
{
  m_items[m_i]->setUChar(i, value);
}

boost::int16_t te::mem::LightDataSet::getInt16(int i) const
{
  return m_items[m_i]->getInt16(i);
}

void te::mem::LightDataSet::setInt16(int i, boost::int16_t value)
{
  m_items[m_i]->setInt16(i, value);
}

boost::int32_t te::mem::LightDataSet::getInt32(int i) const
{
  return m_items[m_i]->getInt32(i);
}

void te::mem::LightDataSet::setInt32(int i, boost::int32_t value)
{
  m_items[m_i]->setInt32(i, value);
}

boost::int64_t te::mem::LightDataSet::getInt64(int i) const
{
  return m_items[m_i]->getInt64(i);
}

void te::mem::LightDataSet::setInt64(int i, boost::int64_t value)
{
  m_items[m_i]->setInt64(i, value);
}

bool te::mem::LightDataSet::getBool(int i) const
{
  return m_items[m_i]->getBool(i);
}

void te::mem::LightDataSet::setBool(int i, bool value)
{
  m_items[m_i]->setBool(i, value);
}

float te::mem::LightDataSet::getFloat(int i) const
{
  return m_items[m_i]->getFloat(i);
}

void te::mem::LightDataSet::setFloat(int i, float value)
{
  m_items[m_i]->setFloat(i, value);
}

double te::mem::LightDataSet::getDouble(int i) const
{
  return m_items[m_i]->getDouble(i);
}

void te::mem::LightDataSet::setDouble(int i, double value)
{
  m_items[m_i]->setDouble(i, value);
}

std::string te::mem::LightDataSet::getNumeric(int i) const
{
  return m_items[m_i]->getNumeric(i);
}

void te::mem::LightDataSet::setNumeric(int i, const std::string& value)
{
  m_items[m_i]->setNumeric(i, value);
}

std::string te::mem::LightDataSet::getString(int i) const
{
  return m_items[m_i]->getString(i);
}

void te::mem::LightDataSet::setString(int i, const std::string& value)
{
  m_items[m_i]->setString(i, value);
}

te::dt::ByteArray* te::mem::LightDataSet::getByteArray(int i) const
{
  return m_items[m_i]->getByteArray(i);
}

void te::mem::LightDataSet::setByteArray(int i, const te::dt::ByteArray& value)
{
  m_items[m_i]->setByteArray(i, value);
}

te::gm::Geometry* te::mem::LightDataSet::getGeometry(int i) const
{
  return m_items[m_i]->getGeometry(i);
}

void te::mem::LightDataSet::setGeometry(int i, const te::gm::Geometry& value)
{
  m_items[m_i]->setGeometry(i, value);
}

te::rst::Raster* te::mem::LightDataSet::getRaster(int /*i*/) const
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::LightDataSet::setRaster(int /*i*/, const te::rst::Raster& /*value*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::dt::DateTime* te::mem::LightDataSet::getDateTime(int /*i*/) const
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::LightDataSet::setDateTime(int i, const te::dt::DateTime& value)
{
  m_items[m_i]->setDateTime(i, value);
}

void te::mem::LightDataSet::getArray(int /*i*/, std::vector<boost::int16_t>& /*values*/) const
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

const unsigned char* te::mem::LightDataSet::getWKB(int /*i*/) const
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::da::DataSet* te::mem::LightDataSet::getDataSet(int /*i*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::LightDataSet::setDataSet(int /*i*/, const te::da::DataSet& /*value*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::dt::AbstractData* te::mem::LightDataSet::getValue(int i) const
{
  return m_items[m_i]->getValue(i);
}

void te::mem::LightDataSet::setValue(int i, te::dt::AbstractData* value)
{
  m_items[m_i]->setValue(i, value);
}

bool te::mem::LightDataSet::isNull(int i) const
{
  return m_items[m_i]->isNull(i);
}

te::mem::LightDataSet::LightDataSet()
  : m_i(static_cast<std::size_t>(-1))
{
}

