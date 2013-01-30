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
  \file terralib/mysql/Utils.h

  \brief Utility functions for the MySQL Data Access driver.
*/

#ifndef __TERRALIB_MYSQL_INTERNAL_UTILS_H
#define __TERRALIB_MYSQL_INTERNAL_UTILS_H

// TerraLib
#include "../geometry/Enums.h"
#include "../dataaccess/Enums.h"
#include "Config.h"

// Forward declaration
#include "../dataaccess_fw.h"
#include "../datatype_fw.h"
#include "../geometry_fw.h"

// STL
#include <map>
#include <string>
#include <vector>

namespace sql
{
  class DatabaseMetaData;
  class PreparedStatement;
  class ResultSet;

  namespace mysql
  {
    class MySQL_Connection;
  }
}

namespace te
{
  namespace mysql
  {
    /*!
      \brief It creates a filter given a MBR.

      \param colName The name of the column where the spatial filter will be applied.
      \param e       The MBR to be applied to the column.
      \param r       The spatial relation.
      \param sql     A string buffer to output the filter.

      \exception Exception It may throws an exception.
    */
    TEMYSQLEXPORT void GetFilter(const std::string& colName,
                                 const te::gm::Envelope* e,
                                 const te::gm::SpatialRelation r,
                                 std::string& sql);

    /*!
      \brief It returns a filter given a Geometry.

      \param colName The name of the column where the spatial filter will be applied.
      \param g       The geometry to be applied as a filter to the column.
      \param r       The spatial relation.
      \param sql     A string buffer to output the filter.

      \exception Exception It may throws an exception.
    */
    TEMYSQLEXPORT void GetFilter(const std::string& colName,
                                 const te::gm::Geometry* g,
                                 const te::gm::SpatialRelation r,
                                 std::string& sql);

    /*!
      \brief It converts the TerraLib geometry to a MySQL geometry representation using MySQL's GeomFromWKB constructor .

      \param g   The geometry to serialize to a MySQL format.
      \param sql The string buffer to output the geometry in MySQL format.

      \exception Exception It may throws an exception.
    */
    TEMYSQLEXPORT void Convert2MySQLGeometry(const te::gm::Geometry* g, std::string& sql);

    /*!
      \brief It returns the TerraLib category for the table (BASE TABLE or VIEW).
    */
    TEMYSQLEXPORT int GetTableCategory(const std::string& tableType);

    /*!
      \brief It returns a property from the information of a column in the current row of a resultset.

      \exception Exception It may throws an exception.

      \note The caller will take the ownership of the returned property.
    */
    TEMYSQLEXPORT te::dt::Property* Convert2TerraLibProperty(const sql::ResultSet* result);

    /*!
      \brief It returns a property from the information of a column.

      \exception Exception It may throws an exception.

      \note The caller will take the ownership of the returned property.
    */
    TEMYSQLEXPORT te::dt::Property* Convert2TerraLibProperty(const std::string& colname,
                                                             const std::string& coltypename,
                                                             const bool isUnsigned,
                                                             const int colid,
                                                             const int datatype,
                                                             const int colsize,
                                                             const int ddigits,
                                                             const bool nullable,
                                                             const bool isAutoIncrement,
                                                             std::string* defvalue);

    /*!
      \brief Given a MySQL index name converts to a TerraLib valid enum.

      \exception Exception It may throws an exception.
    */
    TEMYSQLEXPORT te::da::IndexType Convert2TerraLibIndexType(const std::string& idxType);

    /*!
      \brief Creates the TerraLib schema from the dataset.

      \param result A resultset to extract its schema.

      \return A TerraLib schema for the resultset. The caller will take its ownership.

      \exception Exception It may throws an exception.
    */
    TEMYSQLEXPORT te::da::DataSetType* Convert2TerraLibDataSetType(sql::ResultSet* result);

    /*!
      \brief Creates a connection URL from the connection information.
    */
    TEMYSQLEXPORT std::string GetURLConnection(const std::map<std::string, std::string>& dsInfo);

    /*!
      \brief It creates a string that can be used in a create table to add property p.

      The string will contains PRIMARY KEY definition.

      \param p The property to be encoded in a SQL.

      \return A string that can be used in a create table to add property p.

      \exception Exception It may throws an exception.
    */
    TEMYSQLEXPORT std::string Convert2SQLCreate(const te::dt::Property* p);

    /*!
      \brief Given a TerraLib property returns the MySQL type that can be used in CREATE and ALTER statements.

      \param p The property to extract its SQL equivalent type name.

      \return The MySQL type that can be used in CREATE and ALTER statements.

      \exception Exception It may throws an exception.
    */
    TEMYSQLEXPORT std::string GetSQLType(const te::dt::Property* p);

    /*!
      \brief Given a TerraLib geometric type returns the MySQL equivalent to be used to create a geometry column.

      \param t The TerraLib geometric type to be converted to MySQL.

      \return A textual representation of the geometry type as accepted by MySQL.

      \exception Exception It may throws an exception.

      \note te::gm::UnknownGeometryType is converted to GEOMETRY.
    */
    TEMYSQLEXPORT std::string Convert2MyGeom(const te::gm::GeomType t);

    TEMYSQLEXPORT std::string GetSQLValueNames(const te::da::DataSetType* dt);

    TEMYSQLEXPORT std::string GetSQLValues(const te::da::DataSetType* dt, te::da::DataSet* d, sql::mysql::MySQL_Connection* conn);

    TEMYSQLEXPORT std::string GetSQLValue(const te::dt::Property* p, std::size_t propertyPos, te::da::DataSet* d, sql::mysql::MySQL_Connection* conn);

    TEMYSQLEXPORT std::string GetSQLBindValues(const te::da::DataSetType* dt);

    TEMYSQLEXPORT std::string GetSQLUpdateValues(const te::da::DataSetType* dt, te::da::DataSet* dataset, const std::vector<std::size_t>& propertiesPos, sql::mysql::MySQL_Connection* conn);

    TEMYSQLEXPORT std::string GetSQLWhereValues(const te::da::DataSetType* dt, te::da::DataSet* dataset, const std::vector<std::size_t>& propertiesPos, sql::mysql::MySQL_Connection* conn);

    /*!
      \brief Test to see if a line consists entirely of whitespace.

      \param sqlLine A NULL terminated string.

      \return It returns true if the line has just whitespace otherwise returns false.

      \note By whitespace we mean: blocks of comments, line of comments and just spaces and tabs.
    */
    TEMYSQLEXPORT bool IsAllWhitespace(const char* sqlLine);

    /*!
      \brief Given a list of properties it creates an AND connected expression with bindable parameters ($n).

      \param itBegin The start property.
      \param itEnd   The mark of end of properties.

      \return An AND connected expression with bindable parameters ($n) that can be used in WHERE expressions.
    */
    TEMYSQLEXPORT std::string GetBindableWhereSQL(const std::vector<te::dt::Property*>& properties);

    TEMYSQLEXPORT std::string GetBindableUpdateSQL(const std::vector<te::dt::Property*>& properties);

    TEMYSQLEXPORT std::string GetLoadDataRow(const te::da::DataSetType* dt, te::da::DataSet* d, sql::mysql::MySQL_Connection* conn);

    /*!
      \brief It constructs a list of column names and add an expression for geometry fields.
      
      This may be used for LOAD DATA commands.
    */
    TEMYSQLEXPORT std::string GetLoadDataFieldList(const te::da::DataSetType* dt);

    /*!
      \brief Given a TerraLib geometric type returns the MySQL equivalent to be used to register a geometry column.

      \param t         The TerraLib geometric type to be converted to MySQL.
      \param geomType  A textual representation of the geometry type as registered in MySQL geometry_columns table.
      \param dimension A textual representation of the geometry dimension as registered in MySQL geometry_columns table.
    */
    TEMYSQLEXPORT void Convert2MySQLGeom(const te::gm::GeomType t, std::string& geomType, std::string& dimension);

    /*!
      \brief Given a TerraLib geometric type returns the MySQL equivalent to be used to register a geometry column.

      \param t         The TerraLib geometric type to be converted to MySQL.
      \param geomType  A textual representation of the geometry type as registered in MySQL geometry_columns table.
      \param dimension A textual representation of the geometry dimension as registered in MySQL geometry_columns table.
    */
    TEMYSQLEXPORT void JSON2MySQL(const std::string path, std::auto_ptr<te::da::DataSourceTransactor> t);

  } // end namespace mysql
}   // end namespace te

#endif  // __TERRALIB_MYSQL_INTERNAL_UTILS_H

