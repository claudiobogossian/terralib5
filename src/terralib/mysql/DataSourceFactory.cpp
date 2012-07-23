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
  \file terralib/mysql/DataSourceFactory.cpp

  \brief This is the concrete factory for MySQL data sources.
*/

// TerraLib
#include "DataSource.h"
#include "DataSourceFactory.h"
#include "Globals.h"

// BOOST
#include <boost/assign.hpp>

te::mysql::DataSourceFactory* te::mysql::DataSourceFactory::sm_factory(0);

const std::string& te::mysql::DataSourceFactory::getType() const
{
  return Globals::sm_driverIdentifier;
}

void te::mysql::DataSourceFactory::getConnectionParameters(std::vector<std::string>& params) const
{
  boost::assign::push_back(params)("MY_HOST_NAME")("MY_USER_NAME")("MY_PASSWORD")
                                  ("MY_PORT")("MY_SOCKET")("MY_PIPE")("MY_CHARACTERSET_RESULTS")
                                  ("MY_SCHEMA")("MY_SSL_KEY")("MY_SSL_CERT")("MY_SSL_CA")
                                  ("MY_SSL_CA_PATH")("MY_SSL_CIPHER")("MY_DEFAULT_STATEMENT_RESULT_TYPE")
                                  ("MY_DEFAULT_PREPARED_STATEMENT_RESULT_TYPE")("MY_CLIENT_COMPRESS")
                                  ("MY_CLIENT_FOUND_ROWS")("MY_CLIENT_IGNORE_SIGPIPE")("MY_CLIENT_IGNORE_SPACE")
                                  ("MY_CLIENT_INTERACTIVE")("MY_CLIENT_LOCAL_FILES")("MY_CLIENT_MULTI_RESULTS")("MY_CLIENT_MULTI_STATEMENTS")
                                  ("MY_CLIENT_NO_SCHEMA")("MY_OPT_CONNECT_TIMEOUT")("MY_OPT_READ_TIMEOUT")("MY_OPT_WRITE_TIMEOUT")
                                  ("MY_OPT_RECONNECT")("MY_OPT_CHARSET_NAME")("MY_OPT_REPORT_DATA_TRUNCATION")("MY_INITIAL_POOL_SIZE")
                                  ("MY_MIN_POOL_SIZE")("MY_MAX_POOL_SIZE")("MY_MAX_IDLE_TIME")("MY_CREATE_TERRALIB_RASTER_METADATA_TABLES")
                                  ("MY_HIDE_RASTER_TABLES")("MY_HIDE_METADATA_TABLES")("MY_HIDE_TABLES")("MY_DEFAULT_ENGINE_FOR_SPATIAL_TABLES");
}

te::mysql::DataSourceFactory::~DataSourceFactory()
{
}

te::da::DataSource* te::mysql::DataSourceFactory::build()
{
  return new DataSource;
}

te::mysql::DataSourceFactory::DataSourceFactory()
  : te::da::DataSourceFactory(TE_MYSQL_DRIVER_IDENTIFIER)
{
}

void te::mysql::DataSourceFactory::initialize()
{
  finalize();
  sm_factory = new DataSourceFactory;
}

void te::mysql::DataSourceFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

