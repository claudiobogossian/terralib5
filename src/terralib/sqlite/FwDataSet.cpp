/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/sqlite/DataSetFw.cpp

  \brief Implementation of a forward-only dataset for the TerraLib SQLite Data Access driver.
*/

// TerraLib
#include "../common/Globals.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../datatype/ByteArray.h"
#include "../datatype/TimeInstant.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryProperty.h"
#include "../memory/DataSetItem.h"
#include "DataSourceCatalogLoader.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "EWKBReader.h"
#include "EWKBSize.h"
#include "EWKBWriter.h"
#include "FwDataSet.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

// Boost
#include <boost/date_time.hpp>

// SQLite
#include <sqlite3.h>

te::sqlite::FwDataSet::FwDataSet(sqlite3_stmt* stmt, DataSourceTransactor* t, const bool releaseStmt)
  : m_originalsql(0),
    m_stmt(stmt),
    m_t(t),
    m_dt(0),
    m_releaseStmt(releaseStmt)
{
  assert(m_stmt != 0);

  m_dt = Convert2TerraLib(m_stmt);
}

te::sqlite::FwDataSet::~FwDataSet()
{
  delete m_originalsql;

  if(m_releaseStmt)
  {
    int ret = SQLITE_OK;

    ret = sqlite3_finalize(m_stmt);

    assert(ret == SQLITE_OK);
  }

  delete m_dt;
}

te::common::TraverseType te::sqlite::FwDataSet::getTraverseType() const
{
  return te::common::FORWARDONLY;
}

te::common::AccessPolicy te::sqlite::FwDataSet::getAccessPolicy() const
{
  return te::common::RAccess;
}

te::gm::Envelope* te::sqlite::FwDataSet::getExtent(std::size_t i)
{
  std::string sql("SELECT MIN(MBRMinX(");
              sql += m_dt->getProperty(i)->getName();
              sql += ")), MIN(MBRMinY(";
              sql += m_dt->getProperty(i)->getName();
              sql += ")), MAX(MBRMaxX(";
              sql += m_dt->getProperty(i)->getName();
              sql += ")), MAX(MBRMaxY(";
              sql += m_dt->getProperty(i)->getName();
              sql += ")) FROM (";
              sql += sqlite3_sql(m_stmt);
              sql += ")";

  std::auto_ptr<te::da::DataSet> result(m_t->query(sql));

  if(result->moveNext() == false)
    throw Exception(TR_SQLITE("Could not get a MBR for dataset!"));

  double xmin = result->getDouble(0);
  double ymin = result->getDouble(1);
  double xmax = result->getDouble(2);
  double ymax = result->getDouble(3);

  return new te::gm::Envelope(xmin, ymin, xmax, ymax);
}

std::size_t te::sqlite::FwDataSet::getNumProperties() const
{
  return m_dt->size();
}

int te::sqlite::FwDataSet::getPropertyDataType(std::size_t i) const
{
  return m_dt->getProperty(i)->getType();
}

std::string te::sqlite::FwDataSet::getPropertyName(std::size_t i) const
{
  return m_dt->getProperty(i)->getName();
}

std::string te::sqlite::FwDataSet::getDatasetNameOfProperty(std::size_t i) const
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

bool te::sqlite::FwDataSet::isEmpty() const
{
  throw Exception(TR_SQLITE("Not supported by SQLite!"));
}

std::size_t te::sqlite::FwDataSet::size() const
{
  return static_cast<std::size_t>(-1);
}

bool te::sqlite::FwDataSet::moveNext()
{
  return (sqlite3_step(m_stmt) == SQLITE_ROW);
}

bool te::sqlite::FwDataSet::movePrevious()
{
  throw Exception(TR_SQLITE("Not supported by SQLite!"));
}

bool te::sqlite::FwDataSet::moveFirst()
{
  int ret = sqlite3_reset(m_stmt);

  if(ret != SQLITE_OK)
    return false;

  return (sqlite3_step(m_stmt) == SQLITE_ROW);
}

bool te::sqlite::FwDataSet::moveBeforeFirst()
{
  return sqlite3_reset(m_stmt) == SQLITE_OK;
}

bool te::sqlite::FwDataSet::moveLast()
{
  throw Exception(TR_SQLITE("Not supported by SQLite!"));
}

bool te::sqlite::FwDataSet::move(std::size_t /*i*/)
{
  throw Exception(TR_SQLITE("Not supported by SQLite!"));
}

bool te::sqlite::FwDataSet::isAtBegin() const
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

bool te::sqlite::FwDataSet::isBeforeBegin() const
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

bool te::sqlite::FwDataSet::isAtEnd() const
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

bool te::sqlite::FwDataSet::isAfterEnd() const
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

char te::sqlite::FwDataSet::getChar(std::size_t i) const
{
  int value = sqlite3_column_int(m_stmt, i);

  return static_cast<char>(value);
}

unsigned char te::sqlite::FwDataSet::getUChar(std::size_t i) const
{
  int value = sqlite3_column_int(m_stmt, i);

  return static_cast<unsigned char>(value);
}

boost::int16_t te::sqlite::FwDataSet::getInt16(std::size_t i) const
{
  return static_cast<boost::int16_t>(sqlite3_column_int(m_stmt, i));
}

boost::int32_t te::sqlite::FwDataSet::getInt32(std::size_t i) const
{
  return sqlite3_column_int(m_stmt, i);
}

boost::int64_t te::sqlite::FwDataSet::getInt64(std::size_t i) const
{
  return sqlite3_column_int64(m_stmt, i);
}

bool te::sqlite::FwDataSet::getBool(std::size_t i) const
{
  return sqlite3_column_int(m_stmt, i) == TE_SQLITE_BOOL_TRUE;
}

float te::sqlite::FwDataSet::getFloat(std::size_t i) const
{
  return static_cast<float>(sqlite3_column_double(m_stmt, i));
}

double te::sqlite::FwDataSet::getDouble(std::size_t i) const
{
  return sqlite3_column_double(m_stmt, i);
}

std::string te::sqlite::FwDataSet::getNumeric(std::size_t i) const
{
  std::string value((const char*)(sqlite3_column_text(m_stmt, i)));

  return value; 
}

std::string te::sqlite::FwDataSet::getString(std::size_t i) const
{
  return (const char*)(sqlite3_column_text(m_stmt, i));
}

te::dt::ByteArray* te::sqlite::FwDataSet::getByteArray(std::size_t i) const
{
  char* blob = (char*)(sqlite3_column_blob(m_stmt, i));

  int size = sqlite3_column_bytes(m_stmt, i);

  te::dt::ByteArray* barray = new te::dt::ByteArray(size);

  barray->copy(blob, size);

  return barray;
}

te::gm::Geometry* te::sqlite::FwDataSet::getGeometry(std::size_t i) const
{
#ifdef TE_ENABLE_SPATIALITE 
  unsigned char* ewkb = (unsigned char*)(sqlite3_column_blob(m_stmt, i));
  te::gm::Geometry* g = EWKBReader::read(ewkb);
  return g;
#else
  return 0;
#endif  // TE_ENABLE_SPATIALITE
}

te::rst::Raster* te::sqlite::FwDataSet::getRaster(std::size_t /*i*/) const
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

te::dt::DateTime* te::sqlite::FwDataSet::getDateTime(std::size_t i) const
{
  int coltype = sqlite3_column_type(m_stmt, i);

  if(coltype == SQLITE_INTEGER)
  {
    int value = sqlite3_column_int(m_stmt, i);

    boost::posix_time::seconds s(value);

    boost::gregorian::date d(1970, 1, 1);

    boost::posix_time::ptime pt(d, s);

    return new te::dt::TimeInstant(pt);
  }
  else if(coltype == SQLITE_FLOAT)
  {
    //double value = sqlite3_column_double(m_stmt, i);

    throw Exception(TR_SQLITE("Not implemented yet!"));
  }
  else //if(coltype == SQLITE_TEXT)
  {
    const char* value = (const char*)(sqlite3_column_text(m_stmt, i));

    return new te::dt::TimeInstant(boost::posix_time::time_from_string(value));
  }
}

te::dt::Array* te::sqlite::FwDataSet::getArray(std::size_t /*i*/) const
{
  throw Exception(TR_SQLITE("Not supported by SQLite!"));
}

bool te::sqlite::FwDataSet::isNull(std::size_t i) const
{
  return sqlite3_column_type(m_stmt, i) == SQLITE_NULL;
}

int te::sqlite::FwDataSet::getBlobSize(int i) const
{
  int size = sqlite3_column_bytes(m_stmt, i);

  return size;
}

void te::sqlite::FwDataSet::getBlob(int i, void* buff) const
{
  const void* blob = sqlite3_column_blob(m_stmt, i);

  int size = sqlite3_column_bytes(m_stmt, i);

  memcpy(buff, blob, size);
}

