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
  : m_geomFilter(0),
    m_mbrFilter(0),
    m_originalsql(0),
    m_stmt(stmt),
    m_t(t),
    m_dt(0),
    m_releaseStmt(releaseStmt)
{
  assert(m_stmt != 0);
}

te::sqlite::FwDataSet::~FwDataSet()
{
  delete m_geomFilter;

  delete m_mbrFilter;

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

te::da::DataSetType* te::sqlite::FwDataSet::getType()
{
  if(m_dt)
    return m_dt;

  m_dt = Convert2TerraLib(m_stmt);

  m_dt->setCategory(te::da::QUERY_TYPE);

  return m_dt;
}

const te::da::DataSetType* te::sqlite::FwDataSet::getType() const
{
  if(m_dt)
    return m_dt;

  m_dt = Convert2TerraLib(m_stmt);

  m_dt->setCategory(te::da::QUERY_TYPE);

  return m_dt;
}

te::da::DataSourceTransactor* te::sqlite::FwDataSet::getTransactor() const
{
  return m_t;
}

void te::sqlite::FwDataSet::loadTypeInfo()
{
  if(m_dt && m_dt->isFullLoaded())
    return;

  if(m_dt == 0)
    m_dt = Convert2TerraLib(m_stmt);

  m_dt->setCategory(te::da::QUERY_TYPE);

  const int ncols = sqlite3_column_count(m_stmt);

  if(ncols == 0)
    return;

// try to find a table name
  std::string tname = sqlite3_column_table_name(m_stmt, 0);

  for(int i = 0; i < ncols; ++i)  // note: !!start at 0 to test for a geometry column!!
  {
    const char* ttname = sqlite3_column_table_name(m_stmt, i);

    if(ttname == 0)
      continue;

    if(m_dt->getProperty(i)->getType() == te::dt::GEOMETRY_TYPE)
    {
      std::auto_ptr<DataSourceCatalogLoader> cloader(m_t->getSQLiteCatalogLoader());
      
      std::auto_ptr<te::da::DataSet> ftable(cloader->getGeometryInfo(ttname, m_dt->getProperty(i)->getName()));

      if(!ftable->moveNext())
        continue;

      std::string gtype = ftable->getString(0);

      std::string coorddim = ftable->getString(1);

      te::gm::GeomType t = Convert2TerraLibGeomType(gtype, coorddim);

      te::gm::GeometryProperty* gp = static_cast<te::gm::GeometryProperty*>(m_dt->getProperty(i));

      gp->setGeometryType(t);

      int srid = ftable->getInt32(2);

      gp->setSRID(srid);

      //bool hasSPIDX = ftable->getInt32(3) != 0;
    }

// if dataset has columns from more than one table (a join) we can not get more reliable info!
    if(tname != ttname)
    {
      m_dt->setFullLoaded(true);
      return;
    }     
  }

  m_dt->setName(tname);

// let's try to extract at least pk info

  std::string sql  = "PRAGMA table_info(";
              sql += tname;
              sql += ")";

  std::auto_ptr<te::da::DataSet> tinfo(m_t->query(sql));

  std::auto_ptr<te::da::PrimaryKey> pk(new te::da::PrimaryKey("", 0, 0));

  while(tinfo->moveNext())
  {
    bool ispk = tinfo->getBool(5);

    if(ispk)
    {
      int colid = tinfo->getInt32(0);

      std::string colName = tinfo->getString(1);

      te::dt::Property* p = 0;

      if(colid < ncols)
      {
        p = m_dt->getProperty(colid);

        if(p && p->getName() != colName)
        {
          p = m_dt->getProperty(colName);
        }
      }
      else
      {
        p = m_dt->getProperty(colName);
      }

      if(p == 0)
        continue;

      pk->add(p);
    }
  }

  if(pk->getProperties().size() > 0)
    m_dt->setPrimaryKey(pk.release());

// todo:
// 1. look for uk
// 2. extract more info for geometric columns

  m_dt->setFullLoaded(true);
}

te::da::DataSet* te::sqlite::FwDataSet::getParent() const
{
  return 0;
}

te::gm::Envelope* te::sqlite::FwDataSet::getExtent(const te::dt::Property* p)
{
  std::string sql("SELECT MIN(MBRMinX(");
              sql += p->getName();
              sql += ")), MIN(MBRMinY(";
              sql += p->getName();
              sql += ")), MAX(MBRMaxX(";
              sql += p->getName();
              sql += ")), MAX(MBRMaxY(";
              sql += p->getName();
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

void te::sqlite::FwDataSet::setFilter(te::dt::Property* p,
                                    const te::gm::Geometry* g,
                                    te::gm::SpatialRelation r)
{
#ifdef TE_ENABLE_SPATIALITE
  if(m_originalsql == 0)
    m_originalsql = new std::string(sqlite3_sql(m_stmt));

  if(g == 0)
  {
    m_stmt = m_t->queryLite(*m_originalsql);

    return;
  }

  if(p == 0)
    throw Exception(TR_SQLITE("The property is missing!"));

  if(p->getType() != te::dt::GEOMETRY_TYPE)
    throw Exception(TR_SQLITE("The property is not geometric!"));

  if(m_originalsql == 0)
    m_originalsql = new std::string(sqlite3_sql(m_stmt));

  std::string tname = sqlite3_column_table_name(m_stmt, getType()->getPropertyPosition(p));

  std::string sql("SELECT * FROM (");
              sql += *m_originalsql;
              sql += ") WHERE ";
              sql += GetBindableSpatialRelation(p->getName(), r);
              sql += " AND ROWID IN (SELECT pkid FROM ";
              sql += "idx_" + tname + "_" + p->getName();
              sql += " WHERE pkid MATCH ";
              sql += GetRtreeFilter(g->getMBR(), r);
              sql += ")";

  int ret = sqlite3_finalize(m_stmt);

  m_stmt = 0;

  if(ret != SQLITE_OK)
    throw Exception(TR_SQLITE("Could not apply MBR filter!"));

  m_stmt = m_t->queryLite(sql);

  std::size_t ewkbSize = EWKBSize::getEWKBSize(g);

  unsigned char* ewkb = new unsigned char[ewkbSize];

  EWKBWriter::write(g, ewkb, te::common::Globals::sm_machineByteOrder);

  int retval = sqlite3_bind_blob(m_stmt, 1, ewkb, ewkbSize, SQLITE_TRANSIENT);

  delete [] ewkb;

  if(retval != SQLITE_OK)
    throw Exception(TR_SQLITE("Could not bind geometry in the spatial filter!"));

#endif  // TE_ENABLE_SPATIALITE
}
 
void te::sqlite::FwDataSet::setFilter(te::dt::Property* p,
                                    const te::gm::Envelope* e,
                                    te::gm::SpatialRelation r)
{
#ifdef TE_ENABLE_SPATIALITE
  if(m_originalsql == 0)
    m_originalsql = new std::string(sqlite3_sql(m_stmt));

  if(e == 0)
  {
    m_stmt = m_t->queryLite(*m_originalsql);

    return;
  }

  if(p == 0)
    throw Exception(TR_SQLITE("The property is missing!"));

  if(p->getType() != te::dt::GEOMETRY_TYPE)
    throw Exception(TR_SQLITE("The property is not geometric!"));

  std::string tname = sqlite3_column_table_name(m_stmt, getType()->getPropertyPosition(p));

  std::string sql("SELECT * FROM (");
              sql += *m_originalsql;
              sql += ") WHERE ROWID IN (SELECT pkid FROM ";
              sql += "idx_" + tname + "_" + p->getName();
              sql += " WHERE pkid MATCH ";
              sql += GetRtreeFilter(e, r);
              sql += ")";

  int ret = sqlite3_finalize(m_stmt);

  m_stmt = 0;

  if(ret != SQLITE_OK)
    throw Exception(TR_SQLITE("Could not apply MBR filter!"));

  m_stmt = m_t->queryLite(sql);
#endif //TE_ENABLE_SPATIALITE
}

te::da::DataSetItem* te::sqlite::FwDataSet::getItem() const
{
  return new te::mem::DataSetItem(this);
}

void te::sqlite::FwDataSet::add(te::da::DataSetItem* /*item*/)
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

bool te::sqlite::FwDataSet::moveAfterLast()
{
  while(moveNext())
    ;

  return true;
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

char te::sqlite::FwDataSet::getChar(int i) const
{
  int value = sqlite3_column_int(m_stmt, i);

  return static_cast<char>(value);
}

void te::sqlite::FwDataSet::setChar(int /*i*/, char /*value*/)
{
  throw Exception(TR_SQLITE("Not supported by SQLite!"));
}

unsigned char te::sqlite::FwDataSet::getUChar(int i) const
{
  int value = sqlite3_column_int(m_stmt, i);

  return static_cast<unsigned char>(value);
}

void te::sqlite::FwDataSet::setUChar(int /*i*/, unsigned char /*value*/)
{
  throw Exception(TR_SQLITE("Not supported by SQLite!"));
}

boost::int16_t te::sqlite::FwDataSet::getInt16(int i) const
{
  return static_cast<boost::int16_t>(sqlite3_column_int(m_stmt, i));
}

void te::sqlite::FwDataSet::setInt16(int /*i*/, boost::int16_t /*value*/)
{
  throw Exception(TR_SQLITE("Not supported by SQLite!"));
}

boost::int32_t te::sqlite::FwDataSet::getInt32(int i) const
{
  return sqlite3_column_int(m_stmt, i);
}

void te::sqlite::FwDataSet::setInt32(int /*i*/, boost::int32_t /*value*/)
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

boost::int64_t te::sqlite::FwDataSet::getInt64(int i) const
{
  return sqlite3_column_int64(m_stmt, i);
}

void te::sqlite::FwDataSet::setInt64(int /*i*/, boost::int64_t /*value*/)
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

bool te::sqlite::FwDataSet::getBool(int i) const
{
  return sqlite3_column_int(m_stmt, i) == TE_SQLITE_BOOL_TRUE;
}

void te::sqlite::FwDataSet::setBool(int /*i*/, bool /*value*/)
{
  throw Exception(TR_SQLITE("Not supported by SQLite!"));
}

float te::sqlite::FwDataSet::getFloat(int i) const
{
  return static_cast<float>(sqlite3_column_double(m_stmt, i));
}

void te::sqlite::FwDataSet::setFloat(int /*i*/, float /*value*/)
{
  throw Exception(TR_SQLITE("Not supported by SQLite!"));
}

double te::sqlite::FwDataSet::getDouble(int i) const
{
  return sqlite3_column_double(m_stmt, i);
}

void te::sqlite::FwDataSet::setDouble(int /*i*/, double /*value*/)
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

std::string te::sqlite::FwDataSet::getNumeric(int i) const
{
  std::string value((const char*)(sqlite3_column_text(m_stmt, i)));

  return value; 
}

void te::sqlite::FwDataSet::setNumeric(int /*i*/, const std::string& /*value*/)
{
  throw Exception(TR_SQLITE("Not supported by SQLite!"));
}

std::string te::sqlite::FwDataSet::getString(int i) const
{
  return (const char*)(sqlite3_column_text(m_stmt, i));
}

void te::sqlite::FwDataSet::setString(int /*i*/, const std::string& /*value*/)
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

te::dt::ByteArray* te::sqlite::FwDataSet::getByteArray(int i) const
{
  char* blob = (char*)(sqlite3_column_blob(m_stmt, i));

  int size = sqlite3_column_bytes(m_stmt, i);

  te::dt::ByteArray* barray = new te::dt::ByteArray(size);

  barray->copy(blob, size);

  return barray;
}

void te::sqlite::FwDataSet::setByteArray(int /*i*/, const te::dt::ByteArray& /*value*/)
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

te::gm::Geometry* te::sqlite::FwDataSet::getGeometry(int i) const
{
#ifdef TE_ENABLE_SPATIALITE 
  unsigned char* ewkb = (unsigned char*)(sqlite3_column_blob(m_stmt, i));
  te::gm::Geometry* g = EWKBReader::read(ewkb);
  return g;
#else
  return 0;
#endif  // TE_ENABLE_SPATIALITE
}

void te::sqlite::FwDataSet::setGeometry(int /*i*/, const te::gm::Geometry& /*value*/)
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

te::rst::Raster* te::sqlite::FwDataSet::getRaster(int /*i*/) const
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

void te::sqlite::FwDataSet::setRaster(int /*i*/, const te::rst::Raster& /*value*/)
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

te::dt::DateTime* te::sqlite::FwDataSet::getDateTime(int i) const
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

void te::sqlite::FwDataSet::setDateTime(int /*i*/, const te::dt::DateTime& /*value*/)
{
  throw Exception(TR_SQLITE("Not supported by SQLite!"));
}

void te::sqlite::FwDataSet::getArray(int /*i*/, std::vector<boost::int16_t>& /*values*/) const
{
  throw Exception(TR_SQLITE("Not supported by SQLite!"));
}

const unsigned char* te::sqlite::FwDataSet::getWKB(int i) const
{
  return (const unsigned char*)sqlite3_column_blob(m_stmt, i);
}

te::da::DataSet* te::sqlite::FwDataSet::getDataSet(int /*i*/)
{
  throw Exception(TR_SQLITE("Not supported by SQLite!"));
}

void te::sqlite::FwDataSet::setDataSet(int /*i*/, const te::da::DataSet& /*value*/)
{
  throw Exception(TR_SQLITE("Not supported by SQLite!"));
}

void te::sqlite::FwDataSet::setValue(int /*i*/, te::dt::AbstractData* /*ad*/)
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

bool te::sqlite::FwDataSet::isNull(int i) const
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

