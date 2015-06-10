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
  \file terralib/wms/DataSource.cpp

  \brief Implementation of the data source for the WMS driver.
*/

// TerraLib
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "DataSource.h"
#include "Exception.h"
#include "Transactor.h"
#include "Utils.h"

// GDAL
#include <gdal_priv.h>

te::da::DataSourceCapabilities te::wms::DataSource::sm_capabilities;

te::wms::DataSource::DataSource()
  : m_isOpened(false)
{
}

te::wms::DataSource::~DataSource()
{}

std::string te::wms::DataSource::getType() const
{
  return TE_WMS_DRIVER_IDENTIFIER;
}

const std::map<std::string, std::string>& te::wms::DataSource::getConnectionInfo() const
{
  return m_connectionInfo;
}

void te::wms::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_connectionInfo = connInfo;
}

std::auto_ptr<te::da::DataSourceTransactor> te::wms::DataSource::getTransactor()
{
  if(!m_isOpened)
    throw Exception(TE_TR("The data source is not opened!"));

  return std::auto_ptr<te::da::DataSourceTransactor>(new Transactor(m_connectionInfo.find("URI")->second, m_layersInfo));
}

void te::wms::DataSource::open()
{
  if(m_isOpened)
    return;

  verifyConnectionInfo();

  GDALDataset* gds = static_cast<GDALDataset*>(GDALOpen(m_connectionInfo.find("URI")->second.c_str(), GA_ReadOnly));
  if(gds == 0)
    throw Exception(TE_TR("Error establishing connection with the informed server!"));

  // Gets the layer informations from server
  char** subdatasets = gds->GetMetadata("SUBDATASETS");

  // Builds the layer informations from informed GDAL subdatasets
  BuildLayersInfo(subdatasets, m_layersInfo);

  GDALClose(gds);

  m_isOpened = true;
}

void te::wms::DataSource::close()
{
  m_isOpened = false;
  m_layersInfo.clear();
}

bool te::wms::DataSource::isOpened() const
{
  return m_isOpened;
}

bool te::wms::DataSource::isValid() const
{
  if(m_connectionInfo.empty())
    return false;

  std::map<std::string, std::string>::const_iterator it = m_connectionInfo.find("URI");
  if(it == m_connectionInfo.end())
    return false;

  GDALDataset* gds = static_cast<GDALDataset*>(GDALOpen(it->second.c_str(), GA_ReadOnly));
  if(gds == 0)
    return false;

  GDALClose(gds);

  return true;
}

const te::da::DataSourceCapabilities& te::wms::DataSource::getCapabilities() const
{
  return sm_capabilities;
}

void te::wms::DataSource::setCapabilities(const te::da::DataSourceCapabilities& capabilities)
{
  sm_capabilities = capabilities;
}

const te::da::SQLDialect* te::wms::DataSource::getDialect() const
{
  return 0;
}

const std::map<std::string, te::wms::WMSLayerInfo>& te::wms::DataSource::getLayersInfo() const
{
  return m_layersInfo;
}

void te::wms::DataSource::create(const std::map<std::string, std::string>& /*dsInfo*/)
{
  throw Exception(TE_TR("The create() method is not supported by the WMS driver!"));
}

void te::wms::DataSource::drop(const std::map<std::string, std::string>& /*dsInfo*/)
{
  throw Exception(TE_TR("The drop() method is not supported by the WMS driver!"));
}

bool te::wms::DataSource::exists(const std::map<std::string, std::string>& dsInfo)
{
  if(dsInfo.empty())
    return false;

  std::map<std::string, std::string>::const_iterator it = dsInfo.find("URI");
  if(it == dsInfo.end())
    return false;

  GDALDataset* gds = static_cast<GDALDataset*>(GDALOpen(it->second.c_str(), GA_ReadOnly));
  if(gds == 0)
    return false;

  GDALClose(gds);

  return true;
}

std::vector<std::string> te::wms::DataSource::getDataSourceNames(const std::map<std::string, std::string>& /*dsInfo*/)
{
  return std::vector<std::string>();
}

std::vector<te::common::CharEncoding> te::wms::DataSource::getEncodings(const std::map<std::string, std::string>& /*dsInfo*/)
{
  return std::vector<te::common::CharEncoding>();
}

void te::wms::DataSource::verifyConnectionInfo() const
{
  if(m_connectionInfo.empty())
    throw Exception(TE_TR("The connection information is empty!"));

  std::map<std::string, std::string>::const_iterator it = m_connectionInfo.find("URI");
  if(it == m_connectionInfo.end())
    throw Exception(TE_TR("The connection information is invalid. Missing URI parameter!"));
}
