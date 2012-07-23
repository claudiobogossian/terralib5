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
  \file DataSourceCatalogLoader.cpp

  \brief DataSourceCatalogLoader class implementation for MongoDB.
*/

// TerraLib
#include "../common/Translator.h"
#include "DataSource.h"
#include "DataSourceCatalogLoader.h"
#include "DataSourceTransactor.h"
#include "Exception.h"

// STL
#include <algorithm>
#include <iterator>

// Boost
#include <boost/format.hpp>

// MongoDB
#include <mongo/client/dbclient.h>

te::mongodb::DataSourceCatalogLoader::DataSourceCatalogLoader(DataSourceTransactor* parent)
  : m_t(parent)
{
}

te::mongodb::DataSourceCatalogLoader::~DataSourceCatalogLoader()
{
}

void te::mongodb::DataSourceCatalogLoader::getDataSets(boost::ptr_vector<std::string>& datasets)
{
  mongo::DBClientConnection* conn = m_t->getMongoDataSource()->getConn();

  std::list<std::string> names = conn->getCollectionNames(m_t->getMongoDataSource()->getDB());

  for(std::list<std::string>::iterator it = names.begin(); it != names.end(); ++it)
    datasets.push_back(new std::string(*it));
}

te::da::DataSetType* te::mongodb::DataSourceCatalogLoader::getDataSetType(const std::string& datasetName,
                                                                           const bool full)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::DataSourceCatalogLoader::getProperties(te::da::DataSetType* /*dt*/)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::dt::Property* te::mongodb::DataSourceCatalogLoader::getProperty(const std::string& /*datasetName*/, const std::string& /*propertyName*/)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::DataSourceCatalogLoader::getPrimaryKey(te::da::DataSetType* dt)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::DataSourceCatalogLoader::getUniqueKeys(te::da::DataSetType* dt)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::DataSourceCatalogLoader::getForeignKeys(te::da::DataSetType* dt, std::vector<std::string>& fkNames, std::vector<std::string>& rdts)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::da::ForeignKey* te::mongodb::DataSourceCatalogLoader::getForeignKey(const std::string& fkName, te::da::DataSetType* dt, te::da::DataSetType* rdt)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::DataSourceCatalogLoader::getIndexes(te::da::DataSetType* dt)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::DataSourceCatalogLoader::getCheckConstraints(te::da::DataSetType* dt)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::DataSourceCatalogLoader::getSequences(std::vector<std::string*>& sequences)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::da::Sequence* te::mongodb::DataSourceCatalogLoader::getSequence(const std::string& seqName)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::gm::Envelope* te::mongodb::DataSourceCatalogLoader::getExtent(const te::gm::GeometryProperty* gp)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::DataSourceCatalogLoader::loadCatalog(const bool full)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

bool te::mongodb::DataSourceCatalogLoader::hasDataSets()
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

bool te::mongodb::DataSourceCatalogLoader::datasetExists(const std::string& name)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

bool te::mongodb::DataSourceCatalogLoader::primarykeyExists(const std::string& name)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

bool te::mongodb::DataSourceCatalogLoader::uniquekeyExists(const std::string& name)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

bool te::mongodb::DataSourceCatalogLoader::foreignkeyExists(const std::string& name)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

bool te::mongodb::DataSourceCatalogLoader::checkConstraintExists(const std::string& name)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

bool te::mongodb::DataSourceCatalogLoader::indexExists(const std::string& name)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

bool te::mongodb::DataSourceCatalogLoader::sequenceExists(const std::string& name)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::da::DataSourceTransactor* te::mongodb::DataSourceCatalogLoader::getTransactor() const
{
  return m_t;
}
