/*  Copyright (C) 2009-2013 National Institute For Space Research (INPE) - Brazil.

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

// STL
#include <iosfwd>
#include <map>

extern "C"
{
  struct sqlite3;
  typedef struct sqlite3 sqlite3;

  struct sqlite3_stmt;
  typedef struct sqlite3_stmt sqlite3_stmt;
}

namespace te
{
  namespace da
  {
    class DataSet;
    class DataSource;
    class DataSetType;
  }

  namespace dt
  {
    class Property;
  }

  namespace gm
  {
    class Envelope;
  }

  namespace sqlite
  {
    int GetConnectionFlags(const std::map<std::string, std::string>& connInfo);

    bool Exists(const std::map<std::string, std::string>& dbInfo);

    void ExecuteScript(sqlite3* db, const char* fileName);

    void PerformCommands(sqlite3* db, std::istream& istr);

    void PerformCommands(sqlite3* db, const char* sql);

    std::streamsize ReadLine(std::istream& istr, char** pbuff, std::size_t& buffsize);

    bool IsAllWhitespace(const char* sqlLine);

    bool IsCommandTerminator(const char* sqlLine);

    bool ContainsSemicolon(const char* sql, std::size_t nbytes);

    bool IsComplete(char** sql, size_t len, std::size_t& buffsize);

    void GetHiddenTables(const te::da::DataSource* ds, std::vector<std::string>& tables);

    te::da::DataSetType* Convert2TerraLib(sqlite3_stmt* pStmt);

    te::dt::Property* Convert2TerraLib(int colId,
                                       const std::string& colName,
                                       const std::string& colType,
                                       bool required,
                                       std::string* defaultValue = 0);

    int Convert2TerraLibCategory(const std::string& category);

    std::string GetRtreeFilter(const te::gm::Envelope* e, const te::gm::SpatialRelation r);

    std::string GetBindableSpatialRelation(const std::string& colName, const te::gm::SpatialRelation r);

    void Convert2SpatiaLiteGeom(const te::gm::GeomType t, std::string& geomType, std::string& dimension);

    std::string GetSQLType(const te::dt::Property* p);

    te::da::FKActionType GetAction(const std::string& action);

    std::string Convert2SQLCreate(const te::dt::Property* p);

    std::string GetSQLBindValues(const te::da::DataSet* dataset);

  } // end namespace sqlite
}   // end namespace te

#endif  // __TERRALIB_SQLITE_INTERNAL_UTILS_H
