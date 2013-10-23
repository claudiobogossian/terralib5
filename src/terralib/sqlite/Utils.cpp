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
#include "Utils.h"

// Boost
#include <boost/algorithm/string/case_conv.hpp>

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

