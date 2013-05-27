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
  \file terralib/mysql/Utils.cpp

  \brief Utility functions for the MySQL Data Access driver.
*/

// TerraLib
#include "../common/HexUtils.h"
#include "../common/Translator.h"
#include "../common/StringUtils.h"
#include "../dataaccess/dataset/CheckConstraint.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/ForeignKey.h"
#include "../dataaccess/dataset/Index.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../dataaccess/datasource/BatchExecutor.h"
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "../datatype/ByteArray.h"
#include "../datatype/DateTime.h"
#include "../datatype/DateTimeProperty.h"
#include "../datatype/Enums.h"
#include "../datatype/NumericProperty.h"
#include "../datatype/StringProperty.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Utils.h"
#include "../srs/SpatialReferenceSystemManager.h"
#include "Exception.h"
#include "Utils.h"

// STL
#include <cassert>
#include <cctype>
#include <fstream>
#include <memory>
#include <sstream>


// Boost
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

// MySQL
#include <driver/mysql_public_iface.h>

void te::mysql::GetFilter(const std::string& colName,
                          const te::gm::Envelope* e,
                          const te::gm::SpatialRelation r,
                          std::string& sql)
{
  assert(e != 0);

  switch(r)
  {
    case te::gm::INTERSECTS:
      sql += "MBRIntersects(";
    break;

    case te::gm::DISJOINT:
      sql += "MBRDisjoint(";
    break;

    case te::gm::TOUCHES:
      sql += "MBRTouches(";
    break;

    case te::gm::OVERLAPS:
      sql += "MBROverlaps(";
    break;

    case te::gm::WITHIN:
      sql += "MBRWithin(";
    break;

    case te::gm::CONTAINS:
      sql += "MBRContains(";
    break;

    case te::gm::EQUALS:
      sql += "MBREqual(";
    break;

    default:
      throw Exception(TR_MYSQL("This spatial relation is not supported for MBR Filter creation!"));
  }

  sql += colName;
  sql += ", ";

  std::auto_ptr<te::gm::Geometry> poly(te::gm::GetGeomFromEnvelope(e, 0));

  Convert2MySQLGeometry(poly.get(), sql);

  sql += ")";
}

void te::mysql::GetFilter(const std::string& colName,
                          const te::gm::Geometry* g,
                          const te::gm::SpatialRelation r,
                          std::string& sql
                         )
{
  const te::gm::Envelope* e = g->getMBR();

  switch(r)
  {
    case te::gm::INTERSECTS:
      GetFilter(colName, e, te::gm::INTERSECTS, sql);
      sql += " AND Intersects(";
    break;

    case te::gm::DISJOINT:
      GetFilter(colName, e, te::gm::DISJOINT, sql);
      sql += " AND Disjoint(";
    break;

    case te::gm::TOUCHES:
      GetFilter(colName, e, te::gm::INTERSECTS, sql);
      sql += " AND Touches(";
    break;

    case te::gm::OVERLAPS:
      GetFilter(colName, e, te::gm::INTERSECTS, sql);
      sql += " AND Overlaps(";
    break;

    case te::gm::WITHIN:
      GetFilter(colName, e, te::gm::WITHIN, sql);
      sql += " AND Within(";
    break;

    case te::gm::CONTAINS:
      GetFilter(colName, e, te::gm::CONTAINS, sql);
      sql += " AND Contains(";
    break;

    case te::gm::EQUALS:
      GetFilter(colName, e, te::gm::EQUALS, sql);
      sql += " AND Equals(";
    break;

    case te::gm::CROSSES:
      GetFilter(colName, e, te::gm::INTERSECTS, sql);
      sql += " AND Crosses(";
    break;

    default:
      throw Exception(TR_MYSQL("This spatial relation is not supported for Geometry Filter creation!"));
  }

  Convert2MySQLGeometry(g, sql);

  sql += ")";
}

void te::mysql::Convert2MySQLGeometry(const te::gm::Geometry* g, std::string& sql)
{
  sql += "GeomFromWKB(";

  std::size_t wkbSize = g->getWkbSize();

  char* wkb = new char[wkbSize];

  try
  {
    g->getWkb(wkb, te::common::NDR);

    char* hexWkb = new char[2* wkbSize + 3];  // "0x" .. hex-wkb + '\0'

    hexWkb[0] = '0';
    hexWkb[1] = 'x';

    te::common::Binary2Hex(wkb, wkbSize, hexWkb + 2); // already add a trailing '\0'

    sql += hexWkb;

    sql += ")";

    delete [] hexWkb;
    delete [] wkb;
  }
  catch(...)
  {
    delete [] wkb;
    throw;
  }
}

int te::mysql::GetTableCategory(const std::string& tableType)
{
  if(tableType == "TABLE")
    return te::da::TABLE_TYPE;
  else if(tableType == "VIEW")
    return te::da::VIEW_TYPE;
  
  return te::da::UNKNOWN_DATASET_TYPE;
}

te::dt::Property* te::mysql::Convert2TerraLibProperty(const sql::ResultSet* result)
{
  std::string colname = result->getString("COLUMN_NAME");

  std::string coltypename = result->getString("TYPE_NAME");

  bool isUnsigned = coltypename.find("UNSIGNED") != std::string::npos;

  int colid = result->getInt("ORDINAL_POSITION");

  int dt = result->getInt("DATA_TYPE");

  int colsize = result->getInt("COLUMN_SIZE");

  int ddigits = result->getInt("DECIMAL_DIGITS");

  bool nullable = result->getInt("NULLABLE") != 0;

  bool isAutoIncrement = result->getString("IS_AUTOINCREMENT") == "TRUE";

  std::string* defvalue = result->getString("COLUMN_DEF").asStdString().empty() ? 0 : new std::string(result->getString("COLUMN_DEF"));

  te::dt::Property* p = Convert2TerraLibProperty(colname, coltypename, isUnsigned, colid, dt, colsize, ddigits, nullable, isAutoIncrement, defvalue);

  return p;
}

te::dt::Property* te::mysql::Convert2TerraLibProperty(const std::string& colname,
                                                      const std::string& coltypename,
                                                      const bool isUnsigned,
                                                      const int colid,
                                                      const int datatype,
                                                      const int colsize,
                                                      const int ddigits,
                                                      const bool nullable,
                                                      const bool isAutoIncrement,
                                                      std::string* defvalue)
{
  te::dt::Property* p = 0;

  switch(datatype)
  {
    case sql::DataType::BIT :
      p = new te::dt::SimpleProperty(colname, te::dt::BIT_TYPE, !nullable, defvalue, colid, 0);
    break;

    case sql::DataType::TINYINT :
      {
        //if(colsize == 1)
        //{
        //  te::dt::SimpleProperty* sp = new te::dt::SimpleProperty(colname, te::dt::BOOLEAN_TYPE, !nullable, defvalue, colid, 0);
        //  sp->setAutoNumber(isAutoIncrement);
        //  p = sp;
        //}
        //else
        //{
          te::dt::SimpleProperty* sp = new te::dt::SimpleProperty(colname, isUnsigned ? te::dt::UCHAR_TYPE : te::dt::CHAR_TYPE, !nullable, defvalue, colid, 0);
          sp->setAutoNumber(isAutoIncrement);
          p = sp;
        //}        
      }
    break;

    case sql::DataType::SMALLINT :
      {
        te::dt::SimpleProperty* sp = new te::dt::SimpleProperty(colname, isUnsigned ? te::dt::UINT16_TYPE : te::dt::INT16_TYPE, !nullable, defvalue, colid, 0);
        sp->setAutoNumber(isAutoIncrement);
        p = sp;
      }
    break;

    case sql::DataType::MEDIUMINT :
    case sql::DataType::INTEGER :
      {
        te::dt::SimpleProperty* sp = new te::dt::SimpleProperty(colname, isUnsigned ? te::dt::UINT32_TYPE : te::dt::INT32_TYPE, !nullable, defvalue, colid, 0);
        sp->setAutoNumber(isAutoIncrement);
        p = sp;
      }
    break;

    case sql::DataType::BIGINT :
      {
        te::dt::SimpleProperty* sp = new te::dt::SimpleProperty(colname, isUnsigned ? te::dt::UINT64_TYPE : te::dt::INT64_TYPE, !nullable, defvalue, colid, 0);
        sp->setAutoNumber(isAutoIncrement);
        p = sp;
      }
    break;

    case sql::DataType::REAL :
      p = new te::dt::SimpleProperty(colname, te::dt::FLOAT_TYPE, !nullable, defvalue, colid, 0);
    break;

    case sql::DataType::DOUBLE :
        p = new te::dt::SimpleProperty(colname, te::dt::DOUBLE_TYPE, !nullable, defvalue, colid, 0);
    break;

    case sql::DataType::DECIMAL :
    case sql::DataType::NUMERIC :
      p = new te::dt::NumericProperty(colname, colsize, ddigits, !nullable, defvalue, colid, 0);
    break;

    case sql::DataType::CHAR :
      p = new te::dt::StringProperty(colname, te::dt::FIXED_STRING, colsize, !nullable, defvalue, colid, 0);
    break;

    case sql::DataType::VARCHAR :

      if(coltypename == "VARCHAR")
        p = new te::dt::StringProperty(colname, te::dt::VAR_STRING, colsize, !nullable, defvalue, colid, 0);
      else if(coltypename == "TINYTEXT")
        p = new te::dt::StringProperty(colname, te::dt::STRING, colsize, !nullable, defvalue, colid, 0);
      else
        throw Exception(TR_MYSQL("Unknown text/string type for MySQL databases!"));
    break;

    case sql::DataType::LONGVARCHAR :
      p = new te::dt::StringProperty(colname, te::dt::STRING, colsize, !nullable, defvalue, colid, 0);
    break;

    case sql::DataType::VARBINARY :    
    case sql::DataType::LONGVARBINARY :
    case sql::DataType::BINARY :
      p = new te::dt::SimpleProperty(colname, te::dt::BYTE_ARRAY_TYPE, !nullable, defvalue, colid, 0);
    break;

    case sql::DataType::TIMESTAMP:
      p = new te::dt::DateTimeProperty(colname, te::dt::TIME_INSTANT, te::dt::UNKNOWN, !nullable, defvalue, colid, 0);
    break;

    case sql::DataType::DATE:
      p = new te::dt::DateTimeProperty(colname, te::dt::DATE, te::dt::UNKNOWN, !nullable, defvalue, colid, 0);
    break;

    case sql::DataType::TIME:
      p = new te::dt::DateTimeProperty(colname, te::dt::TIME_DURATION, te::dt::UNKNOWN, !nullable, defvalue, colid, 0);
    break;

    case sql::DataType::GEOMETRY:
      p = new te::gm::GeometryProperty(colname, 0, te::gm::GeometryType, !nullable, defvalue, 0, 0);
    break;

    case sql::DataType::YEAR:
    case sql::DataType::ENUM:
    case sql::DataType::SET:
    case sql::DataType::SQLNULL:
    case sql::DataType::UNKNOWN:
    default:
      if(coltypename == "POINT")
        p = new te::gm::GeometryProperty(colname, 0, te::gm::PointType, !nullable, defvalue, 0, 0);
      else if(coltypename == "POLYGON")
        p = new te::gm::GeometryProperty(colname, 0, te::gm::PolygonType, !nullable, defvalue, 0, 0);
      else if(coltypename == "LINESTRING")
        p = new te::gm::GeometryProperty(colname, 0, te::gm::LineStringType, !nullable, defvalue, 0, 0);
      else if(coltypename == "MULTIPOINT")
        p = new te::gm::GeometryProperty(colname, 0, te::gm::MultiPointType, !nullable, defvalue, 0, 0);
      else if(coltypename == "MULTIPOLYGON")
        p = new te::gm::GeometryProperty(colname, 0, te::gm::MultiPolygonType, !nullable, defvalue, 0, 0);
      else if(coltypename == "MULTILINESTRING")
        p = new te::gm::GeometryProperty(colname, 0, te::gm::MultiLineStringType, !nullable, defvalue, 0, 0);
      else if(coltypename == "GEOMETRYCOLLECTION")
        p = new te::gm::GeometryProperty(colname, 0, te::gm::GeometryCollectionType, !nullable, defvalue, 0, 0);
      else
        p = new te::dt::SimpleProperty(colname, te::dt::UNKNOWN_TYPE, !nullable, defvalue, colid, 0);
    break;
  }

  return p;
}

te::da::IndexType te::mysql::Convert2TerraLibIndexType(const std::string& idxType)
{
  if(idxType == "BTREE")
  {
    return te::da::B_TREE_TYPE;
  }
  else if(idxType == "SPATIAL")
  {
    return te::da::R_TREE_TYPE;
  }
  else if(idxType == "HASH")
  {
    return te::da::HASH_TYPE;
  }
  
  throw Exception(TR_MYSQL("Unknown index type for MySQL table!"));
}

te::da::DataSetType* te::mysql::Convert2TerraLibDataSetType(sql::ResultSet* result)
{
  assert(result);

  sql::ResultSetMetaData* md = result->getMetaData();

  unsigned int ncols = md->getColumnCount();

  std::string tblName = (ncols > 0) ? md->getTableName(1) : std::string("");

  std::auto_ptr<te::da::DataSetType> dtype(new te::da::DataSetType(tblName, 0));

  for(unsigned int i = 1; i <= ncols; ++i)
  {
    std::string colname = md->getColumnName(i);

    int dt = md->getColumnType(i);

    int colsize = md->getColumnDisplaySize(i);

    //unsigned int ddigits = md->getPrecision(i); -- it is not working!!!

    int ddigits = md->getScale(i);

    bool nullable = md->isNullable(i) != 0;

    std::string* defvalue = 0;

    bool isAutoincrement = md->isAutoIncrement(i);

    std::string coltypename = md->getColumnTypeName(i);

    bool isUnsigned = coltypename.find("UNSIGNED") != std::string::npos;

    te::dt::Property* p = Convert2TerraLibProperty(colname, coltypename, isUnsigned, i, dt, colsize, ddigits, nullable, isAutoincrement, defvalue);

    dtype->add(p);

    if(p->getType() == te::dt::GEOMETRY_TYPE)
      dtype->setDefaultGeomProperty(static_cast<te::gm::GeometryProperty*>(p));
  }

  return dtype.release();
}

std::string te::mysql::GetURLConnection(const std::map<std::string, std::string>& dsInfo)
{
  std::string url = "tcp://";

  if(dsInfo.count("HOST") > 0)
    url += dsInfo.find("HOST")->second;
  else
    url += "127.0.0.1";

  if(dsInfo.count("PORT") > 0)
    url += ":" + dsInfo.find("PORT")->second;
  else
    url += ":3306";

  return url;
}

std::string te::mysql::Convert2SQLCreate(const te::dt::Property* p)
{
  std::string sql = p->getName();

  sql += " ";

  sql += GetSQLType(p);

  const te::dt::SimpleProperty* sp = static_cast<const te::dt::SimpleProperty*>(p);

  if(sp->isRequired())
    sql += " NOT NULL";

  if(sp->isAutoNumber())
    sql += " AUTO_INCREMENT";

  //if(p->getParent())
  //{
  //  const te::da::DataSetType* dt = static_cast<const te::da::DataSetType*>(p->getParent());

  //  if(dt->getPrimaryKey() && dt->getPrimaryKey()->has(p))
  //  {
  //    sql += " PRIMARY KEY";
  //  }
  //}

  return sql;
}

std::string te::mysql::GetSQLType(const te::dt::Property* p)
{
  std::string sql;

  switch(p->getType())
  {
    case te::dt::CHAR_TYPE :
      sql += "TINYINT";
    break;

    case te::dt::UCHAR_TYPE :
      sql += "TINYINT UNSIGNED";
    break;
      
    case te::dt::INT16_TYPE :
      sql += "SMALLINT";
    break;

    case te::dt::UINT16_TYPE :
      sql += "SMALLINT UNSIGNED";
    break;

    case te::dt::INT32_TYPE :
      sql += "INT";
    break;

    case te::dt::UINT32_TYPE :
      sql += "INT UNSIGNED";
    break;

    case te::dt::INT64_TYPE :
      sql += "BIGINT";
    break;

    case te::dt::UINT64_TYPE :
      sql += "BIGINT UNSIGNED";
    break;

    case te::dt::FLOAT_TYPE :
      sql = "FLOAT";
    break;

    case te::dt::DOUBLE_TYPE :
      sql = "DOUBLE";
    break;
    
    case te::dt::NUMERIC_TYPE :
      {
        const te::dt::NumericProperty* np = static_cast<const te::dt::NumericProperty*>(p);
        
        sql  = "DECIMAL(";
        sql += boost::lexical_cast<std::string>(np->getPrecision());
        sql += ", ";
        sql += boost::lexical_cast<std::string>(np->getScale());
        sql += ")";
      }
    break;

    case te::dt::STRING_TYPE :
      {
        const te::dt::StringProperty* sp = static_cast<const te::dt::StringProperty*>(p);

        if(sp->getSubType() == te::dt::FIXED_STRING)
        {
          sql = "CHAR(";
          sql += boost::lexical_cast<std::string>(sp->size());
          sql += ")";
        }
        else if(sp->getSubType() == te::dt::VAR_STRING)
        {
          sql = "VARCHAR(";
          sql += boost::lexical_cast<std::string>(sp->size());
          sql += ")";
        }
        else
        {
          if((sp->size() == 0) || (sp->size() > 16777215))
            sql += "LONGTEXT";
          else if(sp->size() <= 255)
            sql += "TINYTEXT";
          else if(sp->size() <= 65535)
            sql += "TEXT";
          else //if(sp->size() <= 16777215)
            sql += "MEDIUMTEXT";
        }      
      }
    break;

    case te::dt::BYTE_ARRAY_TYPE :
      sql = "BLOB";
    break;

    case te::dt::GEOMETRY_TYPE :
      {
        const te::gm::GeometryProperty* gp = static_cast<const te::gm::GeometryProperty*>(p);

        sql += Convert2MyGeom(gp->getGeometryType());
      }
    break;

    case te::dt::DATETIME_TYPE :
      {
        const te::dt::DateTimeProperty* dtp = static_cast<const te::dt::DateTimeProperty*>(p);

        if(dtp->getSubType() == te::dt::DATE)
        {
          sql += "DATE";
        }
        else if(dtp->getSubType() == te::dt::TIME_DURATION)
        {
          sql += "TIME";
        }
        else if(dtp->getSubType() == te::dt::TIME_INSTANT)
        {
          sql += "TIMESTAMP";
        }
        else
        {
          throw Exception(TR_MYSQL("This TerraLib date time type doesn't map to MySQL type system!"));
        }
      }
    break;

    default:
      throw Exception(TR_MYSQL("This TerraLib data type can not be converted to MySQL type system!"));
  }

  return sql;
}

std::string te::mysql::Convert2MyGeom(const te::gm::GeomType t)
{
  switch(t)
  {
    case te::gm::PointType :
      return "POINT";

    case te::gm::LineStringType :
      return "LINESTRING";

    case te::gm::PolygonType :
      return "POLYGON";

    case te::gm::MultiPointType :
      return "MULTIPOINT";

    case te::gm::MultiLineStringType :
      return "MULTILINESTRING";

    case te::gm::MultiPolygonType :
      return "MULTIPOLYGON";

    case te::gm::GeometryCollectionType :
      return "GEOMETRYCOLLECTION";

    case te::gm::GeometryType :
    case te::gm::UnknownGeometryType :
      return "GEOMETRY";

    default:
      throw Exception(TR_MYSQL("This geometric type is not supported by MySQL!"));
  }
}

std::string te::mysql::GetSQLValueNames(const te::da::DataSetType* dt)
{
  std::string valueNames("(");

  const std::size_t np = dt->size();

  for(std::size_t i = 0; i < np; ++i)
  {
    if(i != 0)
      valueNames += ",";

    valueNames += dt->getProperty(i)->getName();
  }

  valueNames += ")";

  return valueNames;
}

std::string te::mysql::GetSQLValues(const te::da::DataSetType* dt, te::da::DataSet* d, sql::mysql::MySQL_Connection* conn)
{
  std::string values("(");

  const std::size_t np = dt->size();

  for(std::size_t i = 0; i < np; ++i)
  {
    if(i != 0)
      values += ",";

    values += GetSQLValue(dt->getProperty(i), i, d, conn);
  }

  values += ")";

  return values;
}

std::string te::mysql::GetSQLValue(const te::dt::Property* p, std::size_t propertyPos, te::da::DataSet* d, sql::mysql::MySQL_Connection* conn)
{
  std::string value;

  switch(p->getType())
  {
    case te::dt::CHAR_TYPE :
      value += boost::lexical_cast<std::string>(static_cast<boost::int16_t>(d->getChar(propertyPos)));
    break;

    case te::dt::UCHAR_TYPE :
      value += boost::lexical_cast<std::string>(static_cast<boost::uint16_t>(d->getUChar(propertyPos)));
    break;

    case te::dt::INT16_TYPE :
      value += boost::lexical_cast<std::string>(d->getInt16(propertyPos));
    break;

    case te::dt::INT32_TYPE :
      value += boost::lexical_cast<std::string>(d->getInt32(propertyPos));
    break;

    case te::dt::INT64_TYPE :
      value += boost::lexical_cast<std::string>(d->getInt64(propertyPos));
    break;

    case te::dt::BOOLEAN_TYPE :
      value += d->getBool(propertyPos) ? "1" : "0";
    break;

    case te::dt::FLOAT_TYPE :
      value += boost::lexical_cast<std::string>(d->getFloat(propertyPos));
    break;

    case te::dt::DOUBLE_TYPE :
      value += boost::lexical_cast<std::string>(d->getDouble(propertyPos));
    break;

    case te::dt::NUMERIC_TYPE :
      value += d->getNumeric(propertyPos);
    break;

    case te::dt::STRING_TYPE :
        value += "'";
        value += conn->escapeString(d->getString(propertyPos));
        value += "'";
    break;

    case te::dt::BYTE_ARRAY_TYPE :
      {
        std::auto_ptr<te::dt::ByteArray> ba(d->getByteArray(propertyPos));
        char* hexba = new char[ba->bytesUsed() * 2  + 1];
        te::common::Binary2Hex(ba->getData(), ba->bytesUsed(), hexba);

        value += "0x";
        value += hexba;

        delete [] hexba;
      }
    break;

    case te::dt::GEOMETRY_TYPE :
      {
        std::auto_ptr<te::gm::Geometry> geom(d->getGeometry(propertyPos));

        std::size_t size = 0;

        char* wkb = geom->asBinary(size);

        char* hexwkb = new char[size * 2  + 1];
        te::common::Binary2Hex(wkb, size, hexwkb);

        value += "GeomFromWKB(0x";
        value += hexwkb;
        value += ")";

        delete [] wkb;
        delete [] hexwkb;
      }
    break;

    case te::dt::DATETIME_TYPE :
      {
        std::auto_ptr<te::dt::DateTime> dt(d->getDateTime(propertyPos));
        value += "'";
        value += dt->toString();
        value += "'";
      }
    break;

    default :
      throw Exception(TR_MYSQL("The TerraLib data type is not supported by the MySQL driver!"));
  }

  return value;
}

std::string te::mysql::GetSQLBindValues(const te::da::DataSet* dataset)
{
  std::string valueNames("(");

  const std::size_t np = dataset->getNumProperties();

  for(std::size_t i = 0; i < np; ++i)
  {
    if(i != 0)
      valueNames += ",";

    if(dataset->getPropertyDataType(i) != te::dt::GEOMETRY_TYPE)
      valueNames += "?";
    else
      valueNames += "GeomFromWKB(?)";
  }

  valueNames += ")";

  return valueNames;
}

std::string te::mysql::GetSQLUpdateValues(const te::da::DataSetType* dt, te::da::DataSet* dataset, const std::vector<std::size_t>& propertiesPos, sql::mysql::MySQL_Connection* conn)
{
  std::string sql;

  for(std::size_t i = 0; i < propertiesPos.size(); ++i)
  {
    if(i != 0)
      sql += ", ";

    const te::dt::Property* p = dt->getProperty(propertiesPos[i]);

    sql += p->getName() + " = " + GetSQLValue(p, propertiesPos[i], dataset, conn);
  }

  return sql;
}

std::string te::mysql::GetSQLWhereValues(const te::da::DataSetType* dt, te::da::DataSet* dataset, const std::vector<std::size_t>& propertiesPos, sql::mysql::MySQL_Connection* conn)
{
  std::string sql;

  for(std::size_t i = 0; i < propertiesPos.size(); ++i)
  {
    if(i != 0)
      sql += " AND ";

    const te::dt::Property* p = dt->getProperty(propertiesPos[i]);

    sql += p->getName() + " = " + GetSQLValue(p, propertiesPos[i], dataset, conn);
  }

  return sql;
}


bool te::mysql::IsAllWhitespace(const char* sqlLine)
{
  for(; *sqlLine; ++sqlLine)
  {
    if(isspace(*(unsigned char*)sqlLine))
      continue;

    if((*sqlLine == '/') && (sqlLine[1] == '*'))
    {
      sqlLine += 2;

      while(*sqlLine && ((*sqlLine != '*') || (sqlLine[1] != '/')))
        ++sqlLine;
    
      if(*sqlLine == 0)
        return false;

      ++sqlLine;

      continue;
    }

    if((*sqlLine == '-') && (sqlLine[1] == '-'))
    {
      sqlLine += 2;
      
      while(*sqlLine && (*sqlLine != '\n'))
        ++sqlLine;

      if(*sqlLine == 0)
        return true;

      continue;
    }

    return false;
  }

  return true;
}

std::string te::mysql::GetBindableWhereSQL(const std::vector<te::dt::Property*>& properties)
{
  std::string wheresql;

  const std::size_t size = properties.size();

  for(std::size_t i = 0; i < size; ++i)
  {
    if(i != 0)
      wheresql += " AND ";

    wheresql += properties[i]->getName();

    wheresql += " = ?";
  }

  return wheresql;
}

std::string te::mysql::GetBindableUpdateSQL(const std::vector<te::dt::Property*>& properties)
{
  std::string sql;

  const std::size_t size = properties.size();

  for(std::size_t i = 0; i < size; ++i)
  {
    if(i != 0)
      sql += ", ";

    sql += properties[i]->getName();

    sql += " = ?";
  }

  return sql;
}

std::string te::mysql::GetLoadDataRow(const te::da::DataSetType* dt, te::da::DataSet* d, sql::mysql::MySQL_Connection* conn)
{
  std::string values;

  const std::size_t np = dt->size();

  for(std::size_t i = 0; i < np; ++i)
  {
    if(i != 0)
      values += ",";

    values += GetSQLValue(dt->getProperty(i), i, d, conn);
  }

  values += "\n";

  return values;
}

std::string te::mysql::GetLoadDataFieldList(const te::da::DataSetType* dt)
{
  std::string values("(");

  std::string setExpression;

  const std::size_t np = dt->size();

  for(std::size_t i = 0; i < np; ++i)
  {
    if(i != 0)
      values += ",";

    const te::dt::Property* p = dt->getProperty(i);

    if(p->getType() == te::dt::GEOMETRY_TYPE)
    {
      std::string varname  = "@VAR";
                  varname += boost::lexical_cast<std::string>(i);

      values += varname;

      if(setExpression.empty())
        setExpression = "SET ";
      else
        setExpression = ", ";

      setExpression += p->getName() + " = GEOMFROMWKB(" + varname + ")";
    }
    else
    {
      values += p->getName();
    }
  }

  values += ") ";

  return (values + setExpression);
}

void te::mysql::Convert2MySQLGeom(const te::gm::GeomType t, std::string& geomType, std::string& dimension)
{
  switch(t)
  {
    case te::gm::PointType :
    //case te::gm::PointZType :
    //case te::gm::PointMType :
    //case te::gm::PointZMType :
      geomType = "POINT";
    break;

    case te::gm::LineStringType :
    //case te::gm::LineStringZType :
    //case te::gm::LineStringMType :
    //case te::gm::LineStringZMType :
      geomType = "LINESTRING";
    break;

    case te::gm::PolygonType :
    //case te::gm::PolygonZType :
    //case te::gm::PolygonMType :
    //case te::gm::PolygonZMType :
      geomType = "POLYGON";
    break;

    case te::gm::MultiPointType :
    //case te::gm::MultiPointZType :
    //case te::gm::MultiPointMType :
    //case te::gm::MultiPointZMType :
      geomType = "MULTIPOINT";
    break;

    case te::gm::MultiLineStringType :
    //case te::gm::MultiLineStringZType :
    //case te::gm::MultiLineStringMType :
    //case te::gm::MultiLineStringZMType :
      geomType = "MULTILINESTRING";
    break;

    case te::gm::MultiPolygonType :
    //case te::gm::MultiPolygonZType :
    //case te::gm::MultiPolygonMType :
    //case te::gm::MultiPolygonZMType :
      geomType = "MULTIPOLYGON";
    break;

    case te::gm::GeometryCollectionType :
    //case te::gm::GeometryCollectionZType :
    //case te::gm::GeometryCollectionMType :
    //case te::gm::GeometryCollectionZMType :
      geomType = "GEOMETRYCOLLECTION";
    break;

    case te::gm::GeometryType :
    //case te::gm::GeometryZType :
    //case te::gm::GeometryMType :
    //case te::gm::GeometryZMType :
      geomType = "GEOMETRY";
    break;

    default:
      throw Exception(TR_MYSQL("This geometric type is not supported by SpatiaLite!"));
  }

  if((t & 0xF00) == 0xB00)    // it is zm
    dimension = "4";
  else if((t & 0x0F00) == 0x300)   // it is z
    dimension = "3";
  else if((t & 0xF00) == 0x700)  // it is m
    dimension = "3";
  else
    dimension = "2";
}

void te::mysql::JSON2MySQL(const std::string path, std::auto_ptr<te::da::DataSourceTransactor> t)
{
  try
  {
    std::ifstream f;
    f.open(path.c_str());
    if (!f.is_open())
      return;
    
    t->begin();
    
    boost::property_tree::ptree pt;
    boost::property_tree::json_parser::read_json(f,pt);
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("SRSs"))
    {  
      std::string fullsql = "";
      fullsql += "INSERT INTO spatial_ref_sys VALUES (";
      fullsql += te::common::Convert2String(v.second.get<unsigned int>("srid")) + ", ";
      fullsql += "NULL, ";   // auth_name???
      fullsql += te::common::Convert2String(0) + ", "; // auth_srid???
      fullsql += "'', "; // srtext
      fullsql += "'');\n"; // proj4text
      //fullsql += "'" + v.second.get<std::string>("wkt") + "', "; // srtext
      //fullsql += "'" + v.second.get<std::string>("pj4txt") + "');\n"; // proj4text

      t->execute(fullsql);
    }
    f.close();

    t->commit();

  }
  catch(boost::property_tree::json_parser::json_parser_error &je)
  {
    std::string errmsg = "Error parsing: " + je.filename() + ": " + je.message();
    te::mysql::Exception ex(TR_MYSQL(errmsg));
    throw(ex);
  }
  catch (std::exception const& e)
  {
    std::cerr << e.what() << std::endl;
  }
  return;
}