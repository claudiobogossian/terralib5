/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/datasource/core/DataSource.cpp

  \brief A conteiner class for keeping information about a data source.
*/

// TerraLib
#include "DataSource.h"

te::qt::widgets::DataSource::DataSource()
{
}

te::qt::widgets::DataSource::~DataSource()
{
}

const std::string& te::qt::widgets::DataSource::getId() const
{
  return m_id;
}

void te::qt::widgets::DataSource::setId(const std::string& id)
{
  m_id = id;
}

const std::string& te::qt::widgets::DataSource::getType() const
{
  return m_dsType;
}

void te::qt::widgets::DataSource::setType(const std::string& dsType)
{
  m_dsType = dsType;
}

const std::string& te::qt::widgets::DataSource::getAccessDriver() const
{
  return m_accessDriver;
}

void te::qt::widgets::DataSource::setAccessDriver(const std::string& d)
{
  m_accessDriver = d;
}

const std::string& te::qt::widgets::DataSource::getTitle() const
{
  return m_title;
}

void te::qt::widgets::DataSource::setTitle(const std::string& title)
{
  m_title = title;
}

const std::string& te::qt::widgets::DataSource::getDescription() const
{
  return m_description;
}

void te::qt::widgets::DataSource::setDescription(const std::string& d)
{
  m_description = d;
}

const std::map<std::string, std::string>& te::qt::widgets::DataSource::getConnInfo() const
{
  return m_conninfo;
}

std::map<std::string, std::string>& te::qt::widgets::DataSource::getConnInfo()
{
  return m_conninfo;
}

void te::qt::widgets::DataSource::setConnInfo(const std::map<std::string, std::string>& conninfo)
{
  m_conninfo = conninfo;
}

bool te::qt::widgets::DataSource::operator<(const DataSource& rhs) const
{
  return m_id < rhs.m_id;
}

