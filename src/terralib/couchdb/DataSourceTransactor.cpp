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
  \file terralib/couchdb/DataSourceTransactor.cpp

  \brief An implementation of DataSourceTransactor class for the TerraLib CouchDB Data Access driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../json/DataSetReader.h"
#include "DataSource.h"
//#include "DataSourceCatalogLoader.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "HttpClient.h"

// STL
#include <cassert>

// Boost
#include <boost/format.hpp>

// JSONCpp
#include <json/json.h>

te::couchdb::DataSourceTransactor::DataSourceTransactor(DataSource* parent)
  : m_ds(parent)
{
}

te::couchdb::DataSourceTransactor::~DataSourceTransactor()
{
}

void te::couchdb::DataSourceTransactor::begin()
{
  throw Exception(TR_COUCHDB("Not implemented yet!"));
}

void te::couchdb::DataSourceTransactor::commit()
{
  throw Exception(TR_COUCHDB("Not implemented yet!"));
}

void te::couchdb::DataSourceTransactor::rollBack()
{
  throw Exception(TR_COUCHDB("Not implemented yet!"));
}

bool te::couchdb::DataSourceTransactor::isInTransaction() const
{
  throw Exception(TR_COUCHDB("Not implemented yet!"));
}

te::da::DataSet* te::couchdb::DataSourceTransactor::getDataSet(const std::string& name, 
                                                               te::common::TraverseType /*travType*/, 
                                                               te::common::AccessPolicy /*rwRole*/)
{
  std::string query = m_ds->getServerURI() + "/" + m_ds->getDBName() + "/" + name;

  HttpClient hc;

  std::string response;

  std::string mime;

  hc.get(query, response, mime);

  Json::Value doc;

  Json::Reader reader;

  bool parsingSuccessful = reader.parse(response, doc);

  if(!parsingSuccessful)
    throw Exception((boost::format(TR_COUCHDB("Failed to parse a JSON file returned by CouchDB Server. Error: %1%")) % reader.getFormatedErrorMessages()).str());

  if(doc.isMember("error"))
    throw Exception((boost::format(TR_COUCHDB("Failed to retrieve dataset %1%. Error: %2%. Reason: %3%.")) % name % doc["error"].asString() % doc["reason"].asString()).str());

  return te::json::DataSetReader::read(name, doc);
}

te::da::DataSet* te::couchdb::DataSourceTransactor::getDataSet(const std::string& /*name*/,
                                                               const te::dt::Property* /*p*/,
                                                               const te::gm::Envelope* /*e*/,
                                                               te::gm::SpatialRelation /*r*/,
                                                               te::common::TraverseType /*travType*/, 
                                                               te::common::AccessPolicy /*rwRole*/)
{
  throw Exception(TR_COUCHDB("Not implemented yet!"));
}

te::da::DataSet* te::couchdb::DataSourceTransactor::getDataSet(const std::string& /*name*/,
                                                               const te::dt::Property* /*p*/,
                                                               const te::gm::Geometry* /*g*/,
                                                               te::gm::SpatialRelation /*r*/,
                                                               te::common::TraverseType /*travType*/,
                                                               te::common::AccessPolicy /*rwRole*/)
{
  throw Exception(TR_COUCHDB("Not implemented yet!"));
}

te::da::DataSet* te::couchdb::DataSourceTransactor::query(const te::da::Select& /*q*/,
                                                          te::common::TraverseType /*travType*/,
                                                          te::common::AccessPolicy /*rwRole*/)
{
  throw Exception(TR_COUCHDB("Not implemented yet!"));
}

te::da::DataSet* te::couchdb::DataSourceTransactor::query(const std::string& /*query*/, 
                                                          te::common::TraverseType /*travType*/, 
                                                          te::common::AccessPolicy /*rwRole*/)
{
  throw Exception(TR_COUCHDB("Not implemented yet!"));
}

void te::couchdb::DataSourceTransactor::execute(const te::da::Query& /*command*/)
{
  throw Exception(TR_COUCHDB("Not implemented yet!"));
}

void te::couchdb::DataSourceTransactor::execute(const std::string& /*command*/)
{
  throw Exception(TR_COUCHDB("Not implemented yet!"));
}

te::da::PreparedQuery* te::couchdb::DataSourceTransactor::getPrepared(const std::string& /*qName*/)
{
  throw Exception(TR_COUCHDB("Not implemented yet!"));
}

te::da::BatchExecutor* te::couchdb::DataSourceTransactor::getBatchExecutor()
{
  throw Exception(TR_COUCHDB("Not implemented yet!"));
}

te::da::DataSourceCatalogLoader* te::couchdb::DataSourceTransactor::getCatalogLoader()
{
  throw Exception(TR_COUCHDB("Not implemented yet!"));
}

te::da::DataSetTypePersistence* te::couchdb::DataSourceTransactor::getDataSetTypePersistence()
{
  throw Exception(TR_COUCHDB("Not implemented yet!"));
}

te::da::DataSetPersistence* te::couchdb::DataSourceTransactor::getDataSetPersistence()
{
  throw Exception(TR_COUCHDB("Not implemented yet!"));
}

void te::couchdb::DataSourceTransactor::cancel()
{
  throw Exception(TR_COUCHDB("Not implemented yet!"));
}

boost::int64_t te::couchdb::DataSourceTransactor::getLastInsertId()
{
  throw Exception(TR_COUCHDB("Not implemented yet!"));
}

te::da::DataSource* te::couchdb::DataSourceTransactor::getDataSource() const
{
  return m_ds;
}

