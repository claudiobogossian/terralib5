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
  \file terralib/dataaccess/datasource/DataSourceManager.cpp

  \brief This is a singleton for managing all data source instances available in the system.
*/

// TerraLib
#include "../../common/Globals.h"
#include "../../common/STLUtils.h"
#include "../../common/StringUtils.h"
#include "../../common/Translator.h"
#include "../Exception.h"
#include "DataSource.h"
#include "DataSourceFactory.h"
#include "DataSourceManager.h"

// STL
#include <algorithm>
#include <cassert>
#include <memory>

te::da::DataSourcePtr te::da::DataSourceManager::make(const std::string& id, const std::string& dsType)
{
  if(find(id))
    return find(id);

// we are optimistic: do the hard job and then see if another thread or the data source already had been inserted in the manager
  DataSourcePtr ds(DataSourceFactory::make(dsType));

  if(ds.get() == 0)
    throw Exception(TE_TR("Could not create the required data source instance!"));

  ds->setId(id);

  insert(ds);

  return ds;
}

te::da::DataSourcePtr te::da::DataSourceManager::open(const std::string& id, const std::string& dsType, const std::map<std::string, std::string>& connInfo)
{
// we are optimistic: do the hard job and then see if another thread or the data source already had been inserted in the manager
  DataSourcePtr ds(DataSourceFactory::make(dsType));

  if(ds.get() == 0)
    throw Exception(TE_TR("Could not create the required data source instance!"));

  ds->setConnectionInfo(connInfo);
  ds->open();

  ds->setId(id);

  insert(ds);

  return ds;
}

te::da::DataSourcePtr te::da::DataSourceManager::open(const std::string& id, const std::string& dsType, const std::string& connInfo)
{
  std::map<std::string, std::string> connInfoMap;

  te::common::ExtractKVP(connInfo, connInfoMap, "&", "=", false);

  return open(id, dsType, connInfoMap);
}

te::da::DataSourcePtr te::da::DataSourceManager::get(const std::string& id, const std::string& dsType, const std::map<std::string, std::string>& connInfo)
{
  LockRead l(this);

  const_iterator it = m_dss.find(id);

  if(it != m_dss.end())
    return it->second;

  DataSourcePtr newds(DataSourceFactory::make(dsType).release());

  newds->setConnectionInfo(connInfo);

  newds->open();

  newds->setId(id);

  insert(newds);

  return newds;
}

te::da::DataSourcePtr te::da::DataSourceManager::find(const std::string& id) const
{
  LockRead l(this);

  const_iterator it = m_dss.find(id);

  if(it != m_dss.end())
    return it->second;
  else
    return DataSourcePtr();
}

void te::da::DataSourceManager::insert(const DataSourcePtr& ds)
{
  if(ds.get() == 0)
    throw Exception(TE_TR("Please, specifify a non-null data source to be managed!"));

  LockWrite l(this);

  if(m_dss.find(ds->getId()) != m_dss.end())
    throw Exception(TE_TR("There is already a data source with the given identification!"));

  m_dss[ds->getId()] = ds;
}

void te::da::DataSourceManager::detach(const DataSourcePtr& ds)
{
  if(ds.get() == 0)
    return;

  detach(ds->getId());
}

te::da::DataSourcePtr te::da::DataSourceManager::detach(const std::string& id)
{
  LockWrite l(this);

  std::map<std::string, DataSourcePtr>::iterator it = m_dss.find(id);

  if(it == m_dss.end())
    throw Exception(TE_TR("Invalid data source to detach!"));

  DataSourcePtr ds = it->second;

  m_dss.erase(it);

  return ds;
}

void te::da::DataSourceManager::detachAll(const std::string& dsType)
{
  LockWrite l(this);

  std::map<std::string, DataSourcePtr>::iterator it = m_dss.begin();
  
  while(it != m_dss.end())
    if(it->second->getType() == dsType)
      m_dss.erase(it++);
    else
      ++it;
  //std::map<std::string, DataSourcePtr>::iterator it = m_dss.begin();
  //std::map<std::string, DataSourcePtr>::iterator itend = m_dss.end();

  //while(it != itend)
  //{
  //  const std::string& ttype = it->second->getType();

  //  if(it->second->getType() == dsType)
  //  {
  //    std::map<std::string, DataSourcePtr>::iterator itaux = it;
  //    ++it;
  //    m_dss.erase(itaux);
  //  }
  //  else
  //  {
  //    ++it;
  //  }
  //}
}

void te::da::DataSourceManager::detachAll()
{
  LockWrite l(this);

  m_dss.clear();
}

te::da::DataSourceManager::DataSourceManager()
{
}

te::da::DataSourceManager::~DataSourceManager()
{
}

