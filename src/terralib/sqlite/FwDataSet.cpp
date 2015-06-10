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
  \file terralib/sqlite/FwDataSet.cpp

  \brief Implementation of a forward-only dataset for the TerraLib SQLite Data Access driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../geometry/Geometry.h"
#include "Config.h"
#include "DataSourceTransactor.h"
#include "EWKBReader.h"
#include "FwDataSet.h"
#include "Utils.h"

// STL
#include <memory>

// SQLite
#include <sqlite3.h>

class te::sqlite::FwDataSet::Impl
{
  public:

    Impl(sqlite3_stmt* stmt, DataSourceTransactor* t);
    
    ~Impl();

    sqlite3_stmt* m_stmt;
    DataSourceTransactor* m_parent;
    te::da::DataSetType* m_dt;
};

te::sqlite::FwDataSet::Impl::Impl(sqlite3_stmt* stmt, DataSourceTransactor* t)
  : m_stmt(stmt), m_parent(t), m_dt(0)
{
}

te::sqlite::FwDataSet::Impl::~Impl()
{
  delete m_dt;

  int ret = SQLITE_OK;

  ret = sqlite3_finalize(m_stmt);

  assert(ret == SQLITE_OK);
}

te::sqlite::FwDataSet::FwDataSet(sqlite3_stmt* stmt, DataSourceTransactor* t)
  : m_pImpl(0)
{
  m_pImpl= new Impl(stmt, t);

  m_pImpl->m_dt = Convert2TerraLib(m_pImpl->m_stmt);
}

te::sqlite::FwDataSet::~FwDataSet()
{
  delete m_pImpl;
}

te::common::TraverseType te::sqlite::FwDataSet::getTraverseType() const
{
  return te::common::FORWARDONLY;
}

te::common::AccessPolicy te::sqlite::FwDataSet::getAccessPolicy() const
{
  return te::common::RAccess;
}

std::size_t te::sqlite::FwDataSet::getNumProperties() const
{
  return m_pImpl->m_dt->size();
}

int te::sqlite::FwDataSet::getPropertyDataType(std::size_t i) const
{
  return m_pImpl->m_dt->getProperty(i)->getType();
}

std::string te::sqlite::FwDataSet::getPropertyName(std::size_t i) const
{
  return m_pImpl->m_dt->getProperty(i)->getName();
}

std::string te::sqlite::FwDataSet::getDatasetNameOfProperty(std::size_t i) const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::isEmpty() const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::isConnected() const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::size_t te::sqlite::FwDataSet::size() const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::gm::Envelope> te::sqlite::FwDataSet::getExtent(std::size_t i)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::moveNext()
{
  return (sqlite3_step(m_pImpl->m_stmt) == SQLITE_ROW);
}

bool te::sqlite::FwDataSet::movePrevious()
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::moveBeforeFirst()
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::moveFirst()
{
  int ret = sqlite3_reset(m_pImpl->m_stmt);

  if(ret != SQLITE_OK)
    return false;

  return (sqlite3_step(m_pImpl->m_stmt) == SQLITE_ROW);
}

bool te::sqlite::FwDataSet::moveLast()
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::move(std::size_t i)
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::isAtBegin() const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::isBeforeBegin() const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::isAtEnd() const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::isAfterEnd() const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

char te::sqlite::FwDataSet::getChar(std::size_t i) const
{
  int value = sqlite3_column_int(m_pImpl->m_stmt, i);

  return static_cast<char>(value);
}

unsigned char te::sqlite::FwDataSet::getUChar(std::size_t i) const
{
  int value = sqlite3_column_int(m_pImpl->m_stmt, i);

  return static_cast<unsigned char>(value);
}

boost::int16_t te::sqlite::FwDataSet::getInt16(std::size_t i) const
{
  return static_cast<boost::int16_t>(sqlite3_column_int(m_pImpl->m_stmt, i));
}

boost::int32_t te::sqlite::FwDataSet::getInt32(std::size_t i) const
{
  return sqlite3_column_int(m_pImpl->m_stmt, i);
}

boost::int64_t te::sqlite::FwDataSet::getInt64(std::size_t i) const
{
  return sqlite3_column_int64(m_pImpl->m_stmt, i);
}

bool te::sqlite::FwDataSet::getBool(std::size_t i) const
{
  return sqlite3_column_int(m_pImpl->m_stmt, i) == TE_SQLITE_BOOL_TRUE;
}

float te::sqlite::FwDataSet::getFloat(std::size_t i) const
{
  return static_cast<float>(sqlite3_column_double(m_pImpl->m_stmt, i));
}

double te::sqlite::FwDataSet::getDouble(std::size_t i) const
{
  return sqlite3_column_double(m_pImpl->m_stmt, i);
}

std::string te::sqlite::FwDataSet::getNumeric(std::size_t i) const
{
  std::string value((const char*)(sqlite3_column_text(m_pImpl->m_stmt, i)));

  return value; 
}

std::string te::sqlite::FwDataSet::getString(std::size_t i) const
{
  return (const char*)(sqlite3_column_text(m_pImpl->m_stmt, i));
}

std::auto_ptr<te::dt::ByteArray> te::sqlite::FwDataSet::getByteArray(std::size_t i) const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::gm::Geometry> te::sqlite::FwDataSet::getGeometry(std::size_t i) const
{
  unsigned char* ewkb = (unsigned char*)(sqlite3_column_blob(m_pImpl->m_stmt, i));
  std::auto_ptr<te::gm::Geometry> g(EWKBReader::read(ewkb));
  return g;
}

std::auto_ptr<te::rst::Raster> te::sqlite::FwDataSet::getRaster(std::size_t i) const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::dt::DateTime> te::sqlite::FwDataSet::getDateTime(std::size_t i) const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

std::auto_ptr<te::dt::Array> te::sqlite::FwDataSet::getArray(std::size_t i) const
{
  throw te::common::Exception(TR_COMMON("Not supported by SQLite driver!"));
}

bool te::sqlite::FwDataSet::isNull(std::size_t i) const
{
  return sqlite3_column_type(m_pImpl->m_stmt, i) == SQLITE_NULL;
}

