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
  \file terralib/sqlite/PreparedQuery.cpp

  \brief A class that implements a prepared query for the TerraLib SQLite Data Access Driver.
*/

// TerraLib
#include "../common/Globals.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/query/Query.h"
#include "../datatype/ByteArray.h"
#include "../datatype/DateTime.h"
#include "../geometry/Geometry.h"
#include "../memory/DataSet.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"
#include "EWKBSize.h"
#include "EWKBWriter.h"
#include "Exception.h"
#include "FwDataSet.h"
#include "PreparedQuery.h"
#include "SQLVisitor.h"

// STL
#include <memory>

// Boost
#include <boost/date_time.hpp>
#include <boost/format.hpp>

// SQLite
#include <sqlite3.h>

namespace te
{
  namespace sqlite
  {

    template<class T> inline void BindValue(sqlite3_stmt* stmt, std::size_t i, std::size_t propertyPos, const te::dt::Property* p, T* d)
    {
      int retval = SQLITE_OK;

      if(!d->isNull(propertyPos))
      {
        switch(p->getType())
        {
          case te::dt::CHAR_TYPE :
            retval = sqlite3_bind_int(stmt, i + 1, d->getChar(propertyPos));
          break;

          case te::dt::UCHAR_TYPE :
            retval = sqlite3_bind_int(stmt, i + 1, d->getUChar(propertyPos));
          break;

          case te::dt::INT16_TYPE :
            retval = sqlite3_bind_int(stmt, i + 1, d->getInt16(propertyPos));
          break;

          case te::dt::INT32_TYPE :
            retval = sqlite3_bind_int(stmt, i + 1, d->getInt32(propertyPos));
          break;

          case te::dt::INT64_TYPE :
            retval = sqlite3_bind_int64(stmt, i + 1, d->getInt64(propertyPos));
          break;

          case te::dt::BOOLEAN_TYPE :
            retval = sqlite3_bind_int(stmt, i + 1, d->getBool(propertyPos) ? TE_SQLITE_BOOL_TRUE : TE_SQLITE_BOOL_FALSE);
          break;

          case te::dt::FLOAT_TYPE :
            retval = sqlite3_bind_double(stmt, i + 1, static_cast<double>(d->getFloat(propertyPos)));
          break;

          case te::dt::DOUBLE_TYPE :
            retval = sqlite3_bind_double(stmt, i + 1, d->getDouble(propertyPos));
          break;

          case te::dt::NUMERIC_TYPE :
            {
              std::string value = d->getNumeric(propertyPos);
              retval = sqlite3_bind_text(stmt, i + 1, value.c_str(), value.size(), SQLITE_TRANSIENT);
            }
          break;

          case te::dt::STRING_TYPE :
            {
              std::string value = d->getString(propertyPos);
              retval = sqlite3_bind_text(stmt, i + 1, value.c_str(), value.size(), SQLITE_TRANSIENT);
            }
          break;

          case te::dt::BYTE_ARRAY_TYPE :
            {
              std::auto_ptr<te::dt::ByteArray> b(d->getByteArray(propertyPos));

              retval = sqlite3_bind_blob(stmt, i + 1, b->getData(), b->bytesUsed(), SQLITE_TRANSIENT);
            }
          break;

          case te::dt::GEOMETRY_TYPE :
            {
#ifdef TE_ENABLE_SPATIALITE
              std::auto_ptr<te::gm::Geometry> g(d->getGeometry(propertyPos));

              std::size_t ewkbSize = EWKBSize::getEWKBSize(g.get());

              unsigned char* ewkb = new unsigned char[ewkbSize];

              EWKBWriter::write(g.get(), ewkb, te::common::Globals::sm_machineByteOrder);

              retval = sqlite3_bind_blob(stmt, i + 1, ewkb, ewkbSize, SQLITE_TRANSIENT);

              delete [] ewkb;
#endif  // TE_ENABLE_SPATIALITE
            }
          break;

          case te::dt::DATETIME_TYPE :
            {
              std::auto_ptr<te::dt::DateTime> dt(d->getDateTime(propertyPos));
              
              std::string sdate = dt->toString();

              retval = sqlite3_bind_text(stmt, i + 1, sdate.c_str(), sdate.size(), SQLITE_TRANSIENT);

              //const te::dt::DateTimeProperty* dtp = static_cast<const te::dt::DateTimeProperty*>(p);

              //if(dtp->getSubType() == te::dt::DateTimeProperty::TIME_INSTANT)
              //{
              //  
              //}
              //else //if(dtp->getSubType() == te::dt::DateTimeProperty::DATE)
              //{
              //  
              //}
            }
          break;

          default:
            throw Exception(TR_SQLITE("There is a TerraLib data type that can not be mapped to SQLite type system!"));
        }    
      }
      else
      {
        retval = sqlite3_bind_null(stmt, i + 1);
      }

      if(retval != SQLITE_OK)
        throw Exception(TR_SQLITE("Could not bind a value!"));
    }

  } // end namespace sqlite
}   // end namespace te

te::sqlite::PreparedQuery::PreparedQuery(DataSourceTransactor* t)
  : m_t(t),
    m_stmt(0)
{
}

te::sqlite::PreparedQuery::~PreparedQuery()
{
  sqlite3_finalize(m_stmt);
}

std::string te::sqlite::PreparedQuery::getName() const
{
  return "UNAMED";
}

void te::sqlite::PreparedQuery::prepare(const te::da::Query& query, const std::vector<te::dt::Property*>& /*paramTypes*/)
{
  assert(m_t && m_t->getLiteDataSource() && m_t->getLiteDataSource()->getDialect());
  std::string sql;

  SQLVisitor visitor(*(m_t->getLiteDataSource()->getDialect()), sql);
  query.accept(visitor);

  prepare(sql);
}

void te::sqlite::PreparedQuery::prepare(const std::string& query, const std::vector<te::dt::Property*>& /*paramTypes*/)
{
  sqlite3_finalize(m_stmt);

  m_stmt = 0;

  m_stmt = m_t->queryLite(query);
}

void te::sqlite::PreparedQuery::execute()
{
  int retval = sqlite3_step(m_stmt);

  if(retval != SQLITE_DONE)
  {
    boost::format errmsg(TR_SQLITE("Could not execute prepared query due to the following error: %1%."));

    const char* litemsg = sqlite3_errmsg(m_t->getLiteDataSource()->getDB());

    if(litemsg != 0)
      errmsg = errmsg % litemsg;

    throw Exception(errmsg.str());
  }

  retval = sqlite3_reset(m_stmt);
      
  if(retval != SQLITE_OK)
  {
    boost::format errmsg(TR_SQLITE("Could not reset the prepared query to its initial state due to the following error: %1%."));

    const char* litemsg = sqlite3_errmsg(m_t->getLiteDataSource()->getDB());

    if(litemsg)
      errmsg = errmsg % litemsg;

    throw Exception(errmsg.str());
  }
}

te::da::DataSet* te::sqlite::PreparedQuery::query(te::common::TraverseType /*travType*/, 
                                                  te::common::AccessPolicy /*rwRole*/)
{
  int retval = sqlite3_step(m_stmt);

  te::mem::DataSet* dataset = 0;

  if(retval == SQLITE_ROW)
  {
    FwDataSet fwdataset(m_stmt, m_t, false);

     dataset = new te::mem::DataSet(fwdataset);
  }
  else if(retval != SQLITE_DONE)
  {
    boost::format errmsg(TR_SQLITE("Could not excute prepared query due to the following error: %1%."));

    const char* litemsg = sqlite3_errmsg(m_t->getLiteDataSource()->getDB());

    if(litemsg)
      errmsg = errmsg % litemsg;

    throw Exception(errmsg.str());
  }
  else
  {
      dataset = new te::mem::DataSet(0);
  }

  retval = sqlite3_reset(m_stmt);
      
  if(retval != SQLITE_OK)
  {
    boost::format errmsg(TR_SQLITE("Could not reset the prepared query to its initial state due to the following error: %1%."));

    const char* litemsg = sqlite3_errmsg(m_t->getLiteDataSource()->getDB());

    if(litemsg)
      errmsg = errmsg % litemsg;

    throw Exception(errmsg.str());
  }

  return dataset;
}

void te::sqlite::PreparedQuery::bind(int i, char value)
{
  int retval = sqlite3_bind_int(m_stmt, i + 1, value);

  if(retval != SQLITE_OK)
    throw Exception(TR_SQLITE("Could not bind a char value!"));
}

void te::sqlite::PreparedQuery::bind(int i, unsigned char value)
{
  int retval = sqlite3_bind_int(m_stmt, i + 1, value);

  if(retval != SQLITE_OK)
    throw Exception(TR_SQLITE("Could not bind a unsigned char value!"));
}

void te::sqlite::PreparedQuery::bind(int i, boost::int16_t value)
{
  int retval = sqlite3_bind_int(m_stmt, i + 1, value);

  if(retval != SQLITE_OK)
    throw Exception(TR_SQLITE("Could not bind a 16-bit integer value!"));
}

void te::sqlite::PreparedQuery::bind(int i, boost::int32_t value)
{
  int retval = sqlite3_bind_int(m_stmt, i + 1, value);

  if(retval != SQLITE_OK)
    throw Exception(TR_SQLITE("Could not bind a 32-bit integer value!"));
}

void te::sqlite::PreparedQuery::bind(int i, boost::int64_t value)
{
  int retval = sqlite3_bind_int64(m_stmt, i + 1, value);

  if(retval != SQLITE_OK)
    throw Exception(TR_SQLITE("Could not bind a 64-bit integer value!"));
}

void te::sqlite::PreparedQuery::bind(int i, bool value)
{
  int retval = sqlite3_bind_int(m_stmt, i + 1, value ? TE_SQLITE_BOOL_TRUE : TE_SQLITE_BOOL_FALSE);

  if(retval != SQLITE_OK)
    throw Exception(TR_SQLITE("Could not bind a boolean value!"));
}

void te::sqlite::PreparedQuery::bind(int i, float value)
{
  int retval = sqlite3_bind_double(m_stmt, i + 1, static_cast<double>(value));

  if(retval != SQLITE_OK)
    throw Exception(TR_SQLITE("Could not bind a float value!"));
}

void te::sqlite::PreparedQuery::bind(int i, double value)
{
  int retval = sqlite3_bind_double(m_stmt, i + 1, value);

  if(retval != SQLITE_OK)
    throw Exception(TR_SQLITE("Could not bind a double value!"));
}

void te::sqlite::PreparedQuery::bindNumeric(int i, const std::string& value)
{
  int retval = sqlite3_bind_text(m_stmt, i + 1, value.c_str(), value.size(), SQLITE_TRANSIENT);

  if(retval != SQLITE_OK)
    throw Exception(TR_SQLITE("Could not bind a numeric value!"));
}

void te::sqlite::PreparedQuery::bind(int i, const std::string& value)
{
  int retval = sqlite3_bind_text(m_stmt, i + 1, value.c_str(), value.size(), SQLITE_TRANSIENT);

  if(retval != SQLITE_OK)
    throw Exception(TR_SQLITE("Could not bind a string value!"));
}

void te::sqlite::PreparedQuery::bind(int i, const te::dt::ByteArray& value)
{
  int retval = sqlite3_bind_blob(m_stmt, i + 1, value.getData(), value.bytesUsed(), SQLITE_TRANSIENT);

  if(retval != SQLITE_OK)
    throw Exception(TR_SQLITE("Could not bind a byte array value!"));
}

void te::sqlite::PreparedQuery::bind(int i, const te::gm::Geometry& value)
{
  std::size_t ewkbSize = EWKBSize::getEWKBSize(&value);

  unsigned char* ewkb = new unsigned char[ewkbSize];

  EWKBWriter::write(&value, ewkb, te::common::Globals::sm_machineByteOrder);

  int retval = sqlite3_bind_blob(m_stmt, i + 1, ewkb, ewkbSize, SQLITE_TRANSIENT);

  delete [] ewkb;

  if(retval != SQLITE_OK)
    throw Exception(TR_SQLITE("Could not bind a geometry value!"));
}

void te::sqlite::PreparedQuery::bind(int /*i*/, const te::rst::Raster& /*value*/)
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

void te::sqlite::PreparedQuery::bind(int i, const te::dt::DateTime& value)
{
  std::string sdate = value.toString();

  int retval = sqlite3_bind_text(m_stmt, i + 1, sdate.c_str(), sdate.size(), SQLITE_TRANSIENT);

  if(retval != SQLITE_OK)
    throw Exception(TR_SQLITE("Could not bind a date-time value!"));
}

void te::sqlite::PreparedQuery::bind(int /*i*/, const te::da::DataSet& /*value*/)
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

void te::sqlite::PreparedQuery::bind(int /*i*/, const te::dt::AbstractData& /*ad*/)
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

te::da::DataSourceTransactor* te::sqlite::PreparedQuery::getTransactor() const
{
  return m_t;
}

// SQLite specific
void te::sqlite::PreparedQuery::prepare(const std::string& query)
{
  sqlite3_finalize(m_stmt);

  m_stmt = 0;

  m_stmt = m_t->queryLite(query);
}

//void te::sqlite::PreparedQuery::bind(const std::vector<std::size_t>& propertiesPos, std::size_t offset, const te::da::DataSetType* dt, te::da::DataSetItem* item)
//{
//  const std::size_t nparams = propertiesPos.size();
//
//  for(std::size_t i = 0; i < nparams; ++i)
//    BindValue(m_stmt, i + offset, propertiesPos[i], dt->getProperty(propertiesPos[i]), item);
//}
//
//void te::sqlite::PreparedQuery::bind(const std::vector<std::size_t>& propertiesPos, const te::da::DataSetType* dt, te::da::DataSetItem* item)
//{
//  const std::size_t nparams = propertiesPos.size();
//
//  for(std::size_t i = 0; i < nparams; ++i)
//    BindValue(m_stmt, i, propertiesPos[i], dt->getProperty(propertiesPos[i]), item);
//}
//
//void te::sqlite::PreparedQuery::bind(const te::da::DataSetType* dt, te::da::DataSetItem* item)
//{
//  const std::size_t nparams = dt->size();
//
//  for(std::size_t i = 0; i < nparams; ++i)
//    BindValue(m_stmt, i, i, dt->getProperty(i), item);
//}

void te::sqlite::PreparedQuery::bind(const std::vector<std::size_t>& propertiesPos, std::size_t offset, const te::da::DataSetType* dt, te::da::DataSet* d)
{
  const std::size_t nparams = propertiesPos.size();

  for(std::size_t i = 0; i < nparams; ++i)
    BindValue(m_stmt, i + offset, propertiesPos[i], dt->getProperty(propertiesPos[i]), d);
}

void te::sqlite::PreparedQuery::bind(const std::vector<std::size_t>& propertiesPos, const te::da::DataSetType* dt, te::da::DataSet* d)
{
  const std::size_t nparams = propertiesPos.size();

  for(std::size_t i = 0; i < nparams; ++i)
    BindValue(m_stmt, i, propertiesPos[i], dt->getProperty(propertiesPos[i]), d);
}

void te::sqlite::PreparedQuery::bind(const te::da::DataSetType* dt, te::da::DataSet* d)
{
  const std::size_t nparams = dt->size();

  for(std::size_t i = 0; i < nparams; ++i)
    BindValue(m_stmt, i, i, dt->getProperty(i), d);
}





