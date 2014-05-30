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
  \file terralib/qt/widgets/datasource/core/DataSourceTypeManager.cpp

  \brief A singleton to keep all the registered data source types.
*/

// TerraLib
#include "../../../../common/STLUtils.h"
#include "../../../../common/Translator.h"
#include "../../Exception.h"
#include "DataSourceType.h"
#include "DataSourceTypeManager.h"

// STL
#include <cassert>

// Boost
#include <boost/foreach.hpp>
#include <boost/format.hpp>

const te::qt::widgets::DataSourceType* te::qt::widgets::DataSourceTypeManager::get(const std::string& name) const
{
  return te::common::GetPValue(m_datasourceTypeMap, name);
}

void te::qt::widgets::DataSourceTypeManager::add(DataSourceType* dst)
{
  if(dst == 0)
    throw Exception(TE_TR("Can not insert a NULL data source type into the manager!"));

  if(te::common::GetPValue(m_datasourceTypeMap, dst->getName()) != 0)
    throw Exception((boost::format(TE_TR("There is already a data source type with the given id (%1%) in data source manager!")) % dst->getName()).str());

  m_datasourceTypeMap[dst->getName()] = dst;
}

void te::qt::widgets::DataSourceTypeManager::remove(const std::string& name)
{
  iterator it = m_datasourceTypeMap.find(name);

  if(it == m_datasourceTypeMap.end())
    return;

  delete it->second;

  m_datasourceTypeMap.erase(it);
}

std::size_t te::qt::widgets::DataSourceTypeManager::size() const
{
  return m_datasourceTypeMap.size();
}

te::qt::widgets::DataSourceTypeManager::const_iterator te::qt::widgets::DataSourceTypeManager::begin() const
{
  return m_datasourceTypeMap.begin();
}

te::qt::widgets::DataSourceTypeManager::iterator te::qt::widgets::DataSourceTypeManager::begin()
{
  return m_datasourceTypeMap.begin();
}

te::qt::widgets::DataSourceTypeManager::const_iterator te::qt::widgets::DataSourceTypeManager::end() const
{
  return m_datasourceTypeMap.end();
}

te::qt::widgets::DataSourceTypeManager::iterator te::qt::widgets::DataSourceTypeManager::end()
{
  return m_datasourceTypeMap.end();
}

te::qt::widgets::DataSourceTypeManager::DataSourceTypeManager()
{
}

te::qt::widgets::DataSourceTypeManager::~DataSourceTypeManager()
{
}

