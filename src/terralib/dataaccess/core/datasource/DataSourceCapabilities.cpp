/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/core/datasource/DataSourceCapabilities.cpp

  \brief A class that represents the known capabilities of a specific data source.
*/

// TerraLib
#include "DataSourceCapabilities2.h"

te::da::core::DataSourceCapabilities::DataSourceCapabilities()
  : m_accessPolicy(te::common::NoAccess),
    m_supportTransactions(false),
    m_supportDataSetPesistenceAPI(false),
    m_supportDataSetTypePesistenceAPI(false),
    m_supportPreparedQueryAPI(false),
    m_supportBatchExecutorAPI(false)
{
}

te::da::core::DataSourceCapabilities::~DataSourceCapabilities()
{
}

te::common::AccessPolicy te::da::core::DataSourceCapabilities::getAccessPolicy() const
{
  return m_accessPolicy;
}

void te::da::core::DataSourceCapabilities::setAccessPolicy(const te::common::AccessPolicy& accessPolicy)
{
  m_accessPolicy = accessPolicy;
}

bool te::da::core::DataSourceCapabilities::supportsTransactions() const
{
  return m_supportTransactions;
}

void te::da::core::DataSourceCapabilities::setSupportTransactions(const bool& support)
{
  m_supportTransactions = support;
}

bool te::da::core::DataSourceCapabilities::supportsDataSetPesistenceAPI() const
{
  return m_supportDataSetPesistenceAPI;
}

void te::da::core::DataSourceCapabilities::setSupportDataSetPesistenceAPI(const bool& support)
{
  m_supportDataSetPesistenceAPI = support;
}

bool te::da::core::DataSourceCapabilities::supportsDataSetTypePesistenceAPI() const
{
  return m_supportDataSetTypePesistenceAPI;
}

void te::da::core::DataSourceCapabilities::setSupportDataSetTypePesistenceAPI(const bool& support)
{
  m_supportDataSetTypePesistenceAPI = support;
}

bool te::da::core::DataSourceCapabilities::supportsPreparedQueryAPI() const
{
  return m_supportPreparedQueryAPI;
}

void te::da::core::DataSourceCapabilities::setSupportPreparedQueryAPI(const bool& support)
{
  m_supportPreparedQueryAPI = support;
}

bool te::da::core::DataSourceCapabilities::supportsBatchExecutorAPI() const
{
  return m_supportBatchExecutorAPI;
}

void te::da::core::DataSourceCapabilities::setSupportBatchExecutorAPI(const bool& support)
{
  m_supportBatchExecutorAPI = support;
}

const te::da::core::DataTypeCapabilities& te::da::core::DataSourceCapabilities::getDataTypeCapabilities() const
{
  return m_dataTypeCapabilities;
}

void te::da::core::DataSourceCapabilities::setDataTypeCapabilities(const te::da::core::DataTypeCapabilities& capabilities)
{
  m_dataTypeCapabilities = capabilities;
}

const te::da::core::DataSetTypeCapabilities& te::da::core::DataSourceCapabilities::getDataSetTypeCapabilities() const
{
  return m_dataSetTypeCapabilities;
}

void te::da::core::DataSourceCapabilities::setDataSetTypeCapabilities(const te::da::core::DataSetTypeCapabilities& capabilities)
{
  m_dataSetTypeCapabilities = capabilities;
}

const te::da::core::DataSetCapabilities& te::da::core::DataSourceCapabilities::getDataSetCapabilities() const
{
  return m_dataSetCapabilities;
}

void te::da::core::DataSourceCapabilities::setDataSetCapabilities(const te::da::core::DataSetCapabilities& capabilities)
{
  m_dataSetCapabilities = capabilities;
}

const te::da::core::QueryCapabilities& te::da::core::DataSourceCapabilities::getQueryCapabilities() const
{
  return m_queryCapabilities;
}

void te::da::core::DataSourceCapabilities::setQueryCapabilities(const te::da::core::QueryCapabilities& capabilities)
{
  m_queryCapabilities = capabilities;
}

const std::map<std::string, std::string>& te::da::core::DataSourceCapabilities::getSpecificCapabilities() const
{
  return m_specificCapabilities;
}

void te::da::core::DataSourceCapabilities::addSpecificCapability(const std::string& key, const std::string& value)
{
  m_specificCapabilities[key] = value;
}

void te::da::core::DataSourceCapabilities::setSupportAll()
{
  m_supportTransactions = true;
  m_supportDataSetPesistenceAPI = true;
  m_supportDataSetTypePesistenceAPI = true;
  m_supportPreparedQueryAPI = true;
  m_supportBatchExecutorAPI = true;
}

const std::vector<te::commom::CharacterEncodingType>& te::da::core::DataSourceCapabilities::getCharEncodings() const
{
  return m_charEncodings;
}

void te::da::core::DataSourceCapabilities::setCharEncodings(const std::vector<te::commom::CharacterEncodingType>& encodings)
{
  m_charEncodings = encodings;
}

