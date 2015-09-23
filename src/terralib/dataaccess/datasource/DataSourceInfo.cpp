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
  \file terralib/dataaccess/datasource/DataSourceInfo.cpp

  \brief A conteiner class for keeping information about a data source.
*/

// TerraLib
#include "../../common/StringUtils.h"
#include "DataSourceInfo.h"

// STL
#include <vector>

te::da::DataSourceInfo::DataSourceInfo()
{
}

te::da::DataSourceInfo::~DataSourceInfo()
{
}

const std::string& te::da::DataSourceInfo::getId() const
{
  return m_id;
}

void te::da::DataSourceInfo::setId(const std::string& id)
{
  m_id = id;
}

const std::string& te::da::DataSourceInfo::getType() const
{
  return m_dsType;
}

void te::da::DataSourceInfo::setType(const std::string& dsType)
{
  m_dsType = dsType;
}

const std::string& te::da::DataSourceInfo::getAccessDriver() const
{
  return m_accessDriver;
}

void te::da::DataSourceInfo::setAccessDriver(const std::string& d)
{
  m_accessDriver = d;
}

const std::string& te::da::DataSourceInfo::getTitle() const
{
  return m_title;
}

void te::da::DataSourceInfo::setTitle(const std::string& title)
{
  m_title = title;
}

const std::string& te::da::DataSourceInfo::getDescription() const
{
  return m_description;
}

void te::da::DataSourceInfo::setDescription(const std::string& d)
{
  m_description = d;
}

const std::map<std::string, std::string>& te::da::DataSourceInfo::getConnInfo() const
{
  return m_conninfo;
}

std::map<std::string, std::string>& te::da::DataSourceInfo::getConnInfo()
{
  return m_conninfo;
}

std::string te::da::DataSourceInfo::getConnInfoAsString()
{
  std::string connInfo;
  std::map<std::string, std::string>::iterator connIt = m_conninfo.begin();
  while (connIt != m_conninfo.end())
  {
    connInfo += connIt->first;
    connInfo += "=";
    connInfo += connIt->second;
    connInfo += ";";
    ++connIt;
  }
  return connInfo;
}

void te::da::DataSourceInfo::setConnInfo(const std::map<std::string, std::string>& conninfo)
{
  m_conninfo = conninfo;
}

void te::da::DataSourceInfo::setConnInfoFromString(const std::string& connInfo)
{
  m_conninfo.clear();

  std::vector<std::string> vec;

  te::common::Tokenize(connInfo, vec, ";");

  for (std::size_t t = 0; t < vec.size(); ++t)
  {
    std::string pairStr = vec[t];

    std::vector<std::string> pairVec;

    te::common::Tokenize(pairStr, pairVec, "=");

    if (pairVec.size() == 2)
    {
      m_conninfo[pairVec[0]] = pairVec[1];
    }
  }
}

bool te::da::DataSourceInfo::operator<(const DataSourceInfo& rhs) const
{
  return m_id < rhs.m_id;
}

