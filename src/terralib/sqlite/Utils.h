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
  \file terralib/sqlite/Utils.h

  \brief Utility functions for the TerraLib SQLite Data Access driver.
*/

#ifndef __TERRALIB_SQLITE_INTERNAL_UTILS_H
#define __TERRALIB_SQLITE_INTERNAL_UTILS_H

// TerraLib
#include "../dataaccess/Enums.h"
#include "../geometry/Enums.h"
#include "Config.h"

// STL
#include <iosfwd>
#include <map>
#include <set>
#include <vector>

// Forward declarations
extern "C"
{
  struct sqlite3;
  typedef struct sqlite3 sqlite3;

  struct sqlite3_stmt;
  typedef struct sqlite3_stmt sqlite3_stmt;
}

namespace te
{
// Forward declarations
  namespace da
  {
    class DataSet;
    class DataSetItem;
    class DataSetType;
    class DataSource;
  }

  namespace dt
  {
    class Property;
  }

  namespace gm
  {
    class Envelope;
    class Geometry;
  }

  namespace sqlite
  {
    /*!
      \brief It reads input from file and process it.
      
      You can use this function to execute an SQL script file.

      \param db       The database handle.
      \param fileName The name of the script file with fully qualified path (or the relative path).

      \exception Exception It throws an exception if it can not read or execute the statements in the script.
    */
    TESQLITEEXPORT void ExecuteScript(sqlite3* db, const char* fileName);

    /*!
      \brief It reads SQL statements from an input stream and process it.
      
      You can use this function to execute an SQL script file.

      \param db    The database handle.
      \param istr  The input stream with SQL statements.

      \exception Exception It throws an exception if it can not read or execute the statements in the script.

      \note The caller of this method will have to open and close the input stream.

      \note Some routines in this file is based on the SQLite command prompt implementation. Please, see the SQLite copyright notice available in the TerraLib licences folder.
    */
    TESQLITEEXPORT void PerformCommands(sqlite3* db, std::istream& istr);

    /*!
      \brief It executes a statement or set of statements.
      
      This will perform a statement or a set of well formed statements.
      Don't use it to send incomplete statements.

      \param db  The database handle.
      \param sql SQL to be evaluated

      \exception Exception It throws an exception if it can not execute the statements.

      \note Some routines in this file is based on the SQLite command prompt implementation. Please, see the SQLite copyright notice available in the TerraLib licences folder.
    */
    TESQLITEEXPORT void PerformCommands(sqlite3* db, const char* sql);

    /*!
      \brief It reads a line of text from the input stream and stores the text into the buffer.
     
      The text is NULL terminated. If it is needed, this
      routine will increase the informed buffer.

      \param istr     An input stream.
      \param pbuff    A pointer to a memory buffer obtained from malloc(). The *pbuff must be a valid buffer before calling this method.
      \param buffsize The buffer size. It can be update by this function if it is needed to increase the buffer size (with realloc).

      \return The number of read bytes from the input stream. This includes a trailing '\0'.

      \note The informed buffer may be increased by TE_SQLITE_BUFFER_SCRIPT_SIZE bytes if it is necessary.

      \note Some routines in this file is based on the SQLite command prompt implementation. Please, see the SQLite copyright notice available in the TerraLib licences folder.
    */
    TESQLITEEXPORT std::streamsize ReadLine(std::istream& istr, char** pbuff, std::size_t& buffsize);

    /*!
      \brief Test to see if a line consists entirely of whitespace.

      \param sqlLine A NULL terminated string.

      \return It returns true if the line has just whitespace otherwise returns false.

      \note By whitespace we mean: blocks of comments, line of comments and just spaces and tabs.

      \note Some routines in this file is based on the SQLite command prompt implementation. Please, see the SQLite copyright notice available in the TerraLib licences folder.
    */
    TESQLITEEXPORT bool IsAllWhitespace(const char* sqlLine);

    /*!
      \brief It returns true if the line is an SQL command terminator other than a semi-colon.

      The SQL Server style "go" command is understood as is the Oracle.

      \param sqlLine The SQL line command to be tested.

      \return It returns true if the line is a command terminator or false otherwise.

      \note Some routines in this file is based on the SQLite command prompt implementation. Please, see the SQLite copyright notice available in the TerraLib licences folder.
    */
    TESQLITEEXPORT bool IsCommandTerminator(const char* sqlLine);

    /*!
      \brief It checks if a semicolon occurs anywhere in the SQL string.

      \param sql    The SQL statement we want to check.
      \param nbytes The maximum number of bytes to be evaluated.

      \return It returns true if a semicolon occurs anywhere in the sql string, otherwise, returns false.

      \note Some routines in this file is based on the SQLite command prompt implementation. Please, see the SQLite copyright notice available in the TerraLib licences folder.
    */
    TESQLITEEXPORT bool ContainsSemicolon(const char* sql, std::size_t nbytes);

    /*!
      \brief It checks if sql is a complete SQL statement.

      \param sql      The SQL statement to be cheked.
      \param len      The SQL statement size (text length).
      \param buffsize The sql buffer size in bytes.
      
      \return It returns false if it ends in the middle of a string literal or C-style comment.

      \note Some routines in this file is based on the SQLite command prompt implementation. Please, see the SQLite copyright notice available in the TerraLib licences folder.
    */
    TESQLITEEXPORT bool IsComplete(char** sql, size_t len, std::size_t& buffsize);

    /*!
      \brief Given information about an SQLite attribute it returns a TerraLib Property object.

      \param colId        The id of the column.
      \param colName      The column name.
      \param colType      The name of column datatype.
      \param required     If true the column is required, otherwise, it can have NULL values.
      \param defaultValue The default value if one exists. The new property will take its ownership.

      \return A Property given some information from SQLite system tables.

      \note In the case of a geometry column this function will not set the appropriate subtype nor the srid.
    */
    TESQLITEEXPORT te::dt::Property* Convert2TerraLib(int colId,
                                                      const std::string& colName,
                                                      const std::string& colType,
                                                      bool required,
                                                      std::string* defaultValue = 0);

    /*!
      \brief It creates a dataset type from the given query.

      \param pStmt A prepared statement generated from a query.

      \return The caller of this function will take the ownership of the returned dataset type.
    */
    TESQLITEEXPORT te::da::DataSetType* Convert2TerraLib(sqlite3_stmt* pStmt);

    /*!
      \brief It checks if the table takes part of a raster dataset or not.

      \param raster  A list of pre-fixed rasters.
      \param tblName The table to check if it is a raster metadata table or the rasters table.

      \return TE_SQLITE_IS_NOT_RASTER_TABLE if table doesn't take part of a raster dataset, otherwise,
              it returns TE_SQLITE_IS_RASTER_METADATA_TABLE if table is a metadata table
              or TE_SQLITE_IS_RASTER_TABLE if table is a raster table.
    */
    TESQLITEEXPORT int IsRasterTable(const std::set<std::string>& rasters, const std::string& tblName);

    /*!
      \brief It extracts the flag parameters to be used by sqlite3_open_v2 (actually, the third parameter).

      \param connInfo A key-value-pair list with connection parameters.

      \return An int with the flag parameters to be used by sqlite3_open_v2 (actually, the third parameter).

      \note This function returns 0 if no flags are present in the input map.
    */
    TESQLITEEXPORT int GetConnectionFlags(const std::map<std::string, std::string>& connInfo);

    /*!
      \brief It checks if the given SQLite data source already exists or not.

      \param dbInfo The same set of key-value pairs used to open a SQLite data source. At least the database name with fully qualified path or URI must be present.

      \return It returns true if the given database already exists, otherwise, it returns false.

      \note If the creational flag is present in the kvp list it will be discarted.

      \note If no flag is present it will use read-only flag.
    */
    TESQLITEEXPORT bool Exists(const std::map<std::string, std::string>& dbInfo);

    /*!
      \brief Given a data source it look its parameters to see the tables choosed as hidden.

      \param ds     The data source to search for hidden tables;
      \param tables The output list of tables that the user has choosed to ommit/hide.
    */
    TESQLITEEXPORT void GetHiddenTables(const te::da::DataSource* ds, std::vector<std::string>& tables);

    /*!
      \brief It returns the DataSetType category based on SQLite internal classification for objecst stored in sqlite_master.

      \param category A string with the SQLite object type: table, index, view, trigger.

      \return The equivalent TerraLib object category: DB_TABLE, DB_SYSTEM_TABLE, DB_VIEW, DB_QUERY, DB_INDEX, DB_SEQUENCE, DB_TRIGGER, REGULAR_FILE.
    */
    TESQLITEEXPORT int Convert2TerraLibCategory(const std::string& category);

    /*!
      \brief Given a SpatiaLite geometry description and its coordinate dimension, returns a TerraLib geometry type code.

      \param geomType  A textual representation of the geometry type as registered in SpatiaLite geometry_columns table.
      \param dimension A textual representation of the geometry dimension as registered in SpatiaLite geometry_columns table.

      \return A TerraLib geometry type code.
    */
    TESQLITEEXPORT te::gm::GeomType Convert2TerraLibGeomType(const std::string& geomType, const std::string& dimension);

    /*!
      \brief Given a TerraLib geometric type returns the SpatiaLite equivalent to be used to register a geometry column.

      \param t         The TerraLib geometric type to be converted to SpatiaLite.
      \param geomType  A textual representation of the geometry type as registered in SpatiaLite geometry_columns table.
      \param dimension A textual representation of the geometry dimension as registered in SpatiaLite geometry_columns table.
    */
    TESQLITEEXPORT void Convert2SpatiaLiteGeom(const te::gm::GeomType t, std::string& geomType, std::string& dimension);

    /*!
      \brief It creates a string that can be used in a create table to add property p.

      \param p The property to be encoded in a SQL.

      \return A string that can be used in a create table to add property p.

      \note Geometric types are converted to SpatiaLite notation.

      \note The returned expression will contains primary key modifiers if it is the case. 
    */
    TESQLITEEXPORT std::string Convert2SQLCreate(const te::dt::Property* p);

    /*!
      \brief Given a TerraLib property returns the SQLite type that can be used in CREATE and ALTER statements.

      \param p The property to extract its SQL equivalent type name.

      \return The SQLite type that can be used in CREATE and ALTER statements.

      \note Geometric types are converted to SpatiaLite notation.
    */
    TESQLITEEXPORT std::string GetSQLType(const te::dt::Property* p);

    /*!
      \brief Given a dataset type it creates a the values list of an SQL INSERT statement.

      \param dt The dataset type to extract the attribute names.

      \return An expression that can be used in an INSERT statement (prop1-name, prop2-name, ...).
    */
    TESQLITEEXPORT std::string GetSQLValueNames(const te::da::DataSetType* dt);

    /*!
      \brief Given a list of properties it creates an AND connected expression with bindable parameters (?).

      \param itBegin The start property.
      \param itEnd   The mark of end of properties.

      \return An AND connected expression with bindable parameters (?) that can be used in WHERE expressions.
    */
    TESQLITEEXPORT std::string GetBindableWhereSQL(std::vector<te::dt::Property*>::const_iterator itBegin,
                                                   std::vector<te::dt::Property*>::const_iterator itEnd);

    /*!
      \brief Given a list of properties it constructs a string with bindable parameters that can be used inside an update query.

      \param properties A vector with property names that will be used to create the expression.

      \return A comma (',') connected expression with bindable parameters (?) that can be used in an UPDATE query.
    */
    TESQLITEEXPORT std::string GetBindableUpdateSQL(const std::vector<te::dt::Property*>& properties);

    /*!
      \brief Given a dataset type it creates list of values to be bind (?).

      \param dt The dataset type with proprties to be binded.

      \return An expression with bindable parameters (?, ?, ? , ...) that can be used in INSERT expressions.
    */
    TESQLITEEXPORT std::string GetSQLBindValues(const te::da::DataSet* dataset);

    /*!
      \brief It creates a bindable expression for a spatial relation involving a column and a parameterized value.
      
      Example: Intersects(column-name, ?).

      \param colName A column name.
      \param r       The spatial relation.

      \return A bindable expression for a spatial relation involving a column and a parameterized value.
    */
    TESQLITEEXPORT std::string GetBindableSpatialRelation(const std::string& colName, const te::gm::SpatialRelation r);

    /*!
      \brief It creates an R-tree filter based on the given rectangle and spatia relation.

      \param e The rectangle to be used as filter.
      \param r The relationship to explore the spatial index.

      \return An expression with RTreeIntersects, RTreeWithin or RTreeContain.
    */
    TESQLITEEXPORT std::string GetRtreeFilter(const te::gm::Envelope* e, const te::gm::SpatialRelation r);

    /*!
      \brief It converts the SQLite foreign key modifier to a TerraLib data type.

      <ul>
      <li>NO ACTION</li>
      <li>RESTRICT</li>
      <li>CASCADE</li>
      <li>SET NULL</li>
      <li>SET DEFAULT</li>
      </ul>

      \param action A SQLite foreign key modifier type.

      \retun The equivalent TerraLib foreign key modifier type.
    */
    TESQLITEEXPORT te::da::FKActionType GetAction(const std::string& action);

  } // end namespace sqlite
}   // end namespace te

#endif  // __TERRALIB_SQLITE_INTERNAL_UTILS_H

