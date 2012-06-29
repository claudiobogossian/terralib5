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
  \file terralib/mongodb/DataSourceTransactor.cpp

  \brief An implementation of DataSourceTransactor class for the TerraLib MongoDB Data Access driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "DataSource.h"
#include "DataSourceCatalogLoader.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "FwDataSet.h"

// STL
#include <memory>

// MongoDB
#include <mongo/client/dbclient.h>

te::mongodb::DataSourceTransactor::DataSourceTransactor(DataSource* parent)
  : m_ds(parent)
{
}

te::mongodb::DataSourceTransactor::~DataSourceTransactor()
{
}

void te::mongodb::DataSourceTransactor::begin()
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::DataSourceTransactor::commit()
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::DataSourceTransactor::rollBack()
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

bool te::mongodb::DataSourceTransactor::isInTransaction() const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::da::DataSet* te::mongodb::DataSourceTransactor::getDataSet(const std::string& name, 
                                                               te::common::TraverseType travType, 
                                                               te::common::AccessPolicy rwRole)
{
  std::string dataset_name = m_ds->getDB() + "." + name;

  std::auto_ptr<mongo::DBClientCursor> cursor(m_ds->getConn()->query(dataset_name, mongo::BSONObj()));

  return new FwDataSet(cursor.release());
}

te::da::DataSet* te::mongodb::DataSourceTransactor::getDataSet(const std::string& name,
                                                                const te::dt::Property* p,
                                                                const te::gm::Envelope* e,
                                                                te::gm::SpatialRelation r,
                                                                te::common::TraverseType travType, 
                                                                te::common::AccessPolicy rwRole)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::da::DataSet* te::mongodb::DataSourceTransactor::getDataSet(const std::string& name,
                                                                const te::dt::Property* p,
                                                                const te::gm::Geometry* g,
                                                                te::gm::SpatialRelation r,
                                                                te::common::TraverseType travType, 
                                                                te::common::AccessPolicy rwRole)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::da::DataSet* te::mongodb::DataSourceTransactor::query(const te::da::Select& q, 
                                                           te::common::TraverseType travType, 
                                                           te::common::AccessPolicy rwRole)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::da::DataSet* te::mongodb::DataSourceTransactor::query(const std::string& query, 
                                                           te::common::TraverseType travType, 
                                                           te::common::AccessPolicy rwRole)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::DataSourceTransactor::execute(const te::da::Query& command)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::DataSourceTransactor::execute(const std::string& command)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::da::PreparedQuery* te::mongodb::DataSourceTransactor::getPrepared(const std::string& qName)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::da::BatchExecutor* te::mongodb::DataSourceTransactor::getBatchExecutor()
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::da::DataSourceCatalogLoader* te::mongodb::DataSourceTransactor::getCatalogLoader()
{
  return new DataSourceCatalogLoader(this);
}

te::da::DataSetTypePersistence* te::mongodb::DataSourceTransactor::getDataSetTypePersistence()
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::da::DataSetPersistence* te::mongodb::DataSourceTransactor::getDataSetPersistence()
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::DataSourceTransactor::cancel()
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

boost::int64_t te::mongodb::DataSourceTransactor::getLastInsertId()
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::da::DataSource* te::mongodb::DataSourceTransactor::getDataSource() const
{
  return m_ds;
}

