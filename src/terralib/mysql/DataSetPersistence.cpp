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
  \file terralib/mysql/DataSetPersistence.cpp

  \brief DataSetPersistence implementation for MySQL data access driver.
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
#include "Connection.h"
#include "DataSetPersistence.h"
#include "DataSetTypePersistence.h"
#include "DataSourceCatalogLoader.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "PreparedQuery.h"
#include "ScopedLock.h"
#include "Utils.h"

// STL
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>

// Boost
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// MySQL
#include <driver/mysql_public_iface.h>

te::mysql::DataSetPersistence::DataSetPersistence(DataSourceTransactor* parent)
  : m_t(parent)
{
}

te::mysql::DataSetPersistence::~DataSetPersistence()
{
}

void te::mysql::DataSetPersistence::remove(const std::string& /*datasetName*/, const te::da::ObjectIdSet* /*oids*/)
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

void te::mysql::DataSetPersistence::add(const std::string& datasetName, te::da::DataSet* d, const std::map<std::string, std::string>& /*options*/, std::size_t limit)
{
  if(limit == 0)
    limit = std::string::npos;

// find out the database engine used by the table in order to lock or start a transaction
  std::auto_ptr<DataSourceCatalogLoader> cloader(m_t->getMyCatalogLoader());

  std::string tengine = cloader->getTableEngine(datasetName);

  std::auto_ptr<te::da::ScopedTransaction> st;
  std::auto_ptr<ScopedLock> sl;

  if(tengine == "MyISAM")
    sl.reset(new ScopedLock(*m_t, datasetName));
  else if(tengine == "InnoDB")
    st.reset(new te::da::ScopedTransaction(*m_t));

  std::string sql  = "INSERT INTO ";
              sql += datasetName;
              sql += te::da::GetSQLValueNames(d);
              sql += " VALUES";
              sql += GetSQLBindValues(d);

  std::auto_ptr<PreparedQuery> pq(static_cast<PreparedQuery*>(m_t->getPrepared()));

  pq->prepare(sql, d->getNumProperties());

  try
  {
    std::size_t nProcessedRows = 0;

    do
    {
      pq->bind(d);
      pq->execute();

      ++nProcessedRows;

    }while(d->moveNext() && (nProcessedRows != limit));

    if(st.get())
      st->commit();
  }
  catch(const sql::SQLException &e)
  {
    throw Exception((boost::format(TR_MYSQL("Could not add dataset to the table due to the following error: %1%")) % e.what()).str());
  }
  catch(...)
  {
    throw Exception(TR_MYSQL("Unknown exception when adding a dataset to a table!"));
  }

  
}

void te::mysql::DataSetPersistence::update(const std::string& /*datasetName*/,
                                          te::da::DataSet* /*dataset*/,
                                          const std::vector<std::size_t>& /*properties*/,
                                          const te::da::ObjectIdSet* /*oids*/,
                                          const std::map<std::string, std::string>& /*options*/,
                                          std::size_t /*limit*/)
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

