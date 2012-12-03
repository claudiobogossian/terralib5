/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/datasource/core/DataSourceManager.cpp

  \brief A singleton to keep all the registered data sources.
*/

// TerraLib
#include "../../../../common/STLUtils.h"
#include "../../../../common/Translator.h"
#include "../../Exception.h"
#include "DataSource.h"
#include "DataSourceManager.h"

// STL
#include <cassert>

// Boost
#include <boost/foreach.hpp>
#include <boost/format.hpp>

te::qt::widgets::DataSourcePtr te::qt::widgets::DataSourceManager::get(const std::string& id) const
{
  std::map<std::string, DataSourcePtr>::const_iterator it = m_datasources.find(id);

  return (it != m_datasources.end()) ? it->second : DataSourcePtr();
}

void te::qt::widgets::DataSourceManager::add(const DataSourcePtr& ds)
{
  if(ds.get() == 0)
    throw Exception(TR_QT_WIDGETS("Can not insert a NULL data source into the manager!"));

  if(m_datasources.find(ds->getId()) != m_datasources.end())
    throw Exception((boost::format(TR_QT_WIDGETS("There is already a data source with the given id (%1%) in data source manager!")) % ds->getId()).str());

  m_datasources[ds->getId()] = ds;
}

void te::qt::widgets::DataSourceManager::remove(const std::string& id)
{
  std::map<std::string, DataSourcePtr>::iterator it = m_datasources.find(id);

  if(it == m_datasources.end())
    throw Exception((boost::format(TR_QT_WIDGETS("There isn't a data source with the given id (%1%) in data source manager!")) % id).str());

  m_datasources.erase(it);
}

void te::qt::widgets::DataSourceManager::removeByType(const std::string& dsTypeName)
{
  std::map<std::string, DataSourcePtr>::iterator it = m_datasources.begin();
  std::map<std::string, DataSourcePtr>::iterator eraseIt;

  while(it!=m_datasources.end())
    if(it->second->getType() == dsTypeName)
    {
      eraseIt = it;
      ++it;
      m_datasources.erase(eraseIt);
    }
    else
      ++it;
}


void te::qt::widgets::DataSourceManager::getByType(const std::string& dsTypeName, std::vector<DataSourcePtr>& datasources) const
{
  std::map<std::string, DataSourcePtr>::const_iterator it = m_datasources.begin();
  std::map<std::string, DataSourcePtr>::const_iterator itend = m_datasources.end();

  while(it != itend)
  {
    if(it->second->getType() == dsTypeName)
      datasources.push_back(it->second);

    ++it;
  }
}

std::size_t te::qt::widgets::DataSourceManager::size() const
{
  return m_datasources.size();
}

te::qt::widgets::DataSourceManager::const_iterator te::qt::widgets::DataSourceManager::begin() const
{
  return m_datasources.begin();
}

te::qt::widgets::DataSourceManager::iterator te::qt::widgets::DataSourceManager::begin()
{
  return m_datasources.begin();
}

te::qt::widgets::DataSourceManager::const_iterator te::qt::widgets::DataSourceManager::end() const
{
  return m_datasources.end();
}

te::qt::widgets::DataSourceManager::iterator te::qt::widgets::DataSourceManager::end()
{
  return m_datasources.end();
}

te::qt::widgets::DataSourceManager::DataSourceManager()
{
}

te::qt::widgets::DataSourceManager::~DataSourceManager()
{
}

