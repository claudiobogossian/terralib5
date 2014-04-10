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
#include "ThemeInfo.h"
#include "Transactor.h"
#include "Utils.h"

// TerraLib 4.x
#include <terralib/kernel/TeDatabase.h>
#include <terralib/kernel/TeDatabaseFactory.h>
#include <terralib/kernel/TeDatabaseFactoryParams.h>
#include <terralib/kernel/TeDBConnectionsPool.h>
#include <terralib/kernel/TeDefines.h>
#include <terralib/kernel/TeTheme.h>
#include <terralib/utils/TeUpdateDBVersion.h>


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

  std::string dbInfo = m_dbInfo.at("T4_DRIVER");
  std::string auxDbName = m_dbInfo.at("T4_DB_NAME");
  std::string hostName = "";
  std::string userName = "";
  std::string password = "";
  int portNumber = -1;


  m_db = TeDBConnectionsPool::instance().getDatabase(dbInfo, auxDbName, hostName, userName, 
                                                          password, portNumber);

  if(!m_db->isConnected())
  {
    if(!m_db->connect(hostName, userName, password, auxDbName, portNumber))
      throw te::da::Exception(TR_TERRALIB4("Could not connect to informed database!"));
  }

  string DBver;
  if(needUpdateDB(m_db, DBver))
  {
    std::string dbVersion = TeDBVERSION;

    if(isLowerVersion(dbVersion, DBver))
    {
      close();

      throw te::da::Exception(TR_TERRALIB4("Cannot connect to database because the version of Terraview is lower than the version of the database!"));
    }

    close();

    throw te::da::Exception(TR_TERRALIB4("The database must be converted to the model ") + dbVersion + "! \n");
  }

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

std::vector<te::common::CharEncoding> terralib4::DataSource::getEncodings(const std::map<std::string, std::string>& dsInfo)
{
  /*std::auto_ptr<TeDatabaseFactoryParams> params(terralib4::Convert2T4DatabaseParams(dsInfo));

  std::auto_ptr<TeDatabase> db(TeDatabaseFactory::make(*params.get()));

  std::vector<std::string> encodings;

  db->getEncodingList(encodings);

  return encodings;*/

  return std::vector<te::common::CharEncoding>();
}

TeDatabase* terralib4::DataSource::getTerralib4Db()
{
  return m_db;
}

std::vector<std::string> terralib4::DataSource::getTL4Layers()
{
  std::vector<std::string> layers;

  std::auto_ptr<te::da::DataSourceTransactor> t = getTransactor();
  
  terralib4::Transactor* t4t = dynamic_cast<terralib4::Transactor*>(t.get());
  layers = t4t->getTL4Layers();

  return layers;
}

std::vector<std::string> terralib4::DataSource::getTL4Tables()
{
  std::vector<std::string> tables;

  std::auto_ptr<te::da::DataSourceTransactor> t = getTransactor();
  
  terralib4::Transactor* t4t = dynamic_cast<terralib4::Transactor*>(t.get());
  tables = t4t->getTL4Tables();

  return tables;
}

std::vector<::terralib4::ThemeInfo> terralib4::DataSource::getTL4Themes()
{
  std::vector<::terralib4::ThemeInfo> themes;

  std::auto_ptr<te::da::DataSourceTransactor> t = getTransactor();
  
  terralib4::Transactor* t4t = dynamic_cast<terralib4::Transactor*>(t.get());
  themes = t4t->getTL4Themes();

  return themes;
}

TeTheme* terralib4::DataSource::getTL4Theme(const ::terralib4::ThemeInfo theme)
{
  TeTheme* tl4Theme = 0;

  std::auto_ptr<te::da::DataSourceTransactor> t = getTransactor();
  
  terralib4::Transactor* t4t = dynamic_cast<terralib4::Transactor*>(t.get());
  tl4Theme = t4t->getTL4Theme(theme);

  return tl4Theme;
}