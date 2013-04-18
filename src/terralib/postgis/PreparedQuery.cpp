/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/postgis/PreparedQuery.cpp

  \brief A class that implements a prepared query for PostgreSQL data access driver.  
*/

// TerraLib
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/query/Query.h"
#include "../datatype/ByteArray.h"
#include "../datatype/DateTime.h"
#include "../geometry/Geometry.h"
#include "Connection.h"
#include "DataSet.h"
#include "DataSource.h"
#include "Exception.h"
#include "EWKBWriter.h"
#include "PreparedQuery.h"
#include "SQLVisitor.h"
#include "Transactor.h"
#include "Utils.h"

// STL
#include <cassert>
#include <cstring>

// Boost
#include <boost/cstdint.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// libpq
#include <libpq-fe.h>

namespace te
{
  namespace pgis
  {
    template<class T> inline void BindValue(te::pgis::PreparedQuery* pq, std::size_t i, std::size_t propertyPos, const te::dt::Property* p, T* d)
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
          throw Exception(TR_PGIS("The TerraLib data type is not supported by the PostgreSQL driver!"));
      }
    }

  } // end namespace pgis
}   // end namespace te

te::pgis::PreparedQuery::PreparedQuery(Transactor* t, const std::string& pqname)
  : m_t(t),
    m_conn(0),
    m_result(0),
    m_paramTypes(0),
    m_paramValues(0),
    m_paramLenghts(0),
    m_paramFormats(0),
    m_nparams(0),
    m_qname(pqname)
{
  m_conn = m_t->getConnection()->getConn();

  m_qname = te::common::Convert2LCase(m_qname);
}

te::pgis::PreparedQuery::~PreparedQuery()
{
  try
  {
    clear();
  }
  catch(...)
  {
  
  }
}

std::string te::pgis::PreparedQuery::getName() const
{
  return m_qname;
}

void te::pgis::PreparedQuery::prepare(const te::da::Query& query, const std::vector<te::dt::Property*>& paramTypes)
{
  assert(m_t && m_t->getPGDataSource() && m_t->getPGDataSource()->getDialect());
  std::string sql;

  SQLVisitor visitor(*(m_t->getPGDataSource()->getDialect()), sql, m_t->getConnection()->getConn());
  query.accept(visitor);

  prepare(sql, paramTypes);
}

void te::pgis::PreparedQuery::prepare(const std::string& query, const std::vector<te::dt::Property*>& queryParams)
{
// clear any previous prepared query
  clear();

// create parameters of prepared query
  m_nparams = queryParams.size();

  m_paramTypes = new unsigned int[m_nparams];

  memset(m_paramTypes, 0, m_nparams * sizeof(unsigned int));

  m_paramValues = new char*[m_nparams];

  memset(m_paramValues, 0, m_nparams * sizeof(char*));

  m_paramLenghts = new int[m_nparams];

  memset(m_paramLenghts, 0, m_nparams * sizeof(int));

  m_paramFormats = new int[m_nparams];

  memset(m_paramFormats, 0, m_nparams * sizeof(int));

// set informational parameters of prepared query
  for(std::size_t i = 0; i < m_nparams; ++i)
  {
    const te::dt::Property* p = queryParams[i];

    switch(p->getType())
    {
      case te::dt::CHAR_TYPE :
      case te::dt::UCHAR_TYPE :
      case te::dt::INT16_TYPE :
        m_paramTypes[i] = PG_INT2_TYPE;
        m_paramLenghts[i] = sizeof(boost::int16_t);
        m_paramFormats[i] = 1;
      break;

      case te::dt::INT32_TYPE :
        m_paramTypes[i] = PG_INT4_TYPE;
        m_paramLenghts[i] = sizeof(boost::int32_t);
        m_paramFormats[i] = 1;
      break;

      case te::dt::INT64_TYPE :
        m_paramTypes[i] = PG_INT8_TYPE;
        m_paramLenghts[i] = sizeof(boost::int64_t);
        m_paramFormats[i] = 1;
      break;

      case te::dt::BOOLEAN_TYPE :
        m_paramTypes[i] = PG_BOOL_TYPE;
        m_paramLenghts[i] = sizeof(char);
        m_paramFormats[i] = 1;
      break;

      case te::dt::FLOAT_TYPE :
        m_paramTypes[i] = PG_FLOAT4_TYPE;
        m_paramLenghts[i] = sizeof(float);
        m_paramFormats[i] = 1;
      break;

      case te::dt::DOUBLE_TYPE :
        m_paramTypes[i] = PG_FLOAT8_TYPE;
        m_paramLenghts[i] = sizeof(double);
        m_paramFormats[i] = 1;
      break;

      case te::dt::NUMERIC_TYPE :
        m_paramTypes[i] = PG_NUMERIC_TYPE;
        m_paramLenghts[i] = sizeof(double);
        m_paramFormats[i] = 1;
      break;

      case te::dt::STRING_TYPE :
      {
        const te::dt::StringProperty* sp = static_cast<const te::dt::StringProperty*>(p);

        if(sp->getSubType() == te::dt::VAR_STRING)
          m_paramTypes[i] = PG_VARCHAR_TYPE;
        else if(sp->getSubType() == te::dt::STRING)
          m_paramTypes[i] = PG_TEXT_TYPE;
        else
          m_paramTypes[i] =  PG_CHARACTER_TYPE;

        m_paramLenghts[i] = 0;
        m_paramFormats[i] = 1;

        break;
      }

      case te::dt::BYTE_ARRAY_TYPE:
        m_paramTypes[i] = PG_BYTEA_TYPE;
        m_paramLenghts[i] = 0;
        m_paramFormats[i] = 1;
      break;

      case te::dt::GEOMETRY_TYPE :
        m_paramTypes[i] = m_t->getPGDataSource()->getGeomTypeId();
        m_paramLenghts[i] = 0;
        m_paramFormats[i] = 1;
      break;

      case te::dt::DATETIME_TYPE:
      {
        const te::dt::DateTimeProperty* dtp = static_cast<const te::dt::DateTimeProperty*>(p);

        if(dtp->getSubType() == te::dt::DATE)
          m_paramTypes[i] = PG_DATE_TYPE;
        else if(dtp->getSubType() == te::dt::TIME_DURATION)
          m_paramTypes[i] = PG_TIME_TYPE;
        else if(dtp->getSubType() == te::dt::TIME_INSTANT)
          m_paramTypes[i] = PG_TIMESTAMP_TYPE;
        else
          m_paramTypes[i] = PG_TIMESTAMPTZ_TYPE;

        m_paramLenghts[i] = 0;
        m_paramFormats[i] = 0;
        break;
      }

      default:

        throw Exception(TR_PGIS("This TerraLib data type is not supported by the PostgreSQL driver!"));

      /*default :
        m_paramTypes[i] = 0;
        m_paramLenghts[i] = 0;
        m_paramFormats[i] = 1;
      break;*/
    }
  }

// make prepared query
  m_result = PQprepare(m_conn, m_qname.c_str(), query.c_str(), m_nparams, m_paramTypes);

// check result
  if((PQresultStatus(m_result) != PGRES_COMMAND_OK) &&
     (PQresultStatus(m_result) != PGRES_TUPLES_OK))
  {
    boost::format errmsg(TR_PGIS("Could not create the prepared query due to the following error: %1%."));
    
    errmsg = errmsg % PQerrorMessage(m_conn);

    throw Exception(errmsg.str());
  }
}

void te::pgis::PreparedQuery::execute()
{
  PQclear(m_result);

  m_result = PQexecPrepared(m_conn, m_qname.c_str(), m_nparams, m_paramValues, m_paramLenghts, m_paramFormats, 1);

// release param values data and set it to a null value
  for(std::size_t i = 0; i < m_nparams; ++i)
  {
    delete [] (m_paramValues[i]);
    m_paramValues[i] = 0;
    m_paramLenghts[i] = 0;
  }

// check result
  if((PQresultStatus(m_result) != PGRES_COMMAND_OK) &&
     (PQresultStatus(m_result) != PGRES_TUPLES_OK))
  {
    boost::format errmsg(TR_PGIS("Could not execute the prepared query due to the following error: %1%."));
    
    errmsg = errmsg % PQerrorMessage(m_conn);

    throw Exception(errmsg.str());
  }
}

te::da::DataSet* te::pgis::PreparedQuery::query(te::common::TraverseType /*travType*/,
                                                te::common::AccessPolicy /*rwRole*/)
{
  PQclear(m_result);

  m_result = PQexecPrepared(m_conn, m_qname.c_str(), m_nparams, m_paramValues, m_paramLenghts, m_paramFormats, 1);

// release param values data and set it to a null value
  for(std::size_t i = 0; i < m_nparams; ++i)
  {
    delete [] (m_paramValues[i]);
    m_paramValues[i] = 0;
    m_paramLenghts[i] = 0;
  }

// check result
  if((PQresultStatus(m_result) != PGRES_COMMAND_OK) &&
     (PQresultStatus(m_result) != PGRES_TUPLES_OK))
  {
    boost::format errmsg(TR_PGIS("Could not execute the prepared query due to the following error: %1%."));
    
    errmsg = errmsg % PQerrorMessage(m_conn);

    throw Exception(errmsg.str());
  }

  DataSet* dataset = new DataSet(m_result, m_t, 0);

  m_result = 0;

  return dataset;
}

void te::pgis::PreparedQuery::bind(int i, char value)
{
  assert(m_paramTypes[i] == PG_INT2_TYPE);

  boost::int16_t ivalue = value;

  if(m_paramValues[i] == 0)
    m_paramValues[i] = new char[sizeof(boost::int16_t)];

  memcpy(m_paramValues[i], &ivalue, sizeof(boost::int16_t));

#if TE_MACHINE_BYTE_ORDER == TE_NDR
  te::common::Swap2Bytes(m_paramValues[i]);
#endif
}

void te::pgis::PreparedQuery::bind(int i, unsigned char value)
{
  assert(m_paramTypes[i] == PG_INT2_TYPE);

  boost::int16_t ivalue = value;

  if(m_paramValues[i] == 0)
    m_paramValues[i] = new char[sizeof(boost::int16_t)];

  memcpy(m_paramValues[i], &ivalue, sizeof(boost::int16_t));

#if TE_MACHINE_BYTE_ORDER == TE_NDR
  te::common::Swap2Bytes(m_paramValues[i]);
#endif
}

void te::pgis::PreparedQuery::bind(int i, boost::int16_t value)
{
  assert(m_paramTypes[i] == PG_INT2_TYPE);

  if(m_paramValues[i] == 0)
    m_paramValues[i] = new char[sizeof(boost::int16_t)];

  memcpy(m_paramValues[i], &value, sizeof(boost::int16_t));

#if TE_MACHINE_BYTE_ORDER == TE_NDR
  te::common::Swap2Bytes(m_paramValues[i]);
#endif
}

void te::pgis::PreparedQuery::bind(int i, boost::int32_t value)
{
  assert(m_paramTypes[i] == PG_INT4_TYPE);

  if(m_paramValues[i] == 0)
    m_paramValues[i] = new char[sizeof(boost::int32_t)];

  memcpy(m_paramValues[i], &value, sizeof(boost::int32_t));

#if TE_MACHINE_BYTE_ORDER == TE_NDR
  te::common::Swap4Bytes(m_paramValues[i]);
#endif
}

void te::pgis::PreparedQuery::bind(int i, boost::int64_t value)
{
  assert(m_paramTypes[i] == PG_INT8_TYPE);

  if(m_paramValues[i] == 0)
    m_paramValues[i] = new char[sizeof(boost::int64_t)];

  memcpy(m_paramValues[i], &value, sizeof(boost::int64_t));

#if TE_MACHINE_BYTE_ORDER == TE_NDR
  te::common::Swap8Bytes(m_paramValues[i]);
#endif
}

void te::pgis::PreparedQuery::bind(int i, bool value)
{
  assert(m_paramTypes[i] == PG_BOOL_TYPE);

  if(m_paramValues[i] == 0)
    m_paramValues[i] = new char[sizeof(char)];

  char bvalue = value ? 1 : 0;

  memcpy(m_paramValues[i], &bvalue, sizeof(char));
}

void te::pgis::PreparedQuery::bind(int i, float value)
{
  assert(m_paramTypes[i] == PG_FLOAT4_TYPE);

  if(m_paramValues[i] == 0)
    m_paramValues[i] = new char[sizeof(float)];

  memcpy(m_paramValues[i], &value, sizeof(float));

#if TE_MACHINE_BYTE_ORDER == TE_NDR
  te::common::Swap4Bytes(m_paramValues[i]);
#endif
}

void te::pgis::PreparedQuery::bind(int i, double value)
{
  assert(m_paramTypes[i] == PG_FLOAT8_TYPE);

  if(m_paramValues[i] == 0)
    m_paramValues[i] = new char[sizeof(double)];

  memcpy(m_paramValues[i], &value, sizeof(double));

#if TE_MACHINE_BYTE_ORDER == TE_NDR
  te::common::Swap8Bytes(m_paramValues[i]);
#endif
}

void te::pgis::PreparedQuery::bindNumeric(int i, const std::string& value)
{
  assert((m_paramTypes[i] == PG_BYTEA_TYPE));

  delete [] (m_paramValues[i]);

  m_paramValues[i] = new char[value.length()];

  memcpy(m_paramValues[i], value.c_str(), value.length());

  m_paramLenghts[i] = value.length();
}

void te::pgis::PreparedQuery::bind(int i, const std::string& value)
{
  assert((m_paramTypes[i] == PG_VARCHAR_TYPE) || (m_paramTypes[i] == PG_CHARACTER_TYPE) || (m_paramTypes[i] == PG_TEXT_TYPE));

  delete [] (m_paramValues[i]);

  m_paramValues[i] = new char[value.length() + 1];

  memcpy(m_paramValues[i], value.c_str(), value.length() + 1);

  m_paramLenghts[i] = value.length();
}

void te::pgis::PreparedQuery::bind(int i, const te::dt::ByteArray& value)
{
  assert(m_paramTypes[i] == PG_BYTEA_TYPE);

  delete [] (m_paramValues[i]);

  m_paramValues[i] = new char[value.bytesUsed()];

  memcpy(m_paramValues[i], value.getData(), value.bytesUsed());

  m_paramLenghts[i] = value.bytesUsed();
}

void te::pgis::PreparedQuery::bind(int i, const te::gm::Geometry& value)
{
  assert(m_paramTypes[i] == m_t->getPGDataSource()->getGeomTypeId());

// clear previous data
  delete [] (m_paramValues[i]);

  m_paramValues[i] = 0;

// get ewkb
  std::size_t ewkbsize = value.getWkbSize() + 4;

  m_paramValues[i] = new char[ewkbsize];

  EWKBWriter::write(&value, m_paramValues[i]);

  m_paramLenghts[i] = ewkbsize;
}

void te::pgis::PreparedQuery::bind(int /*i*/, const te::rst::Raster& /*value*/)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::PreparedQuery::bind(int i, const te::dt::DateTime& value)
{
  assert((m_paramTypes[i] == PG_TIME_TYPE) || (m_paramTypes[i] == PG_DATE_TYPE) || (m_paramTypes[i] == PG_TIMESTAMP_TYPE) || (m_paramTypes[i] == PG_TIMESTAMPTZ_TYPE));

  delete [] (m_paramValues[i]);

  std::string dvalue = value.toString();

  m_paramValues[i] = new char[dvalue.length() + 1];

  memcpy(m_paramValues[i], dvalue.c_str(), dvalue.length() + 1);

  m_paramLenghts[i] = dvalue.length() + 1;
}

void te::pgis::PreparedQuery::bind(int /*i*/, const te::da::DataSet& /*value*/)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::PreparedQuery::bind(int /*i*/, const te::dt::AbstractData& /*ad*/)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

te::da::DataSourceTransactor* te::pgis::PreparedQuery::getTransactor() const
{
  return m_t;
}

void te::pgis::PreparedQuery::bind(const std::vector<std::size_t>& propertiesPos, std::size_t offset, const te::da::DataSetType* dt, te::da::DataSet* d)
{
  const std::size_t nparams = propertiesPos.size();

  for(std::size_t i = 0; i < nparams; ++i)
    BindValue(this, i + offset, propertiesPos[i], dt->getProperty(propertiesPos[i]), d);
}

void te::pgis::PreparedQuery::bind(const std::vector<std::size_t>& propertiesPos, const te::da::DataSetType* dt, te::da::DataSet* d)
{
  for(std::size_t i = 0; i < m_nparams; ++i)
    BindValue(this, i, propertiesPos[i], dt->getProperty(propertiesPos[i]), d);
}

void te::pgis::PreparedQuery::bind(const te::da::DataSetType* dt, te::da::DataSet* d)
{
  for(std::size_t i = 0; i < m_nparams; ++i)
    BindValue(this, i, i, dt->getProperty(i), d);
}

void te::pgis::PreparedQuery::clear()
{
  if(m_nparams == 0)
    return;

// release prepared statement
  m_t->execute("DEALLOCATE PREPARE " + m_qname);

// release any pending result
  PQclear(m_result);

  m_result = 0;

// release param types
  delete [] m_paramTypes;

  m_paramTypes = 0;

// release param values
  for(std::size_t i = 0; i < m_nparams; ++i)
    delete [] (m_paramValues[i]);

  delete [] m_paramValues;

  m_paramValues = 0;

// release param lengths
  delete [] m_paramLenghts;

  m_paramLenghts = 0;

// release param formats
  delete [] m_paramFormats;

  m_paramFormats = 0;

// don't forget to reset number of params!
  m_nparams = 0;
}


