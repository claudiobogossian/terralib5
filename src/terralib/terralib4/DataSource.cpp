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
#include "DataSource.h"
#include "Config.h"
#include "Utils.h"

// TerraLib 4.x
#include <terralib/kernel/TeDatabase.h>
#include <terralib/kernel/TeDatabaseFactory.h>
#include <terralib/kernel/TeDatabaseFactoryParams.h>

class terralib4::DataSource::Impl
{
  public:

    Impl();

    ~Impl();

    std::string getType() const;

    const std::map<std::string, std::string>& getConnectionInfo() const;

    void setConnectionInfo(const std::map<std::string, std::string>& connInfo);

    std::auto_ptr<te::da::DataSourceTransactor> getTransactor();

    void open();

    void close();

    bool isOpened() const;

    bool isValid() const;

    const te::da::DataSourceCapabilities& getCapabilities() const;

    const te::da::SQLDialect* getDialect() const;

    void create(const std::map<std::string, std::string>& dsInfo);

    void drop(const std::map<std::string, std::string>& dsInfo);

    bool exists(const std::map<std::string, std::string>& dsInfo);

    std::vector<std::string> getDataSourceNames(const std::map<std::string, std::string>& dsInfo);

    std::vector<std::string> getEncodings(const std::map<std::string, std::string>& dsInfo);

  private:

    TeDatabase* m_db;
    std::map<std::string, std::string> m_dbInfo;
};

terralib4::DataSource::Impl::Impl()
  : m_db(0)
{
}

terralib4::DataSource::Impl::~Impl()
{
  delete m_db;
}

std::string terralib4::DataSource::Impl::getType() const
{
  return TERRALIB4_DRIVER_IDENTIFIER;
}

const std::map<std::string, std::string>& terralib4::DataSource::Impl::getConnectionInfo() const
{
  return m_dbInfo;
}

void terralib4::DataSource::Impl::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_dbInfo = connInfo;
}

std::auto_ptr<te::da::DataSourceTransactor> terralib4::DataSource::Impl::getTransactor()
{
  throw;
}

void terralib4::DataSource::Impl::open()
{
  close();

  std::auto_ptr<TeDatabaseFactoryParams> fdbparams(Convert2T4DatabaseParams(m_dbInfo));

  m_db = TeDatabaseFactory::make(*fdbparams);

}

void terralib4::DataSource::Impl::close()
{
  delete m_db;

  m_db = 0;
}

bool terralib4::DataSource::Impl::isOpened() const
{
  return m_db != 0;
}

bool terralib4::DataSource::Impl::isValid() const
{
  return m_db != 0;
}

const te::da::DataSourceCapabilities& terralib4::DataSource::Impl::getCapabilities() const
{
  throw;
}

const te::da::SQLDialect* terralib4::DataSource::Impl::getDialect() const
{
  throw;
}

void terralib4::DataSource::Impl::create(const std::map<std::string, std::string>& dsInfo)
{
  TeDatabase* db = 0;

  std::auto_ptr<TeDatabaseFactoryParams> params(terralib4::Convert2T4DatabaseParams(dsInfo));

  db = TeDatabaseFactory::make(*params.get());
  
  db->newDatabase(params->database_, params->user_, params->password_, params->host_, params->port_);

}

void terralib4::DataSource::Impl::drop(const std::map<std::string, std::string>& dsInfo)
{
  throw;
}

bool terralib4::DataSource::Impl::exists(const std::map<std::string, std::string>& dsInfo)
{
  throw;
}

std::vector<std::string> terralib4::DataSource::Impl::getDataSourceNames(const std::map<std::string, std::string>& dsInfo)
{
  throw;
}

std::vector<std::string> terralib4::DataSource::Impl::getEncodings(const std::map<std::string, std::string>& dsInfo)
{
  throw;
}

//-----

terralib4::DataSource::DataSource()
  : m_pImpl(0)
{
  m_pImpl = new Impl;
}

terralib4::DataSource::~DataSource()
{
  delete m_pImpl;
}

std::string terralib4::DataSource::getType() const
{
  return m_pImpl->getType();
}

const std::map<std::string, std::string>& terralib4::DataSource::getConnectionInfo() const
{
  return m_pImpl->getConnectionInfo();
}

void terralib4::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_pImpl->setConnectionInfo(connInfo);
}

std::auto_ptr<te::da::DataSourceTransactor> terralib4::DataSource::getTransactor()
{
  return m_pImpl->getTransactor();
}

void terralib4::DataSource::open()
{
  m_pImpl->open();
}

void terralib4::DataSource::close()
{
  m_pImpl->close();
}

bool terralib4::DataSource::isOpened() const
{
  return m_pImpl->isOpened();
}

bool terralib4::DataSource::isValid() const
{
  return m_pImpl->isValid();
}

const te::da::DataSourceCapabilities& terralib4::DataSource::getCapabilities() const
{
  return m_pImpl->getCapabilities();
}

const te::da::SQLDialect* terralib4::DataSource::getDialect() const
{
  return m_pImpl->getDialect();
}

void terralib4::DataSource::create(const std::map<std::string, std::string>& dsInfo)
{
  m_pImpl->create(dsInfo);
}

void terralib4::DataSource::drop(const std::map<std::string, std::string>& dsInfo)
{
  m_pImpl->drop(dsInfo);
}

bool terralib4::DataSource::exists(const std::map<std::string, std::string>& dsInfo)
{
  return m_pImpl->exists(dsInfo);
}

std::vector<std::string> terralib4::DataSource::getDataSourceNames(const std::map<std::string, std::string>& dsInfo)
{
  return m_pImpl->getDataSourceNames(dsInfo);
}

std::vector<std::string> terralib4::DataSource::getEncodings(const std::map<std::string, std::string>& dsInfo)
{
  return m_pImpl->getEncodings(dsInfo);
}

