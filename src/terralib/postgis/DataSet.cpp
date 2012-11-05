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
#include "../memory/DataSetItem.h"
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
    m_dt(0),
    m_t(transactor),
    m_name(0),
    m_sql(sql),
    m_geomFilter(0),
    m_mbrFilter(0),
    m_relation(te::gm::UNKNOWN_SPATIAL_RELATION)
{
  m_size = PQntuples(m_result);
  m_ncols = PQnfields(m_result);
}

te::pgis::DataSet::DataSet(PGresult* result,
                           Transactor* transactor,
                           std::string* sql,
                           std::string* name)
  : m_i(-1),
    m_result(result),
    m_dt(0),
    m_t(transactor),
    m_name(name),
    m_sql(sql),
    m_geomFilter(0),
    m_mbrFilter(0),
    m_relation(te::gm::UNKNOWN_SPATIAL_RELATION)
{
  m_size = PQntuples(m_result);
  m_ncols = PQnfields(m_result);
}

te::pgis::DataSet::~DataSet()
{
  PQclear(m_result);
  delete m_dt;
  delete m_sql;
  delete m_geomFilter;
  delete m_mbrFilter;
}

te::da::DataSetType* te::pgis::DataSet::getType()
{
  if(m_dt)
    return m_dt;

  m_dt = Convert2TerraLib(m_result,  m_t->getPGDataSource()->getGeomTypeId());

  if(m_name)
    m_dt->setName(*m_name);

  if(m_ncols > 0)
  {
    unsigned int tid = PQftable(m_result, 0);
    m_dt->setId(tid);
  }

  return m_dt;
}

const te::da::DataSetType* te::pgis::DataSet::getType() const
{
  if(m_dt)
    return m_dt;

  m_dt = Convert2TerraLib(m_result,  m_t->getPGDataSource()->getGeomTypeId());

  if(m_name)
    m_dt->setName(*m_name);

  if(m_ncols > 0)
  {
    unsigned int tid = PQftable(m_result, 0);
    m_dt->setId(tid);
  }

  return m_dt;
}

te::da::DataSourceTransactor* te::pgis::DataSet::getTransactor() const
{
  return m_t;
}

void te::pgis::DataSet::loadTypeInfo()
{
  if(m_ncols == 0)
    return;

  if(m_dt == 0)
    getType();

  if(m_dt->getName().empty())
  {
// try to find a table name
    unsigned int tid = PQftable(m_result, 0);

    for(int i = 1; i < m_ncols; ++i)
    {
      unsigned int ttid = PQftable(m_result, i);

// if dataset has more than one table (a join) we can not load more info
      if(tid != ttid)
        return;
    }

    std::auto_ptr<CatalogLoader> cloader(m_t->getPGCatalogLoader());

    std::string tname = cloader->getTableName(tid);

    m_dt->setName(tname);
  }

  std::auto_ptr<CatalogLoader> cloader(m_t->getPGCatalogLoader());

// let's try to find more information about DataSetType
  for(int i = 0; i < m_ncols; ++i)
  {
    te::dt::Property* p = m_dt->getProperty(i);
    te::dt::Property* newp = cloader->getProperty(m_dt->getId(), p->getId());
    m_dt->replace(p, newp);
    delete p;
  }

// find PK, UK, CC ...
  cloader->getConstraints(m_dt);
  m_dt->setFullLoaded(true);
}

te::da::DataSet* te::pgis::DataSet::getParent() const
{
  return 0;
}

te::gm::Envelope* te::pgis::DataSet::getExtent(const te::dt::Property* p)
{
  if(p == 0)
    throw Exception(TR_PGIS("The property is missing!"));

  if(p->getType() != te::dt::GEOMETRY_TYPE)
    throw Exception(TR_PGIS("Not a geometry column to calculate the MBR!"));

  if(m_sql == 0)
  {
    std::size_t ii = getType()->getPropertyPosition(p);

    std::auto_ptr<te::gm::Envelope> mbr;

    for(int i = 0; i < m_size; ++i)
    {
      std::auto_ptr<te::gm::Geometry> g(EWKBReader::read(PQgetvalue(m_result, i, ii)));

      const te::gm::Envelope* box = g->getMBR();

      mbr->Union(*box);
    }

    return mbr.release();
  }

  const te::gm::GeometryProperty* gp = static_cast<const te::gm::GeometryProperty*>(p);

  std::string sql("SELECT ST_Extent(");
              sql += gp->getName();
              sql += ") FROM (";

  if(m_geomFilter)
    getFilterSQL(gp, m_geomFilter, m_relation, sql);
  else if(m_mbrFilter)
    getFilterSQL(gp, m_mbrFilter, m_relation, sql);
  else
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

void te::pgis::DataSet::setFilter(te::dt::Property* p, const te::gm::Geometry* g, te::gm::SpatialRelation r)
{
  if(g == 0)
    throw Exception(TR_PGIS("The geometry is missing!"));

  if(p->getType() != te::dt::GEOMETRY_TYPE)
    throw Exception(TR_PGIS("The property is not geometric!"));

  if(m_sql == 0)
    throw Exception(TR_PGIS("Can not set filter over this type of query. Probably it is a result of a prepared query!"));

  te::gm::GeometryProperty* gp = static_cast<te::gm::GeometryProperty*>(p);

// see if it is possible to retrieve the geometry property srid... let's try this otherwise we go ahead!
  if(gp->getSRID() <= 0)
  {
    std::auto_ptr<CatalogLoader> cloader(m_t->getPGCatalogLoader());
    cloader->getGeometryInfo(gp->getParent()->getName(), gp);
  }

  std::string sql;
  
  getFilterSQL(gp, g, r, sql);

  PQclear(m_result);
  m_result = 0;
  m_i = -1;
  m_size = 0;

  m_result = m_t->getConnection()->query(sql);
  m_size = PQntuples(m_result);  
  delete m_geomFilter;
  m_geomFilter = static_cast<te::gm::Geometry*>(g->clone());
  delete m_mbrFilter;
  m_mbrFilter = 0;
  m_relation = r;
}

void te::pgis::DataSet::setFilter(te::dt::Property* p, const te::gm::Envelope* e, te::gm::SpatialRelation r)
{
  if(e == 0)
    throw Exception(TR_PGIS("The envelope is missing!"));

  if(p->getType() != te::dt::GEOMETRY_TYPE)
    throw Exception(TR_PGIS("The property is not geometric!"));

  if(p->getParent() == 0)
    throw Exception(TR_PGIS("The property is not associated to a dataset type!"));

  if(m_sql == 0)
    throw Exception(TR_PGIS("Can not set filter over this type of query. Probably it is a result of a prepared query!"));

  te::gm::GeometryProperty* gp = static_cast<te::gm::GeometryProperty*>(p);

// see if it is possible to retrieve the geometry property srid... let's try this otherwise we go ahead!
  if(gp->getSRID() <= 0)
  {
    std::auto_ptr<CatalogLoader> cloader(m_t->getPGCatalogLoader());
    cloader->getGeometryInfo(gp->getParent()->getName(), gp);
  }

  std::string sql;

  getFilterSQL(gp, e, r, sql);

  PQclear(m_result);
  m_result = 0;
  m_i = -1;
  m_size = 0;

  m_result = m_t->getConnection()->query(sql);
  m_size = PQntuples(m_result);  
  delete m_geomFilter;
  m_geomFilter = 0;
  delete m_mbrFilter;
  m_mbrFilter = new te::gm::Envelope(*e);
  m_relation = r;
}

te::da::DataSetItem* te::pgis::DataSet::getItem() const
{
  return new te::mem::DataSetItem(this);
}

void te::pgis::DataSet::add(te::da::DataSetItem* /*item*/)
{
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

bool te::pgis::DataSet::moveBeforeFirst()
{
  m_i = -1;
  return m_size != 0;;
}

bool te::pgis::DataSet::moveLast()
{
  m_i = m_size - 1;
  return (m_i < m_size);
}

bool te::pgis::DataSet::moveAfterLast()
{
  m_i = m_size;
  return m_size != 0;
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

char te::pgis::DataSet::getChar(int i) const
{
  char cval = *(PQgetvalue(m_result, m_i, i));
  return cval;
}

char te::pgis::DataSet::getChar(const std::string& name) const
{
  int i = PQfnumber(m_result, name.c_str());
  return getChar(i);
}

void te::pgis::DataSet::setChar(int /*i*/, char /*value*/) 
 {
   throw Exception(TR_PGIS("Not implemented yet!"));
 }

unsigned char te::pgis::DataSet::getUChar(int i) const
{
  unsigned char cval = *(PQgetvalue(m_result, m_i, i));
  return cval;
}

unsigned char te::pgis::DataSet::getUChar(const std::string& name) const
{
  int i = PQfnumber(m_result, name.c_str());
  return getUChar(i);
}

void te::pgis::DataSet::setUChar(int /*i*/, unsigned char /*value*/) 
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

boost::int16_t te::pgis::DataSet::getInt16(int i) const
{
  short int ival = *((short int*)(PQgetvalue(m_result, m_i, i)));

#if TE_MACHINE_BYTE_ORDER == TE_NDR
  te::common::SwapBytes(ival);
#endif

  return ival;
}

boost::int16_t te::pgis::DataSet::getInt16(const std::string& name) const
{
  int i = PQfnumber(m_result, name.c_str());
  return getInt16(i);
}

 void te::pgis::DataSet::setInt16(int /*i*/, boost::int16_t /*value*/) 
 {
   throw Exception(TR_PGIS("Not implemented yet!"));
 }

boost::int32_t te::pgis::DataSet::getInt32(int i) const
{
  int ival = *((int*)(PQgetvalue(m_result, m_i, i)));

#if TE_MACHINE_BYTE_ORDER == TE_NDR
  te::common::SwapBytes(ival);
#endif

  return ival;
}

boost::int32_t te::pgis::DataSet::getInt32(const std::string& name) const
{
  int i = PQfnumber(m_result, name.c_str());
  return getInt32(i);
}

void te::pgis::DataSet::setInt32(int /*i*/, boost::int32_t /*value*/) 
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

boost::int64_t te::pgis::DataSet::getInt64(int i) const
{
  long long int ival = *((long long int*)(PQgetvalue(m_result, m_i, i)));

#if TE_MACHINE_BYTE_ORDER == TE_NDR
  te::common::SwapBytes(ival);
#endif

  return ival;
}

boost::int64_t te::pgis::DataSet::getInt64(const std::string& name) const
{
  int i = PQfnumber(m_result, name.c_str());
  return getInt64(i);
}

void te::pgis::DataSet::setInt64(int /*i*/, boost::int64_t /*value*/) 
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

bool te::pgis::DataSet::getBool(int i) const
{
  char bval = *(PQgetvalue(m_result, m_i, i));
  return bval != 0;
}

bool te::pgis::DataSet::getBool(const std::string& name) const
{
  int i = PQfnumber(m_result, name.c_str());
  return getBool(i);
}

void te::pgis::DataSet::setBool(int /*i*/, bool /*value*/) 
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

float te::pgis::DataSet::getFloat(int i) const
{
  float fval = *((float*)(PQgetvalue(m_result, m_i, i)));

#if TE_MACHINE_BYTE_ORDER == TE_NDR
  te::common::SwapBytes(fval);
#endif

  return fval;
}

float te::pgis::DataSet::getFloat(const std::string& name) const
{
  int i = PQfnumber(m_result, name.c_str());
  return getFloat(i);
}

void te::pgis::DataSet::setFloat(int /*i*/, float /*value*/) 
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

double te::pgis::DataSet::getDouble(int i) const
{
  double dval = *((double*)(PQgetvalue(m_result, m_i, i)));

#if TE_MACHINE_BYTE_ORDER == TE_NDR
  te::common::SwapBytes(dval);
#endif

  return dval;
}

double te::pgis::DataSet::getDouble(const std::string& name) const
{
  int i = PQfnumber(m_result, name.c_str());
  return getDouble(i);
}

void te::pgis::DataSet::setDouble(int /*i*/, double /*value*/) 
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::string te::pgis::DataSet::getNumeric(int i) const
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

std::string te::pgis::DataSet::getNumeric(const std::string& name) const
{
  int i = PQfnumber(m_result, name.c_str());
  return getNumeric(i);
}

void te::pgis::DataSet::setNumeric(int /*i*/, const std::string& /*value*/) 
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::string te::pgis::DataSet::getString(int i) const
{
  std::string value(PQgetvalue(m_result, m_i, i));
  return value;
}

std::string te::pgis::DataSet::getString(const std::string& name) const
{
  int i = PQfnumber(m_result, name.c_str());
  std::string value(PQgetvalue(m_result, m_i, i));
  return value;
}

void te::pgis::DataSet::setString(int /*i*/, const std::string& /*value*/) 
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

te::dt::ByteArray* te::pgis::DataSet::getByteArray(int i) const
{
  int size = PQgetlength(m_result, m_i, i);

  te::dt::ByteArray* b = new te::dt::ByteArray(size);
  b->copy(PQgetvalue(m_result, m_i, i), size);
  return b;
}

te::dt::ByteArray* te::pgis::DataSet::getByteArray(const std::string& name) const
{
  int i = PQfnumber(m_result, name.c_str());
  return getByteArray(i);
}

void te::pgis::DataSet::setByteArray(int /*i*/, const te::dt::ByteArray& /*value*/) 
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

te::gm::Geometry* te::pgis::DataSet::getGeometry(int i) const
{
  return EWKBReader::read(PQgetvalue(m_result, m_i, i));
}

te::gm::Geometry* te::pgis::DataSet::getGeometry(const std::string& name) const
{
  int i = PQfnumber(m_result, name.c_str());
  return EWKBReader::read(PQgetvalue(m_result, m_i, i));
}

void te::pgis::DataSet::setGeometry(int /*i*/, const te::gm::Geometry& /*value*/) 
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

te::rst::Raster* te::pgis::DataSet::getRaster(int /*i*/) const
{
  return 0;
}

te::rst::Raster* te::pgis::DataSet::getRaster(const std::string& name) const
{
  int i = PQfnumber(m_result, name.c_str());
  return getRaster(i);
}

void te::pgis::DataSet::setRaster(int /*i*/, const te::rst::Raster& /*value*/) 
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

te::dt::DateTime* te::pgis::DataSet::getDateTime(int i) const
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

te::dt::DateTime* te::pgis::DataSet::getDateTime(const std::string& name) const
{
  int i = PQfnumber(m_result, name.c_str());
  return getDateTime(i);
}

void te::pgis::DataSet::setDateTime(int /*i*/, const te::dt::DateTime& /*value*/)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSet::getArray(int i, std::vector<boost::int16_t>& values) const
{
  GetArray<short int, int>(m_i, i, m_result, values);
}

void te::pgis::DataSet::getArray(const std::string& name, std::vector<boost::int16_t>& values) const
{
  int i = PQfnumber(m_result, name.c_str());
  getArray(i, values);
}

te::dt::Array* te::pgis::DataSet::getArray(int i) const
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

        bool bittttt = mask[pos];

        ++pos;
      }
    }
  }

  switch(elemtype)
  {
    case PG_TEXT_TYPE:
    case PG_VARCHAR_TYPE:
    case PG_NAME_TYPE:
      {
        std::unique_ptr<te::dt::Array> arr(new te::dt::Array(ndim, te::dt::STRING_TYPE));

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
        std::unique_ptr<te::dt::Array> arr(new te::dt::Array(ndim, te::dt::DOUBLE_TYPE));

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

te::dt::Array* te::pgis::DataSet::getArray(const std::string& name) const
{
  int i = PQfnumber(m_result, name.c_str());
  return getArray(i);
}

const unsigned char* te::pgis::DataSet::getWKB(int i) const
{
  return (unsigned char*)PQgetvalue(m_result, m_i, i);
}

const unsigned char* te::pgis::DataSet::getWKB(const std::string& name) const
{
  int i = PQfnumber(m_result, name.c_str());
  return getWKB(i);
}

te::da::DataSet* te::pgis::DataSet::getDataSet(int /*i*/)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

te::da::DataSet* te::pgis::DataSet::getDataSet(const std::string& name)
{
  int i = PQfnumber(m_result, name.c_str());
  return getDataSet(i);
}

void te::pgis::DataSet::setDataSet(int /*i*/, const te::da::DataSet& /*value*/)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSet::setValue(int /*i*/, te::dt::AbstractData* /*ad*/)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}
        
bool te::pgis::DataSet::isNull(int i) const
{
  return PQgetisnull(m_result, m_i, i) == 1;
}

bool te::pgis::DataSet::isNull(const std::string& name) const
{
  int i = PQfnumber(m_result, name.c_str());
  return PQgetisnull(m_result, m_i, i) == 1;
}


void te::pgis::DataSet::getFilterSQL(const te::gm::GeometryProperty* gp, const te::gm::Geometry* g, te::gm::SpatialRelation r, std::string& sql)
{
  const std::string& colName = gp->getName();
  
  std::string rel = GetSpatialRelation(r);

  sql += "SELECT * FROM (";
  sql += *m_sql;
  sql += ") AS pgis_driver_subquery WHERE ";
  sql += rel;
  sql += "(";

  Convert2PostGIS(m_t->getConnection()->getConn(), g, sql);

  sql += ",";
  sql += colName;
  sql += ")";
}

void te::pgis::DataSet::getFilterSQL(const te::gm::GeometryProperty* gp, const te::gm::Envelope* e, te::gm::SpatialRelation r, std::string& sql)
{
  const std::string& colName = gp->getName();
  
  std::string rel = GetBoxSpatialRelation(r);

  sql += "SELECT * FROM (";
  sql += *m_sql;
  sql += ") AS pgis_driver_subquery WHERE ";
  sql += colName;
  sql += rel;

  Convert2PostGIS(e, gp->getSRID(), sql);
}

