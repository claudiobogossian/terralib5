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
#include "DataSourceInfo.h"

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
  std::string conInfo;
  std::map<std::string, std::string>::iterator conIt = m_conninfo.begin();
  while (conIt != m_conninfo.end())
  {
    conInfo += conIt->second;
    ++conIt;
  }
  return conInfo;
}

void te::da::DataSourceInfo::setConnInfo(const std::map<std::string, std::string>& conninfo)
{
  m_conninfo = conninfo;
}

bool te::da::DataSourceInfo::operator<(const DataSourceInfo& rhs) const
{
  return m_id < rhs.m_id;
}

