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
  \file LightItem.cpp

  \brief A thin and lean implementation of DatasetItem class.
*/

// TerraLib
#include "../common/Translator.h"
#include "../common/STLUtils.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../datatype/AbstractData.h"
#include "../datatype/ByteArray.h"
#include "../datatype/DateTime.h"
#include "../datatype/SimpleData.h"
#include "../geometry/Geometry.h"
#include "Exception.h"
#include "LightItem.h"

te::mem::LightItem::LightItem(std::size_t n)
  : m_data(n, 0)
{
}

te::mem::LightItem::~LightItem()
{
  te::common::FreeContents(m_data);
}

void te::mem::LightItem::resize(std::size_t n)
{
  m_data.resize(n, 0);
}

te::dt::AbstractData* te::mem::LightItem::take(int i)
{
  te::dt::AbstractData* v = m_data[i];
  m_data[i] = 0;
  return v;
}

void te::mem::LightItem::copy(te::da::DataSet* item)
{
  const std::size_t nv = m_data.size();

  for(std::size_t i = 0; i < nv; ++i)
    m_data[i] = item->getValue(i);
}

char te::mem::LightItem::getChar(int i) const
{
  return static_cast<const te::dt::Char*>(m_data[i])->getValue();  
}

void te::mem::LightItem::setChar(int i, char value)
{
  delete m_data[i];
  m_data[i] = new te::dt::Char(value);
}

unsigned char te::mem::LightItem::getUChar(int i) const
{
  return static_cast<const te::dt::UChar*>(m_data[i])->getValue();  
}

void te::mem::LightItem::setUChar(int i, unsigned char value)
{
  delete m_data[i];
  m_data[i] = new te::dt::UChar(value);
}

boost::int16_t te::mem::LightItem::getInt16(int i) const
{
  return static_cast<const te::dt::Int16*>(m_data[i])->getValue();  
}

void te::mem::LightItem::setInt16(int i, boost::int16_t value)
{
  delete m_data[i];
  m_data[i] = new te::dt::Int16(value);
}

boost::int32_t te::mem::LightItem::getInt32(int i) const
{
  return static_cast<const te::dt::Int32*>(m_data[i])->getValue();  
}

void te::mem::LightItem::setInt32(int i, boost::int32_t value)
{
  delete m_data[i];
  m_data[i] = new te::dt::Int32(value);
}

boost::int64_t te::mem::LightItem::getInt64(int i) const
{
  return static_cast<const te::dt::Int64*>(m_data[i])->getValue();  
}

void te::mem::LightItem::setInt64(int i, boost::int64_t value)
{
  delete m_data[i];
  m_data[i] = new te::dt::Int64(value);
}

bool te::mem::LightItem::getBool(int i) const
{
  return static_cast<const te::dt::Boolean*>(m_data[i])->getValue();  
}

void te::mem::LightItem::setBool(int i, bool value)
{
  delete m_data[i];
  m_data[i] = new te::dt::Boolean(value);
}

float te::mem::LightItem::getFloat(int i) const
{
  return static_cast<const te::dt::Float*>(m_data[i])->getValue();  
}

void te::mem::LightItem::setFloat(int i, float value)
{
  delete m_data[i];
  m_data[i] = new te::dt::Float(value);
}

double te::mem::LightItem::getDouble(int i) const
{
  return static_cast<const te::dt::Double*>(m_data[i])->getValue();  
}

void te::mem::LightItem::setDouble(int i, double value)
{
  delete m_data[i];
  m_data[i] = new te::dt::Double(value);
}

std::string te::mem::LightItem::getNumeric(int i) const
{
  return static_cast<const te::dt::Numeric*>(m_data[i])->getValue();  
}

void te::mem::LightItem::setNumeric(int i, const std::string& value)
{
  delete m_data[i];
  m_data[i] = new te::dt::Numeric(value);
}

std::string te::mem::LightItem::getString(int i) const
{
  return static_cast<const te::dt::String*>(m_data[i])->getValue();  
}

void te::mem::LightItem::setString(int i, const std::string& value)
{
  delete m_data[i];
  m_data[i] = new te::dt::String(value);
}

te::dt::ByteArray* te::mem::LightItem::getByteArray(int i) const
{
  return static_cast<te::dt::ByteArray*>(static_cast<const te::dt::ByteArray*>(m_data[i])->clone());
}

void te::mem::LightItem::setByteArray(int i, const te::dt::ByteArray& value)
{
  delete m_data[i];
  m_data[i] = value.clone();
}

te::gm::Geometry* te::mem::LightItem::getGeometry(int i) const
{
  return static_cast<te::gm::Geometry*>(static_cast<const te::gm::Geometry*>(m_data[i])->clone());
}

void te::mem::LightItem::setGeometry(int i, const te::gm::Geometry& value)
{
  delete m_data[i];
  m_data[i] = value.clone();
}

te::rst::Raster* te::mem::LightItem::getRaster(int /*i*/) const
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::LightItem::setRaster(int /*i*/, const te::rst::Raster& /*value*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::dt::DateTime* te::mem::LightItem::getDateTime(int /*i*/) const
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::LightItem::setDateTime(int i, const te::dt::DateTime& value)
{
  delete m_data[i];
  m_data[i] = value.clone();
}

void te::mem::LightItem::getArray(int /*i*/, std::vector<boost::int16_t>& /*values*/) const
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

const unsigned char* te::mem::LightItem::getWKB(int /*i*/) const
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::da::DataSet* te::mem::LightItem::getDataSet(int /*i*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::LightItem::setDataSet(int /*i*/, const te::da::DataSet& /*value*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::dt::AbstractData* te::mem::LightItem::getValue(int i) const
{
  return m_data[i]->clone();
}

void te::mem::LightItem::setValue(int i, te::dt::AbstractData* value)
{
  delete m_data[i];
  m_data[i] = value;
}

bool te::mem::LightItem::isNull(int i) const
{
  return m_data[i] == 0;
}
