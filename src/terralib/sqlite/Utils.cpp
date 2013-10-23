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
  \file terralib/sqlite/Utils.cpp

  \brief Utility functions for the TerraLib SQLite Data Access driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/Exception.h"
#include "Config.h"
#include "Utils.h"

// STL
#include <cassert>
#include <cstring>
#include <fstream>
#include <memory>

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/format.hpp>

// SQLite
#include <sqlite3.h>

int te::sqlite::GetConnectionFlags(const std::map<std::string, std::string>& connInfo)
{
  int flags = 0;

  if((connInfo.find("SQLITE_OPEN_NOMUTEX") != connInfo.end()) &&
     (boost::to_upper_copy(connInfo.find("SQLITE_OPEN_NOMUTEX")->second) == "TRUE"))
    flags |= SQLITE_OPEN_NOMUTEX;

  if((connInfo.find("SQLITE_OPEN_FULLMUTEX") != connInfo.end()) &&
     (boost::to_upper_copy(connInfo.find("SQLITE_OPEN_FULLMUTEX")->second) == "TRUE"))
    flags |= SQLITE_OPEN_FULLMUTEX;

  if((connInfo.find("SQLITE_OPEN_SHAREDCACHE") != connInfo.end()) &&
     (boost::to_upper_copy(connInfo.find("SQLITE_OPEN_SHAREDCACHE")->second) == "TRUE"))
    flags |= SQLITE_OPEN_SHAREDCACHE;

  if((connInfo.find("SQLITE_OPEN_PRIVATECACHE") != connInfo.end()) &&
     (boost::to_upper_copy(connInfo.find("SQLITE_OPEN_PRIVATECACHE")->second) == "TRUE"))
    flags |= SQLITE_OPEN_PRIVATECACHE;

  if((connInfo.find("SQLITE_OPEN_READWRITE") != connInfo.end()) &&
     (boost::to_upper_copy(connInfo.find("SQLITE_OPEN_READWRITE")->second) == "TRUE"))
    flags |= SQLITE_OPEN_READWRITE;
  else if((connInfo.find("SQLITE_OPEN_READONLY") != connInfo.end()) &&
     (boost::to_upper_copy(connInfo.find("SQLITE_OPEN_READONLY")->second) == "TRUE"))
    flags |= SQLITE_OPEN_READONLY;

  if((connInfo.find("SQLITE_OPEN_CREATE") != connInfo.end()) &&
     (boost::to_upper_copy(connInfo.find("SQLITE_OPEN_CREATE")->second) == "TRUE"))
    flags |= SQLITE_OPEN_CREATE;

  if((connInfo.find("SQLITE_OPEN_URI") != connInfo.end()) &&
     (boost::to_upper_copy(connInfo.find("SQLITE_OPEN_URI")->second) == "TRUE"))
    flags |= SQLITE_OPEN_CREATE;

  return flags;
}

bool te::sqlite::Exists(const std::map<std::string, std::string>& dbInfo)
{
  int flags = GetConnectionFlags(dbInfo);

  if(flags == 0)
    flags = SQLITE_OPEN_READONLY;

  flags &= 0xFFFFFFFB;

  std::string fileName;

  {
    std::map<std::string, std::string>::const_iterator it = dbInfo.find("SQLITE_FILE");

    if(it != dbInfo.end())
      fileName = it->second;
    else
      throw te::da::Exception(TR_COMMON("To check the existence of an SQLite database you must inform its file name or URI!"));
  }

  std::string vfs;

  {
    std::map<std::string, std::string>::const_iterator it = dbInfo.find("SQLITE_VFS");

    if(it != dbInfo.end())
      vfs = it->second;
  }

  sqlite3* db = 0;

  int ret = sqlite3_open_v2(fileName.c_str(), &db, flags, vfs.empty() ? 0 : vfs.c_str());

  sqlite3_close(db);

  return (ret == SQLITE_OK);
}

void te::sqlite::ExecuteScript(sqlite3* db, const char* fileName)
{
  std::ifstream istr;

  istr.open(fileName, std::ios_base::in);

  if(!istr.is_open())
    throw te::da::Exception((boost::format(TR_COMMON("Could not open file: %1%.")) % fileName).str());

  try
  {
    PerformCommands(db, istr);
  }
  catch(...)
  {
    istr.close();
    throw;
  }

  istr.close();
}

void te::sqlite::PerformCommands(sqlite3* db, std::istream& istr)
{
  char* buff = (char*)malloc(TE_SQLITE_BUFFER_SCRIPT_SIZE);
  char* sql = (char*)malloc(TE_SQLITE_BUFFER_SCRIPT_SIZE);
  size_t buffsize = TE_SQLITE_BUFFER_SCRIPT_SIZE;
  size_t sqlSize = TE_SQLITE_BUFFER_SCRIPT_SIZE;
  size_t offset = 0;
  size_t priorOffset = 0;

  while(!istr.eof())
  {
    std::streamsize readbytes = ReadLine(istr, &buff, buffsize);

    if(readbytes == 0)
      break;

    if(IsAllWhitespace(buff))
      continue;

    if(IsCommandTerminator(buff) && IsComplete(&sql, offset, sqlSize))
      memcpy(buff, ";", 2);
    
    std::size_t lineLen = strlen(buff);

    if((lineLen + 2) > (sqlSize - offset))
    {
      sql = (char*)realloc(sql, sqlSize + buffsize);
      sqlSize = sqlSize + buffsize;
    }

    if(offset != 0)
      sql[offset++] = '\n';

    memcpy(&sql[offset], buff, lineLen + 1);
    priorOffset = offset;
    offset += lineLen;

    if(offset && ContainsSemicolon(sql + priorOffset, lineLen) && sqlite3_complete(sql))
    {
      try
      {
        PerformCommands(db, sql);
        priorOffset = offset = 0;
      }
      catch(...)
      {
        free(buff);
        free(sql);

        throw;
      }
    }
  }

  if(offset && !IsAllWhitespace(sql))
  {
    boost::format errmsg(TR_COMMON("Error! incomplete SQL: %1%"));

    errmsg = errmsg % sql;

    free(buff);
    free(sql);

    throw te::da::Exception(errmsg.str());
  }


  free(buff);
  free(sql);
}

void te::sqlite::PerformCommands(sqlite3* db, const char* sql)
{
  sqlite3_stmt *pStmt = 0;
  int rc = SQLITE_OK;
  const char* sqlLeftOver = 0;

  while(sql[0] != '\0' && rc == SQLITE_OK)
  {
    rc = sqlite3_prepare_v2(db, sql, -1, &pStmt, &sqlLeftOver);

    if(rc != SQLITE_OK)
      throw te::da::Exception((boost::format(TR_COMMON("Error preparing the SQL statement. The follwoing error has occurried: %1%.")) % sqlite3_errmsg(db)).str()); 

    if(pStmt == 0)
    {      
      sql = sqlLeftOver;

      while(isspace(sql[0]))
        ++sql;

      continue;
    }

    rc = sqlite3_step(pStmt);

    sqlite3_finalize(pStmt);

    if(rc != SQLITE_ROW && rc != SQLITE_OK && rc != SQLITE_DONE)
    {
      std::string errmsg  = TR_COMMON("Error processing the SQL statement. The follwoing error has occurried: ");
                  errmsg += sqlite3_errmsg(db);
                  errmsg += ".";

      throw te::common::Exception(errmsg);
    }

    sql = sqlLeftOver;

    while(isspace(sql[0]))
      ++sql;

    rc = SQLITE_OK;
  }
}

std::streamsize te::sqlite::ReadLine(std::istream& istr, char** pbuff, std::size_t& buffsize)
{
  std::streamsize readbytes = 0;

  char* buff = *pbuff;

  do
  {
    istr.getline(buff + readbytes, static_cast<std::streamsize>(buffsize - readbytes));

    if(istr.flags() & std::ios::failbit)
    {
      buffsize += TE_SQLITE_BUFFER_SCRIPT_SIZE;
      buff = (char*)realloc(buff, buffsize);
      *pbuff = buff;
      readbytes += istr.gcount();
    }
    else
    {
      readbytes = istr.gcount();
      break;
    }
  }while(true);

  return readbytes;
}

bool te::sqlite::IsAllWhitespace(const char* sqlLine)
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

bool te::sqlite::IsCommandTerminator(const char* sqlLine)
{
  while(isspace(*(unsigned char*)sqlLine))
    ++sqlLine;

  if((sqlLine[0] == '/') && IsAllWhitespace(&sqlLine[1]))
    return true;

  if(tolower((sqlLine[0]) == 'g') && (tolower(sqlLine[1]) == 'o') && IsAllWhitespace(&sqlLine[2]))
    return true;

  return false;
}

bool te::sqlite::ContainsSemicolon(const char* sql, std::size_t nbytes)
{
  for(std::size_t i = 0; i < nbytes; ++i)
    if(sql[i] == ';')
      return true;

  return false;
}

bool te::sqlite::IsComplete(char** sql, size_t len, std::size_t& buffsize)
{
  char* psql = *sql;

  if(len == 0)
    return true;

  if(len + 2 > buffsize)
  {
    buffsize += TE_SQLITE_BUFFER_SCRIPT_SIZE;
    psql = (char*)realloc(psql, buffsize);
    *sql = psql;
  }

  psql[len] = ';';
  psql[len + 1] = 0;

  int rc = sqlite3_complete(psql);

  psql[len] = '\0';

  return rc != 0;
}
