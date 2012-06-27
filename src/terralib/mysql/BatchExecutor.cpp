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
  \file terralib/mysql/BatchExecutor.cpp

  \brief Implementation of BatchExecutor for MySQL data access driver.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/datasource/ScopedTransaction.h"
#include "../dataaccess/query/Query.h"
#include "BatchExecutor.h"
#include "Connection.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "SQLVisitor.h"
#include "Utils.h"

// STL
#include <fstream>
#include <limits>
#include <memory>

// Boost
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/trim.hpp>

// MySQL C++ connector
#include <driver/mysql_public_iface.h>

te::mysql::BatchExecutor::BatchExecutor(DataSourceTransactor* t)
  : m_t(t)
{
}

te::mysql::BatchExecutor::~BatchExecutor()
{
  te::common::FreeContents(m_commands);
}

void te::mysql::BatchExecutor::add(te::da::Query* q)
{
  std::auto_ptr<te::da::Query> qq(q);

  std::auto_ptr<std::string> sql(new std::string);

  SQLVisitor visitor(*(m_t->getDataSource()->getDialect()), *sql);
  q->accept(visitor);

  m_commands.push_back(sql.release());
}

void te::mysql::BatchExecutor::execute()
{
  te::da::ScopedTransaction t(*m_t);

  const std::size_t ncommands = m_commands.size();

  for(std::size_t i = 0; i < ncommands; ++i)
    m_t->execute(*m_commands[i]);

  te::common::FreeContents(m_commands);
  m_commands.clear();
  
  t.commit();
}

void te::mysql::BatchExecutor::execute(const std::string& uri, const std::map<std::string, std::string>& options)
{
  std::map<std::string, std::string>::const_iterator it = options.find("USE_LOAD_DATA");
  std::map<std::string, std::string>::const_iterator itend = options.end();

  if(it != itend)
  {
    execLoadData(uri, options);
    return;
  }

// open file
  std::ifstream istr;

  istr.open(uri.c_str(), std::ios_base::in);

  if(!istr.is_open())
    throw Exception((boost::format(TR_MYSQL("Could not open file: %1%, for batch execution.")) % uri).str());

  try
  {
    execute(istr, options);
  }
  catch(...)
  {
    istr.close();
    throw;
  }

  istr.close();
}

void te::mysql::BatchExecutor::execute(std::istream& istr, const std::map<std::string, std::string>& options)
{
  if(options.count("EXEC_SCRIPT") > 0)
    execMultiLineCommand(istr, options);
  else
    execLote(istr, options);
}

te::da::DataSourceTransactor* te::mysql::BatchExecutor::getTransactor() const
{
  return m_t;
}

void te::mysql::BatchExecutor::prepareServer(const std::map<std::string, std::string>& options,
                                             const std::string& tblName,
                                             bool& turnOnFkCheckAtEnd,
                                             bool& enableIndexesAtEnd,
                                             bool& turnOnUniqueChecksAtEnd,
                                             std::string& restoreBulkBufferSizeAtEnd,
                                             std::string& restoreKeyBufferSizeAtEnd)
{
// see if we can disable FK checks during execution
  std::map<std::string, std::string>::const_iterator it = options.find("DISABLE_FK");
  std::map<std::string, std::string>::const_iterator itend = options.end();

  if(it != itend)
  {
    std::auto_ptr<te::da::DataSet> result(m_t->query("SHOW VARIABLES LIKE 'foreign_key_checks'"));

    if(result->moveNext())
    {
      turnOnFkCheckAtEnd = result->getString(1) == "ON";

      if(turnOnFkCheckAtEnd)
        m_t->execute("SET foreign_key_checks = 'OFF'");
    }
  }

// see if we can disable indexes
  it = options.find("DISABLE_INDEXES");

  if(it != itend)
  {
    if(tblName.empty())
      throw Exception(TR_MYSQL("Could not disable table index: missing table name parameter!"));

    std::string sql("ALTER TABLE ");
                sql += tblName;
                sql += " DISABLE KEYS";
    
    m_t->execute(sql);

    enableIndexesAtEnd = true;
  }

// see if we can disable unique checks
  it = options.find("DISABLE_UNIQUE_CHECKS");

  if(it != itend)
  {
    std::auto_ptr<te::da::DataSet> result(m_t->query("SHOW VARIABLES LIKE 'unique_checks'"));

    if(result->moveNext())
    {
      turnOnUniqueChecksAtEnd = result->getString(1) == "ON";

      if(turnOnUniqueChecksAtEnd)
        m_t->execute("SET unique_checks = 'OFF'");
    }
  }

// see if we can increase the insert buffer size
  it = options.find("BULK_BUFFER_SIZE");

  if(it != itend)
  {
    std::auto_ptr<te::da::DataSet> result(m_t->query("SHOW VARIABLES LIKE 'bulk_insert_buffer_size'"));

    if(result->moveNext())
    {
      restoreBulkBufferSizeAtEnd = result->getNumeric(1);

      std::string sql("SET bulk_insert_buffer_size = ");
                  sql += it->second;

      m_t->execute(sql);
    }
  }

// see if we can increase keys buffer size
  it = options.find("KEY_BUFFER_SIZE");

  if(it != itend)
  {
    std::auto_ptr<te::da::DataSet> result(m_t->query("SHOW VARIABLES LIKE 'key_buffer_size'"));

    if(result->moveNext())
    {
      restoreKeyBufferSizeAtEnd = result->getNumeric(1);

      std::string sql("SET GLOBAL key_buffer_size = ");
                  sql += it->second;

      m_t->execute(sql);
    }
  }
}

void te::mysql::BatchExecutor::restoreServer(const std::string& tblName,
                                             bool& turnOnFkCheckAtEnd,
                                             bool& enableIndexesAtEnd,
                                             bool& turnOnUniqueChecksAtEnd,
                                             std::string& restoreBulkBufferSizeAtEnd,
                                             std::string& restoreKeyBufferSizeAtEnd)
{
  if(turnOnFkCheckAtEnd)
    m_t->execute("SET foreign_key_checks = 'ON'");

  if(enableIndexesAtEnd)
  {
    std::string sql("ALTER TABLE ");
                sql += tblName;
                sql += " ENABLE KEYS";
    
    m_t->execute(sql);
  }

  if(turnOnUniqueChecksAtEnd)
    m_t->execute("SET unique_checks = 'ON'");

  if(!restoreBulkBufferSizeAtEnd.empty())
  {
    std::string sql("SET bulk_insert_buffer_size = ");
                sql += restoreBulkBufferSizeAtEnd;

    m_t->execute(sql);
  }

  if(!restoreKeyBufferSizeAtEnd.empty())
  {
    std::string sql("SET GLOBAL key_buffer_size = ");
                sql += restoreKeyBufferSizeAtEnd;

    m_t->execute(sql);
  }
}

void te::mysql::BatchExecutor::execSingle(std::istream& istr)
{
  std::streamsize buffsize = TE_MYSQL_BUFFER_SCRIPT_SIZE;
  char* buff = (char*)malloc(TE_MYSQL_BUFFER_SCRIPT_SIZE);    // used to buffer a line of SQL command

  std::auto_ptr<sql::Statement> stmt(m_t->getConnection()->getConn()->createStatement());

  try
  {
    while(!istr.eof())
    {
      std::streamsize readbytes = 0;

      do
      {
        istr.getline(buff + readbytes, static_cast<std::streamsize>(buffsize - readbytes));

        if(istr.flags() == std::ios::failbit)
        {
          buffsize *= 2;
          buff = (char*)realloc(buff, static_cast<std::size_t>(buffsize));
        
          readbytes += istr.gcount();
        }
        else
        {
          readbytes += istr.gcount();
          break;
        }
      }while(true);

      //if(readbytes == 0)
      //  continue;

      if(!IsAllWhitespace(buff))
        stmt->execute(buff);
    }
  }
  catch(...)
  {
    free(buff);
    throw;
  }

  free(buff);
}

void te::mysql::BatchExecutor::execMultiple(std::istream& istr, std::size_t ncommands)
{
  std::streamsize buffsize = TE_MYSQL_BUFFER_SCRIPT_SIZE;
  char* buff = (char*)malloc(TE_MYSQL_BUFFER_SCRIPT_SIZE);    // used to buffer a line of SQL command

  int i = 0;

  std::streamsize readbytes = 0;
  std::streamsize offset = 0;

  std::auto_ptr<sql::Statement> stmt(m_t->getConnection()->getConn()->createStatement());

  try
  {
    while(!istr.eof())
    {
      do
      {
        istr.getline(buff + offset, static_cast<std::streamsize>(buffsize - offset));

        if(istr.flags() == std::ios::failbit)
        {
          buffsize *= 2;
          buff = (char*)realloc(buff, static_cast<std::size_t>(buffsize));
        
          readbytes = istr.gcount();
          offset += readbytes - 1;
        }
        else
        {
          readbytes = istr.gcount();
          offset += readbytes - 1;
          break;
        }
      }while(true);

      ++i;

      //if(readbytes == 0)
      //{
      //  if(!IsAllWhitespace(buff))
      //  {
      //    stmt->execute(buff);

      //    while(stmt->getMoreResults())
      //      std::auto_ptr<sql::ResultSet> res(stmt->getResultSet());
      //  }

      //  break;  // reached end-of-stream
      //}

      if(i % ncommands != 0)
        continue; // get another command

      if(!IsAllWhitespace(buff))
      {
        stmt->execute(buff);

        while(stmt->getMoreResults())
          std::auto_ptr<sql::ResultSet> res(stmt->getResultSet());
      }

      offset = 0;
      readbytes = 0;
    }

// do we have more commands to execute?
    if(i % ncommands != 0)
    {
      if(!IsAllWhitespace(buff))
      {
        stmt->execute(buff);

        while(stmt->getMoreResults())
          std::auto_ptr<sql::ResultSet> res(stmt->getResultSet());
      }
    }
  }
  catch(...)
  {
    free(buff);
    throw;
  }

  free(buff);
}

void te::mysql::BatchExecutor::execLoadData(const std::string& uri, const std::map<std::string, std::string>& options)
{
  std::map<std::string, std::string>::const_iterator it = options.find("TABLE_NAME");
  std::map<std::string, std::string>::const_iterator itend = options.end();

// get table name if any
  std::string  tblName = it != itend ? it->second : std::string("");

// prepare server and keep parameters to restore later
  bool turnOnFkCheckAtEnd = false;
  bool enableIndexesAtEnd = false;
  bool turnOnUniqueChecksAtEnd = false;
  std::string restoreBulkBufferSizeAtEnd;
  std::string restoreKeyBufferSizeAtEnd;

  try
  {
    prepareServer(options, tblName, turnOnFkCheckAtEnd, enableIndexesAtEnd, turnOnUniqueChecksAtEnd, restoreBulkBufferSizeAtEnd, restoreKeyBufferSizeAtEnd);

    std::string sql("LOAD DATA ");

    it = options.find("LOW_PRIORITY");

    if(it != itend)
      sql += "LOW_PRIORITY ";

    it = options.find("CONCURRENT");

    if(it != itend)
      sql += "CONCURRENT ";

    it = options.find("LOCAL");

    if(it != itend)
      sql += "LOCAL ";

    sql += "INFILE '";
    sql += uri;
    sql += "' ";

    it = options.find("REPLACE");

    if(it != itend)
      sql += "REPLACE ";

    it = options.find("IGNORE");

    if(it != itend)
      sql += "IGNORE ";

    sql += "INTO TABLE ";

    if(tblName.empty())
      throw Exception(TR_MYSQL("Can not execute command because table name was not informed!"));

    sql += tblName;
    sql += " ";

    it = options.find("CHARSET");
  
    if(it != itend)
    {
      sql += "CHARACTER SET ";
      sql += it->second;
      sql += " ";
    }

    bool addedFields = false;

    it = options.find("FIELDS_TERMINATED_BY");

    if(it != itend)
    {
      addedFields = true;

      sql += "FIELDS TERMINATED BY '";
      sql += it->second;
      sql += "' ";
    }

    it = options.find("FIELDS_ENCLOSED_BY");

    if(it != itend)
    {
      if(!addedFields)
      {
        addedFields = true;
        sql += "FIELDS ";
      }

      sql += "ENCLOSED BY '";
      sql += it->second;
      sql += "' ";
    }

    it = options.find("FIELDS_ESCAPED_BY");

    if(it != itend)
    {
      if(!addedFields)
      {
        addedFields = true;
        sql += "FIELDS ";
      }

      sql += "ESCAPED BY '";
      sql += it->second;
      sql += "' ";
    }

    bool addedLines = false;

    it = options.find("LINES_STARTING_BY");

    if(it != itend)
    {
      addedLines = true;

      sql += "LINES STARTING BY '";
      sql += it->second;
      sql += "' ";
    }

    it = options.find("LINES_TERMINATED_BY");

    if(it != itend)
    {
      if(!addedLines)
      {
        addedLines = true;
        sql += "LINES ";
      }

      sql += "TERMINATED BY '";
      sql += it->second;
      sql += "' ";
    }

    it = options.find("IGNORE_LINES");

    if(it != itend)
    {
      sql += "IGNORE ";
      sql += it->second;
      sql += " LINES";
    }

    m_t->execute(sql);
  }
  catch(...)
  {
    restoreServer(tblName, turnOnFkCheckAtEnd, enableIndexesAtEnd, turnOnUniqueChecksAtEnd, restoreBulkBufferSizeAtEnd, restoreKeyBufferSizeAtEnd);
    throw;
  }

  restoreServer(tblName, turnOnFkCheckAtEnd, enableIndexesAtEnd, turnOnUniqueChecksAtEnd, restoreBulkBufferSizeAtEnd, restoreKeyBufferSizeAtEnd);
}

void te::mysql::BatchExecutor::execLote(std::istream& istr, const std::map<std::string, std::string>& options)
{
  std::map<std::string, std::string>::const_iterator it = options.find("TABLE_NAME");
  std::map<std::string, std::string>::const_iterator itend = options.end();

// get table name if any
  std::string  tblName = it != itend ? it->second : std::string("");

// prepare server and keep parameters to restore later
  bool turnOnFkCheckAtEnd = false;
  bool enableIndexesAtEnd = false;
  bool turnOnUniqueChecksAtEnd = false;
  std::string restoreBulkBufferSizeAtEnd;
  std::string restoreKeyBufferSizeAtEnd;

  try
  {
    prepareServer(options, tblName, turnOnFkCheckAtEnd, enableIndexesAtEnd, turnOnUniqueChecksAtEnd, restoreBulkBufferSizeAtEnd, restoreKeyBufferSizeAtEnd);

// see how to group commands to be sent to the server
    std::size_t ncommands = 1;

    it = options.find("LOTE");

    if(it != itend)
      ncommands = boost::lexical_cast<std::size_t>(it->second);
        
    if(ncommands > 1)
      execMultiple(istr, ncommands);
    else //if(lote = 1)
      execSingle(istr);
  }
  catch(...)
  {
    restoreServer(tblName, turnOnFkCheckAtEnd, enableIndexesAtEnd, turnOnUniqueChecksAtEnd, restoreBulkBufferSizeAtEnd, restoreKeyBufferSizeAtEnd);
    throw;
  }

  restoreServer(tblName, turnOnFkCheckAtEnd, enableIndexesAtEnd, turnOnUniqueChecksAtEnd, restoreBulkBufferSizeAtEnd, restoreKeyBufferSizeAtEnd);
}

void te::mysql::BatchExecutor::execMultiLineCommand(std::istream& istr, const std::map<std::string, std::string>& options)
{
  std::map<std::string, std::string>::const_iterator it = options.find("TABLE_NAME");
  std::map<std::string, std::string>::const_iterator itend = options.end();

// get table name if any
  std::string  tblName = it != itend ? it->second : std::string("");

// prepare server and keep parameters to restore later
  bool turnOnFkCheckAtEnd = false;
  bool enableIndexesAtEnd = false;
  bool turnOnUniqueChecksAtEnd = false;
  std::string restoreBulkBufferSizeAtEnd;
  std::string restoreKeyBufferSizeAtEnd;

  std::streamsize buffsize = TE_MYSQL_BUFFER_SCRIPT_SIZE;
  
  char* buff = (char*)malloc(TE_MYSQL_BUFFER_SCRIPT_SIZE);    // used to buffer a line of SQL command

  std::auto_ptr<sql::Statement> stmt(m_t->getConnection()->getConn()->createStatement());

  try
  {
    prepareServer(options, tblName, turnOnFkCheckAtEnd, enableIndexesAtEnd, turnOnUniqueChecksAtEnd, restoreBulkBufferSizeAtEnd, restoreKeyBufferSizeAtEnd);

    std::string sql;

    while(!istr.eof())
    {
      std::streamsize readbytes = 0;

      do
      {
        istr.getline(buff + readbytes, static_cast<std::streamsize>(buffsize - readbytes));

        if(istr.flags() == std::ios::failbit)
        {
          buffsize *= 2;
          buff = (char*)realloc(buff, static_cast<std::size_t>(buffsize));
        
          readbytes += istr.gcount();
        }
        else
        {
          readbytes += istr.gcount();
          break;
        }
      }while(true);

      //if(readbytes == 0)
      //  continue;

      if(IsAllWhitespace(buff))
        continue;

      std::string sqline = buff;

      boost::trim(sqline);

      if(sqline.find("--") == 0)
        continue;

      sql += sqline;

      if(sqline.find(";") == std::string::npos)
        continue;

      stmt->execute(sql);

      sql = "";
    }

  }
  catch(...)
  {
    free(buff);
    restoreServer(tblName, turnOnFkCheckAtEnd, enableIndexesAtEnd, turnOnUniqueChecksAtEnd, restoreBulkBufferSizeAtEnd, restoreKeyBufferSizeAtEnd);
    throw;
  }

  free(buff);
  restoreServer(tblName, turnOnFkCheckAtEnd, enableIndexesAtEnd, turnOnUniqueChecksAtEnd, restoreBulkBufferSizeAtEnd, restoreKeyBufferSizeAtEnd);
}

