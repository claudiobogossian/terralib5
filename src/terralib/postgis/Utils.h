/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/postgis/Utils.h
   
  \brief Utility functions for PostgreSQL.  
*/

#ifndef __TERRALIB_POSTGIS_INTERNAL_UTILS_H
#define __TERRALIB_POSTGIS_INTERNAL_UTILS_H

// TerraLib
#include "../common/ByteSwapUtils.h"
#include "../common/Globals.h"
#include "../common/StringUtils.h"
#include "../dataaccess/Enums.h"
#include "../datatype/ArrayProperty.h"
#include "../datatype/Date.h"
#include "../datatype/DateTimeProperty.h"
#include "../datatype/NumericProperty.h"
#include "../datatype/SimpleProperty.h"
#include "../datatype/StringProperty.h"
#include "../datatype/TimeDuration.h"
#include "../datatype/TimeInstant.h"
#include "../datatype/TimeInstantTZ.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryProperty.h"
#include "../raster/RasterProperty.h"
#include "Config.h"

// STL
#include <cstdlib>
#include <cstring>
#include <vector>

#include <boost/date_time/time_zone_base.hpp>

// Forward declaration for libpq
extern "C"
{
  struct pg_conn;
  typedef struct pg_conn PGconn;

  struct pg_result;
  typedef struct pg_result PGresult;
}

namespace te
{
// Forward declarations
  namespace da
  {
    class DataSet;
    class DataSetType;
  }

  namespace pgis
  {
    /*!
      \brief It returns the geometry names as usual for PostGIS.

      \param t The TerraLib geometry type.

      \return The geometry names as usual for PostGIS.
    */
    const std::string& GetGeometryName(te::gm::GeomType t);

    /*!
      \brief It writes the PostgreSQL column definition to the output string.

      \param s The string to output the column definition.
      \param p The TerraLib propoerty type.

      \return It returns true if the property type is an autonumber.

      \exception Exception It throws an exception if the data type is not mapped to the PostgreSQL type system.
    */
    bool SetColumnDef(std::string& s, const te::dt::Property* p, bool justDataType = false);

    /*!
      \brief It converts the PostgreSQL foreign key modifier to a TerraLib data type.

      <ul>
      <li>a: NO_ACTION</li>
      <li>r: RESTRICT</li>
      <li>c: CASCADE</li>
      <li>n: SET_NULL</li>
      <li>d: SET_DEFAULT</li>
      </ul>

      \param a PostgreSQL foreign key modifier type.

      \retun The equivalent TerraLib foreign key modifier type.
    */
    inline te::da::FKActionType GetAction(char a)
    {
      switch(a)
      {
        case 'a' : return te::da::NO_ACTION;
        case 'r' : return te::da::RESTRICT;
        case 'c' : return te::da::CASCADE;
        case 'n' : return te::da::SET_NULL;
        case 'd' : return te::da::SET_DEFAULT;
      }

      return te::da::NO_ACTION;
    }

    /*!
      \brief It converts the PostgreSQL index string to a TerraLib index type.

      <ul>
      <li>btree: BTreeType</li>
      <li>gist: RTreeType</li>
      <li>gin: BTreeType</li>
      <li>hash: HashType</li>
      </ul>

      \param t PostgreSQL index name (A NULL terminated string).

      \retun The equivalent TerraLib foreign key modifier type.
    */
    inline te::da::IndexType GetIndexType(const char* t)
    {
      if(strcmp(t, "btree") == 0)
      {
        return te::da::B_TREE_TYPE;
      }
      else if(strcmp(t, "gist") == 0)
      {
        return te::da::R_TREE_TYPE;
      }
      else if(strcmp(t, "gin") == 0)
      {
        return te::da::B_TREE_TYPE;
      }
      else //if(strcmp(t, "hash") == 0)
      {
        return te::da::HASH_TYPE;
      }
    }

    /*!
      \brief It converts the pgType to a valid TerraLib data type.

      \param cols An array with column numbers.
      \param dt   The DataSetType to look for the property.

      \return The next.
    */
    inline te::gm::Envelope* GetEnvelope(const char* str)
    {
      std::string enve(str);
      size_t st = enve.find(",");
      if(st != std::string::npos)
        enve.replace(st, 1, ";");
      double dval = atof("1,2");
      if(dval == 1.2) // if decimal separator is ','
      {
        st = enve.find(".");
        while(st != std::string::npos)
        {
          enve.replace(st, 1, ",");
          st = enve.find(".");
        }
      }
      const char* enveStr = enve.c_str();

      te::gm::Envelope* mbr = new te::gm::Envelope;

      enveStr += 4;
      mbr->m_llx = atof(enveStr);

      while(*enveStr != ' ')
        ++enveStr;

      ++enveStr;

      mbr->m_lly = atof(enveStr);

      while(*enveStr != ';')
        ++enveStr;

      ++enveStr;

      mbr->m_urx = atof(enveStr);

      while(*enveStr != ' ')
        ++enveStr;

      ++enveStr;

      mbr->m_ury = atof(enveStr);

      return mbr;
    }

    /*!
      \brief It converts the envelope into a PostGIS BOX3D.

      \param e      The envelope to be converted.
      \param srid   The envelope coordinates SRS.
      \param output The string to output the result.

      \todo Toda chamada de rotinas de conversao de double para string deveria dizer a precisao!
    */
    inline void Convert2PostGIS(const te::gm::Envelope* e, int srid, std::string& output)
    {
      output += "ST_MakeEnvelope(";
      output += te::common::Convert2String(e->m_llx, 15);
      output += ", ";
      output += te::common::Convert2String(e->m_lly, 15);
      output += ", ";
      output += te::common::Convert2String(e->m_urx, 15);
      output += ", ";
      output += te::common::Convert2String(e->m_ury, 15);
      output += ", ";
      output += te::common::Convert2String(srid);
      output += ")";
    }

    /*!
      \brief It converts the geometry into a PostGIS geometry.

      \param conn   The connection to be used with bytearray.
      \param g      The geometry to be converted.
      \param output The string to output the result.
    */
    void Convert2PostGIS(PGconn* conn, const te::gm::Geometry* g, std::string& output);

    /*!
      \brief It escapes a string for use within an SQL command.

      \param conn   The connection to be used with bytearray.
      \param s      The string to be scapaded.
      \param output The string to output the result.
     */
    void ScapeString(PGconn* conn, const std::string& s, std::string& output);

    /*!
      \brief It returns a julian date (in seconds) from a gregorian date.

      It returns a julian date (in seconds) from a gregorian date.
      Internally, postgresql stores julian dates. 
      
      \param y    A year of a gregorian date.  
      \param m    A month of a gregorian date.  
      \param d    A day of a gregorian date.

      \return a julian date in seconds from the informed gregorian date.
    */
    inline int Date2Julian(int y, int m, const int d)
    {
      int julian = 0;
      int century = 0;

      if(m > 2)
      {
        m += 1;
        y += 4800;
      }
      else
      {
        m += 13;
        y += 4799;
      }

      century = y / 100;
      julian = y * 365 - 32167;
      julian += y / 4 - century + century / 4;
      julian += 7834 * m / 256 + d;

      return julian;
    }   

     /*!
      \brief It returns a gregorian date from a julian date (in seconds).

      It returns a gregorian date from a julian date (in seconds).
      
      \param jd     A julian date in seconds (INPUT). 
      \param year   The year of a gregorian date that will be filled by this function (OUTPUT).  
      \param month  The month of a gregorian date that will be filled by this function (OUTPUT). 
      \param day    The day of a gregorian date that will be filled by this function (OUTPUT).           
    */
    inline void Julian2Date(const int jd, int* year, int* month, int* day)
    {
      unsigned int julian = 0;
      unsigned int quad = 0;
      unsigned int extra = 0;
      int y = 0;

      julian = jd;
      julian += 32044;
      quad = julian / 146097;
      extra = (julian - quad * 146097) * 4 + 3;
      julian += 60 + quad * 3 + extra / 146097;
      quad = julian / 1461;
      julian -= quad * 1461;
      y = julian * 4 / 1461;
      julian = ((y != 0) ? ((julian + 305) % 365) : ((julian + 306) % 366)) + 123;
      y += quad * 4;
      *year = y - 4800;
      quad = julian * 2141 / 65536;
      *day = julian - 7834 * quad / 256;
      *month = (quad + 10) % 12 + 1;

      return;
    }
    
    /*!
      \brief It returns a DateTime type from a date loaded by PostgreSQL.

      It returns a DateTime type from a date loaded by PostgreSQL. Internelly,
      PostgreSQL stores julian dates in 4 bytes, so this function has to convert it
      to gregorian date. Besides that, PostgreSQL stores date as seconds before 
      or after midnight 2000-01-01. Therefore, this function has to sum the number
      of seconds (2451545 seconds) between 01-01-01 00:00:00 and 01-01-2000 00:00:00 
      to the date.
      
      \param dDate  A julian date in seconds loaded by PostgreSQL. 
      
      \param return A gregorian date from the input julian date. 
    */
    inline te::dt::DateTime* Internal2Date(const long dDate)
    {      
      int year, month, day;
      //2451545: number of seconds between 01-01-01 00:00:00 and 01-01-2000 00:00:00
      Julian2Date(dDate + 2451545, &year, &month, &day);
      te::dt::DateTime* result = new te::dt::Date(static_cast<unsigned short>(year), static_cast<unsigned short>(month), static_cast<unsigned short>(day));
      return result;
    }

    /*!
      \brief It returns a DateTime type from a time loaded by PostgreSQL.

      It returns a DateTime type from a time loaded by PostgreSQL. Internelly,
      PostgreSQL stores time as seconds. 
      
      \param tval  The amount of seconds loaded by PostgreSQL. 
      
      \param return A time (hour, minute, second) from the input amount of seconds. 
    */
    inline te::dt::DateTime* Internal2Time(boost::int64_t tval)
    {      
      int hour =  (int)(tval / 3600000000UL);
      boost::int64_t timeval = tval - hour * (boost::int64_t)3600000000UL;
      int min = (int)(timeval / 60000000);
      timeval -= min * (boost::int64_t)60000000;
      int sec = (int)(timeval / 1000000);
      timeval -= sec * (boost::int64_t)1000000;
      int fsec = (int) timeval;

      boost::posix_time::time_duration td(hour, min, sec, fsec);
      te::dt::DateTime* result = new te::dt::TimeDuration(td);
      return result; 
    }

    inline te::dt::DateTime* Internal2TimeTZ(boost::int64_t tval, int z)
    {      
      int hour =  (int)(tval / 3600000000UL);
      boost::int64_t timeval = tval - hour * (boost::int64_t)3600000000UL;
      int min = (int)(timeval / 60000000);
      timeval -= min * (boost::int64_t)60000000;
      int sec = (int)(timeval / 1000000);
      timeval -= sec * (boost::int64_t)1000000;
      int fsec = (int) timeval;

      boost::posix_time::time_duration td(hour, min, sec, fsec);
      char buf[10];
      sprintf(buf, "%d", z);
      std::string sz = "OFF";
      if(z >= 0)
        sz += "+";
      sz += buf;
      boost::local_time::time_zone_ptr zp(new boost::local_time::posix_time_zone(sz));
      
      boost::posix_time::ptime pt(boost::gregorian::date(2000, 01, 01), td);
      boost::local_time::local_date_time tz(pt, zp);
      te::dt::TimeInstantTZ* result = new te::dt::TimeInstantTZ(tz);
     
      return result; 
    }

    /*!
      \brief It returns a DateTime type from a timestamp loaded by PostgreSQL.

      It returns a DateTime type from a timestamp loaded by PostgreSQL. Internelly,
      PostgreSQL stores timestamp as seconds before or after midnight 2000-01-01. 
      
      \param ival  The timestamp, in seconds, loaded by PostgreSQL.
      
      \param return A time instant from the input timestamp. 
    */
    inline te::dt::DateTime* Internal2TimeStamp(boost::int64_t ival)
    {
      boost::int64_t dateval;
      boost::int64_t timeval = ival; 

      //1 day has 86400 seconds (* 1000000)
      dateval = timeval / 86400000000LL;
      if(dateval != 0)
        timeval -= dateval * 86400000000LL;  
       
      if (timeval < 0)
      {
          timeval += 86400000000LL;
          dateval -= 1;
      } 

      te::dt::DateTime* aux1 = Internal2Date((long)dateval);
      te::dt::DateTime* aux2 = Internal2Time(timeval);
      te::dt::DateTime* result = new te::dt::TimeInstant(*static_cast<te::dt::Date*>(aux1), *static_cast<te::dt::TimeDuration*>(aux2));
      delete aux1;
      delete aux2;
      return result;
    }


    /*!
      \brief It creates a PropertyType from a PostgreSQL attribute description.

      \param attNum          The column number (a number in the range [1, num-columns]).
      \param attName         The column name.
      \param attType         The column type
      \param attLen          The column size (depends on column type).
      \param attNotNull      If the column is not null.
      \param fmt             Column formats: Numeric(20, 30), VARCHAR(30), CHAR(10).
      \param attHasDefault   True if the column has a default value.
      \param attDefValue     The default value expression if attHasDefValue is true otherwise this has no meaning.
      \param pgisGeomTypeOid The oid of PostGIS geometry type.

      \return The TerraLib equivalent datatype.

      \note The caller of this function will take the ownership of the returned PropertyType.
      \note This method is valid just for types different from Array. For array types, see the method below (Convert2TerraLib) where the dimension info must be available.
    */
    inline te::dt::Property* Convert2TerraLib(unsigned int attNum, const char* attName, unsigned int attType,
                                                  bool attNotNull, const char* fmt,
                                                  bool attHasDefault, const char* attDefValue,
                                                  unsigned int pgisGeomTypeOid,
                                                  unsigned int pgisRasterTypeOid)
    {
      te::dt::Property* p = 0;
      te::dt::SimpleProperty* simpleP = 0;

      std::string* defaultValue = (attHasDefault ? new std::string(attDefValue) : 0);
      std::string name = std::string(attName);

      if(attType == pgisGeomTypeOid)
          return new te::gm::GeometryProperty(name, attNotNull, defaultValue, attNum);
      else if(attType == pgisRasterTypeOid)
        return new te::rst::RasterProperty(name, attNotNull, attNum);

      switch(attType)
      {
        case PG_INT2_TYPE:
          p = new te::dt::SimpleProperty(name, te::dt::INT16_TYPE, attNotNull, defaultValue, attNum);
          break;

        case PG_INT4_TYPE:
          simpleP = new te::dt::SimpleProperty(name, te::dt::INT32_TYPE, attNotNull, defaultValue, attNum);

          if(defaultValue != 0 && defaultValue->find("nextval(") == 0)
            simpleP->setAutoNumber(true);

          p = simpleP;
          break;

        case PG_INT8_TYPE:
          simpleP = new te::dt::SimpleProperty(name, te::dt::INT64_TYPE, attNotNull, defaultValue, attNum);

          if(defaultValue != 0 && defaultValue->find("nextval(") == 0)
            simpleP->setAutoNumber(true);

          p = simpleP;
          break;

        case PG_OID_TYPE:
          p = new te::dt::SimpleProperty(name, te::dt::INT32_TYPE, attNotNull, defaultValue, attNum);
          break;
        
        case PG_FLOAT8_TYPE:
          p = new te::dt::SimpleProperty(name, te::dt::DOUBLE_TYPE, attNotNull, defaultValue, attNum);
          break;

        case PG_NUMERIC_TYPE:
          {
            unsigned int precision = (fmt && (*(fmt + 7) != '\0') ? atoi(fmt + 8) : 0);
            const char* sprecision = fmt ? strstr(fmt,",") : 0;
            unsigned int scale = (sprecision ? atoi(sprecision + 1) : 0);

            p = new te::dt::NumericProperty(name, precision, scale, attNotNull, defaultValue, attNum);
          }
          break;

        case PG_VARCHAR_TYPE:
          {
            unsigned int size = fmt ? atoi(fmt + 18) : 0;
            
            p = new te::dt::StringProperty(name, te::dt::VAR_STRING, size, attNotNull, defaultValue, attNum);
          }
          break;

        case PG_TEXT_TYPE:
           p = new te::dt::StringProperty(name, te::dt::STRING, 0, attNotNull, defaultValue, attNum);
           break;

        case PG_DATE_TYPE:
          p = new te::dt::DateTimeProperty(name, te::dt::DATE, te::dt::DAY, attNotNull, defaultValue, attNum);
          break;
                  
        case PG_TIME_TYPE:
          p = new te::dt::DateTimeProperty(name, te::dt::TIME_DURATION, te::dt::SECOND, attNotNull, defaultValue, attNum);
          break;
        
        case PG_TIMETZ_TYPE:
        case PG_TIMESTAMPTZ_TYPE:
        case PG_TIMESTAMP_TYPE:
          p = new te::dt::DateTimeProperty(name, te::dt::TIME_INSTANT, te::dt::SECOND, attNotNull, defaultValue, attNum);
          break;

        case PG_BOOL_TYPE:
          p = new te::dt::SimpleProperty(name, te::dt::BOOLEAN_TYPE, attNotNull, defaultValue, attNum);
          break;

        case PG_BYTEA_TYPE:
          p = new te::dt::SimpleProperty(name, te::dt::BYTE_ARRAY_TYPE, attNotNull, defaultValue, attNum);
          break;

        case PG_FLOAT4_TYPE:
          p = new te::dt::SimpleProperty(name, te::dt::FLOAT_TYPE, attNotNull, defaultValue, attNum);
          break;

        case PG_CHARACTER_TYPE:
        case PG_NAME_TYPE:
          {
            unsigned int size = (fmt && (*(fmt + 9) != '\0') ? atoi(fmt + 10) : 0);
            
            p = new te::dt::StringProperty(name, te::dt::FIXED_STRING, size, attNotNull, defaultValue, attNum);
          }
          break;

        case PG_CHAR_TYPE:
          p = new te::dt::SimpleProperty(name, te::dt::CHAR_TYPE, attNotNull, defaultValue, attNum);
        break;

        default:
          p = new te::dt::SimpleProperty(name, te::dt::UNKNOWN_TYPE, attNotNull, defaultValue, attNum);
      }

      return p;
    }

    /*!
      \brief It creates a PropertyType from a PostgreSQL attribute description.

      \param attNum          The column number.
      \param attName         The column name.
      \param attType         The column type.
      \param attNotNull      If the column is not null.
      \param fmt             Column formats: Numeric(20, 30), VARCHAR(30), CHAR(10).
      \param attHasDefault   True, if the column has a default value.
      \param attDefValue     The default value expression, if attHasDefValue is true; otherwise, it has no meaning.
      \param ndims           If different of 0, it indicates an array type.
      \param pgisGeomTypeOid The oid of the PostGIS geometry type.

      \return The TerraLib equivalent PropertyType.
      
      \note The caller of this method will take the ownership of the returned PropertyType.
      \note This method may be used for array types also.
    */
    inline te::dt::Property* Convert2TerraLib(unsigned int attNum, const char* attName, unsigned int attType,
                                                  bool attNotNull, const char* fmt,
                                                  bool attHasDefault, const char* attDefValue,
                                                  int ndims, unsigned int pgisGeomTypeOid,
                                                  unsigned int pgisRasterTypeOid)
    {
      if(ndims == 0)
        return Convert2TerraLib(attNum, attName, attType, attNotNull, fmt,
                                attHasDefault, attDefValue,
                                pgisGeomTypeOid, pgisRasterTypeOid);

      std::string* defaultValue = (attHasDefault ? new std::string(attDefValue) : 0);
      std::string name = std::string(attName);

      te::dt::ArrayProperty* arrayElementProperty = 0;

      for(int i = 0; i < ndims - 1; ++i)
        arrayElementProperty = new te::dt::ArrayProperty(std::string(), arrayElementProperty);

      te::dt::ArrayProperty* at = new te::dt::ArrayProperty(name, arrayElementProperty, attNotNull, defaultValue, attNum);

      switch(attType)
      {
        case PG_BOOL_ARRAY_TYPE:
          {
            te::dt::Property* p = Convert2TerraLib(attNum, attName, PG_BOOL_TYPE, attNotNull, fmt, attHasDefault, attDefValue, pgisGeomTypeOid, pgisRasterTypeOid);
            if(arrayElementProperty)
              arrayElementProperty->setElementType(p);
            else
              at->setElementType(p);
            break;
          }

        case PG_BYTEA_ARRAY_TYPE:
          {
            te::dt::Property* p = Convert2TerraLib(attNum, attName, PG_BYTEA_TYPE, attNotNull, fmt, attHasDefault, attDefValue, pgisGeomTypeOid, pgisRasterTypeOid);
            if(arrayElementProperty)
              arrayElementProperty->setElementType(p);
            else
              at->setElementType(p);
            break;
          }

        case PG_CHAR_ARRAY_TYPE:
          {
            te::dt::Property* p = Convert2TerraLib(attNum, attName, PG_CHAR_TYPE, attNotNull, fmt, attHasDefault, attDefValue, pgisGeomTypeOid, pgisRasterTypeOid);
            if(arrayElementProperty)
              arrayElementProperty->setElementType(p);
            else
              at->setElementType(p);
            break;
          }

        case PG_INT8_ARRAY_TYPE:
          {
            te::dt::Property* p = Convert2TerraLib(attNum, attName, PG_INT8_TYPE, attNotNull, fmt, attHasDefault, attDefValue, pgisGeomTypeOid, pgisRasterTypeOid);
            if(arrayElementProperty)
              arrayElementProperty->setElementType(p);
            else
              at->setElementType(p);
            break;
          }

        case PG_INT2_ARRAY_TYPE:
        case PG_INT2_VECTOR_TYPE:
        case PG__INT2_VECTOR_TYPE:
          {
            te::dt::Property* p = Convert2TerraLib(attNum, attName, PG_INT2_TYPE, attNotNull, fmt, attHasDefault, attDefValue, pgisGeomTypeOid, pgisRasterTypeOid);
            if(arrayElementProperty)
              arrayElementProperty->setElementType(p);
            else
              at->setElementType(p);
            break;
          }

        case PG_INT4_ARRAY_TYPE:
        case PG_OID_ARRAY_TYPE:
        case PG_OID_VECTOR_TYPE:
          {
            te::dt::Property* p = Convert2TerraLib(attNum, attName, PG_INT4_TYPE, attNotNull, fmt, attHasDefault, attDefValue, pgisGeomTypeOid, pgisRasterTypeOid);
            if(arrayElementProperty)
              arrayElementProperty->setElementType(p);
            else
              at->setElementType(p);
            break;
          }

        case PG_TEXT_ARRAY_TYPE:
          {
            te::dt::Property* p = Convert2TerraLib(attNum, attName, PG_TEXT_TYPE, attNotNull, fmt, attHasDefault, attDefValue, pgisGeomTypeOid, pgisRasterTypeOid);
            if(arrayElementProperty)
              arrayElementProperty->setElementType(p);
            else
              at->setElementType(p);
            break;
          }

        case PG_FLOAT4_ARRAY_TYPE:
          {
            te::dt::Property* p = Convert2TerraLib(attNum, attName, PG_FLOAT4_TYPE, attNotNull, fmt, attHasDefault, attDefValue, pgisGeomTypeOid, pgisRasterTypeOid);
            if(arrayElementProperty)
              arrayElementProperty->setElementType(p);
            else
              at->setElementType(p);
            break;
          }

        case PG_FLOAT8_ARRAY_TYPE:
          {
            te::dt::Property* p = Convert2TerraLib(attNum, attName, PG_FLOAT8_TYPE, attNotNull, fmt, attHasDefault, attDefValue, pgisGeomTypeOid, pgisRasterTypeOid);
            if(arrayElementProperty)
              arrayElementProperty->setElementType(p);
            else
              at->setElementType(p);
            break;
          }

        case PG_CHARACTER_ARRAY_TYPE:
        case PG_NAME_ARRAY_TYPE:
          {
            te::dt::Property* p = Convert2TerraLib(attNum, attName, PG_CHARACTER_TYPE, attNotNull, fmt, attHasDefault, attDefValue, pgisGeomTypeOid, pgisRasterTypeOid);
            if(arrayElementProperty)
              arrayElementProperty->setElementType(p);
            else
              at->setElementType(p);
            break;
          }

        case PG_VARCHAR_ARRAY_TYPE:
          {
            te::dt::Property* p = Convert2TerraLib(attNum, attName, PG_VARCHAR_TYPE, attNotNull, fmt, attHasDefault, attDefValue, pgisGeomTypeOid, pgisRasterTypeOid);
            if(arrayElementProperty)
              arrayElementProperty->setElementType(p);
            else
              at->setElementType(p);
            break;
          }

        case PG_DATE_ARRAY_TYPE:
          {
            te::dt::Property* p = Convert2TerraLib(attNum, attName, PG_DATE_TYPE, attNotNull, fmt, attHasDefault, attDefValue, pgisGeomTypeOid, pgisRasterTypeOid);
            if(arrayElementProperty)
              arrayElementProperty->setElementType(p);
            else
              at->setElementType(p);
            break;
          }

        case PG_TIME_ARRAY_TYPE:
          {
            te::dt::Property* p = Convert2TerraLib(attNum, attName, PG_TIME_TYPE, attNotNull, fmt, attHasDefault, attDefValue, pgisGeomTypeOid, pgisRasterTypeOid);
            if(arrayElementProperty)
              arrayElementProperty->setElementType(p);
             else
              at->setElementType(p);
           break;
          }

        case PG_TIMETZ_ARRAY_TYPE:
          {
            te::dt::Property* p = Convert2TerraLib(attNum, attName, PG_TIMETZ_TYPE, attNotNull, fmt, attHasDefault, attDefValue, pgisGeomTypeOid, pgisRasterTypeOid);
            if(arrayElementProperty)
              arrayElementProperty->setElementType(p);
             else
              at->setElementType(p);
           break;
          }

        case PG_TIMESTAMP_ARRAY_TYPE:
          {
            te::dt::Property* p = Convert2TerraLib(attNum, attName, PG_TIMESTAMP_TYPE, attNotNull, fmt, attHasDefault, attDefValue, pgisGeomTypeOid, pgisRasterTypeOid);
            if(arrayElementProperty)
              arrayElementProperty->setElementType(p);
            else
              at->setElementType(p);
            break;
          }

        case PG_TIMESTAMPTZ_ARRAY_TYPE:
          {
            te::dt::Property* p = Convert2TerraLib(attNum, attName, PG_TIMESTAMPTZ_TYPE, attNotNull, fmt, attHasDefault, attDefValue, pgisGeomTypeOid, pgisRasterTypeOid);
            if(arrayElementProperty)
              arrayElementProperty->setElementType(p);
            else
              at->setElementType(p);
            break;
          }

        case PG_NUMERIC_ARRAY_TYPE:
          {
            te::dt::Property* p = Convert2TerraLib(attNum, attName, PG_NUMERIC_TYPE, attNotNull, fmt, attHasDefault, attDefValue, pgisGeomTypeOid, pgisRasterTypeOid);
            if(arrayElementProperty)
              arrayElementProperty->setElementType(p);
            else
              at->setElementType(p);
            break;
          }
        
        default:
            return Convert2TerraLib(attNum, attName, attType, attNotNull, fmt, attHasDefault, attDefValue, pgisGeomTypeOid, pgisRasterTypeOid);
        }

      return at;
    }    

    /*!
      \brief It converts the spatial relationship to PostGIS dialect.

      \param rel The spatial relationship.

      \return The PostGIS relationship name.
    */
    inline std::string GetSpatialRelation(te::gm::SpatialRelation rel)
    {
      switch(rel)
      {
        case te::gm::INTERSECTS:
          return "ST_Intersects";

        case te::gm::DISJOINT:
          return "ST_Disjoint";

        case te::gm::TOUCHES:
          return "ST_Touches";

        case te::gm::OVERLAPS:
          return "ST_Overlaps";

        case te::gm::CROSSES:
          return "ST_Crosses";

        case te::gm::WITHIN:
          return "ST_Within";

        case te::gm::CONTAINS:
          return "ST_Contains";

        case te::gm::COVERS:
          return "ST_Covers";

        case te::gm::COVEREDBY:
          return "ST_CoveredBy";

        case te::gm::EQUALS:
          return "ST_Equals";

        default:
          return "";
      }
    }

    /*!
      \brief It converts the spatial relationship to PostGIS dialect.

      \param rel The spatial relationship.

      \return The PostGIS relationship name.
    */
    inline std::string GetBoxSpatialRelation(te::gm::SpatialRelation rel)
    {
      switch(rel)
      {
        case te::gm::INTERSECTS:
          return " && ";

        case te::gm::OVERLAPS:
          return " && ";

        case te::gm::WITHIN:
          return " @ ";

        case te::gm::CONTAINS:
          return " ~ ";

        case te::gm::COVERS:
          return " ~ ";

        case te::gm::COVEREDBY:
          return " @ ";

        case te::gm::EQUALS:
          return " = ";

        default:
          return "";
      }
    }

    /*!
      \brief It creates a data set type from a PostGIS internal result.

      \param result          A PostGIS internal result.
      \param pgisGeomTypeOid The oid of PostGIS geometry type.

      \return The data set type created.
    */
    void Convert2TerraLib(PGresult* result,
                          unsigned int pgisGeomTypeOid,
                          unsigned int pgisRasterTypeOid,
                          std::vector<int>& teTypes);

    /*!
      \brief It converts the PostGIS geometry type to a pure OGC WKB code.

      This function will turn off the PostGIS flags and will turn on
      the right bits for OGC WKB type.

      \param gType A value that can have some flags set by PostGIS.
    */
    inline void Convert2OGCWKBType(unsigned int& gType)
    {
      if(gType & 0xF0000000)  // Is PostGIS?
      {
        if((gType & TE_EWKB_ZM_OFFSET) == 0xC0000000) // z and m is on
        {
          gType = (gType & 0x0FFFFFFF) | 0xBB8;
        }
        else if(gType & TE_EWKB_Z_OFFSET)
        {
          gType = (gType & 0x0FFFFFFF) | 0x3E8;
        }
        else if(gType & TE_EWKB_M_OFFSET)
        {
          gType = (gType & 0x0FFFFFFF) | 0x7D0;
        }   
        else
        {
          gType = (gType & 0x0FFFFFFF);
        }
      }
    }

    std::string MakeConnectionStr(const std::map<std::string, std::string>& dsInfo);    

    void SplitTableName(const std::string& fullName,
                        const std::string* defaultSchema,
                        std::string& schemaName,
                        std::string& tableName);

    inline void Convert2PostGISWKBType(unsigned int& gType)
    {
      if((gType & 0xF00) == 0xB00)    // it is zm
      {
        gType -= 3000;
        gType |= TE_EWKB_ZM_OFFSET;
      }
      else if((gType & 0x0F00) == 0x300)
      {
        gType -= 1000;
        gType |= TE_EWKB_Z_OFFSET;
      }
      else if((gType & 0xF00) == 0x700)
      {
        gType -= 2000;
        gType |= TE_EWKB_M_OFFSET;
      }
    }

    /*!
      \brief Given a list of properties it creates an AND connected expression with bindable parameters ($n).

      \param itBegin The start property.
      \param itEnd   The mark of end of properties.

      \return An AND connected expression with bindable parameters ($n) that can be used in WHERE expressions.
    */
    std::string GetBindableWhereSQL(const std::vector<te::dt::Property*>& properties, const std::size_t offset = 0);

    std::string GetSQLBindValues(std::size_t nproperties);

    /*!
      \brief Given a list of properties it constructs a string with bindable parameters that can be used inside an update query.

      \param properties A vector with property names that will be used to create the expression.

      \return A comma (',') connected expression with bindable parameters ($1) that can be used in an UPDATE query.
    */
    std::string GetBindableUpdateSQL(const std::vector<te::dt::Property*>& properties);

    std::string GetSQLValues(const te::da::DataSetType* dt, te::da::DataSet* d, PGconn *conn);

    std::string GetSQLValue(const te::dt::Property* p, std::size_t propertyPos, te::da::DataSet* d, PGconn *conn);

    std::string GetLoadDataRow(const te::da::DataSetType* dt, te::da::DataSet* d, PGconn *conn);

    const char* GetPGEncoding(te::common::CharEncoding te_encoding);

    te::common::CharEncoding GetTeEncoding(const char* const pg_encoding);

  } // end namespace pgis
}   // end namespace te

#endif  // __TERRALIB_POSTGIS_INTERNAL_UTILS_H
