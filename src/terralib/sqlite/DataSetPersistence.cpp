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
  \file terralib/sqlite/DataSetPersistence.cpp

  \brief DataSetPersistence implementation for the TerraLib SQLite Data Access Driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../dataaccess/datasource/ScopedTransaction.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/Property.h"
#include "../datatype/Utils.h"
#include "DataSetPersistence.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "PreparedQuery.h"
#include "Utils.h"

// STL
#include <memory>

// Boost
#include <boost/format.hpp>

// SQLite
#include <sqlite3.h>

te::sqlite::DataSetPersistence::DataSetPersistence(DataSourceTransactor* parent)
  : m_t(parent)
{
}

te::sqlite::DataSetPersistence::~DataSetPersistence()
{
}

void te::sqlite::DataSetPersistence::remove(const std::string& /*datasetName*/, const te::da::ObjectIdSet* /*oids*/)
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

void te::sqlite::DataSetPersistence::add(const std::string& datasetName, te::da::DataSet* d, const std::map<std::string, std::string>& /*options*/, std::size_t limit)
{
  if(limit == 0)
    limit = std::string::npos;

  std::string sql  = "INSERT INTO ";
              sql += datasetName;
              sql += te::da::GetSQLValueNames(d);
              sql += " VALUES";
              sql += GetSQLBindValues(d);

  te::da::ScopedTransaction st(*m_t);

  std::auto_ptr<PreparedQuery> pq(m_t->getLitePrepared());

  pq->prepare(sql);

  std::size_t nProcessedRows = 0;

  do
  {
    pq->bind(d);
    pq->execute();

    ++nProcessedRows;

  }while(d->moveNext() && (nProcessedRows != limit));

  st.commit();
}

void te::sqlite::DataSetPersistence::update(const std::string& /*datasetName*/,
                                            te::da::DataSet* /*dataset*/,
                                            const std::vector<std::size_t>& /*properties*/,
                                            const te::da::ObjectIdSet* /*oids*/,
                                            const std::map<std::string, std::string>& /*options*/,
                                            std::size_t /*limit*/)
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

