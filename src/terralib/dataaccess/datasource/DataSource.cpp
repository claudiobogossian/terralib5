/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/datasource/DataSource.cpp

  \brief An abstraction for a data provider (like a DBMS, Web Service or a regular file).
*/

// TerraLib
#include "../../common/StringUtils.h"
#include "../../common/Translator.h"
#include "../Exception.h"
#include "DataSource.h"
#include "DataSourceFactory.h"
#include "DataSourceManager.h"

// STL
#include <cassert>
#include <memory>

const std::string& te::da::DataSource::getId() const
{
  return m_id;
}

void te::da::DataSource::setId(const std::string& id)
{
  m_id = id;
}

std::string te::da::DataSource::getConnectionStr() const
{
  return std::string("");
}

void te::da::DataSource::setConnectionStr(const std::string& connInfo)
{
  std::map<std::string, std::string> connInfoMap;

  te::common::ExtractKVP(connInfo, connInfoMap, "&", "=", false);

  setConnectionInfo(connInfoMap);
}

void te::da::DataSource::open(const std::map<std::string, std::string>& connInfo)
{
  setConnectionInfo(connInfo);
  open();
}

void te::da::DataSource::open(const std::string& connInfo)
{
  setConnectionStr(connInfo);
  open();
}

te::da::DataSource* te::da::DataSource::create(const std::string& dsType, const std::map<std::string, std::string>& dsInfo)
{
  std::auto_ptr<DataSource> ds(DataSourceFactory::make(dsType));

  if(ds.get() == 0)
    throw Exception(TR_DATAACCESS("Could not find the appropriate factory in order to create a data source instance!"));

  ds->create(dsInfo);

  return ds.release();
}

void te::da::DataSource::drop(const std::string& dsType, const std::map<std::string, std::string>& dsInfo)
{
  std::auto_ptr<DataSource> ds(DataSourceFactory::make(dsType));

  if(ds.get() == 0)
    throw Exception(TR_DATAACCESS("Could not find the appropriate factory in order to create a data source instance!"));

  ds->drop(dsInfo);
}

bool te::da::DataSource::exists(const std::string& dsType, const std::map<std::string, std::string>& dsInfo)
{
  std::auto_ptr<DataSource> ds(DataSourceFactory::make(dsType));

  if(ds.get() == 0)
    throw Exception(TR_DATAACCESS("Could not find the appropriate factory in order to create a data source instance!"));

  return ds->exists(dsInfo);
}

bool te::da::DataSource::operator<(const DataSource& rhs) const
{
  return getId() < rhs.getId();
}

std::vector<std::string> te::da::DataSource::getDataSources(const std::string& dsType, const std::map<std::string, std::string>& info)
{
  std::auto_ptr<DataSource> ds(DataSourceFactory::make(dsType));

  if(ds.get() == 0)
    throw Exception(TR_DATAACCESS("Could not find the appropriate factory in order to create a data source instance!"));

  return ds->getDataSources(info);
}

std::vector<std::string> te::da::DataSource::getEncodings(const std::string& dsType, const std::map<std::string, std::string>& info)
{
  std::auto_ptr<DataSource> ds(DataSourceFactory::make(dsType));

  if(ds.get() == 0)
    throw Exception(TR_DATAACCESS("Could not find the appropriate factory in order to create a data source instance!"));

  return ds->getEncodings(info);
}