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
#include "../memory/DataSetItem.h"
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

te::da::DataSetType* te::mysql::DataSet::getType()
{
  if(!m_dt)
    m_dt = Convert2TerraLibDataSetType(m_result);

  return m_dt;
}

const te::da::DataSetType* te::mysql::DataSet::getType() const
{
  if(!m_dt)
    m_dt = Convert2TerraLibDataSetType(m_result);

  return m_dt;
}

te::da::DataSourceTransactor* te::mysql::DataSet::getTransactor() const
{
  return m_transactor;
}

void te::mysql::DataSet::loadTypeInfo()
{
// doesn't affect this implementation
}

te::da::DataSet* te::mysql::DataSet::getParent() const
{
  return 0;
}

te::gm::Envelope* te::mysql::DataSet::getExtent(const te::dt::Property* p)
{
  if(p == 0)
    throw Exception(TR_MYSQL("The property is missing!"));

  if(p->getType() != te::dt::GEOMETRY_TYPE)
    throw Exception(TR_MYSQL("Not a geometry column to calculate the MBR!"));

  std::size_t i = getType()->getPropertyPosition(p);

  assert(i != std::string::npos);

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

void te::mysql::DataSet::setFilter(te::dt::Property* p,
                                   const te::gm::Geometry* g,
                                   te::gm::SpatialRelation r)
{
  if(g == 0)
    throw Exception(TR_MYSQL("The geometry is missing!"));

  if(p->getType() != te::dt::GEOMETRY_TYPE)
    throw Exception(TR_MYSQL("The property is not geometric!"));
  
  if(m_originalsql == 0)
    throw Exception(TR_MYSQL("Can not set filter over this type of query. Probably it is a result of a prepared query!"));

  std::auto_ptr<std::string> sql(new std::string("SELECT * FROM ("));

  *sql += *m_originalsql;
  *sql += ") AS mysql_driver_subquery WHERE ";
  
  GetFilter(p->getName(), g, r, *sql);

  delete m_originalsql;

  m_originalsql = sql.release();

  sql::ResultSet* newresult = m_transactor->myQuery(*m_originalsql);

  delete m_result;

  m_result = newresult;

  delete m_dt;

  m_dt = 0;  
}
 
void te::mysql::DataSet::setFilter(te::dt::Property* p,
                                   const te::gm::Envelope* e,
                                   te::gm::SpatialRelation r)
{
  if(e == 0)
    throw Exception(TR_MYSQL("The MBR is missing!"));

  if(p->getType() != te::dt::GEOMETRY_TYPE)
    throw Exception(TR_MYSQL("The property is not geometric!"));
  
  if(m_originalsql == 0)
    throw Exception(TR_MYSQL("Can not set filter over this typq of query. Probably it is a result of a prepared query!"));

  std::auto_ptr<std::string> sql(new std::string("SELECT * FROM ("));

  *sql += *m_originalsql;
  *sql += ") AS mysql_driver_subquery WHERE ";
  
  GetFilter(p->getName(), e, r, *sql);

  delete m_originalsql;

  m_originalsql = sql.release();

  sql::ResultSet* newresult = m_transactor->myQuery(*m_originalsql);

  delete m_result;

  m_result = newresult;

  delete m_dt;

  m_dt = 0;  
}

te::da::DataSetItem* te::mysql::DataSet::getItem() const
{
  return new te::mem::DataSetItem(this);
}

void te::mysql::DataSet::add(te::da::DataSetItem* /*item*/)
{
  throw Exception(TR_MYSQL("Not supported by MySQL driver!"));
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

char te::mysql::DataSet::getChar(int i) const
{
  boost::int32_t value = m_result->getInt(i + 1);

  char cvalue = static_cast<char>(value);

  return cvalue;
}

void te::mysql::DataSet::setChar(int /*i*/, char /*value*/)
{
  throw Exception(TR_MYSQL("Not supported by MySQL driver!"));
}

unsigned char te::mysql::DataSet::getUChar(int i) const
{
  boost::uint32_t value = m_result->getUInt(i + 1);

  char ucvalue = static_cast<unsigned char>(value);

  return ucvalue;
}

void te::mysql::DataSet::setUChar(int /*i*/, unsigned char /*value*/)
{
  throw Exception(TR_MYSQL("Not supported by MySQL driver!"));
}

boost::int16_t te::mysql::DataSet::getInt16(int i) const
{
  boost::int32_t value = m_result->getInt(i + 1);

  boost::int16_t ivalue = static_cast<boost::int16_t>(value);

  return ivalue;
}

void te::mysql::DataSet::setInt16(int /*i*/, boost::int16_t /*value*/)
{
  throw Exception(TR_MYSQL("Not supported by MySQL driver!"));
}

boost::int32_t te::mysql::DataSet::getInt32(int i) const
{
  return m_result->getInt(i + 1);
}

void te::mysql::DataSet::setInt32(int /*i*/, boost::int32_t /*value*/)
{
  throw Exception(TR_MYSQL("Not supported by MySQL driver!"));
}

boost::int64_t te::mysql::DataSet::getInt64(int i) const
{
  return m_result->getInt64(i + 1);
}

void te::mysql::DataSet::setInt64(int /*i*/, boost::int64_t /*value*/)
{
  throw Exception(TR_MYSQL("Not supported by MySQL driver!"));
}

bool te::mysql::DataSet::getBool(int i) const
{
  return m_result->getBoolean(i + 1);
}

void te::mysql::DataSet::setBool(int /*i*/, bool /*value*/)
{
  throw Exception(TR_MYSQL("Not supported by MySQL driver!"));
}

float te::mysql::DataSet::getFloat(int i) const
{
  float value = static_cast<float>(m_result->getDouble(i + 1));

  return value;
}

void te::mysql::DataSet::setFloat(int /*i*/, float /*value*/)
{
  throw Exception(TR_MYSQL("Not supported by MySQL driver!"));
}

double te::mysql::DataSet::getDouble(int i) const
{
  double value = static_cast<double>(m_result->getDouble(i + 1));

  return value;
}

void te::mysql::DataSet::setDouble(int /*i*/, double /*value*/)
{
  throw Exception(TR_MYSQL("Not supported by MySQL driver!"));
}

std::string te::mysql::DataSet::getNumeric(int i) const
{
  std::string value = boost::lexical_cast<std::string>(m_result->getDouble(i + 1));

  return value;
}

void te::mysql::DataSet::setNumeric(int /*i*/, const std::string& /*value*/)
{
  throw Exception(TR_MYSQL("Not supported by MySQL driver!"));
}

std::string te::mysql::DataSet::getString(int i) const
{
  return m_result->getString(i + 1);
}

void te::mysql::DataSet::setString(int /*i*/, const std::string& /*value*/)
{
  throw Exception(TR_MYSQL("Not supported by MySQL driver!"));
}

te::dt::ByteArray* te::mysql::DataSet::getByteArray(int i) const
{
  std::istream* is = m_result->getBlob(i + 1);

// get buffer size
  is->seekg(0, std::ios::end);

  std::size_t size = is->tellg();

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

void te::mysql::DataSet::setByteArray(int /*i*/, const te::dt::ByteArray& /*value*/)
{
  throw Exception(TR_MYSQL("Not supported by MySQL driver!"));
}

te::gm::Geometry* te::mysql::DataSet::getGeometry(int i) const
{
  std::auto_ptr<te::dt::ByteArray> mywkb(getByteArray(i));

// at least: blob-size (4byte) + wkb byte-order (1byte) + geom-type (4byte)
  assert(mywkb->bytesUsed() > 9);

// skip blob size header
  te::gm::Geometry* geom = te::gm::WKBReader::read(mywkb->getData() + 4);

  return geom;
}

void te::mysql::DataSet::setGeometry(int /*i*/, const te::gm::Geometry& /*value*/)
{
  throw Exception(TR_MYSQL("Not supported by MySQL driver!"));
}

te::rst::Raster* te::mysql::DataSet::getRaster(int /*i*/) const
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

void te::mysql::DataSet::setRaster(int /*i*/, const te::rst::Raster& /*value*/)
{
  throw Exception(TR_MYSQL("Not supported by MySQL driver!"));
}

te::dt::DateTime* te::mysql::DataSet::getDateTime(int /*i*/) const
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

void te::mysql::DataSet::setDateTime(int /*i*/, const te::dt::DateTime& /*value*/)
{
  throw Exception(TR_MYSQL("Not supported by MySQL driver!"));
}

void te::mysql::DataSet::getArray(int /*i*/, std::vector<boost::int16_t>& /*values*/) const
{
  throw Exception(TR_MYSQL("Not supported by MySQL driver!"));
}

const unsigned char* te::mysql::DataSet::getWKB(int /*i*/) const
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

te::da::DataSet* te::mysql::DataSet::getDataSet(int /*i*/)
{
  throw Exception(TR_MYSQL("Not supported by MySQL driver!"));
}

void te::mysql::DataSet::setDataSet(int /*i*/, const te::da::DataSet& /*value*/)
{
  throw Exception(TR_MYSQL("Not supported by MySQL driver!"));
}

void te::mysql::DataSet::setValue(int /*i*/, te::dt::AbstractData* /*ad*/)
{
  throw Exception(TR_MYSQL("Not supported by MySQL driver!"));
}

bool te::mysql::DataSet::isNull(int i) const
{
  return m_result->isNull(i + 1);
}


