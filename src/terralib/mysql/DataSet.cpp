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
  \file terralib/mysql/DataSet.cpp

  \brief Implementation of the DataSet class to MySQL data access driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../datatype/ByteArray.h"
#include "../datatype/DateTime.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/WKBReader.h"
#include "DataSet.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <sstream>

// Boost
#include <boost/lexical_cast.hpp>

// MySQL
#include <driver/mysql_public_iface.h>

te::mysql::DataSet::DataSet(DataSourceTransactor* t, sql::ResultSet* result, std::string* sql)
  : m_transactor(t),
    m_result(result),
    m_dt(0),
    m_originalsql(sql)
{
  assert(result);
  m_dt = Convert2TerraLibDataSetType(result);
}

te::mysql::DataSet::~DataSet()
{
  delete m_result;
  delete m_dt;
  delete m_originalsql;
}

te::common::TraverseType te::mysql::DataSet::getTraverseType() const
{
  sql::ResultSet::enum_type r = m_result->getType();

  if (r == sql::ResultSet::TYPE_FORWARD_ONLY)
    return te::common::FORWARDONLY;
  else if(r == sql::ResultSet::TYPE_SCROLL_INSENSITIVE)
    return te::common::RANDOM;
  
  return te::common::FORWARDONLY;
}

te::common::AccessPolicy te::mysql::DataSet::getAccessPolicy() const
{
  return te::common::RAccess;
}

te::da::DataSourceTransactor* te::mysql::DataSet::getTransactor() const
{
  return m_transactor;
}

te::gm::Envelope* te::mysql::DataSet::getExtent(std::size_t i)
{
  assert(i != std::string::npos);

  if(getPropertyDataType(i) != te::dt::GEOMETRY_TYPE)
    throw Exception(TR_MYSQL("Not a geometry column to calculate the MBR!"));

  if(isEmpty() || !moveFirst())
    return new te::gm::Envelope;

  std::auto_ptr<te::gm::Envelope> mbr;

  do
  {
    std::auto_ptr<te::gm::Geometry> g(getGeometry(i));

    const te::gm::Envelope* box = g->getMBR();

    mbr->Union(*box);

  }while(m_result->next());
  
  m_result->beforeFirst();
  
  return mbr.release();
}

std::size_t te::mysql::DataSet::getNumProperties() const
{
  return m_dt->size();
}

int te::mysql::DataSet::getPropertyDataType(std::size_t pos) const
{
  return m_dt->getProperty(pos)->getType();
}

std::string te::mysql::DataSet::getPropertyName(std::size_t pos) const
{
  return m_dt->getProperty(pos)->getName();
}

std::string te::mysql::DataSet::getDatasetNameOfProperty(std::size_t pos) const
{
  return "";
}

bool te::mysql::DataSet::isEmpty() const
{
  return m_result->wasNull();
}

std::size_t te::mysql::DataSet::size() const
{
  return m_result->rowsCount();
}

bool te::mysql::DataSet::moveNext()
{
  return m_result->next();
}

bool te::mysql::DataSet::movePrevious()
{
  return m_result->previous();
}

bool te::mysql::DataSet::moveFirst()
{
  return m_result->first();
}

bool te::mysql::DataSet::moveBeforeFirst()
{
  m_result->beforeFirst();

  return true;
}

bool te::mysql::DataSet::moveLast()
{
  return m_result->last();
}

bool te::mysql::DataSet::moveAfterLast()
{
  m_result->afterLast();

  return true;
}

bool te::mysql::DataSet::move(std::size_t i)
{
  return i != 0 ? m_result->absolute(i) : m_result->first();
}

bool te::mysql::DataSet::isAtBegin() const
{
  return m_result->isFirst();
}

bool te::mysql::DataSet::isBeforeBegin() const
{
  return m_result->isBeforeFirst();
}

bool te::mysql::DataSet::isAtEnd() const
{
  return m_result->isLast();
}

bool te::mysql::DataSet::isAfterEnd() const
{
  return m_result->isAfterLast();
}

char te::mysql::DataSet::getChar(std::size_t i) const
{
  boost::int32_t value = m_result->getInt(i + 1);

  char cvalue = static_cast<char>(value);

  return cvalue;
}

unsigned char te::mysql::DataSet::getUChar(std::size_t i) const
{
  boost::uint32_t value = m_result->getUInt(i + 1);

  char ucvalue = static_cast<unsigned char>(value);

  return ucvalue;
}

boost::int16_t te::mysql::DataSet::getInt16(std::size_t i) const
{
  boost::int32_t value = m_result->getInt(i + 1);

  boost::int16_t ivalue = static_cast<boost::int16_t>(value);

  return ivalue;
}

boost::int32_t te::mysql::DataSet::getInt32(std::size_t i) const
{
  return m_result->getInt(i + 1);
}

boost::int64_t te::mysql::DataSet::getInt64(std::size_t i) const
{
  return m_result->getInt64(i + 1);
}

bool te::mysql::DataSet::getBool(std::size_t i) const
{
  return m_result->getBoolean(i + 1);
}

float te::mysql::DataSet::getFloat(std::size_t i) const
{
  float value = static_cast<float>(m_result->getDouble(i + 1));

  return value;
}

double te::mysql::DataSet::getDouble(std::size_t i) const
{
  double value = static_cast<double>(m_result->getDouble(i + 1));

  return value;
}

std::string te::mysql::DataSet::getNumeric(std::size_t i) const
{
  std::string value = boost::lexical_cast<std::string>(m_result->getDouble(i + 1));

  return value;
}

std::string te::mysql::DataSet::getString(std::size_t i) const
{
  return m_result->getString(i + 1);
}

te::dt::ByteArray* te::mysql::DataSet::getByteArray(std::size_t i) const
{
  std::istream* is = m_result->getBlob(i + 1);

// get buffer size
  is->seekg(0, std::ios::end);

  std::size_t size = static_cast<std::size_t>(is->tellg());

// allocate buffer
  std::auto_ptr<te::dt::ByteArray> byteArray(new te::dt::ByteArray(size));

// go to start
  is->seekg(0, std::ios::beg);

// read bytes
  is->read(byteArray->getData(), size);

  byteArray->setBytesUsed(size);
  
// return byte array
  return byteArray.release();
}

te::gm::Geometry* te::mysql::DataSet::getGeometry(std::size_t i) const
{
  std::auto_ptr<te::dt::ByteArray> mywkb(getByteArray(i));

// at least: blob-size (4byte) + wkb byte-order (1byte) + geom-type (4byte)
  assert(mywkb->bytesUsed() > 9);

// skip blob size header
  te::gm::Geometry* geom = te::gm::WKBReader::read(mywkb->getData() + 4);

  return geom;
}

te::rst::Raster* te::mysql::DataSet::getRaster(std::size_t /*i*/) const
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

te::dt::DateTime* te::mysql::DataSet::getDateTime(std::size_t /*i*/) const
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

te::dt::Array* te::mysql::DataSet::getArray(std::size_t /*i*/) const
{
  throw Exception(TR_MYSQL("Not supported by MySQL driver!"));
}

bool te::mysql::DataSet::isNull(std::size_t i) const
{
  return m_result->isNull(i + 1);
}
