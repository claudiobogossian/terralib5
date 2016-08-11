/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/ws/ogc/wms/dataaccess/DataSource.cpp

  \brief Data Source for WS OGC WMS

  \author Emerson Moraes
*/

#include "DataSource.h"

#include "../../../../core/translator/Translator.h"
#include "../../../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../../core/Exception.h"
#include "Transactor.h"


te::da::DataSourceCapabilities te::ws::ogc::wms::da::DataSource::sm_capabilities;


te::ws::ogc::wms::da::DataSource::DataSource() :
  m_isOpened(false)
{
}

te::ws::ogc::wms::da::DataSource::~DataSource()
{
}

std::string te::ws::ogc::wms::da::DataSource::getType() const
{
  return TE_OGC_WMS_DRIVER_IDENTIFIER;
}

const std::map<std::string, std::string>& te::ws::ogc::wms::da::DataSource::getConnectionInfo() const
{
  return m_connectionInfo;
}

void te::ws::ogc::wms::da::DataSource::setConnectionInfo(const std::map<std::string, std::string> &connInfo)
{
  m_connectionInfo = connInfo;
}

std::auto_ptr<te::da::DataSourceTransactor> te::ws::ogc::wms::da::DataSource::getTransactor()
{
  if(!m_isOpened)
  {
    throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The data source is not opened!"));
  }

  return std::auto_ptr<te::da::DataSourceTransactor>(new Transactor(m_wms));
}

void te::ws::ogc::wms::da::DataSource::open()
{
  if(m_isOpened)
    return;

  verifyConnectionInfo();

  try
  {
    m_wms = std::shared_ptr<te::ws::ogc::WMSClient>(new te::ws::ogc::WMSClient(m_connectionInfo.find("USERDATADIR")->second, m_connectionInfo.find("URI")->second, m_connectionInfo.find("VERSION")->second));

    m_wms->updateCapabilities();
  }
  catch(const te::core::Exception& e)
  {
    throw;
  }
  catch(...)
  {
    throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("Unknow error!"));
  }

  m_isOpened = true;
}

void te::ws::ogc::wms::da::DataSource::close()
{
  m_isOpened = false;
}

bool te::ws::ogc::wms::da::DataSource::isOpened() const
{
  return m_isOpened;
}

bool te::ws::ogc::wms::da::DataSource::isValid() const
{
  if(m_isOpened)
    return true;

  try
  {
    verifyConnectionInfo();

    te::ws::ogc::WMSClient wms(m_connectionInfo.find("USERDATADIR")->second, m_connectionInfo.find("URI")->second, m_connectionInfo.find("VERSION")->second);

    wms.updateCapabilities();
  }
  catch(...)
  {
    return false;
  }

  return true;
}

const te::da::DataSourceCapabilities& te::ws::ogc::wms::da::DataSource::getCapabilities() const
{
  return sm_capabilities;
}

void te::ws::ogc::wms::da::DataSource::setCapabilities(const te::da::DataSourceCapabilities &capabilities)
{
  sm_capabilities = capabilities;
}

const te::da::SQLDialect* te::ws::ogc::wms::da::DataSource::getDialect() const
{
  return 0;
}

void te::ws::ogc::wms::da::DataSource::create(const std::map<std::string, std::string>& /*dsInfo*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The create() method is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::DataSource::drop(const std::map<std::string, std::string>& /*dsInfo*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The drop() method is not supported by the WMS driver!"));
}

bool te::ws::ogc::wms::da::DataSource::exists(const std::map<std::string, std::string>& dsInfo)
{
  if(dsInfo.empty())
    return false;

  std::map<std::string, std::string>::const_iterator usrDataDir = dsInfo.find("USERDATADIR");
  if(usrDataDir == dsInfo.end())
    return false;

  std::map<std::string, std::string>::const_iterator uri = dsInfo.find("URI");
  if(uri == dsInfo.end())
    return false;

  std::map<std::string, std::string>::const_iterator version = dsInfo.find("VERSION");
  if(version == dsInfo.end())
    return false;

  try
  {
    verifyConnectionInfo();

    te::ws::ogc::WMSClient wms(usrDataDir->second, uri->second, version->second);

    wms.updateCapabilities();
  }
  catch(...)
  {
    return false;
  }

  return true;
}

std::vector<std::string> te::ws::ogc::wms::da::DataSource::getDataSourceNames(const std::map<std::string, std::string>& /*dsInfo*/)
{
  return std::vector<std::string>();
}

std::vector<te::core::EncodingType> te::ws::ogc::wms::da::DataSource::getEncodings(const std::map<std::string, std::string>& /*dsInfo*/)
{
  return std::vector<te::core::EncodingType>();
}

void te::ws::ogc::wms::da::DataSource::verifyConnectionInfo() const
{
  if(m_connectionInfo.empty())
    throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The connection information is empty!"));

  std::map<std::string, std::string>::const_iterator uri = m_connectionInfo.find("URI");
  if(uri == m_connectionInfo.end())
    throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The connection information is invalid. Missing URI parameter!"));

  std::map<std::string, std::string>::const_iterator version = m_connectionInfo.find("VERSION");
  if(version == m_connectionInfo.end())
    throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The connection information is invalid. Missing VERSION parameter!"));

  std::map<std::string, std::string>::const_iterator usrDataDir = m_connectionInfo.find("USERDATADIR");
  if(usrDataDir == m_connectionInfo.end())
    throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The connection information is invalid. Missing USERDATADIR parameter!"));

  std::string dataDir = usrDataDir->second + "/wms/";

}
