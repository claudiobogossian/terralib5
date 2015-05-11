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
  \file terralib/dataaccess/datasource/DataSourceCapabilities.cpp

  \brief A class that represents the known capabilities of a specific data source.
*/

// TerraLib
#include "DataSourceCapabilities.h"

te::da::DataSourceCapabilities::DataSourceCapabilities()
  : m_accessPolicy(te::common::NoAccess),
    m_supportTransactions(false),
    m_supportDataSetPesistenceAPI(false),
    m_supportDataSetTypePesistenceAPI(false),
    m_supportPreparedQueryAPI(false),
    m_supportBatchExecutorAPI(false)
{
}

te::da::DataSourceCapabilities::~DataSourceCapabilities()
{
}

te::common::AccessPolicy te::da::DataSourceCapabilities::getAccessPolicy() const
{
  return m_accessPolicy;
}

void te::da::DataSourceCapabilities::setAccessPolicy(const te::common::AccessPolicy& accessPolicy)
{
  m_accessPolicy = accessPolicy;
}

bool te::da::DataSourceCapabilities::supportsTransactions() const
{
  return m_supportTransactions;
}

void te::da::DataSourceCapabilities::setSupportTransactions(const bool& support)
{
  m_supportTransactions = support;
}

bool te::da::DataSourceCapabilities::supportsDataSetPesistenceAPI() const
{
  return m_supportDataSetPesistenceAPI;
}

void te::da::DataSourceCapabilities::setSupportDataSetPesistenceAPI(const bool& support)
{
  m_supportDataSetPesistenceAPI = support;
}

bool te::da::DataSourceCapabilities::supportsDataSetTypePesistenceAPI() const
{
  return m_supportDataSetTypePesistenceAPI;
}

void te::da::DataSourceCapabilities::setSupportDataSetTypePesistenceAPI(const bool& support)
{
  m_supportDataSetTypePesistenceAPI = support;
}

bool te::da::DataSourceCapabilities::supportsPreparedQueryAPI() const
{
  return m_supportPreparedQueryAPI;
}

void te::da::DataSourceCapabilities::setSupportPreparedQueryAPI(const bool& support)
{
  m_supportPreparedQueryAPI = support;
}

bool te::da::DataSourceCapabilities::supportsBatchExecutorAPI() const
{
  return m_supportBatchExecutorAPI;
}

void te::da::DataSourceCapabilities::setSupportBatchExecutorAPI(const bool& support)
{
  m_supportBatchExecutorAPI = support;
}

const te::da::DataTypeCapabilities& te::da::DataSourceCapabilities::getDataTypeCapabilities() const
{
  return m_dataTypeCapabilities;
}

void te::da::DataSourceCapabilities::setDataTypeCapabilities(const te::da::DataTypeCapabilities& capabilities)
{
  m_dataTypeCapabilities = capabilities;
}

const te::da::DataSetTypeCapabilities& te::da::DataSourceCapabilities::getDataSetTypeCapabilities() const
{
  return m_dataSetTypeCapabilities;
}

void te::da::DataSourceCapabilities::setDataSetTypeCapabilities(const te::da::DataSetTypeCapabilities& capabilities)
{
  m_dataSetTypeCapabilities = capabilities;
}

const te::da::DataSetCapabilities& te::da::DataSourceCapabilities::getDataSetCapabilities() const
{
  return m_dataSetCapabilities;
}

void te::da::DataSourceCapabilities::setDataSetCapabilities(const te::da::DataSetCapabilities& capabilities)
{
  m_dataSetCapabilities = capabilities;
}

const te::da::QueryCapabilities& te::da::DataSourceCapabilities::getQueryCapabilities() const
{
  return m_queryCapabilities;
}

void te::da::DataSourceCapabilities::setQueryCapabilities(const te::da::QueryCapabilities& capabilities)
{
  m_queryCapabilities = capabilities;
}

const std::map<std::string, std::string>& te::da::DataSourceCapabilities::getSpecificCapabilities() const
{
  return m_specificCapabilities;
}

void te::da::DataSourceCapabilities::addSpecificCapability(const std::string& key, const std::string& value)
{
  m_specificCapabilities[key] = value;
}

void te::da::DataSourceCapabilities::setSupportAll()
{
  m_supportTransactions = true;
  m_supportDataSetPesistenceAPI = true;
  m_supportDataSetTypePesistenceAPI = true;
  m_supportPreparedQueryAPI = true;
  m_supportBatchExecutorAPI = true;
}
