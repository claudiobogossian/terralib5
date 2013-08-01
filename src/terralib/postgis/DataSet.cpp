/*  Copyright (C) 2001-2010 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/postgis/DataSet.cpp

  \brief Implementation of a dataset for PostGIS driver.
*/

// TerraLib
#include "../common/ByteSwapUtils.h"
#include "../common/Globals.h"
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../datatype/Array.h"
#include "../datatype/ByteArray.h"
#include "../datatype/DateTime.h"
#include "../datatype/SimpleData.h"
#include "../geometry/Geometry.h"
#include "Connection.h"
#include "CatalogLoader.h"
#include "DataSet.h"
#include "DataSource.h"
#include "EWKBReader.h"
#include "Exception.h"
#include "Transactor.h"
#include "Utils.h"

// STL
#include <memory>

// Boost
#include <boost/dynamic_bitset.hpp>

// libpq
#include <libpq-fe.h>

namespace te
{
  namespace pgis
  {
    /*!
      \brief It retrieves information from an unidimensional array.

      Requirements on types:
      <ul>
      <li>T: the type of basic elements of the array.</li>
      <li>ALIGN: the size in bytes of pointer alignment for the type T.</li>
      </ul>

      \param i      The row.
      \param j      The column.
      \param result The result to extract the array.
      \param a      The output vector.

      \warning Don't call this method for multidimensional arrays!
     */
    template<class T, class ALIGN> inline void GetArray(int i, int j, PGresult* result, std::vector<T>& a)
    {
      char* value = PQgetvalue(result, i, j);

      int ndim = *((int*)value);
      value += sizeof(int);

      int dataoffset = *((int*)value);
      value += sizeof(int);

      unsigned int elemtype = *((unsigned int*)value);
      value += sizeof(unsigned int);

      int dimension = *((int*)value);
      value += sizeof(int);

      int lowerbnds = *((int*)value);
      value += (2 * sizeof(int)); //jump null-bitmap field

      if(te::common::Globals::sm_machineByteOrder == TE_NDR)
      {
        te::common::SwapBytes(ndim);
        te::common::SwapBytes(dataoffset);
        te::common::SwapBytes(elemtype);
        te::common::SwapBytes(dimension);
        te::common::SwapBytes(lowerbnds);

        a.reserve(dimension);

        for(int k = 0; k < dimension; ++k)
        {
          T v = *((T*)value);
          te::common::SwapBytes(v);
          a.push_back(v);
          value += sizeof(ALIGN);
        }
      }
      else
      {
        a.reserve(dimension);

        for(int k = 0; k < dimension; ++k)
        {
          T v = *((T*)value);
          a.push_back(v);
          value += sizeof(ALIGN);
        }
      }
    }

  } // end namespace pgis
}   // end namespace te

te::pgis::DataSet::DataSet(PGresult* result,
                           Transactor* transactor,
                           std::string* sql)
  : m_i(-1),
    m_result(result),
    m_t(transactor),
    m_sql(sql)
{
  m_size = PQntuples(m_result);

  Convert2TerraLib(m_result, m_t->getPGDataSource()->getGeomTypeId(), m_t->getPGDataSource()->getRasterTypeId(), m_ptypes);
}

te::pgis::DataSet::~DataSet()
{
  PQclear(m_result);
  delete m_sql;
}

te::common::TraverseType te::pgis::DataSet::getTraverseType() const
{
  return te::common::RANDOM;
}

te::common::AccessPolicy te::pgis::DataSet::getAccessPolicy() const
{
  return te::common::RAccess;
}

te::gm::Envelope* te::pgis::DataSet::getExtent(std::size_t i)
{
  if(m_ptypes[i] != te::dt::GEOMETRY_TYPE)
    throw Exception(TR_PGIS("This driver only supports the getExtent method over a geometry column!"));

  std::string sql("SELECT ST_Extent(");
              sql += PQfname(m_result, i);
              sql += ") FROM (";
              sql += *m_sql;
              sql += ") pgis_driver_subquery";

  PGresult* result = PQexec(m_t->getConnection()->getConn(), sql.c_str());

  if(PQresultStatus(result) != PGRES_TUPLES_OK)
  {
    std::string errmsg(TR_PGIS("Could not find mbr for the given geometry property due to the following error: "));
                errmsg += PQerrorMessage(m_t->getConnection()->getConn());

    PQclear(result);

    throw Exception(errmsg);
  }

  const char* boxStr = PQgetvalue(result, 0, 0);

  te::gm::Envelope* mbr = 0;

  if(*boxStr != '\0')
    mbr = GetEnvelope(boxStr);

  PQclear(result);

  return mbr;
}

std::size_t te::pgis::DataSet::getNumProperties() const
{
  return PQnfields(m_result);
}

int te::pgis::DataSet::getPropertyDataType(std::size_t i) const
{
  return m_ptypes[i];
}

std::string te::pgis::DataSet::getPropertyName(std::size_t i) const
{
  return PQfname(m_result, i);
}

std::string te::pgis::DataSet::getDatasetNameOfProperty(std::size_t i) const
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

bool te::pgis::DataSet::isEmpty() const
{
  return (m_size == 0);
}

std::size_t te::pgis::DataSet::size() const
{
  return m_size;
}

bool te::pgis::DataSet::moveNext()
{
  ++m_i;
  return (m_i < m_size);
}

bool te::pgis::DataSet::movePrevious()
{
  --m_i;
  return (m_i > -1);
}

bool te::pgis::DataSet::moveFirst()
{
  m_i = 0;
  return m_size != 0;
}

bool te::pgis::DataSet::moveLast()
{
  m_i = m_size - 1;
  return (m_i < m_size);
}

bool te::pgis::DataSet::moveBeforeFirst()
{
  m_i = -1;
  return m_size != 0;;
}

bool te::pgis::DataSet::move(std::size_t i)
{
  m_i = static_cast<int>(i);
  return (m_i < m_size);
}

bool te::pgis::DataSet::isAtBegin() const
{
  return m_i == 0;
}

bool te::pgis::DataSet::isBeforeBegin() const
{
  return m_i < 0;
}

bool te::pgis::DataSet::isAtEnd() const
{
  return m_i == (m_size - 1);
}

bool te::pgis::DataSet::isAfterEnd() const
{
  return m_i >= m_size;
}

char te::pgis::DataSet::getChar(std::size_t i) const
{
  char cval = *(PQgetvalue(m_result, m_i, i));
  return cval;
}

unsigned char te::pgis::DataSet::getUChar(std::size_t i) const
{
  unsigned char cval = *(PQgetvalue(m_result, m_i, i));
  return cval;
}

boost::int16_t te::pgis::DataSet::getInt16(std::size_t i) const
{
  short int ival = *((short int*)(PQgetvalue(m_result, m_i, i)));

#if TE_MACHINE_BYTE_ORDER == TE_NDR
  te::common::SwapBytes(ival);
#endif

  return ival;
}

boost::int32_t te::pgis::DataSet::getInt32(std::size_t i) const
{
  int ival = *((int*)(PQgetvalue(m_result, m_i, i)));

#if TE_MACHINE_BYTE_ORDER == TE_NDR
  te::common::SwapBytes(ival);
#endif

  return ival;
}

boost::int64_t te::pgis::DataSet::getInt64(std::size_t i) const
{
  long long int ival = *((long long int*)(PQgetvalue(m_result, m_i, i)));

#if TE_MACHINE_BYTE_ORDER == TE_NDR
  te::common::SwapBytes(ival);
#endif

  return ival;
}

bool te::pgis::DataSet::getBool(std::size_t i) const
{
  char bval = *(PQgetvalue(m_result, m_i, i));
  return bval != 0;
}

float te::pgis::DataSet::getFloat(std::size_t i) const
{
  float fval = *((float*)(PQgetvalue(m_result, m_i, i)));

#if TE_MACHINE_BYTE_ORDER == TE_NDR
  te::common::SwapBytes(fval);
#endif

  return fval;
}

double te::pgis::DataSet::getDouble(std::size_t i) const
{
  double dval = *((double*)(PQgetvalue(m_result, m_i, i)));

#if TE_MACHINE_BYTE_ORDER == TE_NDR
  te::common::SwapBytes(dval);
#endif

  return dval;
}

std::string te::pgis::DataSet::getNumeric(std::size_t i) const
{
  char* val = PQgetvalue(m_result, m_i, i);

// get number of groups of 2 bytes used to represent the numeric number
// each 2 bytes represents 4 digits (ex: 2345678.87654 needs 4 groups. 2 to integer and 2 to decimals)
  unsigned short totalGroups; // total number of groups

  totalGroups = *((unsigned short*)val);

#if TE_MACHINE_BYTE_ORDER == TE_NDR
  te::common::SwapBytes(totalGroups);
#endif

  if(totalGroups == 0)
    return "0";

// get number of groups of 2 bytes used to represent the decimal part of the number
  unsigned short decimalGroups; // total number of groups

  decimalGroups = *((unsigned short*)(val+2));

#if TE_MACHINE_BYTE_ORDER == TE_NDR
  te::common::SwapBytes(decimalGroups);
#endif

  ++decimalGroups;

  decimalGroups = totalGroups - decimalGroups;

// get sign of the number
  unsigned short sign; // sign of the number (positive: sign = 0, negative: sign = 0x4000)

  sign = *((unsigned short*)(val+4));

#if TE_MACHINE_BYTE_ORDER == TE_NDR
    te::common::SwapBytes(sign);
#endif

  if(!(sign == 0 || sign == 0x4000))
    return "";

  std::string intVal;

  if(sign != 0)
    intVal = "-";

//get integer part
  unsigned short n;

  short ii = 0;

  while(ii < ((totalGroups - decimalGroups) * 2))
  {
    n = *((unsigned short*)(val + ii + 8));

#if TE_MACHINE_BYTE_ORDER == TE_NDR
    te::common::SwapBytes(n);
#endif

    intVal += te::common::Convert2String(n);

    ii += 2;
  }

  if(ii == 0)
    intVal += "0";

//get decimal part
  std::string decVal;

  if(decimalGroups == 0)
    decVal = "0";
  else
  {
    while(ii < (totalGroups * 2))
    {
      n = *((unsigned short*)(val + ii + 8));

#if TE_MACHINE_BYTE_ORDER == TE_NDR
      te::common::SwapBytes(n);
#endif

      decVal += te::common::Convert2String(n);

      while (decVal.length() < 4) //fill with zeros (ex: .0700 was "700")
        decVal = "0" + decVal;

      ii += 2;
    }

    while((decVal.empty() == false) && (decVal[decVal.length() - 1] == '0'))
      decVal.erase(decVal.length() - 1);
  }

  return intVal + "." + decVal;
}

std::string te::pgis::DataSet::getString(std::size_t i) const
{
  std::string value(PQgetvalue(m_result, m_i, i));
  return value;
}

te::dt::ByteArray* te::pgis::DataSet::getByteArray(std::size_t i) const
{
  int size = PQgetlength(m_result, m_i, i);

  te::dt::ByteArray* b = new te::dt::ByteArray(size);
  b->copy(PQgetvalue(m_result, m_i, i), size);
  return b;
}

te::gm::Geometry* te::pgis::DataSet::getGeometry(std::size_t i) const
{
  return EWKBReader::read(PQgetvalue(m_result, m_i, i));
}

te::rst::Raster* te::pgis::DataSet::getRaster(std::size_t /*i*/) const
{
  return 0;
}

te::dt::DateTime* te::pgis::DataSet::getDateTime(std::size_t i) const
{
  Oid tid = PQftype(m_result, i);
  boost::int64_t ival = 0;
  int iz;
  double dval;
  long int lval;

  switch(tid)
  {
    case PG_TIME_TYPE:
      {
        if(m_t->getPGDataSource()->m_timeIsInteger)
        {
          ival = getInt64(i);
        }
        else
        {
          dval = getDouble(i);
          ival = (boost::int64_t)(dval * 1000000);
        }

        return Internal2Time(ival);
      }

    case PG_DATE_TYPE:
      {
        char* val = PQgetvalue(m_result, m_i, i);
        lval = *((long int*)val);
    
#if TE_MACHINE_BYTE_ORDER == TE_NDR
        te::common::SwapBytes(lval);
#endif

        return Internal2Date(lval);
      }

    case PG_TIMESTAMP_TYPE:
      {
        if(m_t->getPGDataSource()->m_timeIsInteger)
          ival = getInt64(i);
        else
        {
          dval = getDouble(i);
          ival = (boost::int64_t)(dval * 1000000);      
        }
    
        return Internal2TimeStamp(ival);    
      }

    case PG_TIMESTAMPTZ_TYPE:
      {
        if(m_t->getPGDataSource()->m_timeIsInteger)
          ival = getInt64(i);
        else
        {
          char* c = (char*)PQgetvalue(m_result, m_i, i);
          dval = *((double*)c);

          #if TE_MACHINE_BYTE_ORDER == TE_NDR
            te::common::SwapBytes(dval);
          #endif

          ival = (boost::int64_t)(dval * 1000000);
        }
        return Internal2TimeStamp(ival);    
      }
    case PG_TIMETZ_TYPE:
      {
        if(m_t->getPGDataSource()->m_timeIsInteger)
        {
          ival = getInt64(i);
          iz = 0;
        }
        else
        {
          char* c = (char*)PQgetvalue(m_result, m_i, i);
          dval = *((double*)c);
          c += 8;
          iz = *((int*)c);

          #if TE_MACHINE_BYTE_ORDER == TE_NDR
            te::common::SwapBytes(dval);
            te::common::SwapBytes(iz);
          #endif

          ival = (boost::int64_t)(dval * 1000000);
        }
        iz /= 3600;
        if(iz < -12)
          iz = -12;
        if(iz > 14)
          iz = 14;
        return Internal2TimeTZ(ival, iz);
      }

    default:
      throw Exception(TR_PGIS("This type is not supported by TerraLib!"));
  }
}

te::dt::Array* te::pgis::DataSet::getArray(std::size_t i) const
{
  char* value = PQgetvalue(m_result, m_i, i);

  int ndim = *((int*)value);
  value += sizeof(int);

#if TE_MACHINE_BYTE_ORDER == TE_NDR
  te::common::SwapBytes(ndim);
#endif

  int dataoffset = *((int*)value);
  value += sizeof(int);

#if TE_MACHINE_BYTE_ORDER == TE_NDR
  te::common::SwapBytes(dataoffset);
#endif

  unsigned int elemtype = *((unsigned int*)value);
  value += sizeof(unsigned int);

#if TE_MACHINE_BYTE_ORDER == TE_NDR
  te::common::SwapBytes(elemtype);
#endif

  int* dimensions = (int*)value;
  value += ndim * sizeof(int);

  int* lowerbounds = (int*)value;
  value += ndim * sizeof(int);

  uint32_t* null_bitmap = (unsigned int*)value;

  boost::dynamic_bitset<> mask;

  if(dataoffset != 0)
  {
    int nmasks = (dataoffset + 3) / 4;

    mask.resize(nmasks * 8 * sizeof(uint32_t));

    int pos = 0;

    for(int i = 0; i != nmasks; ++i)
    {
      value += sizeof(uint32_t);

      uint32_t umask = null_bitmap[i];

#if TE_MACHINE_BYTE_ORDER == TE_NDR
      te::common::SwapBytes(umask);
#endif

      for(int j = 0; j != 32; ++j)
      {
        mask[pos] = ((umask >> j) & 1) == 0;
        ++pos;
      }
    }
  }
  else
  {
    value += sizeof(uint32_t);
  }

  switch(elemtype)
  {
    case PG_TEXT_TYPE:
    case PG_VARCHAR_TYPE:
    case PG_NAME_TYPE:
      {
        std::auto_ptr<te::dt::Array> arr(new te::dt::Array(ndim, te::dt::STRING_TYPE));

        std::vector<std::size_t> pos(ndim, 0);

        for(int d = 0; d != ndim; ++d)
        {
          int d_size = dimensions[d];

#if TE_MACHINE_BYTE_ORDER == TE_NDR
          te::common::SwapBytes(d_size);
#endif
          int d_lower_boundary = lowerbounds[d];

#if TE_MACHINE_BYTE_ORDER == TE_NDR
          te::common::SwapBytes(d_lower_boundary);
#endif

          for(i = 0; i != d_size; ++i)
          {
            if((dataoffset != 0) && (mask[i] == false))
            {
              arr->insert(0, pos);
              continue;
            }

            pos[d] = i;

            int text_size = *(int*)value;

#if TE_MACHINE_BYTE_ORDER == TE_NDR
            te::common::SwapBytes(text_size);
#endif
            value += sizeof(int);

            arr->insert(new te::dt::String(value), pos);

            value += text_size;
          }
        }

        return arr.release();
      }
    break;

    case PG_FLOAT8_TYPE:
      {
        std::auto_ptr<te::dt::Array> arr(new te::dt::Array(ndim, te::dt::DOUBLE_TYPE));

        std::vector<std::size_t> pos(ndim, 0);

        for(int d = 0; d != ndim; ++d)
        {
          int d_size = dimensions[d];

#if TE_MACHINE_BYTE_ORDER == TE_NDR
          te::common::SwapBytes(d_size);
#endif
          int d_lower_boundary = lowerbounds[d];

#if TE_MACHINE_BYTE_ORDER == TE_NDR
          te::common::SwapBytes(d_lower_boundary);
#endif

          for(i = 0; i != d_size; ++i)
          {
            if((dataoffset != 0) && (mask[i] == false))
            {
              arr->insert(0, pos);
              continue;
            }

            pos[d] = i;

            double val = *(double*)value;

#if TE_MACHINE_BYTE_ORDER == TE_NDR
            te::common::SwapBytes(val);
#endif

            arr->insert(new te::dt::Double(val), pos);

            value += sizeof(double);
          }
        }

        return arr.release();
      }
    break;

    case PG_INT2_TYPE:
      {
        std::auto_ptr<te::dt::Array> arr(new te::dt::Array(ndim, te::dt::INT16_TYPE));

        std::vector<std::size_t> pos(ndim, 0);

        for(int d = 0; d != ndim; ++d)
        {
          int d_size = dimensions[d];

#if TE_MACHINE_BYTE_ORDER == TE_NDR
          te::common::SwapBytes(d_size);
#endif
          int d_lower_boundary = lowerbounds[d];

#if TE_MACHINE_BYTE_ORDER == TE_NDR
          te::common::SwapBytes(d_lower_boundary);
#endif

          for(i = 0; i != d_size; ++i)
          {
            if((dataoffset != 0) && (mask[i] == false))
            {
              arr->insert(0, pos);
              continue;
            }

            pos[d] = i;

            boost::uint16_t val = *(boost::uint16_t*)value;

#if TE_MACHINE_BYTE_ORDER == TE_NDR
            te::common::SwapBytes(val);
#endif

            arr->insert(new te::dt::Int16(val), pos);

            value += sizeof(boost::int16_t);
          }
        }

        return arr.release();
      }
    break;

    default:
      throw Exception(TR_PGIS("The array element type is not supported yet!"));
  }

  //    {
  //      te::common::SwapBytes(ndim);
  //      te::common::SwapBytes(dataoffset);
  //      te::common::SwapBytes(elemtype);
  //      te::common::SwapBytes(dimension);
  //      te::common::SwapBytes(lowerbnds);

  //      a.reserve(dimension);

  //      for(int k = 0; k < dimension; ++k)
  //      {
  //        T v = *((T*)value);
  //        te::common::SwapBytes(v);
  //        a.push_back(v);
  //        value += sizeof(ALIGN);
  //      }
  //    }
  //    else
  //    {
  //      a.reserve(dimension);

  //      for(int k = 0; k < dimension; ++k)
  //      {
  //        T v = *((T*)value);
  //        a.push_back(v);
  //        value += sizeof(ALIGN);
  //      }
  //    }
}

bool te::pgis::DataSet::isNull(std::size_t i) const
{
  return PQgetisnull(m_result, m_i, i) == 1;
}

std::string* te::pgis::DataSet::getSQL() const
{
  return m_sql;
}

