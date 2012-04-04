/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
#include <cassert>
#include <memory>

te::da::DataSource* te::da::DataSourceManager::make(const std::string& id, const std::string& dsType)
{
// we are optimistic: do the hard job and then see if another thread or the data source already had been inserted in the manager
  std::auto_ptr<DataSource> ds(DataSourceFactory::make(dsType));

  if(ds.get() == 0)
    throw Exception(TR_DATAACCESS("Could not create the required data source instance!"));

  insert(id, ds.get());

  return ds.release();
}

te::da::DataSource* te::da::DataSourceManager::open(const std::string& id, const std::string& dsType, const std::map<std::string, std::string>& connInfo)
{
// we are optimistic: do the hard job and then see if another thread or the data source already had been inserted in the manager
  std::auto_ptr<DataSource> ds(DataSourceFactory::make(dsType));

  if(ds.get() == 0)
    throw Exception(TR_DATAACCESS("Could not create the required data source instance!"));

  ds->open(connInfo);

  insert(id, ds.get());

  return ds.release();
}

te::da::DataSource* te::da::DataSourceManager::open(const std::string& id, const std::string& dsType, const std::string& connInfo)
{
  std::map<std::string, std::string> connInfoMap;

  te::common::ExtractKVP(connInfo, connInfoMap, "&", "=", false);

  return open(id, dsType, connInfoMap);
}

te::da::DataSource* te::da::DataSourceManager::get(const std::string& id, const std::string& dsType, const std::map<std::string, std::string>& connInfo)
{
  LockRead l(this);

  DataSource* ds = te::common::GetPValue(m_dss, id);

  if(ds != 0)
    return ds;

  ds = DataSourceFactory::open(dsType, connInfo);

  insert(id, ds);

  return ds;
}

te::da::DataSource* te::da::DataSourceManager::find(const std::string& id) const
{
  LockRead l(this);

  DataSource* ds = te::common::GetPValue(m_dss, id);

  return ds;
}

std::string te::da::DataSourceManager::find(te::da::DataSource* ds) const
{
  LockRead l(this);

  std::string id;

  std::map<DataSource*, std::string>::const_iterator it = m_invIndex.find(ds);

  if(it != m_invIndex.end())
    return it->second;

  return te::common::Globals::sm_dummyStr;
}

void te::da::DataSourceManager::insert(const std::string& id, DataSource* ds)
{
  if(ds == 0)
    throw Exception(TR_DATAACCESS("Please, specifify a non-null data source to be managed!"));

  LockWrite l(this);

  if(m_dss.find(id) != m_dss.end())
    throw Exception(TR_DATAACCESS("There is already a data source with the given identification!"));

  if(m_invIndex.find(ds) != m_invIndex.end())
    throw Exception(TR_DATAACCESS("The data source is already managed by data source manager!"));

  m_dss[id] = ds;
  m_invIndex[ds] = id;

  if(ds->isOpened())
    m_openedDss.push_back(ds);
}

void te::da::DataSourceManager::detach(DataSource* ds)
{
  if(ds == 0)
    return;

  LockWrite l(this);

  std::map<DataSource*, std::string>::iterator it1 = m_invIndex.find(ds);

  if(it1 != m_invIndex.end())
  {
    std::map<std::string, DataSource*>::iterator it2 = m_dss.find(it1->second);
    m_dss.erase(it2);
    m_invIndex.erase(it1);

    if(ds->isOpened())
      m_openedDss.remove(ds);
  }
}

te::da::DataSource* te::da::DataSourceManager::detach(const std::string& id)
{
  LockWrite l(this);

  std::map<std::string, DataSource*>::iterator it1 = m_dss.find(id);

  DataSource* ds = 0;

  if(it1 != m_dss.end())
  {
    ds = it1->second;
    std::map<DataSource*, std::string>::iterator it2 = m_invIndex.find(it1->second);
    m_invIndex.erase(it2);
    m_dss.erase(it1);

    if(ds->isOpened())
      m_openedDss.remove(ds);
  }

  return ds;
}

void te::da::DataSourceManager::detachAll()
{
  LockWrite l(this);

  m_dss.clear();
  m_invIndex.clear();
  m_openedDss.clear();
}

void te::da::DataSourceManager::remove(DataSource* ds)
{
  detach(ds);
  delete ds;
}

void te::da::DataSourceManager::remove(const std::string& id)
{
  DataSource* ds = detach(id);
  delete ds;
}

void te::da::DataSourceManager::removeAll(const std::string& dsType)
{
  LockWrite l(this);

  std::map<std::string, DataSource*>::iterator it = m_dss.begin();
  std::map<std::string, DataSource*>::iterator itend = m_dss.end();

  while(it != itend)
  {
    if(it->second->getType() == dsType)
    {
      DataSource* ds = it->second;
      ++it;
      remove(ds);
    }
    else
      ++it;
  }
}

void te::da::DataSourceManager::removeAll()
{
  LockWrite l(this);

  te::common::FreeContents(m_dss);
  m_dss.clear();
  m_invIndex.clear();
  m_openedDss.clear();
}

te::da::DataSourceManager::DataSourceManager()
{
}

te::da::DataSourceManager::~DataSourceManager()
{
  te::common::FreeContents(m_dss);
}

