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
  \file terralib/mysql/PreparedQuery.cpp

  \brief This class implements the prepared query for MySQL data access driver.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetItem.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/query/Query.h"
#include "../datatype/ByteArray.h"
#include "../datatype/DateTime.h"
#include "../geometry/Geometry.h"
#include "Connection.h"
#include "DataSet.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "PreparedQuery.h"
#include "SQLVisitor.h"

// STL
#include <cassert>
#include <cctype>
#include <memory>
#include <sstream>

// Boost
#include <boost/format.hpp>

// MySQL
#include <driver/mysql_public_iface.h>

namespace te
{
  namespace mysql
  {
    inline void BindValue(te::mysql::PreparedQuery* pq, std::size_t i, std::size_t propertyPos, const te::dt::Property* p, te::da::DataSetItem* d)
    {
      switch(p->getType())
      {
        case te::dt::CHAR_TYPE :
          pq->bind(i, d->getChar(propertyPos));
        break;

        case te::dt::UCHAR_TYPE :
          pq->bind(i, d->getUChar(propertyPos));
        break;

        case te::dt::INT16_TYPE :
          pq->bind(i, d->getInt16(propertyPos));
        break;

        case te::dt::INT32_TYPE :
          pq->bind(i, d->getInt32(propertyPos));
        break;

        case te::dt::INT64_TYPE :
          pq->bind(i, d->getInt64(propertyPos));
        break;

        case te::dt::BOOLEAN_TYPE :
          pq->bind(i, d->getBool(propertyPos));
        break;

        case te::dt::FLOAT_TYPE :
          pq->bind(i, d->getFloat(propertyPos));
        break;

        case te::dt::DOUBLE_TYPE :
          pq->bind(i, d->getDouble(propertyPos));
        break;

        case te::dt::NUMERIC_TYPE :
          pq->bindNumeric(i, d->getNumeric(propertyPos));
        break;

        case te::dt::STRING_TYPE :
          pq->bind(i, d->getString(propertyPos));
        break;

        case te::dt::BYTE_ARRAY_TYPE :
          {
            std::auto_ptr<te::dt::ByteArray> ba(d->getByteArray(propertyPos));
            pq->bind(i, *ba);
          }
        break;

        case te::dt::GEOMETRY_TYPE :
          {
            std::auto_ptr<te::gm::Geometry> geom(d->getGeometry(propertyPos));
            pq->bind(i, *geom);
          }
        break;

        case te::dt::DATETIME_TYPE :
          {
            std::auto_ptr<te::dt::DateTime> dt(d->getDateTime(propertyPos));
            pq->bind(i, *dt);
          }
        break;

        default :
          throw Exception(TR_MYSQL("The TerraLib data type is not supported by the MySQL driver!"));
      }
    }

  } // end namespace mysql
}   // end namespace te

te::mysql::PreparedQuery::PreparedQuery(DataSourceTransactor* t)
  : m_t(t),
    m_pstmt(0),
    m_nparams(0)
{
}

te::mysql::PreparedQuery::~PreparedQuery()
{
  delete m_pstmt;
  te::common::FreeContents(m_blobs);
}

std::string te::mysql::PreparedQuery::getName() const
{
  return std::string("UNAMED");
}

void te::mysql::PreparedQuery::prepare(const te::da::Query& query, const std::vector<te::dt::Property*>& paramTypes)
{
  assert(m_t && m_t->getDataSource() && m_t->getDataSource()->getDialect());

  std::string sql;

  SQLVisitor visitor(*(m_t->getDataSource()->getDialect()), sql);
  query.accept(visitor);

  prepare(sql, paramTypes);
}

void te::mysql::PreparedQuery::prepare(const std::string& query, const std::vector<te::dt::Property*>& paramTypes)
{
  delete m_pstmt;
  m_pstmt = m_t->getConnection()->getConn()->prepareStatement(query);

  te::common::FreeContents(m_blobs);
  m_blobs.clear();
  m_nparams = paramTypes.size();
  m_blobs.resize(m_nparams, 0);
}

void te::mysql::PreparedQuery::execute()
{
  m_pstmt->executeUpdate();

// free blobs if any
  for(std::size_t i = 0; i < m_blobs.size(); ++i)
  {
    delete m_blobs[i];
    m_blobs[i] = 0;
  }
}

te::da::DataSet* te::mysql::PreparedQuery::query(te::common::TraverseType /*travType*/, 
                                                 te::common::AccessPolicy /*rwRole*/)
{
  try
  {
    sql::ResultSet* result = m_pstmt->executeQuery();

// free blobs if any
    for(std::size_t i = 0; i < m_blobs.size(); ++i)
    {
      delete m_blobs[i];
      m_blobs[i] = 0;
    }

    return new DataSet(m_t, result, 0);
  }
  catch(const sql::SQLException& e)
  {
    throw Exception((boost::format(TR_MYSQL("Could not execute query due to the following error: %1%")) % e.what()).str());
  }
}

void te::mysql::PreparedQuery::bind(int i, char value)
{
  m_pstmt->setInt(i + 1, value);
}

void te::mysql::PreparedQuery::bind(int i, unsigned char value)
{
  m_pstmt->setUInt(i + 1, value);
}

void te::mysql::PreparedQuery::bind(int i, boost::int16_t value)
{
  m_pstmt->setInt(i + 1, value);
}

void te::mysql::PreparedQuery::bind(int i, boost::int32_t value)
{
  m_pstmt->setInt(i + 1, value);
}

void te::mysql::PreparedQuery::bind(int i, boost::int64_t value)
{
  m_pstmt->setInt64(i + 1, value);
}

void te::mysql::PreparedQuery::bind(int i, bool value)
{
  m_pstmt->setBoolean(i + 1, value);
}

void te::mysql::PreparedQuery::bind(int i, float value)
{
  m_pstmt->setDouble(i + 1, value);
}

void te::mysql::PreparedQuery::bind(int i, double value)
{
  m_pstmt->setDouble(i + 1, value);
}

void te::mysql::PreparedQuery::bindNumeric(int i, const std::string& value)
{
  m_pstmt->setString(i + 1, value);
}

void te::mysql::PreparedQuery::bind(int i, const std::string& value)
{
  m_pstmt->setString(i + 1, value);
}

void te::mysql::PreparedQuery::bind(int i, const te::dt::ByteArray& value)
{
  std::string binstr(value.getData(), value.bytesUsed());

  std::istringstream* iss = new std::istringstream(binstr, std::ios_base::binary);

  delete m_blobs[i];
  m_blobs[i] = iss;

  m_pstmt->setBlob(i + 1, iss);
}

void te::mysql::PreparedQuery::bind(int i, const te::gm::Geometry& value)
{
  std::size_t size = 0;

  char* wkb = value.asBinary(size);

  std::string binstr(wkb, size);

  delete [] wkb;

  std::istringstream* iss = new std::istringstream(binstr, std::ios_base::binary);

  delete m_blobs[i];
  m_blobs[i] = iss;

  m_pstmt->setBlob(i + 1, iss);
}

void te::mysql::PreparedQuery::bind(int /*i*/, const te::rst::Raster& /*value*/)
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

void te::mysql::PreparedQuery::bind(int /*i*/, const te::dt::DateTime& /*value*/)
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

void te::mysql::PreparedQuery::bind(int /*i*/, const te::da::DataSet& /*value*/)
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

te::da::DataSourceTransactor* te::mysql::PreparedQuery::getTransactor() const
{
  return m_t;
}

// MySQL specific
void te::mysql::PreparedQuery::bind(const std::vector<std::size_t>& propertiesPos, const te::da::DataSetType* dt, te::da::DataSetItem* item)
{
  for(std::size_t i = 0; i < m_nparams; ++i)
    BindValue(this, i, propertiesPos[i], dt->getProperty(propertiesPos[i]), item);
}

void te::mysql::PreparedQuery::bind(const te::da::DataSetType* dt, te::da::DataSetItem* item)
{
  for(std::size_t i = 0; i < m_nparams; ++i)
    BindValue(this, i, i, dt->getProperty(i), item);
}

void te::mysql::PreparedQuery::bind(const std::vector<std::size_t>& propertiesPos, std::size_t offset, const te::da::DataSetType* dt, te::da::DataSetItem* item)
{
  const std::size_t nparams = propertiesPos.size();

  for(std::size_t i = 0; i < nparams; ++i)
    BindValue(this, i + offset, propertiesPos[i], dt->getProperty(propertiesPos[i]), item);
}

