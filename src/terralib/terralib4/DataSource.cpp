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

// TerraLib 5
#include "../common/Translator.h"
#include "Config.h"
#include "DataSource.h"
#include "Exception.h"
#include "Transactor.h"
#include "Utils.h"

// TerraLib 4.x
#include <terralib/kernel/TeDatabase.h>
#include <terralib/kernel/TeDatabaseFactory.h>
#include <terralib/kernel/TeDatabaseFactoryParams.h>

te::da::DataSourceCapabilities terralib4::DataSource::sm_capabilities;
te::da::SQLDialect* terralib4::DataSource::sm_dialect(0);

terralib4::DataSource::DataSource()
  : m_db(0)
{
}

terralib4::DataSource::~DataSource()
{
}

std::string terralib4::DataSource::getType() const
{
  return TERRALIB4_DRIVER_IDENTIFIER;
}

const std::map<std::string, std::string>& terralib4::DataSource::getConnectionInfo() const
{
  return m_dbInfo;
}

void terralib4::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_dbInfo = connInfo;
}

std::auto_ptr<te::da::DataSourceTransactor> terralib4::DataSource::getTransactor()
{
  std::auto_ptr<te::da::DataSourceTransactor> t(new Transactor(this, m_db));
  return t;
}

void terralib4::DataSource::open()
{
  close();

  std::auto_ptr<TeDatabaseFactoryParams> fdbparams(terralib4::Convert2T4DatabaseParams(m_dbInfo));

  m_db = TeDatabaseFactory::make(*fdbparams);
}

void terralib4::DataSource::close()
{
  delete m_db;

  m_db = 0;
}

bool terralib4::DataSource::isOpened() const
{
  return m_db != 0;
}

bool terralib4::DataSource::isValid() const
{
  return m_db != 0;
}

const te::da::DataSourceCapabilities& terralib4::DataSource::getCapabilities() const
{
  return sm_capabilities;
}

const te::da::SQLDialect* terralib4::DataSource::getDialect() const
{
  return sm_dialect;
}

void terralib4::DataSource::create(const std::map<std::string, std::string>& dsInfo)
{
  throw Exception(TR_TERRALIB4("This driver is read-only!"));
}

void terralib4::DataSource::drop(const std::map<std::string, std::string>& dsInfo)
{
  throw Exception(TR_TERRALIB4("This driver is read-only!"));
}

bool terralib4::DataSource::exists(const std::map<std::string, std::string>& dsInfo)
{
  std::vector<string> dbnames = getDataSourceNames(dsInfo);

  return std::find(dbnames.begin(), dbnames.end(), dsInfo.at("T4_DB_NAME")) != dbnames.end();
}

std::vector<std::string> terralib4::DataSource::getDataSourceNames(const std::map<std::string, std::string>& dsInfo)
{
  std::auto_ptr<TeDatabaseFactoryParams> params(terralib4::Convert2T4DatabaseParams(dsInfo));

  std::auto_ptr<TeDatabase> db(TeDatabaseFactory::make(*params.get()));

  std::vector<std::string> dbnames;

  db->showDatabases(params->host_, params->user_, params->password_, dbnames, params->port_);

  return dbnames;
}

std::vector<std::string> terralib4::DataSource::getEncodings(const std::map<std::string, std::string>& dsInfo)
{
  std::auto_ptr<TeDatabaseFactoryParams> params(terralib4::Convert2T4DatabaseParams(dsInfo));

  std::auto_ptr<TeDatabase> db(TeDatabaseFactory::make(*params.get()));

  std::vector<std::string> encodings;

  db->getEncodingList(encodings);

  return encodings;
}

TeDatabase* terralib4::DataSource::getTerralib4Db()
{
  return m_db;
}
