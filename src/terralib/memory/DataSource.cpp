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

const te::da::SQLDialect te::mem::DataSource::sm_dialect;

te::mem::DataSource::DataSource()
  : m_catalog(0),
    m_ndatasets(0),
    m_maxdatasets(TE_MEMORY_MAX_DATASETS),
    m_isOpened(false),
    m_deepCopy(false)
{
  m_catalog.reset(new te::da::DataSourceCatalog);
  m_catalog->setDataSource(this);
}

te::mem::DataSource::~DataSource()
{
  close();
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

void te::mem::DataSource::getCapabilities(std::map<std::string, std::string>& capabilities) const
{
  capabilities["ACCESS_POLICY"] = "RW_ACCESS";
  capabilities["TRANSACTION"] = "FALSE";
  capabilities["DATASET_PERSISTENCE"] = "TRUE";
  capabilities["DATASETTYPE_PERSISTENCE"] = "TRUE";
  capabilities["PREPARED_QUERY"] = "FALSE";
  capabilities["BATCH_EXECUTOR"] = "FALSE";

  capabilities["BIT_DT"] = "TRUE";
  capabilities["CHAR_DT"] = "TRUE";
  capabilities["UCHAR_DT"] = "TRUE";
  capabilities["INT16_DT"] = "TRUE";
  capabilities["UINT16_DT"] = "TRUE";
  capabilities["INT32_DT"] = "TRUE";
  capabilities["UINT32_DT"] = "TRUE";
  capabilities["INT64_DT"] = "TRUE";
  capabilities["UINT64_DT"] = "TRUE";
  capabilities["BOOLEAN_DT"] = "TRUE";
  capabilities["FLOAT_DT"] = "TRUE";
  capabilities["DOUBLE_DT"] = "TRUE";
  capabilities["NUMERIC_DT"] = "TRUE";
  capabilities["STRING_DT"] = "TRUE";
  capabilities["BYTE_ARRAY_DT"] = "TRUE";
  capabilities["GEOMETRY_DT"] = "TRUE";
  capabilities["DATETIME_DT"] = "TRUE";
  capabilities["ARRAY_DT"] = "TRUE";
  capabilities["COMPOSITE_DT"] = "TRUE";
  capabilities["RASTER_DT"] = "TRUE";
  capabilities["DATASET_DT"] = "TRUE";
  capabilities["XML_DT"] = "TRUE";

  capabilities["PRIMARY_KEY"] = "TRUE";
  capabilities["UNIQUE_KEY"] = "TRUE";
  capabilities["FOREIGN_KEY"] = "FALSE";
  capabilities["SEQUENCE"] = "FALSE";
  capabilities["CHECK_CONSTRAINTS"] = "FALSE";
  capabilities["INDEX"] = "TRUE";
  capabilities["RTREE_INDEX"] = "TRUE";
  capabilities["BTREE_INDEX"] = "TRUE";
  capabilities["HASH_INDEX"] = "TRUE";
  capabilities["QUADTREE_INDEX"] = "FALSE";

  capabilities["BIDIRECTIONAL_DATASET"] = "TRUE";
  capabilities["RANDOM_DATASET"] = "TRUE";
  capabilities["INDEXED_DATASET"] = "TRUE";

  capabilities["EFFICIENT_MOVE_PREVIOUS"] = "TRUE";
  capabilities["EFFICIENT_MOVE_FIRST"] = "TRUE";
  capabilities["EFFICIENT_MOVE_BEFORE_FIRST"] = "TRUE";
  capabilities["EFFICIENT_MOVE_LAST"] = "TRUE";
  capabilities["EFFICIENT_MOVE_AFTER_LAST"] = "TRUE";
  capabilities["EFFICIENT_MOVE"] = "TRUE";
  capabilities["EFFICIENT_DATASET_SIZE"] = "TRUE";
  capabilities["DATASET_INSERTION"] = "TRUE";
  capabilities["DATASET_UPDATE"] = "TRUE";
  capabilities["DATASET_DELETION"] = "TRUE";

  capabilities["SQL_DIALECT"] = "FALSE";
  capabilities["INSERT_QUERY"] = "FALSE";
  capabilities["UPDATE_QUERY"] = "FALSE";
  capabilities["DELETE_QUERY"] = "FALSE";
  capabilities["CREATE_QUERY"] = "FALSE";
  capabilities["DROP_QUERY"] = "FALSE";
  capabilities["ALTER_QUERY"] = "FALSE";
  capabilities["SELECT_QUERY"] = "FALSE";
  capabilities["SELECT_INTO_QUERY"] = "FALSE";
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

  m_datasets.clear();

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
  return m_catalog.get();
}

te::da::DataSourceTransactor* te::mem::DataSource::getTransactor()
{
  return new DataSourceTransactor(this);
}

void te::mem::DataSource::optimize(const std::map<std::string, std::string>& /*opInfo*/)
{
// Not supported by In-Memory driver!
}

void te::mem::DataSource::add(const std::string& name, DataSet* dataset)
{
  LockWrite l(this);

  if(m_ndatasets == m_maxdatasets)
    throw Exception((boost::format(TR_MEMORY("Could not add dataset %1% because the maximum number of datasets has reached its maximum: %2%!")) % name % m_maxdatasets).str());

  boost::ptr_map<std::string, te::da::DataSet>::const_iterator it = m_datasets.find(name);

  if(it != m_datasets.end())
    throw Exception((boost::format(TR_MEMORY("A dataset with the given name %1% already exist!")) % name).str());

  std::string copyname = name;
  m_datasets.insert(copyname, dataset);

  ++m_ndatasets;
}

te::mem::DataSet* te::mem::DataSource::getDataSet(const std::string& name)
{
  LockRead l(this);

  boost::ptr_map<std::string, DataSet>::const_iterator it = m_datasets.find(name);

  if(it == m_datasets.end())
    return 0;

  return new DataSet(*(it->second), m_deepCopy);
}

te::mem::DataSet* te::mem::DataSource::getDataSetRef(const std::string& name)
{
  LockRead l(this);

  boost::ptr_map<std::string, DataSet>::iterator it = m_datasets.find(name);

  if(it == m_datasets.end())
    return 0;

  return it->second;
}

te::da::DataSetType* te::mem::DataSource::getDataSetType(const std::string& name)
{
  LockRead l(this);

  boost::ptr_map<std::string, DataSet>::const_iterator it = m_datasets.find(name);

  if(it == m_datasets.end())
    return 0;

  return static_cast<te::da::DataSetType*>(it->second->getType()->clone());
}

void te::mem::DataSource::getDataSets(boost::ptr_vector<std::string>& datasets) const
{
  LockRead l(this);

  boost::ptr_map<std::string, DataSet>::const_iterator it = m_datasets.begin();
  boost::ptr_map<std::string, DataSet>::const_iterator itend = m_datasets.end();

  while(it != itend)
  {
    datasets.push_back(new std::string(it->first));
    ++it;
  }
}

bool te::mem::DataSource::datasetExists(const std::string& name) const
{
  LockRead l(this);

  boost::ptr_map<std::string, DataSet>::const_iterator it = m_datasets.find(name);

  return it != m_datasets.end();
}

void te::mem::DataSource::remove(const std::string& datasetName)
{
  LockWrite l(this);

  boost::ptr_map<std::string, DataSet>::iterator it = m_datasets.find(datasetName);

  if(it != m_datasets.end())
    m_datasets.erase(it);
  else
    throw Exception((boost::format(TR_MEMORY("There isn't a dataset with the given name %1%")) % datasetName).str());
}

void te::mem::DataSource::rename(const std::string& oldName, std::string newName)
{
  LockWrite l(this);

  boost::ptr_map<std::string, DataSet>::iterator it = m_datasets.find(oldName);

  if(it == m_datasets.end())
    throw Exception((boost::format(TR_MEMORY("There isn't a dataset with the given name %1%")) % oldName).str());

  it = m_datasets.find(newName);

  if(it == m_datasets.end())
    throw Exception((boost::format(TR_MEMORY("There is already a dataset with the new name %1%")) % newName).str());

  boost::ptr_map<std::string, DataSet>::auto_type ptr = m_datasets.release(it);
  m_datasets.insert(newName, ptr.release());
}

bool te::mem::DataSource::hasDataSets()
{
  return m_ndatasets > 0;
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

