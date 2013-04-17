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
  \file terralib/memory/DataSource.cpp

  \brief Implements the DataSource class for the TerraLib In-Memory driver.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "DataSet.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "Globals.h"

// Boost
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

te::da::DataSourceCapabilities te::mem::DataSource::sm_capabilities;
const te::da::SQLDialect te::mem::DataSource::sm_dialect;

te::mem::DataSource::DataSource()
  : m_catalog(0),
    m_ndatasets(0),
    m_maxdatasets(TE_MEMORY_MAX_DATASETS),
    m_isOpened(false),
    m_deepCopy(false)
{
  m_catalog = new te::da::DataSourceCatalog;
  m_catalog->setDataSource(this);
}

te::mem::DataSource::~DataSource()
{
  close();

  delete m_catalog;
}

const std::string& te::mem::DataSource::getType() const
{
  return Globals::sm_driverIdentifier;
}

const std::map<std::string, std::string>& te::mem::DataSource::getConnectionInfo() const
{
  return m_connInfo;
}

void te::mem::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_connInfo = connInfo;
}

const te::da::DataSourceCapabilities& te::mem::DataSource::getCapabilities() const
{
  return sm_capabilities;
}

const te::da::SQLDialect* te::mem::DataSource::getDialect() const
{
  return &sm_dialect;
}

void te::mem::DataSource::open()
{
// assure we are in a closed state
  close();

// check if it is required a different dataset limit
  std::map<std::string, std::string>::const_iterator it = m_connInfo.find("MAX_DATASETS");

  if(it != m_connInfo.end())
    m_maxdatasets = boost::lexical_cast<std::size_t>(it->second);

// check operation mode
  it = m_connInfo.find("OPERATION_MODE");

  if((it != m_connInfo.end()) && (te::common::Convert2UCase(it->second) == "NON-SHARED"))
    m_deepCopy = true;
 
  m_isOpened = true;
}

void te::mem::DataSource::close()
{
  if(!m_isOpened)
    return;

  m_catalog->clear();

  te::common::FreeContents(m_datasets);

  te::common::FreeContents(m_schemas);

  m_ndatasets = 0;

  m_maxdatasets = TE_MEMORY_MAX_DATASETS;

  m_isOpened = false;

  m_deepCopy = false;
}

bool te::mem::DataSource::isOpened() const
{
  return m_isOpened;
}

bool te::mem::DataSource::isValid() const
{
  return m_isOpened;
}

te::da::DataSourceCatalog* te::mem::DataSource::getCatalog() const
{
  return m_catalog;
}

te::da::DataSourceTransactor* te::mem::DataSource::getTransactor()
{
  return new DataSourceTransactor(this);
}

void te::mem::DataSource::optimize(const std::map<std::string, std::string>& /*opInfo*/)
{
// Not supported by In-Memory driver!
}

void te::mem::DataSource::add(te::da::DataSetType* dt, DataSet* dataset)
{
  LockWrite l(this);

  if(m_ndatasets == m_maxdatasets)
    throw Exception((boost::format(TR_MEMORY("Could not add dataset %1% because the maximum number of datasets has reached its maximum: %2%!")) % dt->getName() % m_maxdatasets).str());

  std::map<std::string, DataSet*>::const_iterator it = m_datasets.find(dt->getName());

  if(it != m_datasets.end())
    throw Exception((boost::format(TR_MEMORY("A dataset with the given name %1% already exists!")) % dt->getName()).str());

  m_datasets[dt->getName()] = dataset;
  m_schemas[dt->getName()] = dt;

  ++m_ndatasets;
}

te::mem::DataSet* te::mem::DataSource::getDataSet(const std::string& name)
{
  LockRead l(this);

  std::map<std::string, DataSet*>::const_iterator it = m_datasets.find(name);

  if(it == m_datasets.end())
    return 0;

  return new DataSet(*(it->second), m_deepCopy);
}

te::mem::DataSet* te::mem::DataSource::getDataSetRef(const std::string& name)
{
  LockRead l(this);

  std::map<std::string, DataSet*>::iterator it = m_datasets.find(name);

  if(it == m_datasets.end())
    return 0;

  return it->second;
}

te::da::DataSetType* te::mem::DataSource::getDataSetType(const std::string& name)
{
  LockRead l(this);

  std::map<std::string, te::da::DataSetType*>::const_iterator it = m_schemas.find(name);

  if(it == m_schemas.end())
    return 0;

  return static_cast<te::da::DataSetType*>(it->second->clone());
}

te::da::DataSetType* te::mem::DataSource::getDataSetTypeRef(const std::string& name)
{
  LockRead l(this);

  std::map<std::string, te::da::DataSetType*>::const_iterator it = m_schemas.find(name);

  if(it == m_schemas.end())
    return 0;

  return it->second;
}

void te::mem::DataSource::getDataSets(boost::ptr_vector<std::string>& datasets) const
{
  LockRead l(this);

  std::map<std::string, DataSet*>::const_iterator it = m_datasets.begin();
  std::map<std::string, DataSet*>::const_iterator itend = m_datasets.end();

  while(it != itend)
  {
    datasets.push_back(new std::string(it->first));
    ++it;
  }
}

bool te::mem::DataSource::datasetExists(const std::string& name) const
{
  LockRead l(this);

  std::map<std::string, DataSet*>::const_iterator it = m_datasets.find(name);

  return it != m_datasets.end();
}

void te::mem::DataSource::remove(const std::string& datasetName)
{
  LockWrite l(this);

  {
    std::map<std::string, DataSet*>::iterator it = m_datasets.find(datasetName);

    if(it != m_datasets.end())
    {
      delete it->second;
      m_datasets.erase(it);
    }
    else
    {
      throw Exception((boost::format(TR_MEMORY("There isn't a dataset with the given name %1%")) % datasetName).str());
    }
  }

  {
    std::map<std::string, te::da::DataSetType*>::iterator it = m_schemas.find(datasetName);

    if(it != m_schemas.end())
    {
      delete it->second;
      m_schemas.erase(it);
    }
    else
    {
      throw Exception((boost::format(TR_MEMORY("There isn't a dataset with the given name %1%")) % datasetName).str());
    }
  }
}

void te::mem::DataSource::rename(const std::string& oldName, std::string newName)
{
  LockWrite l(this);

  {
    std::map<std::string, DataSet*>::iterator it = m_datasets.find(oldName);

    if(it == m_datasets.end())
      throw Exception((boost::format(TR_MEMORY("There isn't a dataset with the given name %1%")) % oldName).str());

    it = m_datasets.find(newName);

    if(it == m_datasets.end())
      throw Exception((boost::format(TR_MEMORY("There is already a dataset with the new name %1%")) % newName).str());

    DataSet* dataset = it->second;

    m_datasets.erase(it);

    m_datasets[newName] = dataset;
  }

  {
    std::map<std::string, te::da::DataSetType*>::iterator it = m_schemas.find(oldName);

    if(it == m_schemas.end())
      throw Exception((boost::format(TR_MEMORY("There isn't a dataset with the given name %1%")) % oldName).str());

    it = m_schemas.find(newName);

    if(it == m_schemas.end())
      throw Exception((boost::format(TR_MEMORY("There is already a dataset with the new name %1%")) % newName).str());

    te::da::DataSetType* dataset = it->second;

    m_schemas.erase(it);

    m_schemas[newName] = dataset;
  }
}

bool te::mem::DataSource::hasDataSets()
{
  return m_ndatasets > 0;
}

void te::mem::DataSource::setCapabilities(const te::da::DataSourceCapabilities& capabilities)
{
  sm_capabilities = capabilities;
}

void te::mem::DataSource::create(const std::map<std::string, std::string>& dsInfo)
{
  m_connInfo = dsInfo;

  open();

  close();
}

void te::mem::DataSource::drop(const std::map<std::string, std::string>& /*dsInfo*/)
{
// Not supported by In-Memory driver!
}

bool te::mem::DataSource::exists(const std::map<std::string, std::string>& /*dsInfo*/)
{
// Not supported by In-Memory driver!
  return false;
}

std::vector<std::string> te::mem::DataSource::getDataSources(const std::map<std::string, std::string>& info)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

std::vector<std::string>te::mem::DataSource::getEncodings(const std::map<std::string, std::string>& info)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}