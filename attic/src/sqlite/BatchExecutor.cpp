/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/sqlite/BatchExecutor.cpp

  \brief Implementation of the BatchExecutor class for the TerraLib SQLite Data Access driver.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../dataaccess/datasource/ScopedTransaction.h"
#include "../dataaccess/query/Query.h"
#include "BatchExecutor.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"
#include "SQLVisitor.h"
#include "Utils.h"

te::sqlite::BatchExecutor::BatchExecutor(DataSourceTransactor* t)
  : m_t(t)
{
}

te::sqlite::BatchExecutor::~BatchExecutor()
{
  te::common::FreeContents(m_commands);
}

void te::sqlite::BatchExecutor::add(te::da::Query* q)
{
  std::auto_ptr<te::da::Query> qq(q);

  std::auto_ptr<std::string> sql(new std::string);

  SQLVisitor visitor(*(m_t->getDataSource()->getDialect()), *sql);
  q->accept(visitor);

  m_commands.push_back(sql.release());
}

void te::sqlite::BatchExecutor::execute()
{
  te::da::ScopedTransaction t(*m_t);

  const std::size_t ncommands = m_commands.size();

  for(std::size_t i = 0; i < ncommands; ++i)
    m_t->execute(*m_commands[i]);

  te::common::FreeContents(m_commands);
  m_commands.clear();
  
  t.commit();
}

void te::sqlite::BatchExecutor::execute(const std::string& uri, const std::map<std::string, std::string>& /*options*/)
{
  te::da::ScopedTransaction t(*m_t);

  ExecuteScript(m_t->getDB(), uri.c_str());

  t.commit();
}

void te::sqlite::BatchExecutor::execute(std::istream& istr, const std::map<std::string, std::string>& /*options*/)
{
  te::da::ScopedTransaction t(*m_t);

  PerformCommands(m_t->getDB(), istr);

  t.commit();
}

te::da::DataSourceTransactor* te::sqlite::BatchExecutor::getTransactor() const
{
  return m_t;
}

void te::sqlite::BatchExecutor::add(std::string* sql)
{
  m_commands.push_back(sql);
}

